#include <ansi.h>
#include <armor.h>

inherit F_CLOTH;

void create()
{
    set_name("ë��", ({ "sweater" }) );

    set_weight(1500);
    setup_cloth();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 200);
        set("long", "һ��Ѱ����ë�£��������ͺܱ�ů��\n");
        set("wear_as", "cloth");
        set("apply_armor/cloth", ([
            "armor": 2,
        ]));
    }
    setup();
}

