/*  wimpy.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <statistic.h>

inherit F_CLEAN_UP;

void create() {seteuid(getuid());}

int main(object me, string arg)
{
    string stat, dir;
    int ratio;

    if( !arg )
    {
	string* stat_name = me->query_stat_name();
	string msg = "你的各项指数在低于下列比例时会自动逃命：\n";
	if( arrayp(stat_name) && sizeof(stat_name) )
	    foreach(stat in stat_name)
	    {
		ratio = me->query_stat_notify(stat);
		if( ratio > 0 )
		    msg += sprintf("  %s\t%d%%\n", to_chinese(stat), ratio);
	    }
        else
            msg += "没有设定。\n";
        if( dir = me->query("wimpy_dir") )
            msg += "你的预设逃跑方向是 " + dir + "。\n";
        write(msg);
        return 1;
    }

    if( sscanf(arg, "%s at %d", stat, ratio) == 2 )
    {
	if( !me->query_stat_maximum(stat) )
            return notify_fail("你没有这项指数项目。\n");

        if( (ratio < 0) || (ratio > 80) )
            return notify_fail("比例必须在 0 到 80 之间。\n");

        me->set_stat_notify(stat, ratio);
        write("Ok。\n");

        return 1;
    }
    else if( sscanf(arg, "to %s", dir) ) {
        me->set("wimpy_dir", dir);
        write("Ok。\n");
        return 1;
    }
    else if( arg=="random" ) {
        me->delete("wimpy_dir");
        write("Ok.\n");
        return 1;
    }

    return notify_fail("指令格式﹕wimpy <指数项目> at <比例>\n");

}

int help()
{
    write(@TEXT
指令格式﹕wimpy <指数项目> at <百分比>
          wimpy to <方向>
          wimpy random

这个指令让你设定在人物的某项指数低于一个百分比的时候﹐自动逃跑。
(每个指数项目都可以设﹐只要其中一个低于你设的百分比﹐就会逃跑)

可以设定的指数项目包括﹕

ap     精神值
hp     体力值
mp     查克拉

wimpy to <方向> 可以让你指定逃跑时先往哪一个方向跑。
wimpy random 可以取消原先这定的逃跑方向。
TEXT);
    return 1;
}
