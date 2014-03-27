#include "wqquiqqpanel.h"
#include "wqqm4.h"
#include "wqqutils.h"

enum {
	WQQ_UI_QQPANEL_PROPERTY_SESSION = 1,
};

typedef enum {
	WEBQQ_PROTOCOL_USER_SIGN = 1,
	WEBQQ_PROTOCOL_USER_SIGN_FAILED,
	WEBQQ_PROTOCOL_SELF_SIGN,
	WEBQQ_PROTOCOL_SELF_SIGN_FAILED,
	WEBQQ_PROTOCOL_USER_FACE,
	WEBQQ_PROTOCOL_USER_FACE_FAILED,
	WEBQQ_PROTOCOL_SELF_FACE,
	WEBQQ_PROTOCOL_SELF_FACE_FAILED,
	WEBQQ_PROTOCOL_SEND_MSG,
	WEBQQ_PROTOCOL_SEND_MSG_FAILED,
	WEBQQ_PROTOCOL_FRIENDS,
	WEBQQ_PROTOCOL_FRIENDS_FAILED,
	WEBQQ_PROTOCOL_ONLINE_FRIENDS,
	WEBQQ_PROTOCOL_ONLINE_FRIENDS_FAILED,
	WEBQQ_PROTOCOL_GROUPS,
	WEBQQ_PROTOCOL_GROUPS_FAILED,
} WqqUiQQPanelWebqqProtocol;

typedef struct {
	gpointer data;
	gpointer data1;
	gpointer data2;
	gpointer data3;
} WebqqThreadData;

G_DEFINE_TYPE(WqqUiQQPanel, wqq_ui_qqpanel, GTK_TYPE_WINDOW);

static void wqq_ui_qqpanel_getter(GObject * object, guint property_id,
								  GValue * value, GParamSpec * pspec);
static void wqq_ui_qqpanel_setter(GObject * object, guint property_id,
								  const GValue * value,
								  GParamSpec * pspec);
static void wqq_ui_qqpanel_create_status(WqqUiQQPanel * panel);
static void wqq_ui_qqpanel_create_flist(WqqUiQQPanel * panel);
static void wqq_ui_qqpanel_create_glist(WqqUiQQPanel * panel);
static void wqq_ui_qqpanel_show_online_friends(WqqUiQQPanel * panel);
static WqqUiQQCell *wqq_ui_qqpanel_append_qqcell(WqqUiQQPanel * panel,
												 WqqUiQQCategory *
												 category, guint64 uin,
												 const gchar * nick,
												 const gchar * markname,
												 WqqUiQQCellStatus status,
												 const gchar * sign,
												 WqqClientType ctype);
static WqqUiQQCell *wqq_ui_qqpanel_append_qqcell_str(WqqUiQQPanel * panel,
													 WqqUiQQCategory *
													 category,
													 const gchar * uin,
													 const gchar * nick,
													 const gchar *
													 markname,
													 WqqUiQQCellStatus
													 status,
													 const gchar * sign,
													 WqqClientType ctype);
static void on_qqpanel_note_toggled(GtkToggleToolButton * button,
									gpointer data);
static void wqq_ui_qqpanel_start_poll(WqqUiQQPanel * panel);
static void wqq_ui_qqpanel_show_friend_notification(WqqUiQQPanel * panel,
													WqqFriend * fri,
													WqqPollMessage * pm);
static WqqUiQQChat *wqq_ui_qqpanel_append_fchat(WqqUiQQPanel * panel,
												WqqFriend * fri);
static WqqUiQQChat *wqq_ui_qqpanel_append_gchat(WqqUiQQPanel * panel,
												WqqGroup * group);
static WqqUiQQChat *wqq_ui_qqpanel_find_fchat_by_uin(WqqUiQQPanel * panel,
													 guint64 uin);
static WqqUiQQChat *wqq_ui_qqpanel_find_gchat_by_gid(WqqUiQQPanel * panel,
													 guint64 uin);
static void wqq_ui_qqpanel_fchat_append_pm(WqqUiQQPanel * panel,
										   WqqFriend * fri,
										   WqqUiQQChat * chat,
										   WqqPollMessage * pm);
static void wqq_ui_qqpanel_gchat_append_pm(WqqUiQQPanel * panel,
										   WqqGroupMember * member,
										   guint64 uin,
										   WqqUiQQChat * chat,
										   WqqPollMessage * pm);
static void wqq_ui_qqpanel_reposition_qqcell(WqqUiQQCell * cell);
static const gchar *wqq_ui_qqpanel_get_friend_name(WqqFriend * fri);
static const gchar *wqq_ui_qqpanel_get_group_name(WqqGroup * group);
static const gchar *wqq_ui_qqpanel_get_group_member_name(WqqGroupMember *
														 gm, guint64 uin);
static void wqq_ui_qqpanel_copy_default_face(const gchar * user_dir);

static void wqq_ui_qqpanel_send_fmsg_callback(GtkWidget * button,
											  gpointer data);
static void wqq_ui_qqpanel_send_gmsg_callback(GtkWidget * button,
											  gpointer data);

/* */
static gpointer wqq_set_status_thread(gpointer data);
/* 获取自身的头像和签名 */
static gpointer wqq_get_self_avatar_and_sign_thread(gpointer data);
static gboolean wqq_get_self_avatar_and_sign_timeout(gpointer data);
/* 获取好友列表 */
static gpointer wqq_get_friends_and_groups_list_thread(gpointer data);
static gboolean wqq_get_friends_and_groups_list_timeout(gpointer data);
/* 发送消息 */
static gpointer wqq_send_message_thread(gpointer data);
/* poll消息 */
static gpointer wqq_poll_message_thread(gpointer data);
static gboolean wqq_poll_message_timeout(gpointer data);
/* 获取用户头像 */
static gpointer wqq_get_user_avatar_thread(gpointer data);
static gpointer wqq_get_user_sign_thread(gpointer data);
static gboolean wqq_get_user_avatar_and_sign_timeout(gpointer data);
/* 请求获取用户头像的线程 */
static gpointer wqq_request_user_avatar_and_sign(gpointer data);
/* 获取群信息 */
static gpointer wqq_get_group_info_thread(gpointer data);


