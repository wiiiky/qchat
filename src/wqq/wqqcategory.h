#ifndef __WQQ_CATEGORY_H__
#define __WQQ_CATEGORY_H__

/*
 * Wiky L (wiiiky@yeah.net)
 * 2014-1-2
 * qq分组
 */

#include "wqqfriend.h"

G_BEGIN_DECLS
#define WQQ_TYPE_CATEGORY	(wqq_category_get_type())
#define WQQ_CATEGORY(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_CATEGORY,WqqCategory))
#define WQQ_IS_CATEGORY(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_CATEGORY))
#define WQQ_CATEGORY_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WQQ_TYPE_CATEGORY,WqqCategoryClass))
#define WQQ_IS_CATEGORY_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass),\
				WQQ_TYPE_CATEGORY))
#define WQQ_CATEGORY_GET_CLASS(obj)	(G_TYPE_GET_INSTANCE_CLASS((obj),\
				WQQ_TYPE_CATEGORY,WqqCategoryClass))
	struct _WqqCategory {
	GObject parent;
	guint index;
	guint sort;
	gchar *name;
	GList *members;				/* 分组成员 */

	gpointer data;
};

struct _WqqCategoryClass {
	GObjectClass parent_klass;
};

GType wqq_category_get_type() G_GNUC_CONST;

WqqCategory *wqq_category_new(const gchar * name, guint index, guint sort);
void wqq_category_set_name(WqqCategory * category, const gchar * name);
void wqq_category_set_index(WqqCategory * category, guint index);
void wqq_category_set_sort(WqqCategory * category, guint sort);
const gchar *wqq_category_get_name(WqqCategory * category);
guint wqq_category_get_index(WqqCategory * category);
guint wqq_category_get_sort(WqqCategory * category);
void wqq_category_sort_friends(WqqCategory * category);

guint wqq_category_get_length(WqqCategory * category);
GList *wqq_category_get_friends_list(WqqCategory * category);

/* 在链表中添加好友 */
WqqFriend *wqq_category_append_friend(WqqCategory * category);
WqqFriend *wqq_category_insert_friend(WqqCategory * category, guint index);
/* 移除指定的好友 */
void wqq_category_remove_friend(WqqCategory * category, WqqFriend * fri);
/* 查找好友 */
WqqFriend *wqq_category_find_friend_by_nick(WqqCategory * category,
											const gchar * nick);
WqqFriend *wqq_category_find_friend_by_uin(WqqCategory * category,
										   guint64 uin);
WqqFriend *wqq_category_find_friend_by_qq(WqqCategory * category,
										  guint64 qq);
WqqFriend *wqq_category_find_friend_by_markname(WqqCategory * category,
												const gchar * markname);
WqqFriend *wqq_category_find_friend_by_custom(WqqCategory * category,
											  gconstpointer data,
											  GCompareFunc func);
void wqq_category_set_data(WqqCategory * category, gpointer data);
gpointer wqq_category_get_data(WqqCategory * category);

G_END_DECLS
#endif
