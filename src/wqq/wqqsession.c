#include "wqqsession.h"
#include "wqqcrypto.h"
#include <json-glib/json-glib.h>
#include <string.h>

enum {
	WQQ_SESSION_PROPERTY_USERNAME = 1,
	WQQ_SESSION_PROPERTY_PLAIN,
	WQQ_SESSION_PROPERTY_PASSWORD,
	WQQ_SESSION_PROPERTY_SELF,	//WqqUser 
	WQQ_SESSION_PROPERTY_CLIENTID,
	WQQ_SESSION_PROPERTY_PSESSIONID,
	WQQ_SESSION_PROPERTY_VFWEBQQ,
	WQQ_SESSION_PROPERTY_PTWEBQQ,
	WQQ_SESSION_PROPERTY_LOGIN_SIG,
};

typedef struct {
	WqqSessionCallback callback;
	gpointer data;
	/* extra */
	gpointer data1;
	gpointer data2;
} CallbackData;

G_DEFINE_TYPE(WqqSession, wqq_session, SOUP_TYPE_SESSION);


static void wqq_session_getter(GObject * object, guint property_id,
							   GValue * value, GParamSpec * pspec);
static void wqq_session_setter(GObject * object, guint property_id,
							   const GValue * value, GParamSpec * pspec);

static void wqq_session_finalize(GObject * object);


static void wqq_session_init(WqqSession * session)
{
	g_object_set(G_OBJECT(session),
				 "add-feature-by-type", SOUP_TYPE_PROXY_RESOLVER_DEFAULT,
				 "add-feature-by-type", SOUP_TYPE_COOKIE_JAR,
				 "timeout", 15,
				 "async-context", g_main_context_new(),
				 "ssl-strict", FALSE,
				 "use-thread-context", TRUE,
				 "max-conns-per-host", 15, "max-conns", 50, NULL);
	session->username = NULL;
	session->plain = NULL;
	session->password = NULL;

	session->captcha = NULL;

	session->self = wqq_user_new();
	session->list=NULL;
	session->glist=NULL;

	session->clientid = NULL;
	session->psessionid = NULL;
	session->vfwebqq = NULL;
	session->ptwebqq = NULL;
	session->login_sig = NULL;

}

static void wqq_session_finalize(GObject * object)
{
	WqqSession *session = WQQ_SESSION(object);
	g_free(session->username);
	g_free(session->plain);
	g_free(session->password);
	if (session->captcha )
		g_object_unref(session->captcha);
	g_object_unref(session->self);
	g_free(session->clientid);
	g_free(session->psessionid);
	g_free(session->vfwebqq);
	g_free(session->ptwebqq);
	g_free(session->login_sig);
}

static void wqq_session_class_init(WqqSessionClass * klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->get_property = wqq_session_getter;
	object_class->set_property = wqq_session_setter;
	object_class->finalize = wqq_session_finalize;

	GParamSpec *ps;
	ps = g_param_spec_string("username",
				"user name",
				"User Nmae",
				NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
				WQQ_SESSION_PROPERTY_USERNAME, ps);

	ps = g_param_spec_string("plain",
				"plain password",
				"Plain Password",
				NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
				WQQ_SESSION_PROPERTY_PLAIN, ps);

	ps = g_param_spec_string("password",
							 "encrypted password",
							 "Encrypted Password", NULL, G_PARAM_READABLE);
	g_object_class_install_property(object_class,
									WQQ_SESSION_PROPERTY_PASSWORD, ps);

	ps = g_param_spec_object("self",
							 "user itself",
							 "Use Itself",
							 WQQ_TYPE_USER, G_PARAM_READABLE);
	g_object_class_install_property(object_class,
									WQQ_SESSION_PROPERTY_SELF, ps);

	ps = g_param_spec_string("clientid",
							 "cookie clientid",
							 "Cookie Clientid", NULL, G_PARAM_READABLE);
	g_object_class_install_property(object_class,
									WQQ_SESSION_PROPERTY_CLIENTID, ps);

	ps = g_param_spec_string("psessionid",
							 "cookie psessionid",
							 "Cookie Psessionid", NULL, G_PARAM_READABLE);
	g_object_class_install_property(object_class,
									WQQ_SESSION_PROPERTY_PSESSIONID, ps);

	ps = g_param_spec_string("vfwebqq",
							 "cookie vfwebqq",
							 "Cookie Vfwebqq", NULL, G_PARAM_READABLE);
	g_object_class_install_property(object_class,
									WQQ_SESSION_PROPERTY_VFWEBQQ, ps);

	ps = g_param_spec_string("ptwebqq",
							 "cookie ptwebqq",
							 "Cookie Ptwebqq", NULL, G_PARAM_READABLE);
	g_object_class_install_property(object_class,
									WQQ_SESSION_PROPERTY_PTWEBQQ, ps);

	ps = g_param_spec_string("login_sig",
							 "cookie login_sig",
							 "Cookie Login_sig", NULL, G_PARAM_READABLE);
	g_object_class_install_property(object_class,
									WQQ_SESSION_PROPERTY_LOGIN_SIG, ps);
}


static void wqq_session_getter(GObject * object, guint property_id,
							   GValue * value, GParamSpec * pspec)
{
	WqqSession *session = WQQ_SESSION(object);
	switch (property_id) {
	case WQQ_SESSION_PROPERTY_USERNAME:
		g_value_set_string(value, session->username);
		break;
	case WQQ_SESSION_PROPERTY_PLAIN:
		g_value_set_string(value, session->plain);
		break;
	case WQQ_SESSION_PROPERTY_PASSWORD:
		g_value_set_string(value, session->password);
		break;
	case WQQ_SESSION_PROPERTY_SELF:
		g_value_set_object(value, session->self);
		break;
	case WQQ_SESSION_PROPERTY_CLIENTID:
		g_value_set_string(value, session->clientid);
		break;
	case WQQ_SESSION_PROPERTY_VFWEBQQ:
		g_value_set_string(value, session->vfwebqq);
		break;
	case WQQ_SESSION_PROPERTY_PTWEBQQ:
		g_value_set_string(value, session->ptwebqq);
		break;
	case WQQ_SESSION_PROPERTY_PSESSIONID:
		g_value_set_string(value, session->psessionid);
		break;
	case WQQ_SESSION_PROPERTY_LOGIN_SIG:
		g_value_set_string(value, session->login_sig);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void wqq_session_setter(GObject * object, guint property_id,
							   const GValue * value, GParamSpec * pspec)
{
	WqqSession *session = WQQ_SESSION(object);
	switch (property_id) {
	case WQQ_SESSION_PROPERTY_USERNAME:
		g_free(session->username);
		session->username = g_strdup(g_value_get_string(value));
		break;
	case WQQ_SESSION_PROPERTY_PLAIN:
		g_free(session->plain);
		session->plain = g_strdup(g_value_get_string(value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

/*
 * 将soup的状态转化为wqq的状态
 */
static guint convert_soup_status_to_wqq_status(guint status)
{
	if (SOUP_STATUS_IS_SUCCESSFUL(status))
		return WQQ_SESSION_HTTP_STATUS_SUCCESSFUL;
	else if (SOUP_STATUS_IS_CLIENT_ERROR(status))
		return WQQ_SESSION_HTTP_STATUS_CLIENT_ERROR;
	else if (SOUP_STATUS_IS_INFORMATIONAL(status))
		return WQQ_SESSION_HTTP_STATUS_INFOMATIONAL;
	else if (SOUP_STATUS_IS_REDIRECTION(status))
		return WQQ_SESSION_HTTP_STATUS_REDIRECTION;
	else if (SOUP_STATUS_IS_SERVER_ERROR(status))
		return WQQ_SESSION_HTTP_STATUS_SERVER_ERROR;
	else if (SOUP_STATUS_IS_TRANSPORT_ERROR(status))
		return WQQ_SESSION_HTTP_STATUS_TRANSPORT_ERROR;
	return WQQ_SESSION_MALFORMED;
}

static void _wqq_session_create_password(WqqSession * session)
{
	g_free(session->password);
	session->password =
		wqq_login_encrypt(wqq_captcha_get_uin(session->captcha),
						  session->plain,
						  wqq_captcha_get_code(session->captcha));
}

/* 更新clientid，一个随机的7-8位整数 */
static void _wqq_session_update_clientid(WqqSession * session)
{
	g_free(session->clientid);

	session->clientid = g_strdup_printf("%d",
										g_random_int_range(1000000,
														   99999999));
}

/* 获取cookie */
static gchar *_wqq_session_get_cookie(WqqSession * session,
									  const gchar * name)
{
	SoupCookieJar *cj =
		SOUP_COOKIE_JAR(soup_session_get_feature(SOUP_SESSION(session),
												 SOUP_TYPE_COOKIE_JAR));

	GSList *list = soup_cookie_jar_all_cookies(cj);
	gchar *value = NULL;
	if (list != NULL) {
		GSList *lp = list;
		while (lp != NULL) {
			SoupCookie *cookie = (SoupCookie *) lp->data;
			if (g_strcmp0(name, soup_cookie_get_name(cookie)) == 0) {
				value = g_strdup(soup_cookie_get_value(cookie));
				break;
			}
			lp = g_slist_next(lp);
		}
		g_slist_free_full(list, (GDestroyNotify) soup_cookie_free);
	}
	return value;
}

/***********************************************************
 * PUBLIC
 ***********************************************************/
WqqSession *wqq_session_new(const gchar * username, const gchar * plain)
{
	WqqSession *session = (WqqSession *) g_object_new(WQQ_TYPE_SESSION,
													  "username", username,
													  "plain", plain,
													  NULL);

	return session;
}

void wqq_session_set_username(WqqSession * session, const gchar * username)
{
	g_return_if_fail(WQQ_IS_SESSION(session));
	g_free(session->username);
	session->username = g_strdup(username);
}

void wqq_session_set_plain(WqqSession * session, const gchar * plain)
{
	g_return_if_fail(WQQ_IS_SESSION(session));
	g_free(session->plain);
	session->plain = g_strdup(plain);
}

void wqq_session_set_captcha_code(WqqSession * session, const gchar * code)
{
	g_return_if_fail(WQQ_IS_SESSION(session) && session->captcha != NULL);
	wqq_captcha_set_code(session->captcha, code);
}

const gchar *wqq_session_get_username(WqqSession * session)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), NULL);
	return session->username;
}

const gchar *wqq_session_get_plain(WqqSession * session)
{
	g_return_val_if_fail(WQQ_SESSION(session), NULL);
	return session->plain;
}

const gchar *wqq_session_get_captcha_code(WqqSession * session)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session)
						 && session->captcha != NULL, NULL);
	return wqq_captcha_get_code(session->captcha);
}

WqqUser *wqq_session_get_user_self(WqqSession * session)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), NULL);

	return session->self;
}

/*************************************************
 * 与webqq相关
 **************************************************/



/* ***************************************************
 * 登录
 **************************************************/
