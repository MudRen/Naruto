#include <npc.h>

void create()
{
	set_name("��", ({ "tmr" }));
	set_race("human");
	set("age", 20);
	set("title", "��������");
	set("long", "һλ������������Ĵ���\n");
	set("no_fight", 1);
	set("chat_chance", 3);
	set("chat_msg", ({
		(: random_move :),
	}));
	setup();
}