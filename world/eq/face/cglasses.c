#include <ansi.h>
#include <armor.h>
inherit F_FACE_EQ;

void create()
{
    set_name(BWHT+BLK"д��"NOR+HIW"�۾�"NOR, ({ "copy glasses", "glasses" }) );

    set_weight(800);
    setup_face_eq();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 1500);
        set("long", "һ����Ƭ��ͷ����д�������ӵ��۾����������۵е�ս����\n");
        set("wear_as", "face_eq");
        set("apply_armor/face_eq", ([
        	"armor": 1,
            "str": 2,
        ]));
    }
    setup();
}
