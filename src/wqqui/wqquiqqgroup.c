#include "wqquiqqgroup.h"
enum {
	WQQ_UI_QQGROUP_PROPERTY_NAME = 1,
	WQQ_UI_QQGROUP_PROPERTY_MARKNAME,
	WQQ_UI_QQGROUP_PROPERTY_IMAGE_FILE,
};

G_DEFINE_TYPE(WqqUiQQGroup, wqq_ui_qqgroup, GTK_TYPE_EVENT_BOX);

static void wqq_ui_qqgroup_getter(GObject * obj, guint property_id,
								  GValue * value, GParamSpec * ps);
static void wqq_ui_qqgroup_setter(GObject * obj, guint property_id,
								  const GValue * value, GParamSpec * ps);
static gboolean on_mouse_notify(GtkWidget * widget,
								GdkEventCrossing * event, gpointer data);

static void wqq_ui_qqgroup_init(WqqUiQQGroup * group)
{
	GtkWidget *label = gtk_label_new("GROUP");
	gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
	GtkWidget *image = gtk_image_new();
	GtkWidget *grid = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID(grid), 6);

	gtk_grid_attach(GTK_GRID(grid), image, 0, 0, 2, 2);
	gtk_grid_attach(GTK_GRID(grid), label, 2, 0, 1, 1);
	gtk_container_add(GTK_CONTAINER(group), grid);
	group->label = label;
	group->image = image;
	group->name = NULL;
	group->markname = NULL;

	gtk_container_set_border_width(GTK_CONTAINER(group), 1);
	g_signal_connect(G_OBJECT(group), "leave-notify-event",
					 G_CALLBACK(on_mouse_notify), NULL);
	g_signal_connect(G_OBJECT(group), "enter-notify-event",
					 G_CALLBACK(on_mouse_notify), NULL);
}

static void wqq_ui_qqgroup_finalize(GObject * obj)
{
	WqqUiQQGroup *group = WQQ_UI_QQGROUP(obj);
	g_free(group->name);
	g_free(group->markname);
}

static void wqq_ui_qqgroup_class_init(WqqUiQQGroupClass * klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->get_property = wqq_ui_qqgroup_getter;
	object_class->set_property = wqq_ui_qqgroup_setter;
	object_class->finalize = wqq_ui_qqgroup_finalize;

	GParamSpec *ps;
	ps = g_param_spec_string("name",
							 "group name",
							 "Group Name",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_UI_QQGROUP_PROPERTY_NAME, ps);

	ps = g_param_spec_string("markname",
							 "group markname",
							 "Group Markname",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_UI_QQGROUP_PROPERTY_MARKNAME, ps);

	ps = g_param_spec_string("image_file",
							 "image file",
							 "Image File", NULL, G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_UI_QQGROUP_PROPERTY_IMAGE_FILE,
									ps);
}

static void wqq_ui_qqgroup_getter(GObject * obj, guint property_id,
								  GValue * value, GParamSpec * ps)
{
	WqqUiQQGroup *group = WQQ_UI_QQGROUP(obj);
	switch (property_id) {
	case WQQ_UI_QQGROUP_PROPERTY_NAME:
		g_value_set_string(value, group->name);
		break;
	case WQQ_UI_QQGROUP_PROPERTY_MARKNAME:
		g_value_set_string(value, group->markname);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, property_id, ps);
		break;
	}
}

