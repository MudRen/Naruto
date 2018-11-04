// cat.c

#include "/doc/help.h"

inherit F_CLEAN_UP;
int help(object me);

int main(object me, string arg)
{
	string file, text;

	seteuid(geteuid(this_player(1)));

    if (!arg) return notify_fail("����Ҫ��ʾ�Ǹ�����?\n");
	file = resolve_path(me->query("cwd"), arg);
	if( file_size(file)<0 ) return notify_fail("û�����������\n");
	if( !text = read_file(file) ) text = "";
	write(text);
	return 1;
}

int help(object me)
{
write(@HELP
ָ���ʽ : cat <����>

��ָ�������(��)ֱ�Ӷ�ȡĳ����֮��ʽ��
HELP
    );
    return 1;
}
