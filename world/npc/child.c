// write by -Acme-

#include <npc.h>

inherit F_FIGHTER;

void create()
{
    set_name("С��", ({ "child" }));
    set_race("human");
    set_level(3);
    set("age", 6+random(10));
    set("long", "һ�����������С������֪�������ȫ�������⡣\n");

    // ����ƶ�
    set("chat_chance", 1);
    set("chat_msg", ({
        (: random_move :),
    }));

    setup();

    // ����
    set_attr("str", 12);
    set_attr("con", 12);
    set_attr("dex", 12);
    set_attr("int", 12);
    set_temp("apply/attack", -5);         // ��������
    set_temp("apply/defend", -5);         // ��������
    set_temp("apply/exact", -5);          // ������
    set_temp("apply/evade", -5);          // �ر���
    set_temp("apply/intimidate", -5);     // ��������
    set_temp("apply/wittiness", -5);      // ��������

    carry_money("silver", 1);
}