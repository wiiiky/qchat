#include "wqqsendmessage.h"

enum {
	WQQ_SEND_MESSAGE_PROPERTY_TYPE = 1,
	WQQ_SEND_MESSAGE_PROPERTY_TO,
	WQQ_SEND_MESSAGE_PROPERTY_MSG_ID,
	WQQ_SEND_MESSAGE_PROPERTY_FACE,
	WQQ_SEND_MESSAGE_PROPERTY_CONTENTS,
};

G_DEFINE_TYPE(WqqSendMessage, wqq_send_message, G_TYPE_OBJECT);

static void wqq_send_message_getter(GObject * object, guint property_id,
									GValue * value, GParamSpec * pspec);
static void wqq_send_message_setter(GObject * object, guint property_id,
									const GValue * value,
									GParamSpec * pspec);
static void wqq_send_message_finalize(GObject * object);

static void wqq_send_message_init(WqqSendMessage * sm)
{
	sm->smtype = WQQ_SEND_MESSAGE_TYPE_FRIEND;
	sm->fri.to = 0;
	sm->fri.msg_id = 0;
	sm->fri.face = 0;
	sm->list = NULL;
}

static void wqq_send_message_finalize(GObject * object)
{
	WqqSendMessage *sm = WQQ_SEND_MESSAGE(object);
	g_list_free_full(sm->list, g_object_unref);
}

static void wqq_send_message_class_init(WqqSendMessageClass * klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->get_property = wqq_send_message_getter;
	object_class->set_property = wqq_send_message_setter;
	object_class->finalize = wqq_send_message_finalize;

	GParamSpec *ps;
	ps = g_param_spec_enum("type",
						   "send message type",
						   "Send Messge Type",
						   WQQ_TYPE_SEND_MESSAGE_TYPE,
						   WQQ_SEND_MESSAGE_TYPE_FRIEND,
						   G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_SEND_MESSAGE_PROPERTY_TYPE, ps);

	ps = g_param_spec_uint64("to",
							 "send message to",
							 "Send Message To",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_SEND_MESSAGE_PROPERTY_TO, ps);

	ps = g_param_spec_uint64("msg_id",
							 "send message msg_id",
							 "Send Message Msg_id",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_SEND_MESSAGE_PROPERTY_MSG_ID, ps);

	ps = g_param_spec_uint("face",
						   "send message face",
						   "Send Message Face",
						   0, G_MAXUINT,
						   0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_SEND_MESSAGE_PROPERTY_FACE, ps);

	ps = g_param_spec_pointer("contents",
							  "send message contents",
							  "Send Message Contents", G_PARAM_READABLE);
	g_object_class_install_property(object_class,
									WQQ_SEND_MESSAGE_PROPERTY_CONTENTS,
									ps);
}

