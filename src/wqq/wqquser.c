#include "wqquser.h"

enum {
	WQQ_USER_PROPERTY_UIN = 1,
	WQQ_USER_PROPERTY_QQ,
	WQQ_USER_PROPERTY_NICK,
	WQQ_USER_PROPERTY_STATUS,
	WQQ_USER_PROPERTY_CLIENT_TYPE,
	WQQ_USER_PROPERTY_LEVEL,
	WQQ_USER_PROPERTY_SIGN,
	WQQ_USER_PROPERTY_BIRTH,
	WQQ_USER_PROPERTY_PHONE,
	WQQ_USER_PROPERTY_MOBILE,
	WQQ_USER_PROPERTY_EMAIL,
	WQQ_USER_PROPERTY_COLLEGE,
	WQQ_USER_PROPERTY_ZODIAC,
	WQQ_USER_PROPERTY_BLOOD,
	WQQ_USER_PROPERTY_GENDER,
	WQQ_USER_PROPERTY_HOMEPAGE,
	WQQ_USER_PROPERTY_IS_VIP,
	WQQ_USER_PROPERTY_VIP_LEVEL,
	WQQ_USER_PROPERTY_LOCATION,
	WQQ_USER_PROPERTY_PERSONAL,
	WQQ_USER_PROPERTY_OCCUPATION,
	WQQ_USER_PROPERTY_SHENGXIAO,
	WQQ_USER_PROPERTY_ALLOW,
	WQQ_USER_PROPERTY_FACE,
};

G_DEFINE_TYPE(WqqUser, wqq_user, G_TYPE_OBJECT);

static void wqq_user_getter(GObject * object, guint property_id,
							GValue * value, GParamSpec * pspec);
static void wqq_user_setter(GObject * object, guint property_id,
							const GValue * value, GParamSpec * pspec);

static void wqq_user_finalize(GObject * object);

static void wqq_user_init(WqqUser * user)
{
	user->uin = 0;
	user->qq = 0;
	user->nick = NULL;
	user->status = WQQ_USER_STATUS_OFFLINE;
	user->client_type = WQQ_CLIENT_TYPE_PC;
	user->level.level = 1;
	user->level.days = 1;
	user->level.hours = 1;
	user->level.remain_days = 1;
	user->sign = NULL;
	user->birth.year = 2000;
	user->birth.month = 1;
	user->birth.day = 1;
	user->phone = NULL;
	user->mobile = NULL;
	user->email = NULL;
	user->college = NULL;
	user->zodiac = WQQ_ZODIAC_ARIES;
	user->blood = WQQ_BLOOD_TYPE_O;
	user->gender = NULL;
	user->homepage = NULL;
	user->is_vip = 0;
	user->vip_level = 0;
	user->location.country = NULL;
	user->location.province = NULL;
	user->location.city = NULL;
	user->personal = NULL;
	user->occupation = NULL;
	user->face = 0;
	user->shengxiao = WQQ_SHENGXIAO_MOUSE;
	user->allow = WQQ_ALLOW_CONFIRM;

	/*g_object_weak_ref(G_OBJECT(user), wqq_user_finalize, NULL); */
}

static void wqq_user_finalize(GObject * object)
{
	WqqUser *user = WQQ_USER(object);

	g_free(user->nick);
	g_free(user->sign);
	g_free(user->phone);
	g_free(user->mobile);
	g_free(user->email);
	g_free(user->college);
	g_free(user->gender);
	g_free(user->homepage);
	g_free(user->location.country);
	g_free(user->location.province);
	g_free(user->location.city);
	g_free(user->personal);
	g_free(user->occupation);
}