guint wqq_session_login(WqqSession * session)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session) &&
						 session->username != NULL
						 && session->plain != NULL, WQQ_SESSION_MALFORMED);

	_wqq_session_create_password(session);
	gchar *target = g_strdup_printf(WQQ_WEBQQ_LOGIN_URL,
									session->username, session->password,
									wqq_captcha_get_code
									(session->captcha));
	SoupMessage *msg = soup_message_new("GET", target);
	g_free(target);
	guint status = soup_session_send_message(SOUP_SESSION(session), msg);
	status = convert_soup_status_to_wqq_status(status);
	if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL) {
		g_object_unref(msg);
		return status;
	}

	/* 解析返回结果 */
	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	gchar **str = g_strsplit(body->data, "'", -1);
	if (str[1][0] == '3') {		/* 密码错误 */
		status = WQQ_SESSION_LOGIN_STATUS_PASSWORD_ERROR;
		goto OUT;
	} else if (str[1][0] == '4') {	/* 验证码错误 */
		status = WQQ_SESSION_LOGIN_STATUS_VERIFY_CODE_ERRROR;
		goto OUT;
	} else if (str[1][0] == '0') {	/* 登录成功 */
		/* 对返回的url执行一次GET，获取必要的cookie */
		g_object_unref(msg);
		msg = soup_message_new("GET", str[5]);
		wqq_user_set_nick(WQQ_SESSION(session)->self, str[11]);
		status = soup_session_send_message(SOUP_SESSION(session), msg);
		status = convert_soup_status_to_wqq_status(status);
	} else
		status = WQQ_SESSION_LOGIN_STATUS_UNKNOWN;

  OUT:
	g_object_unref(msg);
	g_strfreev(str);
	return status;
}

static void _wqq_session_login_first_step_callback(SoupSession * session,
												   SoupMessage * msg,
												   gpointer data)
{
	CallbackData *cdata = (CallbackData *) data;
	SoupSessionCallback callback = cdata->callback;
	gpointer user_data = cdata->data;
	g_free(data);
	guint status;
	g_object_get(msg, "status-code", &status, NULL);

	if (!SOUP_STATUS_IS_SUCCESSFUL(status)) {
		goto OUT;
	}

	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	gchar **str = g_strsplit(body->data, "'", -1);
	if (str[1][0] == '3') {		/* 失败 */
		soup_message_set_status(msg,
								WQQ_SESSION_LOGIN_STATUS_PASSWORD_ERROR);
	} else if (str[1][0] == '4') {
		soup_message_set_status(msg,
								WQQ_SESSION_LOGIN_STATUS_VERIFY_CODE_ERRROR);
	} else if (str[1][0] == '0') {	/* 登录成功 */
		/* 对返回的url执行一次GET，获取必要的cookie */
		SoupMessage *message = soup_message_new("GET", str[5]);
		wqq_user_set_nick(WQQ_SESSION(session)->self, str[11]);
		soup_session_queue_message(session, message, callback, user_data);
		g_strfreev(str);
		return;
	} else {
		soup_message_set_status(msg, WQQ_SESSION_LOGIN_STATUS_UNKNOWN);
	}

	g_strfreev(str);
  OUT:
	if (callback != NULL)
		callback(session, msg, user_data);
}

void wqq_session_login_async(WqqSession * session,
							 WqqSessionCallback callback, gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session) &&
					 session->username != NULL && session->plain != NULL);

	_wqq_session_create_password(session);
	gchar *target = g_strdup_printf(WQQ_WEBQQ_LOGIN_URL,
									session->username, session->password,
									wqq_captcha_get_code
									(session->captcha));
	SoupMessage *msg = soup_message_new("GET", target);
	g_free(target);
	CallbackData *cdata = g_malloc(sizeof(CallbackData));
	cdata->callback = callback;
	cdata->data = data;
	soup_session_queue_message(SOUP_SESSION(session), msg,
							   _wqq_session_login_first_step_callback,
							   cdata);
}

/*************************************************
 * 检查是否需要验证码
 *****************************************************/

guint wqq_session_check_verify_code(WqqSession * session)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), WQQ_SESSION_MALFORMED);

	gchar *target = g_strdup_printf(WQQ_WEBQQ_CHECK_VERIFICATION_URL,
									session->username);
	SoupMessage *msg = soup_message_new("GET", target);
	g_free(target);
	guint status = soup_session_send_message(SOUP_SESSION(session), msg);
	status = convert_soup_status_to_wqq_status(status);
	if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL) {
		g_object_unref(msg);
		return status;
	}

	SoupMessageBody *body;
	g_object_get(G_OBJECT(msg), "response-body", &body, NULL);
	gchar **str = g_strsplit(body->data, "'", -1);

	if (session->captcha != NULL)
		g_object_unref(session->captcha);
	session->captcha = wqq_captcha_new_full(str[1][0] == '0' ? 0 : 1,
											str[3], str[5]);
	g_strfreev(str);
	g_object_unref(msg);

	return status;
}

static void _wqq_session_check_verify_code_callback(SoupSession * session,
													SoupMessage * msg,
													gpointer data)
{
	CallbackData *cdata = (CallbackData *) data;
	SoupSessionCallback callback = cdata->callback;
	gpointer user_data = cdata->data;
	g_free(data);

	guint status;
	g_object_get(msg, "status-code", &status, NULL);
	if (!SOUP_STATUS_IS_SUCCESSFUL(status)) {
		goto OUT;
	}

	SoupMessageBody *body;
	g_object_get(G_OBJECT(msg), "response-body", &body, NULL);
	gchar **str = g_strsplit(body->data, "'", -1);

	if (WQQ_SESSION(session)->captcha != NULL)
		g_object_unref(WQQ_SESSION(session)->captcha);
	WQQ_SESSION(session)->captcha =
		wqq_captcha_new_full(str[1][0] == '0' ? 0 : 1, str[3], str[5]);
	g_strfreev(str);
  OUT:
	if (callback != NULL)
		callback(session, msg, user_data);
}

void wqq_session_check_verify_code_async(WqqSession * session,
										 WqqSessionCallback callback,
										 gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session));

	gchar *target = g_strdup_printf(WQQ_WEBQQ_CHECK_VERIFICATION_URL,
									session->username);
	SoupMessage *msg = soup_message_new("GET", target);
	g_free(target);

	CallbackData *cdata = g_malloc(sizeof(CallbackData));
	cdata->callback = callback;
	cdata->data = data;
	soup_session_queue_message(SOUP_SESSION(session), msg,
							   _wqq_session_check_verify_code_callback,
							   cdata);
}


/****************************************************
 * 获取验证码图片
 * 保存在path
 ********************************************/
guint wqq_session_get_captcha_image(WqqSession * session,
									const gchar * path)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), WQQ_SESSION_MALFORMED);

	gchar *target = g_strdup_printf(WQQ_WEBQQ_GET_CAPTCHA_IMAGE_URL,
									session->username,
									(gdouble) g_random_int() /
									100000000.00);

	SoupMessage *msg = soup_message_new("GET", target);
	g_free(target);
	guint status = soup_session_send_message(SOUP_SESSION(session), msg);
	status = convert_soup_status_to_wqq_status(status);
	if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL) {
		g_object_unref(msg);
		return status;
	}
	SoupMessageBody *body;
	g_object_get(G_OBJECT(msg), "response-body", &body, NULL);
	GFile *file = g_file_new_for_path(path);
	g_file_replace_contents(file, body->data, -1,
							NULL, FALSE,
							G_FILE_CREATE_NONE, NULL, NULL, NULL);

	g_object_unref(file);
	g_object_unref(msg);
	return status;
}


static void _wqq_session_get_captcha_image_callback(SoupSession * session,
													SoupMessage * msg,
													gpointer data)
{
	CallbackData *cdata = (CallbackData *) data;
	SoupSessionCallback callback = cdata->callback;
	gpointer user_data = cdata->data;
	gchar *path = (gchar *) cdata->data1;
	g_free(data);

	guint status;
	g_object_get(msg, "status-code", &status, NULL);
	if (!SOUP_STATUS_IS_SUCCESSFUL(status)) {
		goto OUT;
	}
	SoupMessageBody *body;
	g_object_get(G_OBJECT(msg), "response-body", &body, NULL);
	GFile *file = g_file_new_for_path(path);
	g_file_replace_contents(file, body->data, -1,
							NULL, FALSE,
							G_FILE_CREATE_NONE, NULL, NULL, NULL);

	g_object_unref(file);
  OUT:
	if (callback != NULL)
		callback(session, msg, user_data);
}

void wqq_session_get_captcha_image_async(WqqSession * session,
										 const gchar * path,
										 WqqSessionCallback callback,
										 gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session));

	gchar *target = g_strdup_printf(WQQ_WEBQQ_GET_CAPTCHA_IMAGE_URL,
									session->username,
									(gdouble) g_random_int() /
									100000000.00);

	CallbackData *cdata = g_malloc(sizeof(CallbackData));
	cdata->callback = callback;
	cdata->data = data;
	cdata->data1 = (gpointer) path;
	SoupMessage *msg = soup_message_new("GET", target);
	g_free(target);
	soup_session_queue_message(SOUP_SESSION(session), msg,
							   _wqq_session_get_captcha_image_callback,
							   cdata);
}

/**********************************
 * 设置用户的在线状态
 **************************************/

static void _wqq_session_set_status_ret(WqqSession * session,
										const gchar * data)
{
	JsonParser *parser = json_parser_new();
	json_parser_load_from_data(parser, data, -1, NULL);
	JsonNode *root = json_parser_get_root(parser);
	JsonObject *root_obj = json_node_get_object(root);
	gint64 retcode = json_object_get_int_member(root_obj, "retcode");
	JsonObject *result = json_object_get_object_member(root_obj, "result");
	guint64 uin = json_object_get_int_member(result, "uin");
	guint64 cip = json_object_get_int_member(result, "cip");	/* TODO? */
	WqqUserStatus user_status =
		_wqq_user_status_to_enum(json_object_get_string_member
								 (result, "status"));
	const gchar *vfwebqq =
		json_object_get_string_member(result, "vfwebqq");
	const gchar *psessionid =
		json_object_get_string_member(result, "psessionid");

	wqq_user_set_uin(session->self, uin);
	wqq_user_set_status(session->self, user_status);
	g_free(session->vfwebqq);
	g_free(session->psessionid);
	session->vfwebqq = g_strdup(vfwebqq);
	session->psessionid = g_strdup(psessionid);
	g_object_unref(parser);
}

static SoupMessage *_wqq_session_set_status_message(WqqSession * session,
													WqqUserStatus status)
{
	_wqq_session_update_clientid(session);
	g_free(session->ptwebqq);
	session->ptwebqq = _wqq_session_get_cookie(session, "ptwebqq");
	const gchar *status_string = _wqq_user_status_to_string(status);
	gchar *data = g_strdup_printf(WQQ_WEBQQ_SET_STATUS_POST_DATA,
								  status_string, session->ptwebqq,
								  session->clientid,
								  session->psessionid);
	gchar *escape = g_uri_escape_string(data, NULL, FALSE);
	g_free(data);
	data = g_strdup_printf("r=%s&clientid=%s&psessionid=%s", escape,
						   session->clientid, session->psessionid);
	g_free(escape);
	/*g_printf("POST : %s\n", data); */
	SoupMessage *msg = soup_message_new("POST", WQQ_WEBQQ_SET_STATUS_URL);
	soup_message_set_request(msg, "application/x-www-form-urlencoded",
							 SOUP_MEMORY_TAKE, data, strlen(data));
	SoupMessageHeaders *headers;
	g_object_get(msg, "request-headers", &headers, NULL);
	soup_message_headers_append(headers,
								"Referer",
								"http://d.web2.qq.com/proxy.html?"
								"v=20110331002&callback=1&id=3");

	return msg;
}

