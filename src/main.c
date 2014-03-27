#include "wqquilogin.h"
#include "wqquiqqpanel.h"

void on_login_success_callback(WqqUiLogin * login,
							   WqqSession * session, gpointer data);

int main(int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	WqqUiLogin *login_ui = wqq_ui_login_new();
	wqq_ui_login_load_accounts(login_ui);
	wqq_ui_login_set_callback(login_ui, on_login_success_callback, "ok");
	wqq_ui_login_run(login_ui);

	g_message("%s", PACKAGE_PREFIX);
	gtk_main();
	return 0;
}

void on_login_success_callback(WqqUiLogin * login,
							   WqqSession * session, gpointer data)
{
	wqq_ui_login_destroy(login);
	WqqUiQQPanel *panel = wqq_ui_qqpanel_new(session);
	wqq_ui_qqpanel_run(panel);
}
