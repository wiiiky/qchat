#include "wqqmessagecontent.h"

enum {
	WQQ_MESSAGE_CONTENT_PROPERTY_TYPE = 1,
	WQQ_MESSAGE_CONTENT_PROPERTY_TEXT,
	WQQ_MESSAGE_CONTENT_PROPERTY_FACE,
	WQQ_MESSAGE_CONTENT_PROPERTY_PIC,
};

G_DEFINE_TYPE(WqqMessageContent, wqq_message_content, G_TYPE_OBJECT);

static void wqq_message_content_getter(GObject * object, guint property_id,
									   GValue * value, GParamSpec * pspec);
static void wqq_message_content_setter(GObject * object, guint property_id,
									   const GValue * value,
									   GParamSpec * pspec);
static void wqq_message_content_finalize(GObject * object);

static void wqq_message_content_init(WqqMessageContent * c)
{
	c->ctype = WQQ_MESSAGE_CONTENT_TYPE_TEXT;
	c->text = NULL;
}

static void wqq_message_content_finalize(GObject * object)
{
	WqqMessageContent *c = WQQ_MESSAGE_CONTENT(object);
	if (c->ctype == WQQ_MESSAGE_CONTENT_TYPE_TEXT)
		g_free(c->text);
	else if (c->ctype == WQQ_MESSAGE_CONTENT_TYPE_PIC)
		g_free(c->offpic);
}

static void wqq_message_content_class_init(WqqMessageContentClass * klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->get_property = wqq_message_content_getter;
	object_class->set_property = wqq_message_content_setter;
	object_class->finalize = wqq_message_content_finalize;

	GParamSpec *ps;
	ps = g_param_spec_enum("type",
						   "message content type",
						   "Message Content Type",
						   WQQ_TYPE_MESSAGE_CONTENT_TYPE,
						   WQQ_MESSAGE_CONTENT_TYPE_TEXT,
						   G_PARAM_READABLE);
	g_object_class_install_property(object_class,
									WQQ_MESSAGE_CONTENT_PROPERTY_TYPE, ps);

	ps = g_param_spec_string("text",
							 "message content type",
							 "Message Content Type",
							 NULL, G_PARAM_READABLE);
	g_object_class_install_property(object_class,
									WQQ_MESSAGE_CONTENT_PROPERTY_TYPE, ps);

	ps = g_param_spec_uint("face",
						   "message content face",
						   "Message Content Face",
						   0, G_MAXUINT, 0, G_PARAM_READABLE);
	g_object_class_install_property(object_class,
									WQQ_MESSAGE_CONTENT_PROPERTY_FACE, ps);

	ps = g_param_spec_string("pic",
							 "message content pic",
							 "Message Content Pic",
							 NULL, G_PARAM_READABLE);
	g_object_class_install_property(object_class,
									WQQ_MESSAGE_CONTENT_PROPERTY_PIC, ps);

}

