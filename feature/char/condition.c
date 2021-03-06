/*  condition.c - character condition interface

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <condition.h>

mapping conditions = ([]);

/* update_condition()
 *
 * 这个函数会被人物的 heart_beat 在每次 tick 时呼叫一次，负责更新人物
 * 所有的长效状态。
 */

nomask void update_condition()
{
    string cnd;
    mixed cnd_data;

    if( !mapp(conditions) || !sizeof(conditions) ) return;

    foreach(cnd, cnd_data in conditions)
	CONDITION_D(cnd)->condition_update(this_object(), cnd, cnd_data);
}

/* restore_condition()
 *
 * 这个函数用来在一个人物从档案取回长效状态的资料之后，恢复长效状态的
 * 影响。原则上这个函数应该只有人物自己才会需要呼叫，所以宣告成 static。
 */

static nomask void restore_condition()
{
    string cnd;
    mixed cnd_data;

    if( !mapp(conditions) || !sizeof(conditions) ) return;

    foreach(cnd, cnd_data in conditions)
	CONDITION_D(cnd)->condition_apply(this_object(), cnd, cnd_data);
}

nomask void clear_condition()
{
    string cnd;
    mixed cnd_data;

    if( !mapp(conditions) || !sizeof(conditions) ) return;

    foreach(cnd, cnd_data in conditions)
	CONDITION_D(cnd)->condition_unapply(this_object(), cnd);

    conditions = ([]);
}

nomask void set_condition(string cnd, mixed info)
{
    if( !mapp(conditions) ) conditions = ([]);

    /* 呼叫长效状态 daemon 解除(unapply) 原来的同一长效状态 */
    if( conditions[cnd] )
	    CONDITION_D(cnd)->condition_unapply(this_object(), cnd, conditions[cnd]);

    conditions[cnd] = info;
    CONDITION_D(cnd)->condition_apply(this_object(), cnd, info);
}

nomask mapping query_all_conditions()
{
	return conditions;
}

nomask mixed query_condition(string cnd)
{
    return mapp(conditions) ? conditions[cnd] : 0;
}

nomask void delete_condition(string cnd)
{
    if( !mapp(conditions) || undefinedp(conditions[cnd]) ) return;

    CONDITION_D(cnd)->condition_unapply(this_object(), cnd);
    map_delete(conditions, cnd);
}

 