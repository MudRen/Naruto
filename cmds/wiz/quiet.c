// quiet command..

inherit F_CLEAN_UP;

int main(object me, string str)
{
        object ob;
        int i;

        seteuid(getuid(me));
        if(!str) return notify_fail("ָ���ʽ: quiet <ʹ����>\n");
        ob = LOGIN_D->find_body(str);
        if(!ob) return notify_fail("���ϵ�ʹ������û������ˡ�\n");

	seteuid(geteuid(ob));
	if (ob->query("no_chat") == 1) {
		ob->set("no_chat", 0) ; 
		tell_object(ob, "��ʦ("+me->query("id")+")ȡ����Ľ���\n");
		tell_object(me,"��ȡ�������("+ob->query("id")+")�Ľ���\n");
		}
	else {
		ob->set("no_chat", 1) ;
		tell_object(ob, "��ʦ("+me->query("id")+")��ֹ��ʹ�ù���Ƶ��\n");
                tell_object(me,"���������("+ob->query("id")+")�Ľ���\n");
		}
        return 1;
}

int help(object me)
{
write(@HELP
ָ���ʽ : quiet <ĳ��>

���������ٴ�ʹ��quite �Ϳ���ȡ������
��ָ�������(��)��ĳ�˽�ֹʹ�ù���Ƶ����
HELP
    );
    return 1;
}
