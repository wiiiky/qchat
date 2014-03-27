#include "wqqm4.h"
#include "wqquilogin.h"
#include "wqqutils.h"
#include "wqquicaptcha.h"
#define WQQ_ACCOUNTS_GROUP	"Accounts"

enum {
	WQQ_UI_LOGIN_PROPERTY_USERNAME = 1,
	WQQ_UI_LOGIN_PROPERTY_PASSWORD,
	WQQ_UI_LOGIN_PROPERTY_STATUS,
};

G_DEFINE_TYPE(WqqUiLogin, wqq_ui_login, GTK_TYPE_WINDOW);

static void wqq_ui_login_getter(GObject * object, guint property_id,
								GValue * value, GParamSpec * pspec);
static void wqq_ui_login_setter(GObject * object, guint property_id,
								const GValue * value, GParamSpec * pspec);


static gboolean on_entry_block_menu(GtkWidget * entry,
									GdkEventButton * button,
									gpointer data);
static gchar *on_username_combo_format(GtkComboBox * combo,
									   const gchar * path, gpointer data);
static void on_username_combo_changed(GtkComboBox * combo, gpointer data);
static void on_keep_username_toggled(GtkToggleButton * button,
									 gpointer data);
static void on_keep_pwd_toggled(GtkToggleButton * button, gpointer data);
static void on_entry_activate(GtkEntry * entry, gpointer data);

static void wqq_ui_login_set_focus_chain(WqqUiLogin * login);
static void wqq_ui_login_create_logining(WqqUiLogin * login);
static GtkWidget *wqq_ui_login_create_status_combo(void);
static GKeyFile *wqq_ui_login_create_keyfile(WqqUiLogin * login);
static void wqq_ui_login_set_entry_icon(WqqUiLogin * login);
static void wqq_ui_login_set_entry_icon_from_pixbuf(WqqUiLogin * login,
													GdkPixbuf * pixbuf);

static void wqq_ui_login_switch_to_login(WqqUiLogin * login);
static void wqq_ui_login_switch_to_logining(WqqUiLogin * login);

static void on_login_button_pressed(GtkWidget * widget, gpointer data);
static void on_cancel_button_pressed(GtkWidget * widget, gpointer data);

static void on_wqq_session_check_vc_callback(SoupSession * session,
											 SoupMessage * msg,
											 gpointer data);
static void on_wqq_session_get_captcha_image_callback(SoupSession *
													  session,
													  SoupMessage * msg,
													  gpointer data);
static void on_wqq_session_login_callback(SoupSession * session,
										  SoupMessage * msg,
										  gpointer data);
static void on_wqq_session_set_status_callback(SoupSession * session,
											   SoupMessage * msg,
											   gpointer data);
static void on_login_destroy(GtkWidget * widget, gpointer data)
{
	wqq_ui_login_save_accounts(WQQ_UI_LOGIN(widget));
	gtk_main_quit();
}

