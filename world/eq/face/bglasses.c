#include <ansi.h>
#include <armor.h>
inherit F_FACE_EQ;

void create()
{
    set_name(HIK"ī��"NOR, ({ "black glasses", "glasses" }) );
    set_weight(500);
    setup_face_eq();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 450);
        set("long", "һ����Ƭ�൱�ڵ�ī������������֪�����������õ�·��\n");
        set("wear_as", "face_eq");
        set("apply_armor/face_eq", ([
            "exact": 3,
        ]));
    }
    setup();
}
