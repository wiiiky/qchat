#ifndef __WQQ_SESSION_H__
#define __WQQ_SESSION_H__


#include "wqqcaptcha.h"
#include "wqqfriend.h"
#include "wqqcategory.h"
#include "wqqgroup.h"
#include "wqqsendmessage.h"
#include "wqqpollmessagelist.h"
#include <libsoup/soup.h>

G_BEGIN_DECLS
#define WQQ_TYPE_SESSION	(wqq_session_get_type())
#define WQQ_SESSION(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_SESSION,WqqSession))
#define WQQ_IS_SESSION(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_SESSION))
#define WQQ_SESSION_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WQQ_TYPE_SESSION,WqqSessionClass))
#define WQQ_IS_SESSION_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass),\
				WQQ_TYPE_SESSION))
#define WQQ_SESSION_GET_CLASS(obj)	(G_TYPE_GET_INSTANCE_CLASS((obj),\
				WQQ_TYPE_SESSION,WqqSessionClass))
#define APPID	"1003903"
#define WQQ_WEBQQ_CHECK_VERIFICATION_URL	"https://ssl.ptlogin2.qq.com/check?uin=%s&appid=1003903"
#define WQQ_WEBQQ_GET_CAPTCHA_IMAGE_URL		"https://ssl.captcha.qq.com/getimage?&uin=%s&aid=" APPID "&r=%.17f"
#define WQQ_WEBQQ_LOGIN_URL	"https://ssl.ptlogin2.qq.com/login?u=%s&p=%s&verifycode=%s&webqq_type=10&remember_uin=1&login2qq=1&aid=1003903&u1=http%%3A%%2F%%2Fweb2.qq.com%%2Floginproxy.html%%3Flogin2qq%%3D1%%26webqq_type%%3D10&h=1&ptredirect=0&ptlang=2052&daid=164&from_ui=1&pttype=1&dumy=&fp=loginerroralert&action=1-17-14483&mibao_css=m_webqq&t=1&g=1"
#define WQQ_WEBQQ_SET_STATUS_URL	"http://d.web2.qq.com/channel/login2"
#define WQQ_WEBQQ_SET_STATUS_POST_DATA	"{\"status\":\"%s\",\"ptwebqq\":\"%s\",\"passwd_sig\":\"\",\"clientid\":\"%s\", \"psessionid\":\"%s\"}"
#define WQQ_WEBQQ_GET_USER_SIGN_URL	"http://s.web2.qq.com/api/get_single_long_nick2?tuin=%lu&vfwebqq=%s&t=%ld"
#define WQQ_WEBQQ_GET_FRIENDS_LIST_URL	"http://s.web2.qq.com/api/get_user_friends2"
#define WQQ_WEBQQ_GET_USER_INFO_URL	"http://s.web2.qq.com/api/get_friend_info2?tuin=%lu&verifysession=&code=&vfwebqq=%s&t=%lu"
#define WQQ_WEBQQ_GET_ONLINE_FRINEDS_LIST_URL	"http://d.web2.qq.com/channel/get_online_buddies2?clientid=%s&psessionid=%s&t=%lu"
#define WQQ_WEBQQ_GET_GROUPS_LIST_URL	"http://s.web2.qq.com/api/get_group_name_list_mask2"
#define WQQ_WEBQQ_GET_GROUP_INFO_URL	"http://s.web2.qq.com/api/get_group_info_ext2?gcode=%lu&cb=undefined&vfwebqq=%s&t=%lu"
#define WQQ_WEBQQ_GET_USER_FACE_URL	"http://face%d.web.qq.com/cgi/svr/face/getface?cache=0&type=11&fid=0&uin=%lu&vfwebqq=%s"
#define WQQ_WEBQQ_SEND_FRIEND_MSG_URL	"http://d.web2.qq.com/channel/send_buddy_msg2"
#define WQQ_WEBQQ_SEND_GROUP_MSG_URL	"http://d.web2.qq.com/channel/send_qun_msg2"
#define WQQ_WEBQQ_POLL_MSG_URL	"http://d.web2.qq.com/channel/poll2"
#define WQQ_WEBQQ_GET_USER_ACCOUNT	"http://s.web2.qq.com/api/get_friend_uin2?tuin=%lu&verifysession=&type=1&code=&vfwebqq=%s&t=%lu"
typedef struct _WqqSession WqqSession;
typedef struct _WqqSessionClass WqqSessionClass;

struct _WqqSession {
	SoupSession soup;
	/* Private */
	gchar *username;			// 登录时用到的用户名，可能是qq号也可能是邮箱
	gchar *plain;				// 明文密码
	gchar *password;			// 经过加密的密码

	/* 验证码结构 */
	WqqCaptcha *captcha;

