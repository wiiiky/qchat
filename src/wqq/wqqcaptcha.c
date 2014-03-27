#include "wqqcaptcha.h"

enum {
	WQQ_CAPTCHA_PROPERTY_CONFIRM = 1,
	WQQ_CAPTCHA_PROPERTY_CODE,
	WQQ_CAPTCHA_PROPERTY_UIN,
};

G_DEFINE_TYPE(WqqCaptcha, wqq_captcha, G_TYPE_OBJECT);

static void wqq_captcha_getter(GObject * object, guint property_id,
							   GValue * value, GParamSpec * pspec);
static void wqq_captcha_setter(GObject * object, guint property_id,
							   const GValue * value, GParamSpec * pspec);

static void wqq_captcha_finalize(GObject * object);

static void wqq_captcha_init(WqqCaptcha * captcha)
{
	captcha->confirm = 1;
	captcha->code = NULL;
	captcha->uin = NULL;

	/*g_object_weak_ref(G_OBJECT(captcha), wqq_captcha_finalize, NULL); */
}

static void wqq_captcha_finalize(GObject * object)
{
	WqqCaptcha *captcha = WQQ_CAPTCHA(object);
	g_free(captcha->code);
	g_free(captcha->uin);
}

static void wqq_captcha_class_init(WqqCaptchaClass * klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->get_property = wqq_captcha_getter;
	object_class->set_property = wqq_captcha_setter;
	object_class->finalize = wqq_captcha_finalize;

	GParamSpec *ps;
	ps = g_param_spec_uint("confirm",
						   "captcha confirm",
						   "Captcha Confirm",
						   0,
						   G_MAXUINT,
						   0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_CAPTCHA_PROPERTY_CONFIRM, ps);

	ps = g_param_spec_string("code",
							 "captcha cade",
							 "Captcha Code",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_CAPTCHA_PROPERTY_CODE, ps);

	ps = g_param_spec_string("uin",
							 "user uin",
							 "User Uin",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_CAPTCHA_PROPERTY_UIN, ps);
}

static void wqq_captcha_getter(GObject * object, guint property_id,
							   GValue * value, GParamSpec * pspec)
{
	WqqCaptcha *captcha = WQQ_CAPTCHA(object);
	switch (property_id) {
	case WQQ_CAPTCHA_PROPERTY_CONFIRM:
		g_value_set_uint(value, captcha->confirm);
		break;
	case WQQ_CAPTCHA_PROPERTY_CODE:
		g_value_set_string(value, captcha->code);
		break;
	case WQQ_CAPTCHA_PROPERTY_UIN:
		g_value_set_string(value, captcha->uin);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void wqq_captcha_setter(GObject * object, guint property_id,
							   const GValue * value, GParamSpec * pspec)
{
	WqqCaptcha *captcha = WQQ_CAPTCHA(object);
	switch (property_id) {
	case WQQ_CAPTCHA_PROPERTY_CONFIRM:
		captcha->confirm = g_value_get_uint(value);
		break;
	case WQQ_CAPTCHA_PROPERTY_CODE:
		g_free(captcha->code);
		captcha->code = g_strdup(g_value_get_string(value));
		break;
	case WQQ_CAPTCHA_PROPERTY_UIN:
		g_free(captcha->uin);
		captcha->uin = g_strdup(g_value_get_string(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

/*******************************************************
 * PUBLIC
 *****************************************************/
WqqCaptcha *wqq_captcha_new()
{
	return (WqqCaptcha *) g_object_new(WQQ_TYPE_CAPTCHA, NULL);
}

WqqCaptcha *wqq_captcha_new_full(guint confirm,
								 const gchar * code, const gchar * uin)
{
	return (WqqCaptcha *) g_object_new(WQQ_TYPE_CAPTCHA,
									   "confirm", confirm,
									   "code", code, "uin", uin, NULL);
}

void wqq_captcha_set_confirm(WqqCaptcha * captcha, guint confirm)
{
	g_return_if_fail(WQQ_IS_CAPTCHA(captcha));
	captcha->confirm = confirm;
}

void wqq_captcha_set_code(WqqCaptcha * captcha, const gchar * code)
{
	g_return_if_fail(WQQ_IS_CAPTCHA(captcha));
	g_free(captcha->code);
	captcha->code = g_strdup(code);
}

void wqq_captcha_set_uin(WqqCaptcha * captcha, const gchar * uin)
{
	g_return_if_fail(WQQ_IS_CAPTCHA(captcha));
	g_free(captcha->uin);
	captcha->uin = g_strdup(uin);
}

guint wqq_captcha_get_confrim(WqqCaptcha * captcha)
{
	g_return_val_if_fail(WQQ_IS_CAPTCHA(captcha), 2);
	return captcha->confirm;
}

const gchar *wqq_captcha_get_code(WqqCaptcha * captcha)
{
	g_return_val_if_fail(WQQ_IS_CAPTCHA(captcha), NULL);
	return captcha->code;
}

const gchar *wqq_captcha_get_uin(WqqCaptcha * captcha)
{
	g_return_val_if_fail(WQQ_IS_CAPTCHA(captcha), NULL);
	return captcha->uin;
}
