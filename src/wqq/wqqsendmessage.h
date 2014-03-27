#ifndef __WQQ_SEND_MESSAGE_H__
#define __WQQ_SEND_MESSAGE_H__

#include "wqqmessagecontent.h"

G_BEGIN_DECLS
#define WQQ_TYPE_SEND_MESSAGE	(wqq_send_message_get_type())
#define WQQ_SEND_MESSAGE(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_SEND_MESSAGE,WqqSendMessage))
#define WQQ_IS_SEND_MESSAGE(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_SEND_MESSAGE))
#define WQQ_SEND_MESSAGE_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WQQ_TYPE_SEND_MESSAGE,WqqSendMessageClass))
#define WQQ_IS_SEND_MESSAGE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE(\
				(klass),WQQ_TYPE_SEND_MESSAGE))
#define WQQ_SEND_MESSAGE_GET_CLASS(obj)	(G_TYPE_GET_INSTANCE_CLASS((obj),\
				WQQ_TYPE_SEND_MESSAGE,WqqSendMessageClass))
typedef enum _WqqSendMessageType WqqSendMessageType;
typedef struct _WqqSendMessage WqqSendMessage;
typedef struct _WqqSendFriendMessage WqqSendFriendMessage;
typedef struct _WqqSendGroupMessage WqqSendGroupMessage;
typedef struct _WqqSendMessageClass WqqSendMessageClass;

enum _WqqSendMessageType {
	WQQ_SEND_MESSAGE_TYPE_FRIEND,
	WQQ_SEND_MESSAGE_TYPE_GROUP,
	WQQ_SEND_MESSAGE_TYPE_UNKNOWN,
};
GType wqq_send_message_type_get_type() G_GNUC_CONST;
#define WQQ_TYPE_SEND_MESSAGE_TYPE	(wqq_send_message_type_get_type())

struct _WqqSendFriendMessage {
	guint64 to;
	guint face;
	guint64 msg_id;
};
struct _WqqSendGroupMessage {
	guint64 group_uin;
	guint64 msgg_id;
};

struct _WqqSendMessage {
	GObject parent;
	WqqSendMessageType smtype;
	union {
		WqqSendFriendMessage fri;
		WqqSendGroupMessage group;
	};
	GList *list;
};

struct _WqqSendMessageClass {
	GObjectClass parent_class;
};

GType wqq_send_message_get_type() G_GNUC_CONST;

WqqSendMessage *wqq_send_message_new(WqqSendMessageType smtype);
void wqq_send_message_set_smtype(WqqSendMessage * sm,
								 WqqSendMessageType smtype);
void wqq_send_message_set_msg_id(WqqSendMessage * sm, guint64 msg_id);
void wqq_send_message_set_face(WqqSendMessage * sm, guint face);
void wqq_send_message_set_to(WqqSendMessage * sm, guint64 to_uin);
WqqSendMessageType wqq_send_message_get_smtype(WqqSendMessage * sm);
guint64 wqq_send_message_get_to(WqqSendMessage * sm);
guint wqq_send_message_get_face(WqqSendMessage * sm);
guint64 wqq_send_message_get_msg_id(WqqSendMessage * sm);
GList *wqq_send_message_get_contents(WqqSendMessage * sm);

void wqq_send_message_append_text_content(WqqSendMessage * sm,
										  const gchar * text);
void wqq_send_message_append_face_content(WqqSendMessage * sm, guint face);
void wqq_send_message_append_pic_content(WqqSendMessage * sm,
										 const gchar * pic);


G_END_DECLS
#endif
