#include <ansi.h>
#include <armor.h>

inherit F_CLOTH;

void create()
{
    set_name("��֯��", ({ "knit cloth", "cloth" }) );

    set_weight(1200);

    setup_cloth();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 500);
        set("long", "һ�����ֹ���֯���·������ʹ���\n");
        set("wear_as", "cloth");
        set("apply_armor/cloth", ([
            "armor": 5,
        ]));
    }
    setup();
}

