// bag.c

inherit ITEM;

void create()
{
	set_name("麻布袋", ({ "bag" }));
	set_weight(700);
	set_max_encumbrance(30000);
	if( !clonep() ) {
		set("unit", "只");
		set("value", 30);
		set("long", "一只麻布口袋，看来可以装不少东西。\n");
	}
	setup();
}

int accept_object() { return 1; }
