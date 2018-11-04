#include <ansi.h>
#include <command.h>

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    int value;
    object ob;
    seteuid(geteuid(this_player(1)));

    if( me->is_busy() ) return notify_fail("������û�գ�\n");

    if( !arg || arg=="" ) return notify_fail("��Ҫ����ʲô��\n");

    ob = present(arg, me);
    if( !ob ) ob = present(arg, environment(me));
    if( !ob ) return notify_fail("�⸽��û����������...\n");
    if( ob->is_corpse() && ob->query("usper") ) return notify_fail("�㲻�ܹ�����ҵ�ʬ�塣\n");
    if( ob->is_character() && !ob->is_corpse() ) return notify_fail("��ֻ�ܹ��ס���������\n");
    if( ob->query("no_sac") ) return notify_fail("���������޷�����...\n");
    if( ob->query("money_id") ) return notify_fail("���������޷�����...\n");
    if( ob->query("no_get") ) return notify_fail("���������޷�����...\n");
    if( ob->query("equipped") ) return notify_fail("Ҫ������������������ж��װ��...\n");
    if( ob->query("keep_this") ) return notify_fail("���������Ѿ�keep�����Ƚ��keep���ܹ��ס�\n");
    if( (!ob->query("value") || ob->query("value") == 0) && !ob->is_corpse() ) return notify_fail("�ⶫ��̫�ƾ��ˣ��޷����ס�\n");

    message_vision(YEL"$N�����۾�������������Ӱ(Acme)�����ţ�������$n"+YEL"��һ����̰�ס��ʧ�ˡ�\n"NOR, me, ob);
 
    destruct(ob);
    if( ob ) {
        write("���޷���������ǿ�ƴݻ١�\n");
        return 1;
    } 

    value = random(10)+1;
    write("������������" + chinese_number(value) + "������\n");
    me->add("bank",value);

    return 1;
}

int help()
{
    write(@TEXT
ָ���ʽ��sac <��Ʒ>

���ָ�����㽫û���õĶ������׸���أ������һЩ������

TEXT
    );
    return 1;
}