static void wqq_ui_login_init(WqqUiLogin * login)
{
	login->session = wqq_session_new("", "");
	login->captcha_file = g_strdup_printf("%s/wchat_captcha_image",
										  g_get_user_cache_dir());
	login->callback = NULL;
	login->user_data = NULL;
	login->data_dir = g_strdup_printf("%s/wchat", g_get_user_data_dir());

	gtk_window_set_title(GTK_WINDOW(login), _("wchat"));
	gtk_window_set_position(GTK_WINDOW(login), GTK_WIN_POS_CENTER);
	gtk_window_set_resizable(GTK_WINDOW(login), FALSE);
	gtk_window_set_icon_from_file(GTK_WINDOW(login),
								  WQQ_DATA_WCHAT_ICON_FILE, NULL);
	gtk_widget_set_size_request(GTK_WIDGET(login), 360, 300);
	gtk_container_set_border_width(GTK_CONTAINER(login), 30);
	g_signal_connect(G_OBJECT(login), "destroy",
					 G_CALLBACK(on_login_destroy), NULL);

	GtkWidget *grid = gtk_grid_new();
	login->grid = grid;
	gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
	gtk_container_add(GTK_CONTAINER(login), grid);

	/* 设置Label */
	GtkWidget *label = gtk_label_new(_("login"));
	gtk_widget_set_halign(label, GTK_ALIGN_START);
	gtk_widget_set_name(label, "login_label");
	gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

	label = gtk_label_new(_("<a href=\""
							"http://zc.qq.com/chs/index.html\">"
							"don't have a QQ account ?</a>"));
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_widget_set_name(label, "link_label");
	gtk_grid_attach(GTK_GRID(grid), label, 1, 0, 1, 1);

	/* horizontal separator */
	GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_grid_attach(GTK_GRID(grid), separator, 0, 1, 3, 1);

	/* QQ Account */
	label = gtk_label_new(_("<span size=\"medium\""
							" font=\"ubuntu\">QQ account"
							" : </span><span foreground="
							"\"red\">*</span>"));
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_widget_set_halign(label, GTK_ALIGN_START);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

	/* 帐号输入 */
	GtkWidget *username_combo = gtk_combo_box_new_with_entry();
	g_signal_connect(G_OBJECT(username_combo), "format-entry-text",
					 G_CALLBACK(on_username_combo_format), login);
	g_signal_connect(G_OBJECT(username_combo), "changed",
					 G_CALLBACK(on_username_combo_changed), login);
	login->username_combo = username_combo;
	GtkCellRenderer *renderer = gtk_cell_renderer_pixbuf_new();
	gtk_cell_layout_pack_end(GTK_CELL_LAYOUT(username_combo), renderer,
							 FALSE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(username_combo),
								   renderer, "pixbuf", 0, NULL);
	gtk_combo_box_set_entry_text_column(GTK_COMBO_BOX(username_combo), 1);
	gtk_combo_box_set_id_column(GTK_COMBO_BOX(username_combo), 2);
	gtk_widget_set_name(username_combo, "username_combo");
	gtk_grid_attach(GTK_GRID(grid), username_combo, 0, 3, 2, 1);
	GtkWidget *entry = gtk_bin_get_child(GTK_BIN(username_combo));
	login->username = entry;
	gtk_entry_set_icon_sensitive(GTK_ENTRY(entry),
								 GTK_ENTRY_ICON_SECONDARY, TRUE);
	gtk_entry_set_placeholder_text(GTK_ENTRY(entry),
								   _("QQ or Email Account"));
	gtk_entry_set_icon_from_path(GTK_ENTRY(entry),
								 GTK_ENTRY_ICON_SECONDARY,
								 WQQ_DATA_FACE_DEFAULT_FILE);
	g_signal_connect(G_OBJECT(entry), "activate",
					 G_CALLBACK(on_entry_activate), login);
	/* 屏蔽右键菜单 */
	g_signal_connect(G_OBJECT(entry), "button-press-event",
					 G_CALLBACK(on_entry_block_menu), NULL);

	/* 密码输入 */
	label = gtk_label_new(_("<span size=\"medium\" "
							"font=\"ubuntu\">Password"
							" : </span><span foreground"
							"=\"red\">*</span>"));
	gtk_widget_set_halign(label, GTK_ALIGN_START);
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	gtk_grid_attach(GTK_GRID(grid), label, 0, 4, 1, 1);
	GtkWidget *password = gtk_entry_new();
	login->password = password;
	g_signal_connect(G_OBJECT(password), "activate",
					 G_CALLBACK(on_entry_activate), login);
	g_signal_connect(G_OBJECT(password), "button-press-event",
					 G_CALLBACK(on_entry_block_menu), NULL);
	gtk_entry_set_visibility(GTK_ENTRY(password), FALSE);
	gtk_entry_set_input_purpose(GTK_ENTRY(password),
								GTK_INPUT_PURPOSE_PASSWORD);
	gtk_grid_attach(GTK_GRID(grid), password, 0, 5, 2, 1);

	/* 记住帐号、密码 */
	GtkWidget *remember_username =
		gtk_check_button_new_with_label(_("keep account"));
	g_signal_connect(G_OBJECT(remember_username), "toggled",
					 G_CALLBACK(on_keep_username_toggled), login);
	GtkWidget *remember_password =
		gtk_check_button_new_with_label(_("keep pwd"));
	g_signal_connect(G_OBJECT(remember_password), "toggled",
					 G_CALLBACK(on_keep_pwd_toggled), login);
	login->keep_username = remember_username;
	login->keep_pwd = remember_password;
	/* 选择登录状态 */
	GtkWidget *status_combo = wqq_ui_login_create_status_combo();
	login->status_combo = status_combo;

	GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
	login->hbox = hbox;
	gtk_grid_attach(GTK_GRID(grid), hbox, 0, 6, 2, 1);
	gtk_box_pack_start(GTK_BOX(hbox), remember_username, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), remember_password, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(hbox), status_combo, FALSE, FALSE, 0);

	/* 登录按钮 */
	GtkWidget *button = gtk_button_new_with_label(_("Login"));
	login->login_btn = button;
	g_signal_connect(G_OBJECT(button), "clicked",
					 G_CALLBACK(on_login_button_pressed), login);
	gtk_widget_set_name(button, "login_button");
	gtk_grid_attach(GTK_GRID(grid), button, 1, 7, 1, 1);

	gtk_widget_apply_css_from_path(grid, WQQ_DATA_LOGIN_CSS_FILE);

	gtk_widget_show_all(grid);
	/* 保存密码 */
	login->secret = secret_schema_new(WQQ_SECRET_SCHEMA_NAME,
									  SECRET_SCHEMA_NONE,
									  WQQ_SECRET_SCHEMA_ATTRIBUTE_NAME,
									  SECRET_SCHEMA_ATTRIBUTE_STRING,
									  NULL);
	/* 保存用户名 */
	login->accounts = wqq_ui_login_create_keyfile(login);
	/* tab order */
	wqq_ui_login_set_focus_chain(login);
	/* 登录界面 */
	wqq_ui_login_create_logining(login);
	/* 回车快捷键 */
}

