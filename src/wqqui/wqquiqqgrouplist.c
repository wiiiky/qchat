#include "wqquiqqgrouplist.h"

G_DEFINE_TYPE(WqqUiQQGroupList, wqq_ui_qqgroup_list, GTK_TYPE_BOX);

static void wqq_ui_qqgroup_list_getter(GObject * obj, guint property_id,
									   GValue * value, GParamSpec * ps);
static void wqq_ui_qqgroup_list_setter(GObject * obj, guint property_id,
									   const GValue * value,
									   GParamSpec * ps);

static void wqq_ui_qqgroup_list_init(WqqUiQQGroupList * list)
{
	g_object_set(list,
				 "orientation", GTK_ORIENTATION_VERTICAL,
				 "spacing", 5, NULL);
	list->list = NULL;
}

static void wqq_ui_qqgroup_list_finalize(GObject * obj)
{
	WqqUiQQGroupList *list = WQQ_UI_QQGROUP_LIST(obj);
	g_list_free(list->list);
}

static void wqq_ui_qqgroup_list_class_init(WqqUiQQGroupListClass * klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = wqq_ui_qqgroup_list_setter;
	object_class->get_property = wqq_ui_qqgroup_list_getter;
	object_class->finalize = wqq_ui_qqgroup_list_finalize;
}

static void wqq_ui_qqgroup_list_getter(GObject * obj, guint property_id,
									   GValue * value, GParamSpec * ps)
{
	WqqUiQQGroupList *list = WQQ_UI_QQGROUP_LIST(obj);
}

static void wqq_ui_qqgroup_list_setter(GObject * obj, guint property_id,
									   const GValue * value,
									   GParamSpec * ps)
{
	WqqUiQQGroupList *list = WQQ_UI_QQGROUP_LIST(obj);
}

/************************************************************
 * PUBLIC
 *******************************************************/
WqqUiQQGroupList *wqq_ui_qqgroup_list_new(guint spacing)
{
	WqqUiQQGroupList *list =
		(WqqUiQQGroupList *) g_object_new(WQQ_TYPE_UI_QQGROUP_LIST,
					"spacing",spacing,NULL);
	return list;
}

WqqUiQQGroup *wqq_ui_qqgroup_list_append_group(WqqUiQQGroupList * list,
											   const gchar * name,
											   const gchar * markname,
											   const gchar * path)
{
	g_return_val_if_fail(WQQ_IS_UI_QQGROUP_LIST(list), NULL);
	WqqUiQQGroup *group = wqq_ui_qqgroup_new(name, markname);
	wqq_ui_qqgroup_set_image_from_file(group, path);
	list->list = g_list_append(list->list, group);
	gtk_box_pack_start(GTK_BOX(list), GTK_WIDGET(group), FALSE, FALSE, 0);

	return group;
}