static void wqq_ui_qqgroup_setter(GObject * obj, guint property_id,
								  const GValue * value, GParamSpec * ps)
{
	WqqUiQQGroup *group = WQQ_UI_QQGROUP(obj);
	switch (property_id) {
	case WQQ_UI_QQGROUP_PROPERTY_NAME:
		wqq_ui_qqgroup_set_name(group, g_value_get_string(value));
		break;
	case WQQ_UI_QQGROUP_PROPERTY_MARKNAME:
		wqq_ui_qqgroup_set_markname(group, g_value_get_string(value));
		break;
	case WQQ_UI_QQGROUP_PROPERTY_IMAGE_FILE:
		wqq_ui_qqgroup_set_image_from_file(group,
										   g_value_get_string(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(obj, property_id, ps);
		break;
	}
}

static void wqq_ui_qqgroup_update_label(WqqUiQQGroup * group)
{
	gchar *markup;
	if (group->markname)
		markup = g_strdup_printf("%s<span foreground"
								 "=\"grey\">(%s)</span>",
								 group->markname ? group->markname : "",
								 group->name ? group->name : "");
	else
		markup = g_strdup_printf("%s", group->name ? group->name : "");
	gtk_label_set_markup(GTK_LABEL(group->label), markup);
	g_free(markup);
}

static gboolean on_mouse_notify(GtkWidget * widget,
								GdkEventCrossing * event, gpointer data)
{
	static GtkCssProvider *css1 = NULL, *css2 = NULL;
	if (css1 == NULL) {
		css1 = gtk_css_provider_new();
		css2 = gtk_css_provider_new();
		gchar *str = "GtkEventBox{border-radius:6px;"
			"background-color:rgba(100,200,255,255);}";
		gtk_css_provider_load_from_data(css1, str, -1, NULL);
		str = "GtkEventBox{border-radius:6px;"
			"background-color:rgba(255,255,255,0);}";
		gtk_css_provider_load_from_data(css2, str, -1, NULL);
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

/***********************************************************
 * PUBLIC
 ************************************************************/
WqqUiQQGroup *wqq_ui_qqgroup_new(const gchar * name,
								 const gchar * markname)
{
	WqqUiQQGroup *group = g_object_new(WQQ_TYPE_UI_QQGROUP, NULL);
	group->name = g_strdup(name);
	group->markname = g_strdup(markname);
	wqq_ui_qqgroup_update_label(group);
	return group;
}

void wqq_ui_qqgroup_set_name(WqqUiQQGroup * group, const gchar * name)
{
	g_return_if_fail(WQQ_IS_UI_QQGROUP(group));
	g_free(group->name);
	group->name = g_strdup(name);
	wqq_ui_qqgroup_update_label(group);
}

void wqq_ui_qqgroup_set_markname(WqqUiQQGroup * group,
								 const gchar * markname)
{
	g_return_if_fail(WQQ_IS_UI_QQGROUP(group));
	g_free(group->markname);
	group->markname = g_strdup(markname);
	wqq_ui_qqgroup_update_label(group);
}

void wqq_ui_qqgroup_set_image_from_file(WqqUiQQGroup * group,
										const gchar * file)
{
	g_return_if_fail(WQQ_IS_UI_QQGROUP(group));
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file_at_size(file,
														 40, 40, NULL);
	if (pixbuf) {
		wqq_ui_qqgroup_set_image(group, pixbuf);
		g_object_unref(pixbuf);
	} else
		gtk_image_set_from_file(GTK_IMAGE(group->image), "");
}

void wqq_ui_qqgroup_set_image(WqqUiQQGroup * group, GdkPixbuf * pixbuf)
{
	g_return_if_fail(WQQ_IS_UI_QQGROUP(group));
	gtk_image_set_from_pixbuf(GTK_IMAGE(group->image), pixbuf);
}

const gchar *wqq_ui_qqgroup_get_name(WqqUiQQGroup * group)
{
	g_return_val_if_fail(WQQ_IS_UI_QQGROUP(group), NULL);
	return group->name;
}

const gchar *wqq_ui_qqgroup_get_markname(WqqUiQQGroup * group)
{
	g_return_val_if_fail(WQQ_IS_UI_QQGROUP(group), NULL);
	return group->markname;
}

void wqq_ui_qqgroup_set_data(WqqUiQQGroup * group, gpointer data)
{
	g_return_if_fail(WQQ_IS_UI_QQGROUP(group));
	group->data = data;
}

gpointer wqq_ui_qqgroup_get_data(WqqUiQQGroup * group)
{
	g_return_val_if_fail(WQQ_IS_UI_QQGROUP(group), NULL);
	return group->data;
}
