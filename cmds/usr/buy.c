/* buy.c write by -Acme- */

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
	int amount, number;
	string from;
	object who;
	
	if( !arg ) return notify_fail("����Ҫ��ʲô������\n");
	
	if( sscanf(arg, "%d %d from %s", amount, number, from) == 3 ) {
		if( !objectp(who=present(from, environment(me))) )
		    return notify_fail("�㸽��û������ˡ�\n");
	    if( !who->to_sell(me, number, amount) )
	    	return notify_fail("������ƺ��������������\n");
	} else if( sscanf(arg, "%d from %s", number, from) == 2 ) {
		if( !objectp(who=present(from, environment(me))) )
		    return notify_fail("�㸽��û������ˡ�\n");
	    if( !who->to_sell(me, number, 1) )
	    	return notify_fail("������ƺ��������������\n");
	} else if( sscanf(arg, "%d %d", amount, number) == 2 ) {
		if( !environment(me)->to_sell(me, number, amount) )
			return notify_fail("�����ƺ������̵ꡣ\n");
	} else if( sscanf(arg, "%d", number) == 1 ) {
        if( !environment(me)->to_sell(me, number, 1) )
            return notify_fail("�����ƺ������̵ꡣ\n");
	} else return notify_fail("����Ҫ��ʲô������\n");
	
	return 1;
}

int help(object me)
{
   write(@HELP
ָ���ʽ: buy [����] ��Ʒ��� [from ĳ��]

��ָ�����������̵�����ĳ����Ʒ��Ҳ�ɴ�ĳ����������Ʒ������
���̵�����Ʒʱ��ָ���ʽ�в���Ҫ���� from ĳ�ˡ�����

buy 3                ���̵����±��Ϊ3����Ʒ
buy 2 5              ���̵����¶������Ϊ5����Ʒ
buy 2 from waiter    ��water�������±��Ϊ2����Ʒ
buy 10 7 from waiter ��water��������ʮ�����Ϊ7����Ʒ

���ָ� list, sell
HELP);
   return 1;
}