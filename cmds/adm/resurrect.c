// resurrect command..

inherit F_CLEAN_UP;

int main(object me, string str)
{
        object ob;
        int i;

        seteuid(ROOT_UID);
        if(!str) return notify_fail("ָ���ʽ: resurrect <ʹ����>\n");
        ob = LOGIN_D->find_body(str);
        if(!ob) return notify_fail("���ϵ�ʹ������û������ˡ�\n");
        write(file_name(environment(ob))+"\n");
	CHAR_D->make_living(ob);
	ob->setup();

        // ok ..
        write("ok.\n");
        return 1;
}

int help(object me)
{
write(@HELP
ָ���ʽ : resurrect <ĳ��>

��ָ�������(��)��ĳ�˸��
HELP
    );
    return 1;
}