static void wqq_user_class_init(WqqUserClass * klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->get_property = wqq_user_getter;
	object_class->set_property = wqq_user_setter;
	object_class->finalize = wqq_user_finalize;

	GParamSpec *ps;
	ps = g_param_spec_uint64("uin",
							 "user uin",
							 "User Uin",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_UIN, ps);

	ps = g_param_spec_uint64("qq",
							 "user qq number",
							 "User QQ Number",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_QQ, ps);

	ps = g_param_spec_string("nick",
							 "qq nick",
							 "QQ Nick",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_NICK, ps);

	ps = g_param_spec_enum("status",
						   "online status",
						   "Online Status",
						   WQQ_TYPE_USER_STATUS,
						   WQQ_USER_STATUS_ONLINE,
						   G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_STATUS, ps);

	ps = g_param_spec_enum("client-type",
						   "client type",
						   "Client Type",
						   WQQ_TYPE_CLIENT_TYPE,
						   WQQ_CLIENT_TYPE_PC,
						   G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_CLIENT_TYPE, ps);

	ps = g_param_spec_pointer("level",
							  "qq level",
							  "QQ Level",
							  G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_LEVEL, ps);

	ps = g_param_spec_string("sign",
							 "user sign",
							 "User Sign",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_SIGN, ps);

	ps = g_param_spec_pointer("birth",
							  "birthday",
							  "user birthday",
							  G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_BIRTH, ps);

	ps = g_param_spec_string("phone",
							 "user phone",
							 "User Phone",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_PHONE, ps);

	ps = g_param_spec_string("mobile",
							 "user mobile phone",
							 "User Mobile Phone",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_MOBILE, ps);

	ps = g_param_spec_string("email",
							 "user email",
							 "User Email",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_EMAIL, ps);

	ps = g_param_spec_string("college",
							 "user college",
							 "User college",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_COLLEGE, ps);

	ps = g_param_spec_enum("zodiac",
						   "user zodiac",
						   "User Zodiac",
						   WQQ_TYPE_ZODIAC,
						   WQQ_ZODIAC_ARIES,
						   G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_ZODIAC, ps);

	ps = g_param_spec_enum("blood",
						   "blood type",
						   "Blood Type",
						   WQQ_TYPE_BLOOD_TYPE,
						   WQQ_BLOOD_TYPE_O,
						   G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_BLOOD, ps);

	ps = g_param_spec_string("gender",
							 "user gender",
							 "User gender",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_GENDER, ps);

	ps = g_param_spec_string("homepage",
							 "user homepage",
							 "User Homepage",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_HOMEPAGE, ps);

	ps = g_param_spec_uint("is-vip",
						   "is vip?",
						   "Is Vip?",
						   0, 1, 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_IS_VIP, ps);

	ps = g_param_spec_uint("vip-level",
						   "vip level",
						   "Vip Level",
						   0, G_MAXUINT,
						   0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_VIP_LEVEL, ps);

	ps = g_param_spec_pointer("location",
							  "user location",
							  "User Location",
							  G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_LOCATION, ps);

	ps = g_param_spec_string("personal",
							 "user personal",
							 "User Personal",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_PERSONAL, ps);

	ps = g_param_spec_string("occupation",
							 "user occupation",
							 "User Occupation",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_OCCUPATION, ps);

	ps = g_param_spec_enum("shengxiao",
						   "user shengxiao",
						   "User Shengxiao",
						   WQQ_TYPE_SHENGXIAO,
						   WQQ_SHENGXIAO_MOUSE,
						   G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_SHENGXIAO, ps);

	ps = g_param_spec_enum("allow",
						   "user allow",
						   "User Allow",
						   WQQ_TYPE_ALLOW,
						   WQQ_ALLOW_CONFIRM,
						   G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_ALLOW, ps);

	ps = g_param_spec_uint("face",
						   "user face",
						   "User Face",
						   0, G_MAXUINT,
						   0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_USER_PROPERTY_FACE, ps);
}