static void wqq_ui_qqpanel_init(WqqUiQQPanel * panel)
{
	notify_init(WQQ_WCHAT);
	panel->session = NULL;
	panel->data_dir = g_strdup_printf("%s/wchat", g_get_user_data_dir());
	panel->user_dir = NULL;
	panel->faces_dir = NULL;
	panel->avatar_file = NULL;
	panel->ready_poll = FALSE;
	panel->status_queue = g_async_queue_new();
	panel->poll_queue = g_async_queue_new();
	panel->self_queue = g_async_queue_new();
	panel->flist_queue = g_async_queue_new();
	panel->send_queue = g_async_queue_new();
	panel->avatar_queue = g_async_queue_new();
	panel->avatar_queue_r = g_async_queue_new();
	panel->sign_queue = g_async_queue_new();
	panel->sign_queue_r = g_async_queue_new();
	panel->group_queue = g_async_queue_new();
	panel->fchat = NULL;
	panel->gchat = NULL;


	gtk_window_set_default_size(GTK_WINDOW(panel), 280, 640);
	gtk_window_set_title(GTK_WINDOW(panel), WQQ_WCHAT);
	gtk_window_set_position(GTK_WINDOW(panel), GTK_WIN_POS_MOUSE);
	g_signal_connect(G_OBJECT(panel), "destroy",
					 G_CALLBACK(gtk_main_quit), NULL);

	GtkWidget *main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_container_set_border_width(GTK_CONTAINER(main_vbox), 12);
	gtk_container_add(GTK_CONTAINER(panel), main_vbox);
	GtkWidget *nick = gtk_label_new("Wiky L");
	gtk_widget_set_name(nick, "nick_label");
	gtk_widget_set_halign(nick, GTK_ALIGN_START);
	gtk_label_set_ellipsize(GTK_LABEL(nick), PANGO_ELLIPSIZE_END);
	GtkWidget *avatar = (GtkWidget *)
		gtk_tool_button_new(gtk_image_new_from_file
							(WQQ_DATA_FACE_DEFAULT_FILE),
							NULL);
	gtk_widget_set_name(avatar, "avatar_image");
	GtkWidget *sign = gtk_label_new("The quieter you become,"
									"the more you can hear!");
	gtk_label_set_single_line_mode(GTK_LABEL(sign), TRUE);
	gtk_widget_set_name(sign, "sign_label");
	gtk_widget_set_halign(sign, GTK_ALIGN_START);
	gtk_label_set_ellipsize(GTK_LABEL(sign), PANGO_ELLIPSIZE_END);
	GtkWidget *info_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_box_pack_start(GTK_BOX(main_vbox), info_hbox, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(info_hbox), avatar, FALSE, FALSE, 0);
	GtkWidget *ns_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_box_pack_start(GTK_BOX(info_hbox), ns_vbox, TRUE, TRUE, 0);
	GtkWidget *ns_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_box_pack_start(GTK_BOX(ns_vbox), ns_hbox, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(ns_vbox), sign, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(ns_hbox), nick, TRUE, TRUE, 0);
	wqq_ui_qqpanel_create_status(panel);
	gtk_box_pack_start(GTK_BOX(ns_hbox), panel->status, FALSE, FALSE, 0);

	GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_box_pack_start(GTK_BOX(main_vbox), separator, FALSE, FALSE, 0);

#define WQQ_NOTE_IMAGE_SIZE	50
	GtkWidget *note_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(main_vbox), note_hbox, FALSE, FALSE, 0);
	GtkWidget *image =
		gtk_image_new_from_file_at_size(WQQ_DATA_FRIEND_ICON_FILE,
										WQQ_NOTE_IMAGE_SIZE,
										WQQ_NOTE_IMAGE_SIZE);
	GtkToolItem *friend_note = gtk_toggle_tool_button_new();
	gtk_widget_set_name(GTK_WIDGET(friend_note), "friend_note");
	gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(friend_note), image);
	gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(friend_note),
									  TRUE);
	gtk_box_pack_start(GTK_BOX(note_hbox), GTK_WIDGET(friend_note),
					   TRUE, TRUE, 0);

	image = gtk_image_new_from_file_at_size(WQQ_DATA_GROUP_ICON_FILE,
											WQQ_NOTE_IMAGE_SIZE,
											WQQ_NOTE_IMAGE_SIZE);
	GtkToolItem *group_note = gtk_toggle_tool_button_new();
	gtk_widget_set_name(GTK_WIDGET(group_note), "group_note");
	gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(group_note), image);
	gtk_box_pack_start(GTK_BOX(note_hbox), GTK_WIDGET(group_note),
					   TRUE, TRUE, 0);

	image = gtk_image_new_from_file_at_size(WQQ_DATA_CHAT_ICON_FILE,
											WQQ_NOTE_IMAGE_SIZE,
											WQQ_NOTE_IMAGE_SIZE);
	GtkToolItem *chat_note = gtk_toggle_tool_button_new();
	gtk_widget_set_name(GTK_WIDGET(chat_note), "chat_note");
	gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(chat_note), image);
	gtk_box_pack_start(GTK_BOX(note_hbox), GTK_WIDGET(chat_note),
					   TRUE, TRUE, 0);

	g_signal_connect(G_OBJECT(group_note), "toggled",
					 G_CALLBACK(on_qqpanel_note_toggled), panel);
	g_signal_connect(G_OBJECT(chat_note), "toggled",
					 G_CALLBACK(on_qqpanel_note_toggled), panel);
	g_signal_connect(G_OBJECT(friend_note), "toggled",
					 G_CALLBACK(on_qqpanel_note_toggled), panel);

	/* QQLIST */
	GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *viewport = gtk_viewport_new(NULL, NULL);
	gtk_box_pack_start(GTK_BOX(main_vbox), scrolled, TRUE, TRUE, 0);
	WqqUiQQList *flist = wqq_ui_qqlist_new(6);
	WqqUiQQGroupList *glist = wqq_ui_qqgroup_list_new(6);
	gtk_container_add(GTK_CONTAINER(scrolled), viewport);
	gtk_container_add(GTK_CONTAINER(viewport), GTK_WIDGET(flist));

	panel->nick = nick;
	panel->avatar = avatar;
	panel->sign = sign;
	panel->friend_note = friend_note;
	panel->group_note = group_note;
	panel->chat_note = chat_note;
	panel->flist = flist;
	panel->glist = glist;
	panel->viewport = viewport;
	panel->current_widget = GTK_WIDGET(flist);
	gtk_widget_show_all(GTK_WIDGET(glist));
	gtk_widget_show_all(main_vbox);
}

static void wqq_ui_qqpanel_finalize(GObject * object)
{
	WqqUiQQPanel *panel = WQQ_UI_QQPANEL(object);
	g_free(panel->data_dir);
	g_free(panel->user_dir);
	g_free(panel->faces_dir);
	g_free(panel->avatar_file);
	g_async_queue_unref(panel->status_queue);
	g_async_queue_unref(panel->poll_queue);
	g_async_queue_unref(panel->self_queue);
	g_async_queue_unref(panel->flist_queue);
	g_async_queue_unref(panel->send_queue);
	g_async_queue_unref(panel->avatar_queue);
	g_async_queue_unref(panel->avatar_queue_r);
	g_async_queue_unref(panel->sign_queue);
	g_async_queue_unref(panel->sign_queue_r);
	g_async_queue_unref(panel->group_queue);
	g_list_free_full(panel->fchat, (GDestroyNotify) gtk_widget_destroy);
	g_list_free_full(panel->gchat, (GDestroyNotify) gtk_widget_destroy);
}

static void wqq_ui_qqpanel_class_init(WqqUiQQPanelClass * klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = wqq_ui_qqpanel_setter;
	object_class->get_property = wqq_ui_qqpanel_getter;
	object_class->finalize = wqq_ui_qqpanel_finalize;

	GParamSpec *ps;
	ps = g_param_spec_pointer("session",
							  "wqq session",
							  "WqqSession",
							  G_PARAM_READABLE | G_PARAM_WRITABLE |
							  G_PARAM_CONSTRUCT_ONLY);
	g_object_class_install_property(object_class,
									WQQ_UI_QQPANEL_PROPERTY_SESSION, ps);
}

static void wqq_ui_qqpanel_getter(GObject * object, guint property_id,
								  GValue * value, GParamSpec * pspec)
{
	WqqUiQQPanel *panel = WQQ_UI_QQPANEL(object);
	switch (property_id) {
	case WQQ_UI_QQPANEL_PROPERTY_SESSION:
		g_value_set_pointer(value, panel->session);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void wqq_ui_qqpanel_setter(GObject * object, guint property_id,
								  const GValue * value, GParamSpec * pspec)
{
	WqqUiQQPanel *panel = WQQ_UI_QQPANEL(object);
	switch (property_id) {
	case WQQ_UI_QQPANEL_PROPERTY_SESSION:
		panel->session = g_value_get_pointer(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}


static void on_qqpanel_note_toggled(GtkToggleToolButton * button,
									gpointer data)
{
	WqqUiQQPanel *panel = (WqqUiQQPanel *) data;
	/* 暂时屏蔽回调函数，防止递归 */
	g_signal_handlers_block_by_func(panel->friend_note,
									on_qqpanel_note_toggled, panel);
	g_signal_handlers_block_by_func(panel->group_note,
									on_qqpanel_note_toggled, panel);
	g_signal_handlers_block_by_func(panel->chat_note,
									on_qqpanel_note_toggled, panel);
	if (gtk_toggle_tool_button_get_active(button) == FALSE) {
		gtk_toggle_tool_button_set_active(button, TRUE);
		goto OUT;
	}
	g_object_ref(panel->current_widget);
	gtk_container_remove(GTK_CONTAINER(panel->viewport),
						 panel->current_widget);
	if (button == (GtkToggleToolButton *) panel->friend_note) {
		if (gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON
											  (panel->group_note))) {
			gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON
											  (panel->group_note), FALSE);
		} else {
			gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON
											  (panel->chat_note), FALSE);
		}
		panel->current_widget = GTK_WIDGET(panel->flist);
		gtk_container_add(GTK_CONTAINER(panel->viewport),
						  panel->current_widget);
	} else if (button == (GtkToggleToolButton *) panel->group_note) {
		if (gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON
											  (panel->friend_note))) {
			gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON
											  (panel->friend_note), FALSE);
		} else {
			gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON
											  (panel->chat_note), FALSE);
		}
		panel->current_widget = GTK_WIDGET(panel->glist);
		gtk_container_add(GTK_CONTAINER(panel->viewport),
						  panel->current_widget);
	} else {
		if (gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON
											  (panel->group_note))) {
			gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON
											  (panel->group_note), FALSE);
		} else {
			gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON
											  (panel->friend_note), FALSE);
		}
		GtkWidget *label = gtk_label_new("chat");
		gtk_widget_show(label);
		panel->current_widget = label;
		gtk_container_add(GTK_CONTAINER(panel->viewport),
						  panel->current_widget);
	}
  OUT:
	g_signal_handlers_unblock_by_func(panel->friend_note,
									  on_qqpanel_note_toggled, panel);
	g_signal_handlers_unblock_by_func(panel->group_note,
									  on_qqpanel_note_toggled, panel);
	g_signal_handlers_unblock_by_func(panel->chat_note,
									  on_qqpanel_note_toggled, panel);
}

static void on_status_item_activate(GtkMenuItem * item, gpointer data)
{
	WqqUiQQPanel *panel = (WqqUiQQPanel *) data;
	GtkWidget *image =
		gtk_image_menu_item_get_image(GTK_IMAGE_MENU_ITEM(item));
	gtk_button_set_image(GTK_BUTTON(panel->status),
						 gtk_image_new_from_pixbuf(gtk_image_get_pixbuf
												   (GTK_IMAGE(image))));
	WqqUserStatus status =
		_wqq_user_status_to_enum(gtk_menu_item_get_label
								 (GTK_MENU_ITEM(item)));
	WqqUser *user = wqq_session_get_user_self(panel->session);
	if (status == wqq_user_get_status(user))
		return;
	g_async_queue_push(panel->status_queue, (gpointer) status);
}

