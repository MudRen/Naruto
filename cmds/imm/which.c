// which.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	string f;

	if( !arg ) return notify_fail("ָ���ʽ�swhich <ָ��>\n");

	if( f = me->find_command(arg) ) {
		write(f + "\n");
		return 1;	
	}
	write("���ָ��·����û�����ָ��o������ localcmds ���ָ�\n");
	return 1;
}

int help(object me)
{
	write(@Help
ָ���ʽ: which <ָ��>

�ҳ�ָ�����ڵ�Ŀ¼��

Help
	);
	return 1;
}
