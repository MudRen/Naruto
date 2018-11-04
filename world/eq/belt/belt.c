#include <ansi.h>
#include <armor.h>

inherit F_WAIST_EQ;

void create()
{
    set_name("Ƥ��", ({ "leather belt", "belt" }) );
    set_weight(1000);
    setup_waist_eq();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 200);
        set("long", "һ��ϵ�������ÿ��Ӳ������Ƥ����\n");
        set("wear_as", "waist_eq");
        set("apply_armor/waist_eq", ([
            "armor": 2,
        ]));
    }

    setup();
}