static void wqq_ui_qqpanel_create_status(WqqUiQQPanel * panel)
{
#define WQQ_UI_QQPANEL_STATUS_ICON_SIZE	16
	GtkWidget *status = gtk_menu_button_new();
	panel->status = status;
	GtkWidget *menu, *item;
	menu = gtk_menu_new();
	item = gtk_image_menu_item_new_with_label(_("online"));
	g_signal_connect(G_OBJECT(item), "activate",
					 G_CALLBACK(on_status_item_activate), panel);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item),
								  gtk_image_new_from_file_at_size
								  (WQQ_DATA_STATUS_ONLINE_FILE,
								   WQQ_UI_QQPANEL_STATUS_ICON_SIZE,
								   WQQ_UI_QQPANEL_STATUS_ICON_SIZE));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_image_menu_item_set_always_show_image(GTK_IMAGE_MENU_ITEM(item),
											  TRUE);

	item = gtk_image_menu_item_new_with_label(_("away"));
	g_signal_connect(G_OBJECT(item), "activate",
					 G_CALLBACK(on_status_item_activate), panel);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item),
								  gtk_image_new_from_file_at_size
								  (WQQ_DATA_STATUS_AWAY_FILE,
								   WQQ_UI_QQPANEL_STATUS_ICON_SIZE,
								   WQQ_UI_QQPANEL_STATUS_ICON_SIZE));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_image_menu_item_set_always_show_image(GTK_IMAGE_MENU_ITEM(item),
											  TRUE);

	item = gtk_image_menu_item_new_with_label(_("busy"));
	g_signal_connect(G_OBJECT(item), "activate",
					 G_CALLBACK(on_status_item_activate), panel);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item),
								  gtk_image_new_from_file_at_size
								  (WQQ_DATA_STATUS_BUSY_FILE,
								   WQQ_UI_QQPANEL_STATUS_ICON_SIZE,
								   WQQ_UI_QQPANEL_STATUS_ICON_SIZE));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_image_menu_item_set_always_show_image(GTK_IMAGE_MENU_ITEM(item),
											  TRUE);

	item = gtk_image_menu_item_new_with_label(_("hidden"));
	g_signal_connect(G_OBJECT(item), "activate",
					 G_CALLBACK(on_status_item_activate), panel);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item),
								  gtk_image_new_from_file_at_size
								  (WQQ_DATA_STATUS_HIDDEN_FILE,
								   WQQ_UI_QQPANEL_STATUS_ICON_SIZE,
								   WQQ_UI_QQPANEL_STATUS_ICON_SIZE));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_image_menu_item_set_always_show_image(GTK_IMAGE_MENU_ITEM(item),
											  TRUE);

	item = gtk_image_menu_item_new_with_label(_("callme"));
	g_signal_connect(G_OBJECT(item), "activate",
					 G_CALLBACK(on_status_item_activate), panel);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item),
								  gtk_image_new_from_file_at_size
								  (WQQ_DATA_STATUS_CALLME_FILE,
								   WQQ_UI_QQPANEL_STATUS_ICON_SIZE,
								   WQQ_UI_QQPANEL_STATUS_ICON_SIZE));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_image_menu_item_set_always_show_image(GTK_IMAGE_MENU_ITEM(item),
											  TRUE);

	item = gtk_image_menu_item_new_with_label(_("silent"));
	g_signal_connect(G_OBJECT(item), "activate",
					 G_CALLBACK(on_status_item_activate), panel);
	gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item),
								  gtk_image_new_from_file_at_size
								  (WQQ_DATA_STATUS_SILENT_FILE,
								   WQQ_UI_QQPANEL_STATUS_ICON_SIZE,
								   WQQ_UI_QQPANEL_STATUS_ICON_SIZE));
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
	gtk_image_menu_item_set_always_show_image(GTK_IMAGE_MENU_ITEM(item),
											  TRUE);

	gtk_menu_button_set_popup(GTK_MENU_BUTTON(status), menu);
	gtk_widget_show_all(menu);

	gtk_button_set_image(GTK_BUTTON(status),
						 gtk_image_new_from_file_at_size
						 (WQQ_DATA_STATUS_ONLINE_FILE,
						  WQQ_UI_QQPANEL_STATUS_ICON_SIZE,
						  WQQ_UI_QQPANEL_STATUS_ICON_SIZE));

	gtk_widget_set_name(status, "status_menu");

}

static void wqq_ui_qqpanel_show_online_friends(WqqUiQQPanel * panel)
{
	GList *glp = wqq_session_get_category_list(panel->session);
	while (glp != NULL) {
		WqqCategory *category = (WqqCategory *) glp->data;
		WqqUiQQCategory *qc = wqq_category_get_data(category);
		GList *flp = wqq_category_get_friends_list(category);
		while (flp != NULL) {
			WqqFriend *fri = (WqqFriend *) flp->data;
			WqqUiQQCell *cell = wqq_friend_get_data(fri);
			wqq_ui_qqcell_set_status(cell,
									 wqq_user_get_status(WQQ_USER(fri)));
			g_message("%s: %s", wqq_friend_get_markname(fri),
					  wqq_user_get_status_to_string(WQQ_USER(fri)));
			flp = g_list_next(flp);
		}
		wqq_ui_qqcategory_count_online_and_total(qc);
		wqq_ui_qqcategory_reorder_qqcell(qc);
		glp = g_list_next(glp);
	}
}

static gboolean on_wqq_ui_friend_qqcell_pressed(WqqUiQQCell * cell,
												GdkEventButton * event,
												gpointer data)
{
	if (event->type == GDK_2BUTTON_PRESS && event->button == 1) {
		WqqUiQQPanel *panel = wqq_ui_qqcell_get_data(cell);
		WqqFriend *fri = wqq_ui_qqcell_get_data1(cell);
		WqqUiQQChat *chat = wqq_ui_qqpanel_append_fchat(panel, fri);
		wqq_ui_qqchat_run(chat);
	} else if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
		g_message("right mouse clicked");
	}
	return TRUE;
}

static gboolean on_wqq_ui_group_qqcell_pressed(WqqUiQQGroup * cell,
											   GdkEventButton * event,
											   gpointer data)
{
	if (event->type == GDK_2BUTTON_PRESS && event->button == 1) {
		WqqUiQQPanel *panel = (WqqUiQQPanel *) data;
		WqqGroup *group = wqq_ui_qqgroup_get_data(cell);
		WqqUiQQChat *chat = wqq_ui_qqpanel_append_gchat(panel, group);
		wqq_ui_qqchat_run(chat);
	} else if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
		g_message("right mouse clicked");
	}
	return TRUE;
}


static void wqq_ui_qqpanel_create_flist(WqqUiQQPanel * panel)
{
	wqq_ui_qqlist_clear_all(panel->flist);
	GList *clist = wqq_session_get_category_list(panel->session);
	while (clist != NULL) {
		WqqCategory *category = (WqqCategory *) clist->data;
		WqqUiQQCategory *qc;
		qc = wqq_ui_qqlist_append_category(panel->flist,
										   wqq_category_get_name
										   (category),
										   0,
										   wqq_category_get_length
										   (category));
		wqq_category_set_data(category, qc);
		GList *flist = wqq_category_get_friends_list(category);
		while (flist != NULL) {
			WqqFriend *fri = (WqqFriend *) flist->data;
			WqqUiQQCell *cell;
			cell = wqq_ui_qqpanel_append_qqcell(panel, qc,
												wqq_user_get_uin
												(WQQ_USER
												 (fri)),
												wqq_user_get_nick
												(WQQ_USER
												 (fri)),
												wqq_friend_get_markname
												(fri),
												wqq_user_get_status
												(WQQ_USER
												 (fri)),
												"", WQQ_CLIENT_TYPE_PC);
			wqq_ui_qqcell_set_uin(cell, wqq_user_get_uin(WQQ_USER(fri)));
			wqq_ui_qqcell_set_data(cell, panel);
			wqq_ui_qqcell_set_data1(cell, fri);
			wqq_ui_qqcell_set_data2(cell, qc);
			g_signal_connect(G_OBJECT(cell), "button-press-event",
							 G_CALLBACK(on_wqq_ui_friend_qqcell_pressed),
							 cell);
			wqq_friend_set_data(fri, cell);
			flist = g_list_next(flist);

			 /**/				/* 获取用户头像和签名 */
				/*g_async_queue_push(panel->avatar_queue, (gpointer) fri); */
				/*g_async_queue_push(panel->sign_queue, (gpointer) fri); */
		}
		clist = g_list_next(clist);
	}

	gtk_widget_show_all(GTK_WIDGET(panel->flist));
}

