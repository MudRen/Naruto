#include <ansi.h>
#include <armor.h>

inherit F_NECK_EQ;

void create()
{
    set_name(HIW"����"NOR"����", ({ "pearl necklace", "necklace" }) );
    set_weight(600);
    setup_neck_eq();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 1500);
        set("long", "��һ�ſŵ����鴮�������������൱�ĸ��š�\n");
        set("wear_as", "neck_eq");
        set("apply_armor/neck_eq", ([
            "hp": 30,
        ]));
    }

    setup();
}