static void wqq_ui_login_finalize(GObject * obj)
{
	WqqUiLogin *login = WQQ_UI_LOGIN(obj);
	secret_schema_unref(login->secret);

	gsize length;

	gchar *contents = g_key_file_to_data(login->accounts, &length, NULL);
	if (contents != NULL) {
		GFile *file = g_file_new_for_path(login->accounts_file);
		g_file_replace_contents(file, contents, length, NULL, FALSE,
								G_FILE_CREATE_NONE, NULL, NULL, NULL);
		g_object_unref(file);
		g_free(contents);
	}
	g_free(login->accounts_file);
	g_free(login->captcha_file);
	g_free(login->data_dir);
	g_key_file_unref(login->accounts);
}

static void wqq_ui_login_class_init(WqqUiLoginClass * klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = wqq_ui_login_setter;
	object_class->get_property = wqq_ui_login_getter;
	object_class->finalize = wqq_ui_login_finalize;

	GParamSpec *ps;
	ps = g_param_spec_string("username",
							 "login username",
							 "Login Username",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_UI_LOGIN_PROPERTY_USERNAME, ps);

	ps = g_param_spec_string("password",
							 "login password",
							 "Login Password",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_UI_LOGIN_PROPERTY_PASSWORD, ps);

	ps = g_param_spec_enum("status",
						   "login status",
						   "Login Status",
						   WQQ_TYPE_USER_STATUS,
						   WQQ_USER_STATUS_ONLINE,
						   G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_UI_LOGIN_PROPERTY_STATUS, ps);
}

static void wqq_ui_login_getter(GObject * object, guint property_id,
								GValue * value, GParamSpec * pspec)
{
	WqqUiLogin *login = WQQ_UI_LOGIN(object);
	switch (property_id) {
	case WQQ_UI_LOGIN_PROPERTY_USERNAME:
		g_value_set_string(value,
						   gtk_entry_get_text(GTK_ENTRY(login->username)));
		break;
	case WQQ_UI_LOGIN_PROPERTY_PASSWORD:
		g_value_set_string(value,
						   gtk_entry_get_text(GTK_ENTRY(login->password)));
		break;
	case WQQ_UI_LOGIN_PROPERTY_STATUS:
		g_value_set_enum(value,
						 _wqq_user_status_to_enum
						 (gtk_combo_box_get_active_id
						  (GTK_COMBO_BOX(login->status_combo))));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
	}
}

static void wqq_ui_login_setter(GObject * object, guint property_id,
								const GValue * value, GParamSpec * pspec)
{
	WqqUiLogin *login = WQQ_UI_LOGIN(object);
	switch (property_id) {
	case WQQ_UI_LOGIN_PROPERTY_USERNAME:
		gtk_entry_set_text(GTK_ENTRY(login->username),
						   g_value_get_string(value));
		break;
	case WQQ_UI_LOGIN_PROPERTY_PASSWORD:
		gtk_entry_set_text(GTK_ENTRY(login->password),
						   g_value_get_string(value));
		break;
	case WQQ_UI_LOGIN_PROPERTY_STATUS:
		gtk_combo_box_set_active_id(GTK_COMBO_BOX(login->status_combo),
									_wqq_user_status_to_string
									(g_value_get_enum(value)));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
	}
}

static void on_entry_activate(GtkEntry * entry, gpointer data)
{
	WqqUiLogin *login = (WqqUiLogin *) data;
	g_signal_emit_by_name(login->login_btn, "clicked", login);
}

