/* get.c */

#pragma save_binary

#include <ansi.h>

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

int do_get(object me, object item, object env)
{
	int equipped;
	object *guard;

	if( !item || !objectp(item) ) if( !objectp(item=find_object((string)item)) ) return 0;

    if( item->is_character() ) return notify_fail("��ֻ���ö�����\n");
    
    if( env->hold_object(item) ) return notify_fail("���޷���ȡ����������\n");
    
    if( guard = item->query_temp("guarded") ) {
		guard = filter_array(guard, (: objectp($1) && present($1, environment($2)) && living($1) && ($1!=$2) :), me);
		if( sizeof(guard) ) return notify_fail( guard[0]->name() + "������" + item->name() + "һ�ԩo��ֹ�κ������ߡ�\n");
    }

    if( item->query("no_get") ) return notify_fail("���޷���ȡ����������\n");
    
    if( me->over_encumbranced() ) return notify_fail("���Ѿ����ɹ����ˣ�\n");
    
    if( item->query("equipped") ) equipped = 1;

    if( !item->move(me) ) return 0;
    if( me->is_fighting() ) me->start_busy(1);
    if( item->is_character() ) message_vision( "$N��$n�����������ڱ��ϡ�\n", me, item);
    else {
    	message_vision( sprintf("$N%sһ%s$n��\n", 
          env == environment(me) ? "����" : (env->is_character() ? "��" + env->name() + "����" + (equipped? "����" : "�ѳ�") : "��" + env->name() + "���ó�"),
		  item->query("unit")), me, item);
	}
    return 1;

}

int main(object me, string arg)
{
	int amount, item_amount;
	string item, from;
	object item_obj, from_obj, *inv;
	
	if( !arg ) return notify_fail("����Ҫ����ʲô��\n");
	if( me->is_busy() ) return notify_fail("����һ��������û����ɣ�\n");
	
    // ���Ӻ���ȡ����
	if( sscanf(arg, "%s from %s", arg, from) == 2 ) {
		from_obj = present(from, me);
		if( !objectp(from_obj) ) from_obj = present(from, environment(me));
		if( !objectp(from_obj) ) return notify_fail("�㸽��û�� " + from + " ����������\n");
		if( (living(from_obj) || userp(from_obj)) && (wiz_level(me) <= wiz_level(from_obj)))
            return notify_fail("�����ʦ�ȼ�����ȶԷ��ߣ���������\n");
	} else from_obj = environment(me);
	
	// ����Ƿ���ȡ�������ĳ������
    if( sscanf(arg, "%d %s", amount, item) == 2 ) {
    	if( !objectp(item_obj = present(item, from_obj)) )
    		return notify_fail("����û������������\n");
    	if( !item_obj->query_amount() )
    		return notify_fail( item_obj->name() + "�޷��ֿ����ߡ�\n");
    	if( amount < 1 )
    		return notify_fail("��ȡ�Ķ�����������Ҫһ�����ϡ�\n");

    	item_amount = item_obj->query_amount();

    	if( amount > item_amount )
    		return notify_fail("����û����ô���" + item_obj->name() + "��\n");
    	
    	// ��ȡ�������պ�����������һ��
    	if( amount == item_amount ) {
    		return do_get(me, item_obj, from_obj);
    	// ��ȡ������������������һ��
    	} else {
    		int new_amount;
    		object new_obj;
    		new_obj = new(base_name(item_obj));
    		
    		if( !objectp(new_obj) )
    			return notify_fail("���ƺ��޷���ȡ����������\n");
    		
    		// �ֶ�
    		new_obj->set_amount(amount);
    	    item_obj->add_amount(-amount);

    		if( me->is_fighting() ) me->start_busy(3);

            // �޷�����dest new obj, ����ԭ����item amount �ָ�
    		if( !do_get(me, new_obj, from_obj) ) {
    			destruct(new_obj);
    			item_obj->add_amount(amount);
    			return notify_fail("���ƺ��޷���������������\n");
    		}

    		return 1;
    	}
    }

    if( arg == "all" ) {
    	if( me->is_fighting() )
    		return notify_fail("�㻹��ս���У�ֻ��һ����һ����\n");
    	if( !function_exists("receive_object", from_obj) )
    		return notify_fail("���޷���������ȡ�κζ�����\n");
    	
    	// ȡ�û��������������
    	if( from_obj->is_area() ) {
    		mapping info;
    		if( (info = me->query("area_info")) )
    		inv = from_obj->query_inventory(info["x_axis"], info["y_axis"]);
        } else inv = all_inventory(from_obj);
        
        // ѭ������
        foreach(item_obj in inv) do_get(me, item_obj, from_obj);
        write("Ok.\n");
        return 1;
    }

    if( !objectp(item_obj = present(arg, from_obj)) || living(item_obj) )
    	return notify_fail("�㸽��û������������\n");

	return do_get(me, item_obj, from_obj);
}

int help(object me)
{
    write(@HELP
ָ���ʽ : get <��Ʒ����> [from <������>]
 
���ָ��������������ϻ������ڵ�ĳ����Ʒ��
 
HELP
    );
	return 1;
}