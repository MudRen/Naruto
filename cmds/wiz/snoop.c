/*  snoop.c

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

int main(object me, string arg)
{
    object ob;

    SECURED_WIZARD_COMMAND;

    if( !arg ) {
        if( objectp(ob = query_snooping(me)) )
            write("���������ڼ���" + ob->query("name") + "���յ���ѶϢ��\n");
        return 1;
    } else if( arg=="none" ) {
        if( objectp(ob = query_snooping(me))
        &&    wiz_level(ob) >= wiz_level(me) )
            tell_object(ob, HIW + me->name(1) + "ֹͣ���������յ���ѶϢ��\n" NOR);
        snoop(me);
        write("Ok.\n");
        return 1;
    }

    ob = find_player(arg);
    if(!ob) ob = find_living(arg);
    if(!ob || !ob->visible(me)) return notify_fail("û������ˡ�\n");

    if( wizhood(me) != "(admin)"
    &&    wiz_level(me) <= wiz_level(ob) || wiz_level(me) < 3)
        return notify_fail("��û�м���" + ob->name() + "������ѶϢ��Ȩ����\n");

    if( me==ob ) return notify_fail("���� snoop none ���������\n");
        
    snoop(me, ob);
    write("�����ڿ�ʼ����" + ob->name(1) + "���յ���ѶϢ��\n");
    if( userp(ob) ) {
	log_file("SNOOP_PLAYER",
	    sprintf("[%s] %s snoops %O\n", ctime(time()), geteuid(me), ob));
	if( wizardp(ob) && wiz_level(me) < wiz_level("(arch)") )
	    tell_object(ob, HIW + me->name(1) + "��ʼ���������յ���ѶϢ��\n" NOR);
    }

    return 1;
}

int help()
{
    write(@TEXT
ָ���ʽ�ssnoop <ĳ��>|none

��������ʹ������������ѶϢ�osnoop none ��ȡ��������
TEXT
    );
    return 1;
}
