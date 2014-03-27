#include "wqqfriend.h"

enum {
	WQQ_FRIEND_PROPERTY_MARKNAME = 1,
	WQQ_FRIEND_PROPERTY_CATEGORY,
};

G_DEFINE_TYPE(WqqFriend, wqq_friend, WQQ_TYPE_USER);

static void wqq_friend_getter(GObject * object, guint property_id,
							  GValue * value, GParamSpec * pspec);
static void wqq_friend_setter(GObject * object, guint property_id,
							  const GValue * value, GParamSpec * pspec);
static void wqq_friend_finalize(GObject * object);

static void wqq_friend_init(WqqFriend * friend)
{
	friend->markname = NULL;
	friend->category = NULL;
}

static void wqq_friend_finalize(GObject * object)
{
	WqqFriend *friend = (WqqFriend *) object;
	g_free(friend->markname);
}

static void wqq_friend_class_init(WqqFriendClass * klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->get_property = wqq_friend_getter;
	object_class->set_property = wqq_friend_setter;
	object_class->finalize = wqq_friend_finalize;

	GParamSpec *ps;
	ps = g_param_spec_string("markname",
							 "friend markname",
							 "Friend Markname",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_FRIEND_PROPERTY_MARKNAME, ps);

	ps = g_param_spec_object("category",
							 "belong to category",
							 "Belong to Category",
							 WQQ_TYPE_CATEGORY,
							 G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_FRIEND_PROPERTY_CATEGORY, ps);
}

static void wqq_friend_getter(GObject * object, guint property_id,
							  GValue * value, GParamSpec * pspec)
{
	WqqFriend *friend = WQQ_FRIEND(object);
	switch (property_id) {
	case WQQ_FRIEND_PROPERTY_MARKNAME:
		g_value_set_string(value, friend->markname);
		break;
	case WQQ_FRIEND_PROPERTY_CATEGORY:
		g_value_set_object(value, friend->category);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void wqq_friend_setter(GObject * object, guint property_id,
							  const GValue * value, GParamSpec * pspec)
{
	WqqFriend *friend = WQQ_FRIEND(object);
	switch (property_id) {
	case WQQ_FRIEND_PROPERTY_MARKNAME:
		g_free(friend->markname);
		friend->markname = g_strdup(g_value_get_string(value));
		break;
	case WQQ_FRIEND_PROPERTY_CATEGORY:
		friend->category = g_value_get_object(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

/****************************************************
 * PUBLIC
 *************************************************/
WqqFriend *wqq_friend_new(void)
{
	return (WqqFriend *) g_object_new(WQQ_TYPE_FRIEND, NULL);
}

void wqq_friend_set_markanme(WqqFriend * fri, const gchar * markname)
{
	g_return_if_fail(WQQ_IS_FRIEND(fri));
	g_free(fri->markname);
	fri->markname = g_strdup(markname);
}

void wqq_friend_set_category(WqqFriend * fri, WqqCategory * category)
{
	g_return_if_fail(WQQ_IS_FRIEND(fri));
	fri->category = category;
}

const gchar *wqq_friend_get_markname(WqqFriend * fri)
{
	g_return_val_if_fail(WQQ_IS_FRIEND(fri), NULL);
	return fri->markname;
}

WqqCategory *wqq_friend_get_category(WqqFriend * fri)
{
	g_return_val_if_fail(WQQ_IS_FRIEND(fri), NULL);
	return fri->category;
}

void wqq_friend_set_data(WqqFriend * fri, gpointer data)
{
	g_return_if_fail(WQQ_IS_FRIEND(fri));
	fri->data = data;
}

gpointer wqq_friend_get_data(WqqFriend * fri)
{
	g_return_val_if_fail(WQQ_IS_FRIEND(fri), NULL);
	return fri->data;
}
