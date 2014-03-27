#ifndef __WQQ_USER_H__
#define __WQQ_USER_H__

/*
 * Wiky L(wiiiky@yeah.net)
 * 2013-12-29
 *
 * QQ用户的通用信息
 */

#include<glib-object.h>

G_BEGIN_DECLS
#define WQQ_TYPE_USER	(wqq_user_get_type())
#define WQQ_USER(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_USER,WqqUser))
#define WQQ_IS_USER(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_USER))
#define WQQ_USER_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WQQ_TYPE_USER,WqqUserClass))
#define WQQ_IS_USER_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass),\
				WQQ_TYPE_USER))
#define WQQ_USER_GET_CLASS(obj)	(G_TYPE_GET_INSTANCE_CLASS((obj),\
				WQQ_TYPE_USER,WqqUserClass))
typedef struct _WqqUser WqqUser;
typedef struct _WqqUserClass WqqUserClass;

typedef enum _WqqUserStatus WqqUserStatus;
typedef enum _WqqClientType WqqClientType;
typedef struct _WqqLevel WqqLevel;
typedef struct _WqqDate WqqDate;
typedef enum _WqqZodiac WqqZodiac;
typedef enum _WqqBloodType WqqBloodType;
typedef struct _WqqLocation WqqLocation;
typedef enum _WqqShengxiao WqqShengxiao;
typedef enum _WqqAllow WqqAllow;

enum _WqqUserStatus {
	WQQ_USER_STATUS_ONLINE = 10,	/* 10 */
	WQQ_USER_STATUS_OFFLINE = 20,	/* 20 */
	WQQ_USER_STATUS_AWAY = 30,	/* 30 */
	WQQ_USER_STATUS_HIDDEN = 40,	/* 40 */
	WQQ_USER_STATUS_BUSY = 50,	/* 50 */
	WQQ_USER_STATUS_CALLME = 60,	/* 60 */
	WQQ_USER_STATUS_SILENT = 70,	/* 70 */
};
GType wqq_user_status_get_type(void) G_GNUC_CONST;
#define WQQ_TYPE_USER_STATUS	(wqq_user_status_get_type())

enum _WqqClientType {
	WQQ_CLIENT_TYPE_PC,			/* 1,2,3,4,5,6,10,0x1e4 */
	WQQ_CLIENT_TYPE_WEBQQ,		/* 41 */
	WQQ_CLIENT_TYPE_MOBILE,		/* 21,22,23,24 */
	WQQ_CLIENT_TYPE_PAD,		/* 42 */
	WQQ_CLIENT_TYPE_OTHER,		/* unknown */
};
GType wqq_client_type_get_type(void) G_GNUC_CONST;
#define WQQ_TYPE_CLIENT_TYPE	(wqq_client_type_get_type())

struct _WqqLevel {
	guint level;
	guint days;
	guint hours;
	guint remain_days;
};

struct _WqqDate {
	guint year;
	guint month;
	guint day;
};

enum _WqqZodiac {
	WQQ_ZODIAC_ARIES = 0,		/* 白羊座 3.21~4.20 */
	WQQ_ZODIAC_TAURUS,			/* 金牛座 4.21~5.21 */
	WQQ_ZODIAC_GEMINI,			/* 双子座 5.22~6.21 */
	WQQ_ZODIAC_GANCER,			/* 巨蟹座 6.22~7.22 */
	WQQ_ZODIAC_LEO,				/* 狮子座 7.23~8.23 */
	WQQ_ZODIAC_VIRGO,			/* 处女座 8.24~9.23 */
	WQQ_ZODIAC_LIBRA,			/* 天平座 9.24~10.23 */
	WQQ_ZODIAC_SCORPIO,			/* 天蝎座 10.24~11.22 */
	WQQ_ZODIAC_SAGITTARIUS,		/* 射手座 11.23~12.21 */
	WQQ_ZODIAC_CAPRICORN,		/* 魔羯座 12.22~1.20 */
	WQQ_ZODIAC_AQUARIUS,		/* 水瓶座 1.21~2.19 */
	WQQ_ZODIAC_PISCES,			/* 双鱼座 2.20~3.20 */
};
GType wqq_zodiac_get_type(void) G_GNUC_CONST;
#define WQQ_TYPE_ZODIAC	(wqq_zodiac_get_type())

