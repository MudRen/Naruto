// arrest command..

inherit F_CLEAN_UP;

int main(object me, string str)
{
        object ob;
        int i;

        seteuid(getuid(me));
        if(!str) return notify_fail("ָ���ʽ: arrest <ʹ����>\n");
        ob = LOGIN_D->find_body(str);
        if(!ob) return notify_fail("���ϵ�ʹ������û������ˡ�\n");
	if(ob->query("level") > 4 ) 
		return notify_fail("�������Ǳ������ĵȼ�����ȡ�����ı���\n");

	seteuid(geteuid(ob));
	if (ob->query("get_kill") == 1) {
		ob->set("get_kill", 0) ; 
		tell_object(ob, "��ʦ("+me->query("id")+")�ָ���ı���\n");
		tell_object(me,"��ָ������("+ob->query("id")+")�ı���\n");
		}
	else {
		ob->set("get_kill", 1) ;
		tell_object(ob, "��ʦ("+me->query("id")+")ȡ����ı���\n");
                tell_object(me,"��ȡ�����("+ob->query("id")+")�ı���\n");
		}
        return 1;
}

int help(object me)
{
write(@HELP
ָ���ʽ : arrest <ĳ��>

��ָ�������ȡ���ض�������ҵĵȼ�����
HELP
    );
    return 1;
}
