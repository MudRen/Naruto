// bottle.c
// This is the DEFAULT container for clone command to contain cloned liquid.

inherit ITEM;

void create()
{
	set_name("����ƿ", ({ "glass bottle", "bottle" }));
	set_weight(700);
	set_max_encumbrance(3000);
	if( !clonep() ) {
		set("unit", "��");
                set("value",100);
		set("container_unit", "ƿ");
		set("long", "һ��͸���Ĳ���ƿ��\n");
		set("liquid_container", 1);
	}
	setup();
}

int accept_object(object me, object ob)
{
	if( ob->weight() > 500
	&&	!function_exists("set_volume", ob) )
		return notify_fail("����ƿ��̫С�˩oװ����" + ob->name() + "��\n");

	return 1;
}
