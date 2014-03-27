#include "wqquiqqcategory.h"

enum {
	GTK_QQCATEGORY_PROPERTY_TITLE = 1,	//gchar *
	GTK_QQCATEGORY_PROPERTY_ONLINE,	//guint
	GTK_QQCATEGORY_PROPERTY_TOTAL,	//guint
	GTK_QQCATEGORY_PROPERTY_HOVER_COLOR,	//GdkColor*
};

G_DEFINE_TYPE(WqqUiQQCategory, wqq_ui_qqcategory, GTK_TYPE_EXPANDER);

static void wqq_ui_qqcategory_getter(GObject * object, guint property_id,
									 GValue * value, GParamSpec * pspec);
static void wqq_ui_qqcategory_setter(GObject * object, guint property_id,
									 const GValue * value,
									 GParamSpec * pspec);
static void wqq_ui_qqcategory_finalize(GObject * object);

static gboolean on_mouse_notify(GtkWidget * widget,
								GdkEventCrossing * event, gpointer data);

static void wqq_ui_qqcategory_init(WqqUiQQCategory * category)
{
	category->header = wqq_ui_qqheader_new();
	category->box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 3);
	category->list = NULL;
	category->count = 0;
	category->hover_color.red = 204;
	category->hover_color.green = 205;
	category->hover_color.blue = 255;

}

static void wqq_ui_qqcategory_finalize(GObject * object)
{
	WqqUiQQCategory *category = WQQ_UI_QQCATEGORY(object);
	g_list_free(category->list);
}

