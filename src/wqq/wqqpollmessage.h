#ifndef __WQQ_POLL_MESSAGE_H__
#define __WQQ_POLL_MESSAGE_H__

#include "wqquser.h"
#include "wqqmessagecontent.h"

G_BEGIN_DECLS
#define WQQ_TYPE_POLL_MESSAGE	(wqq_poll_message_get_type())
#define WQQ_POLL_MESSAGE(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_POLL_MESSAGE,WqqPollMessage))
#define WQQ_IS_POLL_MESSAGE(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_POLL_MESSAGE))
#define WQQ_POLL_MESSAGE_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WQQ_TYPE_POLL_MESSAGE,WqqPollMessageClass))
#define WQQ_IS_POLL_MESSAGE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE(\
				(klass),WQQ_TYPE_POLL_MESSAGE))
#define WQQ_POLL_MESSAGE_GET_CLASS(obj)	(G_TYPE_GET_INSTANCE_CLASS((obj),\
				WQQ_TYPE_POLL_MESSAGE,WqqPollMessageClass))
typedef enum _WqqPollMessageType WqqPollMessageType;
typedef struct _WqqPollFriendMessage WqqPollFriendMessage;
typedef struct _WqqPollGroupMessage WqqPollGroupMessage;
typedef struct _WqqPollStatusMessage WqqPollStatusMessage;
typedef struct _WqqPollMessage WqqPollMessage;
typedef struct _WqqPollMessageClass WqqPollMessageClass;

enum _WqqPollMessageType {
	WQQ_POLL_MESSAGE_TYPE_FRIEND,
	WQQ_POLL_MESSAGE_TYPE_GROUP,
	WQQ_POLL_MESSAGE_TYPE_STATUS,
	WQQ_POLL_MESSAGE_TYPE_UNKNOWN,
};
#define WQQ_TYPE_POLL_MESSAGE_TYPE	(wqq_poll_message_type_get_type())
GType wqq_poll_message_type_get_type() G_GNUC_CONST;

struct _WqqPollFriendMessage {
	guint64 msg_id;
	guint64 from_uin;
	guint64 to_uin;
	guint64 msg_id2;
	guint msg_type;
	guint64 reply_ip;
	guint64 time;
};

struct _WqqPollGroupMessage {
	guint64 msg_id;
	guint64 from_uin;
	guint64 to_uin;
	guint64 msg_id2;
	guint msg_type;
	guint64 reply_ip;
	guint64 group_code;
	guint64 send_uin;
	guint64 seq;
	guint64 time;
	guint64 info_seq;
};

struct _WqqPollStatusMessage {
	guint64 uin;
	WqqUserStatus status;
	WqqClientType ctype;
};

struct _WqqPollMessage {
	GObject parent;
	WqqPollMessageType pmtype;
	union {
		WqqPollFriendMessage fri;
		WqqPollGroupMessage group;
		WqqPollStatusMessage status;
	};
	GList *contents;
};

struct _WqqPollMessageClass {
	GObjectClass parent_class;
};

GType wqq_poll_message_get_type() G_GNUC_CONST;

WqqPollMessage *wqq_poll_message_new(WqqPollMessageType pmtype);
WqqPollMessageType wqq_poll_message_get_pmtype(WqqPollMessage * pmsg);
void wqq_poll_message_set_pmtype(WqqPollMessage * pmsg,
								 WqqPollMessageType type);
void wqq_poll_message_set_friend(WqqPollMessage * pmsg,
								 guint64 msg_id, guint64 from_uin,
								 guint64 to_uin, guint64 msg_id2,
								 guint msg_type, guint64 reply_ip,
								 guint64 time);
void wqq_poll_message_set_group(WqqPollMessage * pmsg,
								guint64 msg_id, guint64 from_uin,
								guint64 to_uin, guint64 msg_id2,
								guint msg_type, guint64 reply_ip,
								guint64 time, guint64 group_code,
								guint64 send_uin, guint64 seq,
								guint64 info_seq);
void wqq_poll_message_set_status(WqqPollMessage * pmsg,
								 guint64 uin, WqqUserStatus status,
								 WqqClientType ctype);
void wqq_poll_message_append_text_content(WqqPollMessage * pmsg,
										  const gchar * text);
void wqq_poll_message_append_face_content(WqqPollMessage * pmsg,
										  guint face);
void wqq_poll_message_append_pic_content(WqqPollMessage * pmsg,
										 const gchar * pic);
GList *wqq_poll_message_get_contents(WqqPollMessage * pmsg);

guint64 wqq_poll_message_get_from_uin(WqqPollMessage * pmsg);
guint64 wqq_poll_message_get_time(WqqPollMessage * pmsg);
guint64 wqq_poll_message_get_send_uin(WqqPollMessage *pmsg);
WqqUserStatus wqq_poll_message_get_status(WqqPollMessage *pmsg);
WqqClientType wqq_poll_message_get_ctype(WqqPollMessage *pmsg);

G_END_DECLS
#endif
