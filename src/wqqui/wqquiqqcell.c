#include "wqquiqqcell.h"

#define WQQ_UI_QQCELL_AVATAR_IMAGE_SIZE	40
#define WQQ_UI_QQCELL_CLIENT_IMAGE_SIZE	20

static const gchar *_wqq_ui_qqcell_status_to_string(WqqUiQQCellStatus
													status);

enum {
	WQQ_UI_QQCELL_PROPERTY_AVATAR = 1,	//GdkPixbuf
	WQQ_UI_QQCELL_PROPERTY_REMARK,	//gchar*
	WQQ_UI_QQCELL_PROPERTY_NICK,	//gchar*
	WQQ_UI_QQCELL_PROPERTY_STATUS,	//WqqUiQQCellStatus
	WQQ_UI_QQCELL_PROPERTY_SIGN,	//gchar*
	WQQ_UI_QQCELL_PROPERTY_CLIENT,	//GdkPixbuf
	WQQ_UI_QQCELL_PROPERTY_UIN,
};

G_DEFINE_TYPE(WqqUiQQCell, wqq_ui_qqcell, GTK_TYPE_EVENT_BOX);

static void wqq_ui_qqcell_finalize(gpointer data, GObject * object);
static void wqq_ui_qqcell_getter(GObject * object, guint property_id,
								 GValue * value, GParamSpec * pspec);
static void wqq_ui_qqcell_setter(GObject * object, guint property_id,
								 const GValue * value, GParamSpec * pspec);
static gchar *_wqq_string_char_replace(const gchar * src, const gchar c,
									   const gchar * s);

static void wqq_ui_qqcell_init(WqqUiQQCell * cell)
{
	GtkWidget *grid = gtk_grid_new();
	gtk_grid_set_column_spacing(GTK_GRID(grid), 1);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 3);
	gtk_container_set_border_width(GTK_CONTAINER(cell), 1);
	gtk_container_add(GTK_CONTAINER(GTK_EVENT_BOX(cell)), grid);

	GtkWidget *avatar = gtk_image_new();
	/*gtk_widget_set_margin_left(avatar, 3); */
	gtk_widget_set_margin_right(avatar, 5);
	gtk_grid_attach(GTK_GRID(grid), avatar, 0, 0, 1, 2);

	GtkWidget *nick = gtk_label_new("");
	gtk_label_set_single_line_mode(GTK_LABEL(nick), TRUE);
	gtk_label_set_ellipsize(GTK_LABEL(nick), PANGO_ELLIPSIZE_END);
	gtk_widget_set_hexpand(nick, TRUE);
	gtk_widget_set_valign(nick, GTK_ALIGN_START);
	gtk_widget_set_halign(nick, GTK_ALIGN_START);
	gtk_grid_attach(GTK_GRID(grid), nick, 1, 0, 2, 1);

	GtkWidget *status = gtk_label_new("");
	gtk_label_set_single_line_mode(GTK_LABEL(status), TRUE);
	/*gtk_label_set_ellipsize(GTK_LABEL(status), PANGO_ELLIPSIZE_END); */
	gtk_widget_set_valign(status, GTK_ALIGN_END);
	gtk_widget_set_halign(status, GTK_ALIGN_START);
	gtk_grid_attach(GTK_GRID(grid), status, 1, 1, 1, 1);

	GtkWidget *sign = gtk_label_new("");
	gtk_label_set_single_line_mode(GTK_LABEL(sign), TRUE);
	gtk_label_set_justify(GTK_LABEL(sign), GTK_JUSTIFY_LEFT);
	gtk_label_set_ellipsize(GTK_LABEL(sign), PANGO_ELLIPSIZE_END);
	gtk_widget_set_hexpand(sign, TRUE);
	gtk_widget_set_valign(sign, GTK_ALIGN_END);
	gtk_widget_set_halign(sign, GTK_ALIGN_START);
	gtk_grid_attach(GTK_GRID(grid), sign, 2, 1, 1, 1);

	GtkWidget *client = gtk_image_new();
	gtk_widget_set_margin_right(client, 5);
	gtk_grid_attach(GTK_GRID(grid), client, 3, 0, 1, 1);

	cell->avatar_image = avatar;
	cell->online_pf = NULL;
	cell->offline_pf = NULL;
	cell->grid = grid;
	cell->client_image = client;
	cell->nick_label = nick;
	cell->status_label = status;
	cell->sign_label = sign;
	cell->nick = NULL;
	cell->remark = NULL;
	cell->status = WQQ_UI_QQCELL_STATUS_OFFLINE;
	cell->sign = NULL;
	cell->data = NULL;
	cell->data1 = NULL;
	cell->data2 = NULL;
	cell->uin = 0;
	gtk_container_set_border_width(GTK_CONTAINER(cell), 1);

	g_object_weak_ref(G_OBJECT(cell), wqq_ui_qqcell_finalize, NULL);
}


