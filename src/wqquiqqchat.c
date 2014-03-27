#include "wqqutils.h"
#include "wqquiqqchat.h"
#include <string.h>

G_DEFINE_TYPE(WqqUiQQChat, wqq_ui_qqchat, GTK_TYPE_WINDOW);

static void wqq_ui_qqchat_getter(GObject * object, guint propety_id,
								 GValue * value, GParamSpec * ps);
static void wqq_ui_qqchat_setter(GObject * object, guint propety_id,
								 const GValue * value, GParamSpec * ps);

static void wqq_ui_qqchat_init(WqqUiQQChat * chat)
{
	WqqQQChat *qqchat = wqq_qqchat_new("/", WQQ_DATA_CSS_QQCHAT_FILE);
	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_window_set_default_size(GTK_WINDOW(chat), 480, 360);
	gtk_window_set_position(GTK_WINDOW(chat), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(box), 8);
	gtk_container_add(GTK_CONTAINER(chat), box);

	gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(qqchat), TRUE, TRUE, 0);

	GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
	gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, FALSE, 0);

	GtkWidget *textview = gtk_entry_new();
	/*GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL); */
	/*gtk_container_add(GTK_CONTAINER(scrolled), textview); */
	/*gtk_widget_set_size_request(scrolled, -1, 50); */
	gtk_box_pack_start(GTK_BOX(hbox), textview, TRUE, TRUE, 0);

	GtkWidget *button = gtk_button_new_with_label(_("Send"));
	gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, TRUE, 0);
	gtk_widget_apply_css_from_path(box, WQQ_DATA_CSS_CHAT_FILE);

	chat->chat = qqchat;
	chat->textview = textview;
	chat->button = button;
	chat->uin = 0;
	chat->data = NULL;
	chat->data1 = NULL;
	chat->data2 = NULL;
	chat->time = NULL;

	g_signal_connect_swapped(G_OBJECT(textview), "activate",
							 G_CALLBACK(gtk_button_clicked), button);
}

static void wqq_ui_qqchat_finalize(GObject * object)
{
	WqqUiQQChat *chat = (WqqUiQQChat *) object;
	g_free(chat->time);
}

static void wqq_ui_qqchat_class_init(WqqUiQQChatClass * klass)
{
	GObjectClass *objclass = G_OBJECT_CLASS(klass);
	objclass->set_property = wqq_ui_qqchat_setter;
	objclass->get_property = wqq_ui_qqchat_getter;
	objclass->finalize = wqq_ui_qqchat_finalize;
}

static void wqq_ui_qqchat_getter(GObject * object, guint propety_id,
								 GValue * value, GParamSpec * ps)
{
}

static void wqq_ui_qqchat_setter(GObject * object, guint propety_id,
								 const GValue * value, GParamSpec * ps)
{
}


/******************************************************
 * PUBLIC
 ******************************************************/
WqqUiQQChat *wqq_ui_qqchat_new(const gchar * base)
{
	WqqUiQQChat *chat =
		(WqqUiQQChat *) g_object_new(WQQ_TYPE_UI_QQCHAT, NULL);
	wqq_qqchat_set_base_uri(chat->chat, base);

	return chat;
}

void wqq_ui_qqchat_set_data(WqqUiQQChat * chat, gpointer data)
{
	g_return_if_fail(WQQ_IS_UI_QQCHAT(chat));
	chat->data = data;
}

gpointer wqq_ui_qqchat_get_data(WqqUiQQChat * chat)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCHAT(chat), NULL);
	return chat->data;
}

void wqq_ui_qqchat_set_data1(WqqUiQQChat * chat, gpointer data)
{
	g_return_if_fail(WQQ_IS_UI_QQCHAT(chat));
	chat->data1 = data;
}

gpointer wqq_ui_qqchat_get_data1(WqqUiQQChat * chat)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCHAT(chat), NULL);
	return chat->data1;
}

void wqq_ui_qqchat_set_data2(WqqUiQQChat * chat, gpointer data)
{
	g_return_if_fail(WQQ_IS_UI_QQCHAT(chat));
	chat->data2 = data;
}

gpointer wqq_ui_qqchat_get_data2(WqqUiQQChat * chat)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCHAT(chat), NULL);
	return chat->data2;
}

void wqq_ui_qqchat_set_send_callback(WqqUiQQChat * chat,
									 GCallback callback, gpointer data)
{
	g_return_if_fail(WQQ_IS_UI_QQCHAT(chat));
	g_signal_connect(G_OBJECT(chat->button), "clicked",
					 G_CALLBACK(callback), data);
}

GtkWidget *wqq_ui_qqchat_get_textview(WqqUiQQChat * chat)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCHAT(chat), NULL);
	return chat->textview;
}

const gchar *wqq_ui_qqchat_get_textview_text(WqqUiQQChat * chat)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCHAT(chat), NULL);
	/*return gtk_text_view_get_text(GTK_TEXT_VIEW(chat->textview)); */
	return gtk_entry_get_text(GTK_ENTRY(chat->textview));
}

void wqq_ui_qqchat_clear_textview_text(WqqUiQQChat * chat)
{
	g_return_if_fail(WQQ_IS_UI_QQCHAT(chat));
	/*gtk_text_view_clear_text(GTK_TEXT_VIEW(chat->textview)); */
	gtk_entry_set_text(GTK_ENTRY(chat->textview), "");
}

void wqq_ui_qqchat_set_title(WqqUiQQChat * chat, const gchar * title)
{
	g_return_if_fail(WQQ_IS_UI_QQCHAT(chat));
	gtk_window_set_title(GTK_WINDOW(chat), title);
}

void wqq_ui_qqchat_run(WqqUiQQChat * chat)
{
	g_return_if_fail(chat);
	gtk_widget_show_all(GTK_WIDGET(chat));
}

void wqq_ui_qqchat_hide(WqqUiQQChat * chat)
{
	g_return_if_fail(chat);
	gtk_widget_hide(GTK_WIDGET(chat));
}

void wqq_ui_qqchat_append_message_from_self(WqqUiQQChat * chat,
											const gchar * avatar,
											WqqQQChatMessage * msg)
{
	g_return_if_fail(WQQ_IS_UI_QQCHAT(chat));
	wqq_qqchat_append_message_from_self(chat->chat, avatar, msg);
}

void wqq_ui_qqchat_append_message_from_other(WqqUiQQChat * chat,
											 const gchar * avatar,
											 WqqQQChatMessage * msg)
{
	g_return_if_fail(WQQ_IS_UI_QQCHAT(chat));
	wqq_qqchat_append_message_from_buddy(chat->chat, avatar, msg);
}

void wqq_ui_qqchat_set_uin(WqqUiQQChat * chat, guint64 uin)
{
	g_return_if_fail(WQQ_IS_UI_QQCHAT(chat));
	chat->uin = uin;
}

guint64 wqq_ui_qqchat_get_uin(WqqUiQQChat * chat)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCHAT(chat), 0);
	return chat->uin;
}

void wqq_ui_qqchat_append_time_now(WqqUiQQChat * chat)
{
	g_return_if_fail(WQQ_IS_UI_QQCHAT(chat));
	GDateTime *dt = g_date_time_new_now_local();
	gchar *now = g_date_time_format(dt, "%m-%d %H:%M");
	if (g_strcmp0(chat->time, now) == 0) {
		g_free(now);
		return;
	}
	g_free(chat->time);
	chat->time = now;
	wqq_qqchat_append_time(chat->chat, dt);
	g_date_time_unref(dt);
}