static void wqq_user_getter(GObject * object, guint property_id,
							GValue * value, GParamSpec * pspec)
{
	WqqUser *user = WQQ_USER(object);
	gpointer ptr;
	switch (property_id) {
	case WQQ_USER_PROPERTY_UIN:
		g_value_set_uint64(value, user->uin);
		break;
	case WQQ_USER_PROPERTY_QQ:
		g_value_set_uint64(value, user->qq);
		break;
	case WQQ_USER_PROPERTY_NICK:
		g_value_set_string(value, user->nick);
		break;
	case WQQ_USER_PROPERTY_STATUS:
		g_value_set_enum(value, user->status);
		break;
	case WQQ_USER_PROPERTY_CLIENT_TYPE:
		g_value_set_enum(value, user->client_type);
		break;
	case WQQ_USER_PROPERTY_LEVEL:
		ptr = &(user->level);
		g_value_set_pointer(value, ptr);
		break;
	case WQQ_USER_PROPERTY_SIGN:
		g_value_set_string(value, user->sign);
		break;
	case WQQ_USER_PROPERTY_BIRTH:
		ptr = &(user->birth);
		g_value_set_pointer(value, ptr);
		break;
	case WQQ_USER_PROPERTY_PHONE:
		g_value_set_string(value, user->phone);
		break;
	case WQQ_USER_PROPERTY_MOBILE:
		g_value_set_string(value, user->mobile);
		break;
	case WQQ_USER_PROPERTY_EMAIL:
		g_value_set_string(value, user->email);
		break;
	case WQQ_USER_PROPERTY_COLLEGE:
		g_value_set_string(value, user->college);
		break;
	case WQQ_USER_PROPERTY_ZODIAC:
		g_value_set_enum(value, user->zodiac);
		break;
	case WQQ_USER_PROPERTY_BLOOD:
		g_value_set_enum(value, user->blood);
		break;
	case WQQ_USER_PROPERTY_GENDER:
		g_value_set_string(value, user->gender);
		break;
	case WQQ_USER_PROPERTY_HOMEPAGE:
		g_value_set_string(value, user->homepage);
		break;
	case WQQ_USER_PROPERTY_IS_VIP:
		g_value_set_uint(value, user->is_vip);
		break;
	case WQQ_USER_PROPERTY_VIP_LEVEL:
		g_value_set_uint(value, user->vip_level);
		break;
	case WQQ_USER_PROPERTY_LOCATION:
		ptr = &(user->location);
		g_value_set_pointer(value, ptr);
		break;
	case WQQ_USER_PROPERTY_PERSONAL:
		g_value_set_string(value, user->personal);
		break;
	case WQQ_USER_PROPERTY_OCCUPATION:
		g_value_set_string(value, user->occupation);
		break;
	case WQQ_USER_PROPERTY_SHENGXIAO:
		g_value_set_enum(value, user->shengxiao);
		break;
	case WQQ_USER_PROPERTY_ALLOW:
		g_value_set_enum(value, user->allow);
		break;
	case WQQ_USER_PROPERTY_FACE:
		g_value_set_uint(value, user->face);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void wqq_user_setter(GObject * object, guint property_id,
							const GValue * value, GParamSpec * pspec)
{
	WqqUser *user = WQQ_USER(object);
	gpointer ptr;
	switch (property_id) {
	case WQQ_USER_PROPERTY_UIN:
		user->uin = g_value_get_uint64(value);
		break;
	case WQQ_USER_PROPERTY_QQ:
		user->qq = g_value_get_uint64(value);
		break;
	case WQQ_USER_PROPERTY_NICK:
		g_free(user->nick);
		user->nick = g_strdup(g_value_get_string(value));
		break;
	case WQQ_USER_PROPERTY_STATUS:
		user->status = g_value_get_enum(value);
		break;
	case WQQ_USER_PROPERTY_CLIENT_TYPE:
		user->client_type = g_value_get_enum(value);
		break;
	case WQQ_USER_PROPERTY_LEVEL:
		ptr = g_value_get_pointer(value);
		user->level.level = ((WqqLevel *) ptr)->level;
		user->level.days = ((WqqLevel *) ptr)->days;
		user->level.hours = ((WqqLevel *) ptr)->hours;
		user->level.remain_days = ((WqqLevel *) ptr)->remain_days;
		break;
	case WQQ_USER_PROPERTY_SIGN:
		g_free(user->sign);
		user->sign = g_strdup(g_value_get_string(value));
		break;
	case WQQ_USER_PROPERTY_BIRTH:
		ptr = g_value_get_pointer(value);
		user->birth.year = ((WqqDate *) ptr)->year;
		user->birth.month = ((WqqDate *) ptr)->month;
		user->birth.day = ((WqqDate *) ptr)->day;
		break;
	case WQQ_USER_PROPERTY_PHONE:
		g_free(user->phone);
		user->phone = g_strdup(g_value_get_string(value));
		break;
	case WQQ_USER_PROPERTY_MOBILE:
		g_free(user->mobile);
		user->mobile = g_strdup(g_value_get_string(value));
		break;
	case WQQ_USER_PROPERTY_EMAIL:
		g_free(user->email);
		user->email = g_strdup(g_value_get_string(value));
		break;
	case WQQ_USER_PROPERTY_COLLEGE:
		g_free(user->college);
		user->college = g_strdup(g_value_get_string(value));
		break;
	case WQQ_USER_PROPERTY_ZODIAC:
		user->zodiac = g_value_get_enum(value);
		break;
	case WQQ_USER_PROPERTY_BLOOD:
		user->blood = g_value_get_enum(value);
		break;
	case WQQ_USER_PROPERTY_GENDER:
		g_free(user->gender);
		user->gender = g_strdup(g_value_get_string(value));
		break;
	case WQQ_USER_PROPERTY_HOMEPAGE:
		g_free(user->homepage);
		user->homepage = g_strdup(g_value_get_string(value));
		break;
	case WQQ_USER_PROPERTY_IS_VIP:
		user->is_vip = g_value_get_uint(value);
		break;
	case WQQ_USER_PROPERTY_VIP_LEVEL:
		user->vip_level = g_value_get_uint(value);
		break;
	case WQQ_USER_PROPERTY_LOCATION:
		ptr = g_value_get_pointer(value);
		user->location.country = g_strdup(((WqqLocation *) ptr)->country);
		user->location.province =
			g_strdup(((WqqLocation *) ptr)->province);
		user->location.city = g_strdup(((WqqLocation *) ptr)->city);
		break;
	case WQQ_USER_PROPERTY_PERSONAL:
		g_free(user->personal);
		user->personal = g_strdup(g_value_get_string(value));
		break;
	case WQQ_USER_PROPERTY_OCCUPATION:
		g_free(user->occupation);
		user->occupation = g_strdup(g_value_get_string(value));
		break;
	case WQQ_USER_PROPERTY_SHENGXIAO:
		user->shengxiao = g_value_get_enum(value);
		break;
	case WQQ_USER_PROPERTY_ALLOW:
		user->allow = g_value_get_enum(value);
		break;
	case WQQ_USER_PROPERTY_FACE:
		user->face = g_value_get_uint(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

/*********************************************************
 * PUBLIC
 ********************************************************/
WqqUser *wqq_user_new(void)
{
	return (WqqUser *) g_object_new(WQQ_TYPE_USER, NULL);
}

guint64 wqq_user_get_uin(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), 0);
	return user->uin;
}

void wqq_user_set_uin(WqqUser * user, guint64 uin)
{
	g_return_if_fail(WQQ_IS_USER(user));
	user->uin = uin;
}

guint64 wqq_user_get_qq(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), 0);
	return user->qq;
}

void wqq_user_set_qq(WqqUser * user, guint64 qq)
{
	g_return_if_fail(WQQ_IS_USER(user));
	user->qq = qq;
}

const gchar *wqq_user_get_nick(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), NULL);
	return user->nick;
}

