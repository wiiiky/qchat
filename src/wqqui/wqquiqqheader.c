#include "wqquiqqheader.h"

enum {
	WQQ_UI_QQHEADER_PROPERTY_TITLE = 1,	//gchar *
	WQQ_UI_QQHEADER_PROPERTY_ONLINE,	//guint
	WQQ_UI_QQHEADER_PROPERTY_TOTAL,	//guint
};

G_DEFINE_TYPE(WqqUiQQHeader, wqq_ui_qqheader, GTK_TYPE_BOX);

static void wqq_ui_qqheader_getter(GObject * object, guint property_id,
								   GValue * value, GParamSpec * pspec);
static void wqq_ui_qqheader_setter(GObject * object, guint property_id,
								   const GValue * value,
								   GParamSpec * pspec);

static void wqq_ui_qqheader_init(WqqUiQQHeader * header)
{
	header->title = gtk_label_new("unknown");
	gtk_label_set_ellipsize(GTK_LABEL(header->title), PANGO_ELLIPSIZE_END);
	gtk_widget_set_halign(header->title, GTK_ALIGN_START);
	header->online = 0;
	header->total = 0;
	header->ratio = gtk_label_new("0/0");
	gtk_label_set_ellipsize(GTK_LABEL(header->ratio), PANGO_ELLIPSIZE_END);
	gtk_widget_set_halign(header->ratio, GTK_ALIGN_END);

	/* GtkBox 默认为横向布局，详见gtk+-3.0源代码中的gtk_box_init() */
	GtkBox *box = GTK_BOX(header);
	gtk_box_set_spacing(box, 2);	//默认为0
	gtk_box_pack_start(box, header->title, TRUE, TRUE, 0);
	gtk_box_pack_start(box, header->ratio, FALSE, FALSE, 0);
}

static void wqq_ui_qqheader_class_init(WqqUiQQHeaderClass * klass)
{
	G_OBJECT_CLASS(klass)->get_property = wqq_ui_qqheader_getter;
	G_OBJECT_CLASS(klass)->set_property = wqq_ui_qqheader_setter;

	GParamSpec *pspec;
	pspec = g_param_spec_string("title",
								"the title of header",
								"specify the header title",
								"unknown",
								G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(G_OBJECT_CLASS(klass),
									WQQ_UI_QQHEADER_PROPERTY_TITLE, pspec);

	pspec = g_param_spec_uint("online",
							  "online count",
							  "the count of online friends",
							  0, G_MAXUINT, 0,
							  G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(G_OBJECT_CLASS(klass),
									WQQ_UI_QQHEADER_PROPERTY_ONLINE,
									pspec);

	pspec = g_param_spec_uint("total",
							  "total count",
							  "the count of total friends",
							  0, G_MAXUINT, 0,
							  G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(G_OBJECT_CLASS(klass),
									WQQ_UI_QQHEADER_PROPERTY_TOTAL, pspec);
}

static void wqq_ui_qqheader_getter(GObject * object, guint property_id,
								   GValue * value, GParamSpec * pspec)
{
	WqqUiQQHeader *header = WQQ_UI_QQHEADER(object);
	switch (property_id) {
	case WQQ_UI_QQHEADER_PROPERTY_TITLE:
		g_value_set_string(value,
						   gtk_label_get_text(GTK_LABEL(header->title)));
		break;
	case WQQ_UI_QQHEADER_PROPERTY_ONLINE:
		g_value_set_uint(value, header->online);
		break;
	case WQQ_UI_QQHEADER_PROPERTY_TOTAL:
		g_value_set_uint(value, header->total);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
	}
}

static void wqq_ui_qqheader_setter(GObject * object, guint property_id,
								   const GValue * value,
								   GParamSpec * pspec)
{
	WqqUiQQHeader *header = WQQ_UI_QQHEADER(object);
	gchar *str;
	switch (property_id) {
	case WQQ_UI_QQHEADER_PROPERTY_TITLE:
		gtk_label_set_text(GTK_LABEL(header->title),
						   g_value_get_string(value));
		break;
	case WQQ_UI_QQHEADER_PROPERTY_ONLINE:
		header->online = g_value_get_uint(value);
		str = g_strdup_printf("%u/%u", header->online, header->total);
		gtk_label_set_text(GTK_LABEL(header->ratio), str);
		g_free(str);
		break;
	case WQQ_UI_QQHEADER_PROPERTY_TOTAL:
		header->total = g_value_get_uint(value);
		str = g_strdup_printf("%u/%u", header->online, header->total);
		gtk_label_set_text(GTK_LABEL(header->ratio), str);
		g_free(str);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
	}
}

/***************************************************
 * PUBLIC
 **************************************************/
WqqUiQQHeader *wqq_ui_qqheader_new()
{
	return (WqqUiQQHeader *) g_object_new(WQQ_TYPE_UI_QQHEADER, NULL);
}

WqqUiQQHeader *wqq_ui_qqheader_new_full(const gchar * title,
										guint online, guint total)
{
	return (WqqUiQQHeader *) g_object_new(WQQ_TYPE_UI_QQHEADER,
										  "title", title,
										  "online", online,
										  "total", total, NULL);
}

void wqq_ui_qqheader_set_title(WqqUiQQHeader * header, const gchar * title)
{
	g_return_if_fail(WQQ_IS_UI_QQHEADER(header));
	gtk_label_set_text(GTK_LABEL(header->title), title);
}

void wqq_ui_qqheader_set_online(WqqUiQQHeader * header, guint online)
{
	g_return_if_fail(WQQ_IS_UI_QQHEADER(header));
	g_object_set(header, "online", online, NULL);
}

void wqq_ui_qqheader_set_total(WqqUiQQHeader * header, guint total)
{
	g_return_if_fail(WQQ_IS_UI_QQHEADER(header));
	g_object_set(header, "total", total, NULL);
}

const gchar *wqq_ui_qqheader_get_title(WqqUiQQHeader * header)
{
	g_return_if_fail(WQQ_IS_UI_QQHEADER(header));
	return gtk_label_get_text(GTK_LABEL(header->title));
}

guint wqq_ui_qqheader_get_online(WqqUiQQHeader * header)
{
	g_return_if_fail(WQQ_IS_UI_QQHEADER(header));
	return header->online;
}

guint wqq_ui_qqheader_get_total(WqqUiQQHeader * header)
{
	g_return_if_fail(WQQ_IS_UI_QQHEADER(header));
	return header->total;
}
