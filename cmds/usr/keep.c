/*  keep.c

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

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    object ob, *all_ob;
    int i;

    if( !arg ) return notify_fail("ָ���ʽ��keep <������Ʒ>\n");
    if( arg=="all" ) {
        all_ob = all_inventory(me);
        i = sizeof(all_ob);
        while( i-- )
            all_ob[i]->set("keep_this", 1);
        write("�㽫�������ϵĶ������˸�С�Ǻš�\n");
        return 1;
    }
    if( arg=="none" ) {
        all_ob = all_inventory(me);
        i = sizeof(all_ob);
        while( i-- )
            all_ob[i]->set("keep_this", 0);
        write("�㽫�������ϵĶ���֮С�Ǻš������\n");
        return 1;
    }
    ob = present(arg, me);
    if( !ob ) return notify_fail("������û������������\n");
    if( userp(ob) ) return notify_fail("���С�\n");
    if( ob->query("keep_this") ) {
        ob->delete("keep_this");
        write("��� " + ob->name() + " �����С�Ǻš�����ˡ�\n");
    }
    else {
        ob->set("keep_this", 1);
        write("���� " + ob->name() + " ������˸�С�Ǻš�\n");
    }
    return 1;
}

int help(object me)
{
    write(@HELP
ָ���ʽ��keep <������Ʒ>|<all>|<none>

����:
keep none    ��������Ʒ��� keep
keep sword    ��� sword �趨 keep ���� keep

�趨������Ҫ��Ʒ���ᱻ��С�Ķ���(�� drop all �� sell all)������ inventory
ָ��鿴�Ƿ����趨 keep ��

�趨�����ⶪ���Ķ���ֻ�е���ָ������/�����������ʱ�Żᱻ������������������
�趨�ᱻ��������㽫�趨�����ⶪ������Ʒ�������ϣ��ټ��������������趨һ�Ρ�
HELP
    );
    return 1;
}