static gboolean on_entry_block_menu(GtkWidget * entry,
									GdkEventButton * event, gpointer data)
{
	if (event->button == GDK_BUTTON_SECONDARY &&
		event->type == GDK_BUTTON_PRESS)
		return TRUE;
	return FALSE;
}

static void on_keep_username_toggled(GtkToggleButton * button,
									 gpointer data)
{
	WqqUiLogin *login = (WqqUiLogin *) data;
	if (!gtk_toggle_button_get_active(button))
		wqq_ui_login_set_keep_pwd(login, FALSE);
}

static void on_keep_pwd_toggled(GtkToggleButton * button, gpointer data)
{
	WqqUiLogin *login = (WqqUiLogin *) data;
	if (gtk_toggle_button_get_active(button))
		wqq_ui_login_set_keep_username(login, TRUE);
}

static gchar *on_username_combo_format(GtkComboBox * combo,
									   const gchar * path, gpointer data)
{
	WqqUiLogin *login = (WqqUiLogin *) data;
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *username, *pwd;
	GdkPixbuf *pixbuf;

	model = gtk_combo_box_get_model(combo);
	gtk_tree_model_get_iter_from_string(model, &iter, path);
	gtk_tree_model_get(model, &iter, 0, &pixbuf, 1, &username, 2, &pwd,
					   -1);
	wqq_ui_login_set_entry_icon_from_pixbuf(login, pixbuf);
	if (pwd) {
		wqq_ui_login_set_password(login, pwd);
	} else
		wqq_ui_login_set_password(login, "");
	wqq_ui_login_set_keep_pwd(login, pwd != NULL);
	wqq_ui_login_set_keep_username(login, TRUE);
	return username;
}

static void on_username_combo_changed(GtkComboBox * combo, gpointer data)
{
	/*g_message("!!!!!!!!"); */
	WqqUiLogin *login = (WqqUiLogin *) data;
	const gchar *username = wqq_ui_login_get_username(login);
	/*if (gtk_combo_box_get_active_id(GTK_COMBO_BOX(login->username_combo))) */
	/*return; */
	if (gtk_combo_box_get_active(GTK_COMBO_BOX(login->username_combo)) ==
		-1)
		return;
	wqq_ui_login_set_password(login, "");
}

static void wqq_ui_login_create_logining(WqqUiLogin * login)
{
	login->vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	GdkPixbufAnimation *animation =
		gdk_pixbuf_animation_new_from_file(WQQ_DATA_LOGINING_GIF_FILE,
										   NULL);
	GtkWidget *gif = gtk_image_new_from_animation(animation);
	g_object_unref(animation);
	gtk_box_pack_start(GTK_BOX(login->vbox), gif, TRUE, TRUE, 0);

	GtkWidget *cancel =
		(GtkWidget *) gtk_tool_button_new(NULL, _("Cancel"));
	login->cancel_btn = cancel;
	login->cancel = FALSE;
	gtk_widget_set_size_request(cancel, 100, 40);
	gtk_widget_set_halign(cancel, GTK_ALIGN_CENTER);
	g_signal_connect(G_OBJECT(cancel), "clicked",
					 G_CALLBACK(on_cancel_button_pressed), login);
	gtk_box_pack_start(GTK_BOX(login->vbox), cancel, FALSE, FALSE, 0);

	gtk_widget_apply_css_from_path(login->vbox,
								   WQQ_DATA_LOGINING_CSS_FILE);
	gtk_widget_show_all(login->vbox);
}

static GtkWidget *wqq_ui_login_create_status_combo(void)
{
	GtkWidget *combo = gtk_combo_box_new();
	gtk_combo_box_set_id_column(GTK_COMBO_BOX(combo), 1);
	gtk_widget_set_name(combo, "status_combo");
	GtkCellRenderer *renderer = gtk_cell_renderer_pixbuf_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combo), renderer, FALSE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(combo),
								   renderer, "pixbuf", 0, NULL);

	GtkListStore *store = gtk_list_store_new(2, GDK_TYPE_PIXBUF,
											 G_TYPE_STRING);
	GtkTreeIter iter;
	GdkPixbuf *pixbuf;
	pixbuf = gdk_pixbuf_new_from_file(WQQ_DATA_STATUS_ONLINE_FILE, NULL);
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, pixbuf, 1, "online", -1);
	g_object_unref(pixbuf);
	pixbuf = gdk_pixbuf_new_from_file(WQQ_DATA_STATUS_HIDDEN_FILE, NULL);
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, pixbuf, 1, "hidden", -1);
	g_object_unref(pixbuf);
	pixbuf = gdk_pixbuf_new_from_file(WQQ_DATA_STATUS_BUSY_FILE, NULL);
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, pixbuf, 1, "busy", -1);
	g_object_unref(pixbuf);
	pixbuf = gdk_pixbuf_new_from_file(WQQ_DATA_STATUS_AWAY_FILE, NULL);
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, pixbuf, 1, "away", -1);
	g_object_unref(pixbuf);
	pixbuf = gdk_pixbuf_new_from_file(WQQ_DATA_STATUS_CALLME_FILE, NULL);
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, pixbuf, 1, "callme", -1);
	g_object_unref(pixbuf);
	pixbuf = gdk_pixbuf_new_from_file(WQQ_DATA_STATUS_SILENT_FILE, NULL);
	gtk_list_store_append(store, &iter);
	gtk_list_store_set(store, &iter, 0, pixbuf, 1, "silent", -1);
	g_object_unref(pixbuf);

	gtk_combo_box_set_model(GTK_COMBO_BOX(combo), GTK_TREE_MODEL(store));
	gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);

	return combo;
}

