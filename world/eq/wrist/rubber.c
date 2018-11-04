#include <ansi.h>
#include <armor.h>

inherit F_WRIST_EQ;

void create()
{
    set_name(YEL"��Ƥ��"NOR, ({ "rubber band", "band" }) );
    set_weight(100);
    setup_wrist_eq();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 3900);
        set("long", "һ���൱�ֵ���Ƥ�����������̫�����ƻỵ����\n");
        set("wear_as", "wrist_eq");
        set("apply_armor/wrist_eq", ([
            "str": 3,
        ]));
    }

    setup();
}
