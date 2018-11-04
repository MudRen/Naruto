/*  order.c

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

int main(object me, string arg)
{
    int res;
    object ob;
    string who, cmd, verb, path;

    if( me != this_player(1) ) return 0;

    if( me->is_busy() ) return notify_fail("������û�пա�\n");

    if( !arg || arg=="" || sscanf(arg, "%s to %s", who, cmd)!=2 )
        return notify_fail("��Ҫ����˭��ʲô�©t\n");

    if( who == "_MY_PET_" ) {
    	if( !objectp(ob = me->query_temp("pet")) ) 
    		return notify_fail("���ƺ�û����Ȯ...\n");
    } else {
        ob = present(who, environment(me));
        if( !ob || !ob->is_character())
            return notify_fail("����û������ˡ�\n");
    }

    if( sscanf(cmd, "%s %*s", verb) != 2 ) verb = cmd;

    if( stringp(path = ob->find_command(verb)) && path != "/cmds/std/" + verb )
        return notify_fail("�㲻����������������¡�\n");

    if( wizardp(ob) && wiz_level(me) <= wiz_level(ob) )
        return notify_fail("�㲻��ָ������ˡ�\n");

    write("������" + ob->name() + ": " + cmd + "\n");
    if( (object)ob->query_owner() != me || !ob->accept_order(me, verb) )
        return notify_fail( "������" + ob->name() + "����������ָ�ӡ�\n");

    me->start_busy(2);

    notify_fail( ob->name() + "�޷����������\n");
    seteuid(getuid());
    res = ob->force_me(cmd);
    return res;
}

int help(object me)
{
    write(@TEXT
ָ���ʽ�sorder <ĳ��> to <ָ��>

��������ִ��ĳ��ָ�
���Ҫ�������ѵ���Ȯ����ֱ��ʹ�� _MY_PET_ ������Ҫʹ����
Ȯ��id��������ͬһ��̫����Ȯ���������ҡ�
����order _MY_PET_ to say hi
TEXT
    );
    return 1;
}
