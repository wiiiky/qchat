#include "wqqgroup.h"

enum {
	WQQ_GROUP_PROPERTY_GID = 1,
	WQQ_GROUP_PROPERTY_GIN,
	WQQ_GROUP_PROPERTY_LEVEL,
	WQQ_GROUP_PROPERTY_CODE,
	WQQ_GROUP_PROPERTY_FLAG,
	WQQ_GROUP_PROPERTY_MASK,
	WQQ_GROUP_PROPERTY_NAME,
	WQQ_GROUP_PROPERTY_MARK,
	WQQ_GROUP_PROPERTY_CT,
	WQQ_GROUP_PROPERTY_OWNER,
	WQQ_GROUP_PROPERTY_MEMBER,
};

G_DEFINE_TYPE(WqqGroup, wqq_group, G_TYPE_OBJECT);

static void wqq_group_getter(GObject * object, guint property_id,
							 GValue * value, GParamSpec * pspec);
static void wqq_group_setter(GObject * object, guint property_id,
							 const GValue * value, GParamSpec * pspec);

static void wqq_group_init(WqqGroup * group)
{
	group->gid = 0;
	group->gin = 0;
	group->level = 0;
	group->code = 0;
	group->flag = 0;
	group->mask = WQQ_GROUP_MASK_OK;
	group->name = NULL;
	group->mark = NULL;
	group->ct = 0;
	group->owner = 0;
	group->members = NULL;
	group->data = NULL;
}

static void wqq_group_finalize(GObject * object)
{
	WqqGroup *group = WQQ_GROUP(object);
	g_free(group->name);
	g_free(group->mark);
	g_list_free_full(group->members, g_object_unref);
}

static void wqq_group_class_init(WqqGroupClass * klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->get_property = wqq_group_getter;
	object_class->set_property = wqq_group_setter;
	object_class->finalize = wqq_group_finalize;

	GParamSpec *ps;
	ps = g_param_spec_uint64("gid",
							 "group id",
							 "Group Id",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_GROUP_PROPERTY_GID, ps);

	ps = g_param_spec_uint64("gin",
							 "group in",
							 "Group In",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_GROUP_PROPERTY_GIN, ps);

	ps = g_param_spec_uint("level",
						   "group level",
						   "Group Level",
						   0, G_MAXUINT,
						   0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_GROUP_PROPERTY_LEVEL, ps);

	ps = g_param_spec_uint64("code",
							 "group code",
							 "Group Code",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_GROUP_PROPERTY_CODE, ps);

	ps = g_param_spec_uint64("flag",
							 "group flag",
							 "Group Flag",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_GROUP_PROPERTY_FLAG, ps);

	ps = g_param_spec_enum("mask",
						   "group mask",
						   "Group Mask",
						   WQQ_TYPE_GROUP_MASK,
						   WQQ_GROUP_MASK_OK,
						   G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_GROUP_PROPERTY_MASK, ps);

	ps = g_param_spec_string("name",
							 "group name",
							 "Group Name",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_GROUP_PROPERTY_NAME, ps);

	ps = g_param_spec_string("mark",
							 "group markname",
							 "Group Markname",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_GROUP_PROPERTY_MARK, ps);

	ps = g_param_spec_uint64("ct",
							 "create time",
							 "Create Time",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_GROUP_PROPERTY_CT, ps);

	ps = g_param_spec_uint64("owner",
							 "group owner",
							 "Group Owner",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_GROUP_PROPERTY_OWNER, ps);

	ps = g_param_spec_pointer("members",
							  "group members",
							  "Group Members",
							  G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_GROUP_PROPERTY_MEMBER, ps);
}