static void wqq_ui_qqpanel_send_fmsg_callback(GtkWidget * button,
											  gpointer data)
{
	WqqUiQQChat *chat = (WqqUiQQChat *) data;
	WqqUiQQPanel *panel = wqq_ui_qqchat_get_data(chat);
	WqqFriend *fri = wqq_ui_qqchat_get_data1(chat);
	WqqUser *user = wqq_session_get_user_self(panel->session);
	const gchar *text = wqq_ui_qqchat_get_textview_text(chat);
	if (text == NULL || strlen(text) == 0)
		return;
	wqq_ui_qqchat_append_time_now(chat);
	WqqQQChatMessage *msg =
		wqq_qqchat_message_new(wqq_user_get_nick(user));
	wqq_qqchat_message_append_text(msg, text);
	if (g_file_test(panel->avatar_file, G_FILE_TEST_IS_REGULAR))
		wqq_ui_qqchat_append_message_from_self(chat, "avatar", msg);
	else
		wqq_ui_qqchat_append_message_from_self(chat, "default_face", msg);
	wqq_qqchat_message_free(msg);

	WqqSendMessage *sm =
		wqq_send_message_new(WQQ_SEND_MESSAGE_TYPE_FRIEND);
	wqq_send_message_set_to(sm, wqq_user_get_uin(WQQ_USER(fri)));
	wqq_send_message_set_face(sm, wqq_user_get_face(WQQ_USER(fri)));
	wqq_send_message_append_text_content(sm, text);
	wqq_ui_qqchat_clear_textview_text(chat);

	GAsyncQueue *queue = panel->send_queue;
	g_async_queue_lock(queue);
	g_async_queue_push_unlocked(queue, (gpointer) sm);
	g_async_queue_unlock(queue);
}

static void wqq_ui_qqpanel_send_gmsg_callback(GtkWidget * button,
											  gpointer data)
{
	WqqUiQQChat *chat = (WqqUiQQChat *) data;
	WqqUiQQPanel *panel = wqq_ui_qqchat_get_data(chat);
	WqqGroup *group = wqq_ui_qqchat_get_data1(chat);
	WqqUser *user = wqq_session_get_user_self(panel->session);
	const gchar *text = wqq_ui_qqchat_get_textview_text(chat);
	if (text == NULL || strlen(text) == 0)
		return;
	wqq_ui_qqchat_append_time_now(chat);
	WqqQQChatMessage *msg =
		wqq_qqchat_message_new(wqq_user_get_nick(user));
	wqq_qqchat_message_append_text(msg, text);
	if (g_file_test(panel->avatar_file, G_FILE_TEST_IS_REGULAR))
		wqq_ui_qqchat_append_message_from_self(chat, "avatar", msg);
	else
		wqq_ui_qqchat_append_message_from_self(chat, "default_face", msg);
	wqq_qqchat_message_free(msg);

	WqqSendMessage *sm = wqq_send_message_new(WQQ_SEND_MESSAGE_TYPE_GROUP);
	wqq_send_message_set_to(sm, wqq_group_get_gid(group));
	wqq_send_message_append_text_content(sm, text);
	wqq_ui_qqchat_clear_textview_text(chat);

	GAsyncQueue *queue = panel->send_queue;
	g_async_queue_lock(queue);
	g_async_queue_push_unlocked(queue, (gpointer) sm);
	g_async_queue_unlock(queue);
}

static WqqUiQQChat *wqq_ui_qqpanel_find_fchat_by_uin(WqqUiQQPanel * panel,
													 guint64 uin)
{
	GList *flp = panel->fchat;
	while (flp) {
		WqqUiQQChat *chat = (WqqUiQQChat *) flp->data;
		if (wqq_ui_qqchat_get_uin(chat) == uin)
			return chat;
		flp = g_list_next(flp);
	}
	return NULL;
}

static void on_wqq_ui_qqpanel_fchat_destroy(WqqUiQQChat * chat,
											gpointer data)
{
	WqqUiQQPanel *panel = (WqqUiQQPanel *) data;
	panel->fchat = g_list_remove(panel->fchat, chat);
}

static void on_wqq_ui_qqpanel_gchat_destroy(WqqUiQQChat * chat,
											gpointer data)
{
	WqqUiQQPanel *panel = (WqqUiQQPanel *) data;
	panel->gchat = g_list_remove(panel->gchat, chat);
}

static WqqUiQQChat *wqq_ui_qqpanel_append_fchat(WqqUiQQPanel * panel,
												WqqFriend * fri)
{
	WqqUiQQChat *chat;
	chat =
		wqq_ui_qqpanel_find_fchat_by_uin(panel,
										 wqq_user_get_uin(WQQ_USER(fri)));
	if (chat == NULL) {
		gchar *base = g_strdup_printf("file://%s/", panel->user_dir);
		chat = wqq_ui_qqchat_new(base);
		wqq_ui_qqchat_set_data(chat, panel);
		wqq_ui_qqchat_set_data1(chat, fri);
		wqq_ui_qqchat_set_uin(chat, wqq_user_get_uin(WQQ_USER(fri)));
		const gchar *name = wqq_ui_qqpanel_get_friend_name(fri);
		gchar *title = g_strdup_printf(_("talk with %s"), name);
		wqq_ui_qqchat_set_title(chat, title);
		wqq_ui_qqchat_set_send_callback(chat,
										G_CALLBACK
										(wqq_ui_qqpanel_send_fmsg_callback),
										chat);
		panel->fchat = g_list_append(panel->fchat, chat);
		g_signal_connect(G_OBJECT(chat), "destroy",
						 G_CALLBACK(on_wqq_ui_qqpanel_fchat_destroy),
						 panel);
		g_free(title);
		g_free(base);
	}
	return chat;
}

static WqqUiQQChat *wqq_ui_qqpanel_append_gchat(WqqUiQQPanel * panel,
												WqqGroup * group)
{
	WqqUiQQChat *chat;
	chat = wqq_ui_qqpanel_find_gchat_by_gid(panel,
											wqq_group_get_gid(group));
	if (chat == NULL) {
		gchar *base = g_strdup_printf("file://%s/", panel->user_dir);
		chat = wqq_ui_qqchat_new(base);
		wqq_ui_qqchat_set_data(chat, panel);
		wqq_ui_qqchat_set_data1(chat, group);
		wqq_ui_qqchat_set_uin(chat, wqq_group_get_gid(group));
		const gchar *name = wqq_ui_qqpanel_get_group_name(group);
		gchar *title = g_strdup_printf(_("talk in group %s"), name);
		wqq_ui_qqchat_set_title(chat, title);
		wqq_ui_qqchat_set_send_callback(chat,
										G_CALLBACK
										(wqq_ui_qqpanel_send_gmsg_callback),
										chat);
		panel->gchat = g_list_append(panel->gchat, chat);
		g_signal_connect(G_OBJECT(chat), "destroy",
						 G_CALLBACK(on_wqq_ui_qqpanel_gchat_destroy),
						 panel);
		g_free(title);
		g_free(base);
	}
	return chat;
}

static WqqUiQQChat *wqq_ui_qqpanel_find_gchat_by_gid(WqqUiQQPanel * panel,
													 guint64 gid)
{
	GList *glp = panel->gchat;
	while (glp) {
		WqqUiQQChat *chat = (WqqUiQQChat *) glp->data;
		if (wqq_ui_qqchat_get_uin(chat) == gid)
			return chat;
		glp = g_list_next(glp);
	}
	return NULL;
}

static void wqq_ui_qqpanel_create_glist(WqqUiQQPanel * panel)
{
	GList *glp = wqq_session_get_glist(panel->session);
	while (glp != NULL) {
		WqqUiQQGroup *g;
		WqqGroup *group = (WqqGroup *) glp->data;
		gchar *path = g_strdup_printf("%s/%lu",
									  panel->faces_dir,
									  wqq_group_get_gid(group));
		if (g_file_test(path, G_FILE_TEST_IS_REGULAR))
			g = wqq_ui_qqgroup_list_append_group(panel->glist,
												 wqq_group_get_name(group),
												 wqq_group_get_mark(group),
												 path);
		else
			g = wqq_ui_qqgroup_list_append_group(panel->glist,
												 wqq_group_get_name(group),
												 wqq_group_get_mark(group),
												 WQQ_DATA_GROUP_FACE_DEFAULT_FILE);
		g_signal_connect(G_OBJECT(g), "button-press-event",
						 G_CALLBACK(on_wqq_ui_group_qqcell_pressed),
						 panel);
		g_free(path);
		wqq_ui_qqgroup_set_data(g, group);
		g_message("group: %s(%s) gid=%lu",
				  wqq_group_get_name(group),
				  wqq_group_get_mark(group), wqq_group_get_gid(group));
		glp = g_list_next(glp);
	}
	gtk_widget_show_all(GTK_WIDGET(panel->glist));
}

static WqqUiQQCell *wqq_ui_qqpanel_append_qqcell(WqqUiQQPanel * panel,
												 WqqUiQQCategory *
												 category, guint64 uin,
												 const gchar * nick,
												 const gchar * markname,
												 WqqUiQQCellStatus status,
												 const gchar * sign,
												 WqqClientType ctype)
{
	gchar *str = g_strdup_printf("%lu", uin);
	WqqUiQQCell *cell = wqq_ui_qqpanel_append_qqcell_str(panel, category,
														 str, nick,
														 markname, status,
														 sign, ctype);
	g_free(str);
	return cell;
}

static WqqUiQQCell *wqq_ui_qqpanel_append_qqcell_str(WqqUiQQPanel * panel,
													 WqqUiQQCategory *
													 category,
													 const gchar * uin,
													 const gchar * nick,
													 const gchar *
													 markname,
													 WqqUiQQCellStatus
													 status,
													 const gchar * sign,
													 WqqClientType ctype)
{
	gchar *avatar_file = g_strdup_printf("%s/%s", panel->faces_dir, uin);
	GdkPixbuf *avatar = gdk_pixbuf_new_from_file(avatar_file, NULL);
	if (avatar == NULL) {
		avatar =
			gdk_pixbuf_new_from_file(WQQ_DATA_FACE_DEFAULT_FILE, NULL);
	}
	WqqUiQQCell *cell = wqq_ui_qqcategory_append_cell(category, avatar,
													  nick, markname,
													  status, sign, NULL);
	if (avatar != NULL)
		g_object_unref(avatar);
	g_free(avatar_file);
	return cell;
}

