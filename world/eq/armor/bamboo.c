#include <ansi.h>
#include <armor.h>
inherit F_ARMOR;

void create()
{
    set_name(GRN"竹甲"NOR, ({ "bamboo armor", "armor" }) );

    set_weight(3000);
    setup_armor();

    if( !clonep() ) {
        set("unit", "件");
        set("value", 300);
        set("long", "一件竹子制成的铠甲，好像没什么防护的功\能。\n");
        set("wear_as", "armor");
        set("apply_armor/armor", ([
            "armor": 3,
        ]));
    }

    setup();
}