static void wqq_group_getter(GObject * object, guint property_id,
							 GValue * value, GParamSpec * pspec)
{
	WqqGroup *group = WQQ_GROUP(object);
	switch (property_id) {
	case WQQ_GROUP_PROPERTY_GID:
		g_value_set_uint64(value, group->gid);
		break;
	case WQQ_GROUP_PROPERTY_GIN:
		g_value_set_uint64(value, group->gin);
		break;
	case WQQ_GROUP_PROPERTY_LEVEL:
		g_value_set_uint(value, group->level);
		break;
	case WQQ_GROUP_PROPERTY_CODE:
		g_value_set_uint64(value, group->code);
		break;
	case WQQ_GROUP_PROPERTY_FLAG:
		g_value_set_uint64(value, group->flag);
		break;
	case WQQ_GROUP_PROPERTY_MASK:
		g_value_set_enum(value, group->mask);
		break;
	case WQQ_GROUP_PROPERTY_NAME:
		g_value_set_string(value, group->name);
		break;
	case WQQ_GROUP_PROPERTY_MARK:
		g_value_set_string(value, group->name);
		break;
	case WQQ_GROUP_PROPERTY_CT:
		g_value_set_uint64(value, group->ct);
		break;
	case WQQ_GROUP_PROPERTY_OWNER:
		g_value_set_uint64(value, group->owner);
		break;
	case WQQ_GROUP_PROPERTY_MEMBER:
		g_value_set_pointer(value, group->members);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void wqq_group_setter(GObject * object, guint property_id,
							 const GValue * value, GParamSpec * pspec)
{
	WqqGroup *group = WQQ_GROUP(object);
	switch (property_id) {
	case WQQ_GROUP_PROPERTY_GID:
		group->gid = g_value_get_uint64(value);
		break;
	case WQQ_GROUP_PROPERTY_GIN:
		group->gin = g_value_get_uint64(value);
		break;
	case WQQ_GROUP_PROPERTY_LEVEL:
		group->level = g_value_get_uint(value);
		break;
	case WQQ_GROUP_PROPERTY_CODE:
		group->code = g_value_get_uint64(value);
		break;
	case WQQ_GROUP_PROPERTY_FLAG:
		group->flag = g_value_get_uint64(value);
		break;
	case WQQ_GROUP_PROPERTY_MASK:
		group->level = g_value_get_enum(value);
		break;
	case WQQ_GROUP_PROPERTY_NAME:
		g_free(group->name);
		group->name = g_strdup(g_value_get_string(value));
		break;
	case WQQ_GROUP_PROPERTY_MARK:
		g_free(group->mark);
		group->mark = g_strdup(g_value_get_string(value));
		break;
	case WQQ_GROUP_PROPERTY_CT:
		group->ct = g_value_get_uint64(value);
		break;
	case WQQ_GROUP_PROPERTY_OWNER:
		group->owner = g_value_get_uint64(value);
		break;
	case WQQ_GROUP_PROPERTY_MEMBER:
		group->members = g_value_get_pointer(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}


GType wqq_group_mask_get_type(void)
{
	static GType etype = 0;
	if (G_UNLIKELY(etype == 0)) {
		static const GEnumValue values[] = {
			{WQQ_GROUP_MASK_OK, "WQQ_GROUP_MASK_OK", "ok"},
			{WQQ_GROUP_MASK_NO_NOTIFY, "WQQ_GROUP_MASK_NO_NOTIFY",
			 "no-notfiy"},
			{WQQ_GROUP_MASK_BAN, "WQQ_GROUP_MASK_BAN", "ban"},
			{0, NULL, NULL}
		};
		etype =
			g_enum_register_static(g_intern_static_string("WqqGroupMask"),
								   values);
	}
	return etype;
}

/*********************************************************
 * PUBLIC
 ********************************************************/

WqqGroup *wqq_group_new(guint64 gid, const gchar * name)
{
	return (WqqGroup *) g_object_new(WQQ_TYPE_GROUP,
									 "gid", gid, "name", name, NULL);
}

guint64 wqq_group_get_gid(WqqGroup * group)
{
	g_return_val_if_fail(WQQ_IS_GROUP(group), 0);
	return group->gid;
}

guint64 wqq_group_get_gin(WqqGroup * group)
{
	g_return_val_if_fail(WQQ_IS_GROUP(group), 0);
	return group->gin;
}

guint wqq_group_get_level(WqqGroup * group)
{
	g_return_val_if_fail(WQQ_IS_GROUP(group), 0);
	return group->level;
}

const gchar *wqq_group_get_name(WqqGroup * group)
{
	g_return_val_if_fail(WQQ_IS_GROUP(group), NULL);
	return group->name;
}

const gchar *wqq_group_get_mark(WqqGroup * group)
{
	g_return_val_if_fail(WQQ_IS_GROUP(group), NULL);
	return group->mark;
}

guint64 wqq_group_get_owner(WqqGroup * group)
{
	g_return_val_if_fail(WQQ_IS_GROUP(group), 0);
	return group->owner;
}

GList *wqq_group_get_members(WqqGroup * group)
{
	g_return_val_if_fail(WQQ_IS_GROUP(group), NULL);
	return group->members;
}

guint64 wqq_group_get_code(WqqGroup * group)
{
	g_return_val_if_fail(WQQ_IS_GROUP(group), 0);
	return group->code;
}

guint64 wqq_group_get_flag(WqqGroup * group)
{
	g_return_val_if_fail(WQQ_IS_GROUP(group), 0);
	return group->flag;
}

guint wqq_group_get_mask(WqqGroup * group)
{
	g_return_val_if_fail(WQQ_IS_GROUP(group), 0);
	return group->mask;
}

guint64 wqq_group_get_createtime(WqqGroup * group)
{
	g_return_val_if_fail(WQQ_IS_GROUP(group), 0);
	return group->ct;
}

void wqq_group_set_createtime(WqqGroup * group, guint64 createtime)
{
	g_return_if_fail(WQQ_IS_GROUP(group));
	group->ct = createtime;
}

void wqq_group_set_mask(WqqGroup * group, WqqGroupMask mask)
{
	g_return_if_fail(WQQ_IS_GROUP(group));
	group->mask = mask;
}

void wqq_group_set_flag(WqqGroup * group, guint64 flag)
{
	g_return_if_fail(WQQ_IS_GROUP(group));
	group->flag = flag;
}

void wqq_group_set_code(WqqGroup * group, guint64 code)
{
	g_return_if_fail(WQQ_IS_GROUP(group));
	group->code = code;
}

void wqq_group_set_gid(WqqGroup * group, guint64 gid)
{
	g_return_if_fail(WQQ_IS_GROUP(group));
	group->gid = gid;
}

void wqq_group_set_gin(WqqGroup * group, guint64 gin)
{
	g_return_if_fail(WQQ_IS_GROUP(group));
	group->gin = gin;
}

void wqq_group_set_level(WqqGroup * group, guint level)
{
	g_return_if_fail(WQQ_IS_GROUP(group));
	group->level = level;
}

void wqq_group_set_name(WqqGroup * group, const gchar * name)
{
	g_return_if_fail(WQQ_IS_GROUP(group));
	g_free(group->name);
	group->name = g_strdup(name);
}

void wqq_group_set_mark(WqqGroup * group, const gchar * mark)
{
	g_return_if_fail(WQQ_IS_GROUP(group));
	g_free(group->mark);
	group->mark = g_strdup(mark);
}

void wqq_group_set_owner(WqqGroup * group, guint64 owner)
{
	g_return_if_fail(WQQ_IS_GROUP(group));
	group->owner = owner;
}


/* 群成员相关 */
WqqGroupMember *wqq_group_append_group_member(WqqGroup * group,
											  guint64 uin)
{
	g_return_val_if_fail(WQQ_IS_GROUP(group), NULL);
	WqqGroupMember *member = wqq_group_member_new();
	wqq_user_set_uin(WQQ_USER(member), uin);
	wqq_group_member_set_group(member, group);
	group->members = g_list_append(group->members, member);
	return member;
}

WqqGroupMember *wqq_group_insert_group_member(WqqGroup * group,
											  guint64 uin, gint index)
{
	g_return_val_if_fail(WQQ_IS_GROUP(group), NULL);
	WqqGroupMember *member = wqq_group_member_new();
	wqq_user_set_uin(WQQ_USER(member), uin);
	wqq_group_member_set_group(member, group);
	group->members = g_list_insert(group->members, member, index);
	return member;
}

static gint _wqq_group_find_group_member_by_uin_func(gconstpointer a,
													 gconstpointer b)
{
	WqqGroupMember *member = (WqqGroupMember *) a;
	guint64 uin = *((guint64 *) b);
	return wqq_user_get_uin(WQQ_USER(member)) - uin;
}

WqqGroupMember *wqq_group_find_group_member_by_uin(WqqGroup * group,
												   guint64 uin)
{
	g_return_val_if_fail(WQQ_IS_GROUP(group), NULL);
	GList *lp = g_list_find_custom(group->members,
								   &uin,
								   _wqq_group_find_group_member_by_uin_func);
	if (lp == NULL)
		return NULL;
	return (WqqGroupMember *) lp->data;
}

void wqq_group_remove_group_member_by_uin(WqqGroup * group, guint64 uin)
{
	g_return_if_fail(WQQ_IS_GROUP(group));
	WqqGroupMember *member =
		wqq_group_find_group_member_by_uin(group, uin);
	if (member) {
		group->members = g_list_remove(group->members, member);
		g_object_unref(member);
	}
}

GList *wqq_group_get_mlist(WqqGroup * group)
{
	g_return_val_if_fail(WQQ_IS_GROUP(group), NULL);
	return group->members;
}
