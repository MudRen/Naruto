#include <ansi.h>
#include <armor.h>
inherit F_ARMOR;

void create()
{
    set_name(GRN"���"NOR, ({ "bamboo armor", "armor" }) );

    set_weight(3000);
    setup_armor();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 300);
        set("long", "һ�������Ƴɵ����ף�����ûʲô�����Ĺ�\�ܡ�\n");
        set("wear_as", "armor");
        set("apply_armor/armor", ([
            "armor": 3,
        ]));
    }

    setup();
}

