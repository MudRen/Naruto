/*  save.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <login.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object link_ob;
    int last_save, delta_t;
    
    seteuid(getuid());

    if( !objectp(link_ob = me->link()) )
        return notify_fail("你不是经由正常连线进入﹐不能储存。\n");

    if( !environment(me) ) 
    	return notify_fail("你不知道你自已在哪里，无法储存。\n");

    if( environment(me)->query("recoverroom") ) {
        me->set("recoverroom", base_name(environment(me)));
        write("你的灵魂将会在此重生。\n");
    }

    if( !wizardp(me) && (last_save = me->query_temp("last_save")) && (delta_t = (time()/60 - last_save)) < 5 )
        return notify_fail("不用急, " +delta_t+ " 分钟前你才刚储存过, " +(5 - delta_t)+ " 分钟后再试吧。\n");

    me->set("startroom", base_name(environment(me)));

#ifdef SAVE_USER
    if( (int)me->save() ) {
        write("档案储存完毕。\n");
        me->set_temp("last_save", (int)(time()/60) );
        return 1;
    } else {
        write("储存失败。\n");
        return 0;
    }
#else
    return notify_fail("系统目前不储存使用者资料。\n");
#endif
}

int help(object me)
{
    write(@HELP
指令格式﹕save

把你辛苦奋斗的结果存起来。为了提升系统效率, 目前两次 save 之时间间距最短为
五分钟。 quit 时系统会试著帮你 save。
see also: quit, backup
HELP
    );
    return 1;
}
