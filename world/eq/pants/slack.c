#include <ansi.h>
#include <armor.h>

inherit F_PANTS;

void create()
{
    set_name("����", ({ "slack" }) );

    set_weight(1500);
    setup_pants();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 200);
        set("long", "һ������ͨ�ĳ��㡣\n");
        set("wear_as", "pants");
        set("apply_armor/pants", ([
            "armor": 2,
        ]));
    }

    setup();
}
