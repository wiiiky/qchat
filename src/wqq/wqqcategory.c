#include "wqqcategory.h"
#include "wqqfriend.h"

enum {
	WQQ_CATEGORY_PROPERTY_INDEX = 1,
	WQQ_CATEGORY_PROPERTY_SORT,
	WQQ_CATEGORY_PROPERTY_NAME,
	WQQ_CATEGORY_PROPERTY_MEMBERS
};

G_DEFINE_TYPE(WqqCategory, wqq_category, G_TYPE_OBJECT);

static void wqq_category_getter(GObject * object, guint property_id,
								GValue * value, GParamSpec * pspec);
static void wqq_category_setter(GObject * object, guint property_id,
								const GValue * value, GParamSpec * pspec);

static void wqq_category_init(WqqCategory * category)
{
	category->index = 0;
	category->sort = 0;
	category->name = NULL;
	category->members = NULL;
}

static void wqq_category_finalize(GObject * object)
{
	WqqCategory *category = WQQ_CATEGORY(object);
	g_free(category->name);
	/*g_free(category->members); */
	g_list_free_full(category->members, (GDestroyNotify) g_object_unref);
}

static void wqq_category_class_init(WqqCategoryClass * klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = wqq_category_setter;
	object_class->get_property = wqq_category_getter;
	object_class->finalize = wqq_category_finalize;

	GParamSpec *ps = g_param_spec_uint("index",
									   "category index",
									   "Category Index",
									   0, G_MAXUINT,
									   0,
									   G_PARAM_READABLE |
									   G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_CATEGORY_PROPERTY_INDEX, ps);

	ps = g_param_spec_uint("sort",
						   "category sort",
						   "Category Sort",
						   0, G_MAXUINT,
						   0, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_CATEGORY_PROPERTY_SORT, ps);

	ps = g_param_spec_string("name",
							 "category name",
							 "Category Name",
							 NULL, G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_CATEGORY_PROPERTY_NAME, ps);

	ps = g_param_spec_pointer("members",
							  "category members",
							  "Category Members",
							  G_PARAM_READABLE | G_PARAM_WRITABLE);
	g_object_class_install_property(object_class,
									WQQ_CATEGORY_PROPERTY_MEMBERS, ps);
}