static void wqq_ui_qqcategory_class_init(WqqUiQQCategoryClass * klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->get_property = wqq_ui_qqcategory_getter;
	object_class->set_property = wqq_ui_qqcategory_setter;
	object_class->finalize = wqq_ui_qqcategory_finalize;

	GParamSpec *pspec;
	pspec = g_param_spec_string("title",
								"header title",
								"the title of the header",
								NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(G_OBJECT_CLASS(klass),
									GTK_QQCATEGORY_PROPERTY_TITLE, pspec);

	pspec = g_param_spec_uint("online",
							  "online count",
							  "the count of online friends",
							  0, G_MAXUINT,
							  0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(G_OBJECT_CLASS(klass),
									GTK_QQCATEGORY_PROPERTY_ONLINE, pspec);

	pspec = g_param_spec_uint("total",
							  "total count",
							  "the count of total friends",
							  0, G_MAXUINT,
							  0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(G_OBJECT_CLASS(klass),
									GTK_QQCATEGORY_PROPERTY_TOTAL, pspec);

	pspec = g_param_spec_boxed("hover-color",
							   "hover color",
							   "backgroudn color when mouse hover",
							   GDK_TYPE_COLOR,
							   G_PARAM_READABLE | G_PARAM_WRITABLE);

	g_object_class_install_property(object_class,
									GTK_QQCATEGORY_PROPERTY_HOVER_COLOR,
									pspec);
}

static void wqq_ui_qqcategory_getter(GObject * object, guint property_id,
									 GValue * value, GParamSpec * pspec)
{
	WqqUiQQCategory *category = WQQ_UI_QQCATEGORY(object);
	switch (property_id) {
	case GTK_QQCATEGORY_PROPERTY_TITLE:
		g_value_set_string(value,
						   wqq_ui_qqheader_get_title(WQQ_UI_QQHEADER
													 (category->header)));
		break;
	case GTK_QQCATEGORY_PROPERTY_TOTAL:
		g_value_set_uint(value,
						 wqq_ui_qqheader_get_total(WQQ_UI_QQHEADER
												   (category->header)));
		break;
	case GTK_QQCATEGORY_PROPERTY_ONLINE:
		g_value_set_uint(value,
						 wqq_ui_qqheader_get_online(WQQ_UI_QQHEADER
													(category->header)));
		break;
	case GTK_QQCATEGORY_PROPERTY_HOVER_COLOR:
		g_value_set_boxed(value, &category->hover_color);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
	}
}

static void wqq_ui_qqcategory_setter(GObject * object, guint property_id,
									 const GValue * value,
									 GParamSpec * pspec)
{
	WqqUiQQCategory *category = WQQ_UI_QQCATEGORY(object);
	GdkColor *color;
	switch (property_id) {
	case GTK_QQCATEGORY_PROPERTY_TITLE:
		wqq_ui_qqheader_set_title(WQQ_UI_QQHEADER(category->header),
								  g_value_get_string(value));
		break;
	case GTK_QQCATEGORY_PROPERTY_ONLINE:
		wqq_ui_qqheader_set_online(WQQ_UI_QQHEADER(category->header),
								   g_value_get_uint(value));
		break;
	case GTK_QQCATEGORY_PROPERTY_TOTAL:
		wqq_ui_qqheader_set_total(WQQ_UI_QQHEADER(category->header),
								  g_value_get_uint(value));
		break;
	case GTK_QQCATEGORY_PROPERTY_HOVER_COLOR:
		color = g_value_get_boxed(value);
		category->hover_color.red = color->red;
		category->hover_color.green = color->green;
		category->hover_color.blue = color->blue;
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
	}
}

static gboolean on_mouse_notify(GtkWidget * widget,
								GdkEventCrossing * event, gpointer data)
{
	static GtkCssProvider *css1 = NULL, *css2 = NULL;
	if (css1 == NULL) {
		gchar *str;
		WqqUiQQCategory *category = WQQ_UI_QQCATEGORY(data);
		GdkColor *color = &category->hover_color;
		str = g_strdup_printf("GtkEventBox{border-radius:6px;"
							  "background-color:rgba(%u,%u,%u,255);}",
							  color->red, color->green, color->blue);
		css1 = gtk_css_provider_new();
		css2 = gtk_css_provider_new();
		gtk_css_provider_load_from_data(css1, str, -1, NULL);
		g_free(str);
		str = g_strdup("GtkEventBox{border-radius:6px;"
					   "background-color:rgba(255,255,255,0);}");
		gtk_css_provider_load_from_data(css2, str, -1, NULL);
		g_free(str);
	}
	if (event->type == GDK_ENTER_NOTIFY) {
		gtk_style_context_add_provider(gtk_widget_get_style_context
									   (widget), GTK_STYLE_PROVIDER(css1),
									   G_MAXUINT);
	} else if (event->type == GDK_LEAVE_NOTIFY) {
		gtk_style_context_add_provider(gtk_widget_get_style_context
									   (widget), GTK_STYLE_PROVIDER(css2),
									   G_MAXUINT);
	}
	return FALSE;
}

/**********************************************
 * PUBLIC
 *********************************************/

WqqUiQQCategory *wqq_ui_qqcategory_new(const gchar * title,
									   guint online, guint total)
{
	WqqUiQQCategory *category = g_object_new(WQQ_TYPE_UI_QQCATEGORY,
											 "title", title,
											 "online", online,
											 "total", total,
											 NULL);

	GtkExpander *expander = GTK_EXPANDER(category);
	gtk_expander_set_label_widget(expander, GTK_WIDGET(category->header));
	gtk_expander_set_spacing(expander, 5);
	gtk_expander_set_label_fill(expander, TRUE);
	gtk_container_add(GTK_CONTAINER(expander), category->box);

	return category;
}

void wqq_ui_qqcategory_set_title(WqqUiQQCategory * category,
								 const gchar * title)
{
	g_return_if_fail(WQQ_IS_UI_QQCATEGORY(category));
	wqq_ui_qqheader_set_title(WQQ_UI_QQHEADER(category->header), title);
}

void wqq_ui_qqcategory_set_online(WqqUiQQCategory * category, guint online)
{
	g_return_if_fail(WQQ_IS_UI_QQCATEGORY(category));
	wqq_ui_qqheader_set_online(WQQ_UI_QQHEADER(category->header), online);
}

void wqq_ui_qqcategory_set_total(WqqUiQQCategory * category, guint total)
{
	g_return_if_fail(WQQ_IS_UI_QQCATEGORY(category));
	wqq_ui_qqheader_set_total(WQQ_UI_QQHEADER(category->header), total);
}

const gchar *wqq_ui_qqcategory_get_title(WqqUiQQCategory * category)
{
	g_return_if_fail(WQQ_IS_UI_QQCATEGORY(category));
	return wqq_ui_qqheader_get_title(WQQ_UI_QQHEADER(category->header));
}

guint wqq_ui_qqcategory_get_online(WqqUiQQCategory * category)
{
	g_return_if_fail(WQQ_IS_UI_QQCATEGORY(category));
	return wqq_ui_qqheader_get_online(WQQ_UI_QQHEADER(category->header));
}

guint wqq_ui_qqcategory_get_total(WqqUiQQCategory * category)
{
	g_return_if_fail(WQQ_IS_UI_QQCATEGORY(category));
	return wqq_ui_qqheader_get_total(WQQ_UI_QQHEADER(category->header));
}

/* INSERT CELLS */
WqqUiQQCell *wqq_ui_qqcategory_append_cell(WqqUiQQCategory * category,
										   GdkPixbuf * avatar,
										   const gchar * nick,
										   const gchar * remark,
										   WqqUiQQCellStatus status,
										   const gchar * sign,
										   GdkPixbuf * client)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCATEGORY(category), NULL);
	WqqUiQQCell *cell = wqq_ui_qqcell_new_full(avatar, nick,
											   remark, status,
											   sign, client);
	category->list = g_list_append(category->list, cell);
	gtk_box_pack_start(GTK_BOX(category->box), GTK_WIDGET(cell),
					   FALSE, TRUE, 0);
	category->count++;

	g_signal_connect(G_OBJECT(cell), "enter-notify-event",
					 G_CALLBACK(on_mouse_notify), category);
	g_signal_connect(G_OBJECT(cell), "leave-notify-event",
					 G_CALLBACK(on_mouse_notify), category);

	return cell;
}

WqqUiQQCell *wqq_ui_qqcategory_append_cell_from_file(WqqUiQQCategory *
													 category,
													 const gchar * avatar,
													 const gchar * nick,
													 const gchar * remark,
													 WqqUiQQCellStatus
													 status,
													 const gchar * sign,
													 const gchar * client)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCATEGORY(category), NULL);
	GdkPixbuf *avatar_pixbuf = gdk_pixbuf_new_from_file(avatar, NULL);
	GdkPixbuf *client_pixbuf = gdk_pixbuf_new_from_file(client, NULL);
	WqqUiQQCell *cell = wqq_ui_qqcategory_append_cell(category,
													  avatar_pixbuf, nick,
													  remark, status, sign,
													  client_pixbuf);
	if (avatar_pixbuf)
		g_object_unref(avatar_pixbuf);
	if (client_pixbuf)
		g_object_unref(client_pixbuf);

	return cell;
}

