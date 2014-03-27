#ifndef __WQQ_UI_QQCELL_H__
#define __WQQ_UI_QQCELL_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS
#define WQQ_TYPE_UI_QQCELL	(wqq_ui_qqcell_get_type())
#define WQQ_UI_QQCELL(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_UI_QQCELL,WqqUiQQCell))
#define WQQ_IS_UI_QQCELL(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_UI_QQCELL))
#define WQQ_UI_QQCELL_CALSS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WQQ_TYPE_UI_QQCELL,WqqUiQQCellClass))
#define WQQ_IS_UI_QQCELL_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass),\
				WQQ_TYPE_UI_QQCELL))
#define WQQ_UI_QQCELL_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS((obj),\
				WQQ_TYPE_UI_QQCELL,WqqUiQQCellClass))
typedef struct _WqqUiQQCell WqqUiQQCell;
typedef struct _WqqUiQQCellClass WqqUiQQCellClass;
typedef enum _WqqUiQQCellStatus WqqUiQQCellStatus;

enum _WqqUiQQCellStatus {
	WQQ_UI_QQCELL_STATUS_ONLINE = 10,
	WQQ_UI_QQCELL_STATUS_OFFLINE = 20,
	WQQ_UI_QQCELL_STATUS_BUSY = 50,
	WQQ_UI_QQCELL_STATUS_AWAY = 30,
	WQQ_UI_QQCELL_STATUS_CALLME = 60,
	WQQ_UI_QQCELL_STATUS_HIDDEN = 40,
	WQQ_UI_QQCELL_STATUS_SILENT = 70,
};

struct _WqqUiQQCell {
	GtkEventBox box;
	/* GtkWidgets */
	GtkWidget *grid;
	GtkWidget *avatar_image;
	GdkPixbuf *online_pf;
	GdkPixbuf *offline_pf;
	GtkWidget *nick_label;
	GtkWidget *status_label;
	GtkWidget *sign_label;
	GtkWidget *client_image;
	/* infos */
	gchar *nick;
	gchar *remark;
	WqqUiQQCellStatus status;
	gchar *sign;
	/* more TODO */
	guint64 uin;
	gpointer *data;
	gpointer *data1;
	gpointer *data2;
};

struct _WqqUiQQCellClass {
	GtkEventBoxClass parent_class;
};
GType wqq_ui_qqcell_status_get_type(void) G_GNUC_CONST;
#define WQQ_TYPE_UI_QQCELL_STATUS	(wqq_ui_qqcell_status_get_type())

GType wqq_ui_qqcell_get_type(void) G_GNUC_CONST;
/* creators */
WqqUiQQCell *wqq_ui_qqcell_new(void);
WqqUiQQCell *wqq_ui_qqcell_new_full(GdkPixbuf * avatar, const gchar * nick,
									const gchar * remark,
									WqqUiQQCellStatus status,
									const gchar * sign,
									GdkPixbuf * client);
WqqUiQQCell *wqq_ui_qqcell_new_full_from_path(const gchar * avatar_path,
											  const gchar * nick,
											  const gchar * remark,
											  WqqUiQQCellStatus status,
											  const gchar * sign,
											  const gchar * client_path);

void wqq_ui_qqcell_set_avatar(WqqUiQQCell * cell, GdkPixbuf * pixbuf);
void wqq_ui_qqcell_set_avatar_from_path(WqqUiQQCell * cell,
										const gchar * path);
void wqq_ui_qqcell_set_avatar_from_path_not_update(WqqUiQQCell *cell,
			const gchar *path);
void wqq_ui_qqcell_update_avatar(WqqUiQQCell *cell);
void wqq_ui_qqcell_set_nick(WqqUiQQCell * cell, const gchar * nick);
const gchar *wqq_ui_qqcell_get_nick(WqqUiQQCell * cell);
void wqq_ui_qqcell_set_remark(WqqUiQQCell * cell, const gchar * remark);
const gchar *wqq_ui_qqcell_get_remark(WqqUiQQCell * cell);
void wqq_ui_qqcell_set_status(WqqUiQQCell * cell,
							  WqqUiQQCellStatus status);
WqqUiQQCellStatus wqq_ui_qqcell_get_status(WqqUiQQCell * cell);
const gchar *wqq_ui_qqcell_get_status_to_string(WqqUiQQCell *cell);
void wqq_ui_qqcell_set_sign(WqqUiQQCell * cell, const gchar * sign);
const gchar *wqq_ui_qqcell_get_sign(WqqUiQQCell * cell);
void wqq_ui_qqcell_set_client(WqqUiQQCell * cell, GdkPixbuf * pixbuf);
void wqq_ui_qqcell_set_client_from_path(WqqUiQQCell * cell,
										const gchar * path);
guint64 wqq_ui_qqcell_get_uin(WqqUiQQCell * cell);
void wqq_ui_qqcell_set_uin(WqqUiQQCell * cell, guint64 uin);
void wqq_ui_qqcell_set_data(WqqUiQQCell * cell, gpointer data);
void wqq_ui_qqcell_set_data1(WqqUiQQCell * cell, gpointer data1);
void wqq_ui_qqcell_set_data2(WqqUiQQCell * cell, gpointer data2);
gpointer wqq_ui_qqcell_get_data(WqqUiQQCell * cell);
gpointer wqq_ui_qqcell_get_data1(WqqUiQQCell * cell);
gpointer wqq_ui_qqcell_get_data2(WqqUiQQCell * cell);

G_END_DECLS
#endif
