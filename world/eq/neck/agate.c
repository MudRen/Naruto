#include <ansi.h>
#include <armor.h>

inherit F_NECK_EQ;

void create()
{
    set_name(HIG"���"NOR"����", ({ "agate necklace", "necklace" }) );
    set_weight(600);
    setup_neck_eq();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 5000);
        set("long", "һ������������������ص�������\n");
        set("wear_as", "neck_eq");
        set("apply_armor/neck_eq", ([
            "hp": 100,
        ]));
    }

    setup();
}
