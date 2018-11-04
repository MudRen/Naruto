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
	string msg = "��ĸ���ָ���ڵ������б���ʱ���Զ�������\n";
	if( arrayp(stat_name) && sizeof(stat_name) )
	    foreach(stat in stat_name)
	    {
		ratio = me->query_stat_notify(stat);
		if( ratio > 0 )
		    msg += sprintf("  %s\t%d%%\n", to_chinese(stat), ratio);
	    }
        else
            msg += "û���趨��\n";
        if( dir = me->query("wimpy_dir") )
            msg += "���Ԥ�����ܷ����� " + dir + "��\n";
        write(msg);
        return 1;
    }

    if( sscanf(arg, "%s at %d", stat, ratio) == 2 )
    {
	if( !me->query_stat_maximum(stat) )
            return notify_fail("��û������ָ����Ŀ��\n");

        if( (ratio < 0) || (ratio > 80) )
            return notify_fail("���������� 0 �� 80 ֮�䡣\n");

        me->set_stat_notify(stat, ratio);
        write("Ok��\n");

        return 1;
    }
    else if( sscanf(arg, "to %s", dir) ) {
        me->set("wimpy_dir", dir);
        write("Ok��\n");
        return 1;
    }
    else if( arg=="random" ) {
        me->delete("wimpy_dir");
        write("Ok.\n");
        return 1;
    }

    return notify_fail("ָ���ʽ�swimpy <ָ����Ŀ> at <����>\n");

}

int help()
{
    write(@TEXT
ָ���ʽ�swimpy <ָ����Ŀ> at <�ٷֱ�>
          wimpy to <����>
          wimpy random

���ָ�������趨�������ĳ��ָ������һ���ٷֱȵ�ʱ��o�Զ����ܡ�
(ÿ��ָ����Ŀ��������oֻҪ����һ����������İٷֱȩo�ͻ�����)

�����趨��ָ����Ŀ�����s

ap     ����ֵ
hp     ����ֵ
mp     �����

wimpy to <����> ��������ָ������ʱ������һ�������ܡ�
wimpy random ����ȡ��ԭ���ⶨ�����ܷ���
TEXT);
    return 1;
}