guint wqq_session_set_status(WqqSession * session, WqqUserStatus status)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), WQQ_SESSION_MALFORMED);

	SoupMessage *msg = _wqq_session_set_status_message(session, status);
	guint st = soup_session_send_message(SOUP_SESSION(session), msg);
	st = convert_soup_status_to_wqq_status(st);
	if (st != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL)
		goto FAIL;

	/* 解析返回结果(貌似不会失败,因此我就不判断retcode的返回值了) */
	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	_wqq_session_set_status_ret(session, body->data);
  FAIL:
	g_object_unref(msg);
	return st;
}

static void _wqq_session_set_status_callback(SoupSession * session,
											 SoupMessage * msg,
											 gpointer data)
{
	CallbackData *cdata = (CallbackData *) data;
	WqqSessionCallback callback = cdata->callback;
	gpointer user_data = cdata->data;
	guint status;
	g_object_get(msg, "status-code", &status, NULL);
	if (!SOUP_STATUS_IS_SUCCESSFUL(status)) {
		goto OUT;
	}
	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	_wqq_session_set_status_ret(WQQ_SESSION(session), body->data);
  OUT:
	if (callback != NULL)
		callback(session, msg, user_data);
}

void wqq_session_set_status_async(WqqSession * session,
								  WqqUserStatus status,
								  WqqSessionCallback callback,
								  gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session));

	SoupMessage *msg = _wqq_session_set_status_message(session, status);
	CallbackData *cdata = (CallbackData *) g_malloc(sizeof(CallbackData));
	cdata->callback = callback;
	cdata->data = data;

	soup_session_queue_message(SOUP_SESSION(session), msg,
							   _wqq_session_set_status_callback, cdata);
}

/***************************************
 * 获取用户的个性签名
 ***************************************/

static void _wqq_session_get_user_sign_ret(WqqUser * user,
										   const gchar * data)
{
	JsonParser *parser = json_parser_new();
	json_parser_load_from_data(parser, data, -1, NULL);
	JsonNode *root = json_parser_get_root(parser);
	JsonObject *root_obj = json_node_get_object(root);
	gint64 retcode = json_object_get_int_member(root_obj, "retcode");
	if (retcode != 0) {			/* 失败 */
		goto OUT;
	}
	JsonArray *array = json_object_get_array_member(root_obj, "result");
	JsonObject *result = json_array_get_object_element(array, 0);
	guint64 uin = json_object_get_int_member(result, "uin");
	const gchar *lnick = json_object_get_string_member(result, "lnick");

	if (uin != wqq_user_get_uin(user))
		goto OUT;
	wqq_user_set_sign(user, lnick);
  OUT:
	g_object_unref(parser);
}

static SoupMessage *_wqq_session_get_user_sign_message(WqqSession *
													   session,
													   WqqUser * user)
{
	GDateTime *dt = g_date_time_new_now_local();
	gchar *target = g_strdup_printf(WQQ_WEBQQ_GET_USER_SIGN_URL,
									wqq_user_get_uin(user),
									session->vfwebqq,
									g_date_time_to_unix(dt));
	g_date_time_unref(dt);
	SoupMessage *msg = soup_message_new("GET", target);
	g_free(target);

	SoupMessageHeaders *headers;
	g_object_get(msg, "request-headers", &headers, NULL);
	soup_message_headers_append(headers,
								"Referer",
								"http://s.web2.qq.com/proxy.html?v=20110412001&callback=1&id=3");

	return msg;
}

guint wqq_session_get_user_sign(WqqSession * session, WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session) && WQQ_IS_USER(user),
						 WQQ_SESSION_MALFORMED);

	SoupMessage *msg = _wqq_session_get_user_sign_message(session, user);
	guint status = soup_session_send_message(SOUP_SESSION(session), msg);
	status = convert_soup_status_to_wqq_status(status);
	if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL) {
		goto OUT;
	}

	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	_wqq_session_get_user_sign_ret(user, body->data);

  OUT:
	g_object_unref(msg);
	return status;
}

guint wqq_session_get_self_sign(WqqSession * session)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), WQQ_SESSION_MALFORMED);
	return wqq_session_get_user_sign(session, session->self);
}

static void _wqq_session_get_user_sign_callback(SoupSession * session,
												SoupMessage * msg,
												gpointer data)
{
	CallbackData *cdata = (CallbackData *) data;
	WqqSessionCallback callback = cdata->callback;
	gpointer user_data = cdata->data;
	WqqUser *user = (WqqUser *) cdata->data1;
	g_free(data);

	guint status;
	g_object_get(msg, "status-code", &status, NULL);
	if (!SOUP_STATUS_IS_SUCCESSFUL(status)) {
		goto OUT;
	}
	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	_wqq_session_get_user_sign_ret(user, body->data);
  OUT:
	if (callback != NULL)
		callback(session, msg, user_data);
}

void wqq_session_get_user_sign_async(WqqSession * session, WqqUser * user,
									 WqqSessionCallback callback,
									 gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session) && WQQ_IS_USER(user));

	SoupMessage *msg = _wqq_session_get_user_sign_message(session, user);
	CallbackData *cdata = (CallbackData *) g_malloc(sizeof(CallbackData));
	cdata->callback = callback;
	cdata->data = data;
	cdata->data1 = user;
	soup_session_queue_message(SOUP_SESSION(session), msg,
							   _wqq_session_get_user_sign_callback, cdata);
}

void wqq_session_get_self_sign_async(WqqSession * session,
									 WqqSessionCallback callback,
									 gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session));

	wqq_session_get_user_sign_async(session, session->self,
									callback, data);
}


/*
 * 获取用户头像
 */
static SoupMessage *_wqq_session_get_user_face_message(WqqSession *
													   session,
													   WqqUser * user)
{
	static gint face = 1;
	gchar *target = g_strdup_printf(WQQ_WEBQQ_GET_USER_FACE_URL,
									face++,
									wqq_user_get_uin(user),
									session->vfwebqq);
	if (face == 9)
		face = 1;
	SoupMessage *msg = soup_message_new("GET", target);
	g_free(target);
	SoupMessageHeaders *headers;
	g_object_get(msg, "request-headers", &headers, NULL);
	soup_message_headers_append(headers,
								"Referer",
								"http://web2.qq.com/webqq.html");
	return msg;
}

guint wqq_session_get_user_face(WqqSession * session, WqqUser * user,
								const gchar * path)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session) && WQQ_IS_USER(user),
						 WQQ_SESSION_MALFORMED);
	SoupMessage *msg = _wqq_session_get_user_face_message(session, user);
	guint status = soup_session_send_message(SOUP_SESSION(session), msg);
	status = convert_soup_status_to_wqq_status(status);
	if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL)
		goto OUT;

	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	GFile *file = g_file_new_for_path(path);
	g_file_replace_contents(file,
							body->data,
							body->length,
							NULL,
							FALSE, G_FILE_CREATE_NONE, NULL, NULL, NULL);
	g_object_unref(file);
  OUT:
	g_object_unref(msg);
	return status;
}

static void _wqq_session_get_user_face_callback(SoupSession * session,
												SoupMessage * msg,
												gpointer data)
{
	CallbackData *cdata = (CallbackData *) data;
	WqqSessionCallback callback = cdata->callback;
	gpointer user_data = cdata->data;
	gchar *path = (gchar *) cdata->data1;
	g_free(data);
	guint status;
	g_object_get(msg, "status-code", &status, NULL);
	if (!SOUP_STATUS_IS_SUCCESSFUL(status))
		goto OUT;

	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	GFile *file = g_file_new_for_path(path);
	g_file_replace_contents(file,
							body->data,
							body->length,
							NULL,
							FALSE, G_FILE_CREATE_NONE, NULL, NULL, NULL);
	g_object_unref(file);
  OUT:
	g_free(path);
	if (callback != NULL)
		callback(session, msg, user_data);
}

void wqq_session_get_user_face_async(WqqSession * session, WqqUser * user,
									 const gchar * path,
									 WqqSessionCallback callback,
									 gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session) && WQQ_IS_USER(user));
	CallbackData *cdata = (CallbackData *) g_malloc(sizeof(CallbackData));
	cdata->callback = callback;
	cdata->data = data;
	cdata->data1 = (void *) g_strdup(path);
	SoupMessage *msg = _wqq_session_get_user_face_message(session, user);
	soup_session_queue_message(SOUP_SESSION(session), msg,
							   _wqq_session_get_user_face_callback, cdata);
}

guint wqq_session_get_self_face(WqqSession * session, const gchar * path)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), WQQ_SESSION_MALFORMED);
	return wqq_session_get_user_face(session, session->self, path);
}

void wqq_session_get_self_face_async(WqqSession * session,
									 const gchar * path,
									 WqqSessionCallback callback,
									 gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session));
	wqq_session_get_user_face_async(session, session->self,
									path, callback, data);
}

guint wqq_session_get_user_face_by_uin(WqqSession * session, guint64 uin,
									   const gchar * path)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), WQQ_SESSION_MALFORMED);
	WqqFriend *fri = wqq_session_find_friend_by_uin(session, uin);
	if (fri != NULL)
		return wqq_session_get_user_face(session, WQQ_USER(fri), path);
	return WQQ_SESSION_MALFORMED;
}

void wqq_session_get_user_face_by_uin_async(WqqSession * session,
											guint64 uin,
											const gchar * path,
											WqqSessionCallback callback,
											gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session));
	WqqFriend *fri = wqq_session_find_friend_by_uin(session, uin);
	if (fri != NULL)
		wqq_session_get_user_face_async(session, WQQ_USER(fri),
										path, callback, data);
}

/*
 * 获取好友列表
 */
static SoupMessage *_wqq_session_get_friends_message(WqqSession * session)
{
	SoupMessage *msg = soup_message_new("POST",
										WQQ_WEBQQ_GET_FRIENDS_LIST_URL);

	gchar *data = g_strdup_printf("r={\"vfwebqq\":\"%s\","
								  "\"hash\":\"%s\"}",
								  session->vfwebqq,
								  getHash(wqq_user_get_uin(session->self),
										  session->ptwebqq));
	soup_message_set_request(msg,
							 "application/x-www-form-urlencoded",
							 SOUP_MEMORY_TAKE, data, strlen(data));
	SoupMessageHeaders *headers;
	g_object_get(msg, "request-headers", &headers, NULL);
	soup_message_headers_append(headers, "Referer",
								"http://s.web2.qq.com/proxy.html?"
								"v=20110412001&callback=1&id=3");
	return msg;
}

