// rehash.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
        seteuid(geteuid(this_player(1)));
	if( !arg )
		return notify_fail("ָ���ʽ�srehash <·��>\n");

	if( arg[strlen(arg)-1]!='/' ) arg += "/";
	if(  file_size(arg)!=-2 )
		return notify_fail("û�����Ŀ¼�u\n");

	write("����ָ���s" + arg + "\n");
	seteuid(getuid());
	COMMAND_D->rehash(arg);
	return 1;
}

int help(object me)
{
	write(@HELP
ָ���ʽ�srehash <Ŀ¼����>

���ָ������ϵͳ�ڲ���ָ���o�����������ɾ����ĳ��Ŀ¼�µ�ָ��o������
���ָ����¸�Ŀ¼����ʹ�á�
HELP
	);
	return 1;
}
