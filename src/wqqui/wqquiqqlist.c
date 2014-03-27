#include "wqquiqqlist.h"

static void wqq_ui_qqlist_getter(GObject * object, guint property_id,
								 GValue * value, GParamSpec * pspec);
static void wqq_ui_qqlist_setter(GObject * object, guint property_id,
								 const GValue * value, GParamSpec * pspec);

G_DEFINE_TYPE(WqqUiQQList, wqq_ui_qqlist, GTK_TYPE_BOX);

static void wqq_ui_qqlist_init(WqqUiQQList * list)
{
	GtkBox *box = GTK_BOX(list);

	g_object_set(box,
				 "orientation", GTK_ORIENTATION_VERTICAL,
				 "spacing", 5, NULL);
	list->list = NULL;
	list->count = 0;
}

static void wqq_ui_qqlist_finalize(GObject * object)
{
	WqqUiQQList *list = WQQ_UI_QQLIST(object);
	g_list_free(list->list);
}

static void wqq_ui_qqlist_class_init(WqqUiQQListClass * klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = wqq_ui_qqlist_setter;
	object_class->get_property = wqq_ui_qqlist_getter;
	object_class->finalize = wqq_ui_qqlist_finalize;
}

static void wqq_ui_qqlist_getter(GObject * object, guint property_id,
								 GValue * value, GParamSpec * pspec)
{
}

static void wqq_ui_qqlist_setter(GObject * object, guint property_id,
								 const GValue * value, GParamSpec * pspec)
{
}

/*************************************************
 * PUBLIC
 ************************************************/
WqqUiQQList *wqq_ui_qqlist_new(guint spacing)
{
	return (WqqUiQQList *) g_object_new(WQQ_TYPE_UI_QQLIST,
										"spacing", spacing, NULL);
}


WqqUiQQCategory *wqq_ui_qqlist_append_category(WqqUiQQList * list,
											   const gchar * title,
											   guint online, guint total)
{
	g_return_val_if_fail(WQQ_IS_UI_QQLIST(list), NULL);
	WqqUiQQCategory *category =
		wqq_ui_qqcategory_new(title, online, total);

	list->list = g_list_append(list->list, category);
	gtk_box_pack_start(GTK_BOX(list), GTK_WIDGET(category), FALSE, TRUE,
					   0);
	list->count++;

	return category;
}

gboolean wqq_ui_qqlist_remove_category(WqqUiQQList * list,
									   const WqqUiQQCategory * category)
{
	g_return_val_if_fail(WQQ_IS_UI_QQLIST(list), FALSE);
	GList *lp = g_list_find(list->list, category);
	if (lp == NULL)
		return FALSE;			// 未找到
	gtk_container_remove(GTK_CONTAINER(list), GTK_WIDGET(lp->data));
	list->list = g_list_remove(list->list, lp);
	list->count--;

	return TRUE;
}

void wqq_ui_qqlist_clear_all(WqqUiQQList * list)
{
	g_return_if_fail(WQQ_IS_UI_QQLIST(list));
	GList *lp = list->list;
	while (lp != NULL) {
		gtk_container_remove(GTK_CONTAINER(list), GTK_WIDGET(lp->data));
		lp = g_list_next(lp);
	}
	g_list_free(list->list);
	list->list = NULL;
	list->count = 0;
}
