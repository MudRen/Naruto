/* bigdog.c
   write by -Acme-
*/

#include <ansi.h>
#include <npc.h>

void create()
{
    set_name("��Ұ��", ({ "big dog", "dog" }) );
    set_race("beast");
    set_level(2);

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
    set_attr("str", 10);
    set_attr("con", 10);
    set_attr("dex", 10);
    set_attr("int", 10);
    set_temp("apply/attack", -5);         // ��������
    set_temp("apply/defend", -5);         // ��������
    set_temp("apply/exact", -5);          // ������
    set_temp("apply/evade", -5);          // �ر���
    set_temp("apply/intimidate", -5);     // ��������
    set_temp("apply/wittiness", -5);      // ��������
}