static gchar *wqq_message_contents_list_to_string(GList * clist)
{
	GString *string = g_string_new("");
	while (clist != NULL) {
		WqqMessageContent *content = (WqqMessageContent *) clist->data;
		WqqMessageContentType mctype =
			wqq_message_content_get_ctype(content);
		if (mctype == WQQ_MESSAGE_CONTENT_TYPE_TEXT) {
			g_string_append(string, wqq_message_content_get_text(content));
		} else if (mctype == WQQ_MESSAGE_CONTENT_TYPE_FACE) {
			g_string_append_printf(string, _("[face,%u]"),
								   wqq_message_content_get_face(content));
		} else if (mctype == WQQ_MESSAGE_CONTENT_TYPE_PIC) {
			g_string_append_printf(string, _("[offpic]"));
		} else {
			g_string_append(string, _("[unknown]"));
		}
		clist = g_list_next(clist);
	}
	gchar *body = g_string_free(string, FALSE);
	return body;
}

static gchar *wqq_ui_qqpanel_get_face_path(WqqUiQQPanel * panel,
										   WqqFriend * fri)
{
	gchar *path = g_strdup_printf("%s/%lu", panel->faces_dir,
								  wqq_user_get_uin(WQQ_USER(fri)));
	if (!g_file_test(path, G_FILE_TEST_IS_REGULAR)) {
		g_free(path);
		path = g_strdup_printf("%s/default_face", panel->user_dir);
	}
	return path;
}

static gchar *wqq_ui_qqpanel_get_group_face_path(WqqUiQQPanel * panel,
												 WqqGroup * group)
{
	gchar *path = g_strdup_printf("%s/%lu", panel->faces_dir,
								  wqq_group_get_gid(group));
	if (!g_file_test(path, G_FILE_TEST_IS_REGULAR)) {
		g_free(path);
		path = g_strdup_printf("%s/default_group_face", panel->user_dir);
	}
	return path;
}

static void wqq_ui_qqpanel_show_friend_notification(WqqUiQQPanel * panel,
													WqqFriend * fri,
													WqqPollMessage * pm)
{
	gchar *path = wqq_ui_qqpanel_get_face_path(panel, fri);
	const gchar *summary = wqq_ui_qqpanel_get_friend_name(fri);
	gchar *body =
		wqq_message_contents_list_to_string(wqq_poll_message_get_contents
											(pm));
	NotifyNotification *notification =
		notify_notification_new(summary, body, path);
	notify_notification_show(notification, NULL);
	g_object_unref(notification);
	g_free(path);
	g_free(body);
}

static void wqq_ui_qqpanel_show_group_notification(WqqUiQQPanel * panel,
												   WqqGroup * group,
												   WqqPollMessage * pm)
{
	gchar *path = wqq_ui_qqpanel_get_group_face_path(panel, group);
	const gchar *summary = wqq_ui_qqpanel_get_group_name(group);
	gchar *body =
		wqq_message_contents_list_to_string(wqq_poll_message_get_contents
											(pm));
	NotifyNotification *notification =
		notify_notification_new(summary, body, path);
	notify_notification_show(notification, NULL);
	g_object_unref(notification);
	g_free(path);
	g_free(body);
}

static void wqq_ui_qqpanel_show_status_notification(WqqUiQQPanel * panel,
													WqqFriend * fri,
													const gchar * status)
{
	gchar *path = wqq_ui_qqpanel_get_face_path(panel, fri);
	const gchar *summary = wqq_ui_qqpanel_get_friend_name(fri);
	gchar *body = g_strdup_printf("[%s]", status);
	NotifyNotification *notification =
		notify_notification_new(summary, body, path);
	notify_notification_show(notification, NULL);
	g_object_unref(notification);
	g_free(path);
	g_free(body);
}

static void wqq_ui_qqpanel_fchat_append_pm(WqqUiQQPanel * panel,
										   WqqFriend * fri,
										   WqqUiQQChat * chat,
										   WqqPollMessage * pm)
{
	GList *ml = wqq_poll_message_get_contents(pm);
	const gchar *name = wqq_ui_qqpanel_get_friend_name(fri);
	WqqQQChatMessage *qm = wqq_qqchat_message_new(name);
	wqq_ui_qqchat_append_time_now(chat);
	while (ml) {
		WqqMessageContent *mc = (WqqMessageContent *) ml->data;
		WqqMessageContentType mctype = wqq_message_content_get_ctype(mc);
		if (mctype == WQQ_MESSAGE_CONTENT_TYPE_TEXT) {
			wqq_qqchat_message_append_text(qm,
										   wqq_message_content_get_text
										   (mc));
			g_message("poll message:%s", wqq_message_content_get_text(mc));
		} else if (mctype == WQQ_MESSAGE_CONTENT_TYPE_FACE) {
			wqq_qqchat_message_append_face(qm,
										   wqq_message_content_get_face
										   (mc));
		} else if (mctype == WQQ_MESSAGE_CONTENT_TYPE_PIC) {
			wqq_qqchat_message_append_image(qm,
											wqq_message_content_get_pic
											(mc));
		} else {
			g_warning("Unknown message content type!");
		}
		ml = g_list_next(ml);
	}
	gchar *face = g_strdup_printf("%s/%lu",
								  panel->faces_dir,
								  wqq_ui_qqchat_get_uin(chat));
	if (g_file_test(face, G_FILE_TEST_IS_REGULAR)) {
		g_free(face);
		face = g_strdup_printf("faces/%lu", wqq_ui_qqchat_get_uin(chat));
		wqq_ui_qqchat_append_message_from_other(chat, face, qm);
	} else
		wqq_ui_qqchat_append_message_from_other(chat, "default_face", qm);
	g_free(face);
	wqq_qqchat_message_free(qm);
}

static void wqq_ui_qqpanel_gchat_append_pm(WqqUiQQPanel * panel,
										   WqqGroupMember * member,
										   guint64 uin,
										   WqqUiQQChat * chat,
										   WqqPollMessage * pm)
{
	GList *ml = wqq_poll_message_get_contents(pm);
	const gchar *name = wqq_ui_qqpanel_get_group_member_name(member, uin);
	WqqQQChatMessage *qm = wqq_qqchat_message_new(name);
	wqq_ui_qqchat_append_time_now(chat);
	while (ml) {
		WqqMessageContent *mc = (WqqMessageContent *) ml->data;
		WqqMessageContentType mctype = wqq_message_content_get_ctype(mc);
		if (mctype == WQQ_MESSAGE_CONTENT_TYPE_TEXT) {
			wqq_qqchat_message_append_text(qm,
										   wqq_message_content_get_text
										   (mc));
			g_message("poll message:%s", wqq_message_content_get_text(mc));
		} else if (mctype == WQQ_MESSAGE_CONTENT_TYPE_FACE) {
			wqq_qqchat_message_append_face(qm,
										   wqq_message_content_get_face
										   (mc));
		} else if (mctype == WQQ_MESSAGE_CONTENT_TYPE_PIC) {
			wqq_qqchat_message_append_image(qm,
											wqq_message_content_get_pic
											(mc));
		} else {
			g_warning("Unknown message content type!");
		}
		ml = g_list_next(ml);
	}
	gchar *face = g_strdup_printf("%s/%lu",
								  panel->faces_dir, uin);
	if (g_file_test(face, G_FILE_TEST_IS_REGULAR)) {
		g_free(face);
		face = g_strdup_printf("faces/%lu", uin);
		wqq_ui_qqchat_append_message_from_other(chat, face, qm);
	} else
		wqq_ui_qqchat_append_message_from_other(chat, "default_face", qm);
	g_free(face);
	wqq_qqchat_message_free(qm);
}

static void wqq_ui_qqpanel_reposition_qqcell(WqqUiQQCell * cell)
{
	WqqUiQQCategory *category = wqq_ui_qqcell_get_data2(cell);
	wqq_ui_qqcategory_reposition_qqcell(category, cell);
}

static void wqq_ui_qqpanel_start_poll(WqqUiQQPanel * panel)
{
	g_thread_new("poll-message", wqq_poll_message_thread, panel);
	g_thread_new("send-message", wqq_send_message_thread, panel);
	g_thread_new("get-user-avatar", wqq_get_user_avatar_thread, panel);
	g_thread_new("get-user-sign", wqq_get_user_sign_thread, panel);
	g_thread_new("request-user-avatar-sign",
				 wqq_request_user_avatar_and_sign, panel);
	g_thread_new("get-group-info", wqq_get_group_info_thread, panel);
	g_timeout_add(1500, wqq_poll_message_timeout, panel);
	g_timeout_add(2500, wqq_get_user_avatar_and_sign_timeout, panel);
	g_message("Start to poll message from server!");
}

