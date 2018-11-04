/* balance.c by -Acme- */

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object who;
	
    if( arg && !wizardp(me) )
    	return notify_fail("ֻ����ʦ���ܲ�ѯ���˵Ĵ�\n");

    if( !arg ) who = me;
    else {
        if( !who ) who = present(arg, environment(me));
        if( !who ) who = find_living(arg);
        if( !who ) return notify_fail("û�� " + arg + " ������\n");
        write("[" + who->name() + "] ");
    }

    tell_object(me, "��Ŀǰ��" + chinese_number(who->query("bank")) + "�����ӵĴ�\n");

    return 1;
}

int help()
{
    write(@TEXT
ָ���ʽ�sbalance [ĳ��]

���ָ����Բ�ѯ������Ŀǰ�Ĵ�
TEXT);
    return 1;
}