static void wqq_category_getter(GObject * object, guint property_id,
								GValue * value, GParamSpec * pspec)
{
	WqqCategory *category = WQQ_CATEGORY(object);
	switch (property_id) {
	case WQQ_CATEGORY_PROPERTY_INDEX:
		g_value_set_uint(value, category->index);
		break;
	case WQQ_CATEGORY_PROPERTY_SORT:
		g_value_set_uint(value, category->sort);
		break;
	case WQQ_CATEGORY_PROPERTY_NAME:
		g_value_set_string(value, category->name);
		break;
	case WQQ_CATEGORY_PROPERTY_MEMBERS:
		g_value_set_pointer(value, category->members);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

static void wqq_category_setter(GObject * object, guint property_id,
								const GValue * value, GParamSpec * pspec)
{
	WqqCategory *category = WQQ_CATEGORY(object);
	switch (property_id) {
	case WQQ_CATEGORY_PROPERTY_INDEX:
		category->index = g_value_get_uint(value);
		break;
	case WQQ_CATEGORY_PROPERTY_SORT:
		category->sort = g_value_get_uint(value);
		break;
	case WQQ_CATEGORY_PROPERTY_NAME:
		category->name = g_strdup(g_value_get_string(value));
		break;
	case WQQ_CATEGORY_PROPERTY_MEMBERS:
		category->members = (GList *) g_value_get_pointer(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
		break;
	}
}

/****************************************************
 * PUBLIC
 *****************************************************/
WqqCategory *wqq_category_new(const gchar * name, guint index, guint sort)
{
	return (WqqCategory *) g_object_new(WQQ_TYPE_CATEGORY,
										"name", name,
										"index", index,
										"sort", sort, NULL);
}

void wqq_category_set_name(WqqCategory * category, const gchar * name)
{
	g_return_if_fail(WQQ_IS_CATEGORY(category));
	g_free(category->name);
	category->name = g_strdup(name);
}

void wqq_category_set_index(WqqCategory * category, guint index)
{
	g_return_if_fail(WQQ_IS_CATEGORY(category));
	category->index = index;
}

void wqq_category_set_sort(WqqCategory * category, guint sort)
{
	g_return_if_fail(WQQ_IS_CATEGORY(category));
	category->sort = sort;
}

const gchar *wqq_category_get_name(WqqCategory * category)
{
	g_return_val_if_fail(WQQ_IS_CATEGORY(category), NULL);
	return category->name;
}

guint wqq_category_get_index(WqqCategory * category)
{
	g_return_val_if_fail(WQQ_IS_CATEGORY(category), 0);
	return category->index;
}

guint wqq_category_get_sort(WqqCategory * category)
{
	g_return_val_if_fail(WQQ_IS_CATEGORY(category), 0);
	return category->sort;
}

static gint _wqq_user_status_to_weight(WqqUserStatus status)
{
	switch (status) {
	case WQQ_USER_STATUS_ONLINE:
		return 9;
		break;
	case WQQ_USER_STATUS_CALLME:
		return 10;
		break;
	case WQQ_USER_STATUS_BUSY:
		return 8;
		break;
	case WQQ_USER_STATUS_AWAY:
		return 7;
		break;
	case WQQ_USER_STATUS_SILENT:
		return 6;
		break;
	case WQQ_USER_STATUS_HIDDEN:
		return 5;
		break;
	default:
		return 4;
	}
	return 0;
}

static gint _wqq_category_sort_friends_func(gconstpointer a,
											gconstpointer b)
{
	const WqqFriend *fa = (WqqFriend *) a;
	const WqqFriend *fb = (WqqFriend *) b;
	WqqUserStatus sa = wqq_user_get_status(WQQ_USER(fa));
	WqqUserStatus sb = wqq_user_get_status(WQQ_USER(fb));
	gint wa = _wqq_user_status_to_weight(sa);
	gint wb = _wqq_user_status_to_weight(sb);
	return wa - wb;
}

void wqq_category_sort_friends(WqqCategory * category)
{
	g_return_if_fail(WQQ_IS_CATEGORY(category));
	category->members = g_list_sort(category->members,
									_wqq_category_sort_friends_func);
}


guint wqq_category_get_length(WqqCategory * category)
{
	g_return_val_if_fail(WQQ_IS_CATEGORY(category), 0);
	return g_list_length(category->members);
}

GList *wqq_category_get_friends_list(WqqCategory * category)
{
	g_return_val_if_fail(WQQ_IS_CATEGORY(category), NULL);
	return category->members;
}

/* 在链表中添加好友 */
WqqFriend *wqq_category_append_friend(WqqCategory * category)
{
	g_return_val_if_fail(WQQ_IS_CATEGORY(category), NULL);
	WqqFriend *fri = wqq_friend_new();
	wqq_friend_set_category(fri, category);
	category->members = g_list_append(category->members, fri);
	return fri;
}

WqqFriend *wqq_category_insert_friend(WqqCategory * category, guint index)
{
	g_return_val_if_fail(WQQ_IS_CATEGORY(category), NULL);
	WqqFriend *fri = wqq_friend_new();
	wqq_friend_set_category(fri, category);
	category->members = g_list_insert(category->members, fri, index);
	return fri;
}

/* 移除指定的好友 */
void wqq_category_remove_friend(WqqCategory * category, WqqFriend * fri)
{
	g_return_val_if_fail(WQQ_IS_CATEGORY(category) &&
						 WQQ_IS_FRIEND(fri), FALSE);
	category->members = g_list_remove_all(category->members, fri);
	g_object_unref(fri);
}

/* 查找好友 */
static gint _find_by_nick(gconstpointer a, gconstpointer b)
{
	WqqFriend *fri = (WqqFriend *) a;
	const gchar *nick = (const gchar *) b;
	return g_strcmp0(wqq_user_get_nick(WQQ_USER(fri)), nick);
}

WqqFriend *wqq_category_find_friend_by_nick(WqqCategory * category,
											const gchar * nick)
{
	return wqq_category_find_friend_by_custom(category, nick,
											  _find_by_nick);
}

static gint _find_by_uin(gconstpointer a, gconstpointer b)
{
	WqqFriend *fri = (WqqFriend *) a;
	guint64 uin = *(guint64 *) b;
	if (uin == wqq_user_get_uin(WQQ_USER(fri)))
		return 0;
	return -1;
}

WqqFriend *wqq_category_find_friend_by_uin(WqqCategory * category,
										   guint64 uin)
{
	return wqq_category_find_friend_by_custom(category, &uin,
											  _find_by_uin);
}

static gint _find_by_qq(gconstpointer a, gconstpointer b)
{
	WqqFriend *fri = (WqqFriend *) a;
	guint64 qq = *(guint64 *) b;
	if (qq == wqq_user_get_qq(WQQ_USER(fri)))
		return 0;
	return 0;
}

WqqFriend *wqq_category_find_friend_by_qq(WqqCategory * category,
										  guint64 qq)
{
	return wqq_category_find_friend_by_custom(category, &qq, _find_by_qq);
}

static gint _find_by_markname(gconstpointer a, gconstpointer b)
{
	WqqFriend *fri = (WqqFriend *) a;
	const gchar *markanme = (const gchar *) b;
	return g_strcmp0(wqq_friend_get_markname(fri), markanme);
}

WqqFriend *wqq_category_find_friend_by_markname(WqqCategory * category,
												const gchar * markname)
{
	return wqq_category_find_friend_by_custom(category, markname,
											  _find_by_markname);
}

WqqFriend *wqq_category_find_friend_by_custom(WqqCategory * category,
											  gconstpointer data,
											  GCompareFunc func)
{
	g_return_val_if_fail(WQQ_IS_CATEGORY(category), NULL);
	GList *lp = g_list_find_custom(category->members, data, func);
	if (lp)
		return lp->data;
	return NULL;
}

void wqq_category_set_data(WqqCategory * category, gpointer data)
{
	g_return_if_fail(WQQ_IS_CATEGORY(category));
	category->data = data;
}

gpointer wqq_category_get_data(WqqCategory * category)
{
	g_return_val_if_fail(WQQ_IS_CATEGORY(category), NULL);
	return category->data;
}
