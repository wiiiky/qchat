#include "wqquicaptcha.h"

enum {
	WQQ_UI_CAPTCHA_PROPERTY_SESSION = 1,
	WQQ_UI_CAPTCHA_PROPERTY_FILEPATH,
};

G_DEFINE_TYPE(WqqUiCaptcha, wqq_ui_captcha, GTK_TYPE_DIALOG);

static void wqq_ui_captcha_getter(GObject * object, guint property_id,
								  GValue * value, GParamSpec * pspec);
static void wqq_ui_captcha_setter(GObject * object, guint property_id,
								  const GValue * value,
								  GParamSpec * pspec);

static gboolean on_ui_captcha_image_pressed(GtkWidget * ebox,
											GdkEventButton * btn,
											gpointer data);
static void on_session_get_captcha_image_callback(SoupSession * session,
												  SoupMessage * msg,
												  gpointer data);

static void wqq_ui_captcha_init(WqqUiCaptcha * captcha)
{
	captcha->session = NULL;
	captcha->filepath = NULL;
	gtk_widget_set_size_request(GTK_WIDGET(captcha), 100, -1);
	gtk_dialog_add_buttons(GTK_DIALOG(captcha),
						   _("OK"), GTK_RESPONSE_OK,
						   _("Cancel"), GTK_RESPONSE_CANCEL, NULL);
	GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(captcha));
	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_container_set_border_width(GTK_CONTAINER(vbox), 10);
	gtk_box_pack_start(GTK_BOX(content), vbox, TRUE, TRUE, 0);

	GtkWidget *event_box = gtk_event_box_new();
	captcha->ebox = event_box;
	captcha->signal_handler =
		g_signal_connect(G_OBJECT(event_box), "button-press-event",
						 G_CALLBACK(on_ui_captcha_image_pressed), captcha);
	captcha->image = gtk_image_new_from_file("");
	gtk_container_add(GTK_CONTAINER(event_box), captcha->image);
	gtk_box_pack_start(GTK_BOX(vbox), event_box, TRUE, TRUE, 0);

	captcha->entry = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(vbox), captcha->entry, FALSE, FALSE, 0);
	captcha->ok_btn =
		gtk_dialog_get_widget_for_response(GTK_DIALOG(captcha),
										   GTK_RESPONSE_OK);
	captcha->cancel_btn =
		gtk_dialog_get_widget_for_response(GTK_DIALOG(captcha),
										   GTK_RESPONSE_CANCEL);
	g_signal_connect_swapped(G_OBJECT(captcha->entry), "activate",
							 G_CALLBACK(gtk_button_clicked),
							 captcha->ok_btn);

	gtk_widget_show_all(vbox);
}

static void wqq_ui_captcha_finalize(GObject * object)
{
	WqqUiCaptcha *captcha = WQQ_UI_CAPTCHA(object);
	g_free(captcha->filepath);
}

static void wqq_ui_captcha_class_init(WqqUiCaptchaClass * klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = wqq_ui_captcha_setter;
	object_class->get_property = wqq_ui_captcha_getter;
	object_class->finalize = wqq_ui_captcha_finalize;

	GParamSpec *ps;
	ps = g_param_spec_pointer("session",
							  "wqq session",
							  "WqqSession",
							  G_PARAM_READABLE |
							  G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY);
	g_object_class_install_property(object_class,
									WQQ_UI_CAPTCHA_PROPERTY_SESSION, ps);

	ps = g_param_spec_string("filepath",
							 "captcha image filepath",
							 "Captcha Image Filepath",
							 NULL,
							 G_PARAM_READABLE |
							 G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY);
	g_object_class_install_property(object_class,
									WQQ_UI_CAPTCHA_PROPERTY_FILEPATH, ps);
}

static void wqq_ui_captcha_getter(GObject * object, guint property_id,
								  GValue * value, GParamSpec * pspec)
{
	WqqUiCaptcha *captcha = WQQ_UI_CAPTCHA(object);
	switch (property_id) {
	case WQQ_UI_CAPTCHA_PROPERTY_SESSION:
		g_value_set_pointer(value, captcha->session);
		break;
	case WQQ_UI_CAPTCHA_PROPERTY_FILEPATH:
		g_value_set_string(value, captcha->filepath);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void wqq_ui_captcha_setter(GObject * object, guint property_id,
								  const GValue * value, GParamSpec * pspec)
{
	WqqUiCaptcha *captcha = WQQ_UI_CAPTCHA(object);
	switch (property_id) {
	case WQQ_UI_CAPTCHA_PROPERTY_SESSION:
		captcha->session = g_value_get_pointer(value);
		break;
	case WQQ_UI_CAPTCHA_PROPERTY_FILEPATH:
		g_free(captcha->filepath);
		captcha->filepath = g_strdup(g_value_get_string(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static gboolean on_ui_captcha_image_pressed(GtkWidget * ebox,
											GdkEventButton * event,
											gpointer data)
{
	WqqUiCaptcha *captcha = (WqqUiCaptcha *) data;
	if (event->type == GDK_2BUTTON_PRESS) {
		g_signal_handler_block(captcha->ebox, captcha->signal_handler);
		gtk_image_set_from_stock(GTK_IMAGE(captcha->image),
								 GTK_STOCK_INFO, GTK_ICON_SIZE_BUTTON);
		wqq_session_get_captcha_image_async(captcha->session,
											captcha->filepath,
											on_session_get_captcha_image_callback,
											captcha);
		return TRUE;
	}
	return FALSE;
}

static void on_session_get_captcha_image_callback(SoupSession * session,
												  SoupMessage * msg,
												  gpointer data)
{
	WqqUiCaptcha *captcha = (WqqUiCaptcha *) data;
	guint status;
	g_object_get(msg, "status-code", &status, NULL);
	if (!SOUP_STATUS_IS_SUCCESSFUL(status)) {
		gtk_image_set_from_stock(GTK_IMAGE(captcha->image),
								 GTK_STOCK_NO, GTK_ICON_SIZE_BUTTON);
	} else {
		gtk_image_set_from_file(GTK_IMAGE(captcha->image),
								captcha->filepath);
	}
	g_signal_handler_unblock(captcha->ebox, captcha->signal_handler);
}

/************************************************************
 * PUBLIC
 ***********************************************************/
WqqUiCaptcha *wqq_ui_captcha_new(WqqSession * session,
								 const gchar * filepath)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session) &&
						 g_file_test(filepath, G_FILE_TEST_IS_REGULAR),
						 NULL);
	WqqUiCaptcha *captcha =
		(WqqUiCaptcha *) g_object_new(WQQ_TYPE_UI_CAPTCHA,
									  "session", session,
									  "filepath", filepath,
									  NULL);
	gtk_image_set_from_file(GTK_IMAGE(captcha->image), filepath);
	return captcha;
}

gchar *wqq_ui_captcha_run(WqqUiCaptcha * captcha)
{
	gint result = gtk_dialog_run(GTK_DIALOG(captcha));
	if (result == GTK_RESPONSE_OK)
		return g_strdup(gtk_entry_get_text(GTK_ENTRY(captcha->entry)));
	return NULL;
}
