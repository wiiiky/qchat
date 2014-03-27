#ifndef __WQQ_UI_QQPANEL_H__
#define __WQQ_UI_QQPANEL_H__
#include "wqqui/wqquiqqlist.h"
#include "wqqui/wqquiqqgrouplist.h"
#include "wqq/wqq.h"
#include "wqquiqqchat.h"
#include <libnotify/notify.h>

G_BEGIN_DECLS
#define WQQ_TYPE_UI_QQPANEL	(wqq_ui_qqpanel_get_type())
#define WQQ_UI_QQPANEL(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_UI_QQPANEL,WqqUiQQPanel))
#define WQQ_IS_UI_QQPANEL(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_UI_QQPANEL))
#define WQQ_UI_QQPANEL_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WQQ_TYPE_UI_QQPANEL,WqqUiQQPanelClass))
#define WQQ_IS_UI_QQPANEL_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass),\
				WQQ_TYPE_UI_QQPANEL))
#define WQQ_UI_QQPANEL_GET_CLASS(obj)	(G_TYPE_GET_INSTANCE_CLASS((obj),\
				WQQ_TYPE_UI_QQPANEL,WqqUiQQPanelClass))
typedef struct _WqqUiQQPanel WqqUiQQPanel;
typedef struct _WqqUiQQPanelClass WqqUiQQPanelClass;

struct _WqqUiQQPanel {
	GtkWindow parent;
	WqqSession *session;
	GtkWidget *nick;			/* GtkLable */
	GtkWidget *avatar;			/* GtkToolButton */
	GtkWidget *sign;			/* GtkLabel */
	GtkWidget *status;			/* GtkMenuButton */
	GtkToolItem *friend_note;	/* GtkToggleToolButton */
	GtkToolItem *group_note;	/* GtkToggleToolButton */
	GtkToolItem *chat_note;		/* GtkToggleToolButton */

	GtkWidget *viewport;

	GtkWidget *current_widget;
	WqqUiQQList *flist;
	WqqUiQQGroupList *glist;

	gchar *data_dir;
	gchar *user_dir;
	gchar *faces_dir;
	gchar *avatar_file;
	/* poll消息是一个专门的线程处理的 */
	GAsyncQueue *poll_queue;
	/* 其他http链接又是另一个线程处理的 */
	GAsyncQueue *req_queue;
	GAsyncQueue *res_queue;
	/* 多线程通信 */
	GAsyncQueue *self_queue;
	GAsyncQueue *flist_queue;
	GAsyncQueue *send_queue;
	GAsyncQueue *avatar_queue;
	GAsyncQueue *avatar_queue_r;
	GAsyncQueue *sign_queue;
	GAsyncQueue *sign_queue_r;
	GAsyncQueue *status_queue;
	GAsyncQueue *group_queue;

	/* 在接受了好友列表和群列表后才开始poll消息 */
	gboolean ready_poll;

	/* 聊天窗口 */
	GList *fchat;
	GList *gchat;
};

struct _WqqUiQQPanelClass {
	GtkWindowClass parent_class;
};

GType wqq_ui_qqpanel_get_type() G_GNUC_CONST;
WqqUiQQPanel *wqq_ui_qqpanel_new(WqqSession * session);
void wqq_ui_qqpanel_run(WqqUiQQPanel * panel);
void wqq_ui_qqpanel_set_status(WqqUiQQPanel * panel, WqqUserStatus status);
void wqq_ui_qqpanel_set_avatar_from_file(WqqUiQQPanel * panel,
										 const gchar * filepath);
void wqq_ui_qqpanel_set_nick(WqqUiQQPanel * panel, const gchar * nick);
void wqq_ui_qqpanel_set_sign(WqqUiQQPanel * panel, const gchar * sign);

G_END_DECLS
#endif
