#include "wqqpollmessagelist.h"

G_DEFINE_TYPE(WqqPollMessageList, wqq_poll_message_list, G_TYPE_OBJECT);

static void wqq_poll_message_list_getter(GObject * object,
										 guint property_id, GValue * value,
										 GParamSpec * pspec)
{
}

static void wqq_poll_message_list_setter(GObject * object,
										 guint property_id,
										 const GValue * value,
										 GParamSpec * pspec)
{
}

static void wqq_poll_message_list_init(WqqPollMessageList * ml)
{
	ml->list = NULL;
}

static void wqq_poll_message_list_finalize(GObject * object)
{
	WqqPollMessageList *ml = WQQ_POLL_MESSAGE_LIST(object);
	g_list_free_full(ml->list, g_object_unref);
}

static void wqq_poll_message_list_class_init(WqqPollMessageListClass *
											 klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->get_property = wqq_poll_message_list_getter;
	object_class->set_property = wqq_poll_message_list_setter;
	object_class->finalize = wqq_poll_message_list_finalize;
}

/***********************************************************
 * PUBLIC
 **********************************************************/

WqqPollMessageList *wqq_poll_message_list_new()
{
	return (WqqPollMessageList *) g_object_new(WQQ_TYPE_POLL_MESSAGE_LIST,
											   NULL);
}

void wqq_poll_message_list_append(WqqPollMessageList * ml,
								  WqqPollMessage * pmsg)
{
	g_return_if_fail(WQQ_IS_POLL_MESSAGE_LIST(ml) &&
					 WQQ_IS_POLL_MESSAGE(pmsg));
	ml->list = g_list_append(ml->list, pmsg);
}

GList *wqq_poll_message_list_get_glist(WqqPollMessageList * ml)
{
	g_return_val_if_fail(WQQ_IS_POLL_MESSAGE_LIST(ml), NULL);
	return ml->list;
}

void wqq_poll_message_list_clear(WqqPollMessageList * ml)
{
	g_return_if_fail(WQQ_IS_POLL_MESSAGE_LIST(ml));
	g_list_free_full(ml->list, g_object_unref);
	ml->list = NULL;
}

gboolean wqq_poll_message_list_is_empty(WqqPollMessageList * ml)
{
	g_return_val_if_fail(WQQ_IS_POLL_MESSAGE_LIST(ml), TRUE);
	return ml->list == NULL;
}

static gint _wqq_poll_message_list_sort_by_time_func(gconstpointer a,
													 gconstpointer b)
{
	WqqPollMessage *ma = (WqqPollMessage *) a;
	WqqPollMessage *mb = (WqqPollMessage *) b;
	guint64 ta = wqq_poll_message_get_time(ma);
	guint64 tb = wqq_poll_message_get_time(mb);
	if (ta < tb)
		return -1;
	else if (ta > tb)
		return 1;
	return 0;
}

void wqq_poll_message_list_sort_by_time(WqqPollMessageList * ml)
{
	g_return_if_fail(WQQ_IS_POLL_MESSAGE_LIST(ml));
	ml->list = g_list_sort(ml->list,
						   _wqq_poll_message_list_sort_by_time_func);
}
