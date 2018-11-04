// whisper.c

#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	string dest, msg;
	object ob;

	if( me->is_fighting() )
		return notify_fail("������ս���Щu\n");

	if( !arg || sscanf(arg, "%s %s", dest, msg)!=2 )
		return notify_fail("��Ҫ��˭����Щʲô�t\n");

	ob = present(dest, environment(me));
	if( !ob || !ob->is_character() )
		return notify_fail("��Ҫ��˭����t\n");

	if( ob->is_fighting() )
		return notify_fail( ob->name() + "����ս���Щu\n");

	write( GRN "����" + ob->name() + "�Ķ�������˵���s" + msg + "\n" NOR);
	tell_room( environment(me), me->name() + "��" + ob->name()
		+ "����С����˵��Щ����\n", ({ me, ob }) );
	if( !userp(ob) ) ob->relay_whisper(me, msg);
	else
		tell_object( ob, GRN + me->name() + "����Ķ�������˵���s" + msg + "\n" NOR);
	return 1;
}

int help(object me)
{
	write( @TEXT
ָ���ʽ�swhisper <ĳ��> <ѶϢ>

���ָ�����������ͬһ�����е��˶���o���� NPC ���ڡ�
TEXT
	);
	return 1;
}
