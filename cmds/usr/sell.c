/* sell.c by -Acme-
*/

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int sell_item(object item)
{
	int value;
    string name;

    if( item->query("money_id") )
        return notify_fail("��Ҫ����Ǯ���t\n");

    if( item->query("no_sell") )
        return notify_fail("����������������\n");

    if( !(value = item->query("value")) )
        return notify_fail("����������ֵǮ��\n");

    if( item->query("keep_this") )
    	return notify_fail("�����������Ѿ� keep �޷�������\n");
    
    if( item->query("equipped") )
        return notify_fail("�����������Ѿ�װ�������޷�������\n");

    value /= 2;

    name = ( item->query_amount() ? chinese_number(item->query_amount()) : "һ" );
    name += ( item->query("base_unit") ? item->query("base_unit") : 
            ( item->query("unit") ? item->query("unit") : "��" ) );
    name += item->query("name");

    if( !environment(this_player())->to_stock(item) )
    	return notify_fail("�����ƺ���Ը����������Ʒ��\n");

    message_vision("$N��" + chinese_number(value) + "�����ӵļ�Ǯ��" + name + "����ȥ��\n", this_player() );
    
    // ֱ�ӽ�Ǯ�ӵ�������
    this_player()->add("bank", value);

    return 1;
}

int main(object me, string arg)
{
	int amount;
	object item;

	if( !environment(me) )
	    return notify_fail("�������������㶼��֪���ˣ�\n");
	if( !environment(me)->is_hockshop() )
	    return notify_fail("��ֻ���ڵ�����������\n");
    if( me->is_busy() )
        return notify_fail("������û�пգ�\n");
    if( !arg )
    	return notify_fail("������ʲô������\n");
    
    if( arg == "all" ) {
        object *inv;
        
        inv = all_inventory(this_player());
        foreach(item in inv) {
            if( !item ) continue;
            sell_item(item);
        }
        write("Ok��\n");
        return 1;
    }

    if( sscanf(arg, "%d %s", amount, arg) == 2 ) {
    	object new_item;
    	if( !(item = present(arg, me)) )
    		return notify_fail("����Ҫ����ʲô��Ʒ��\n");
    	if( amount <= 0 ) 
    		return notify_fail("����Ҫ��������Ʒ��\n");
    	if( item->query_amount() < amount )
    		return notify_fail("������û����ô���" + item->query("name") + "��\n");
    	
    	// ��ͬ����
    	if( amount == item->query_amount() )
    		return sell_item(item);
    	
    	if( !objectp(new_item = new(base_name(item))) )
    		return notify_fail("���ƺ��޷�����������Ʒ��\n");
    	
    	new_item->set_amount(amount);
    	item->add_amount(-amount);
    	
    	// �޷���������dest new_item, ����ԭ����item amount�ָ�
    	if( !sell_item(new_item) ) {
    		destruct(new_item);
    		item->add_amount(amount);
    	}
    	
    	return 1;
    }

    if( !(item = present(arg, me)) )
        return notify_fail("����Ҫ����ʲô��Ʒ��\n");

    return sell_item(item);
}

int help(object me)
{
  write(@HELP
ָ���ʽ : sell [����] ��Ʒ

���ָ����Խ������ϵ���Ʒ����ȥ��ͨ��ֻ���ڵ��̵ĵط����ܹ�
������Ʒ������

sell blade     �����ϵĵ�һ�ѵ�����ȥ
sell blade 2   �����ϵĵڶ��ѵ�����ȥ
sell 3 pill    �����ϵ�pill����������ͨ�����ڡ�������Ʒ����
sell 4 pill 2  �����ϵڶ��ѵ�pill�����ĸ�

���ָ� list, buy
HELP);
    return 1;
}