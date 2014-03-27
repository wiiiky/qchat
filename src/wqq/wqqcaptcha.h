#ifndef __WQQ_CAPTCHA_H__
#define __WQQ_CAPTCHA_H__

/*
 * Wiky L(wiiiky@yeah.net)
 * 2013-12-29
 *
 * webqq登录时用到的验证码结构
 */

#include <glib-object.h>
/*#include "wqqtypes.h"*/

G_BEGIN_DECLS
#define WQQ_TYPE_CAPTCHA	(wqq_captcha_get_type())
#define WQQ_CAPTCHA(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_CAPTCHA,WqqCaptcha))
#define WQQ_IS_CAPTCHA(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_CAPTCHA))
#define WQQ_CAPTCHA_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WQQ_TYPE_CAPTCHA,WqqCaptchaClass))
#define WQQ_IS_CAPTCHA_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass),\
				WQQ_TYPE_CAPTCHA))
#define WQQ_CAPTCHA_GET_CLASS(obj)	(G_TYPE_GET_INSTANCE_CLASS((obj),\
				WQQ_TYPE_CAPTCHA,WqqCaptchaClass))
typedef struct _WqqCaptcha WqqCaptcha;
typedef struct _WqqCaptchaClass WqqCaptchaClass;

struct _WqqCaptcha {
	GObject object;
	guint confirm;				//0表示不需要验证码
	gchar *code;				//验证码，通过用户输入计算或者从服务器获取
	gchar *uin;					//与验证码一同获取的uin值
};

struct _WqqCaptchaClass {
	GObjectClass object_class;
};

GType wqq_captcha_get_type() G_GNUC_CONST;

WqqCaptcha *wqq_captcha_new();
WqqCaptcha *wqq_captcha_new_full(guint confirm,
								 const gchar * code, const gchar * uin);
void wqq_captcha_set_confirm(WqqCaptcha * captcha, guint confirm);
void wqq_captcha_set_code(WqqCaptcha * captcha, const gchar * code);
void wqq_captcha_set_uin(WqqCaptcha * captcha, const gchar * uin);
guint wqq_captcha_get_confrim(WqqCaptcha * captcha);
const gchar *wqq_captcha_get_code(WqqCaptcha * captcha);
const gchar *wqq_captcha_get_uin(WqqCaptcha * captcha);

#define WQQ_CAPTCHA_NEED_VERIFICATION(captcha)	\
	(WQQ_CAPTCHA(captcha)->confirm==1)

G_END_DECLS
#endif
