#include <ansi.h>
#include <armor.h>

inherit F_WRIST_EQ;

void create()
{
    set_name(GRN"���ֻ�"NOR, ({ "jade wrist", "wrist" }) );
    set_weight(100);
    setup_wrist_eq();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 1600);
        set("long", "һ��ʮ�ֱ��µ����ֻ�����ͷ����·�ܹ���\n");
        set("wear_as", "wrist_eq");
        set("apply_armor/wrist_eq", ([
            "armor": 2,
            "con": 2
        ]));
    }

    setup();
}