static void wqq_ui_qqcell_finalize(gpointer data, GObject * object)
{
	WqqUiQQCell *cell = WQQ_UI_QQCELL(object);
	g_free(cell->nick);
	g_free(cell->remark);
	g_free(cell->sign);
	g_object_unref(cell->online_pf);
	g_object_unref(cell->offline_pf);
}

static void wqq_ui_qqcell_class_init(WqqUiQQCellClass * klass)
{
	G_OBJECT_CLASS(klass)->get_property = wqq_ui_qqcell_getter;
	G_OBJECT_CLASS(klass)->set_property = wqq_ui_qqcell_setter;

	GParamSpec *pspec = g_param_spec_object("avatar",
											"avatar image",
											"specify the image(GdkPixbuf"
											" format) for avatar",
											GDK_TYPE_PIXBUF,
											G_PARAM_WRITABLE);
	g_object_class_install_property(G_OBJECT_CLASS(klass),
									WQQ_UI_QQCELL_PROPERTY_AVATAR, pspec);

	pspec = g_param_spec_string("remark",
								"friend reamrk",
								"the remark of a friend",
								NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(G_OBJECT_CLASS(klass),
									WQQ_UI_QQCELL_PROPERTY_REMARK, pspec);

	pspec = g_param_spec_string("nick",
								"friend nick",
								"the nick of a friend",
								NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(G_OBJECT_CLASS(klass),
									WQQ_UI_QQCELL_PROPERTY_NICK, pspec);

	pspec = g_param_spec_enum("status",
							  "friend status",
							  "the status of a friend",
							  WQQ_TYPE_UI_QQCELL_STATUS,
							  WQQ_UI_QQCELL_STATUS_OFFLINE,
							  G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(G_OBJECT_CLASS(klass),
									WQQ_UI_QQCELL_PROPERTY_STATUS, pspec);

	pspec = g_param_spec_string("sign",
								"friend sign",
								"the sign of a friend",
								NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(G_OBJECT_CLASS(klass),
									WQQ_UI_QQCELL_PROPERTY_SIGN, pspec);

	pspec = g_param_spec_object("client",
								"client image",
								"specify the image(GdkPixbuf format)"
								" for client",
								GDK_TYPE_PIXBUF, G_PARAM_WRITABLE);
	g_object_class_install_property(G_OBJECT_CLASS(klass),
									WQQ_UI_QQCELL_PROPERTY_CLIENT, pspec);

	pspec = g_param_spec_uint64("uin",
								"qq uin",
								"QQ Uin",
								0, G_MAXUINT64,
								0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(G_OBJECT_CLASS(klass),
									WQQ_UI_QQCELL_PROPERTY_UIN, pspec);


}


static void wqq_ui_qqcell_getter(GObject * object, guint property_id,
								 GValue * value, GParamSpec * pspec)
{
	WqqUiQQCell *cell = WQQ_UI_QQCELL(object);
	GdkPixbuf *pixbuf;
	switch (property_id) {
	case WQQ_UI_QQCELL_PROPERTY_REMARK:
		g_value_set_string(value, cell->remark);
		break;
	case WQQ_UI_QQCELL_PROPERTY_NICK:
		g_value_set_string(value, cell->nick);
		break;
	case WQQ_UI_QQCELL_PROPERTY_STATUS:
		g_value_set_enum(value, cell->status);
		break;
	case WQQ_UI_QQCELL_PROPERTY_SIGN:
		g_value_set_string(value, cell->sign);
	case WQQ_UI_QQCELL_PROPERTY_UIN:
		g_value_set_uint64(value, cell->uin);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void wqq_ui_qqcell_setter(GObject * object, guint property_id,
								 const GValue * value, GParamSpec * pspec)
{
	WqqUiQQCell *cell = WQQ_UI_QQCELL(object);
	GdkPixbuf *pixbuf;
	GdkPixbuf *pf = NULL;
	gchar *str;
	switch (property_id) {
	case WQQ_UI_QQCELL_PROPERTY_AVATAR:
		pixbuf = g_value_get_object(value);
		pf = NULL;
		if (GDK_IS_PIXBUF(pixbuf)) {
			pf = gdk_pixbuf_scale_simple(pixbuf,
						WQQ_UI_QQCELL_AVATAR_IMAGE_SIZE,
						WQQ_UI_QQCELL_AVATAR_IMAGE_SIZE,
						GDK_INTERP_HYPER);
			if (cell->online_pf != NULL) {
				g_object_unref(cell->online_pf);
				g_object_unref(cell->offline_pf);
			}
			cell->online_pf = pf;
			cell->offline_pf = gdk_pixbuf_copy(pf);
			gdk_pixbuf_saturate_and_pixelate(cell->online_pf,
											 cell->offline_pf, 0.0, FALSE);
			if (cell->status == WQQ_UI_QQCELL_STATUS_OFFLINE) {
				g_object_ref(cell->offline_pf);
				gtk_image_set_from_pixbuf(GTK_IMAGE(cell->avatar_image),
							cell->offline_pf);
			} else {
				g_object_ref(cell->online_pf);
				gtk_image_set_from_pixbuf(GTK_IMAGE(cell->avatar_image),
							cell->online_pf);
			}
		}
		break;
	case WQQ_UI_QQCELL_PROPERTY_REMARK:
		g_free(cell->remark);
		cell->remark = g_strdup(g_value_get_string(value));
		if (cell->remark != NULL) {
			str = g_strdup_printf("%s<span foreground=\"grey\">"
								  "(%s)</span>", cell->remark, cell->nick);
			gtk_label_set_markup(GTK_LABEL(cell->nick_label), str);
			g_free(str);
		} else {
			gtk_label_set_markup(GTK_LABEL(cell->nick_label), cell->nick);
		}
		break;
	case WQQ_UI_QQCELL_PROPERTY_NICK:
		g_free(cell->nick);
		cell->nick = g_strdup(g_value_get_string(value));
		if (cell->remark != NULL) {
			str = g_strdup_printf("%s<span foreground=\"grey\">"
								  "(%s)</span>", cell->remark, cell->nick);
			gtk_label_set_markup(GTK_LABEL(cell->nick_label), str);
			g_free(str);
		} else {
			gtk_label_set_markup(GTK_LABEL(cell->nick_label), cell->nick);
		}
		break;
	case WQQ_UI_QQCELL_PROPERTY_STATUS:
		cell->status = g_value_get_enum(value);
		str = g_strdup_printf("<small><span foreground=\"grey\">[%s]"
							  "</span> </small>",
							  _wqq_ui_qqcell_status_to_string
							  (cell->status));
		gtk_label_set_markup(GTK_LABEL(cell->status_label), str);
		if (cell->status == WQQ_UI_QQCELL_STATUS_OFFLINE) {
			g_object_ref(cell->offline_pf);
			g_object_set(cell->avatar_image, "pixbuf", cell->offline_pf,
						 NULL);
		} else {
			g_object_ref(cell->online_pf);
			g_object_set(cell->avatar_image, "pixbuf", cell->online_pf,
						 NULL);
		}
		g_free(str);
		break;
	case WQQ_UI_QQCELL_PROPERTY_SIGN:
		g_free(cell->sign);
		cell->sign =
			_wqq_string_char_replace(g_value_get_string(value), '\n', "");
		str =
			g_strdup_printf("<small>%s</small>",
							cell->sign == NULL ? "" : cell->sign);
		gtk_label_set_markup(GTK_LABEL(cell->sign_label), str);
		gtk_widget_set_tooltip_text(cell->sign_label, cell->sign);
		g_free(str);
		break;
	case WQQ_UI_QQCELL_PROPERTY_CLIENT:
		pixbuf = g_value_get_object(value);
		pf = NULL;
		if (GDK_IS_PIXBUF(pixbuf)) {
			pf = gdk_pixbuf_scale_simple(pixbuf,
										 WQQ_UI_QQCELL_CLIENT_IMAGE_SIZE,
										 WQQ_UI_QQCELL_CLIENT_IMAGE_SIZE,
										 GDK_INTERP_HYPER);
			g_object_set(cell->client_image, "pixbuf", pf, NULL);
		}
		if (pf != NULL)
			g_object_unref(pf);
		break;
	case WQQ_UI_QQCELL_PROPERTY_UIN:
		cell->uin = g_value_get_uint64(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
	}
}


/**********************************************************
 *  * PUBLIC
 *   *********************************************************/


/* creators */
WqqUiQQCell *wqq_ui_qqcell_new(void)
{
	return (WqqUiQQCell *) g_object_new(WQQ_TYPE_UI_QQCELL, NULL);
}

WqqUiQQCell *wqq_ui_qqcell_new_full(GdkPixbuf * avatar, const gchar * nick,
									const gchar * remark,
									WqqUiQQCellStatus status,
									const gchar * sign, GdkPixbuf * client)
{
	return (WqqUiQQCell *) g_object_new(WQQ_TYPE_UI_QQCELL, "avatar",
										avatar, "remark", remark, "nick",
										nick, "status", status, "sign",
										sign, "client", client, NULL);
}

WqqUiQQCell *wqq_ui_qqcell_new_full_from_path(const gchar * avatar_path,
											  const gchar * nick,
											  const gchar * remark,
											  WqqUiQQCellStatus status,
											  const gchar * sign,
											  const gchar * client_path)
{
	GdkPixbuf *p1, *p2;
	p1 = gdk_pixbuf_new_from_file(avatar_path, NULL);
	p2 = gdk_pixbuf_new_from_file(client_path, NULL);
	WqqUiQQCell *cell = (WqqUiQQCell *) g_object_new(WQQ_TYPE_UI_QQCELL,
													 "avatar", p1,
													 "remark", remark,
													 "nick", nick,
													 "status", status,
													 "sign", sign,
													 "client", p2,
													 NULL);
	if (p1 != NULL)
		g_object_unref(p1);
	if (p2 != NULL)
		g_object_unref(p2);
	return cell;
}

void wqq_ui_qqcell_set_avatar(WqqUiQQCell * cell, GdkPixbuf * pixbuf)
{
	g_return_if_fail(WQQ_IS_UI_QQCELL(cell));
	g_object_set(cell, "avatar", pixbuf, NULL);
}

void wqq_ui_qqcell_set_avatar_from_path(WqqUiQQCell * cell,
										const gchar * path)
{
	g_return_if_fail(WQQ_IS_UI_QQCELL(cell));
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(path, NULL);
	g_object_set(cell, "avatar", pixbuf, NULL);
	if (pixbuf != NULL)
		g_object_unref(pixbuf);
}

void wqq_ui_qqcell_set_nick(WqqUiQQCell * cell, const gchar * nick)
{
	g_return_if_fail(WQQ_IS_UI_QQCELL(cell));
	g_object_set(cell, "nick", nick, NULL);
}

const gchar *wqq_ui_qqcell_get_nick(WqqUiQQCell * cell)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCELL(cell), NULL);
	return cell->nick;
}

void wqq_ui_qqcell_set_remark(WqqUiQQCell * cell, const gchar * remark)
{
	g_return_if_fail(WQQ_IS_UI_QQCELL(cell));
	g_object_set(cell, "remark", remark, NULL);
}

const gchar *wqq_ui_qqcell_get_remark(WqqUiQQCell * cell)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCELL(cell), NULL);
	return cell->remark;
}

void wqq_ui_qqcell_set_status(WqqUiQQCell * cell, WqqUiQQCellStatus status)
{
	g_return_if_fail(WQQ_IS_UI_QQCELL(cell));
	g_object_set(cell, "status", status, NULL);
}

WqqUiQQCellStatus wqq_ui_qqcell_get_status(WqqUiQQCell * cell)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCELL(cell),
						 WQQ_UI_QQCELL_STATUS_OFFLINE);
	return cell->status;
}
const gchar *wqq_ui_qqcell_get_status_to_string(WqqUiQQCell *cell)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCELL(cell),NULL);
	return _wqq_ui_qqcell_status_to_string(cell->status);
}

