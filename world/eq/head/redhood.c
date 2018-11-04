#include <ansi.h>
#include <armor.h>

inherit F_HEAD_EQ;

void create()
{
    set_name(HIR"��ɫͷ��"NOR, ({ "red hood", "hood" }) );

    set_weight(500);

    setup_head_eq();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 450);
        set("long", "һ���൱�и��˷���ͷ��");
        set("wear_as", "head_eq");
        set("apply_armor/head_eq", ([
            "exact": 3,
        ]));
    }

    setup();
}
