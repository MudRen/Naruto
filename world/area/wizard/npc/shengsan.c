#include <npc.h>

void create()
{
	set_name("��������", ({ "shengsan" }));
	set_race("human");
	set("age", 30);
	set("title", "�λ�����");
	set("long", "һλ�����λ�����Ĵ���\n");
	set("no_fight", 1);
	set("chat_chance", 3);
	set("chat_msg", ({
		(: random_move :),
	}));
	setup();
}