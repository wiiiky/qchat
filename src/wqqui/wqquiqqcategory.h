#ifndef __GTK_QQCATEGORY_H__
#define __GTK_QQCATEGORY_H__

#include "wqquiqqcell.h"
#include "wqquiqqheader.h"

G_BEGIN_DECLS
#define WQQ_TYPE_UI_QQCATEGORY	(wqq_ui_qqcategory_get_type())
#define WQQ_UI_QQCATEGORY(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_UI_QQCATEGORY,WqqUiQQCategory))
#define WQQ_IS_UI_QQCATEGORY(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_UI_QQCATEGORY))
#define WQQ_UI_QQCATEGORY_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WQQ_TYPE_UI_QQCATEGORY,WqqUiQQCategoryClass))
#define WQQ_IS_UI_QQCATEGORY_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass),\
				WQQ_TYPE_UI_QQCATEGORY))
#define WQQ_UI_QQCATEGORY_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj),\
				WQQ_TYPE_UI_QQCATEGORY,WqqUiQQCategoryClass))
typedef struct _WqqUiQQCategory WqqUiQQCategory;
typedef struct _WqqUiQQCategoryClass WqqUiQQCategoryClass;
struct _WqqUiQQCategory {
	GtkExpander parent;
	WqqUiQQHeader *header;
	GtkWidget *box;
	GList *list;				/* WqqUiQQCell */
	guint count;

	/* background-color when mouse hovering */
	GdkColor hover_color;
};

struct _WqqUiQQCategoryClass {
	GtkExpanderClass parent_class;
};

GType wqq_ui_qqcategory_get_type() G_GNUC_CONST;

WqqUiQQCategory *wqq_ui_qqcategory_new(const gchar * title,
									   guint online, guint total);

void wqq_ui_qqcategory_set_title(WqqUiQQCategory * category,
								 const gchar * title);
void wqq_ui_qqcategory_set_online(WqqUiQQCategory * category,
								  guint online);
void wqq_ui_qqcategory_set_total(WqqUiQQCategory * category, guint total);
void wqq_ui_qqcategory_set_hover_color(WqqUiQQCategory * category,
									   guint red, guint green, guint blue);

const gchar *wqq_ui_qqcategory_get_title(WqqUiQQCategory * category);
guint wqq_ui_qqcategory_get_online(WqqUiQQCategory * category);
guint wqq_ui_qqcategory_get_total(WqqUiQQCategory * category);

/* 
 * 根据GtkQQCellInfo结构提供的信息创建一个WqqUiQQCell对象，
 * 并添加到WqqUiQQCategory当中。并返回所创建的WqqUiQQCell对象
 */
WqqUiQQCell *wqq_ui_qqcategory_append_cell(WqqUiQQCategory * category,
										   GdkPixbuf * avatar,
										   const gchar * nick,
										   const gchar * remark,
										   WqqUiQQCellStatus status,
										   const gchar * sign,
										   GdkPixbuf * client);
WqqUiQQCell *wqq_ui_qqcategory_append_cell_from_file(WqqUiQQCategory *
													 category,
													 const gchar * avatar,
													 const gchar * nick,
													 const gchar * remark,
													 WqqUiQQCellStatus
													 status,
													 const gchar * sign,
													 const gchar * client);
/*
 * 从WqqUiQQCategory当中删除指定的WqqUiQQCell
 * 成功返回TRUE
 * 失败返回FALSE(所指定的WqqUiQQCell不存在)
 */
gboolean wqq_ui_qqcategory_remove_cell(WqqUiQQCategory * category,
									   WqqUiQQCell * cell);

gint wqq_ui_qqcategory_get_count(WqqUiQQCategory * category);
void wqq_ui_qqcategory_count_online_and_total(WqqUiQQCategory * category);
void wqq_ui_qqcategory_refirst_qqcell(WqqUiQQCategory * category,
									  WqqUiQQCell * cell);
void wqq_ui_qqcategory_relast_qqcell(WqqUiQQCategory * category,
									 WqqUiQQCell * cell);
void wqq_ui_qqcategory_reorder_qqcell(WqqUiQQCategory * category);
void wqq_ui_qqcategory_reposition_qqcell(WqqUiQQCategory *category,
			WqqUiQQCell *cell);

G_END_DECLS
#endif
