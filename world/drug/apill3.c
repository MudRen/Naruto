inherit F_FOOD;

void create()
{
	set_name("��������", ({ "big ap pill", "pill" }));
	if( !clonep() ) {
		set("unit", "��");
		set("base_unit", "��");
		set("base_value", 200);            // ���ڵ�ҩ������Ҫ̫������������
		set("base_weight", 10);
		set("food_stuff", 10);             // ʳ������ʳƷһ��Ҫ��ʳ����
		set("long", "һ�ų��˿��Լӿ�ָ����������\n");
		set("condition", ([ "apheal3": ([ "duration": 10 ]) ]) );
	}
	setup();
}
