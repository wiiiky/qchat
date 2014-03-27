#ifndef __GTK_QQLIST_H__
#define __GTK_QQLIST_H__

#include "wqquiqqcategory.h"

G_BEGIN_DECLS
#define WQQ_TYPE_UI_QQLIST	(wqq_ui_qqlist_get_type())
#define WQQ_UI_QQLIST(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_UI_QQLIST,WqqUiQQList))
#define WQQ_IS_UI_QQLIST(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_UI_QQLIST))
#define WQQ_UI_QQLIST_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WQQ_TYPE_UI_QQLIST,WqqUiQQListClass))
#define WQQ_IS_UI_QQLIST_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass),\
				WQQ_TYPE_UI_QQLIST))
#define WQQ_UI_QQLIST_GET_CLASS(obj)	(G_TYPE_GET_INSTANCE_CLASS((obj),\
				WQQ_TYPE_UI_QQLIST,WqqUiQQListClass))
typedef struct _WqqUiQQList WqqUiQQList;
typedef struct _WqqUiQQListClass WqqUiQQListClass;
struct _WqqUiQQList {
	GtkBox vbox;
	GList *list;				//<WqqUiQQCategory*>
	guint count;				//当前包含多少个分组
};

struct _WqqUiQQListClass {
	GtkBoxClass parent_class;
};

GType wqq_ui_qqlist_get_type() G_GNUC_CONST;
WqqUiQQList *wqq_ui_qqlist_new(guint spacing);

WqqUiQQCategory *wqq_ui_qqlist_append_category(WqqUiQQList * list,
											   const gchar * title,
											   guint online, guint total);
gboolean wqq_ui_qqlist_remove_category(WqqUiQQList * list,
									   const WqqUiQQCategory * category);
void wqq_ui_qqlist_clear_all(WqqUiQQList * list);

G_END_DECLS
#endif