static GKeyFile *wqq_ui_login_create_keyfile(WqqUiLogin * login)
{
	GKeyFile *keyfile = g_key_file_new();
	gchar *config_dir = g_strdup_printf("%s/%s", g_get_user_config_dir(),
										WQQ_WCHAT);
	g_mkdir_with_parents(config_dir, 0777);
	g_free(config_dir);
	gchar *filepath = g_strdup_printf("%s/%s/%s",
									  g_get_user_config_dir(),
									  WQQ_WCHAT,
									  WQQ_DATA_ACCOUNTS_FILE_NAME);
	GFile *file = g_file_new_for_path(filepath);
	g_file_create(file, G_FILE_CREATE_NONE, NULL, NULL);
	g_object_unref(file);
	login->accounts_file = filepath;
	g_key_file_load_from_file(keyfile, filepath, G_KEY_FILE_NONE, NULL);
	return keyfile;
}

static void wqq_ui_login_set_entry_icon(WqqUiLogin * login)
{
	GtkWidget *entry = gtk_bin_get_child(GTK_BIN(login->username_combo));
	gchar *face = g_strdup_printf("%s/%s/avatar",
								  login->data_dir,
								  gtk_entry_get_text(GTK_ENTRY(entry)));
	gtk_entry_set_icon_from_path(GTK_ENTRY(entry),
								 GTK_ENTRY_ICON_SECONDARY, face);
	g_free(face);
}

static void wqq_ui_login_set_entry_icon_from_pixbuf(WqqUiLogin * login,
													GdkPixbuf * pixbuf)
{
	GtkWidget *entry = gtk_bin_get_child(GTK_BIN(login->username_combo));
	gtk_entry_set_icon_from_pixbuf(GTK_ENTRY(entry),
								   GTK_ENTRY_ICON_SECONDARY, pixbuf);
}

static void wqq_ui_login_set_focus_chain(WqqUiLogin * login)
{
	GList *chain = NULL;
	chain = g_list_append(chain, login->username_combo);
	chain = g_list_append(chain, login->password);
	chain = g_list_append(chain, login->hbox);
	chain = g_list_append(chain, login->login_btn);

	gtk_container_set_focus_chain(GTK_CONTAINER(login->grid), chain);
	g_list_free(chain);

	chain = NULL;
	chain = g_list_append(chain, login->username);
	gtk_container_set_focus_chain(GTK_CONTAINER(login->username_combo),
								  chain);
	g_list_free(chain);
}

static void on_login_button_pressed(GtkWidget * widget, gpointer data)
{
	WqqUiLogin *login = (WqqUiLogin *) data;
	const gchar *username = gtk_entry_get_text(GTK_ENTRY(login->username));
	const gchar *password = gtk_entry_get_text(GTK_ENTRY(login->password));
	if (username == NULL || password == NULL ||
		username[0] == '\0' || password[0] == '\0') {
		g_log(G_LOG_DOMAIN, G_LOG_LEVEL_WARNING,
			  _("Invalid username or password!\n"));
		return;
	}
	wqq_session_set_username(login->session, username);
	wqq_session_set_plain(login->session, password);
	wqq_ui_login_switch_to_logining(login);
	wqq_session_check_verify_code_async(login->session,
										on_wqq_session_check_vc_callback,
										login);
}