	/* self */
	WqqUser *self;
	/* 好友分组的列表，每个分组多个好友 */
	GList *list;
	/* 群列表 */
	GList *glist;

	/* Cookies */
	/*
	 * SoupSession可以自动管理Cookies，
	 * 这里保留这些字段主要是为了快速访问Cookie
	 * 而且这些Cookie在登录后就不会改变
	 */
	gchar *clientid;
	gchar *psessionid;
	gchar *vfwebqq;
	gchar *ptwebqq;
	gchar *login_sig;
};

struct _WqqSessionClass {
	SoupSessionClass parent_class;
};

GType wqq_session_get_type() G_GNUC_CONST;

WqqSession *wqq_session_new(const gchar * username, const gchar * plain);

/* http状态*/
#define WQQ_SESSION_HTTP_STATUS_SUCCESSFUL	0
#define WQQ_SESSION_HTTP_STATUS_CLIENT_ERROR	1
#define WQQ_SESSION_HTTP_STATUS_INFOMATIONAL	2
#define WQQ_SESSION_HTTP_STATUS_REDIRECTION		3
#define WQQ_SESSION_HTTP_STATUS_SERVER_ERROR	4
#define WQQ_SESSION_HTTP_STATUS_TRANSPORT_ERROR	5
/* 传递了非法指针 */
#define WQQ_SESSION_MALFORMED	12345
/* 登录状态 */
#define WQQ_SESSION_LOGIN_STATUS_VERIFY_CODE_ERRROR	6
#define WQQ_SESSION_LOGIN_STATUS_PASSWORD_ERROR		7
#define WQQ_SESSION_LOGIN_STATUS_UNKNOWN		8
/* 通用状态(失败) */
#define WQQ_SESSION_STATUS_FAILED	9

void wqq_session_set_username(WqqSession * session,
							  const gchar * username);
void wqq_session_set_plain(WqqSession * session, const gchar * plain);
void wqq_session_set_captcha_code(WqqSession * session,
								  const gchar * code);
const gchar *wqq_session_get_username(WqqSession * session);
const gchar *wqq_session_get_plain(WqqSession * session);
const gchar *wqq_session_get_captcha_code(WqqSession * session);
WqqUser *wqq_session_get_user_self(WqqSession * session);

/* 与好友分组有关 */
GList *wqq_session_get_category_list(WqqSession * session);
void wqq_session_set_category_list(WqqSession *session,GList *list);
WqqFriend *wqq_session_find_friend_by_uin(WqqSession * session,
										  guint64 uin);
WqqFriend *wqq_session_find_friend_by_qq(WqqSession * session, guint64 qq);
WqqFriend *wqq_session_find_friend_by_markname(WqqSession * session,
											   const gchar * markname);
WqqFriend *wqq_session_find_friend_by_nick(WqqSession * session,
										   const gchar * nick);
WqqFriend *wqq_session_find_friend_by_custom(WqqSession * session,
											 gconstpointer data,
											 GCompareFunc func);
WqqCategory *wqq_session_find_category_by_index(WqqSession * session,
												guint index);
WqqCategory *wqq_session_append_category(WqqSession * session,
										 const gchar * name,
										 guint index, guint sort);
WqqFriend *wqq_session_append_friend_by_uin(WqqSession * session,
											guint index, guint64 uin);
void wqq_session_sort_category(WqqSession * session);
void wqq_session_sort_friends(WqqSession * session);
void wqq_session_sort_category_and_friends(WqqSession * session);
void wqq_session_clear_category(WqqSession * session);
/* 与群相关 */
GList *wqq_session_get_glist(WqqSession * session);
void wqq_session_set_glist(WqqSession *session,GList *list);
WqqGroup *wqq_session_find_group_by_gin(WqqSession * session, guint64 gin);
WqqGroup *wqq_session_find_group_by_gid(WqqSession * session, guint64 gid);
WqqGroupMember *wqq_session_find_group_member_by_uin(WqqSession * session,
													 guint64 uin);
WqqGroup *wqq_session_append_group(WqqSession * session,
								   guint64 gid, const gchar * name);
WqqGroupMember *wqq_session_append_group_member_by_uin(WqqSession *
													   session,
													   guint64 gid,
													   guint64 uin);



/*********************************************************************
 * WEBQQ协议相关
 ********************************************************************/

typedef SoupSessionCallback WqqSessionCallback;
/*
 * 登录
 * 返回登录状态
 */
guint wqq_session_login(WqqSession * session);
void wqq_session_login_async(WqqSession * session,
							 WqqSessionCallback callback, gpointer data);

/*
 * 检查是否需要验证码
 * 返回http状态
 */
