// follow.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object ob;

	if( !arg ) return notify_fail("ָ���ʽ�sfollow <ĳ��>|none��\n");

	if( arg=="none")
		if( me->query_leader() ) {
			me->set_leader(0);
			write("Ok.\n");
			return 1;
		} else {
			write("�����ڲ�û�и����κ��ˡ�\n");
			return 1;
		}

	if( !objectp(ob = present(arg, environment(me))) )
		return notify_fail("����û�� " + arg + "��\n");

        // add by dragoon
        if( userp(ob) && !interactive(ob) )
                return notify_fail("�������ڲ������ϡ�\n");   

	if( !ob->is_character() )
		return notify_fail("ʲô�t����...." + ob->name() + "��\n");

	if( ob==me )
		return notify_fail("�����Լ��t\n");

	me->set_leader(ob);
	message_vision("$N������ʼ����$nһ���ж���\n", me, ob);

	return 1;
}

int help (object me)
{
	write(@HELP
ָ���ʽ�sfollow [<ĳ��>|none]
 
���ָ�������ܸ���ĳ�˻����
������� follow none ��ֹͣ���档
 
HELP
	);
	return 1;
}