void wqq_user_set_nick(WqqUser * user, const gchar * nick)
{
	g_return_if_fail(WQQ_IS_USER(user));
	g_free(user->nick);
	user->nick = g_strdup(nick);
}

WqqUserStatus wqq_user_get_status(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), 0);
	return user->status;
}

const gchar *wqq_user_get_status_to_string(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), NULL);
	switch (user->status) {
	case WQQ_USER_STATUS_ONLINE:
		return "online";
		break;
	case WQQ_USER_STATUS_AWAY:
		return "away";
		break;
	case WQQ_USER_STATUS_BUSY:
		return "busy";
		break;
	case WQQ_USER_STATUS_SILENT:
		return "silent";
		break;
	case WQQ_USER_STATUS_CALLME:
		return "callme";
		break;
	case WQQ_USER_STATUS_HIDDEN:
		return "hidden";
		break;
	default:
		return "offline";
	}
	return "offline";
}

void wqq_user_set_status(WqqUser * user, WqqUserStatus status)
{
	g_return_if_fail(WQQ_IS_USER(user));
	user->status = status;
}

void wqq_user_set_status_from_string(WqqUser * user, const gchar * status)
{
	g_return_if_fail(WQQ_IS_USER(user));
	if (g_strcmp0(status, "online") == 0)
		user->status = WQQ_USER_STATUS_ONLINE;
	else if (g_strcmp0(status, "busy") == 0)
		user->status = WQQ_USER_STATUS_BUSY;
	else if (g_strcmp0(status, "away") == 0)
		user->status = WQQ_USER_STATUS_AWAY;
	else if (g_strcmp0(status, "silent") == 0)
		user->status = WQQ_USER_STATUS_SILENT;
	else if (g_strcmp0(status, "callme") == 0)
		user->status = WQQ_USER_STATUS_CALLME;
	else if (g_strcmp0(status, "hidden") == 0)
		user->status = WQQ_USER_STATUS_HIDDEN;
	else
		user->status = WQQ_USER_STATUS_OFFLINE;
}

