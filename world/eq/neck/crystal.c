#include <ansi.h>
#include <armor.h>

inherit F_NECK_EQ;

void create()
{
    set_name(HIC"ˮ��"NOR"����", ({ "crystal necklace", "necklace" }) );
    set_weight(600);
    setup_neck_eq();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 3000);
        set("long", "��ͷ��һ��������͸��ˮ�����ǲ����װ��Ʒ��\n");
        set("wear_as", "neck_eq");
        set("apply_armor/neck_eq", ([
            "hp": 60,
        ]));
    }

    setup();
}
