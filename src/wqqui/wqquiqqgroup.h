#ifndef __WQQ_UI_QQ_GROUP_H__
#define __WQQ_UI_QQ_GROUP_H__

#include <gtk/gtk.h>
G_BEGIN_DECLS
#define WQQ_TYPE_UI_QQGROUP	wqq_ui_qqgroup_get_type()
#define WQQ_UI_QQGROUP(obj) G_TYPE_CHECK_INSTANCE_CAST((obj),\
			WQQ_TYPE_UI_QQGROUP,WqqUiQQGroup)
#define WQQ_IS_UI_QQGROUP(obj)	G_TYPE_CHECK_INSTANCE_TYPE((obj),\
			WQQ_TYPE_UI_QQGROUP)
#define WQQ_UI_QQGROUP_CLASS(klass)	G_TYPE_CHECK_CLASS_CAST((klass),\
			WQQ_TYPE_UI_QQGROUP,WqqUiQQGroupClass)
#define WQQ_IS_UI_QQGROUP_CLASS(klass)	G_TYPE_CHECK_CLASS_TYPE((klass),\
			WQQ_TYPE_UI_QQGROUP)
#define WQQ_UI_QQGROUP_GET_CLASS(obj)	G_TYPE_INSTANCE_GET_CLASS((obj),\
			WQQ_TYPE_UI_QQGROUP,WqqUiQQGroupClass)
typedef struct _WqqUiQQGroup WqqUiQQGroup;
typedef struct _WqqUiQQGroupClass WqqUiQQGroupClass;

struct _WqqUiQQGroup {
	GtkEventBox parent;
	GtkWidget *image;
	GtkWidget *label;
	gchar *name;
	gchar *markname;
	gpointer data;
};

struct _WqqUiQQGroupClass {
	GtkEventBoxClass parent;
};

GType wqq_ui_qqgroup_get_type() G_GNUC_CONST;
WqqUiQQGroup *wqq_ui_qqgroup_new(const gchar * name,
								 const gchar * markname);
void wqq_ui_qqgroup_set_name(WqqUiQQGroup * group, const gchar * name);
void wqq_ui_qqgroup_set_markname(WqqUiQQGroup * group,
								 const gchar * markname);
void wqq_ui_qqgroup_set_image(WqqUiQQGroup * group, GdkPixbuf * pixbuf);
void wqq_ui_qqgroup_set_image_from_file(WqqUiQQGroup * group,
										const gchar * file);
const gchar *wqq_ui_qqgroup_get_name(WqqUiQQGroup * group);
const gchar *wqq_ui_qqgroup_get_markname(WqqUiQQGroup * group);
void wqq_ui_qqgroup_set_data(WqqUiQQGroup * group, gpointer data);
gpointer wqq_ui_qqgroup_get_data(WqqUiQQGroup * group);

G_END_DECLS
#endif
