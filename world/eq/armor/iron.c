#include <ansi.h>
#include <armor.h>
inherit F_ARMOR;

void create()
{
    set_name(HIK"���ؼ�"NOR, ({ "iron armor", "armor" }) );

    set_weight(4000);
    setup_armor();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 500);
        set("long", "һ������������ؼף��ṩ�����ı�����\n");
        set("wear_as", "armor");
        set("apply_armor/armor", ([
            "armor": 5,
        ]));
    }

    setup();
}

