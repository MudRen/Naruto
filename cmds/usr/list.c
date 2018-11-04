/* sell.c by -Acme-
*/

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
	object who;
	string msg;
	
	if( !environment(me) ) return notify_fail("�������������ﶼ��֪���ˡ�\n");
	
	if( !arg ) {
		if( !stringp(msg = environment(me)->do_list()) )
			return notify_fail("����ط��޷� list , ��������������� list �����ID ...\n");
		me->start_more(msg);
		return 1;
	}
	
	if( !objectp(who=present(arg, environment(me))) )
		return notify_fail("�㸽���ƺ�û������ˡ�\n");
	
	if( !stringp(msg = who->do_list()) )
		return notify_fail("������޷� list ...\n");
	
	me->start_more(msg);
	return 1;
}

int help(object me)
{
  write(@HELP
ָ���ʽ : list [ĳ��]

��ָ������г�ĳ�������ڷ�������Ʒ�б�����ĳ�˿���ѵ���ļ�
���б������ڵ�����ֻ�� list �����г����̷�������Ʒ�б���
�ü���ĳ�˵�Ӣ�����ơ�

���ָ� train, sell, buy
HELP);
    return 1;
}