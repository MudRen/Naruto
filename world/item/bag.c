// bag.c

inherit ITEM;

void create()
{
	set_name("�鲼��", ({ "bag" }));
	set_weight(700);
	set_max_encumbrance(30000);
	if( !clonep() ) {
		set("unit", "ֻ");
		set("value", 30);
		set("long", "һֻ�鲼�ڴ�����������װ���ٶ�����\n");
	}
	setup();
}

int accept_object() { return 1; }