WqqClientType wqq_user_get_client_type(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), 0);
	return user->client_type;
}

void wqq_user_set_client_type(WqqUser * user, WqqClientType type)
{
	g_return_if_fail(WQQ_IS_USER(user));
	user->client_type = type;
}

const WqqLevel *wqq_user_get_level(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), NULL);
	WqqLevel *level = &(user->level);
	return level;
}

void wqq_user_set_level(WqqUser * user, guint level, guint days,
						guint hours, guint remain_days)
{
	g_return_if_fail(WQQ_IS_USER(user));
	user->level.level = level;
	user->level.days = days;
	user->level.hours = hours;
	user->level.remain_days = remain_days;
}

const gchar *wqq_user_get_sign(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), NULL);
	return user->sign;
}

void wqq_user_set_sign(WqqUser * user, const gchar * sign)
{
	g_return_if_fail(WQQ_IS_USER(user));
	g_free(user->sign);
	user->sign = g_strdup(sign);
}

const WqqDate *wqq_user_get_birth(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), NULL);
	WqqDate *date = &(user->birth);
	return date;
}

void wqq_user_set_birth(WqqUser * user, guint year, guint month, guint day)
{
	g_return_if_fail(WQQ_IS_USER(user));
	user->birth.year = year;
	user->birth.month = month;
	user->birth.day = day;
}

const gchar *wqq_user_get_phone(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), NULL);
	return user->phone;
}

void wqq_user_set_phone(WqqUser * user, const gchar * phone)
{
	g_return_if_fail(WQQ_IS_USER(user));
	g_free(user->phone);
	user->phone = g_strdup(phone);
}

const gchar *wqq_user_get_mobile(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), NULL);
	return user->mobile;
}

void wqq_user_set_mobile(WqqUser * user, const gchar * mobile)
{
	g_return_if_fail(WQQ_IS_USER(user));
	g_free(user->mobile);
	user->mobile = g_strdup(mobile);
}

const gchar *wqq_user_get_email(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), NULL);
	return user->email;
}

void wqq_user_set_email(WqqUser * user, const gchar * email)
{
	g_return_if_fail(WQQ_IS_USER(user));
	g_free(user->email);
	user->email = g_strdup(email);
}

const gchar *wqq_user_get_college(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), NULL);
	return user->college;
}

void wqq_user_set_college(WqqUser * user, const gchar * college)
{
	g_return_if_fail(WQQ_IS_USER(user));
	g_free(user->college);
	user->college = g_strdup(college);
}

WqqZodiac wqq_user_get_zodiac(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), 0);
	return user->zodiac;
}

void wqq_user_set_zodiac(WqqUser * user, WqqZodiac zodiac)
{
	g_return_if_fail(WQQ_IS_USER(user));
	user->zodiac = zodiac;
}

WqqBloodType wqq_user_get_blood_type(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), 0);
	return user->blood;
}

void wqq_user_set_blood_type(WqqUser * user, WqqBloodType type)
{
	g_return_if_fail(WQQ_IS_USER(user));
	user->blood = type;
}

const gchar *wqq_user_get_gender(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), NULL);
	return user->gender;
}

void wqq_user_set_gender(WqqUser * user, const gchar * gender)
{
	g_return_if_fail(WQQ_IS_USER(user));
	g_free(user->gender);
	user->gender = g_strdup(gender);
}

