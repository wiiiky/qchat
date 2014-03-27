#ifndef __WQQ_GROUP_MEMBER_H__
#define __WQQ_GROUP_MEMBER_H__

#include "wqquser.h"
#include "wqqtypes.h"
#include "wqqgroup.h"

G_BEGIN_DECLS
#define WQQ_TYPE_GROUP_MEMBER (wqq_group_member_get_type())
#define WQQ_GROUP_MEMBER(obj)	(G_TYPE_CHECK_INSTANCE_CAST((obj),\
				WQQ_TYPE_GROUP_MEMBER,WqqGroupMember))
#define WQQ_IS_GROUP_MEMBER(obj)	(G_TYPE_CHECK_INSTANCE_TYPE((obj),\
				WQQ_TYPE_GROUP_MEMBER))
#define WQQ_GROUP_MEMBER_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST((klass),\
				WQQ_TYPE_GROUP_MEMBER,WqqGroupMemberClass))
#define WQQ_IS_GROUP_MEMBER_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE((klass),\
				WQQ_TYPE_GROUP_MEMBER))
#define WQQ_GROUP_MEMBER_GET_TYPE(obj)	(G_TYPE_GET_INSTANCE_CLASS((obj),\
				WQQ_TYPE_GROUP_MEMBER,WqqGroupMemberClass))
	struct _WqqGroupMember {
	WqqUser parent;
	gchar *card;				/* 群昵称 */
	guint64 flag;
	WqqGroup *group;			/* 指向所在群 */
};

struct _WqqGroupMemberClass {
	WqqUserClass parent_class;
};

GType wqq_group_member_get_type() G_GNUC_CONST;
WqqGroupMember *wqq_group_member_new();
const gchar *wqq_group_member_get_card(WqqGroupMember * member);
void wqq_group_member_set_card(WqqGroupMember * member,
							   const gchar * card);
WqqGroup *wqq_group_member_get_group(WqqGroupMember * member);
void wqq_group_member_set_group(WqqGroupMember * member, WqqGroup * group);
guint64 wqq_group_member_get_flag(WqqGroupMember * member);
void wqq_group_member_set_flag(WqqGroupMember * member, guint64 flag);

G_END_DECLS
#endif
