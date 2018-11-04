/*  to.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

// inherit F_CLEAN_UP;
// Don't want this command be cleaned up while typing long string.

int main(object me, string arg)
{
    me->edit( (: call_other, __FILE__, "do_to", me, arg :) );
    return 1;
}

void do_to(object me, string arg, string str)
{
    seteuid(getuid());
    if(arg) me->force_me(arg + " " + str);
    else me->force_me(str);
}

int help(object me)
{
write(@HELP
ָ���ʽ : to <ָ��>

����һ������������ϵ�����, <ָ��>ͨ�������ѶϢ��ָ����
say , tell , describe , chat , reple ........�ȡ�
���� : 
 to say
 �����뿪�� '.'�oȡ�������� '~q'�oʹ���ڽ��б༭���� '~e'��
 ����������������������������������������������������������
 ������̽��� : ����ϯ���ض�
 ����������߷��������� : Elon
 ����С���������㲿���� : ������
 ������ߴ�첿���� : ���Ѫ��
 .

�����өĻ�ϵĽ������ :

��˵���s������̽��� : ����ϯ���ض�
����������߷��������� : Elon
����С���������㲿���� : ������
������ߴ�첿���� : ���Ѫ��
HELP
    );
    return 1;
}

