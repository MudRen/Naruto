//traveller.c

#include <npc.h>

inherit F_VILLAGER;

void create()
{
    set_name("��·��", ({ "traveller" }) );
    set("unit", "λ");
    set("gender", "male");
    set_level(6);
    set("age", 18+random(60));
    set("long", "һλ��ɫ�Ҵң�������˪�Ĺ�·�ͣ�һ��װ���ƺ���ʱ��Ҫ��·ȥ��\n");
    set("chat_chance", 1);
    set("chat_msg", ({
        "��·�ʹ��˼����������� ��... ���ۣ���Զ��\n",
        "��·���ƺ��۵Ľ����������ˡ�\n",
        (: command,"sweat":),
        (: random_move :),
    }));
    setup();
    carry_money("silver", 10);
    carry_object(_DIR_EQ_"cloth/cloth.c")->wear();
}

int accept_fight(object ob)
{
    do_chat("��·�Ͳ��˲������ϵĺ�ˮЦ��������ɶ�������۷��˻��򣡡�\n");
    return 0;
}