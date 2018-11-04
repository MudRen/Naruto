/*  abandon.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <command.h>

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    SECURED_COMMAND;

    if( !arg ) return notify_fail("��Ҫ��������ѧϰʲô���ܣ�\n");

    if( sscanf(arg, "-c %s", arg)==1 )
        me->abandon_skill(arg, 1);
    else
        me->abandon_skill(arg);

    write("Ok.\n");
    return 1;
}

int help()
{
    write(@TEXT
ָ���ʽ��abandon <��������>

���ָ�����㡰��ֹ��ѧϰһ��ܣ�����������󲻻��پ���ʹ�û�ѧϰ������
�����Ⲣ����ʾ�����ȫ��������ܡ���ֹѧϰ�ļ�����Ȼ����ʹ�ã�ֻ��ֹͣ
ѧϰ���ѡ�

�������Ҫ��������� abandon -c <��������> ���ָ�������ֹ�ļ��ܡ�

�������ָ�enable��disable��skills
TEXT);
    return 1;
}
