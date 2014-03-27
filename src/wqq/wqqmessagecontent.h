#ifndef __WQQ_MESSAGE_CONTENT_H__
#define __WQQ_MESSAGE_CONTENT_H__

#include <glib-object.h>

G_BEGIN_DECLS
#define WQQ_TYPE_MESSAGE_CONTENT	(wqq_message_content_get_type())
#define WQQ_MESSAGE_CONTENT(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_MESSAGE_CONTENT,WqqMessageContent))
#define WQQ_IS_MESSAGE_CONTENT(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_MESSAGE_CONTENT))
#define WQQ_MESSAGE_CONTENT_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((obj),\
				WQQ_TYPE_MESSAGE_CONTENT,WqqMessageContentClass))
#define WQQ_IS_MESSAGE_CONTENT_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((obj),\
				WQQ_TYPE_MESSAGE_CONTENT))
#define WQQ_MESSAGE_CONTENT_GET_CLASS(obj)	(G_TYPE_GET_INSTANCE_CLASS(\
				(obj),WQQ_TYPE_MESSAGE_CONTENT,WqqMessageContentClass))
typedef enum _WqqMessageContentType WqqMessageContentType;
typedef struct _WqqMessageContent WqqMessageContent;
typedef struct _WqqMessageContentClass WqqMessageContentClass;

enum _WqqMessageContentType {
	WQQ_MESSAGE_CONTENT_TYPE_TEXT,
	WQQ_MESSAGE_CONTENT_TYPE_FACE,
	WQQ_MESSAGE_CONTENT_TYPE_PIC,
	WQQ_MESSAGE_CONTENT_TYPE_UNKNOWN,
};
GType wqq_message_content_type_get_type() G_GNUC_CONST;
#define WQQ_TYPE_MESSAGE_CONTENT_TYPE (wqq_message_content_type_get_type())

struct _WqqMessageContent {
	GObject parent;
	WqqMessageContentType ctype;
	union {
		gchar *text;
		guint face;
		gchar *offpic;
	};
};

struct _WqqMessageContentClass {
	GObjectClass parent_class;
};

GType wqq_message_content_get_type() G_GNUC_CONST;
WqqMessageContent *wqq_message_content_new_text(const gchar * text);
WqqMessageContent *wqq_message_content_new_face(guint face);
WqqMessageContent *wqq_message_content_new_pic(const gchar * pic);

WqqMessageContentType wqq_message_content_get_ctype(WqqMessageContent *
													content);
const gchar *wqq_message_content_get_text(WqqMessageContent * content);
guint wqq_message_content_get_face(WqqMessageContent * content);
const gchar *wqq_message_content_get_pic(WqqMessageContent * content);
void wqq_message_content_set_text(WqqMessageContent * content,
								  const gchar * text);
void wqq_message_content_set_face(WqqMessageContent * content, guint face);
void wqq_message_content_set_pic(WqqMessageContent * content,
								 const gchar * pic);

G_END_DECLS
#endif