guint _wqq_session_get_friends_ret(WqqSession * session,
								   const gchar * data)
{
	/* 解析好友列表 */
	JsonParser *parser = json_parser_new();
	json_parser_load_from_data(parser, data, -1, NULL);
	JsonNode *root = json_parser_get_root(parser);
	JsonObject *root_obj = json_node_get_object(root);
	gint64 retcode = json_object_get_int_member(root_obj, "retcode");
	if (retcode != 0) {			/* 失败 */
		goto OUT;
	}
	JsonObject *result = json_object_get_object_member(root_obj, "result");
	/* 分组 categories */
	JsonArray *categories =
		json_object_get_array_member(result, "categories");
	guint category_len = json_array_get_length(categories);
	guint i;
	for (i = 0; i < category_len; i++) {
		JsonObject *category =
			json_array_get_object_element(categories, i);
		guint index = json_object_get_int_member(category, "index");
		guint sort = json_object_get_int_member(category, "sort");
		const gchar *name =
			json_object_get_string_member(category, "name");
		WqqCategory *wc =
			wqq_session_append_category(session, name, index, sort);
	}
	WqqCategory *wc = wqq_session_find_category_by_index(session, 0);
	if (wc == NULL) {
		wc = wqq_session_append_category(session, "我的好友", 0, 0);
	}
	/* 好友信息 friends info */
	JsonArray *friends = json_object_get_array_member(result, "friends");
	JsonArray *vipinfo = json_object_get_array_member(result, "vipinfo");
	JsonArray *info = json_object_get_array_member(result, "info");
	guint friend_len = json_array_get_length(friends);
	for (i = 0; i < friend_len; i++) {
		/* friends */
		JsonObject *friend = json_array_get_object_element(friends, i);
		guint flag = json_object_get_int_member(friend, "flag");
		guint64 uin = json_object_get_int_member(friend, "uin");
		guint index = json_object_get_int_member(friend, "categories");
		WqqFriend *fri =
			wqq_session_append_friend_by_uin(session, index, uin);
		/* vipinfo */
		JsonObject *vip = json_array_get_object_element(vipinfo, i);
		guint vip_level = json_object_get_int_member(vip, "vip_level");
		guint64 u = json_object_get_int_member(vip, "u");
		guint is_vip = json_object_get_int_member(vip, "is_vip");
		wqq_user_set_vip(WQQ_USER(fri), is_vip);
		wqq_user_set_vip_level(WQQ_USER(fri), vip_level);
		/* info */
		JsonObject *finfo = json_array_get_object_element(info, i);
		guint face = json_object_get_int_member(finfo, "face");
		guint64 flag_ = json_object_get_int_member(finfo, "flag");
		const gchar *nick = json_object_get_string_member(finfo, "nick");
		guint64 uin_ = json_object_get_int_member(finfo, "uin");
		wqq_user_set_face(WQQ_USER(fri), face);
		wqq_user_set_nick(WQQ_USER(fri), nick);
		wqq_user_set_status(WQQ_USER(fri), WQQ_USER_STATUS_OFFLINE);
	}
	/* 好友备注 */
	JsonArray *marknames =
		json_object_get_array_member(result, "marknames");
	guint markname_len = json_array_get_length(marknames);
	for (i = 0; i < markname_len; i++) {
		JsonObject *markname = json_array_get_object_element(marknames, i);
		guint64 uin = json_object_get_int_member(markname, "uin");
		const gchar *name =
			json_object_get_string_member(markname, "markname");
		guint type = json_object_get_int_member(markname, "type");
		WqqFriend *fri = wqq_session_find_friend_by_uin(session, uin);
		if (fri) {
			wqq_friend_set_markanme(fri, name);
		}
	}
	wqq_session_sort_category_and_friends(session);
  OUT:
	g_object_unref(parser);
	return retcode;
}

guint wqq_session_get_friends_list(WqqSession * session)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), WQQ_SESSION_MALFORMED);

	SoupMessage *msg = _wqq_session_get_friends_message(session);
	guint status = soup_session_send_message(SOUP_SESSION(session), msg);
	status = convert_soup_status_to_wqq_status(status);
	if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL) {
		/*g_printf("failed!\n"); */
		goto OUT;
	}

	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	if (_wqq_session_get_friends_ret(session, body->data) != 0){
		status=WQQ_SESSION_STATUS_FAILED;
		goto OUT;
	}

OUT:
	g_object_unref(msg);
	return status;
}


void _wqq_session_get_friends_callback(SoupSession * session,
									   SoupMessage * msg, gpointer data)
{
	CallbackData *cdata = (CallbackData *) data;
	WqqSessionCallback callback = cdata->callback;
	gpointer user_data = cdata->data;
	g_free(data);

	guint status;
	g_object_get(msg, "status-code", &status, NULL);
	if (!SOUP_STATUS_IS_SUCCESSFUL(status)) {
		goto OUT;
	}
	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	if (_wqq_session_get_friends_ret(WQQ_SESSION(session),
									 body->data) != 0)
		soup_message_set_status(msg, WQQ_SESSION_STATUS_FAILED);
  OUT:
	if (callback != NULL)
		callback(session, msg, user_data);
}

void wqq_session_get_friends_list_async(WqqSession * session,
										WqqSessionCallback callback,
										gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session));
	SoupMessage *msg = _wqq_session_get_friends_message(session);
	CallbackData *cdata = (CallbackData *) g_malloc(sizeof(CallbackData));
	cdata->callback = callback;
	cdata->data = data;
	soup_session_queue_message(SOUP_SESSION(session), msg,
							   _wqq_session_get_friends_callback, cdata);
}

/*
 * 获取好友信息
 */

static SoupMessage *_wqq_session_get_user_info_message(WqqSession *
													   session,
													   WqqUser * user)
{
	GDateTime *dt = g_date_time_new_now_local();
	gchar *target = g_strdup_printf(WQQ_WEBQQ_GET_USER_INFO_URL,
									wqq_user_get_uin(user),
									session->vfwebqq,
									g_date_time_to_unix(dt));
	g_date_time_unref(dt);
	SoupMessage *msg = soup_message_new("GET", target);
	g_free(target);
	SoupMessageHeaders *headers;
	g_object_get(msg, "request-headers", &headers, NULL);
	soup_message_headers_append(headers,
								"Referer",
								"http://s.web2.qq.com/proxy.html?v=20110412001&callback=1&id=1");

	return msg;
}

static void _wqq_user_set_client_type_from_uint(WqqUser * user,
												guint client_type)
{
	if (client_type == 41)
		wqq_user_set_client_type(user, WQQ_CLIENT_TYPE_WEBQQ);
	else if (client_type == 42)
		wqq_user_set_client_type(user, WQQ_CLIENT_TYPE_PAD);
	else if (client_type >= 21 && client_type <= 24)
		wqq_user_set_client_type(user, WQQ_CLIENT_TYPE_MOBILE);
	else
		wqq_user_set_client_type(user, WQQ_CLIENT_TYPE_PC);
}

static guint _wqq_session_get_user_info_ret(WqqUser * user,
											const gchar * data)
{
	JsonParser *parser = json_parser_new();
	json_parser_load_from_data(parser, data, -1, NULL);
	JsonNode *root = json_parser_get_root(parser);
	JsonObject *root_obj = json_node_get_object(root);
	guint retcode = json_object_get_int_member(root_obj, "retcode");
	if (retcode != 0)
		goto OUT;
	JsonObject *result = json_object_get_object_member(root_obj, "result");
	guint face = json_object_get_int_member(result, "face");
	JsonObject *birthday =
		json_object_get_object_member(result, "birthday");
	guint month = json_object_get_int_member(birthday, "month");
	guint year = json_object_get_int_member(birthday, "year");
	guint day = json_object_get_int_member(birthday, "day");
	const gchar *occupation =
		json_object_get_string_member(result, "occupation");
	const gchar *phone = json_object_get_string_member(result, "phone");
	guint allow = json_object_get_int_member(result, "allow");
	const gchar *college =
		json_object_get_string_member(result, "college");
	guint64 uin = json_object_get_int_member(result, "uin");
	guint constel = json_object_get_int_member(result, "constel");
	guint blood = json_object_get_int_member(result, "blood");
	const gchar *homepage =
		json_object_get_string_member(result, "homepage");
	guint stat = json_object_get_int_member(result, "stat");
	guint vip_info = json_object_get_int_member(result, "vip_info");
	const gchar *country =
		json_object_get_string_member(result, "country");
	const gchar *city = json_object_get_string_member(result, "city");
	const gchar *personal =
		json_object_get_string_member(result, "personal");
	const gchar *nick = json_object_get_string_member(result, "nick");
	guint shengxiao = json_object_get_int_member(result, "shengxiao");
	const gchar *email = json_object_get_string_member(result, "email");
	const gchar *province =
		json_object_get_string_member(result, "province");
	const gchar *gender = json_object_get_string_member(result,
														"gender");
	const gchar *mobile = json_object_get_string_member(result,
														"mobile");

	wqq_user_set_face(user, face);
	wqq_user_set_birth(user, year, month, day);
	wqq_user_set_occupation(user, occupation);
	wqq_user_set_phone(user, phone);
	wqq_user_set_allow(user, allow);
	wqq_user_set_college(user, college);
	wqq_user_set_uin(user, uin);
	/*wqq_user_set_zodiac(); */
	/*wqq_user_set_blood_type(user,); */
	wqq_user_set_homepage(user, homepage);
	wqq_user_set_vip(user, vip_info);
	wqq_user_set_location(user, country, province, city);
	wqq_user_set_personal(user, personal);
	wqq_user_set_nick(user, nick);
	wqq_user_set_shengxiao(user, shengxiao);
	wqq_user_set_email(user, email);
	if (json_object_get_member(result, "client_type")) {
		gint client_type =
			json_object_get_int_member(result, "client_type");
		_wqq_user_set_client_type_from_uint(user, client_type);
	}
	wqq_user_set_gender(user, gender);
	wqq_user_set_mobile(user, mobile);

  OUT:
	g_object_unref(parser);
	return retcode;
}

guint wqq_session_get_user_info(WqqSession * session, WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session) && WQQ_IS_USER(user),
						 WQQ_SESSION_MALFORMED);
	SoupMessage *msg = _wqq_session_get_user_info_message(session, user);
	guint status = soup_session_send_message(SOUP_SESSION(session), msg);
	status = convert_soup_status_to_wqq_status(status);
	if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL)
		goto OUT;

	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	if (_wqq_session_get_user_info_ret(user, body->data) != 0)
		status = WQQ_SESSION_STATUS_FAILED;

  OUT:
	g_object_unref(msg);
	return status;
}

static void _wqq_session_get_user_info_callback(SoupSession * session,
												SoupMessage * msg,
												gpointer data)
{
	CallbackData *cdata = (CallbackData *) data;
	WqqSessionCallback callback = cdata->callback;
	gpointer user_data = cdata->data;
	WqqUser *user = (WqqUser *) cdata->data1;
	g_free(data);
	guint status;
	g_object_get(msg, "status-code", &status, NULL);
	if (!SOUP_STATUS_IS_SUCCESSFUL(status))
		goto OUT;
	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	if (_wqq_session_get_user_info_ret(user, body->data) != 0)
		soup_message_set_status(msg, WQQ_SESSION_STATUS_FAILED);
  OUT:
	if (callback != NULL)
		callback(session, msg, user_data);
}

void wqq_session_get_user_info_async(WqqSession * session,
									 WqqUser * user,
									 WqqSessionCallback callback,
									 gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session) && WQQ_IS_USER(user));
	SoupMessage *msg = _wqq_session_get_user_info_message(session, user);
	CallbackData *cdata = (CallbackData *) g_malloc(sizeof(CallbackData));
	cdata->callback = callback;
	cdata->data = data;
	cdata->data1 = user;
	soup_session_queue_message(SOUP_SESSION(session), msg,
							   _wqq_session_get_user_info_callback, cdata);
}

guint wqq_session_get_self_info(WqqSession * session)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session),
						 WQQ_SESSION_STATUS_FAILED);
	return wqq_session_get_user_info(session, session->self);
}

void wqq_session_get_self_info_async(WqqSession * session,
									 WqqSessionCallback callback,
									 gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session));
	wqq_session_get_user_info_async(session, session->self,
									callback, data);
}

/*
 * 获取在线好友
 */