const gchar *wqq_user_get_homepage(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), NULL);
	return user->homepage;
}

void wqq_user_set_homepage(WqqUser * user, const gchar * homepage)
{
	g_return_if_fail(WQQ_IS_USER(user));
	g_free(user->homepage);
	user->homepage = g_strdup(homepage);
}

gboolean wqq_user_is_vip(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), FALSE);
	return user->is_vip == 0 ? FALSE : TRUE;
}

void wqq_user_set_vip(WqqUser * user, gboolean is_vip)
{
	g_return_if_fail(WQQ_IS_USER(user));
	user->is_vip = is_vip;
}

guint wqq_user_get_vip_level(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), 0);
	return user->vip_level;
}

void wqq_user_set_vip_level(WqqUser * user, guint level)
{
	g_return_if_fail(WQQ_IS_USER(user));
	user->vip_level = level;
}

const WqqLocation *wqq_user_get_location(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), NULL);
	WqqLocation *l = &(user->location);
	return l;
}

void wqq_user_set_location(WqqUser * user, const gchar * country,
						   const gchar * province, const gchar * city)
{
	g_return_if_fail(WQQ_IS_USER(user));
	g_free(user->location.country);
	g_free(user->location.province);
	g_free(user->location.city);
	user->location.country = g_strdup(country);
	user->location.province = g_strdup(province);
	user->location.city = g_strdup(city);
}

const gchar *wqq_user_get_occupation(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), NULL);
	return user->occupation;
}

void wqq_user_set_occupation(WqqUser * user, const gchar * occupation)
{
	g_return_if_fail(WQQ_IS_USER(user));
	g_free(user->occupation);
	user->occupation = g_strdup(occupation);
}

const gchar *wqq_user_get_personal(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), NULL);
	return user->personal;
}

void wqq_user_set_personal(WqqUser * user, const gchar * personal)
{
	g_return_if_fail(WQQ_IS_USER(user));
	g_free(user->personal);
	user->personal = g_strdup(personal);
}

WqqShengxiao wqq_user_get_shengxiao(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), 0);
	return user->shengxiao;
}

void wqq_user_set_shengxiao(WqqUser * user, WqqShengxiao shengxiao)
{
	g_return_if_fail(WQQ_IS_USER(user));
	user->shengxiao = shengxiao;
}

WqqAllow wqq_user_get_allow(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), 0);
	return user->allow;
}

void wqq_user_set_allow(WqqUser * user, WqqAllow allow)
{
	g_return_if_fail(WQQ_IS_USER(user));
	user->allow = allow;
}

guint wqq_user_get_face(WqqUser * user)
{
	g_return_val_if_fail(WQQ_IS_USER(user), 0);
	return user->face;
}

void wqq_user_set_face(WqqUser * user, guint face)
{
	g_return_if_fail(WQQ_IS_USER(user));
	user->face = face;
}

GType wqq_user_status_get_type(void)
{
	static GType etype = 0;
	if (G_UNLIKELY(etype == 0)) {
		static const GEnumValue values[] = {
			{WQQ_USER_STATUS_ONLINE, "WQQ_USER_STATUS_ONLINE", "online"},
			{WQQ_USER_STATUS_OFFLINE, "WQQ_USER_STATUS_OFFLINE",
			 "offline"},
			{WQQ_USER_STATUS_BUSY, "WQQ_USER_STATUS_BUSY", "busy"},
			{WQQ_USER_STATUS_AWAY, "WQQ_USER_STATUS_AWAY", "away"},
			{WQQ_USER_STATUS_SILENT, "WQQ_USER_STATUS_SILENT", "silent"},
			{WQQ_USER_STATUS_CALLME, "WQQ_USER_STATUS_CALLME", "callme"},
			{WQQ_USER_STATUS_HIDDEN, "WQQ_USER_STATUS_HIDDEN", "hidden"},
			{0, NULL, NULL}
		};
		etype =
			g_enum_register_static(g_intern_static_string("WqqUserStatus"),
								   values);
	}
	return etype;
}