static void on_wqq_session_check_vc_callback(SoupSession * session,
											 SoupMessage * msg,
											 gpointer data)
{
	WqqUiLogin *login = (WqqUiLogin *) data;
	guint status;
	g_object_get(msg, "status-code", &status, NULL);
	if (!SOUP_STATUS_IS_SUCCESSFUL(status)) {	/* 失败 */
		if (status != SOUP_STATUS_CANCELLED)
			wqq_message_dialog_run(GTK_WINDOW(login),
								   GTK_DIALOG_MODAL,
								   GTK_MESSAGE_ERROR,
								   GTK_BUTTONS_OK,
								   _("Fail to get verify code!\n"
									 "Check your network connection!"));
		wqq_ui_login_switch_to_login(login);
		return;
	}
	if (WQQ_SESSION_NEED_VERIFICATION(login->session)) {
		wqq_session_get_captcha_image_async(login->session,
											login->captcha_file,
											on_wqq_session_get_captcha_image_callback,
											login);
	} else {
		wqq_session_login_async(login->session,
								on_wqq_session_login_callback, login);
	}
}

static void on_wqq_session_get_captcha_image_callback(SoupSession *
													  session,
													  SoupMessage * msg,
													  gpointer data)
{
	WqqUiLogin *login = (WqqUiLogin *) data;
	guint status;
	g_object_get(msg, "status-code", &status, NULL);
	if (!SOUP_STATUS_IS_SUCCESSFUL(status)) {
		if (status != SOUP_STATUS_CANCELLED)
			wqq_message_dialog_run(GTK_WINDOW(login),
								   GTK_DIALOG_MODAL,
								   GTK_MESSAGE_ERROR,
								   GTK_BUTTONS_OK,
								   _("Fail to get captcha image!\n"
									 "Check network connection!"));
		wqq_ui_login_switch_to_login(login);
	} else {
		WqqUiCaptcha *captcha = wqq_ui_captcha_new(login->session,
												   login->captcha_file);
		gchar *code = wqq_ui_captcha_run(captcha);
		if (code == NULL)		/*  cancelled */
			wqq_ui_login_switch_to_login(login);
		else {
			wqq_session_set_captcha_code(login->session, code);
			wqq_session_login_async(login->session,
									on_wqq_session_login_callback, login);
			g_free(code);
		}
		gtk_widget_destroy(GTK_WIDGET(captcha));
	}
}

static void on_wqq_session_login_callback(SoupSession * session,
										  SoupMessage * msg, gpointer data)
{
	WqqUiLogin *login = (WqqUiLogin *) data;
	guint status;
	g_object_get(msg, "status-code", &status, NULL);
	if (SOUP_STATUS_IS_SUCCESSFUL(status)) {
		wqq_session_set_status_async(login->session,
									 wqq_ui_login_get_status(login),
									 on_wqq_session_set_status_callback,
									 login);
		return;
	} else if (status != SOUP_STATUS_CANCELLED) {
		if (status == WQQ_SESSION_LOGIN_STATUS_PASSWORD_ERROR) {
			wqq_message_dialog_run(GTK_WINDOW(login),
								   GTK_DIALOG_MODAL,
								   GTK_MESSAGE_ERROR,
								   GTK_BUTTONS_OK, _("Password error!"));
		} else if (status == WQQ_SESSION_LOGIN_STATUS_VERIFY_CODE_ERRROR) {
			wqq_message_dialog_run(GTK_WINDOW(login), GTK_DIALOG_MODAL,
								   GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
								   _("Verify code error!"));
			wqq_session_get_captcha_image_async(login->session,
												login->captcha_file,
												on_wqq_session_get_captcha_image_callback,
												login);
			return;
		}
	}
	wqq_ui_login_switch_to_login(login);
}

static void on_wqq_session_set_status_callback(SoupSession * session,
											   SoupMessage * msg,
											   gpointer data)
{
	WqqUiLogin *login = (WqqUiLogin *) data;
	guint status;
	g_object_get(msg, "status-code", &status, NULL);
	if (!SOUP_STATUS_IS_SUCCESSFUL(status)) {
		if (status != SOUP_STATUS_CANCELLED)
			wqq_message_dialog_run(GTK_WINDOW(login),
								   GTK_DIALOG_MODAL,
								   GTK_MESSAGE_ERROR,
								   GTK_BUTTONS_OK,
								   _("Fail to set status!"));
		wqq_ui_login_switch_to_login(login);
	} else {
		wqq_ui_login_save_accounts(login);
		if (login->callback != NULL)
			login->callback(login, login->session, login->user_data);
	}
}

static void on_cancel_button_pressed(GtkWidget * widget, gpointer data)
{
	WqqUiLogin *login = (WqqUiLogin *) data;
	soup_session_abort(SOUP_SESSION(login->session));
	/*wqq_ui_login_switch_to_login(login); */
}

