// vagabond.c

#include <npc.h>

inherit F_BANDIT;

void create()
{
    set_name("市井无赖", ({ "vagabond" }));
    set_race("human");
    set_level(8);

    set("age", 24+random(20));
    set("long", "一个满脸凶相，恶行恶状的无赖汉，最好别靠他太近。\n");
    set("chat_chance", 6);
    set("chat_msg", ({
        "无赖汉“嘿嘿”地怪笑著，一边不怀好意地盯著你。\n",
        "无赖汉装作若无其事的样子挨近你身边，然后出其不意地撞你一下。\n",
        "无赖汉用很恶心的方式抠了抠鼻屎，然后往你这边一弹 ... 快躲！\n",
    }));

    setup();
    carry_object(_DIR_WP_"radish.c")->wield();
    carry_object(_DIR_WP_"radish.c")->wield("lefthand");
    carry_object(_DIR_EQ_"belt/cord.c")->wear();
    carry_object(_DIR_EQ_"cloth/cloth.c")->wear();
    carry_money("silver", 20);
}