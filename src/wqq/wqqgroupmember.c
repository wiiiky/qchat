#include "wqqgroupmember.h"

enum {
	WQQ_GROUP_MEMBER_PROPERTY_CARD = 1,
	WQQ_GROUP_MEMBER_PROPERTY_FLAG,
	WQQ_GROUP_MEMBER_PROPERTY_GROUP,
};

G_DEFINE_TYPE(WqqGroupMember, wqq_group_member, WQQ_TYPE_USER);

static void wqq_group_member_getter(GObject * object, guint property_id,
									GValue * value, GParamSpec * pspec);
static void wqq_group_member_setter(GObject * object, guint property_id,
									const GValue * value,
									GParamSpec * psepc);

static void wqq_group_member_init(WqqGroupMember * member)
{
	member->card = NULL;
	member->flag = 0;
	member->group = NULL;
}

static void wqq_group_member_finalize(GObject * object)
{
	WqqGroupMember *member = WQQ_GROUP_MEMBER(object);
	g_free(member->card);
}

static void wqq_group_member_class_init(WqqGroupMemberClass * klass)
{
	GObjectClass *obj_class = G_OBJECT_CLASS(klass);
	obj_class->set_property = wqq_group_member_setter;
	obj_class->get_property = wqq_group_member_getter;
	obj_class->finalize = wqq_group_member_finalize;

	GParamSpec *ps;
	ps = g_param_spec_string("card",
							 "group member card",
							 "Group Member Card",
							 NULL, G_PARAM_WRITABLE | G_PARAM_READABLE);
	g_object_class_install_property(obj_class,
									WQQ_GROUP_MEMBER_PROPERTY_CARD, ps);

	ps = g_param_spec_uint64("flag",
							 "group member flag",
							 "Group Member Flag",
							 0, G_MAXUINT64,
							 0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(obj_class,
									WQQ_GROUP_MEMBER_PROPERTY_FLAG, ps);

	ps = g_param_spec_pointer("group",
							  "referenced group",
							  "Referenced Group",
							  G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(obj_class,
									WQQ_GROUP_MEMBER_PROPERTY_GROUP, ps);
}

static void wqq_group_member_getter(GObject * object, guint property_id,
									GValue * value, GParamSpec * pspec)
{
	WqqGroupMember *member = WQQ_GROUP_MEMBER(object);
	switch (property_id) {
	case WQQ_GROUP_MEMBER_PROPERTY_CARD:
		g_value_set_string(value, member->card);
		break;
	case WQQ_GROUP_MEMBER_PROPERTY_GROUP:
		g_value_set_pointer(value, member->group);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void wqq_group_member_setter(GObject * object, guint property_id,
									const GValue * value,
									GParamSpec * pspec)
{
	WqqGroupMember *member = WQQ_GROUP_MEMBER(object);
	switch (property_id) {
	case WQQ_GROUP_MEMBER_PROPERTY_CARD:
		g_free(member->card);
		member->card = g_strdup(g_value_get_string(value));
		break;
	case WQQ_GROUP_MEMBER_PROPERTY_GROUP:
		member->group = g_value_get_pointer(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

WqqGroupMember *wqq_group_member_new()
{
	return (WqqGroupMember *) g_object_new(WQQ_TYPE_GROUP_MEMBER, NULL);
}

const gchar *wqq_group_member_get_card(WqqGroupMember * member)
{
	g_return_val_if_fail(WQQ_IS_GROUP_MEMBER(member), NULL);
	return member->card;
}

void wqq_group_member_set_card(WqqGroupMember * member, const gchar * card)
{
	g_return_if_fail(WQQ_IS_GROUP_MEMBER(member));
	g_free(member->card);
	member->card = g_strdup(card);
}

WqqGroup *wqq_group_member_get_group(WqqGroupMember * member)
{
	g_return_val_if_fail(WQQ_IS_GROUP_MEMBER(member), NULL);
	return member->group;
}

void wqq_group_member_set_group(WqqGroupMember * member, WqqGroup * group)
{
	g_return_if_fail(WQQ_IS_GROUP_MEMBER(member));
	member->group = group;
}

guint64 wqq_group_member_get_flag(WqqGroupMember * member)
{
	g_return_val_if_fail(WQQ_IS_GROUP_MEMBER(member), 0);
	return member->flag;
}

void wqq_group_member_set_flag(WqqGroupMember * member, guint64 flag)
{
	g_return_if_fail(WQQ_IS_GROUP_MEMBER(member));
	member->flag = flag;
}
