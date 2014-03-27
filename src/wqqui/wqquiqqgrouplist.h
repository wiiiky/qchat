#ifndef __WQQ_UI_QQGROUP_LIST_H__
#define __WQQ_UI_QQGROUP_LIST_H__
#include "wqquiqqgroup.h"

G_BEGIN_DECLS
#define WQQ_TYPE_UI_QQGROUP_LIST	wqq_ui_qqgroup_list_get_type()
#define WQQ_UI_QQGROUP_LIST(obj) G_TYPE_CHECK_INSTANCE_CAST((obj),\
			WQQ_TYPE_UI_QQGROUP_LIST,WqqUiQQGroupList)
#define WQQ_IS_UI_QQGROUP_LIST(obj)	G_TYPE_CHECK_INSTANCE_TYPE((obj),\
			WQQ_TYPE_UI_QQGROUP_LIST)
#define WQQ_UI_QQGROUP_LIST_CLASS(klass)	G_TYPE_CHECK_CLASS_CAST(\
			(klass),WQQ_TYPE_UI_QQGROUP_LIST,WqqUiQQGroupListClass)
#define WQQ_IS_UI_QQGROUP_LIST_CLASS(klass)	G_TYPE_CHECK_CLASS_TYPE(\
			(klass),WQQ_TYPE_UI_QQGROUP_LIST)
#define WQQ_UI_QQGROUP_LIST_GET_CLASS(obj)	G_TYPE_INSTANCE_GET_CLASS(\
			(obj),WQQ_TYPE_UI_QQGROUP_LIST,WqqUiQQGroupListClass)
typedef struct _WqqUiQQGroupList WqqUiQQGroupList;
typedef struct _WqqUiQQGroupListClass WqqUiQQGroupListClass;

struct _WqqUiQQGroupList {
	GtkBox parent;
	GList *list;
	gpointer data;
};

struct _WqqUiQQGroupListClass {
	GtkBoxClass parent_class;
};

GType wqq_ui_qqgroup_list_get_type() G_GNUC_CONST;
WqqUiQQGroupList *wqq_ui_qqgroup_list_new(guint spacing);
WqqUiQQGroup *wqq_ui_qqgroup_list_append_group(WqqUiQQGroupList * list,
											   const gchar * name,
											   const gchar * markname,
											   const gchar * path);

G_END_DECLS
#endif