static void wqq_message_content_getter(GObject * object, guint property_id,
									   GValue * value, GParamSpec * pspec)
{
	WqqMessageContent *content = WQQ_MESSAGE_CONTENT(object);
	switch (property_id) {
	case WQQ_MESSAGE_CONTENT_PROPERTY_TYPE:
		g_value_set_enum(value, content->ctype);
		break;
	case WQQ_MESSAGE_CONTENT_PROPERTY_TEXT:
		if (content->ctype == WQQ_MESSAGE_CONTENT_TYPE_TEXT)
			g_value_set_string(value, content->text);
		break;
	case WQQ_MESSAGE_CONTENT_PROPERTY_FACE:
		if (content->ctype == WQQ_MESSAGE_CONTENT_TYPE_FACE)
			g_value_set_uint(value, content->face);
		break;
	case WQQ_MESSAGE_CONTENT_PROPERTY_PIC:
		if (content->ctype == WQQ_MESSAGE_CONTENT_TYPE_PIC)
			g_value_set_string(value, content->offpic);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void wqq_message_content_setter(GObject * object, guint property_id,
									   const GValue * value,
									   GParamSpec * pspec)
{
	WqqMessageContent *content = WQQ_MESSAGE_CONTENT(object);
	switch (property_id) {
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}


GType wqq_message_content_type_get_type(void)
{
	static GType etype = 0;
	if (G_UNLIKELY(etype == 0)) {
		static const GEnumValue values[] = {
			{WQQ_MESSAGE_CONTENT_TYPE_TEXT,
			 "WQQ_MESSAGE_CONTENT_TYPE_TEXT",
			 "text"},
			{WQQ_MESSAGE_CONTENT_TYPE_FACE,
			 "WQQ_MESSAGE_CONTENT_TYPE_FACE",
			 "face"},
			{WQQ_MESSAGE_CONTENT_TYPE_PIC,
			 "WQQ_MESSAGE_CONTENT_TYPE_PIC",
			 "pic"},
			{WQQ_MESSAGE_CONTENT_TYPE_UNKNOWN,
			 "WQQ_MESSAGE_CONTENT_TYPE_UNKNOWN",
			 "unknown"},
			{0, NULL, NULL}
		};
		etype =
			g_enum_register_static(g_intern_static_string
								   ("WqqMessageContentType"), values);
	}
	return etype;
}

/****************************************************
 * PUBLIC
 ***************************************************/

WqqMessageContent *wqq_message_content_new_text(const gchar * text)
{
	WqqMessageContent *content =
		(WqqMessageContent *) g_object_new(WQQ_TYPE_MESSAGE_CONTENT, NULL);
	content->ctype = WQQ_MESSAGE_CONTENT_TYPE_TEXT;
	content->text = g_strdup(text);
	return content;
}

WqqMessageContent *wqq_message_content_new_face(guint face)
{
	WqqMessageContent *content =
		(WqqMessageContent *) g_object_new(WQQ_TYPE_MESSAGE_CONTENT, NULL);
	content->ctype = WQQ_MESSAGE_CONTENT_TYPE_FACE;
	content->face = face;
	return content;
}

WqqMessageContent *wqq_message_content_new_pic(const gchar * pic)
{
	WqqMessageContent *content =
		(WqqMessageContent *) g_object_new(WQQ_TYPE_MESSAGE_CONTENT, NULL);
	content->ctype = WQQ_MESSAGE_CONTENT_TYPE_PIC;
	content->offpic = g_strdup(pic);
	return content;
}

WqqMessageContentType wqq_message_content_get_ctype(WqqMessageContent *
													content)
{
	g_return_val_if_fail(WQQ_IS_MESSAGE_CONTENT(content),
						 WQQ_MESSAGE_CONTENT_TYPE_UNKNOWN);
	return content->ctype;
}

const gchar *wqq_message_content_get_text(WqqMessageContent * content)
{
	g_return_val_if_fail(WQQ_IS_MESSAGE_CONTENT(content), NULL);
	if (content->ctype != WQQ_MESSAGE_CONTENT_TYPE_TEXT)
		return NULL;
	return content->text;
}

guint wqq_message_content_get_face(WqqMessageContent * content)
{
	g_return_val_if_fail(WQQ_IS_MESSAGE_CONTENT(content), 0);
	if (content->ctype != WQQ_MESSAGE_CONTENT_TYPE_FACE)
		return 0;
	return content->face;
}

const gchar *wqq_message_content_get_pic(WqqMessageContent * content)
{
	g_return_val_if_fail(WQQ_IS_MESSAGE_CONTENT(content), NULL);
	if (content->ctype != WQQ_MESSAGE_CONTENT_TYPE_PIC)
		return NULL;
	return content->offpic;
}

void wqq_message_content_set_text(WqqMessageContent * content,
								  const gchar * text)
{
	g_return_if_fail(WQQ_IS_MESSAGE_CONTENT(content));
	if (content->ctype == WQQ_MESSAGE_CONTENT_TYPE_TEXT)
		g_free(content->text);
	else if (content->ctype == WQQ_MESSAGE_CONTENT_TYPE_PIC)
		g_free(content->offpic);
	content->ctype = WQQ_MESSAGE_CONTENT_TYPE_TEXT;
	content->text = g_strdup(text);
}

void wqq_message_content_set_face(WqqMessageContent * content, guint face)
{
	g_return_if_fail(WQQ_IS_MESSAGE_CONTENT(content));
	if (content->ctype == WQQ_MESSAGE_CONTENT_TYPE_TEXT)
		g_free(content->text);
	else if (content->ctype == WQQ_MESSAGE_CONTENT_TYPE_PIC)
		g_free(content->offpic);
	content->ctype = WQQ_MESSAGE_CONTENT_TYPE_FACE;
	content->face = face;
}

void wqq_message_content_set_pic(WqqMessageContent * content,
								 const gchar * pic)
{
	g_return_if_fail(WQQ_IS_MESSAGE_CONTENT(content));
	if (content->ctype == WQQ_MESSAGE_CONTENT_TYPE_TEXT)
		g_free(content->text);
	else if (content->ctype == WQQ_MESSAGE_CONTENT_TYPE_PIC)
		g_free(content->offpic);
	content->ctype = WQQ_MESSAGE_CONTENT_TYPE_PIC;
	content->offpic = g_strdup(pic);
}
