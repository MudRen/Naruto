#include <ansi.h>
#include <armor.h>
inherit F_FACE_EQ;

void create()
{
    set_name(GRN"�ܾ�"NOR, ({ "frog glasses", "glasses" }) );

    set_weight(800);
    setup_face_eq();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 800);
        set("long", "����Ǳˮʱ���õ��ܾ���\n");
        set("wear_as", "face_eq");
        set("apply_armor/face_eq", ([
        	"armor": 1,
            "dex": 1,
        ]));
    }
    setup();
}
