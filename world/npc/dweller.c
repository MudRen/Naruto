// man.c

#include <npc.h>

inherit F_VILLAGER;

void create()
{
    set_name("����", ({ "dweller" }) );
    set_race("human");
    set_level(7);
    set("age", random(50)+18);
    set("long", "һ����ס���⸽���ľ��񣬿���������ͨͨ��ûʲô�ر�\n");

    // ����ƶ�
    set("chat_chance", 1);
    set("chat_msg", ({
        (: random_move :),
    }));
    setup();

    carry_money("silver", 8);
    carry_object(_DIR_WP_"radish.c")->wield();
    carry_object(_DIR_EQ_"cloth/cloth.c")->wear();
}
