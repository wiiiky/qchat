#ifndef __GTK_QQHEADER_H__
#define __GTK_QQHEADER_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS
#define WQQ_TYPE_UI_QQHEADER	(wqq_ui_qqheader_get_type())
#define WQQ_UI_QQHEADER(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_UI_QQHEADER,WqqUiQQHeader))
#define WQQ_IS_UI_QQHEADER(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_UI_QQHEADER))
#define WQQ_UI_QQHEADER_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WQQ_TYPE_UI_QQHEADER,WqqUiQQHeaderClass))
#define WQQ_IS_UI_QQHEADER_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass),\
				WQQ_TYPE_UI_QQHEADER))
#define WQQ_UI_QQHEADER_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj),\
				WQQ_TYPE_UI_QQHEADER,WqqUiQQHeaderClass))
typedef struct _WqqUiQQHeader WqqUiQQHeader;
typedef struct _WqqUiQQHeaderClass WqqUiQQHeaderClass;
struct _WqqUiQQHeader {
	GtkBox box;
	GtkWidget *title;
	GtkWidget *ratio;
	guint online;
	guint total;
};

struct _WqqUiQQHeaderClass {
	GtkBoxClass parent_class;
};

GType wqq_ui_qqheader_get_type() G_GNUC_CONST;

/* creators */
WqqUiQQHeader *wqq_ui_qqheader_new();
WqqUiQQHeader *wqq_ui_qqheader_new_full(const gchar * title,
										guint online, guint total);

void wqq_ui_qqheader_set_title(WqqUiQQHeader * header,
							   const gchar * title);
void wqq_ui_qqheader_set_online(WqqUiQQHeader * header, guint online);
void wqq_ui_qqheader_set_total(WqqUiQQHeader * header, guint total);
const gchar *wqq_ui_qqheader_get_title(WqqUiQQHeader * header);
guint wqq_ui_qqheader_get_online(WqqUiQQHeader * header);
guint wqq_ui_qqheader_get_total(WqqUiQQHeader * header);

G_END_DECLS
#endif
