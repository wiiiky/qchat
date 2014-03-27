#include "wqqpollmessage.h"

enum {
	WQQ_POLL_MESSAGE_PROPERTY_TYPE = 1,
	WQQ_POLL_MESSAGE_PROPERTY_MSG_ID,
	WQQ_POLL_MESSAGE_PROPERTY_FROM_UIN,
	WQQ_POLL_MESSAGE_PROPERTY_TO_UIN,
	WQQ_POLL_MESSAGE_PROPERTY_MSG_ID2,
	WQQ_POLL_MESSAGE_PROPERTY_MSG_TYPE,
	WQQ_POLL_MESSAGE_PROPERTY_REPLY_IP,
	WQQ_POLL_MESSAGE_PROPERTY_TIME,
	WQQ_POLL_MESSAGE_PROPERTY_GROUP_CODE,
	WQQ_POLL_MESSAGE_PROPERTY_SEND_UIN,
	WQQ_POLL_MESSAGE_PROPERTY_SEQ,
	WQQ_POLL_MESSAGE_PROPERTY_INFO_SEQ,
	WQQ_POLL_MESSAGE_PROPERTY_UIN,
	WQQ_POLL_MESSAGE_PROPERTY_STATUS,
	WQQ_POLL_MESSAGE_PROPERTY_CTYPE,
	WQQ_POLL_MESSAGE_PROPERTY_CONTENTS,
};

G_DEFINE_TYPE(WqqPollMessage, wqq_poll_message, G_TYPE_OBJECT);

static void wqq_poll_message_getter(GObject * object, guint property_id,
									GValue * value, GParamSpec * pspec);
static void wqq_poll_message_setter(GObject * object, guint property_id,
									const GValue * value,
									GParamSpec * psepc);
static void wqq_poll_message_finalize(GObject * object);

static void wqq_poll_message_init(WqqPollMessage * pmsg)
{
	pmsg->contents = NULL;
	pmsg->pmtype = WQQ_POLL_MESSAGE_TYPE_FRIEND;
}

static void wqq_poll_message_finalize(GObject * object)
{
	WqqPollMessage *pm = WQQ_POLL_MESSAGE(object);
	g_list_free_full(pm->contents, g_object_unref);
}

static void wqq_poll_message_class_init(WqqPollMessageClass * klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->get_property = wqq_poll_message_getter;
	object_class->set_property = wqq_poll_message_setter;
	object_class->finalize = wqq_poll_message_finalize;

	GParamSpec *ps;
	ps = g_param_spec_enum("type",
						   "poll message type",
						   "Poll Message Type",
						   WQQ_TYPE_POLL_MESSAGE_TYPE,
						   WQQ_POLL_MESSAGE_TYPE_FRIEND,
						   G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_POLL_MESSAGE_PROPERTY_TYPE, ps);

	ps = g_param_spec_uint64("msg_id",
							 "poll message msg_id",
							 "Poll Message Msg_id",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_POLL_MESSAGE_PROPERTY_MSG_ID, ps);

	ps = g_param_spec_uint64("from_uin",
							 "poll message from_uin",
							 "Poll Message From_uin",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_POLL_MESSAGE_PROPERTY_FROM_UIN,
									ps);

	ps = g_param_spec_uint64("to_uin",
							 "poll message to_uin",
							 "Poll Message To_uin",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_POLL_MESSAGE_PROPERTY_TO_UIN, ps);

	ps = g_param_spec_uint64("msg_id2",
							 "poll message msg_id2",
							 "Poll Message Msg_id2",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_POLL_MESSAGE_PROPERTY_MSG_ID2, ps);

	ps = g_param_spec_uint("msg_type",
						   "poll message msg_type",
						   "Poll Message Msg_type",
						   0, G_MAXUINT,
						   0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_POLL_MESSAGE_PROPERTY_MSG_TYPE,
									ps);

	ps = g_param_spec_uint64("reply_ip",
							 "poll message reply_ip",
							 "Poll Message Reply_ip",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_POLL_MESSAGE_PROPERTY_REPLY_IP,
									ps);

	ps = g_param_spec_uint64("time",
							 "poll message time",
							 "Poll Message Time",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_POLL_MESSAGE_PROPERTY_TIME, ps);

	ps = g_param_spec_uint64("group_code",
							 "poll message group_code",
							 "Poll Message Group_code",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_POLL_MESSAGE_PROPERTY_GROUP_CODE,
									ps);

	ps = g_param_spec_uint64("send_uin",
							 "poll message send_uin",
							 "Poll Message Send_uin",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_POLL_MESSAGE_PROPERTY_SEND_UIN,
									ps);

	ps = g_param_spec_uint64("seq",
							 "poll message seq",
							 "Poll Message Seq",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_POLL_MESSAGE_PROPERTY_SEQ, ps);

	ps = g_param_spec_uint64("info_seq",
							 "poll message info_seq",
							 "Poll Message Info_seq",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_POLL_MESSAGE_PROPERTY_INFO_SEQ,
									ps);

	ps = g_param_spec_uint64("uin",
							 "poll message uin",
							 "Poll Message Uin",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_POLL_MESSAGE_PROPERTY_UIN, ps);

	ps = g_param_spec_enum("status",
						   "poll message status",
						   "Poll Message Status",
						   WQQ_TYPE_USER_STATUS,
						   WQQ_USER_STATUS_ONLINE,
						   G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_POLL_MESSAGE_PROPERTY_STATUS, ps);

	ps = g_param_spec_enum("ctype",
						   "poll message ctype",
						   "Poll Message Ctype",
						   WQQ_TYPE_CLIENT_TYPE,
						   WQQ_CLIENT_TYPE_PC,
						   G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_POLL_MESSAGE_PROPERTY_CTYPE, ps);

	ps = g_param_spec_pointer("contents",
							  "poll message contents",
							  "Poll Message Contents", G_PARAM_READABLE);
	g_object_class_install_property(object_class,
									WQQ_POLL_MESSAGE_PROPERTY_CONTENTS,
									ps);
}

