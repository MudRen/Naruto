#include <ansi.h>
#include <armor.h>

inherit F_PANTS;

void create()
{
    set_name(HIR"�ȿ�"NOR, ({ "hot short", "short" }) );
    set_weight(800);
    setup_pants();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 100);
        set("long", "һ�����̵��ȿ㣬�������൱�л�����\n");
        set("wear_as", "pants");
        set("apply_armor/pants", ([
            "armor": 1,
        ]));
    }

    setup();
}