void wqq_ui_qqcell_set_sign(WqqUiQQCell * cell, const gchar * sign)
{
	g_return_if_fail(WQQ_IS_UI_QQCELL(cell)&&sign!=NULL);
	g_object_set(cell, "sign", sign, NULL);
}

const gchar *wqq_ui_qqcell_get_sign(WqqUiQQCell * cell)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCELL(cell), NULL);
	return cell->sign;
}

void wqq_ui_qqcell_set_client(WqqUiQQCell * cell, GdkPixbuf * pixbuf)
{
	g_return_if_fail(WQQ_IS_UI_QQCELL(cell));
	g_object_set(cell, "client", pixbuf, NULL);
}

void wqq_ui_qqcell_set_client_from_path(WqqUiQQCell * cell,
										const gchar * path)
{
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(path, NULL);
	g_object_set(cell, "client", pixbuf, NULL);
	if (pixbuf != NULL)
		g_object_unref(pixbuf);
}

guint64 wqq_ui_qqcell_get_uin(WqqUiQQCell * cell)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCELL(cell), 0);
	return cell->uin;
}

void wqq_ui_qqcell_set_uin(WqqUiQQCell * cell, guint64 uin)
{
	g_return_if_fail(WQQ_IS_UI_QQCELL(cell));
	cell->uin = uin;
}

