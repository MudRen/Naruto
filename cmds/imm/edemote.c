/*  edemote.c

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

int in_use;

int main(object me, string arg)
{
    mapping emote;

    SECURED_COMMAND;

    if( !arg ) return notify_fail("��Ҫ�༭ʲô emote�t\n");

    if( sscanf(arg, "-d %s", arg) ) {
        write("ɾ�� emote�s" + arg + "\n");
        EMOTE_D->delete_emote(arg);
        return 1;
    }

    if( sscanf(arg, "-p %s", arg) ) {
        emote = EMOTE_D->query_emote(arg);
        printf("�ϴ��޸ĩs%s\n", emote["updated"]);
        printf("��������������������������\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
            emote["myself"], emote["others"], emote["myself_self"],
            emote["others_self"], emote["myself_target"], emote["target"],
            emote["others_target"] );
        return 1;
    }

    emote = EMOTE_D->query_emote(arg);
    emote = (["updated":geteuid(me)]);

    write("�༭ emote�s" + arg + "\n");
    write("ѶϢ�����кü��Щo�� . ��ʾ������\n");
    write("ѶϢ�п�ʹ�õĲ��������¼��֩s\n");
    write("  $N  �Լ������֡�\n");
    write("  $n  ʹ�ö�������֡�\n");
    write("  $P  �Լ����˳ƴ����ʩo���㡢�㡢����������������\n");
    write("  $p  ʹ�ö�����˳ƴ����ʩo���㡢�㡢����������������\n");
    write("������������������������������������������������������������������������\n");
    write("��ָ������ʹ����� emote ʱ�o���Լ�������ѶϢ�s\n->");
    input_to("get_msg_myself", emote, arg);
    return 1;
}


private int
get_msg_myself(string msg, mapping emote, string pattern)
{
    if (msg==".") {
        if( !undefinedp(emote["myself"]) ) emote["myself"] += "\n";
        write("��ָ������ʹ����� emote ʱ�o�����˿�����ѶϢ�s\n->");
        input_to("get_msg_others", emote, pattern);
        return 1;
    }
    if( !undefinedp(emote["myself"]) )
        emote["myself"] += msg + "\n";
    else emote["myself"] = msg;
    write("->");
    input_to("get_msg_myself", emote, pattern);
    return 1;
}

private int
get_msg_others(string msg, mapping emote, string pattern)
{
    if (msg==".") {
        if( !undefinedp(emote["others"]) ) emote["others"] += "\n";
        write("���Լ�ʹ����� emote ʱ�o�Լ�������ѶϢ�s\n->");
        input_to("get_msg_myself_self", emote, pattern);
        return 1;
    }
    if( !undefinedp(emote["others"]) )
        emote["others"] += msg + "\n";
    else emote["others"] = msg;
    write("->");
    input_to("get_msg_others", emote, pattern);
    return 1;
}

private int
get_msg_myself_self(string msg, mapping emote, string pattern)
{
    if (msg==".") {
        if( !undefinedp(emote["myself_self"]) ) emote["myself_self"] += "\n";
        write("���Լ�ʹ����� emote ʱ�o�����˿�����ѶϢ�s\n->");
        input_to("get_msg_others_self", emote, pattern);
        return 1;
    }
    if( !undefinedp(emote["myself_self"]) )
        emote["myself_self"] += msg + "\n";
    else emote["myself_self"] = msg;
    write("->");
    input_to("get_msg_myself_self", emote, pattern);
    return 1;
}

private int
get_msg_others_self(string msg, mapping emote, string pattern)
{
    if (msg==".") {
        if( !undefinedp(emote["others_self"]) ) emote["others_self"] += "\n";
        write("�Ա���ʹ����� emote ʱ�o�Լ�������ѶϢ�s\n->");
        input_to("get_msg_myself_target", emote, pattern);
        return 1;
    }
    if( !undefinedp(emote["others_self"]) )
        emote["others_self"] += msg + "\n";
    else emote["others_self"] = msg;
    write("->");
    input_to("get_msg_others_self", emote, pattern);
    return 1;
}

private int
get_msg_myself_target(string msg, mapping emote, string pattern)
{
    if (msg==".") {
        if( !undefinedp(emote["myself_target"]) ) emote["myself_target"] += "\n";
        write("�Ա���ʹ����� emote ʱ�oʹ�ö��󿴵���ѶϢ�s\n->");
        input_to("get_msg_target", emote, pattern);
        return 1;
    }
    if( !undefinedp(emote["myself_target"]) )
        emote["myself_target"] += msg + "\n";
    else emote["myself_target"] = msg;
    write("->");
    input_to("get_msg_myself_target", emote, pattern);
    return 1;
}

private int
get_msg_target(string msg, mapping emote, string pattern)
{
    if (msg==".") {
        if( !undefinedp(emote["target"]) ) emote["target"] += "\n";
        write("�Ա���ʹ����� emote ʱ�o�����Լ���ʹ�ö�����o�����˿�����ѶϢ�s\n->");
        input_to("get_msg_others_target", emote, pattern);
        return 1;
    }
    if( !undefinedp(emote["target"]) )
        emote["target"] += msg + "\n";
    else emote["target"] = msg;
    write("->");
    input_to("get_msg_target", emote, pattern);
    return 1;
}

private int
get_msg_others_target(string msg, mapping emote, string pattern)
{
    if (msg==".") {
        if( !undefinedp(emote["others_target"]) ) emote["others_target"] += "\n";
        EMOTE_D->set_emote(pattern, emote);
        write("Emote �༭������\n");
        return 1;
    }
    if( !undefinedp(emote["others_target"]) )
        emote["others_target"] += msg + "\n";
    else emote["others_target"] = msg;
    write("->");
    input_to("get_msg_others_target", emote, pattern);
    return 1;
}

int help(object me)
{
    write(@HELP
ָ���ʽ��edemote [-d|-p] <emote>
 
���ָ������޸�, ɾ�� emote ���г�������. ���� -d ������ɾ��
ָ���� emote, -p ��������г�ָ�� emote ������. �г���˳�����
�� emote ʱ��ͬ.
 
���� emote ѶϢʱ��������Ŀ: û��Ŀ��, ָ��Ŀ����Ƕ��Լ�. ��
������ĳ��ѶϢ, ��ֱ���ڿհ������� '.' ����.
 
һ�� emote ѶϢ�����кܶ���, �ڿհ������� '.' ����������� emote.
 
�༭ emote ʱ���������µķ�������ʾ:
 
$N : �Լ�������.
$n : Ŀ�������.
$P : �Լ����˳ƴ�����.
$p : Ŀ����˳ƴ�����.
 
HELP
    );
    return 1;
}