GType wqq_client_type_get_type(void)
{
	static GType etype = 0;
	if (G_UNLIKELY(etype == 0)) {
		static const GEnumValue values[] = {
			{WQQ_CLIENT_TYPE_PC, "WQQ_CLIENT_TYPE_PC", "pc"},
			{WQQ_CLIENT_TYPE_WEBQQ, "WQQ_CLIENT_TYPE_WEBQQ", "webqq"},
			{WQQ_CLIENT_TYPE_MOBILE, "WQQ_CLIENT_TYPE_MOBILE", "mobile"},
			{WQQ_CLIENT_TYPE_PAD, "WQQ_CLIENT_TYPE_PAD", "pad"},
			{WQQ_CLIENT_TYPE_OTHER, "WQQ_CLIENT_TYPE_OTHER", "other"},
			{0, NULL, NULL}
		};
		etype =
			g_enum_register_static(g_intern_static_string("WqqClientType"),
								   values);
	}
	return etype;
}

GType wqq_zodiac_get_type(void)
{
	static GType etype = 0;
	if (G_UNLIKELY(etype == 0)) {
		static const GEnumValue values[] = {
			{WQQ_ZODIAC_ARIES, "WQQ_ZODIAC_ARIES", "aries"},
			{WQQ_ZODIAC_PISCES, "WQQ_ZODIAC_PISCES", "pisces"},
			{WQQ_ZODIAC_AQUARIUS, "WQQ_ZODIAC_AQUARIUS", "aquarius"},
			{WQQ_ZODIAC_CAPRICORN, "WQQ_ZODIAC_CAPRICORN", "capricorn"},
			{WQQ_ZODIAC_SAGITTARIUS, "WQQ_ZODIAC_SAGITTARIUS",
			 "sagittarius"},
			{WQQ_ZODIAC_SCORPIO, "WQQ_ZODIAC_SCORPIO", "scorpio"},
			{WQQ_ZODIAC_LIBRA, "WQQ_ZODIAC_LIBRA", "libra"},
			{WQQ_ZODIAC_VIRGO, "WQQ_ZODIAC_VIRGO", "virgo"},
			{WQQ_ZODIAC_LEO, "WQQ_ZODIAC_LEO", "leo"},
			{WQQ_ZODIAC_GANCER, "WQQ_ZODIAC_GANCER", "gancer"},
			{WQQ_ZODIAC_GEMINI, "WQQ_ZODIAC_GEMINI", "gemini"},
			{WQQ_ZODIAC_TAURUS, "WQQ_ZODIAC_TAURUS", "taurus"},
			{0, NULL, NULL},
		};
		etype = g_enum_register_static(g_intern_static_string("WqqZodiac"),
									   values);
	}
	return etype;
}

GType wqq_blood_type_get_type(void)
{
	static GType etype = 0;
	if (G_UNLIKELY(etype == 0)) {
		static const GEnumValue values[] = {
			{WQQ_BLOOD_TYPE_A, "WQQ_BLOOD_TYPE_A", "blood_a"},
			{WQQ_BLOOD_TYPE_O, "WQQ_BLOOD_TYPE_O", "blood_o"},
			{WQQ_BLOOD_TYPE_B, "WQQ_BLOOD_TYPE_B", "blood_b"},
			{WQQ_BLOOD_TYPE_AB, "WQQ_BLOOD_TYPE_AB", "blood_ab"},
			{WQQ_BLOOD_TYPE_OTHER, "WQQ_BLOOD_TYPE_OTHER", "blood_other"},
			{0, NULL, NULL}
		};
		etype =
			g_enum_register_static(g_intern_static_string("WqqBloodType"),
								   values);
	}
	return etype;
}