static const gchar *wqq_ui_qqpanel_get_friend_name(WqqFriend * fri)
{
	const gchar *name = wqq_friend_get_markname(fri);
	if (name)
		return name;
	return wqq_user_get_nick(WQQ_USER(fri));
}

static const gchar *wqq_ui_qqpanel_get_group_name(WqqGroup * group)
{
	const gchar *name = wqq_group_get_mark(group);
	if (name)
		return name;
	return wqq_group_get_name(group);
}

static const gchar *wqq_ui_qqpanel_get_group_member_name(WqqGroupMember *
														 gm, guint64 uin)
{
	static gchar *uin_name = NULL;
	if (gm) {
		const gchar *name = wqq_group_member_get_card(gm);
		if (name)
			return name;
		return wqq_user_get_nick(WQQ_USER(gm));
	}
	g_free(uin_name);
	uin_name = g_strdup_printf("%lu", uin);
	return uin_name;
}

static void wqq_ui_qqpanel_copy_default_face(const gchar * user_dir)
{
	gchar *path = g_strdup_printf("%s/default_face", user_dir);
	GFile *file = g_file_new_for_path(path);
	GFile *src = g_file_new_for_path(WQQ_DATA_FACE_DEFAULT_FILE);
	g_file_copy_async(src, file, G_FILE_COPY_OVERWRITE, 0,
					  NULL, NULL, NULL, NULL, NULL);
	g_object_unref(file);
	g_object_unref(src);
	g_free(path);

	path = g_strdup_printf("%s/default_group_face", user_dir);
	file = g_file_new_for_path(path);
	src = g_file_new_for_path(WQQ_DATA_GROUP_FACE_DEFAULT_FILE);
	g_file_copy_async(src, file, G_FILE_COPY_OVERWRITE, 0,
					  NULL, NULL, NULL, NULL, NULL);
	g_object_unref(file);
	g_object_unref(src);
	g_free(path);
}

static gpointer wqq_set_status_thread(gpointer data)
{
	WqqUiQQPanel *panel = (WqqUiQQPanel *) data;
	WqqSession *session = panel->session;
	GAsyncQueue *queue = panel->status_queue;
	WqqUserStatus status;
	gpointer popdata;
	guint wqq_status;
	g_async_queue_ref(queue);
	while ((popdata = g_async_queue_pop(queue))) {
		status = (WqqUserStatus) popdata;
		wqq_status = wqq_session_set_status(session, status);
		if (wqq_status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL) {
			g_warning("Fail to set status to %s",
					  _wqq_user_status_to_string(status));
		} else {
			g_message("Set status to %s",
					  _wqq_user_status_to_string(status));
		}
	}
	g_async_queue_unref(queue);
	return NULL;
}

/* 获取自身的头像和签名 */
static gpointer wqq_get_self_avatar_and_sign_thread(gpointer data)
{
	WqqUiQQPanel *panel = (WqqUiQQPanel *) data;
	WqqSession *session = panel->session;
	GAsyncQueue *queue = panel->self_queue;
	g_async_queue_ref(queue);
	guint status = wqq_session_get_self_sign(session);
	if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL) {
		g_async_queue_push(queue,
						   (gpointer) WEBQQ_PROTOCOL_SELF_SIGN_FAILED);
	} else {
		g_async_queue_push(queue, (gpointer) WEBQQ_PROTOCOL_SELF_SIGN);
	}
	status = wqq_session_get_self_face(session, panel->avatar_file);
	if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL) {
		g_async_queue_push(queue,
						   (gpointer) WEBQQ_PROTOCOL_SELF_FACE_FAILED);
	} else {
		g_async_queue_push(queue, (gpointer) WEBQQ_PROTOCOL_SELF_FACE);
	}
	g_async_queue_unref(queue);
	return NULL;
}

static gboolean wqq_get_self_avatar_and_sign_timeout(gpointer data)
{
	WqqUiQQPanel *panel = (WqqUiQQPanel *) data;
	GAsyncQueue *queue = panel->self_queue;
	WqqSession *session = panel->session;
	gpointer popdata = g_async_queue_try_pop(queue);
	WqqUser *user;
	if (popdata) {
		switch ((gulong) popdata) {
		case WEBQQ_PROTOCOL_SELF_SIGN:
			user = wqq_session_get_user_self(session);
			wqq_ui_qqpanel_set_sign(panel, wqq_user_get_sign(user));
			break;
		case WEBQQ_PROTOCOL_SELF_FACE:
			wqq_ui_qqpanel_set_avatar_from_file(panel, panel->avatar_file);
			return FALSE;
		case WEBQQ_PROTOCOL_SELF_SIGN_FAILED:
			g_warning("Get self sign failed");
			break;
		case WEBQQ_PROTOCOL_SELF_FACE_FAILED:
			g_warning("Get self face failed");
			return FALSE;
		default:
			g_warning("wqq_get_self_avatar_and_sign_timeout:"
					  "Unknown response!");
			break;
		}
	}
	return TRUE;
}

/* 获取好友列表 */
static gpointer wqq_get_friends_and_groups_list_thread(gpointer data)
{
	WqqUiQQPanel *panel = (WqqUiQQPanel *) data;
	WqqSession *session = panel->session;
	GAsyncQueue *queue = panel->flist_queue;
	g_async_queue_ref(queue);
	guint status = wqq_session_get_friends_list(session);
	if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL) {
		g_async_queue_push(queue,
						   (gpointer) WEBQQ_PROTOCOL_FRIENDS_FAILED);
	} else {
		g_async_queue_push(queue, (gpointer) WEBQQ_PROTOCOL_FRIENDS);
	}

	status = wqq_session_get_groups_list(session);
	if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL) {
		g_async_queue_push(queue, (gpointer) WEBQQ_PROTOCOL_GROUPS_FAILED);
	} else {
		g_async_queue_push(queue, (gpointer) WEBQQ_PROTOCOL_GROUPS);
	}

	status = wqq_session_get_online_friends_list(session);
	if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL) {
		g_async_queue_push(queue, (gpointer)
						   WEBQQ_PROTOCOL_ONLINE_FRIENDS_FAILED);
	} else {
		g_async_queue_push(queue,
						   (gpointer) WEBQQ_PROTOCOL_ONLINE_FRIENDS);
	}
	g_async_queue_unref(queue);
	return NULL;
}

static gboolean wqq_get_friends_and_groups_list_timeout(gpointer data)
{
	WqqUiQQPanel *panel = (WqqUiQQPanel *) data;
	WqqSession *session = panel->session;
	GAsyncQueue *queue = panel->flist_queue;
	gpointer popdata = g_async_queue_try_pop(queue);
	if (popdata) {
		switch ((gulong) popdata) {
		case WEBQQ_PROTOCOL_FRIENDS:
			wqq_ui_qqpanel_create_flist(panel);
			break;
		case WEBQQ_PROTOCOL_GROUPS:
			wqq_ui_qqpanel_create_glist(panel);
			break;
		case WEBQQ_PROTOCOL_ONLINE_FRIENDS:
			wqq_ui_qqpanel_show_online_friends(panel);
			wqq_ui_qqpanel_start_poll(panel);
			return FALSE;
		case WEBQQ_PROTOCOL_FRIENDS_FAILED:
			g_error("Fail to get friends list!!!");
			break;
		case WEBQQ_PROTOCOL_GROUPS_FAILED:
			g_error("Fail to get groups list!!!");
			break;
		case WEBQQ_PROTOCOL_ONLINE_FRIENDS_FAILED:
			g_error("Fail to get online friends!!!");
			break;
		default:
			g_warning("wqq_get_friends_and_groups_list_timeout:"
					  "Unknown response!");
			break;
		}
	}
	return TRUE;
}

static gpointer wqq_send_message_thread(gpointer data)
{
	WqqUiQQPanel *panel = (WqqUiQQPanel *) data;
	WqqSession *session = panel->session;
	GAsyncQueue *queue = panel->send_queue;
	g_async_queue_ref(queue);
	gpointer popdata;
	WqqSendMessage *smsg;
	guint status;
	while ((popdata = g_async_queue_pop(queue))) {
		smsg = (WqqSendMessage *) popdata;
		status = wqq_session_send_message(session, smsg);
		if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL) {
			g_warning("Fail to send message to %lu",
					  wqq_send_message_get_to(smsg));
		} else {
			g_message("Send message to %lu",
					  wqq_send_message_get_to(smsg));
		}
		g_object_unref(smsg);
	}
	g_async_queue_unref(queue);
	return NULL;
}

/* poll消息 */
static gpointer wqq_poll_message_thread(gpointer data)
{
	WqqUiQQPanel *panel = (WqqUiQQPanel *) data;
	WqqSession *session = panel->session;
	GAsyncQueue *queue = panel->poll_queue;
	g_async_queue_ref(queue);
	WqqPollMessageList *list;
	guint status;
	gboolean first_poll = FALSE;
	while (TRUE) {
		list = wqq_poll_message_list_new();
		status = wqq_session_poll_message(session, list);
		if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL) {
			if (status != WQQ_SESSION_HTTP_STATUS_TRANSPORT_ERROR) {
				g_warning("Poll message error:%u", status);
			} else {
				g_message("No message from server!");
			}
		} else {
			if (wqq_poll_message_list_is_empty(list)) {
				g_message("No message from server!");
			} else {
				g_async_queue_push(queue, list);
				continue;
			}
			/* 第一次POLL成功后才开始发送数据，否则发送会失败 */
			if (first_poll == FALSE)
				g_thread_new("send-message", wqq_send_message_thread,
							 panel);
			first_poll = TRUE;
		}
		g_object_unref(list);
	}
	g_async_queue_unref(queue);
	return NULL;
}

