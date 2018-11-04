/*  wield.c

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

varargs int do_wield(object me, object ob);

int main(object me, string arg)
{
    string skill;
    object ob, *inv;
    int i, count;

    if( me->is_busy() ) return notify_fail("������û�пթu\n");

    if( !arg ) return notify_fail("��Ҫװ��ʲô�����t\n");

    if(arg=="all") {
        inv = all_inventory(me);
        for(count=0, i=0; i<sizeof(inv); i++) {
            if( inv[i]->query("equipped") ) continue;
            if(do_wield(me, inv[i]) ) count ++;
        }
        write("Ok.\n");
        return 1;
    }

    // Get skill type if specified.
    sscanf(arg, "%s as %s", arg, skill);

    if( !objectp(ob = present(arg, me)) )
        return notify_fail("������û������������\n");

    return do_wield(me, ob, skill);
}

varargs int do_wield(object me, object ob, string skill)
{
    string str;

    if( ob->wield(skill) ) {
        if( !stringp(str = ob->query("wield_msg")) )
            str = "$Nװ��$n��������\n";
        message_vision(str, me, ob);
        return 1;
    } else
        return 0;
}

int help(object me)
{
    write(@HELP
ָ���ʽ�swield <��������> [as <������>]
 
���ָ������װ��ĳ����Ʒ�������������Ҫӵ��������Ʒ��

wield blade                 ������װ�������֣����Ӳ���������
wield blade as righthand    ������װ��������
wield blade as lefthand     ������װ��������
wield blade 2 as twohanded  �����ϵĵڶ��ѵ���װ����˫��

ĳ���������շ�������һ�����ϣ��շ���ͬʱ������������������Ҳ��ͬ��
������������ identify �����������Ѷ��
HELP);
    return 1;
}