gboolean wqq_ui_qqcategory_remove_cell(WqqUiQQCategory * category,
									   WqqUiQQCell * cell)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCATEGORY(category), FALSE);
	GList *lp = g_list_find(category->list, cell);
	if (lp == NULL)
		return FALSE;
	/* 删除Cell */
	category->list = g_list_remove(category->list, cell);
	gtk_container_remove(GTK_CONTAINER(category->box), GTK_WIDGET(cell));
	category->count--;

	return TRUE;
}

void wqq_ui_qqcategory_set_hover_color(WqqUiQQCategory * category,
									   guint red, guint green, guint blue)
{
	g_return_if_fail(WQQ_IS_UI_QQCATEGORY(category));
	category->hover_color.red = red;
	category->hover_color.green = green;
	category->hover_color.blue = blue;
}

gint wqq_ui_qqcategory_get_count(WqqUiQQCategory * category)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCATEGORY(category), -1);
	return category->count;
}

void wqq_ui_qqcategory_count_online_and_total(WqqUiQQCategory * category)
{
	g_return_if_fail(WQQ_IS_UI_QQCATEGORY(category));
	GList *flist = category->list;
	guint online = 0;
	guint total = 0;
	while (flist != NULL) {
		WqqUiQQCell *cell = (WqqUiQQCell *) flist->data;
		if (wqq_ui_qqcell_get_status(cell) != WQQ_UI_QQCELL_STATUS_OFFLINE)
			online++;
		total++;
		flist = g_list_next(flist);
	}
	wqq_ui_qqcategory_set_online(category, online);
	wqq_ui_qqcategory_set_total(category, total);
}

void wqq_ui_qqcategory_refirst_qqcell(WqqUiQQCategory * category,
									  WqqUiQQCell * cell)
{
	g_return_if_fail(WQQ_IS_UI_QQCATEGORY(category) &&
					 WQQ_IS_UI_QQCELL(cell));

	GList *lp = category->list;
	while (lp != NULL) {
		WqqUiQQCell *qc = (WqqUiQQCell *) lp->data;
		if (cell == qc)
			break;
		lp = g_list_next(lp);
	}
	if (lp == NULL || lp->prev == NULL)
		return;
	gtk_box_reorder_child(GTK_BOX(category->box), GTK_WIDGET(cell),
						  GTK_PACK_START);
}

void wqq_ui_qqcategory_relast_qqcell(WqqUiQQCategory * category,
									 WqqUiQQCell * cell)
{
	g_return_if_fail(WQQ_IS_UI_QQCATEGORY(category) &&
					 WQQ_IS_UI_QQCELL(cell));

	GList *lp = category->list;
	while (lp != NULL) {
		WqqUiQQCell *qc = (WqqUiQQCell *) lp->data;
		if (cell == qc)
			break;
		lp = g_list_next(lp);
	}
	if (lp == NULL || lp->next == NULL)
		return;
	gtk_box_reorder_child(GTK_BOX(category->box), GTK_WIDGET(cell),
						  GTK_PACK_END);
}

