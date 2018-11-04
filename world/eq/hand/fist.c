#include <ansi.h>
#include <armor.h>
inherit F_HAND_EQ;

void create()
{
    set_name(HIR"拳击手套"NOR, ({ "fist gloves","gloves" }) );

    set_weight(1500);
    setup_hand_eq();

    if( !clonep() ) {
        set("unit", "双");
        set("value", 7500);
        set("long", "一个打拳击时所用的红色手套。\n");
        set("wear_as", "hand_eq");
        set("apply_armor/hand_eq", ([
            "attack" : 5,
        ]));
    }

    setup();
}
