#include <ansi.h>
#include <armor.h>

inherit F_LEG_EQ;

void create()
{
    set_name("����", ({ "puttee" }) );

    set_weight(800);
    setup_leg_eq();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 100);
        set("long", "���Ƶİ��ȣ��ṩһ��ķ�����\n");
        set("wear_as", "leg_eq");
        set("apply_armor/leg_eq", ([
            "armor": 1,
        ]));
    }

    setup();
}

