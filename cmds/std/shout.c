// shout.c

#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	if( !wizardp(me) )
		return notify_fail("Ŀǰ�ݲ�������� shout��\n");
	if (!arg) return notify_fail("����Ҫ���ʲô?\n");
	shout( me->name() + "������Х�s" + HIY + arg + NOR + "\n");
	write("��������Х�s" + HIY + arg + NOR + "\n");
	return 1;
}
int help(object me)
{
  write(@HELP
ָ���ʽ : shout <ѶϢ>
 
���ָ�����㽫 <ѶϢ> ���ͳ�ȥ, ������������δ����˶���������Ļ���Ŀǰֻ
����ʦ���á�
 
see also : tune
HELP
    );
    return 1;
}
 