guint wqq_session_check_verify_code(WqqSession * session);
void wqq_session_check_verify_code_async(WqqSession * session,
										 WqqSessionCallback callback,
										 gpointer data);
#define WQQ_SESSION_NEED_VERIFICATION(session)	\
	(WQQ_CAPTCHA_NEED_VERIFICATION(WQQ_SESSION(session)->captcha))
/*
 * 获取验证码图片
 * 如果指定的路径无效,则保存在默认位置
 */
guint wqq_session_get_captcha_image(WqqSession * session,
									const gchar * path);
void wqq_session_get_captcha_image_async(WqqSession * session,
										 const gchar * path,
										 WqqSessionCallback callback,
										 gpointer data);

/*
 * 设置用户的在线状态
 */
guint wqq_session_set_status(WqqSession * session, WqqUserStatus status);
void wqq_session_set_status_async(WqqSession * session,
								  WqqUserStatus status,
								  WqqSessionCallback callback,
								  gpointer data);

/*
 * 获取用户的个性签名
 */
guint wqq_session_get_self_sign(WqqSession * session);
guint wqq_session_get_user_sign(WqqSession * session, WqqUser * user);
void wqq_session_get_user_sign_async(WqqSession * session, WqqUser * user,
									 WqqSessionCallback callback,
									 gpointer data);
void wqq_session_get_self_sign_async(WqqSession * session,
									 WqqSessionCallback callback,
									 gpointer data);

/*
 * 获取用户头像
 */
guint wqq_session_get_user_face(WqqSession * session, WqqUser * user,
								const gchar * path);
guint wqq_session_get_user_face_by_uin(WqqSession * session, guint64 uin,
									   const gchar * path);
guint wqq_session_get_self_face(WqqSession * session, const gchar * path);
void wqq_session_get_self_face_async(WqqSession * session,
									 const gchar * path,
									 WqqSessionCallback callback,
									 gpointer data);
void wqq_session_get_user_face_by_uin_async(WqqSession * session,
											guint64 uin,
											const gchar * path,
											WqqSessionCallback callback,
											gpointer data);
void wqq_session_get_user_face_async(WqqSession * session, WqqUser * user,
									 const gchar * path,
									 WqqSessionCallback callback,
									 gpointer data);

/*
 * 获取好友列表
 */
guint wqq_session_get_friends_list(WqqSession * session);
void wqq_session_get_friends_list_async(WqqSession * session,
										WqqSessionCallback callback,
										gpointer data);

/*
 * 获取好友信息
 */
guint wqq_session_get_user_info(WqqSession * session, WqqUser * user);
guint wqq_session_get_self_info(WqqSession * session);
void wqq_session_get_user_info_async(WqqSession * session,
									 WqqUser * user,
									 WqqSessionCallback callback,
									 gpointer data);
void wqq_session_get_self_info_async(WqqSession * session,
									 WqqSessionCallback callback,
									 gpointer data);

/*
 * 获取在线好友
 */
guint wqq_session_get_online_friends_list(WqqSession * session);
void wqq_session_get_online_friends_list_async(WqqSession * session,
											   WqqSessionCallback callback,
											   gpointer data);
/*
 * 获取群列表
 */
guint wqq_session_get_groups_list(WqqSession * session);
void wqq_session_get_groups_list_async(WqqSession * session,
									   WqqSessionCallback callback,
									   gpointer data);
/*
 * 获取群信息，群成员
 */
guint wqq_session_get_group_info(WqqSession * session, WqqGroup * group);
guint wqq_session_get_group_info_by_gid(WqqSession * session, guint64 gid);
void wqq_session_get_group_info_async(WqqSession * session,
									  WqqGroup * group,
									  WqqSessionCallback callback,
									  gpointer data);
void wqq_session_get_group_info_by_gid_async(WqqSession * session,
											 guint64 gid,
											 WqqSessionCallback callback,
											 gpointer data);

/*
 * 发送好友消息
 */
guint wqq_session_send_message(WqqSession * session, WqqSendMessage * msg);
void wqq_session_send_message_async(WqqSession * session,
									WqqSendMessage * msg,
									WqqSessionCallback callback,
									gpointer data);
/*
 * POLL消息
 */
guint wqq_session_poll_message(WqqSession * session,
							   WqqPollMessageList * list);
void wqq_session_poll_message_async(WqqSession * session,
									WqqPollMessageList * list,
									WqqSessionCallback callback,
									gpointer data);
/*
 * 获取qq帐号
 */
guint wqq_session_get_user_account(WqqSession * session, WqqUser * user);
void wqq_session_get_user_account_async(WqqSession * session,
										WqqUser * user,
										WqqSessionCallback callback,
										gpointer data);

/* 复制一个session */
WqqSession *wqq_session_duplicate(WqqSession * session);

G_END_DECLS
#endif
