/*  nick.c

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

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    if( !arg ) return notify_fail("��Ҫ���Լ�ȡʲô�ºũt\n");

    arg = color(arg);

    if( strlen(uncolor(arg)) > 14 )
	    return notify_fail("��Ĵº�̫���˩o��һ����һ��ġ�����һ��ġ�\n");

    seteuid(getuid());
    me->set("nickname", arg);    // use arg + NOR if colorful nick is open
    write("Ok��\n");
    return 1;
}

int help(object me)
{
    write(@HELP
ָ���ʽ : nick <���, �º�>
 
���ָ���������Ϊ�Լ�ȡһ�����������Ż�ͷ�Ρ�
�ºſ���ʹ�� ANSI ��ɫ�����룬������ help ansi
HELP);
        return 1;
}
