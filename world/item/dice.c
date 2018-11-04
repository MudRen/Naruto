// dice.c

inherit ITEM;

void create()
{
	set_name("����", ({ "dice"}));
	set_weight(3);
	if( !clonep() ) {
		set("unit", "��");
		set("value", 5);
		set("long",
			"һ�����ӣ�������� roll �������档\n");
	}
	setup();
}

void init()
{
	add_action("do_roll", ({ "roll", "throw" }));
}

int do_roll(string arg)
{
	if( !id(arg) ) return 0;

    if( environment(this_player())->is_area() )
    	if( !area_environment(this_player(), this_object()) ) return 0;

	message_vision("$N�ó�����һ�������ӹ��˹�ͣ����������Ǹ�" + (random(6)+1) + "�㡣\n", this_player());
	return 1;
}
