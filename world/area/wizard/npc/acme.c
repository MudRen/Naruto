#include <npc.h>

void create()
{
	set_name("��Ӱ", ({ "acme" }));
	set_race("human");
	set("age", 20);
	set("title", "��Ӱ����");
	set("long", "һλ���Ի�Ӱ���ߵĴ���\n");
	set("no_fight", 1);
	set("chat_chance", 3);
	set("chat_msg", ({
		(: random_move :),
	}));
	setup();
}