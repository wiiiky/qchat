#ifndef __WQQ_UI_LOGIN_H__
#define __WQQ_UI_LOGIN_H__
#include <libsecret/secret.h>
#include "wqq/wqq.h"

G_BEGIN_DECLS
#define WQQ_TYPE_UI_LOGIN	(wqq_ui_login_get_type())
#define WQQ_UI_LOGIN(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_UI_LOGIN,WqqUiLogin))
#define WQQ_IS_UI_LOGIN(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_UI_LOGIN))
#define WQQ_UI_LOGIN_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WQQ_TYPE_UI_LOGIN,WqqUiLoginClass))
#define WQQ_IS_UI_LOGIN_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass),\
				WQQ_TYPE_UI_LOGIN))
#define WQQ_UI_LOGIN_GET_CLASS(obj)	(G_TYPE_GET_INSTANCE_CLASS((obj),\
				WQQ_TYPE_UI_LOGIN,WqqUiLoginClass))
typedef struct _WqqUiLogin WqqUiLogin;
typedef struct _WqqUiLoginClass WqqUiLoginClass;

typedef void (*WqqUiLoginSuccessCallback) (WqqUiLogin * login,
										   WqqSession * session,
										   gpointer data);

struct _WqqUiLogin {
	GtkWindow parent;
	GtkWidget *grid;
	GtkWidget *vbox;
	GtkWidget *username_combo;
	GtkWidget *username;
	GtkWidget *password;
	GtkWidget *hbox;
	GtkWidget *keep_username;
	GtkWidget *keep_pwd;
	GtkWidget *status_combo;
	GtkWidget *login_btn;
	GtkWidget *cancel_btn;
	gboolean cancel;
	SecretSchema *secret;
	gchar *accounts_file;
	gchar *captcha_file;
	GKeyFile *accounts;

	/* 登录完成后的回调函数 */
	WqqUiLoginSuccessCallback callback;
	gpointer user_data;

	WqqSession *session;

	gchar *data_dir;
	gchar *user_dir;
};

struct _WqqUiLoginClass {
	GtkWindowClass parent_class;
};

GType wqq_ui_login_get_type() G_GNUC_CONST;

WqqUiLogin *wqq_ui_login_new();

void wqq_ui_login_load_accounts(WqqUiLogin * login);
void wqq_ui_login_save_accounts(WqqUiLogin * login);
const gchar *wqq_ui_login_get_username(WqqUiLogin * login);
const gchar *wqq_ui_login_get_password(WqqUiLogin * login);
void wqq_ui_login_set_password(WqqUiLogin * login, const gchar * pwd);
gboolean wqq_ui_login_keep_username(WqqUiLogin * login);
gboolean wqq_ui_login_keep_pwd(WqqUiLogin * login);
void wqq_ui_login_set_keep_username(WqqUiLogin * login, gboolean set);
void wqq_ui_login_set_keep_pwd(WqqUiLogin * login, gboolean set);
WqqUserStatus wqq_ui_login_get_status(WqqUiLogin * login);
const gchar *wqq_ui_login_get_status_to_string(WqqUiLogin * login);
WqqSession *wqq_ui_login_get_session(WqqUiLogin * login);
void wqq_ui_login_set_callback(WqqUiLogin * login,
							   WqqUiLoginSuccessCallback callback,
							   gpointer data);
void wqq_ui_login_run(WqqUiLogin * login);
void wqq_ui_login_destroy(WqqUiLogin * login);
void wqq_ui_login_destroy_quit(WqqUiLogin * login);

void wqq_ui_login_set_icon_from_file(WqqUiLogin * login,
									 const gchar * path);

G_END_DECLS
#endif
