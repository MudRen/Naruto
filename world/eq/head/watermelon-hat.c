#include <ansi.h>
#include <armor.h>

inherit F_HEAD_EQ;

void create()
{
    set_name(HIK"西瓜皮"NOR, ({ "watermelon hat", "hat" }) );

    set_weight(3300);

    setup_head_eq();

    if( !clonep() ) {
        set("unit", "顶");
        set("value", 3700);
        set("long", "一顶西瓜皮，涂成黑色的拿来当假发用，里头还有著夹子，让你遇到强风也吹不掉。");
        set("wear_as", "head_eq");
        set("apply_armor/head_eq", ([
            "armor": 6,
            "dex": 1,
        ]));
    }

    setup();
}

