#include <ansi.h>
#include <armor.h>

inherit F_FEET_EQ;

void create()
{
    set_name(GRN"²ÝÐ¬"NOR, ({ "straw boots","boots" }) );

    set_weight(800);

    setup_feet_eq();

    if( !clonep() ) {
        set("unit", "Ë«");
        set("value", 100);
        set("long", "ÓÃÂé²Ý±à³ÉµÄÐ¬×Ó£¬ºÜÑô´º¡£\n");
        set("wear_as", "feet_eq");
        set("apply_armor/feet_eq", ([
            "armor": 1,
        ]));
    }

    setup();
}
