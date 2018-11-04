#include <ansi.h>
#include <armor.h>
inherit F_FACE_EQ;

void create()
{
    set_name(GRN"蛙镜"NOR, ({ "frog glasses", "glasses" }) );

    set_weight(800);
    setup_face_eq();

    if( !clonep() ) {
        set("unit", "副");
        set("value", 800);
        set("long", "这是潜水时在用的蛙镜。\n");
        set("wear_as", "face_eq");
        set("apply_armor/face_eq", ([
        	"armor": 1,
            "dex": 1,
        ]));
    }
    setup();
}