static void wqq_send_message_getter(GObject * object, guint property_id,
									GValue * value, GParamSpec * pspec)
{
	WqqSendMessage *sm = WQQ_SEND_MESSAGE(object);
	switch (property_id) {
	case WQQ_SEND_MESSAGE_PROPERTY_TYPE:
		g_value_set_enum(value, sm->smtype);
		break;
	case WQQ_SEND_MESSAGE_PROPERTY_TO:
		if (sm->smtype == WQQ_SEND_MESSAGE_TYPE_FRIEND)
			g_value_set_uint64(value, sm->fri.to);
		else if (sm->smtype == WQQ_SEND_MESSAGE_TYPE_GROUP)
			g_value_set_uint64(value, sm->group.group_uin);
		break;
	case WQQ_SEND_MESSAGE_PROPERTY_FACE:
		if (sm->smtype == WQQ_SEND_MESSAGE_TYPE_FRIEND)
			g_value_set_uint(value, sm->fri.face);
		break;
	case WQQ_SEND_MESSAGE_PROPERTY_MSG_ID:
		if (sm->smtype == WQQ_SEND_MESSAGE_TYPE_FRIEND)
			g_value_set_uint64(value, sm->fri.msg_id);
		else if (sm->smtype == WQQ_SEND_MESSAGE_TYPE_GROUP)
			g_value_set_uint64(value, sm->group.msgg_id);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void wqq_send_message_setter(GObject * object, guint property_id,
									const GValue * value,
									GParamSpec * pspec)
{
	WqqSendMessage *sm = WQQ_SEND_MESSAGE(object);
	switch (property_id) {
	case WQQ_SEND_MESSAGE_PROPERTY_TYPE:
		sm->smtype = g_value_get_enum(value);
		break;
	case WQQ_SEND_MESSAGE_PROPERTY_TO:
		if (sm->smtype == WQQ_SEND_MESSAGE_TYPE_FRIEND)
			sm->fri.to = g_value_get_uint64(value);
		else if (sm->smtype == WQQ_SEND_MESSAGE_TYPE_GROUP)
			sm->group.group_uin = g_value_get_uint64(value);
		break;
	case WQQ_SEND_MESSAGE_PROPERTY_MSG_ID:
		if (sm->smtype == WQQ_SEND_MESSAGE_TYPE_FRIEND)
			sm->fri.msg_id = g_value_get_uint64(value);
		else if (sm->smtype == WQQ_SEND_MESSAGE_TYPE_GROUP)
			sm->group.msgg_id = g_value_get_uint64(value);
		break;
	case WQQ_SEND_MESSAGE_PROPERTY_FACE:
		if (sm->smtype == WQQ_SEND_MESSAGE_TYPE_FRIEND)
			sm->fri.face = g_value_get_uint(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

GType wqq_send_message_type_get_type(void)
{
	static GType etype = 0;
	if (G_UNLIKELY(etype == 0)) {
		static const GEnumValue values[] = {
			{WQQ_SEND_MESSAGE_TYPE_FRIEND,
			 "WQQ_SEND_MESSAGE_TYPE_FRIEND",
			 "friend message"},
			{WQQ_SEND_MESSAGE_TYPE_GROUP,
			 "WQQ_SEND_MESSAGE_TYPE_GROUP",
			 "group message"},
			{WQQ_SEND_MESSAGE_TYPE_UNKNOWN,
			 "WQQ_SEND_MESSAGE_TYPE_UNKNOWN",
			 "unknown type"},
			{0, NULL, NULL}
		};
		etype =
			g_enum_register_static(g_intern_static_string
								   ("WqqSendMessageType"), values);
	}
	return etype;
}

/*****************************************************
 * PUBLIC
 * **************************************************/

WqqSendMessage *wqq_send_message_new(WqqSendMessageType smtype)
{
	WqqSendMessage *sm =
		(WqqSendMessage *) g_object_new(WQQ_TYPE_SEND_MESSAGE,
										"type", smtype,
										NULL);
	if (smtype == WQQ_SEND_MESSAGE_TYPE_FRIEND)
		sm->fri.msg_id = g_random_int();
	else
		sm->group.msgg_id = g_random_int();
	return sm;
}

void wqq_send_message_set_smtype(WqqSendMessage * sm,
								 WqqSendMessageType smtype)
{
	g_return_if_fail(WQQ_IS_SEND_MESSAGE(sm));
	sm->smtype = smtype;
}

void wqq_send_message_set_msg_id(WqqSendMessage * sm, guint64 msg_id)
{
	g_return_if_fail(WQQ_IS_SEND_MESSAGE(sm));
	if (sm->smtype == WQQ_SEND_MESSAGE_TYPE_FRIEND)
		sm->fri.msg_id = msg_id;
	else
		sm->group.msgg_id = msg_id;
}

void wqq_send_message_set_face(WqqSendMessage * sm, guint face)
{
	g_return_if_fail(WQQ_IS_SEND_MESSAGE(sm));
	if (sm->smtype == WQQ_SEND_MESSAGE_TYPE_FRIEND)
		sm->fri.face = face;
}

void wqq_send_message_set_to(WqqSendMessage * sm, guint64 to_uin)
{
	g_return_if_fail(WQQ_IS_SEND_MESSAGE(sm));
	if (sm->smtype == WQQ_SEND_MESSAGE_TYPE_FRIEND)
		sm->fri.to = to_uin;
	else
		sm->group.group_uin = to_uin;
}

WqqSendMessageType wqq_send_message_get_smtype(WqqSendMessage * sm)
{
	g_return_val_if_fail(WQQ_IS_SEND_MESSAGE(sm),
						 WQQ_SEND_MESSAGE_TYPE_UNKNOWN);
	return sm->smtype;
}

guint64 wqq_send_message_get_to(WqqSendMessage * sm)
{
	g_return_if_fail(WQQ_IS_SEND_MESSAGE(sm));
	if (sm->smtype == WQQ_SEND_MESSAGE_TYPE_FRIEND)
		return sm->fri.to;
	return sm->group.group_uin;
}

guint wqq_send_message_get_face(WqqSendMessage * sm)
{
	g_return_val_if_fail(WQQ_IS_SEND_MESSAGE(sm), 0);
	if (sm->smtype == WQQ_SEND_MESSAGE_TYPE_FRIEND)
		return sm->fri.face;
	return 0;
}

guint64 wqq_send_message_get_msg_id(WqqSendMessage * sm)
{
	g_return_val_if_fail(WQQ_IS_SEND_MESSAGE(sm), 0);
	if (sm->smtype == WQQ_SEND_MESSAGE_TYPE_FRIEND)
		return sm->fri.msg_id;
	return sm->group.msgg_id;
}

GList *wqq_send_message_get_contents(WqqSendMessage * sm)
{
	g_return_val_if_fail(WQQ_IS_SEND_MESSAGE(sm), NULL);
	return sm->list;
}


void wqq_send_message_append_text_content(WqqSendMessage * sm,
										  const gchar * text)
{
	g_return_if_fail(WQQ_IS_SEND_MESSAGE(sm));
	sm->list = g_list_append(sm->list, wqq_message_content_new_text(text));
}

void wqq_send_message_append_face_content(WqqSendMessage * sm, guint face)
{
	g_return_if_fail(WQQ_IS_SEND_MESSAGE(sm));
	sm->list = g_list_append(sm->list, wqq_message_content_new_face(face));
}

void wqq_send_message_append_pic_content(WqqSendMessage * sm,
										 const gchar * pic)
{
	g_return_if_fail(WQQ_IS_SEND_MESSAGE(sm));
	sm->list = g_list_append(sm->list, wqq_message_content_new_pic(pic));
}
