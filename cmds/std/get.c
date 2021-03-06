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

    if( item->is_character() ) return notify_fail("你只能拿东西。\n");
    
    if( env->hold_object(item) ) return notify_fail("你无法拿取这样东西。\n");
    
    if( guard = item->query_temp("guarded") ) {
		guard = filter_array(guard, (: objectp($1) && present($1, environment($2)) && living($1) && ($1!=$2) :), me);
		if( sizeof(guard) ) return notify_fail( guard[0]->name() + "正守在" + item->name() + "一旁﹐防止任何人拿走。\n");
    }

    if( item->query("no_get") ) return notify_fail("你无法拿取这样东西。\n");
    
    if( me->over_encumbranced() ) return notify_fail("你已经负荷过重了！\n");
    
    if( item->query("equipped") ) equipped = 1;

    if( !item->move(me) ) return 0;
    if( me->is_fighting() ) me->start_busy(1);
    if( item->is_character() ) message_vision( "$N将$n扶了起来背在背上。\n", me, item);
    else {
    	message_vision( sprintf("$N%s一%s$n。\n", 
          env == environment(me) ? "捡起" : (env->is_character() ? "从" + env->name() + "身上" + (equipped? "除下" : "搜出") : "从" + env->name() + "中拿出"),
		  item->query("unit")), me, item);
	}
    return 1;

}

int main(object me, string arg)
{
	int amount, item_amount;
	string item, from;
	object item_obj, from_obj, *inv;
	
	if( !arg ) return notify_fail("你想要捡起什么？\n");
	if( me->is_busy() ) return notify_fail("你上一个动作还没有完成！\n");
	
    // 捡查从何拿取东西
	if( sscanf(arg, "%s from %s", arg, from) == 2 ) {
		from_obj = present(from, me);
		if( !objectp(from_obj) ) from_obj = present(from, environment(me));
		if( !objectp(from_obj) ) return notify_fail("你附近没有 " + from + " 这样东西。\n");
		if( (living(from_obj) || userp(from_obj)) && (wiz_level(me) <= wiz_level(from_obj)))
            return notify_fail("你的巫师等级必须比对方高，才能搜身。\n");
	} else from_obj = environment(me);
	
	// 捡查是否拿取复合物的某个数量
    if( sscanf(arg, "%d %s", amount, item) == 2 ) {
    	if( !objectp(item_obj = present(item, from_obj)) )
    		return notify_fail("这里没有这样东西。\n");
    	if( !item_obj->query_amount() )
    		return notify_fail( item_obj->name() + "无法分开拿走。\n");
    	if( amount < 1 )
    		return notify_fail("拿取的东西个数至少要一个以上。\n");

    	item_amount = item_obj->query_amount();

    	if( amount > item_amount )
    		return notify_fail("这里没有这么多的" + item_obj->name() + "。\n");
    	
    	// 捡取的数量刚好与所有数量一样
    	if( amount == item_amount ) {
    		return do_get(me, item_obj, from_obj);
    	// 捡取的数量不与所有数量一样
    	} else {
    		int new_amount;
    		object new_obj;
    		new_obj = new(base_name(item_obj));
    		
    		if( !objectp(new_obj) )
    			return notify_fail("你似乎无法拿取这样东西。\n");
    		
    		// 分堆
    		new_obj->set_amount(amount);
    	    item_obj->add_amount(-amount);

    		if( me->is_fighting() ) me->start_busy(3);

            // 无法捡起dest new obj, 并将原来的item amount 恢复
    		if( !do_get(me, new_obj, from_obj) ) {
    			destruct(new_obj);
    			item_obj->add_amount(amount);
    			return notify_fail("你似乎无法捡起这样东西。\n");
    		}

    		return 1;
    	}
    }

    if( arg == "all" ) {
    	if( me->is_fighting() )
    		return notify_fail("你还在战斗中！只能一次拿一样。\n");
    	if( !function_exists("receive_object", from_obj) )
    		return notify_fail("你无法从这里拿取任何东西。\n");
    	
    	// 取得环境中所有物件集
    	if( from_obj->is_area() ) {
    		mapping info;
    		if( (info = me->query("area_info")) )
    		inv = from_obj->query_inventory(info["x_axis"], info["y_axis"]);
        } else inv = all_inventory(from_obj);
        
        // 循序拿起
        foreach(item_obj in inv) do_get(me, item_obj, from_obj);
        write("Ok.\n");
        return 1;
    }

    if( !objectp(item_obj = present(arg, from_obj)) || living(item_obj) )
    	return notify_fail("你附近没有这样东西。\n");

	return do_get(me, item_obj, from_obj);
}

int help(object me)
{
    write(@HELP
指令格式 : get <物品名称> [from <容器名>]
 
这个指令可以让你捡起地上或容器内的某样物品。
 
HELP
    );
	return 1;
}