static SoupMessage *_wqq_session_get_online_friends_list_message(WqqSession
																 * session)
{
	GDateTime *dt = g_date_time_new_now_local();
	gchar *target = g_strdup_printf(WQQ_WEBQQ_GET_ONLINE_FRINEDS_LIST_URL,
									session->clientid,
									session->psessionid,
									g_date_time_to_unix(dt));
	g_date_time_unref(dt);
	SoupMessage *msg = soup_message_new("GET", target);
	g_free(target);

	SoupMessageHeaders *headers;
	g_object_get(msg, "request-headers", &headers, NULL);
	soup_message_headers_append(headers,
								"Referer",
								"https://d.web2.qq.com/cfproxy.html?v=20110331002&callback=1");
	return msg;
}

static guint
_wqq_session_get_online_friends_list_ret(WqqSession * session,
										 const gchar * data)
{
	JsonParser *parser = json_parser_new();
	json_parser_load_from_data(parser, data, -1, NULL);
	JsonNode *root = json_parser_get_root(parser);
	JsonObject *root_obj = json_node_get_object(root);
	guint retcode = json_object_get_int_member(root_obj, "retcode");
	if (retcode != 0)
		goto OUT;
	JsonArray *result = json_object_get_array_member(root_obj, "result");
	guint i, len;
	len = json_array_get_length(result);
	for (i = 0; i < len; i++) {
		JsonObject *online = json_array_get_object_element(result, i);
		guint64 uin = json_object_get_int_member(online, "uin");
		const gchar *status =
			json_object_get_string_member(online, "status");
		guint client_type =
			json_object_get_int_member(online, "client_type");
		WqqFriend *fri = wqq_session_find_friend_by_uin(session, uin);
		if (fri != NULL) {
			wqq_user_set_status_from_string(WQQ_USER(fri), status);
			_wqq_user_set_client_type_from_uint(WQQ_USER(fri),
												client_type);
		}
	}

  OUT:
	g_object_unref(parser);
	return retcode;
}

guint wqq_session_get_online_friends_list(WqqSession * session)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session),
						 WQQ_SESSION_STATUS_FAILED);
	SoupMessage *msg =
		_wqq_session_get_online_friends_list_message(session);
	guint status;
	status = soup_session_send_message(SOUP_SESSION(session), msg);
	status = convert_soup_status_to_wqq_status(status);
	if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL)
		goto OUT;

	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	if (_wqq_session_get_online_friends_list_ret(session, body->data) != 0)
		status = WQQ_SESSION_STATUS_FAILED;
  OUT:
	g_object_unref(msg);
	return status;
}

static void _wqq_session_get_online_friends_list_callback(SoupSession *
														  session,
														  SoupMessage *
														  msg,
														  gpointer data)
{
	CallbackData *cdata = (CallbackData *) data;
	WqqSessionCallback callback = cdata->callback;
	gpointer user_data = cdata->data;
	g_free(data);
	guint status;
	g_object_get(msg, "status-code", &status, NULL);
	if (!SOUP_STATUS_IS_SUCCESSFUL(status))
		goto OUT;

	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	if (_wqq_session_get_online_friends_list_ret(WQQ_SESSION(session),
												 body->data) != 0)
		soup_message_set_status(msg, WQQ_SESSION_STATUS_FAILED);
  OUT:
	if (callback != NULL)
		callback(session, msg, user_data);
}

void wqq_session_get_online_friends_list_async(WqqSession * session,
											   WqqSessionCallback callback,
											   gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session));

	SoupMessage *msg =
		_wqq_session_get_online_friends_list_message(session);
	CallbackData *cdata = (CallbackData *) g_malloc(sizeof(CallbackData));
	cdata->callback = callback;
	cdata->data = data;
	soup_session_queue_message(SOUP_SESSION(session), msg,
							   _wqq_session_get_online_friends_list_callback,
							   cdata);
}

/*
 * 获取群列表
 */
static SoupMessage *_wqq_session_get_groups_list_message(WqqSession *
														 session)
{
	SoupMessage *msg =
		soup_message_new("POST", WQQ_WEBQQ_GET_GROUPS_LIST_URL);
	gchar *post = g_strdup_printf("r={\"vfwebqq\":\"%s\"}",
								  session->vfwebqq);
	soup_message_set_request(msg,
							 "application/x-www-form-urlencoded",
							 SOUP_MEMORY_TAKE, post, strlen(post));
	SoupMessageHeaders *headers;
	g_object_get(msg, "request-headers", &headers, NULL);
	soup_message_headers_append(headers, "Referer",
								"http://s.web2.qq.com/proxy.html?v=20110412001&callback=1&id=1");

	return msg;
}

static guint _wqq_session_get_groups_list_ret(WqqSession * session,
											  const gchar * data)
{
	JsonParser *parser = json_parser_new();
	json_parser_load_from_data(parser, data, -1, NULL);
	JsonNode *root = json_parser_get_root(parser);
	JsonObject *root_obj = json_node_get_object(root);
	guint retcode = json_object_get_int_member(root_obj, "retcode");
	if (retcode == 100003)		/* 当retcode为100003时，没有gmasklist */
		retcode = 0;
	if (retcode != 0)
		goto OUT;
	JsonObject *result = json_object_get_object_member(root_obj, "result");
	JsonArray *gnamelist =
		json_object_get_array_member(result, "gnamelist");
	JsonArray *gmarklist =
		json_object_get_array_member(result, "gmarklist");
	guint gnamelist_len = json_array_get_length(gnamelist);
	guint gmarklist_len = json_array_get_length(gmarklist);
	guint i;
	for (i = 0; i < gnamelist_len; i++) {
		JsonObject *gname = json_array_get_object_element(gnamelist, i);
		guint64 flag = json_object_get_int_member(gname, "flag");
		const gchar *name = json_object_get_string_member(gname, "name");
		guint64 gid = json_object_get_int_member(gname, "gid");
		guint64 code = json_object_get_int_member(gname, "code");
		WqqGroup *group = wqq_session_append_group(session, gid, name);
		wqq_group_set_code(group, code);
		wqq_group_set_flag(group, flag);
	}
	if (json_object_get_member(result, "gmasklist")) {
		JsonArray *gmasklist =
			json_object_get_array_member(result, "gmasklist");
		guint gmasklist_len = json_array_get_length(gmasklist);
		for (i = 0; i < gmasklist_len; i++) {
			JsonObject *gmask =
				json_array_get_object_element(gmasklist, i);
			guint64 gid = json_object_get_int_member(gmask, "gid");
			WqqGroup *group = wqq_session_find_group_by_gid(session, gid);
			if (group != NULL) {
				guint mask = json_object_get_int_member(gmask, "mask");
				wqq_group_set_mask(group, mask);
			}
		}
	}
	for (i = 0; i < gmarklist_len; i++) {
		JsonObject *gmark = json_array_get_object_element(gmarklist, i);
		guint64 uin = json_object_get_int_member(gmark, "uin");
		WqqGroup *group = wqq_session_find_group_by_gid(session, uin);
		if (group != NULL) {
			const gchar *markname =
				json_object_get_string_member(gmark, "markname");
			wqq_group_set_mark(group, markname);
		}
	}

  OUT:
	g_object_unref(parser);
	return retcode;
}

guint wqq_session_get_groups_list(WqqSession * session)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), WQQ_SESSION_MALFORMED);

	SoupMessage *msg = _wqq_session_get_groups_list_message(session);
	guint status = soup_session_send_message(SOUP_SESSION(session), msg);
	status = convert_soup_status_to_wqq_status(status);
	if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL)
		goto OUT;

	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	if (_wqq_session_get_groups_list_ret(session, body->data) != 0)
		status = WQQ_SESSION_STATUS_FAILED;
  OUT:
	g_object_unref(msg);
	return status;
}

static void _wqq_session_get_groups_list_callback(SoupSession * session,
												  SoupMessage * msg,
												  gpointer data)
{
	CallbackData *cdata = (CallbackData *) data;
	WqqSessionCallback callback = cdata->callback;
	gpointer user_data = cdata->data;
	g_free(data);
	guint status;
	g_object_get(msg, "status-code", &status, NULL);
	if (!SOUP_STATUS_IS_SUCCESSFUL(status))
		goto OUT;
	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	if (_wqq_session_get_groups_list_ret(WQQ_SESSION(session), body->data)
		!= 0) {
		soup_message_set_status(msg, WQQ_SESSION_STATUS_FAILED);
	}
  OUT:
	if (callback != NULL)
		callback(session, msg, user_data);
}

void wqq_session_get_groups_list_async(WqqSession * session,
									   WqqSessionCallback callback,
									   gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session));
	SoupMessage *msg = _wqq_session_get_groups_list_message(session);
	CallbackData *cdata = (CallbackData *) g_malloc(sizeof(CallbackData));
	cdata->callback = callback;
	cdata->data = data;
	soup_session_queue_message(SOUP_SESSION(session), msg,
							   _wqq_session_get_groups_list_callback,
							   cdata);
}

/*
 * 获取群信息，群成员
 */
static SoupMessage *_wqq_session_get_group_info_message(WqqSession *
														session,
														WqqGroup * group)
{
	GDateTime *dt = g_date_time_new_now_local();
	gchar *target = g_strdup_printf(WQQ_WEBQQ_GET_GROUP_INFO_URL,
									wqq_group_get_code(group),
									session->vfwebqq,
									g_date_time_to_unix(dt));
	g_date_time_unref(dt);
	SoupMessage *msg = soup_message_new("GET", target);
	g_free(target);
	SoupMessageHeaders *headers;
	g_object_get(msg, "request-headers", &headers, NULL);
	soup_message_headers_append(headers,
								"Referer",
								"http://s.web2.qq.com/proxy.html?v=20110412001&callback=1&id=3");
	return msg;
}

