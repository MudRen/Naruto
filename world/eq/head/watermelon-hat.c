#include <ansi.h>
#include <armor.h>

inherit F_HEAD_EQ;

void create()
{
    set_name(HIK"����Ƥ"NOR, ({ "watermelon hat", "hat" }) );

    set_weight(3300);

    setup_head_eq();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 3700);
        set("long", "һ������Ƥ��Ϳ�ɺ�ɫ���������ٷ��ã���ͷ���������ӣ���������ǿ��Ҳ��������");
        set("wear_as", "head_eq");
        set("apply_armor/head_eq", ([
            "armor": 6,
            "dex": 1,
        ]));
    }

    setup();
}