static gboolean wqq_poll_message_timeout(gpointer data)
{
	WqqUiQQPanel *panel = (WqqUiQQPanel *) data;
	GAsyncQueue *queue = panel->poll_queue;
	WqqSession *session = panel->session;
	gpointer popdata;
	while ((popdata = g_async_queue_try_pop(queue))) {
		WqqPollMessageList *pmlist = (WqqPollMessageList *) popdata;

		GList *list = wqq_poll_message_list_get_glist(pmlist);
		guint64 uin, suin;
		while (list != NULL) {
			WqqPollMessage *pm = (WqqPollMessage *) list->data;
			WqqPollMessageType pmtype = wqq_poll_message_get_pmtype(pm);
			if (pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND) {
				uin = wqq_poll_message_get_from_uin(pm);
				WqqFriend *fri = wqq_session_find_friend_by_uin(session,
																uin);
				if (fri) {
					wqq_ui_qqpanel_show_friend_notification(panel, fri,
															pm);
					WqqUiQQChat *chat =
						wqq_ui_qqpanel_append_fchat(panel, fri);
					wqq_ui_qqpanel_fchat_append_pm(panel, fri, chat, pm);
					wqq_ui_qqchat_run(chat);
				} else {
					g_warning("Message from unknown friend(uin=%lu)", uin);
				}
			} else if (pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP) {
				uin = wqq_poll_message_get_from_uin(pm);
				suin = wqq_poll_message_get_send_uin(pm);
				WqqGroup *group = wqq_session_find_group_by_gid(session,
																uin);
				if (group) {
					wqq_ui_qqpanel_show_group_notification(panel, group,
														   pm);
					WqqUiQQChat *chat =
						wqq_ui_qqpanel_append_gchat(panel, group);
					WqqGroupMember *gm =
						wqq_group_find_group_member_by_uin(group, suin);
					wqq_ui_qqpanel_gchat_append_pm(panel, gm, suin, chat,
												   pm);
					wqq_ui_qqchat_run(chat);
				} else {
					g_warning("Message from unknown group(gin=%lu)", uin);
				}
			} else if (pmtype == WQQ_POLL_MESSAGE_TYPE_STATUS) {
				uin = wqq_poll_message_get_from_uin(pm);
				WqqUserStatus status = wqq_poll_message_get_status(pm);
				WqqClientType ctype = wqq_poll_message_get_ctype(pm);
				WqqFriend *fri = wqq_session_find_friend_by_uin(session,
																uin);
				if (fri) {
					WqqUiQQCell *cell = wqq_friend_get_data(fri);
					const gchar *str =
						wqq_ui_qqcell_get_status_to_string(cell);
					WqqUiQQCellStatus former_status =
						wqq_ui_qqcell_get_status(cell);
					wqq_ui_qqcell_set_status(cell, status);
					if ((former_status == WQQ_UI_QQCELL_STATUS_OFFLINE &&
						 status != WQQ_USER_STATUS_OFFLINE) ||
						(former_status != WQQ_UI_QQCELL_STATUS_OFFLINE &&
						 status == WQQ_USER_STATUS_OFFLINE))
						wqq_ui_qqpanel_reposition_qqcell(cell);
					g_message("%s(%s)",
							  wqq_ui_qqpanel_get_friend_name(fri), str);
					wqq_ui_qqpanel_show_status_notification(panel, fri,
															str);
				} else {
					g_warning("Status message from unknown friend(%lu)",
							  uin);
				}
			} else {
				g_error("Unknown poll message type!");
			}
			list = g_list_next(list);
		}
		g_object_unref(pmlist);
	}
	return TRUE;
}

/* 获取用户头像 */
static gpointer wqq_get_user_avatar_thread(gpointer data)
{
	WqqUiQQPanel *panel = (WqqUiQQPanel *) data;
	WqqSession *session = panel->session;
	GAsyncQueue *queue = panel->avatar_queue;
	GAsyncQueue *queue_r = panel->avatar_queue_r;
	gpointer popdata;
	WqqUser *user;
	gchar *path;
	guint status;
	WqqUiQQCell *cell;
	g_async_queue_ref(queue);
	g_async_queue_ref(queue_r);
	while ((popdata = g_async_queue_pop(queue))) {
		user = (WqqUser *) popdata;
		if (!WQQ_IS_USER(user))
			continue;
		path = g_strdup_printf("%s/%lu", panel->faces_dir,
							   wqq_user_get_uin(user));
		if (g_file_test(path, G_FILE_TEST_IS_REGULAR))
			goto CONTINUE;
		status = wqq_session_get_user_face(session, user, path);
		g_async_queue_lock(queue_r);
		if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL) {
			g_async_queue_push_unlocked(queue_r, (gpointer)
										WEBQQ_PROTOCOL_USER_FACE_FAILED);
		} else {
			cell = wqq_friend_get_data(WQQ_FRIEND(user));
			wqq_ui_qqcell_set_avatar_from_path_not_update(cell, path);
			g_async_queue_push_unlocked(queue_r, (gpointer)
										WEBQQ_PROTOCOL_USER_FACE);
		}
		g_async_queue_push_unlocked(queue_r, (gpointer) user);
		g_async_queue_unlock(queue_r);
	  CONTINUE:
		g_free(path);
	}
	g_async_queue_unref(queue);
	g_async_queue_unref(queue_r);
	return NULL;
}

static gpointer wqq_get_user_sign_thread(gpointer data)
{
	WqqUiQQPanel *panel = (WqqUiQQPanel *) data;
	WqqSession *session = panel->session;
	GAsyncQueue *queue = panel->sign_queue;
	GAsyncQueue *queue_r = panel->sign_queue_r;
	gpointer popdata;
	WqqUser *user;
	guint status;
	g_async_queue_ref(queue);
	g_async_queue_ref(queue_r);
	while ((popdata = g_async_queue_pop(queue))) {
		user = (WqqUser *) popdata;
		if (!WQQ_IS_USER(user))
			continue;
		status = wqq_session_get_user_sign(session, user);
		g_async_queue_lock(queue_r);
		if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL) {
			g_async_queue_push_unlocked(queue_r, (gpointer)
										WEBQQ_PROTOCOL_USER_SIGN_FAILED);
		} else {
			g_async_queue_push_unlocked(queue_r, (gpointer)
										WEBQQ_PROTOCOL_USER_SIGN);
		}
		g_async_queue_push_unlocked(queue_r, (gpointer) user);
		g_async_queue_unlock(queue_r);
	}
	g_async_queue_unref(queue);
	g_async_queue_unref(queue_r);
	return NULL;
}

static gboolean wqq_get_user_avatar_and_sign_timeout(gpointer data)
{
	WqqUiQQPanel *panel = (WqqUiQQPanel *) data;
	GAsyncQueue *avatar_queue = panel->avatar_queue_r;
	GAsyncQueue *sign_queue = panel->sign_queue_r;
	gpointer popdata;
	WqqUser *user;
	WqqUiQQCell *cell;
	guint count = 12;

	while ((popdata = g_async_queue_try_pop(avatar_queue))) {
		switch ((gulong) popdata) {
		case WEBQQ_PROTOCOL_USER_FACE:
			user = g_async_queue_pop(avatar_queue);
			cell = wqq_friend_get_data(WQQ_FRIEND(user));
			wqq_ui_qqcell_update_avatar(cell);
			g_message("Get user face(%lu)", wqq_user_get_uin(user));
			break;
		case WEBQQ_PROTOCOL_USER_FACE_FAILED:
			user = g_async_queue_pop(avatar_queue);
			g_warning("Fail to get user face(%lu)",
					  wqq_user_get_uin(user));
			break;
		default:
			g_warning("wqq_get_user_avatar_and_sign_timeout:"
					  "Unknown response!!!");
			break;
		}
		count--;
		if (count == 0)
			goto OUT;
	}

	while ((popdata = g_async_queue_try_pop(sign_queue))) {
		switch ((gulong) popdata) {
		case WEBQQ_PROTOCOL_USER_SIGN:
			user = g_async_queue_pop(sign_queue);
			cell = wqq_friend_get_data(WQQ_FRIEND(user));
			wqq_ui_qqcell_set_sign(cell, wqq_user_get_sign(user));
			g_message("Get user sign(%lu):%s",
					  wqq_user_get_uin(user), wqq_user_get_sign(user));
			break;
		case WEBQQ_PROTOCOL_USER_SIGN_FAILED:
			user = g_async_queue_pop(sign_queue);
			g_warning("Fail to get user(%lu) sign",
					  wqq_user_get_uin(user));
			break;
		default:
			g_warning("wqq_get_user_avatar_and_sign_timeout:"
					  "Unknown response!!!");
			break;
		}
		count--;
		if (count == 0)
			goto OUT;
	}

  OUT:
	return TRUE;
}