static guint _wqq_session_get_group_info_ret(WqqGroup * group,
											 const gchar * data)
{
	JsonParser *parser = json_parser_new();
	json_parser_load_from_data(parser, data, -1, NULL);
	JsonNode *root = json_parser_get_root(parser);
	JsonObject *root_obj = json_node_get_object(root);
	guint retcode = json_object_get_int_member(root_obj, "retcode");
	if (retcode != 0)
		goto OUT;
	JsonObject *result = json_object_get_object_member(root_obj, "result");
	JsonObject *ginfo = json_object_get_object_member(result, "ginfo");
	guint64 code = json_object_get_int_member(ginfo, "code");
	guint64 createtime = json_object_get_int_member(ginfo, "createtime");
	guint64 flag = json_object_get_int_member(ginfo, "flag");
	guint level = json_object_get_int_member(ginfo, "level");
	const gchar *gname = json_object_get_string_member(ginfo, "name");
	guint64 gid = json_object_get_int_member(ginfo, "gid");
	guint64 owner = json_object_get_int_member(ginfo, "owner");
	JsonArray *members = json_object_get_array_member(ginfo, "members");
	JsonArray *minfo = json_object_get_array_member(result, "minfo");
	JsonArray *vipinfo = json_object_get_array_member(result, "vipinfo");
	wqq_group_set_code(group, code);
	wqq_group_set_createtime(group, createtime);
	wqq_group_set_flag(group, flag);
	wqq_group_set_level(group, level);
	wqq_group_set_name(group, gname);
	wqq_group_set_gid(group, gid);
	wqq_group_set_owner(group, owner);
	guint members_len = json_array_get_length(members);
	guint i;
	for (i = 0; i < members_len; i++) {
		JsonObject *m = json_array_get_object_element(members, i);
		guint64 muin = json_object_get_int_member(m, "muin");
		guint64 mflag = json_object_get_int_member(m, "mflag");
		WqqGroupMember *member =
			wqq_group_append_group_member(group, muin);
		wqq_group_member_set_flag(member, mflag);
		JsonObject *info = json_array_get_object_element(minfo, i);
		const gchar *nick = json_object_get_string_member(info, "nick");
		const gchar *province =
			json_object_get_string_member(info, "province");
		const gchar *gender =
			json_object_get_string_member(info, "gender");
		guint64 uin = json_object_get_int_member(info, "uin");
		const gchar *country =
			json_object_get_string_member(info, "country");
		const gchar *city = json_object_get_string_member(info, "city");
		wqq_user_set_nick(WQQ_USER(member), nick);
		wqq_user_set_location(WQQ_USER(member), country, province, city);
		wqq_user_set_gender(WQQ_USER(member), gender);
		JsonObject *vip = json_array_get_object_element(vipinfo, i);
		guint vip_level = json_object_get_int_member(vip, "vip_level");
		guint64 u = json_object_get_int_member(vip, "u");
		guint is_vip = json_object_get_int_member(vip, "is_vip");
		wqq_user_set_vip(WQQ_USER(member), is_vip);
		wqq_user_set_vip_level(WQQ_USER(member), vip_level);
	}
	if (json_object_get_member(result, "cards")) {	/* cards可能不存在 */
		JsonArray *cards = json_object_get_array_member(result, "cards");
		guint cards_len = json_array_get_length(cards);
		for (i = 0; i < cards_len; i++) {
			JsonObject *card = json_array_get_object_element(cards, i);
			guint64 muin = json_object_get_int_member(card, "muin");
			WqqGroupMember *member =
				wqq_group_find_group_member_by_uin(group, muin);
			if (member != NULL) {
				const gchar *c =
					json_object_get_string_member(card, "card");
				wqq_group_member_set_card(member, c);
			}
		}
	}

	JsonArray *stats = json_object_get_array_member(result, "stats");
	guint stats_len = json_array_get_length(stats);
	for (i = 0; i < stats_len; i++) {
		JsonObject *stat = json_array_get_object_element(stats, i);
		guint client_type =
			json_object_get_int_member(stat, "client_type");
		guint64 uin = json_object_get_int_member(stat, "uin");
		guint status = json_object_get_int_member(stat, "stat");
		WqqGroupMember *member =
			wqq_group_find_group_member_by_uin(group, uin);
		if (member != NULL) {
			_wqq_user_set_client_type_from_uint(WQQ_USER(member),
												client_type);
			wqq_user_set_status(WQQ_USER(member), status);
		}
	}
  OUT:
	g_object_unref(parser);
	return retcode;
}

guint wqq_session_get_group_info(WqqSession * session, WqqGroup * group)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session) && WQQ_IS_GROUP(group),
						 WQQ_SESSION_MALFORMED);
	SoupMessage *msg = _wqq_session_get_group_info_message(session, group);
	guint status;
	status = soup_session_send_message(SOUP_SESSION(session), msg);
	status = convert_soup_status_to_wqq_status(status);
	if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL)
		goto OUT;
	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	if (_wqq_session_get_group_info_ret(group, body->data) != 0)
		status = WQQ_SESSION_STATUS_FAILED;
  OUT:
	g_object_unref(msg);
	return status;
}

guint wqq_session_get_group_info_by_gid(WqqSession * session, guint64 gid)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), WQQ_SESSION_MALFORMED);
	WqqGroup *group = wqq_session_find_group_by_gid(session, gid);
	if (group == NULL)
		return WQQ_SESSION_STATUS_FAILED;
	return wqq_session_get_group_info(session, group);
}

static void _wqq_session_get_group_info_callback(SoupSession * session,
												 SoupMessage * msg,
												 gpointer data)
{
	CallbackData *cdata = (CallbackData *) data;
	WqqSessionCallback callback = cdata->callback;
	gpointer user_data = cdata->data;
	WqqGroup *group = (WqqGroup *) cdata->data1;
	g_free(data);
	guint status;
	g_object_get(msg, "status-code", &status, NULL);
	if (!SOUP_STATUS_IS_SUCCESSFUL(status))
		goto OUT;
	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	if (_wqq_session_get_group_info_ret(group, body->data) != 0)
		soup_message_set_status(msg, WQQ_SESSION_STATUS_FAILED);
  OUT:
	if (callback != NULL)
		callback(session, msg, user_data);
}

void wqq_session_get_group_info_async(WqqSession * session,
									  WqqGroup * group,
									  WqqSessionCallback callback,
									  gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session) && WQQ_IS_GROUP(group));
	CallbackData *cdata = (CallbackData *) g_malloc(sizeof(CallbackData));
	cdata->callback = callback;
	cdata->data = data;
	cdata->data1 = group;
	SoupMessage *msg = _wqq_session_get_group_info_message(session, group);
	soup_session_queue_message(SOUP_SESSION(session), msg,
							   _wqq_session_get_group_info_callback,
							   cdata);
}

void wqq_session_get_group_info_by_gid_async(WqqSession * session,
											 guint64 gid,
											 WqqSessionCallback callback,
											 gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session));
	WqqGroup *group = wqq_session_find_group_by_gid(session, gid);
	if (group != NULL)
		wqq_session_get_group_info_async(session, group, callback, data);
}


/*
 * 发送好友消息
 */
static gchar *_wqq_string_char_replace(gchar * src, const gchar c,
									   const gchar * s)
{
	GString *string = g_string_new("");
	gchar *ptr = src;
	while (*ptr != '\0') {
		gchar ch = *ptr;
		if (ch != c)
			string = g_string_append_c(string, ch);
		else
			string = g_string_append(string, s);
		ptr++;
	}
	g_free(src);
	return g_string_free(string, FALSE);
}

static gchar *_wqq_session_message_content(WqqSendMessage * msg)
{
	GList *lp = wqq_send_message_get_contents(msg);
	GString *contents = g_string_new("[");
	while (lp != NULL) {
		WqqMessageContent *c = (WqqMessageContent *) lp->data;
		WqqMessageContentType ctype = wqq_message_content_get_ctype(c);
		if (ctype == WQQ_MESSAGE_CONTENT_TYPE_TEXT) {
			g_string_append_printf(contents, "\\\"%s\\\",",
								   wqq_message_content_get_text(c));
		} else if (ctype == WQQ_MESSAGE_CONTENT_TYPE_FACE) {
			g_string_append_printf(contents, "[\\\"face\\\",%u],",
								   wqq_message_content_get_face(c));
		} else if (ctype == WQQ_MESSAGE_CONTENT_TYPE_PIC) {
			/*TODO */
		}
		lp = g_list_next(lp);
	}
	g_string_append_printf(contents,
						   "\\\"\\\",[\\\"font\\\",{\\\"name\\\""
						   ":\\\"幼圆\\\","
						   "\\\"size\\\":\\\"11\\\",\\\"style\\\":[0,0,0],"
						   "\\\"color\\\":\\\"000000\\\"}]]");
	return _wqq_string_char_replace(g_string_free(contents, FALSE), '\n',
									"\\n");
}

static SoupMessage *_wqq_session_send_friend_message(WqqSession * session,
													 WqqSendMessage * smsg)
{
	gchar *contents = _wqq_session_message_content(smsg);
	SoupMessage *msg =
		soup_message_new("POST", WQQ_WEBQQ_SEND_FRIEND_MSG_URL);
	SoupMessageHeaders *headers;
	g_object_get(msg, "request-headers", &headers, NULL);
	soup_message_headers_append(headers, "Referer",
								"https://d.web2.qq.com/cfproxy.html?v=20110331002&callback=1");
	gchar *post = g_strdup_printf("r={\"to\":%lu,\"face\":%u,"
								  "\"content\":\"%s\",\"msg_id\":%lu,"
								  "\"clientid\":\"%s\","
								  "\"psessionid\":\"%s\"}&clientid=%s"
								  "&psessionid=%s",
								  wqq_send_message_get_to(smsg),
								  wqq_send_message_get_face(smsg),
								  contents,
								  wqq_send_message_get_msg_id(smsg),
								  session->clientid,
								  session->psessionid,
								  session->clientid,
								  session->psessionid);
	g_free(contents);
	/*g_message("send:%s\n", post); */
	soup_message_set_request(msg,
							 "application/x-www-form-urlencoded",
							 SOUP_MEMORY_TAKE, post, strlen(post));
	return msg;
}

static SoupMessage *_wqq_session_send_group_message(WqqSession * session,
													WqqSendMessage * smsg)
{
	gchar *contents = _wqq_session_message_content(smsg);
	SoupMessage *msg =
		soup_message_new("POST", WQQ_WEBQQ_SEND_GROUP_MSG_URL);
	SoupMessageHeaders *headers;
	g_object_get(msg, "request-headers", &headers, NULL);
	soup_message_headers_append(headers, "Referer",
								"https://d.web2.qq.com/cfproxy.html?v=20110331002&callback=1");
	gchar *post =
		g_strdup_printf("r={\"group_uin\":%lu,\"content\":\"%s\","
						"\"msg_id\":%lu,\"clientid\":\"%s\","
						"\"psessionid\":\"%s\"}&clientid=%s&"
						"psessionid=%s",
						wqq_send_message_get_to(smsg),
						contents, wqq_send_message_get_msg_id(smsg),
						session->clientid,
						session->psessionid,
						session->clientid,
						session->psessionid);
	g_free(contents);
	soup_message_set_request(msg,
							 "application/x-www-form-urlencoded",
							 SOUP_MEMORY_TAKE, post, strlen(post));
	return msg;
}

static SoupMessage *_wqq_session_send_message(WqqSession * session,
											  WqqSendMessage * msg)
{
	WqqSendMessageType smtype = wqq_send_message_get_smtype(msg);
	if (smtype == WQQ_SEND_MESSAGE_TYPE_FRIEND) {
		return _wqq_session_send_friend_message(session, msg);
	} else if (smtype == WQQ_SEND_MESSAGE_TYPE_GROUP) {
		return _wqq_session_send_group_message(session, msg);
	}
	return NULL;
}

static guint _wqq_session_send_message_ret(const gchar * data)
{
	JsonParser *parser = json_parser_new();
	json_parser_load_from_data(parser, data, -1, NULL);
	JsonNode *root = json_parser_get_root(parser);
	JsonObject *root_object = json_node_get_object(root);
	guint retcode = json_object_get_int_member(root_object,
											   "retcode");
	return retcode;
}

guint wqq_session_send_message(WqqSession * session, WqqSendMessage * smsg)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session)
						 && WQQ_IS_SEND_MESSAGE(smsg),
						 WQQ_SESSION_MALFORMED);
	SoupMessage *msg = _wqq_session_send_message(session, smsg);
	guint status = WQQ_SESSION_MALFORMED;
	if (msg == NULL)
		goto OUT;
	status = soup_session_send_message(SOUP_SESSION(session), msg);
	status = convert_soup_status_to_wqq_status(status);
	if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL)
		goto OUT;
	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	guint retcode = _wqq_session_send_message_ret(body->data);
	if (retcode != 0)
		status = WQQ_SESSION_STATUS_FAILED;
  OUT:
	g_object_unref(msg);
	return status;
}

