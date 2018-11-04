#include <ansi.h>
#include <armor.h>

inherit F_HEAD_EQ;

void create()
{
    set_name("粗皮帽", ({ "leather hat", "hat" }) );

    set_weight(2000);

    setup_head_eq();

    if( !clonep() ) {
        set("unit", "顶");
        set("value", 2500);
        set("long", "一顶不知用什么皮做成的帽子，相当的厚实保暖。");
        set("wear_as", "head_eq");
        set("apply_armor/head_eq", ([
            "armor": 5,
        ]));
    }

    setup();
}