void wqq_ui_qqcategory_reorder_qqcell(WqqUiQQCategory * category)
{
	g_return_if_fail(WQQ_IS_UI_QQCATEGORY(category));
	GList *lp;
	lp = category->list;
	while (lp != NULL) {
		GtkWidget *cell = lp->data;
		WqqUiQQCellStatus status =
			wqq_ui_qqcell_get_status(WQQ_UI_QQCELL(cell));
		if (status == WQQ_UI_QQCELL_STATUS_OFFLINE)
			gtk_box_reorder_child(GTK_BOX(category->box), cell,0);
		lp = g_list_next(lp);
	}
	lp = category->list;
	while (lp != NULL) {
		GtkWidget *cell = lp->data;
		WqqUiQQCellStatus status =
			wqq_ui_qqcell_get_status(WQQ_UI_QQCELL(cell));
		if (status == WQQ_UI_QQCELL_STATUS_SILENT)
			gtk_box_reorder_child(GTK_BOX(category->box), cell,0);
		lp = g_list_next(lp);
	}
	lp = category->list;
	while (lp != NULL) {
		GtkWidget *cell = lp->data;
		WqqUiQQCellStatus status =
			wqq_ui_qqcell_get_status(WQQ_UI_QQCELL(cell));
		if (status == WQQ_UI_QQCELL_STATUS_AWAY)
			gtk_box_reorder_child(GTK_BOX(category->box), cell,0);
		lp = g_list_next(lp);
	}
	lp = category->list;
	while (lp != NULL) {
		GtkWidget *cell = lp->data;
		WqqUiQQCellStatus status =
			wqq_ui_qqcell_get_status(WQQ_UI_QQCELL(cell));
		if (status == WQQ_UI_QQCELL_STATUS_BUSY)
			gtk_box_reorder_child(GTK_BOX(category->box), cell,0);
		lp = g_list_next(lp);
	}
	lp = category->list;
	while (lp != NULL) {
		GtkWidget *cell = lp->data;
		WqqUiQQCellStatus status =
			wqq_ui_qqcell_get_status(WQQ_UI_QQCELL(cell));
		if (status == WQQ_UI_QQCELL_STATUS_HIDDEN)
			gtk_box_reorder_child(GTK_BOX(category->box), cell,0);
		lp = g_list_next(lp);
	}
	lp = category->list;
	while (lp != NULL) {
		GtkWidget *cell = lp->data;
		WqqUiQQCellStatus status =
			wqq_ui_qqcell_get_status(WQQ_UI_QQCELL(cell));
		if (status == WQQ_UI_QQCELL_STATUS_ONLINE)
			gtk_box_reorder_child(GTK_BOX(category->box), cell,0);
		lp = g_list_next(lp);
	}
	lp = category->list;
	while (lp != NULL) {
		GtkWidget *cell = lp->data;
		WqqUiQQCellStatus status =
			wqq_ui_qqcell_get_status(WQQ_UI_QQCELL(cell));
		if (status == WQQ_UI_QQCELL_STATUS_CALLME)
			gtk_box_reorder_child(GTK_BOX(category->box), cell,0);
		lp = g_list_next(lp);
	}
}
void wqq_ui_qqcategory_reposition_qqcell(WqqUiQQCategory *category,
			WqqUiQQCell *cell)
{
	g_return_if_fail(WQQ_IS_UI_QQCATEGORY(category)&&
				WQQ_IS_UI_QQCELL(cell));
	WqqUiQQCellStatus status=wqq_ui_qqcell_get_status(cell);
	if(status==WQQ_UI_QQCELL_STATUS_OFFLINE){
		gtk_box_reorder_child(GTK_BOX(category->box),
					GTK_WIDGET(cell),-1);
		return;
	}
	gtk_box_reorder_child(GTK_BOX(category->box),
				GTK_WIDGET(cell),0);
	/*gint i=0;*/
	/*GList *lp=gtk_container_get_children(GTK_CONTAINER(category->box));*/
	/*while(lp){*/
	/*WqqUiQQCell *qqcell=lp->data;*/
	/*if(wqq_ui_qqcell_get_status(qqcell)!=WQQ_UI_QQCELL_STATUS_ONLINE){*/
	/*gtk_box_reorder_child(GTK_BOX(category->box),*/
	/*GTK_WIDGET(cell),i);*/
	/*return;*/
	/*}*/
	/*i++;*/
	/*lp=g_list_next(lp);*/
	/*}*/
}
