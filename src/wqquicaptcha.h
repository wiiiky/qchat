#ifndef __WQQ_UI_CAPTCHA_H__
#define __WQQ_UI_CAPTCHA_H__
#include "wqq/wqq.h"

G_BEGIN_DECLS
#define WQQ_TYPE_UI_CAPTCHA	(wqq_ui_captcha_get_type())
#define WQQ_UI_CAPTCHA(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_UI_CAPTCHA,WqqUiCaptcha))
#define WQQ_IS_UI_CAPTCHA(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_UI_CAPTCHA))
#define WQQ_UI_CAPTCHA_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WQQ_TYPE_UI_CAPTCHA,WqqUiCaptchaClass))
#define WQQ_IS_UI_CAPTCHA_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass),\
				WQQ_TYPE_UI_CAPTCHA))
#define WQQ_UI_CAPTCHA_GET_CLASS(obj)	(G_TYPE_GET_INSTANCE_CLASS((obj),\
				WQQ_TYPE_UI_CAPTCHA,WqqUiCaptchaClass))
typedef struct _WqqUiCaptcha WqqUiCaptcha;
typedef struct _WqqUiCaptchaClass WqqUiCaptchaClass;


struct _WqqUiCaptcha {
	GtkDialog parent;
	WqqSession *session;
	gchar *filepath;
	GtkWidget *ebox;
	gulong signal_handler;
	GtkWidget *image;
	GtkWidget *entry;
	GtkWidget *ok_btn;
	GtkWidget *cancel_btn;
};

struct _WqqUiCaptchaClass {
	GtkDialogClass parent_class;
};

GType wqq_ui_captcha_get_type(void) G_GNUC_CONST;
WqqUiCaptcha *wqq_ui_captcha_new(WqqSession * session,
								 const gchar * filepath);
/* return the input or NULL if cancelled */
gchar *wqq_ui_captcha_run(WqqUiCaptcha * captcha);

G_END_DECLS
#endif
