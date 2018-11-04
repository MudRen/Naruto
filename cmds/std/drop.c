// drop.c

#include <login.h>
inherit F_CLEAN_UP;

int do_drop(object me, object obj);

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    object obj, *inv, obj2;
    int i, amount;
    string item;

    if( me->is_busy() )
        return notify_fail("����æ�������©u\n");

    if(!arg) return notify_fail("��Ҫ����ʲô�����t\n");
    if( !environment(me) ) return 0;

    if(sscanf(arg, "%d %s", amount, item)==2) {
        if( !objectp(obj = present(item, me)) )
            return notify_fail("������û������������\n");
        if( stringp(obj->query("no_drop")) || obj->query("keep_this") )
            return notify_fail("�������û�취�����������Ѿ�keep��...\n");
        if( !obj->query_amount() )
            return notify_fail( obj->name() + "���ܱ��ֿ�������\n");
        if( amount < 1 )
            return notify_fail("����������������һ����\n");
        if( amount > obj->query_amount() )
            return notify_fail("��û����ô���" + obj->name() + "��\n");
        else if( amount == (int)obj->query_amount() )
            return do_drop(me, obj);
        else {
            obj->set_amount( (int)obj->query_amount() - amount );
            seteuid(MUDLIB_UID);
            obj2 = new(base_name(obj));
            seteuid(getuid());
            obj2->set_amount(amount);
            return do_drop(me, obj2);
        }
    }

    if(arg=="all") {
        inv = all_inventory(me);
        for(i=0; i<sizeof(inv); i++) {
            do_drop(me, inv[i]);
        }
        write("Ok.\n");
        return 1;
    }

    if(!objectp(obj = present(arg, me)))
        return notify_fail("������û������������\n");
    return do_drop(me, obj);
}

int do_drop(object me, object obj)
{
    mixed no_drop;

    if( no_drop = obj->query("no_drop") )
        return notify_fail( stringp(no_drop) ? no_drop : "���������������ⶪ����\n");

    if( obj->query("keep_this") ) 
    	return notify_fail("�����������Ѿ� keep �޷�������\n");
    
    if( obj->query("equipped") )
    	return notify_fail("�����������Ѿ�װ�������޷�������\n");

    if( environment(me)->is_area() ) {
    	if( !area_move(environment(me), obj, me->query("area_info/x_axis"), me->query("area_info/y_axis")) ) return 0;
    } else if( !obj->move(environment(me)) ) return 0;

    if( obj->is_character() )
        message_vision("$N��$n�ӱ��Ϸ��������o���ڵ��ϡ�\n", me, obj);
    else {
        message_vision( sprintf("$N����һ%s$n��\n", obj->query("unit")), me, obj );
		if( !obj->query("value") && !obj->value() ) {
		    write("��Ϊ������������ֵǮ�o�������ǲ�����ע�⵽���Ĵ��ڡ�\n");
			destruct(obj);
			return 1;
	    }
	}

	// ��ֵǮ�Ķ�����ͱ� dest ����... ����û�� obj.... -elon 12/2/96
	if( obj ) obj->delete("keep_this");

	return 1;
}

int help(object me)
{
    write(@HELP
ָ���ʽ : drop <��Ʒ����>
 
���ָ��������㶪������Я������Ʒ.
����:
drop all	��������δ keep ����Ʒ.
drop sword 2	��������Я���ĵڶ��� sword.
drop 200 gold	���¶������ƽ�. 
HELP
    );
    return 1;
}
