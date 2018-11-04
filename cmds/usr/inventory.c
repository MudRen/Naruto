/*  inventory.c

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
    int i;
    object *inv, ob;

    if( wizardp(me) && arg ) {
        ob = find_player(arg);
        if( !ob ) ob = find_living(arg);
        if( !ob ) ob = present(arg, environment(me));
        if( !ob ) return notify_fail("��Ҫ��˭����Ʒ��\n");
    }
    else ob = me;

    inv = all_inventory(ob);
    if( !sizeof(inv) ) {
        write((ob==me)? "Ŀǰ" + gender_self(me) + "����û���κζ�����\n"
            : ob->name() + "����û��Я���κζ�����\n");
        return 1;
    }
    printf("%s���ϴ���������Щ����(���� %d%%)�s\n%s\n",
        (ob==me)? gender_self(me) : ob->name(),
        (int)ob->query_encumbrance() * 100 / (1+(int)ob->query_ability("carriage")),
        implode(map_array(inv, "inventory_desc", this_object()), "\n") );

    return 1;
}

string inventory_desc(object ob)
{
    string desc = ob->short();

    if( ob->query("keep_this")) desc += "��";

    if( ob->query("equipped"))  desc = HIY"��"NOR + desc;
    else desc = "  " + desc;
    desc+=" ";
    
    if( wizardp(this_player()) ) desc += " " + base_name(ob);
    
    return desc;
}

int help (object me)
{
    write(@HELP
ָ���ʽ: inventory
 
���г���(��)Ŀǰ������Я����������Ʒ��
��  �Ǻż�����ɫ��ʾ����ʹ�ã� �� �Ǻű�ʾ�� keep��
ע : ��ָ����� " i " ���档
 
HELP
    );
    return 1;
}