/* 请求获取用户头像的线程 */
static gpointer wqq_request_user_avatar_and_sign(gpointer data)
{
	WqqUiQQPanel *panel = (WqqUiQQPanel *) data;
	GAsyncQueue *avatar_queue = panel->avatar_queue;
	GAsyncQueue *sign_queue = panel->sign_queue;
	WqqSession *session = panel->session;
	GList *lp = wqq_session_get_category_list(session);
	g_async_queue_ref(avatar_queue);
	g_async_queue_ref(sign_queue);
	while (lp) {
		WqqCategory *category = (WqqCategory *) lp->data;
		GList *flist = wqq_category_get_friends_list(category);
		while (flist) {
			WqqFriend *fri = (WqqFriend *) flist->data;
			if (wqq_user_get_status(WQQ_USER(fri)) !=
				WQQ_USER_STATUS_OFFLINE) {
				g_async_queue_push(avatar_queue, (gpointer) fri);
				g_async_queue_push(sign_queue, (gpointer) fri);
			}
			flist = g_list_next(flist);
		}
		lp = g_list_next(lp);
	}
	lp = wqq_session_get_category_list(session);
	while (lp) {
		WqqCategory *category = (WqqCategory *) lp->data;
		GList *flist = wqq_category_get_friends_list(category);
		while (flist) {
			WqqFriend *fri = (WqqFriend *) flist->data;
			if (wqq_user_get_status(WQQ_USER(fri)) ==
				WQQ_USER_STATUS_OFFLINE) {
				g_async_queue_push(avatar_queue, (gpointer) fri);
				g_async_queue_push(sign_queue, (gpointer) fri);
			}
			flist = g_list_next(flist);
		}
		lp = g_list_next(lp);
	}
	g_async_queue_unref(avatar_queue);
	g_async_queue_unref(sign_queue);
	return NULL;
}

/* 获取群信息 */
static gpointer wqq_get_group_info_thread(gpointer data)
{
	WqqUiQQPanel *panel = (WqqUiQQPanel *) data;
	GAsyncQueue *queue = panel->group_queue;
	WqqSession *session = panel->session;
	GList *glist = wqq_session_get_glist(session);
	WqqGroup *group;
	guint status;
	while (glist) {
		group = glist->data;
		status = wqq_session_get_group_info(session, group);
		if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL) {
			g_warning("Fail to get group(%s) info",
					  wqq_ui_qqpanel_get_group_name(group));
		} else {
			g_message("Get group(%s) info: ",
					  wqq_ui_qqpanel_get_group_name(group));
			GList *gmlist = wqq_group_get_members(group);
			while (gmlist) {
				WqqGroupMember *member = gmlist->data;
				if (!g_strcmp0("WQ", wqq_user_get_nick(WQQ_USER(member))))
					g_message("%lu - %s(%s)",
							  wqq_user_get_uin(WQQ_USER(member)),
							  wqq_user_get_nick(WQQ_USER(member)),
							  wqq_group_member_get_card(member));
				gmlist = g_list_next(gmlist);
			}
		}
		glist = g_list_next(glist);
	}
	return NULL;
}


/********************************************************
 * PUBLIC
 ********************************************************/
WqqUiQQPanel *wqq_ui_qqpanel_new(WqqSession * session)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), NULL);
	WqqUiQQPanel *panel =
		(WqqUiQQPanel *) g_object_new(WQQ_TYPE_UI_QQPANEL,
									  "session", session,
									  NULL);
	WqqUser *user = wqq_session_get_user_self(session);
	panel->user_dir = g_strdup_printf("%s/%lu",
									  panel->data_dir,
									  wqq_user_get_uin(user));
	panel->faces_dir =
		g_strdup_printf("%s/qchat/faces", g_get_user_cache_dir());
	g_mkdir_with_parents(panel->faces_dir, 0777);
	wqq_ui_qqpanel_set_nick(panel, wqq_user_get_nick(user));
	wqq_ui_qqpanel_set_status(panel, wqq_user_get_status(user));
	panel->avatar_file = g_strdup_printf("%s/avatar", panel->user_dir);
	wqq_ui_qqpanel_copy_default_face(panel->user_dir);

	wqq_ui_qqpanel_set_avatar_from_file(panel, panel->avatar_file);


	g_thread_new("set-status", wqq_set_status_thread, panel);
	g_thread_new("get-self-avatar-and-sign",
				 wqq_get_self_avatar_and_sign_thread, panel);
	g_thread_new("get-friends-and-groups-list",
				 wqq_get_friends_and_groups_list_thread, panel);
	g_timeout_add(2500, wqq_get_self_avatar_and_sign_timeout, panel);
	g_timeout_add(2000, wqq_get_friends_and_groups_list_timeout, panel);

	return panel;
}

void wqq_ui_qqpanel_run(WqqUiQQPanel * panel)
{
	g_return_if_fail(WQQ_IS_UI_QQPANEL(panel));
	gtk_widget_show_all(GTK_WIDGET(panel));
}

void wqq_ui_qqpanel_set_status(WqqUiQQPanel * panel, WqqUserStatus status)
{
	g_return_if_fail(WQQ_IS_UI_QQPANEL(panel));
	switch (status) {
	case WQQ_USER_STATUS_ONLINE:
		gtk_button_set_image(GTK_BUTTON(panel->status),
							 gtk_image_new_from_file_at_size
							 (WQQ_DATA_STATUS_ONLINE_FILE,
							  WQQ_UI_QQPANEL_STATUS_ICON_SIZE,
							  WQQ_UI_QQPANEL_STATUS_ICON_SIZE));
		break;
	case WQQ_USER_STATUS_BUSY:
		gtk_button_set_image(GTK_BUTTON(panel->status),
							 gtk_image_new_from_file_at_size
							 (WQQ_DATA_STATUS_BUSY_FILE,
							  WQQ_UI_QQPANEL_STATUS_ICON_SIZE,
							  WQQ_UI_QQPANEL_STATUS_ICON_SIZE));
		break;
	case WQQ_USER_STATUS_AWAY:
		gtk_button_set_image(GTK_BUTTON(panel->status),
							 gtk_image_new_from_file_at_size
							 (WQQ_DATA_STATUS_AWAY_FILE,
							  WQQ_UI_QQPANEL_STATUS_ICON_SIZE,
							  WQQ_UI_QQPANEL_STATUS_ICON_SIZE));
		break;
	case WQQ_USER_STATUS_HIDDEN:
		gtk_button_set_image(GTK_BUTTON(panel->status),
							 gtk_image_new_from_file_at_size
							 (WQQ_DATA_STATUS_HIDDEN_FILE,
							  WQQ_UI_QQPANEL_STATUS_ICON_SIZE,
							  WQQ_UI_QQPANEL_STATUS_ICON_SIZE));
		break;
	case WQQ_USER_STATUS_CALLME:
		gtk_button_set_image(GTK_BUTTON(panel->status),
							 gtk_image_new_from_file_at_size
							 (WQQ_DATA_STATUS_CALLME_FILE,
							  WQQ_UI_QQPANEL_STATUS_ICON_SIZE,
							  WQQ_UI_QQPANEL_STATUS_ICON_SIZE));
		break;
	case WQQ_USER_STATUS_SILENT:
		gtk_button_set_image(GTK_BUTTON(panel->status),
							 gtk_image_new_from_file_at_size
							 (WQQ_DATA_STATUS_SILENT_FILE,
							  WQQ_UI_QQPANEL_STATUS_ICON_SIZE,
							  WQQ_UI_QQPANEL_STATUS_ICON_SIZE));
		break;
	case WQQ_USER_STATUS_OFFLINE:
		gtk_button_set_image(GTK_BUTTON(panel->status),
							 gtk_image_new_from_file_at_size
							 (WQQ_DATA_STATUS_OFFLINE_FILE,
							  WQQ_UI_QQPANEL_STATUS_ICON_SIZE,
							  WQQ_UI_QQPANEL_STATUS_ICON_SIZE));
		break;
	default:
		break;
	}
}

void wqq_ui_qqpanel_set_avatar_from_file(WqqUiQQPanel * panel,
										 const gchar * filepath)
{
	g_return_if_fail(WQQ_IS_UI_QQPANEL(panel));
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filepath, NULL);
	if (pixbuf == NULL)
		pixbuf =
			gdk_pixbuf_new_from_file(WQQ_DATA_FACE_DEFAULT_FILE, NULL);
	gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(panel->avatar),
									gtk_image_new_from_pixbuf(pixbuf));
	gtk_widget_show_all(panel->avatar);
	if (pixbuf != NULL)
		g_object_unref(pixbuf);
}

void wqq_ui_qqpanel_set_nick(WqqUiQQPanel * panel, const gchar * nick)
{
	g_return_if_fail(WQQ_IS_UI_QQPANEL(panel));
	gtk_label_set_text(GTK_LABEL(panel->nick), nick);
}

void wqq_ui_qqpanel_set_sign(WqqUiQQPanel * panel, const gchar * sign)
{
	g_return_if_fail(WQQ_IS_UI_QQPANEL(panel));
	gchar *s = g_str_replace_c(sign, '\n', " ");
	gtk_label_set_text(GTK_LABEL(panel->sign), s);
	gtk_widget_set_tooltip_text(panel->sign, sign);
	g_free(s);
}
