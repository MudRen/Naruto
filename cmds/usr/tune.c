/*  tune.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string *tuned_ch;

    tuned_ch = me->link()->query("channels");
    if( !arg ) {
        if( !pointerp(tuned_ch) || !sizeof(tuned_ch) )
            write("�����ڲ�û�������κ�Ƶ����\n");
        else
            write("������������Ƶ���s" + implode(tuned_ch, ", ") + "��\n");
        return 1;
    }

    if( pointerp(tuned_ch) && member_array(arg, tuned_ch)!=-1 ) {
        write("�ر� " + arg + " Ƶ����\n");
        tuned_ch -= ({ arg });
        me->link()->set("channels", tuned_ch);
        return 1;
    } else {
        write("Ҫ��ĳ��Ƶ��ֻҪ�ø�Ƶ��˵�����ɡ�\n");
        return 1;
    }
}

int help(object me)
{
    write(@HELP
ָ���ʽ�stune [<Ƶ������>]

���ָ������ѡ���Ƿ�Ҫ����ĳһƵ����ѶϢ�o���û��ָ��Ƶ�����Ʃo�ͻ��г�
��Ŀǰ�����е�Ƶ���o���ָ����Ƶ���oԭ�������еľͻ�ص��o��֮�򿪡�

������һ��û�������е�Ƶ�������o���Զ������򿪡�
Ҫ��һ��Ƶ�������oֻҪ�és

<Ƶ������> <ѶϢ> ....

���өs
  chat hello everyone!
  
������� help channels ��ѯĿǰ����ЩƵ������������

see also : shout
HELP
    );
    return 1;
}

