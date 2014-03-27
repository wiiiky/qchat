#ifndef __WQQ_QQCHAT_MESSAGE_H__
#define __WQQ_QQCHAT_MESSAGE_H__

#include <gtk/gtk.h>


typedef struct _WqqQQChatMessageContent WqqQQChatMessageContent;
typedef struct _WqqQQChatMessage WqqQQChatMessage;

typedef enum {
	WQQ_QQCHAT_MESSAGE_TYPE_TEXT,	//文本
	WQQ_QQCHAT_MESSAGE_TYPE_IMAGE,	//图片
	WQQ_QQCHAT_MESSAGE_TYPE_FACE	//表情
} WqqQQChatMessageType;

struct _WqqQQChatMessage {
	GList *list;
	gchar *nick;
};

struct _WqqQQChatMessageContent {
	WqqQQChatMessageType type;
	union {
		gchar *text;
		gchar *image;
		gulong face;
	};
};

WqqQQChatMessageContent
	* wqq_qqchat_message_content_new(WqqQQChatMessageType type,
									 gconstpointer data);
void wqq_qqchat_message_content_free(WqqQQChatMessageContent * ct);

WqqQQChatMessage *wqq_qqchat_message_new(const gchar * nick);
WqqQQChatMessage *wqq_qqchat_message_new_with_content(const gchar * nick,
													  WqqQQChatMessageType
													  type,
													  gconstpointer data);
void wqq_qqchat_message_append_message(WqqQQChatMessage * msg,
									   WqqQQChatMessageType type,
									   gconstpointer data);
void wqq_qqchat_message_append_text(WqqQQChatMessage * msg,
									const gchar * text);
void wqq_qqchat_message_append_image(WqqQQChatMessage * msg,
									 const gchar * image);
void wqq_qqchat_message_append_face(WqqQQChatMessage * msg, gulong face);
void wqq_qqchat_message_free(WqqQQChatMessage * msg);


#endif
