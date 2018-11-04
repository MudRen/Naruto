// write by -Acme-

#include <npc.h>

inherit F_VILLAGER;

void create()
{
    set_name("��ؤ", ({ "beggar" }));
    set_race("human");
    set_level(5);
    set("title", "ؤ��");
    set("nickname", "�л���");
    set("age", random(50)+10);
    set("long", "һλ��������������ؤ����ͷ���治ʱ��ɢ����ζ����������\n");
    // ����ƶ�
    set("chat_chance", 1);
    set("chat_msg", ({
        (: random_move :),
    }));
    setup();

    // ����
    set_attr("str", 15);
    set_attr("con", 15);
    set_attr("dex", 15);
    set_attr("int", 15);
    set_temp("apply/attack", -3);         // ��������
    set_temp("apply/defend", -3);         // ��������
    set_temp("apply/exact", -3);          // ������
    set_temp("apply/evade", -3);          // �ر���
    set_temp("apply/intimidate", -3);     // ��������
    set_temp("apply/wittiness", -3);      // ��������
}