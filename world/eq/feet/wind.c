#include <ansi.h>
#include <armor.h>

inherit F_FEET_EQ;

void create()
{
    set_name(HIW"����ѥ"NOR, ({ "white cloth boots", "boots" }) );

    set_weight(1500);

    setup_feet_eq();

    if( !clonep() ) {
        set("unit", "˫");
        set("value", 7000);
        set("long", "�������ƶ��ٶ����ӵ�ѥ�ӡ�\n");
        set("wear_as", "feet_eq");
        set("apply_armor/feet_eq", ([
            "move": 10,
        ]));
    }

    setup();
}