enum _WqqBloodType {
	WQQ_BLOOD_TYPE_A,
	WQQ_BLOOD_TYPE_B,
	WQQ_BLOOD_TYPE_AB,
	WQQ_BLOOD_TYPE_O,
	WQQ_BLOOD_TYPE_OTHER,
};
GType wqq_blood_type_get_type(void) G_GNUC_CONST;
#define WQQ_TYPE_BLOOD_TYPE	(wqq_blood_type_get_type())

struct _WqqLocation {
	gchar *country;
	gchar *province;
	gchar *city;
};

enum _WqqShengxiao {
	WQQ_SHENGXIAO_MOUSE = 1,
	WQQ_SHENGXIAO_COW,
	WQQ_SHENGXIAO_TIGER,
	WQQ_SHENGXIAO_RABBIT,
	WQQ_SHENGXIAO_LONG,
	WQQ_SHENGXIAO_SNAKE,
	WQQ_SHENGXIAO_HORSE,
	WQQ_SHENGXIAO_SHEEP,
	WQQ_SHENGXIAO_MONKEY,
	WQQ_SHENGXIAO_CHICKKEN,
	WQQ_SHENGXIAO_DOG,
	WQQ_SHENGXIAO_PIG,
};
GType wqq_shengxiao_get_type(void) G_GNUC_CONST;
#define WQQ_TYPE_SHENGXIAO	(wqq_shengxiao_get_type())

enum _WqqAllow {
	WQQ_ALLOW_ALL = 0,			/* 0 允许所有人加好友 */
	WQQ_ALLOW_CONFIRM,			/* 1 需要验证信息 */
	WQQ_ALLOW_REFUSE_ALL,		/* 2 拒绝所有 */
	WQQ_ALLOW_ANSWER,			/* 3 回答问题 */
	WQQ_ALLOW_ANSWER_AND_CONFIRM,	/* 4 回答问题且验证 */
};
GType wqq_allow_get_type(void) G_GNUC_CONST;
#define WQQ_TYPE_ALLOW	(wqq_allow_get_type())

struct _WqqUser {
	GObject object;
	/* private */
	guint64 uin;				/* 与qq帐号对应的一个值 */
	guint64 qq;					/* qq号码 */
	gchar *nick;				/* qq昵称 */
	WqqUserStatus status;		/* 在线状态 */
	WqqClientType client_type;	/* 客户端类型 */
	WqqLevel level;				/* 等级 */
	gulong login_date;			/* 登录时间？FIXME */
	gchar *sign;				/* 个性签名 */
	WqqDate birth;				/* 出生日期 */
	gchar *phone;				/* 电话 */
	gchar *mobile;				/* 手机 */
	gchar *email;				/* 电子邮件 */
	gchar *college;				/* 学校 */
	guint reg_time;				/* 注册时间？FIXME */
	WqqZodiac zodiac;			/* 星座 */
	WqqBloodType blood;			/* 血型 */
	gchar *gender;				/* 性别 */
	gchar *homepage;			/* 个人主页 */
	gint stat;					/* FIXME */
	guint is_vip;				/* 0表示不是会员 */
	guint vip_level;			/* vip等级 */
	WqqLocation location;		/* 所在位置 */
	gchar *personal;			/* 个人说明 */
	gchar *occupation;			/* 职业 */
	WqqShengxiao shengxiao;		/* 生肖 */
	gint flag;					/* FIXME */
	gint64 cip;					/* FIXME */
	guint face;					/* 头像？FIXME */
	WqqAllow allow;				/* 加好友的认证方式 */
};

