#include <ansi.h>
#include <armor.h>

inherit F_CLOTH;

void create()
{
    set_name("����", ({ "cloth" }) );

    set_weight(1500);
    setup_cloth();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 100);
        set("long", "һ��Ѱ���Ĳ��¡�\n");
        set("wear_as", "cloth");
        set("apply_armor/cloth", ([
            "armor": 1,
        ]));
    }
    setup();
}

