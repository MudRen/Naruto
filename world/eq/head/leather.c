#include <ansi.h>
#include <armor.h>

inherit F_HEAD_EQ;

void create()
{
    set_name("��Ƥñ", ({ "leather hat", "hat" }) );

    set_weight(2000);

    setup_head_eq();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 2500);
        set("long", "һ����֪��ʲôƤ���ɵ�ñ�ӣ��൱�ĺ�ʵ��ů��");
        set("wear_as", "head_eq");
        set("apply_armor/head_eq", ([
            "armor": 5,
        ]));
    }

    setup();
}
