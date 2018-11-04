/*  enable.c - enable a special skill with another skill

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <ansi.h>
#include <command.h>

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    mapping map;
    string *types, *skill, ski, map_to;
    int i, modify;

    SECURED_COMMAND;

    if( !arg ) {
        map = me->query_skill_map();
        if( !mapp(map) ) map = ([]);

        write("��������Ŀǰʹ���е����⼼�ܡ�\n");
        foreach( skill, map_to in map ) {
            printf("%-20s��%s\n", to_chinese(skill),
                map_to ? to_chinese(map_to) : "��ʹ��");
        }
        return 1;
    }

    if( sscanf(arg, "%s with %s", ski, map_to)==2 ) {

//        if( !me->query_skill(map_to, 1) && !me->query_learn(map_to) )
        if( !me->query_skill(map_to, 1)  )
            return notify_fail("�㲻�����ּ��ܡ�\n");

        if( !function_exists("valid_enable", SKILL_D(map_to)) )
            return notify_fail( "��" + to_chinese(map_to) + "�����ܺ��������ܺ��á�\n");

        notify_fail("������ܲ��ܵ���������;��\n");
        if( !SKILL_D(map_to)->valid_enable(ski) )
            return 0;

        me->map_skill(map_to, map_to);
        me->map_skill(ski, map_to);

        write("�����ʹ�á�" + to_chinese(map_to) + "������" + to_chinese(ski) + "��\n");
        return 1;
    }

//    if( !me->query_learn(arg) )
    if( !me->query_skill(arg, 1) )
        return notify_fail("��û��ѧ�����ּ��ܡ�\n");

    if( me->skill_mapped(arg)==arg ) {
        write("�����ʵ��ĳ��ϻ��Զ�ʹ��������ܡ�\n");
        return 1;
    }

    me->map_skill(arg);
    write("������ָ�ʹ����ѧ�йء�" + to_chinese(arg) + "���ļ��ɡ�\n");

    return 1;
}
int help(object me)
{
    write(@HELP
ָ���ʽ : enable <��������> with <��������>   <- ʹ�����⼼�ܡ�����
           enable <��������>                   <- ֻ�û�������

���ָ������ָ����Ҫ�õļ��ܩo��ָ����������ͼ������ơ�������Ӳ������
��ʾ���������༰��Ŀǰ��ʹ�õļ������ơ�

�������ָ�disable��skills��abandon
HELP);
    return 1;
}