static void wqq_ui_login_switch_to_login(WqqUiLogin * login)
{
	g_object_ref(login->vbox);
	gtk_container_remove(GTK_CONTAINER(login), login->vbox);
	gtk_container_add(GTK_CONTAINER(login), login->grid);
}

static void wqq_ui_login_switch_to_logining(WqqUiLogin * login)
{
	g_object_ref(login->grid);
	gtk_container_remove(GTK_CONTAINER(login), login->grid);
	gtk_container_add(GTK_CONTAINER(login), login->vbox);
}

/********************************************************
 * PUBLIC
 *******************************************************/
WqqUiLogin *wqq_ui_login_new()
{
	WqqUiLogin *login =
		(WqqUiLogin *) g_object_new(WQQ_TYPE_UI_LOGIN, NULL);
	return login;
}

void wqq_ui_login_load_accounts(WqqUiLogin * login)
{
#define WQQ_ACCOUNT_FACE_SIZE	20
	g_return_if_fail(WQQ_IS_UI_LOGIN(login));
	gchar **keys = g_key_file_get_keys(login->accounts, WQQ_ACCOUNTS_GROUP,
									   NULL, NULL);
	if (keys == NULL)
		return;
	if (keys[0] != NULL) {
		guint i = g_strv_length(keys);
		GtkListStore *store = gtk_list_store_new(3, GDK_TYPE_PIXBUF,
												 G_TYPE_STRING,
												 G_TYPE_STRING);
		GtkTreeIter iter;
		GdkPixbuf *pixbuf;
		while (i != 0) {
			i--;
			gboolean keep_pwd = g_key_file_get_boolean(login->accounts,
													   WQQ_ACCOUNTS_GROUP,
													   keys[i],
													   NULL);
			gchar *face =
				g_strdup_printf("%s/%s/avatar", login->data_dir, keys[i]);
			pixbuf =
				gdk_pixbuf_new_from_file_at_size(face,
												 WQQ_ACCOUNT_FACE_SIZE,
												 WQQ_ACCOUNT_FACE_SIZE,
												 NULL);
			g_free(face);
			if (pixbuf == NULL)
				pixbuf =
					gdk_pixbuf_new_from_file_at_size
					(WQQ_DATA_FACE_DEFAULT_FILE, WQQ_ACCOUNT_FACE_SIZE,
					 WQQ_ACCOUNT_FACE_SIZE, NULL);
			gchar *pwd;
			pwd = secret_password_lookup_sync(login->secret,
											  NULL, NULL,
											  WQQ_SECRET_SCHEMA_ATTRIBUTE_NAME,
											  keys[i], NULL);
			gtk_list_store_append(store, &iter);
			gtk_list_store_set(store, &iter,
							   0, pixbuf, 1, keys[i], 2, pwd, -1);
			if (pwd) {
				g_free(pwd);
			}
			if (pixbuf != NULL)
				g_object_unref(pixbuf);
		}
		gtk_combo_box_set_model(GTK_COMBO_BOX(login->username_combo),
								GTK_TREE_MODEL(store));
		gtk_combo_box_set_active(GTK_COMBO_BOX(login->username_combo), 0);
		wqq_ui_login_set_entry_icon(login);

		const gchar *pwd =
			gtk_combo_box_get_active_id(GTK_COMBO_BOX
										(login->username_combo));
		if (pwd) {
			wqq_ui_login_set_keep_pwd(login, TRUE);
			wqq_ui_login_set_password(login, pwd);
		} else {
			wqq_ui_login_set_keep_pwd(login, FALSE);
			wqq_ui_login_set_password(login, "");
		}
		wqq_ui_login_set_keep_username(login, TRUE);
	}
	g_strfreev(keys);
}

void wqq_ui_login_save_accounts(WqqUiLogin * login)
{
	g_return_if_fail(WQQ_IS_UI_LOGIN(login));
	const gchar *username = wqq_ui_login_get_username(login);
	const gchar *password = wqq_ui_login_get_password(login);
	if (username == NULL || password == NULL || username[0] == '\0')
		return;
	g_key_file_remove_key(login->accounts, WQQ_ACCOUNTS_GROUP,
						  username, NULL);
	secret_password_clear_sync(login->secret,
							   NULL, NULL,
							   WQQ_SECRET_SCHEMA_ATTRIBUTE_NAME,
							   username, NULL);
	if (!wqq_ui_login_keep_username(login)) {
		return;
	}
	gboolean keep_pwd = wqq_ui_login_keep_pwd(login);
	g_key_file_set_boolean(login->accounts, WQQ_ACCOUNTS_GROUP,
						   username, keep_pwd);
	if (keep_pwd)
		secret_password_store_sync(login->secret,
								   SECRET_COLLECTION_DEFAULT,
								   "wchat user password", password, NULL,
								   NULL, WQQ_SECRET_SCHEMA_ATTRIBUTE_NAME,
								   username, NULL);
	else
		secret_password_clear(login->secret,
							  NULL, NULL, NULL,
							  WQQ_SECRET_SCHEMA_ATTRIBUTE_NAME,
							  username, NULL);
}

