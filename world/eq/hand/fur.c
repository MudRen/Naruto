#include <ansi.h>
#include <armor.h>
inherit F_HAND_EQ;

void create()
{
    set_name(YEL"Ƥ������"NOR, ({ "fur gloves","gloves" }) );

    set_weight(1000);
    setup_hand_eq();

    if( !clonep() ) {
        set("unit", "˫");
        set("value", 200);
        set("long", "һ˫Ƥ�Ƶ����ף��������ܱ�ů��\n");
        set("wear_as", "hand_eq");
        set("apply_armor/hand_eq", ([
            "armor": 2,
        ]));
    }

    setup();
}
