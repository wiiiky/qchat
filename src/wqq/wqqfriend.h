#ifndef __WQQ_FRIEND_H__
#define __WQQ_FRIEND_H__

/*
 * Wiky L(wiiiky@yeah.net)
 * 2014-1-2
 * qq好友
 */

#include "wqquser.h"
#include "wqqtypes.h"
#include "wqqcategory.h"

G_BEGIN_DECLS
#define WQQ_TYPE_FRIEND	(wqq_friend_get_type())
#define WQQ_FRIEND(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
					WQQ_TYPE_FRIEND,WqqFriend))
#define WQQ_IS_FRIEND(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_FRIEND))
#define WQQ_FRIEND_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WQQ_TYPE_FRIEND,WqqFriendClass))
#define WQQ_IS_FRIEND_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass),\
				WQQ_TYPE_FRIEND))
#define WQQ_FRIEND_GET_CLASS(obj)	(G_TYPE_GET_INSTANCE_CLASS((obj),\
				WQQ_TYPE_FRIEND,WqqFriendClass))
	struct _WqqFriend {
	WqqUser parent;

	gchar *markname;			/* 备注 */
	WqqCategory *category;		/* 指向所在分组的指针 */
	gpointer data;				/* 附加指针 */
};

struct _WqqFriendClass {
	WqqUserClass parent_class;
};

GType wqq_friend_get_type() G_GNUC_CONST;

WqqFriend *wqq_friend_new(void);
void wqq_friend_set_markanme(WqqFriend * fri, const gchar * markname);
void wqq_friend_set_category(WqqFriend * fri, WqqCategory * category);
const gchar *wqq_friend_get_markname(WqqFriend * fri);
WqqCategory *wqq_friend_get_category(WqqFriend * fri);
void wqq_friend_set_data(WqqFriend * fri, gpointer data);
gpointer wqq_friend_get_data(WqqFriend * fri);

G_END_DECLS
#endif