const gchar *wqq_ui_login_get_username(WqqUiLogin * login)
{
	g_return_val_if_fail(WQQ_IS_UI_LOGIN(login), NULL);
	return gtk_entry_get_text(GTK_ENTRY(login->username));
}

const gchar *wqq_ui_login_get_password(WqqUiLogin * login)
{
	g_return_val_if_fail(WQQ_IS_UI_LOGIN(login), NULL);
	return gtk_entry_get_text(GTK_ENTRY(login->password));
}

gboolean wqq_ui_login_keep_username(WqqUiLogin * login)
{
	g_return_val_if_fail(WQQ_IS_UI_LOGIN(login), FALSE);
	return
		gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON
									 (login->keep_username));
}

gboolean wqq_ui_login_keep_pwd(WqqUiLogin * login)
{
	g_return_val_if_fail(WQQ_IS_UI_LOGIN(login), FALSE);
	return
		gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(login->keep_pwd));
}

void wqq_ui_login_set_password(WqqUiLogin * login, const gchar * pwd)
{
	g_return_if_fail(WQQ_IS_UI_LOGIN(login));
	if (pwd)
		gtk_entry_set_text(GTK_ENTRY(login->password), pwd);
	else
		gtk_entry_set_text(GTK_ENTRY(login->password), "");
}

void wqq_ui_login_set_keep_username(WqqUiLogin * login, gboolean set)
{
	g_return_if_fail(WQQ_IS_UI_LOGIN(login));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(login->keep_username),
								 set);
}

void wqq_ui_login_set_keep_pwd(WqqUiLogin * login, gboolean set)
{
	g_return_if_fail(WQQ_IS_UI_LOGIN(login));
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(login->keep_pwd), set);
}

WqqUserStatus wqq_ui_login_get_status(WqqUiLogin * login)
{
	g_return_val_if_fail(WQQ_IS_UI_LOGIN(login), WQQ_USER_STATUS_OFFLINE);
	return
		_wqq_user_status_to_enum(gtk_combo_box_get_active_id
								 (GTK_COMBO_BOX(login->status_combo)));
}

const gchar *wqq_ui_login_get_status_to_string(WqqUiLogin * login)
{
	g_return_val_if_fail(WQQ_IS_UI_LOGIN(login), NULL);
	return gtk_combo_box_get_active_id(GTK_COMBO_BOX(login->status_combo));
}

void wqq_ui_login_set_icon_from_file(WqqUiLogin * login,
									 const gchar * path)
{
	g_return_if_fail(WQQ_IS_UI_LOGIN(login));
	gtk_window_set_icon_from_file(GTK_WINDOW(login), path, NULL);
}

WqqSession *wqq_ui_login_get_session(WqqUiLogin * login)
{
	g_return_val_if_fail(WQQ_IS_UI_LOGIN(login), NULL);
	return login->session;
}

void wqq_ui_login_set_callback(WqqUiLogin * login,
							   WqqUiLoginSuccessCallback callback,
							   gpointer data)
{
	g_return_if_fail(WQQ_IS_UI_LOGIN(login));
	login->callback = callback;
	login->user_data = data;
}

void wqq_ui_login_run(WqqUiLogin * login)
{
	g_return_if_fail(WQQ_IS_UI_LOGIN(login));
	gtk_widget_show_all(GTK_WIDGET(login));
}

void wqq_ui_login_destroy(WqqUiLogin * login)
{
	g_return_if_fail(WQQ_IS_UI_LOGIN(login));
	g_signal_handlers_disconnect_by_func(login, on_login_destroy, NULL);
	gtk_widget_destroy(GTK_WIDGET(login));
}

void wqq_ui_login_destroy_quit(WqqUiLogin * login)
{
	g_return_if_fail(WQQ_IS_UI_LOGIN(login));
	gtk_widget_destroy(GTK_WIDGET(login));
}
