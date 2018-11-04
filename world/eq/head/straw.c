#include <ansi.h>
#include <armor.h>

inherit F_HEAD_EQ;

void create()
{
    set_name(GRN"��ñ"NOR, ({ "straw hat", "hat" }) );

    set_weight(500);

    setup_head_eq();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 100);
        set("long", "һ���ò�����֯���ɵĲ�ñ���������൱�ı��¡�");
        set("wear_as", "head_eq");
        set("apply_armor/head_eq", ([
            "armor": 1,
        ]));
    }

    setup();
}