struct _WqqUserClass {
	GObjectClass object_class;
};

GType wqq_user_get_type();

WqqUser *wqq_user_new(void);

guint64 wqq_user_get_uin(WqqUser * user);
void wqq_user_set_uin(WqqUser * user, guint64 uin);
guint64 wqq_user_get_qq(WqqUser * user);
void wqq_user_set_qq(WqqUser * user, guint64 qq);
const gchar *wqq_user_get_nick(WqqUser * user);
void wqq_user_set_nick(WqqUser * user, const gchar * nick);
WqqUserStatus wqq_user_get_status(WqqUser * user);
const gchar *wqq_user_get_status_to_string(WqqUser * user);
void wqq_user_set_status(WqqUser * user, WqqUserStatus status);
void wqq_user_set_status_from_string(WqqUser * user, const gchar * status);
WqqClientType wqq_user_get_client_type(WqqUser * user);
void wqq_user_set_client_type(WqqUser * user, WqqClientType type);
const WqqLevel *wqq_user_get_level(WqqUser * user);
void wqq_user_set_level(WqqUser * user, guint level, guint days,
						guint hours, guint remain_days);
const gchar *wqq_user_get_sign(WqqUser * user);
void wqq_user_set_sign(WqqUser * user, const gchar * sign);
const WqqDate *wqq_user_get_birth(WqqUser * user);
void wqq_user_set_birth(WqqUser * user, guint year, guint month,
						guint day);
const gchar *wqq_user_get_phone(WqqUser * user);
void wqq_user_set_phone(WqqUser * user, const gchar * phone);
const gchar *wqq_user_get_mobile(WqqUser * user);
void wqq_user_set_mobile(WqqUser * user, const gchar * mobile);
const gchar *wqq_user_get_email(WqqUser * user);
void wqq_user_set_email(WqqUser * user, const gchar * email);
const gchar *wqq_user_get_college(WqqUser * user);
void wqq_user_set_college(WqqUser * user, const gchar * college);
WqqZodiac wqq_user_get_zodiac(WqqUser * user);
void wqq_user_set_zodiac(WqqUser * user, WqqZodiac zodiac);
WqqBloodType wqq_user_get_blood_type(WqqUser * user);
void wqq_user_set_blood_type(WqqUser * user, WqqBloodType type);
const gchar *wqq_user_get_gender(WqqUser * user);
void wqq_user_set_gender(WqqUser * user, const gchar * gender);
const gchar *wqq_user_get_homepage(WqqUser * user);
void wqq_user_set_homepage(WqqUser * user, const gchar * homepage);
gboolean wqq_user_is_vip(WqqUser * user);
void wqq_user_set_vip(WqqUser * user, gboolean is_vip);
guint wqq_user_get_vip_level(WqqUser * user);
void wqq_user_set_vip_level(WqqUser * user, guint level);
const WqqLocation *wqq_user_get_location(WqqUser * user);
void wqq_user_set_location(WqqUser * user, const gchar * country,
						   const gchar * province, const gchar * city);
const gchar *wqq_user_get_occupation(WqqUser * user);
void wqq_user_set_occupation(WqqUser * user, const gchar * occupation);
const gchar *wqq_user_get_personal(WqqUser * user);
void wqq_user_set_personal(WqqUser * user, const gchar * personal);
WqqShengxiao wqq_user_get_shengxiao(WqqUser * user);
void wqq_user_set_shengxiao(WqqUser * user, WqqShengxiao shengxiao);
WqqAllow wqq_user_get_allow(WqqUser * user);
void wqq_user_set_allow(WqqUser * user, WqqAllow allow);
guint wqq_user_get_face(WqqUser * user);
void wqq_user_set_face(WqqUser * user, guint face);

const gchar *_wqq_user_status_to_string(WqqUserStatus status);
WqqUserStatus _wqq_user_status_to_enum(const gchar * string);
WqqClientType _wqq_user_client_type_to_enum(guint client_type);

G_END_DECLS
#endif
