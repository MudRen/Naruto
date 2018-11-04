// vagabond.c

#include <npc.h>

inherit F_BANDIT;

void create()
{
    set_name("�о�����", ({ "vagabond" }));
    set_race("human");
    set_level(8);

    set("age", 24+random(20));
    set("long", "һ���������࣬���ж�״������������ñ���̫����\n");
    set("chat_chance", 6);
    set("chat_msg", ({
        "���������ٺ١��ع�Ц����һ�߲�������ض����㡣\n",
        "������װ���������µ����Ӱ�������ߣ�Ȼ����䲻���ײ��һ�¡�\n",
        "�������úܶ��ĵķ�ʽ���˿ٱ�ʺ��Ȼ���������һ�� ... ��㣡\n",
    }));

    setup();
    carry_object(_DIR_WP_"radish.c")->wield();
    carry_object(_DIR_WP_"radish.c")->wield("lefthand");
    carry_object(_DIR_EQ_"belt/cord.c")->wear();
    carry_object(_DIR_EQ_"cloth/cloth.c")->wear();
    carry_money("silver", 20);
}