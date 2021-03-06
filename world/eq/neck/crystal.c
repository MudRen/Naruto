#include <ansi.h>
#include <armor.h>

inherit F_NECK_EQ;

void create()
{
    set_name(HIC"水晶"NOR"项炼", ({ "crystal necklace", "necklace" }) );
    set_weight(600);
    setup_neck_eq();

    if( !clonep() ) {
        set("unit", "串");
        set("value", 3000);
        set("long", "上头有一颗明亮清透的水晶，是不错的装饰品。\n");
        set("wear_as", "neck_eq");
        set("apply_armor/neck_eq", ([
            "hp": 60,
        ]));
    }

    setup();
}
