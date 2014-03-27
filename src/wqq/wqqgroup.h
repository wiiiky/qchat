#ifndef __WQQ_GROUP_H__
#define __WQQ_GROUP_H__

#include <glib-object.h>
#include "wqqtypes.h"
#include "wqqgroupmember.h"

G_BEGIN_DECLS
#define WQQ_TYPE_GROUP	(wqq_group_get_type())
#define WQQ_GROUP(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_GROUP,WqqGroup))
#define WQQ_IS_GROUP(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_GROUP))
#define WQQ_GROUP_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WQQ_TYPE_GROUP,WqqGroupClass))
#define WQQ_IS_GROUP_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass),\
				WQQ_TYPE_GROUP))
#define WQQ_GROUP_GET_CLASS(obj)	(G_TYPE_GET_INSTANCE_CLASS((obj),\
				WQQ_TYPE_GROUP,WqqGroupClass))
typedef enum _WqqGroupMask WqqGroupMask;

struct _WqqGroup {
	GObject parent;
	guint64 gid;				/* 真实群号 */
	guint64 gin;				/* 变换后的群号 */
	guint64 code;
	guint klass;
	guint64 flag;
	guint level;
	guint mask;
	gchar *name;
	gchar *mark;
	gchar *memo;
	gchar *fingermemo;
	guint64 ct;					/* create time */
	guint64 owner;				/* 群主 */
	GList *members;
	void *data;					/* 自定义数据 */
};

struct _WqqGroupClass {
	GObjectClass parent_class;
};

enum _WqqGroupMask {
	WQQ_GROUP_MASK_OK = 0,
	WQQ_GROUP_MASK_NO_NOTIFY,
	WQQ_GROUP_MASK_BAN,
};
GType wqq_group_mask_get_type(void) G_GNUC_CONST;
#define WQQ_TYPE_GROUP_MASK	(wqq_group_mask_get_type())

GType wqq_group_get_type() G_GNUC_CONST;

WqqGroup *wqq_group_new(guint64 gid, const gchar * name);
guint64 wqq_group_get_gid(WqqGroup * group);
guint64 wqq_group_get_gin(WqqGroup * group);
guint wqq_group_get_level(WqqGroup * group);
const gchar *wqq_group_get_name(WqqGroup * group);
const gchar *wqq_group_get_mark(WqqGroup * group);
guint64 wqq_group_get_owner(WqqGroup * group);
GList *wqq_group_get_members(WqqGroup * group);
guint64 wqq_group_get_code(WqqGroup * group);
guint64 wqq_group_get_flag(WqqGroup * group);
guint wqq_group_get_mask(WqqGroup * group);
guint64 wqq_group_get_createtime(WqqGroup * group);
void wqq_group_set_createtime(WqqGroup * group, guint64 createtime);
void wqq_group_set_mask(WqqGroup * group, WqqGroupMask mask);
void wqq_group_set_flag(WqqGroup * group, guint64 flag);
void wqq_group_set_code(WqqGroup * group, guint64 code);
void wqq_group_set_gid(WqqGroup * group, guint64 gid);
void wqq_group_set_gin(WqqGroup * group, guint64 gin);
void wqq_group_set_level(WqqGroup * group, guint level);
void wqq_group_set_name(WqqGroup * group, const gchar * name);
void wqq_group_set_mark(WqqGroup * group, const gchar * mark);
void wqq_group_set_owner(WqqGroup * group, guint64 owner);

/* 群成员相关 */
WqqGroupMember *wqq_group_append_group_member(WqqGroup * group,
											  guint64 uin);
WqqGroupMember *wqq_group_insert_group_member(WqqGroup * group,
											  guint64 uin, gint index);
WqqGroupMember *wqq_group_find_group_member_by_uin(WqqGroup * group,
												   guint64 uin);
void wqq_group_remove_group_member_by_uin(WqqGroup * group, guint64 uin);
GList *wqq_group_get_mlist(WqqGroup * group);

G_END_DECLS
#endif