static void _wqq_session_send_message_callback(SoupSession * session,
											   SoupMessage * msg,
											   gpointer data)
{
	CallbackData *cdata = (CallbackData *) data;
	gpointer user_data = cdata->data;
	WqqSessionCallback callback = cdata->callback;
	g_free(data);
	guint status;
	g_object_get(msg, "status-code", &status, NULL);
	if (!SOUP_STATUS_IS_SUCCESSFUL(status))
		goto OUT;
	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	guint retcode = _wqq_session_send_message_ret(body->data);
	if (retcode != 0)
		soup_message_set_status(msg, retcode);
  OUT:
	callback(session, msg, user_data);
}

void wqq_session_send_message_async(WqqSession * session,
									WqqSendMessage * smsg,
									WqqSessionCallback callback,
									gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session) && WQQ_IS_SEND_MESSAGE(smsg));
	CallbackData *cdata = (CallbackData *) g_malloc(sizeof(CallbackData));
	cdata->callback = callback;
	cdata->data = data;
	SoupMessage *msg = _wqq_session_send_message(session, smsg);
	if (msg == NULL)
		return;
	soup_session_queue_message(SOUP_SESSION(session), msg,
							   _wqq_session_send_message_callback, cdata);
}

/****************************************************************
 * POLL消息
 ********************************************************/
static SoupMessage *_wqq_session_poll_message_message(WqqSession * session)
{
	SoupMessage *msg = soup_message_new("POST", WQQ_WEBQQ_POLL_MSG_URL);
	SoupMessageHeaders *headers;
	g_object_get(msg, "request-headers", &headers, NULL);
	soup_message_headers_append(headers, "Referer",
								"https://d.web2.qq.com/cfproxy.html?"
								"v=20110331002&callback=1");

	gchar *data = g_strdup_printf("r={\"clientid\":\"%s\","
								  "\"psessionid\":\"%s\",\"key\":0,"
								  "\"ids\":[]}&clientid=%s&psessionid=%s",
								  session->clientid,
								  session->psessionid,
								  session->clientid,
								  session->psessionid);
	soup_message_set_request(msg,
							 "application/x-www-form-urlencoded",
							 SOUP_MEMORY_TAKE, data, strlen(data));
	return msg;
}

static guint _wqq_session_poll_message_ret(WqqPollMessageList * ml,
										   const gchar * data)
{
	wqq_poll_message_list_clear(ml);
	JsonParser *parser = json_parser_new();
	json_parser_load_from_data(parser, data, -1, NULL);
	JsonNode *root = json_parser_get_root(parser);
	JsonObject *root_object = json_node_get_object(root);
	guint retcode = json_object_get_int_member(root_object, "retcode");
	if (retcode != 0)
		goto OUT;
	JsonArray *result =
		json_object_get_array_member(root_object, "result");
	guint result_len = json_array_get_length(result);
	guint i;
	for (i = 0; i < result_len; i++) {
		JsonObject *message = json_array_get_object_element(result, i);
		const gchar *poll_type = json_object_get_string_member(message,
															   "poll_type");
		if (g_strcmp0(poll_type, "message") == 0) {	/* friend message */
			JsonObject *value = json_object_get_object_member(message,
															  "value");
			guint64 msg_id = json_object_get_int_member(value, "msg_id");
			guint64 from_uin = json_object_get_int_member(value,
														  "from_uin");
			guint64 to_uin = json_object_get_int_member(value, "to_uin");
			guint64 msg_id2 = json_object_get_int_member(value, "msg_id2");
			guint msg_type = json_object_get_int_member(value, "msg_type");
			guint64 reply_ip = json_object_get_int_member(value,
														  "reply_ip");
			guint64 time = json_object_get_int_member(value, "time");
			WqqPollMessage *pmsg =
				wqq_poll_message_new(WQQ_POLL_MESSAGE_TYPE_FRIEND);
			wqq_poll_message_set_friend(pmsg, msg_id, from_uin, to_uin,
										msg_id2, msg_type, reply_ip, time);
			wqq_poll_message_list_append(ml, pmsg);
			JsonArray *contents = json_object_get_array_member(value,
															   "content");
			guint contents_len = json_array_get_length(contents);
			guint j;
			for (j = 0; j < contents_len; j++) {
				JsonNode *content = json_array_get_element(contents, j);
				JsonNodeType ntype = json_node_get_node_type(content);
				if (ntype == JSON_NODE_ARRAY) {
					JsonArray *font_or_face = json_node_get_array(content);
					const gchar *first =
						json_array_get_string_element(font_or_face, 0);
					if (g_strcmp0(first, "font") == 0) {
					} else if (g_strcmp0(first, "face") == 0) {
						guint face =
							json_array_get_int_element(font_or_face, 1);
						wqq_poll_message_append_face_content(pmsg, face);
					}
				} else if (ntype == JSON_NODE_VALUE) {
					const gchar *text = json_node_get_string(content);
					wqq_poll_message_append_text_content(pmsg, text);
				}
			}
		} else if (g_strcmp0(poll_type, "group_message") == 0) {
			/* group message */
			JsonObject *value = json_object_get_object_member(message,
															  "value");
			guint64 msg_id = json_object_get_int_member(value, "msg_id");
			guint64 from_uin = json_object_get_int_member(value,
														  "from_uin");
			guint64 to_uin = json_object_get_int_member(value, "to_uin");
			guint64 msg_id2 = json_object_get_int_member(value, "msg_id2");
			guint msg_type = json_object_get_int_member(value, "msg_type");
			guint64 reply_ip = json_object_get_int_member(value,
														  "reply_ip");
			guint64 time = json_object_get_int_member(value, "time");
			guint64 group_code =
				json_object_get_int_member(value, "group_code");
			guint64 send_uin =
				json_object_get_int_member(value, "send_uin");
			guint64 seq = json_object_get_int_member(value, "seq");
			guint64 info_seq =
				json_object_get_int_member(value, "info_seq");
			WqqPollMessage *pmsg =
				wqq_poll_message_new(WQQ_POLL_MESSAGE_TYPE_GROUP);
			wqq_poll_message_set_group(pmsg, msg_id, from_uin, to_uin,
									   msg_id2, msg_type, reply_ip, time,
									   group_code, send_uin, seq,
									   info_seq);
			wqq_poll_message_list_append(ml, pmsg);
			JsonArray *contents = json_object_get_array_member(value,
															   "content");
			guint contents_len = json_array_get_length(contents);
			guint j;
			for (j = 0; j < contents_len; j++) {
				JsonNode *content = json_array_get_element(contents, j);
				JsonNodeType ntype = json_node_get_node_type(content);
				if (ntype == JSON_NODE_ARRAY) {
					JsonArray *font_or_face = json_node_get_array(content);
					const gchar *first =
						json_array_get_string_element(font_or_face, 0);
					if (g_strcmp0(first, "font") == 0) {
					} else if (g_strcmp0(first, "face") == 0) {
						guint face =
							json_array_get_int_element(font_or_face, 1);
						wqq_poll_message_append_face_content(pmsg, face);
					} else if (g_strcmp0(first, "offpic") == 0) {	/* TODO */
					}
				} else if (ntype == JSON_NODE_VALUE) {
					const gchar *text = json_node_get_string(content);
					wqq_poll_message_append_text_content(pmsg, text);
				}
			}
		} else if (g_strcmp0(poll_type, "buddies_status_change") == 0) {
			/* friend status change */
			JsonObject *value =
				json_object_get_object_member(message, "value");
			guint64 uin = json_object_get_int_member(value, "uin");
			const gchar *status =
				json_object_get_string_member(value, "status");
			guint ctype = json_object_get_int_member(value, "client_type");
			WqqPollMessage *pmsg =
				wqq_poll_message_new(WQQ_POLL_MESSAGE_TYPE_STATUS);
			wqq_poll_message_list_append(ml, pmsg);
			wqq_poll_message_set_status(pmsg, uin,
										_wqq_user_status_to_enum(status),
										_wqq_user_client_type_to_enum
										(ctype));
		} else {				/*  unknown type */
		}
		wqq_poll_message_list_sort_by_time(ml);
	}
  OUT:
	g_object_unref(parser);
	return retcode;
}

guint wqq_session_poll_message(WqqSession * session,
							   WqqPollMessageList * list)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session) &&
						 WQQ_IS_POLL_MESSAGE_LIST(list),
						 WQQ_SESSION_MALFORMED);
	SoupMessage *msg = _wqq_session_poll_message_message(session);
	guint status = soup_session_send_message(SOUP_SESSION(session), msg);
	status = convert_soup_status_to_wqq_status(status);
	if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL)
		goto OUT;
	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	guint ret = _wqq_session_poll_message_ret(list, body->data);
	if (ret != 0){
		g_message("%s:%s(%s)",
					"vfwebqq",
					session->vfwebqq,
					_wqq_session_get_cookie(WQQ_SESSION(session),
						"vfwebqq"));
		g_message("%s:%s(%s)",
					"psessionid",
					session->psessionid,
					_wqq_session_get_cookie(WQQ_SESSION(session),
						"psessionid"));
		g_message("poll message response data:\n%s",
					body->data);
		status = ret;
	}
OUT:
	g_object_unref(msg);
	return status;
}

static void _wqq_session_poll_message_callback(SoupSession * session,
			SoupMessage * msg,
			gpointer data)
{
	CallbackData *cdata = (CallbackData *) data;
	gpointer user_data = cdata->data;
	WqqPollMessageList *list = (WqqPollMessageList *) cdata->data1;
	WqqSessionCallback callback = cdata->callback;
	g_free(data);
	guint status;
	g_object_get(msg, "status-code", &status, NULL);
	if (!SOUP_STATUS_IS_SUCCESSFUL(status))
	   goto OUT;
	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	if (_wqq_session_poll_message_ret(list, body->data) != 0)
	   soup_message_set_status(msg, WQQ_SESSION_STATUS_FAILED);
OUT:
	if (callback)
		callback(session, msg, user_data);
}

void wqq_session_poll_message_async(WqqSession * session,
									WqqPollMessageList * list,
									WqqSessionCallback callback,
									gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session) &&
					 WQQ_IS_POLL_MESSAGE_LIST(list));
	CallbackData *cdata = (CallbackData *) g_malloc(sizeof(CallbackData));
	cdata->callback = callback;
	cdata->data = data;
	cdata->data1 = list;
	SoupMessage *msg = _wqq_session_poll_message_message(session);
	soup_session_queue_message(SOUP_SESSION(session), msg,
							   _wqq_session_poll_message_callback, cdata);
}

SoupMessage *_wqq_session_get_user_account_message(WqqSession * session,
												   WqqUser * user)
{
	GDateTime *dt = g_date_time_new_now_local();
	gchar *target = g_strdup_printf(WQQ_WEBQQ_GET_USER_ACCOUNT,
									wqq_user_get_uin(user),
									session->vfwebqq,
									g_date_time_to_unix(dt));
	g_date_time_unref(dt);
	SoupMessage *msg = soup_message_new("GET", target);
	SoupMessageHeaders *headers;
	g_object_get(msg, "request-headers", &headers, NULL);
	soup_message_headers_append(headers, "Referer",
								"http://s.web2.qq.com/proxy.html?v=20110412001&callback=1&id=3");

	return msg;
}

guint _wqq_session_get_user_account_ret(const gchar * data, WqqUser * user)
{
	JsonParser *parser = json_parser_new();
	json_parser_load_from_data(parser, data, -1, NULL);
	JsonNode *root = json_parser_get_root(parser);
	JsonObject *root_obj = json_node_get_object(root);
	guint retcode = json_object_get_int_member(root_obj, "retcode");
	if (retcode != 0)
		goto OUT;
	JsonObject *result = json_object_get_object_member(root_obj, "result");
	guint64 account = json_object_get_int_member(result, "account");
	guint64 uin = json_object_get_int_member(result, "uin");
	if (uin == wqq_user_get_uin(user))
		wqq_user_set_qq(user, account);
  OUT:
	g_object_unref(parser);
	return retcode;
}