GType wqq_shengxiao_get_type(void)
{
	static GType etype = 0;
	if (G_UNLIKELY(etype == 0)) {
		static const GEnumValue values[] = {
			{WQQ_SHENGXIAO_MOUSE, "WQQ_SHENGXIAO_MOUSE", "mouse"},
			{WQQ_SHENGXIAO_COW, "WQQ_SHENGXIAO_COW", "cow"},
			{WQQ_SHENGXIAO_TIGER, "WQQ_SHENGXIAO_TIGER", "tiger"},
			{WQQ_SHENGXIAO_RABBIT, "WQQ_SHENGXIAO_RABBIT", "rabbit"},
			{WQQ_SHENGXIAO_LONG, "WQQ_SHENGXIAO_LONG", "long"},
			{WQQ_SHENGXIAO_SNAKE, "WQQ_SHENGXIAO_SNAKE", "snake"},
			{WQQ_SHENGXIAO_HORSE, "WQQ_SHENGXIAO_HORSE", "horse"},
			{WQQ_SHENGXIAO_SHEEP, "WQQ_SHENGXIAO_SHEEP", "sheep"},
			{WQQ_SHENGXIAO_MONKEY, "WQQ_SHENGXIAO_MONKEY", "monkey"},
			{WQQ_SHENGXIAO_CHICKKEN, "WQQ_SHENGXIAO_CHICKKEN", "chicken"},
			{WQQ_SHENGXIAO_DOG, "WQQ_SHENGXIAO_DOG", "dog"},
			{WQQ_SHENGXIAO_PIG, "WQQ_SHENGXIAO_PIG", "pig"},
			{0, NULL, NULL}
		};
		etype =
			g_enum_register_static(g_intern_static_string("WqqShengxiao"),
								   values);
	}
	return etype;
}

GType wqq_allow_get_type(void)
{
	static GType etype = 0;
	if (G_UNLIKELY(etype == 0)) {
		static const GEnumValue values[] = {
			{WQQ_ALLOW_ALL, "WQQ_ALLOW_ALL", "all"},
			{WQQ_ALLOW_CONFIRM, "WQQ_ALLOW_CONFIRM", "confirm"},
			{WQQ_ALLOW_REFUSE_ALL, "WQQ_ALLOW_REFUSE_ALL", "refuse"},
			{WQQ_ALLOW_ANSWER, "WQQ_ALLOW_ANSWER", "answer"},
			{WQQ_ALLOW_ANSWER_AND_CONFIRM, "WQQ_ALLOW_ANSWER_AND_CONFIRM",
			 "answer&confirm"},
			{0, NULL, NULL}
		};
		etype =
			g_enum_register_static(g_intern_static_string("WqqAllow"),
								   values);
	}
	return etype;
}



/* 将在线状态的枚举类型转化为字符串 */
const gchar *_wqq_user_status_to_string(WqqUserStatus status)
{
	switch (status) {
	case WQQ_USER_STATUS_ONLINE:
		return "online";
	case WQQ_USER_STATUS_AWAY:
		return "away";
	case WQQ_USER_STATUS_BUSY:
		return "busy";
	case WQQ_USER_STATUS_SILENT:
		return "silent";
	case WQQ_USER_STATUS_CALLME:
		return "callme";
	case WQQ_USER_STATUS_HIDDEN:
		return "hidden";
	default:
		return "offline";
	}
	return "offline";
}

/* 将字符串表示的状态转化为枚举类型 */
WqqUserStatus _wqq_user_status_to_enum(const gchar * string)
{
	if (g_strcmp0(string, "online") == 0)
		return WQQ_USER_STATUS_ONLINE;
	else if (g_strcmp0(string, "busy") == 0)
		return WQQ_USER_STATUS_BUSY;
	else if (g_strcmp0(string, "away") == 0)
		return WQQ_USER_STATUS_AWAY;
	else if (g_strcmp0(string, "silent") == 0)
		return WQQ_USER_STATUS_SILENT;
	else if (g_strcmp0(string, "callme") == 0)
		return WQQ_USER_STATUS_CALLME;
	else if (g_strcmp0(string, "hidden") == 0)
		return WQQ_USER_STATUS_HIDDEN;
	return WQQ_USER_STATUS_OFFLINE;
}

/* 将客户端类型转化为枚举 */
WqqClientType _wqq_user_client_type_to_enum(guint client_type)
{
	if (client_type == 41)
		return WQQ_CLIENT_TYPE_WEBQQ;
	else if (client_type == 42)
		return WQQ_CLIENT_TYPE_PAD;
	else if (client_type >= 21 && client_type <= 24)
		return WQQ_CLIENT_TYPE_MOBILE;
	else
		return WQQ_CLIENT_TYPE_PC;
}
