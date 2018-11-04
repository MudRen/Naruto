#include <ansi.h>
#include <armor.h>

inherit F_LEG_EQ;

void create()
{
    set_name(HIG"°¢¸Ç\"NOR, ({ "gai" }) );

    set_weight(1500);
    setup_leg_eq();

    if( !clonep() ) {
        set("unit", "ãû");
        set("value", 15000);
        set("long", "ÌìÌì´©°¢¸Ç£¬ÄãÓĞ½¡¿µµÄÏ¥¸Ç¡£\n");
        set("wear_as", "leg_eq");
        set("apply_armor/leg_eq", ([
            "armor": 5,
            "con": 5,
        ]));
    }

    setup();
}