// silver.c

inherit MONEY;

void create()
{
	set_name("����", ({"silver", "silver_money"}));
	if( !clonep() ) {
		set("money_id", "silver");
		set("long", "�׻��������өo�˼��˰������ӡ�\n");
		set("unit", "Щ");
		set("material", "silver");
		set("base_value", 1);
		set("base_unit", "��");
		set("base_weight", 5);
	}
	set_amount(1);
	setup();
}

