#include <npc.h>

void create()
{
	set_name("��������", ({ "annihilator" }));
	set_race("human");
	set("title", "�쳯�۹�");
	set("age", 30);
	set("long", "һλ�����쳯�۹��Ĵ���\n");
	set("no_fight", 1);
	set("chat_chance", 3);
	set("chat_msg", ({
		(: random_move :),
	}));
	setup();
}