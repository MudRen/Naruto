// write by -Acme-

#include <npc.h>

inherit F_VILLAGER;

void create()
{
    set_name("·��", ({ "passerby" }));
    set_race("human");
    set_level(5);
    set("age", random(50)+18);
    set("long", "һλ����ͨ��·�ˣ�һ����·��Ҳ����������ʮ����\n");
    // ����ƶ�
    set("chat_chance", 1);
    set("chat_msg", ({
        (: random_move :),
    }));
    setup();
    carry_money("silver", 5);
    carry_object(_DIR_EQ_"cloth/cloth.c")->wear();
}