#ifndef __WQQ_POLL_MESSAGE_LIST_H__
#define __WQQ_POLL_MESSAGE_LIST_H__
#include "wqqpollmessage.h"

G_BEGIN_DECLS
#define WQQ_TYPE_POLL_MESSAGE_LIST	(wqq_poll_message_list_get_type())
#define WQQ_POLL_MESSAGE_LIST(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_POLL_MESSAGE_LIST,WqqPollMessageList))
#define WQQ_IS_POLL_MESSAGE_LIST(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_POLL_MESSAGE_LIST))
#define WQQ_POLL_MESSAGE_LIST_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST(\
				(klass),WQQ_TYPE_POLL_MESSAGE_LIST,WqqPollMessageListClass))
#define WQQ_IS_POLL_MESSAGE_LIST_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE(\
				(klass),WQQ_TYPE_POLL_MESSAGE_LIST))
#define WQQ_POLL_MESSAGE_LIST_GET_CLASS(obj)	(G_TYPE_GET_INSTANCE_CLASS(\
				(obj),WQQ_TYPE_POLL_MESSAGE_LIST,WqqPollMessageListClass))
typedef struct _WqqPollMessageList WqqPollMessageList;
typedef struct _WqqPollMessageListClass WqqPollMessageListClass;

struct _WqqPollMessageList {
	GObject parent;
	GList *list;
};

struct _WqqPollMessageListClass {
	GObjectClass parent_class;
};

GType wqq_poll_message_list_get_type() G_GNUC_CONST;
WqqPollMessageList *wqq_poll_message_list_new();
void wqq_poll_message_list_append(WqqPollMessageList * ml,
								  WqqPollMessage * pmsg);
GList *wqq_poll_message_list_get_glist(WqqPollMessageList * ml);
void wqq_poll_message_list_clear(WqqPollMessageList * ml);
gboolean wqq_poll_message_list_is_empty(WqqPollMessageList * ml);
void wqq_poll_message_list_sort_by_time(WqqPollMessageList * ml);

G_END_DECLS
#endif
