#include <ansi.h>
#include <armor.h>

inherit F_HEAD_EQ;

void create()
{
    set_name(HIK"����"NOR, ({ "boiler" }) );

    set_weight(2000);

    setup_head_eq();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 400);
        set("long", "һ���������ܼ�̵Ĺ��ӣ����˻��а��֡�");
        set("wear_as", "head_eq");
        set("apply_armor/head_eq", ([
            "armor": 4,
        ]));
    }

    setup();
}
