// gold.c

inherit MONEY;

void create()
{
	set_name("�ƽ�", ({"gold", "ingot", "gold_money"}));
	if( !clonep() ) {
		set("money_id", "gold");
		set("long", "�ƳγεĽ��өo�˼��˰��Ľ��өo���������өu\n");
		set("unit", "Щ");
		set("material", "gold");
		set("base_value", 1000);
		set("base_unit", "��");
		set("base_weight", 10);
	}
	set_amount(1);
	setup();
}

