#include <ansi.h>
#include <armor.h>
inherit F_HAND_EQ;

void create()
{
    set_name(HIR"ȭ������"NOR, ({ "fist gloves","gloves" }) );

    set_weight(1500);
    setup_hand_eq();

    if( !clonep() ) {
        set("unit", "˫");
        set("value", 7500);
        set("long", "һ����ȭ��ʱ���õĺ�ɫ���ס�\n");
        set("wear_as", "hand_eq");
        set("apply_armor/hand_eq", ([
            "attack" : 5,
        ]));
    }

    setup();
}
