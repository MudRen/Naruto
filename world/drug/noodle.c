inherit F_FOOD;

void create()
{
	set_name("�������", ({ "noodles" }));
	if( !clonep() ) {
		set("unit", "��");
		set("base_unit", "��");
		set("base_value", 30);             // ���ڵ�ҩ������Ҫ̫������������
		set("base_weight", 10);
		set("food_stuff", 20);              // ʳ������ʳƷһ��Ҫ��ʳ����
		set("long", "һ��������㻹���ٳԵĺ��棡\n");
		set("condition", ([ "apheal1": ([ "duration": 10 ]),
			                "hpheal1": ([ "duration": 10 ]),
			                "mpheal1": ([ "duration": 10 ]),
		                 ]));
	}
	setup();
}
