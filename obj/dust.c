// dust.c

inherit COMBINED_ITEM;

void create()
{
	set_name("��ʬ��", ({"dust"}));
	set("long",
		"����һ��������ʬ�𼣵Ļ�ʬ�۩oֻҪ��һ����Ϳ���\n"
		"��ȥ(dissolve)һ��ʬ��u\n");
	set("unit", "��");
	set("base_unit", "��");
	set("base_weight", 1);
	set_amount(1);
}

void init()
{
	if( this_player()==environment() )
		add_action("do_dissolve", "dissolve");
}

int do_dissolve(string arg)
{
	object ob;

	if( !arg ) return notify_fail("��Ҫ�û�ʬ���ܽ�ʲô�����t\n");
	if( !objectp(ob = present(arg, environment(this_player()))) )
		return notify_fail("����û������������\n");
	if( living(ob) || !ob->id("corpse") )
		return notify_fail("��ʬ��ֻ�������ܽ�ʬ�塣\n");
	message_vision(
		"$N��ָ������һ�㻯ʬ����$n�ϩoֻ����һ�����͡��������\n"
		"һ�ɿ��µĶ���o$nֻʣ��һ̲��ˮ��\n", this_player(), ob);
	destruct(ob);
	add_amount(-1);
	return 1;
}

