/* eat.c */

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	int stuff;
    object ob;

    if( me->is_busy() ) return 0;

    if( !arg || arg=="" ) return notify_fail("��Ҫ��ʲô�t\n");

    ob = present(arg, me);
    if( !ob ) ob = present(arg, environment(me));
    if( !ob ) return notify_fail("����û���������� ...��\n");

    stuff = ob->query("food_stuff");
    if( !stuff || !ob->stuff_ob(me)) {
        write("�����������ܳ�...\n");
        return 1;
    }

    message_vision("$N������һ" + ( ob->query("base_unit") ? ob->query("base_unit") : ob->query("unit") ) +
    	           "$n��\n", me, ob);
    
    ob->add_amount(-1);
    if( ob->query_amount() <= 0 ) destruct(ob);

    return 1;
}

int help()
{
    write(@TEXT
ָ���ʽ��eat <ʳ��>

���ָ����������������ʳ�����ҩ�
TEXT);
    return 1;
}
