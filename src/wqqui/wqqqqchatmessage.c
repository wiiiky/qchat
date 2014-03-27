#include "wqqqqchatmessage.h"

WqqQQChatMessageContent
	* wqq_qqchat_message_content_new(WqqQQChatMessageType type,
									 gconstpointer data)
{
	WqqQQChatMessageContent *ct = g_new(WqqQQChatMessageContent, 1);
	ct->type = type;
	switch (type) {
	case WQQ_QQCHAT_MESSAGE_TYPE_TEXT:
		ct->text = g_strdup((gchar *) data);
		break;
	case WQQ_QQCHAT_MESSAGE_TYPE_IMAGE:
		ct->image = g_strdup((gchar *) data);
		break;
	case WQQ_QQCHAT_MESSAGE_TYPE_FACE:
		ct->face = (gulong) data;
		break;
	default:
		g_free(ct);
		return NULL;
	}
	return ct;
}

void wqq_qqchat_message_content_free(WqqQQChatMessageContent * ct)
{
	g_return_if_fail(ct != NULL);
	switch (ct->type) {
	case WQQ_QQCHAT_MESSAGE_TYPE_TEXT:
		g_free(ct->text);
		break;
	case WQQ_QQCHAT_MESSAGE_TYPE_IMAGE:
		g_free(ct->image);
		break;
	default:
		break;
	}
	g_free(ct);
}

WqqQQChatMessage *wqq_qqchat_message_new(const gchar * nick)
{
	WqqQQChatMessage *msg =
		(WqqQQChatMessage *) g_new(WqqQQChatMessage, 1);
	msg->nick = g_strdup(nick);
	msg->list = NULL;
	return msg;
}

WqqQQChatMessage *wqq_qqchat_message_new_with_content(const gchar * nick,
													  WqqQQChatMessageType
													  type,
													  gconstpointer data)
{
	WqqQQChatMessage *msg = wqq_qqchat_message_new(nick);
	WqqQQChatMessageContent *ct =
		wqq_qqchat_message_content_new(type, data);
	if (ct == NULL)
		return msg;
	msg->list = g_list_append(msg->list, ct);
	return msg;
}

void wqq_qqchat_message_append_message(WqqQQChatMessage * msg,
									   WqqQQChatMessageType type,
									   gconstpointer data)
{
	WqqQQChatMessageContent *ct =
		wqq_qqchat_message_content_new(type, data);
	g_return_if_fail(ct != NULL);
	msg->list = g_list_append(msg->list, ct);
}

void wqq_qqchat_message_append_text(WqqQQChatMessage * msg,
									const gchar * text)
{
	WqqQQChatMessageContent *ct =
		wqq_qqchat_message_content_new(WQQ_QQCHAT_MESSAGE_TYPE_TEXT, text);
	msg->list = g_list_append(msg->list, ct);
}

void wqq_qqchat_message_append_image(WqqQQChatMessage * msg,
									 const gchar * image)
{
	WqqQQChatMessageContent *ct =
		wqq_qqchat_message_content_new(WQQ_QQCHAT_MESSAGE_TYPE_IMAGE,
									   image);
	msg->list = g_list_append(msg->list, ct);
}

void wqq_qqchat_message_append_face(WqqQQChatMessage * msg, gulong face)
{
	WqqQQChatMessageContent *ct =
		wqq_qqchat_message_content_new(WQQ_QQCHAT_MESSAGE_TYPE_FACE,
									   (gconstpointer) face);
	msg->list = g_list_append(msg->list, ct);
}

void wqq_qqchat_message_free(WqqQQChatMessage * msg)
{
	g_return_if_fail(msg != NULL);
	g_list_free_full(msg->list,
					 (GDestroyNotify) wqq_qqchat_message_content_free);

	g_free(msg->nick);
	g_free(msg);
}
