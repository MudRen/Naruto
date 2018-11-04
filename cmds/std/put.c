// put.c
#include <login.h>

inherit F_CLEAN_UP;

int do_put(object me, object obj, object dest);

void create() {seteuid(getuid());}

int main(object me, string arg)
{
	string target, item;
	object obj, dest, *inv, obj2;
	int i, amount;

	if(!arg || sscanf(arg, "%s in %s", item, target)!=2 )
		return notify_fail("��Ҫ��ʲô�����Ž�����t\n");

	dest = present(target, me);
	if( !dest || living(dest) ) dest = present(target, environment(me));
	if( !dest || living(dest) )
		return notify_fail("����û������������\n");

	if(sscanf(item, "%d %s", amount, item)==2) {
		if( !objectp(obj = present(item, me)) )
			return notify_fail("������û������������\n");
		if( !obj->query_amount() )
			return notify_fail( obj->name() + "���ܱ��ֿ���\n");
		if( amount < 1 )
			return notify_fail("����������������һ����\n");
		if( amount > obj->query_amount() )
			return notify_fail("��û����ô���" + obj->name() + "��\n");
		else if( amount == (int)obj->query_amount() ) {
			// add buy dragoon to fix the bug of amount
			if( obj->weight() > dest->query_max_encumbrance()
			- dest->query_encumbrance() )
				return notify_fail(dest->name() + "�ƺ�װ�����ˡ�\n");
			return do_put(me, obj, dest); }
		else {
			// add buy dragoon to fix the bug of amount
			if( amount*obj->weight()/obj->query_amount() >
			dest->query_max_encumbrance()-dest->query_encumbrance() ) 
				return notify_fail(dest->name() + "�ƺ�װ�����ˡ�\n");
			obj->set_amount( (int)obj->query_amount() - amount );
			seteuid(MUDLIB_UID);
			obj2 = new(base_name(obj));
			seteuid(getuid());
			obj2->set_amount(amount);
			return do_put(me, obj2, dest);
		}
	}

	if(item=="all") {
		inv = all_inventory(me);
		for(i=0; i<sizeof(inv); i++)
			if( inv[i] != dest ) do_put(me, inv[i], dest);
		write("Ok.\n");
		return 1;
	}

	if(!objectp(obj = present(item, me)))
		return notify_fail("������û������������\n");
	// add buy dragoon to fix the bug of amount
	if( obj->weight() > dest->query_max_encumbrance()
	- dest->query_encumbrance() )
		return notify_fail(dest->name() + "�ƺ�װ�����ˡ�\n");
	return do_put(me, obj, dest);
}

int do_put(object me, object obj, object dest)
{
	object inv;	

	if( obj->query("no_drop") || obj->query("no_get"))
		return notify_fail("���������޷����ƶ���\n");

    if( obj->query("keep_this") ) 
        return notify_fail("�����������Ѿ� keep �޷����ƶ���\n");
    
    if( obj->query("equipped") )
        return notify_fail("�����������Ѿ�װ�����޷����ƶ���\n");

	if( obj==dest ) 
		return notify_fail("��" + obj->name() + "�Ž�" + dest->name() + "�t�t�t\n");

	if( !dest->accept_object(me, obj) ) return 0;

	if( inv=present(obj->query_id(), dest) ) {
		if( obj->query_amount() > 0 ) {
			inv->set_amount((int)inv->query_amount() + (int)obj->query_amount());
			message_vision( sprintf("$N��һ%s%s�Ž�%s��\n", obj->query("unit"), obj->name(), dest->name()), me);
			destruct(obj);
		} else {
			if( obj->move(dest) )
			    message_vision( sprintf("$N��һ%s%s�Ž�%s��\n", obj->query("unit"), obj->name(), dest->name()), me); }
	} else { 
		if( obj->move(dest) )
		    message_vision( sprintf("$N��һ%s%s�Ž�%s��\n", obj->query("unit"), obj->name(), dest->name()),	me );
	}
#ifdef SAVE_USER
        me->save();
#endif
	return 1;
}

int help(object me)
{
write(@HELP
ָ���ʽ : put <��Ʒ����> in <ĳ����>
 
���ָ��������㽫ĳ����Ʒ�Ž�һ�������o��Ȼ�o������Ҫӵ��������Ʒ��
 
HELP
    );
    return 1;
}
