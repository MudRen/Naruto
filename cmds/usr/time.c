/*  time.c */

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    seteuid(getuid());
    write("��Ϸʱ�䣺" + NATURE_D->game_time() + "��\n");
    write("��ʵʱ�䣺" + CHINESE_D->chinese_date(time()) + "\n");
    write("��Ӱ�����Ѿ�ִ����" + CHINESE_D->chinese_period(uptime()) + "��\n");
    return 1;
}

int help(object me)
{
     write(@HELP
ָ���ʽ: time

���ָ������(��)֪�����ڵ�ʱ�䡣
HELP);
    return 1;
}