void wqq_ui_qqcell_set_data(WqqUiQQCell * cell, gpointer data)
{
	g_return_if_fail(WQQ_IS_UI_QQCELL(cell));
	cell->data = data;
}

void wqq_ui_qqcell_set_data1(WqqUiQQCell * cell, gpointer data1)
{
	g_return_if_fail(WQQ_IS_UI_QQCELL(cell));
	cell->data1 = data1;
}

void wqq_ui_qqcell_set_data2(WqqUiQQCell * cell, gpointer data2)
{
	g_return_if_fail(WQQ_IS_UI_QQCELL(cell));
	cell->data2 = data2;
}

gpointer wqq_ui_qqcell_get_data(WqqUiQQCell * cell)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCELL(cell), NULL);
	return cell->data;
}

gpointer wqq_ui_qqcell_get_data1(WqqUiQQCell * cell)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCELL(cell), NULL);
	return cell->data1;
}

gpointer wqq_ui_qqcell_get_data2(WqqUiQQCell * cell)
{
	g_return_val_if_fail(WQQ_IS_UI_QQCELL(cell), NULL);
	return cell->data2;
}


GType wqq_ui_qqcell_status_get_type(void)
{
	static GType etype = 0;
	if (G_UNLIKELY(etype == 0)) {
		static const GEnumValue values[] = {
			{WQQ_UI_QQCELL_STATUS_ONLINE, "WQQ_UI_QQCELL_STATUS_ONLINE",
			 "online"},
			{WQQ_UI_QQCELL_STATUS_OFFLINE, "WQQ_UI_QQCELL_STATUS_OFFLINE",
			 "offline"},
			{WQQ_UI_QQCELL_STATUS_HIDDEN, "WQQ_UI_QQCELL_STATUS_HIDDEN",
			 "hidden"},
			{WQQ_UI_QQCELL_STATUS_CALLME, "WQQ_UI_QQCELL_STATUS_CALLME",
			 "callme"},
			{WQQ_UI_QQCELL_STATUS_AWAY, "WQQ_UI_QQCELL_STATUS_AWAY",
			 "away"},
			{WQQ_UI_QQCELL_STATUS_BUSY, "WQQ_UI_QQCELL_STATUS_BUSY",
			 "busy"},
			{WQQ_UI_QQCELL_STATUS_CALLME, "WQQ_UI_QQCELL_STATUS_CALLME",
			 "callme"},
			{WQQ_UI_QQCELL_STATUS_SILENT, "WQQ_UI_QQCELL_STATUS_SILENT",
			 "silent"},
			{0, NULL, NULL}
		};
		etype =
			g_enum_register_static(g_intern_static_string
								   ("WqqUiQQCellStatus"), values);
	}
	return etype;
}

