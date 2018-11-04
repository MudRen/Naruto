#include <ansi.h>
#include <armor.h>

inherit F_NECK_EQ;

void create()
{
    set_name(RED"�����"NOR, ({ "amulet" }) );
    set_weight(600);
    setup_neck_eq();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 450);
        set("long", "һ�����������Ļ�������ܹ��������ƽ����\n");
        set("wear_as", "neck_eq");
        set("apply_armor/neck_eq", ([
            "evade": 3,
        ]));
    }

    setup();
}
