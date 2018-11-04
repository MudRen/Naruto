/*  chfn.c

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

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    object link;

    SECURED_COMMAND;

    if( link = me->link() ) {
	write("���ĵ����ʼ������ַ [" + link->query("email") + "]: ");
	input_to("get_email", link);
	return 1;
    }
    return notify_fail("����ʹ���߲����޸ĸ������ϡ�\n");
}

void get_email(string new_email, object link)
{
    if( new_email != "" )
	link->set("email", new_email);

    write("�������ҽ���: (fingerʱ��ʾ)\n");
    link->body()->edit((: call_other, __FILE__, "get_profile", link :));
}

void get_profile(object link, string str)
{
    link->set("profile", str);
    write("����ǩ����ʽ: (postʱ��)\n");
    link->body()->edit((: call_other, __FILE__, "get_signature", link :));
}

void get_signature(object link, string str)
{
    if( strlen(str) > 400 )
	write("ǩ����ʽ̫���u�������趨��\n");
    else
	link->set("signature", str);
    write("��ǵ��� save �������������޸ġ�\n");
}

int help(object me)
{
    write(@HELP
ָ���ʽ : chfn

�����޸���ĸ������ϣ����������ʼ������ҽ��ܣ��Լ�ǩ����
HELP
    );
    return 1;
}
