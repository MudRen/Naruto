#include <ansi.h>
#include <armor.h>

inherit F_FEET_EQ;

void create()
{
    set_name("�ײ�Ь", ({ "white cloth shoes", "shoes" }) );

    set_weight(1200);

    setup_feet_eq();

    if( !clonep() ) {
        set("unit", "˫");
        set("value", 1600);
        set("long", "��˵�еġ�bad boy����˵������Ь�ӣ������á�\n");
        set("wear_as", "feet_eq");
        set("apply_armor/feet_eq", ([
            "armor": 2,
            "int" : 2,
        ]));
    }

    setup();
}
