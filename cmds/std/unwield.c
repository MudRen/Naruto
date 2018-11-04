// unwield.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object ob;
	string equipped, str;

	if( !arg ) {
		mapping weapons;
		string skill;

		if( mapp(weapons = me->query_temp("weapon")) && sizeof(weapons) ) {
			foreach(skill, ob in weapons) ob->unequip();
			write("Ok.\n");
			return 1;
		} else
			return notify_fail("�����ڲ�û��װ���κ�������\n");
	}

	if( !objectp(ob = present(arg, me)) )
		return notify_fail("������û������������\n");

	if( !stringp(equipped = ob->query("equipped"))
	||	!sscanf(equipped, "weapon/%*s") )
		return notify_fail("�㲢û��װ������������Ϊ������\n");

	if( ob->unequip() ) {
		if( !stringp(str = ob->query("unwield_msg")) )
			str = "$N�������е�$n��\n";
		message_vision(str, me, ob);
		return 1;
	} else
		return 0;
}

int help(object me)
{
  write(@HELP
ָ���ʽ : unwield <��Ʒ��>
 
���ָ������������е�������
 
HELP
    );
    return 1;
}
 
