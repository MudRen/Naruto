/* sell.c by -Acme-
*/

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
	object who;
	string msg;
	
	if( !environment(me) ) return notify_fail("你现在人在哪里都不知道了。\n");
	
	if( !arg ) {
		if( !stringp(msg = environment(me)->do_list()) )
			return notify_fail("这个地方无法 list , 若你是想对人物做 list 请加上ID ...\n");
		me->start_more(msg);
		return 1;
	}
	
	if( !objectp(who=present(arg, environment(me))) )
		return notify_fail("你附近似乎没有这个人。\n");
	
	if( !stringp(msg = who->do_list()) )
		return notify_fail("这个人无法 list ...\n");
	
	me->start_more(msg);
	return 1;
}

int help(object me)
{
  write(@HELP
指令格式 : list [某人]

这指令可以列出某人所正在贩卖的物品列表，或是某人可以训练的技
能列表，若是在当铺里只需 list 即可列出当铺贩卖的物品列表，不
用加上某人的英文名称。

相关指令： train, sell, buy
HELP);
    return 1;
}