inherit F_FOOD;

void create()
{
	set_name("����", ({ "ap pill", "pill" }));
	if( !clonep() ) {
		set("unit", "��");
		set("base_unit", "��");
		set("base_value", 10);             // ���ڵ�ҩ������Ҫ̫������������
		set("base_weight", 10);
		set("food_stuff", 5);              // ʳ������ʳƷһ��Ҫ��ʳ����
		set("long", "һ�ų��˿��Լӿ�ָ���ľ�������\n");
		set("condition", ([ "apheal1": ([ "duration": 10 ]) ]) );
	}
	setup();
}
