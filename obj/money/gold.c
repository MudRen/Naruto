// gold.c

inherit MONEY;

void create()
{
	set_name("黄金", ({"gold", "ingot", "gold_money"}));
	if( !clonep() ) {
		set("money_id", "gold");
		set("long", "黄澄澄的金子﹐人见人爱的金子﹐啊～～金子﹗\n");
		set("unit", "些");
		set("material", "gold");
		set("base_value", 1000);
		set("base_unit", "两");
		set("base_weight", 10);
	}
	set_amount(1);
	setup();
}

