#ifndef __WQQ_UI_QQCHAT_H__
#define __WQQ_UI_QQCHAT_H__
#include "wqq/wqq.h"
#include "wqqui/wqqqqchat.h"
#include "wqqm4.h"

G_BEGIN_DECLS
#define WQQ_TYPE_UI_QQCHAT	(wqq_ui_qqchat_get_type())
#define WQQ_UI_QQCHAT(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_UI_QQCHAT,WqqUiQQChat))
#define WQQ_UI_QQCHAT_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WQQ_TYPE_UI_QQCHAT,WqqUiQQChatClass))
#define WQQ_IS_UI_QQCHAT(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_UI_QQCHAT))
#define WQQ_IS_UI_QQCHAT_CLASS(klass)	(G_TYPE_CHECK_INSTANCE_TYPE(\
				(klass),WQQ_TYPE_UI_QQCHAT))
#define WQQ_UI_QQCHAT_GET_CLASS(obj)	(G_TYPE_GET_INSTANCE_CLASS((obj),\
				WQQ_TYPE_UI_QQCHAT,WqqUiQQChatClass))
typedef struct _WqqUiQQChat WqqUiQQChat;
typedef struct _WqqUiQQChatClass WqqUiQQChatClass;

struct _WqqUiQQChat {
	GtkWindow parent;
	WqqQQChat *chat;
	GtkWidget *textview;
	GtkWidget *button;
	guint64 uin;
	gchar *time;
	gpointer data;
	gpointer data1;
	gpointer data2;
};

struct _WqqUiQQChatClass {
	GtkWindowClass parent_class;
};

GType wqq_ui_qqchat_get_type() G_GNUC_CONST;


WqqUiQQChat *wqq_ui_qqchat_new(const gchar * base);
void wqq_ui_qqchat_set_data(WqqUiQQChat * chat, gpointer data);
gpointer wqq_ui_qqchat_get_data(WqqUiQQChat * chat);
void wqq_ui_qqchat_set_data1(WqqUiQQChat * chat, gpointer data);
gpointer wqq_ui_qqchat_get_data1(WqqUiQQChat * chat);
void wqq_ui_qqchat_set_data2(WqqUiQQChat * chat, gpointer data);
gpointer wqq_ui_qqchat_get_data2(WqqUiQQChat * chat);
void wqq_ui_qqchat_set_send_callback(WqqUiQQChat * chat,
									 GCallback callback, gpointer data);
GtkWidget *wqq_ui_qqchat_get_textview(WqqUiQQChat * chat);
const gchar *wqq_ui_qqchat_get_textview_text(WqqUiQQChat * chat);
void wqq_ui_qqchat_clear_textview_text(WqqUiQQChat * chat);
void wqq_ui_qqchat_set_title(WqqUiQQChat * chat, const gchar * title);
void wqq_ui_qqchat_run(WqqUiQQChat * chat);
void wqq_ui_qqchat_hide(WqqUiQQChat * chat);
void wqq_ui_qqchat_append_message_from_self(WqqUiQQChat * chat,
											const gchar * avatar,
											WqqQQChatMessage * msg);
void wqq_ui_qqchat_append_message_from_other(WqqUiQQChat * chat,
											 const gchar * avatar,
											 WqqQQChatMessage * msg);
void wqq_ui_qqchat_append_time_now(WqqUiQQChat * chat);
void wqq_ui_qqchat_set_uin(WqqUiQQChat * chat, guint64 uin);
guint64 wqq_ui_qqchat_get_uin(WqqUiQQChat * chat);

G_END_DECLS
#endif