guint wqq_session_get_user_account(WqqSession * session, WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session) && WQQ_IS_USER(user),
						 WQQ_SESSION_MALFORMED);
	SoupMessage *msg =
		_wqq_session_get_user_account_message(session, user);
	guint status;
	status = soup_session_send_message(SOUP_SESSION(session), msg);
	status = convert_soup_status_to_wqq_status(status);
	if (status != WQQ_SESSION_HTTP_STATUS_SUCCESSFUL)
		goto OUT;
	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	guint ret = _wqq_session_get_user_account_ret(body->data, user);
	if (ret != 0)
		status = WQQ_SESSION_STATUS_FAILED;
  OUT:
	g_object_unref(msg);
	return status;
}

static void _wqq_session_get_user_account_callback(SoupSession * session,
												   SoupMessage * msg,
												   gpointer data)
{
	CallbackData *cdata = (CallbackData *) data;
	WqqSessionCallback callback = cdata->callback;
	gpointer user_data = cdata->data;
	WqqUser *user = cdata->data1;
	g_free(data);
	guint status;
	g_object_get(msg, "status-code", &status, NULL);
	if (!SOUP_STATUS_IS_SUCCESSFUL(status))
		goto OUT;
	SoupMessageBody *body;
	g_object_get(msg, "response-body", &body, NULL);
	guint ret = _wqq_session_get_user_account_ret(body->data, user);
	if (ret != 0)
		soup_message_set_status(msg, WQQ_SESSION_STATUS_FAILED);
  OUT:
	if (callback)
		callback(session, msg, user_data);
}

void wqq_session_get_user_account_async(WqqSession * session,
										WqqUser * user,
										WqqSessionCallback callback,
										gpointer data)
{
	g_return_if_fail(WQQ_IS_SESSION(session) && WQQ_IS_USER(user));
	SoupMessage *msg =
		_wqq_session_get_user_account_message(session, user);
	CallbackData *cdata = (CallbackData *) g_malloc(sizeof(CallbackData));
	cdata->callback = callback;
	cdata->data = data;
	cdata->data1 = user;
	soup_session_queue_message(SOUP_SESSION(session), msg,
							   _wqq_session_get_user_account_callback,
							   cdata);
}

/* ******************************************************
 * 与好友分组有关 
 * *******************************************************/
GList *wqq_session_get_category_list(WqqSession * session)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), NULL);
	return session->list;
}
void wqq_session_set_category_list(WqqSession *session,GList *list)
{
	g_return_if_fail(WQQ_IS_SESSION(session));
	g_list_free_full(session->list,g_object_unref);
	session->list=list;
}

WqqFriend *wqq_session_find_friend_by_uin(WqqSession * session,
										  guint64 uin)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session)
						 && session->list != NULL, NULL);
	GList *lp = session->list;
	while (lp != NULL) {
		WqqCategory *category = (WqqCategory *) lp->data;
		WqqFriend *fri = wqq_category_find_friend_by_uin(category, uin);
		if (fri != NULL)
			return fri;
		lp = g_list_next(lp);
	}
	return NULL;
}

WqqFriend *wqq_session_find_friend_by_qq(WqqSession * session, guint64 qq)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session)
						 && session->list != NULL, NULL);
	GList *lp = session->list;
	while (lp != NULL) {
		WqqCategory *category = (WqqCategory *) lp->data;
		WqqFriend *fri = wqq_category_find_friend_by_qq(category, qq);
		if (fri != NULL)
			return fri;
		lp = g_list_next(lp);
	}
	return NULL;
}

WqqFriend *wqq_session_find_friend_by_markname(WqqSession * session,
											   const gchar * markname)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session)
						 && session->list != NULL, NULL);
	GList *lp = session->list;
	while (lp != NULL) {
		WqqCategory *category = (WqqCategory *) lp->data;
		WqqFriend *fri =
			wqq_category_find_friend_by_markname(category, markname);
		if (fri != NULL)
			return fri;
		lp = g_list_next(lp);
	}
	return NULL;
}

WqqFriend *wqq_session_find_friend_by_nick(WqqSession * session,
										   const gchar * nick)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session)
						 && session->list != NULL, NULL);
	GList *lp = session->list;
	while (lp != NULL) {
		WqqCategory *category = (WqqCategory *) lp->data;
		WqqFriend *fri = wqq_category_find_friend_by_nick(category, nick);
		if (fri != NULL)
			return fri;
		lp = g_list_next(lp);
	}
	return NULL;
}

WqqFriend *wqq_session_find_friend_by_custom(WqqSession * session,
											 gconstpointer data,
											 GCompareFunc func)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session)
						 && session->list != NULL, NULL);
	GList *lp = session->list;
	while (lp != NULL) {
		WqqCategory *category = (WqqCategory *) lp->data;
		WqqFriend *fri =
			wqq_category_find_friend_by_custom(category, data, func);
		if (fri != NULL)
			return fri;
		lp = g_list_next(lp);
	}
	return NULL;
}

WqqCategory *wqq_session_find_category_by_index(WqqSession * session,
												guint index)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session)
						 && session->list != NULL, NULL);
	GList *lp = session->list;
	while (lp != NULL) {
		WqqCategory *category = (WqqCategory *) lp->data;
		if (category->index == index)
			return category;
		lp = g_list_next(lp);
	}
	return NULL;
}

WqqCategory *wqq_session_append_category(WqqSession * session,
										 const gchar * name,
										 guint index, guint sort)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), NULL);
	WqqCategory *category = wqq_category_new(name, index, sort);
	session->list = g_list_append(session->list, category);
	return category;
}

WqqFriend *wqq_session_append_friend_by_uin(WqqSession * session,
											guint index, guint64 uin)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), NULL);
	WqqCategory *category =
		wqq_session_find_category_by_index(session, index);
	g_return_val_if_fail(category != NULL, NULL);
	WqqFriend *fri = wqq_category_append_friend(category);
	wqq_user_set_uin(WQQ_USER(fri), uin);
	return fri;
}

static gint _wqq_session_sort_category_func(gconstpointer a,
											gconstpointer b)
{
	const WqqCategory *ca = (WqqCategory *) a;
	const WqqCategory *cb = (WqqCategory *) b;
	return (gint) ca->sort - (gint) cb->sort;
}

/* 排序 */
void wqq_session_sort_category(WqqSession * session)
{
	g_return_if_fail(WQQ_IS_SESSION(session));
	session->list = g_list_sort(session->list,
								_wqq_session_sort_category_func);
}

void wqq_session_sort_friends(WqqSession * session)
{
	g_return_if_fail(WQQ_IS_SESSION(session));
	GList *lp = session->list;
	while (lp != NULL) {
		WqqCategory *category = (WqqCategory *) lp->data;
		wqq_category_sort_friends(category);
		lp = g_list_next(lp);
	}
}

void wqq_session_sort_category_and_friends(WqqSession * session)
{
	g_return_if_fail(WQQ_IS_SESSION(session));
	wqq_session_sort_friends(session);
	wqq_session_sort_category(session);
}

/* 与群相关 */
GList *wqq_session_get_glist(WqqSession * session)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), NULL);
	return session->glist;
}
void wqq_session_set_glist(WqqSession *session,GList *list)
{
	g_return_if_fail(WQQ_IS_SESSION(session));
	g_list_free_full(session->glist,g_object_unref);
	session->glist=list;
}

static gint _wqq_session_find_group_by_gin_func(gconstpointer a,
												gconstpointer b)
{
	WqqGroup *group = (WqqGroup *) a;
	guint64 gin = *((guint64 *) b);
	if (wqq_group_get_gin(group) == gin)
		return 0;
	return -1;
}

WqqGroup *wqq_session_find_group_by_gin(WqqSession * session, guint64 gin)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), NULL);
	GList *lp = g_list_find_custom(session->glist,
								   &gin,
								   _wqq_session_find_group_by_gin_func);
	if (lp == NULL)
		return NULL;
	return (WqqGroup *) lp->data;
}

static gint _wqq_session_find_group_by_gid_func(gconstpointer a,
												gconstpointer b)
{
	WqqGroup *group = (WqqGroup *) a;
	guint64 gid = *((guint64 *) b);
	if (wqq_group_get_gid(group) == gid)
		return 0;
	return -1;
}

WqqGroup *wqq_session_find_group_by_gid(WqqSession * session, guint64 gid)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), NULL);
	GList *lp = g_list_find_custom(session->glist,
								   &gid,
								   _wqq_session_find_group_by_gid_func);
	if (lp == NULL)
		return NULL;
	return (WqqGroup *) lp->data;
}

WqqGroupMember *wqq_session_find_group_member_by_uin(WqqSession * session,
													 guint64 uin)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), NULL);
	GList *lp = session->glist;
	while (lp != NULL) {
		WqqGroup *group = (WqqGroup *) lp->data;
		WqqGroupMember *member = wqq_group_find_group_member_by_uin(group,
																	uin);
		if (member != NULL)
			return member;
		lp = g_list_next(lp);
	}
	return NULL;
}

WqqGroup *wqq_session_append_group(WqqSession * session,
								   guint64 gid, const gchar * name)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), NULL);
	WqqGroup *group = wqq_group_new(gid, name);
	session->glist = g_list_append(session->glist, group);
	return group;
}

WqqGroupMember *wqq_session_append_group_member_by_uin(WqqSession *
													   session,
													   guint64 gid,
													   guint64 uin)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), NULL);
	WqqGroup *group = wqq_session_find_group_by_gid(session, gid);
	if (group != NULL) {
		WqqGroupMember *member = wqq_group_append_group_member(group, uin);
		return member;
	}
	return NULL;
}

void wqq_session_clear_category(WqqSession * session)
{
	g_return_if_fail(WQQ_IS_SESSION(session));
	if (session->list != NULL)
		g_list_free_full(session->list, g_object_unref);
}

WqqSession *wqq_session_duplicate(WqqSession * session)
{
	g_return_val_if_fail(WQQ_IS_SESSION(session), NULL);
	WqqSession *wqqsession = wqq_session_new("", "");
	wqqsession->vfwebqq = g_strdup(session->vfwebqq);
	wqqsession->clientid = g_strdup(session->clientid);
	wqqsession->ptwebqq = g_strdup(session->ptwebqq);
	wqqsession->psessionid = g_strdup(session->psessionid);
	wqqsession->login_sig = g_strdup(session->login_sig);
	SoupCookieJar *cjar =
		(SoupCookieJar *) soup_session_get_feature(SOUP_SESSION(session),
												   SOUP_TYPE_COOKIE_JAR);
	SoupCookieJar *ncjar = soup_cookie_jar_new();

	GSList *cookies = soup_cookie_jar_all_cookies(cjar);
	GSList *lp = cookies;
	while (lp) {
		SoupCookie *cookie = (SoupCookie *) lp->data;
		soup_cookie_jar_add_cookie(ncjar, cookie);
		lp = g_slist_next(lp);
	}
	g_slist_free(cookies);
	soup_session_add_feature(SOUP_SESSION(wqqsession),
							 SOUP_SESSION_FEATURE(ncjar));

	wqq_user_set_uin(wqqsession->self,wqq_user_get_uin(session->self));
	return wqqsession;
}