/* 将在线状态的枚举类型转化为字符串 */
static const gchar *_wqq_ui_qqcell_status_to_string(WqqUiQQCellStatus
													status)
{
	switch (status) {
	case WQQ_UI_QQCELL_STATUS_ONLINE:
		return "online";
	case WQQ_UI_QQCELL_STATUS_AWAY:
		return "away";
	case WQQ_UI_QQCELL_STATUS_BUSY:
		return "busy";
	case WQQ_UI_QQCELL_STATUS_SILENT:
		return "silent";
	case WQQ_UI_QQCELL_STATUS_CALLME:
		return "callme";
	case WQQ_UI_QQCELL_STATUS_HIDDEN:
		return "hidden";
	default:
		return "offline";
	}
	return "offline";
}

static gchar *_wqq_string_char_replace(const gchar * src, const gchar c,
									   const gchar * s)
{
	if (src == NULL)
		return NULL;
	GString *string = g_string_new("");
	const gchar *ptr = src;
	while (*ptr != '\0') {
		gchar ch = *ptr;
		if (ch != c)
			string = g_string_append_c(string, ch);
		else
			string = g_string_append(string, s);
		ptr++;
	}
	return g_string_free(string, FALSE);
}
void wqq_ui_qqcell_set_avatar_from_path_not_update(WqqUiQQCell *cell,
			const gchar *path)
{
	g_return_if_fail(WQQ_IS_UI_QQCELL(cell));
	GdkPixbuf *pixbuf=gdk_pixbuf_new_from_file_at_size(path,
				WQQ_UI_QQCELL_AVATAR_IMAGE_SIZE,
				WQQ_UI_QQCELL_AVATAR_IMAGE_SIZE,
				NULL);
	GdkPixbuf *offline;
	if(pixbuf){
			if (cell->online_pf != NULL) {
				g_object_unref(cell->online_pf);
				g_object_unref(cell->offline_pf);
			}
			cell->online_pf=pixbuf;
			cell->offline_pf = gdk_pixbuf_copy(pixbuf);
			gdk_pixbuf_saturate_and_pixelate(cell->online_pf,
											 cell->offline_pf, 0.0, FALSE);
	}
}
void wqq_ui_qqcell_update_avatar(WqqUiQQCell *cell)
{
	g_return_if_fail(WQQ_IS_UI_QQCELL(cell));
	if(cell->status!=WQQ_UI_QQCELL_STATUS_OFFLINE){
		g_object_ref(cell->online_pf);
		gtk_image_set_from_pixbuf(GTK_IMAGE(cell->avatar_image),
					cell->online_pf);
	}else{
		g_object_ref(cell->offline_pf);
		gtk_image_set_from_pixbuf(GTK_IMAGE(cell->avatar_image),
					cell->offline_pf);
	}
}
