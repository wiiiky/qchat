#ifndef __WQQ_QQCHAT_H__
#define __WQQ_QQCHAT_H__

#include <webkit/webkit.h>
#include "wqqqqchatmessage.h"

G_BEGIN_DECLS
#define WQQ_TYPE_QQCHAT	(wqq_qqchat_get_type())
#define WQQ_QQCHAT(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_QQCHAT,WqqQQChat))
#define WQQ_IS_QQCHAT(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_QQCHAT))
#define WQQ_QQCHAT_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WQQ_TYPE_QQCHAT,WqqQQChatClass))
#define WQQ_IS_QQCHAT_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass),\
				WQQ_TYPE_QQCHAT))
#define WQQ_QQCHAT_GET_CLASS(obj)	(G_TYPE_GET_INSTANCE_CLASS((obj),\
				WQQ_TYPE_QQCHAT,WqqQQChatClass))
typedef struct _WqqQQChat WqqQQChat;
typedef struct _WqqQQChatClass WqqQQChatClass;

struct _WqqQQChat {
	GtkScrolledWindow parent;
	/*GtkWidget *viewport; */
	GtkWidget *webview;
	gboolean loading;
};

struct _WqqQQChatClass {
	GtkScrolledWindowClass parent_class;
};


GType wqq_qqchat_get_type() G_GNUC_CONST;

WqqQQChat *wqq_qqchat_new(const gchar * base_uri, const gchar * css_file);

void wqq_qqchat_set_base_uri(WqqQQChat * chat, const gchar * base);

void wqq_qqchat_append_time(WqqQQChat * chat, GDateTime * dt);
void wqq_qqchat_append_time_now(WqqQQChat * chat);

void wqq_qqchat_append_message_from_buddy(WqqQQChat * chat,
										  const gchar * avatar,
										  WqqQQChatMessage * msg);

void wqq_qqchat_append_message_from_self(WqqQQChat * chat,
										 const gchar * avatar,
										 WqqQQChatMessage * msg);

G_END_DECLS
#endif
