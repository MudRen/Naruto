// reclaim.c

int main(object me, string arg)
{
	int mem;

	write("�������o����� " + reclaim_objects() + " ��������\n");
	return 1;
}

int help(object me)
{
write(@HELP
ָ���ʽ : reclaim

�ô� : �������ڼ����������õı��������, �Լ��ټ������ʹ�á�
HELP
     );
     return 1;
}
