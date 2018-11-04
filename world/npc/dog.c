/* dog.c
   write by -Acme-
*/

#include <npc.h>

void create()
{
    set_name("Ұ��", ({ "dog" }) );
    set_race("beast");
    set_level(1);

    set("long", "һֻ�������Ұ������ǻ�������ˮ��\n");

    set("limbs", ({ "��ͷ", "����", "����", "����", "β��" }) );
    set("verbs", ({ "bite" }) );

    // ����ƶ�
    set("chat_chance", 1);
    set("chat_msg", ({
        (: random_move :),
    }));

    setup();
    
    // ����
    set_attr("str", 5);
    set_attr("con", 5);
    set_attr("dex", 5);
    set_attr("int", 5);
    set_temp("apply/attack", -10);         // ��������
    set_temp("apply/defend", -10);         // ��������
    set_temp("apply/exact", -10);          // ������
    set_temp("apply/evade", -10);          // �ر���
    set_temp("apply/intimidate", -10);     // ��������
    set_temp("apply/wittiness", -10);      // ��������
}