GType wqq_poll_message_type_get_type()
{
	static GType etype = 0;
	if (G_UNLIKELY(etype == 0)) {
		static const GEnumValue values[] = {
			{WQQ_POLL_MESSAGE_TYPE_FRIEND,
			 "WQQ_POLL_MESSAGE_TYPE_FRIEND",
			 "friend"},
			{WQQ_POLL_MESSAGE_TYPE_GROUP,
			 "WQQ_POLL_MESSAGE_TYPE_GROUP",
			 "group"},
			{WQQ_POLL_MESSAGE_TYPE_STATUS,
			 "WQQ_POLL_MESSAGE_TYPE_STATUS",
			 "status"},
			{WQQ_POLL_MESSAGE_TYPE_UNKNOWN,
			 "WQQ_POLL_MESSAGE_TYPE_UNKNOWN",
			 "unknown"},
			{0, NULL, NULL}
		};
		etype =
			g_enum_register_static(g_intern_static_string
								   ("WqqPollMessageType"), values);
	}
	return etype;
}

static void wqq_poll_message_getter(GObject * object, guint property_id,
									GValue * value, GParamSpec * pspec)
{
	WqqPollMessage *pmsg = WQQ_POLL_MESSAGE(object);
	switch (property_id) {
	case WQQ_POLL_MESSAGE_PROPERTY_TYPE:
		g_value_set_enum(value, pmsg->pmtype);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_MSG_ID:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND)
			g_value_set_uint64(value, pmsg->fri.msg_id);
		else if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			g_value_set_uint64(value, pmsg->group.msg_id);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_FROM_UIN:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND)
			g_value_set_uint64(value, pmsg->fri.from_uin);
		else if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			g_value_set_uint64(value, pmsg->group.from_uin);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_TO_UIN:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND)
			g_value_set_uint64(value, pmsg->fri.to_uin);
		else if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			g_value_set_uint64(value, pmsg->group.to_uin);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_MSG_ID2:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND)
			g_value_set_uint64(value, pmsg->fri.msg_id2);
		else if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			g_value_set_uint64(value, pmsg->group.msg_id2);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_MSG_TYPE:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND)
			g_value_set_uint64(value, pmsg->fri.msg_type);
		else if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			g_value_set_uint64(value, pmsg->group.msg_type);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_REPLY_IP:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND)
			g_value_set_uint64(value, pmsg->fri.reply_ip);
		else if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			g_value_set_uint64(value, pmsg->group.reply_ip);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_TIME:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND)
			g_value_set_uint64(value, pmsg->fri.time);
		else if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			g_value_set_uint64(value, pmsg->group.time);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_GROUP_CODE:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			g_value_set_uint64(value, pmsg->group.group_code);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_SEND_UIN:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			g_value_set_uint64(value, pmsg->group.send_uin);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_SEQ:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			g_value_set_uint64(value, pmsg->group.seq);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_INFO_SEQ:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			g_value_set_uint64(value, pmsg->group.info_seq);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_UIN:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_STATUS)
			g_value_set_uint64(value, pmsg->status.uin);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_STATUS:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_STATUS)
			g_value_set_enum(value, pmsg->status.status);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_CTYPE:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_STATUS)
			g_value_set_enum(value, pmsg->status.ctype);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_CONTENTS:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND ||
			pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			g_value_set_pointer(value, pmsg->contents);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void wqq_poll_message_setter(GObject * object, guint property_id,
									const GValue * value,
									GParamSpec * pspec)
{
	WqqPollMessage *pmsg = WQQ_POLL_MESSAGE(object);
	switch (property_id) {
	case WQQ_POLL_MESSAGE_PROPERTY_TYPE:
		pmsg->pmtype = g_value_get_enum(value);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_MSG_ID:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND)
			pmsg->fri.msg_id = g_value_get_uint64(value);
		else if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			pmsg->group.msg_id = g_value_get_uint64(value);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_FROM_UIN:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND)
			pmsg->fri.from_uin = g_value_get_uint64(value);
		else if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			pmsg->group.from_uin = g_value_get_uint64(value);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_TO_UIN:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND)
			pmsg->fri.to_uin = g_value_get_uint64(value);
		else if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			pmsg->group.to_uin = g_value_get_uint64(value);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_MSG_ID2:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND)
			pmsg->fri.msg_id2 = g_value_get_uint64(value);
		else if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			pmsg->group.msg_id2 = g_value_get_uint64(value);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_MSG_TYPE:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND)
			pmsg->fri.msg_type = g_value_get_uint(value);
		else if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			pmsg->group.msg_type = g_value_get_uint(value);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_REPLY_IP:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND)
			pmsg->fri.reply_ip = g_value_get_uint64(value);
		else if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			pmsg->group.reply_ip = g_value_get_uint64(value);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_TIME:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND)
			pmsg->fri.time = g_value_get_uint64(value);
		else if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			pmsg->group.time = g_value_get_uint64(value);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_GROUP_CODE:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			pmsg->group.group_code = g_value_get_uint64(value);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_SEND_UIN:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			pmsg->group.send_uin = g_value_get_uint64(value);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_SEQ:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			pmsg->group.seq = g_value_get_uint64(value);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_INFO_SEQ:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
			pmsg->group.info_seq = g_value_get_uint64(value);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_UIN:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_STATUS)
			pmsg->status.uin = g_value_get_uint64(value);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_STATUS:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_STATUS)
			pmsg->status.status = g_value_get_enum(value);
		break;
	case WQQ_POLL_MESSAGE_PROPERTY_CTYPE:
		if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_STATUS)
			pmsg->status.ctype = g_value_get_enum(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

/**************************************************************
 * PUBLIC
 **************************************************************/
WqqPollMessage *wqq_poll_message_new(WqqPollMessageType pmtype)
{
	WqqPollMessage *pmsg =
		WQQ_POLL_MESSAGE(g_object_new(WQQ_TYPE_POLL_MESSAGE, NULL));
	pmsg->pmtype = pmtype;
	return pmsg;
}

WqqPollMessageType wqq_poll_message_get_pmtype(WqqPollMessage * pmsg)
{
	g_return_val_if_fail(WQQ_IS_POLL_MESSAGE(pmsg),
						 WQQ_POLL_MESSAGE_TYPE_UNKNOWN);
	return pmsg->pmtype;
}

void wqq_poll_message_set_pmtype(WqqPollMessage * pmsg,
								 WqqPollMessageType type)
{
	g_return_if_fail(WQQ_IS_POLL_MESSAGE(pmsg));
	pmsg->pmtype = type;
}

void wqq_poll_message_set_friend(WqqPollMessage * pmsg,
								 guint64 msg_id, guint64 from_uin,
								 guint64 to_uin, guint64 msg_id2,
								 guint msg_type, guint64 reply_ip,
								 guint64 time)
{
	g_return_if_fail(WQQ_IS_POLL_MESSAGE(pmsg) &&
					 pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND);
	pmsg->fri.msg_id = msg_id;
	pmsg->fri.from_uin = from_uin;
	pmsg->fri.to_uin = to_uin;
	pmsg->fri.msg_id2 = msg_id2;
	pmsg->fri.msg_type = msg_type;
	pmsg->fri.reply_ip = reply_ip;
	pmsg->fri.time = time;
}

void wqq_poll_message_set_group(WqqPollMessage * pmsg,
								guint64 msg_id, guint64 from_uin,
								guint64 to_uin, guint64 msg_id2,
								guint msg_type, guint64 reply_ip,
								guint64 time, guint64 group_code,
								guint64 send_uin, guint64 seq,
								guint64 info_seq)
{
	g_return_if_fail(WQQ_IS_POLL_MESSAGE(pmsg) &&
					 pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP);
	pmsg->group.msg_id = msg_id;
	pmsg->group.from_uin = from_uin;
	pmsg->group.to_uin = to_uin;
	pmsg->group.msg_id2 = msg_id2;
	pmsg->group.msg_type = msg_type;
	pmsg->group.reply_ip = reply_ip;
	pmsg->group.time = time;
	pmsg->group.group_code = group_code;
	pmsg->group.send_uin = send_uin;
	pmsg->group.seq = seq;
	pmsg->group.info_seq = info_seq;
}

void wqq_poll_message_set_status(WqqPollMessage * pmsg,
								 guint64 uin, WqqUserStatus status,
								 WqqClientType ctype)
{
	g_return_if_fail(WQQ_IS_POLL_MESSAGE(pmsg) &&
					 pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_STATUS);
	pmsg->status.uin = uin;
	pmsg->status.status = status;
	pmsg->status.ctype = ctype;
}


void wqq_poll_message_append_text_content(WqqPollMessage * pmsg,
										  const gchar * text)
{
	g_return_if_fail(WQQ_IS_POLL_MESSAGE(pmsg));
	WqqMessageContent *mc = wqq_message_content_new_text(text);
	pmsg->contents = g_list_append(pmsg->contents, mc);
}

void wqq_poll_message_append_face_content(WqqPollMessage * pmsg,
										  guint face)
{
	g_return_if_fail(WQQ_IS_POLL_MESSAGE(pmsg));
	WqqMessageContent *mc = wqq_message_content_new_face(face);
	pmsg->contents = g_list_append(pmsg->contents, mc);
}

void wqq_poll_message_append_pic_content(WqqPollMessage * pmsg,
										 const gchar * pic)
{
	g_return_if_fail(WQQ_IS_POLL_MESSAGE(pmsg));
	WqqMessageContent *mc = wqq_message_content_new_pic(pic);
	pmsg->contents = g_list_append(pmsg->contents, mc);
}

GList *wqq_poll_message_get_contents(WqqPollMessage * pmsg)
{
	g_return_val_if_fail(WQQ_IS_POLL_MESSAGE(pmsg), NULL);
	return pmsg->contents;
}

guint64 wqq_poll_message_get_from_uin(WqqPollMessage * pmsg)
{
	g_return_val_if_fail(WQQ_IS_POLL_MESSAGE(pmsg), 0);
	if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND)
		return pmsg->fri.from_uin;
	else if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
		return pmsg->group.from_uin;
	else
	   return pmsg->status.uin;
	return 0;
}
guint64 wqq_poll_message_get_send_uin(WqqPollMessage *pmsg)
{
	g_return_val_if_fail(WQQ_IS_POLL_MESSAGE(pmsg),0);
	if(pmsg->pmtype==WQQ_POLL_MESSAGE_TYPE_GROUP)
	   return pmsg->group.send_uin;
	return 0;
}

guint64 wqq_poll_message_get_time(WqqPollMessage * pmsg)
{
	g_return_val_if_fail(WQQ_IS_POLL_MESSAGE(pmsg), 0);
	if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_FRIEND)
		return pmsg->fri.time;
	else if (pmsg->pmtype == WQQ_POLL_MESSAGE_TYPE_GROUP)
		return pmsg->group.time;
	return 0;
}
WqqUserStatus wqq_poll_message_get_status(WqqPollMessage *pmsg)
{
	g_return_val_if_fail(WQQ_IS_POLL_MESSAGE(pmsg)&&
				pmsg->pmtype==WQQ_POLL_MESSAGE_TYPE_STATUS,
				WQQ_USER_STATUS_OFFLINE);
	return pmsg->status.status;
}
WqqClientType wqq_poll_message_get_ctype(WqqPollMessage *pmsg)
{
	g_return_val_if_fail(WQQ_IS_POLL_MESSAGE(pmsg)&&
				pmsg->pmtype==WQQ_POLL_MESSAGE_TYPE_STATUS,
				WQQ_CLIENT_TYPE_PC);
	return pmsg->status.ctype;
}
