// guard.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
        object ob;

	if( me->query("life_form") == "ghost" )
		return notify_fail("�����˾�һ�˰���, ��ȥ�ҳ��򸴻��!!\n");

        if( !arg || arg=="" ) {
                if( ob = me->query_guarding() ) {
                        write("�����ڱ����Ķ�����" + ob->name() + "��\n");
                        return 1;
                }
                write("�����ڲ�û��ѡ�������Ķ���\n");
                return 1;
        }

        if( arg=="cancel" ) {
                write("Ok.\n");
                me->guard_ob(0);
                return 1;
        }

        ob = present(arg, environment(me));
        if( !ob ) return notify_fail("����û������ˡ�\n");
        if( ob==me ) return notify_fail("�㡰������Ȼ���Ļᱣ���Լ���\n");
        if( !userp(ob) )
                return notify_fail("������Ҫ��ı�����\n");
        if( me->is_fighting(ob) )
                return notify_fail("���뱣�����ڸ���ս���Ķ��֣�\n");

        me->guard_ob(ob);
        write("Ok.\n");

        return 1;
}

int help(object me)
{
        write(@TEXT
ָ���ʽ�sguard [<ս��Ŀ��>|cancel]

����������ָ���趨��Ҫ�����Ķ��󣬵��㱣���Ķ������ܹ���ʱ������������
��һʱ�����ս����ս��

Ҫȡ�� guard ģʽ�o������ guard cancel�o�����κβ�������ʾ��Ŀǰ�������Ķ�
��
TEXT
        );
        return 1;
}
