#include <ansi.h>
#include <armor.h>

inherit F_HEAD_EQ;

void create()
{
    set_name(HIK"Ϭţ"NOR+YEL"���ñ"NOR, ({ "rhino shit hat", "hat" }) );

    set_weight(3300);

    setup_head_eq();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 3700);
        set("long", "һ��Ϭţ���ͼ��ϴ�����͵�ñ�ӣ������Ц�ֻ������档");
        set("wear_as", "head_eq");
        set("apply_armor/head_eq", ([
            "armor": 6,
            "int": 1,
        ]));
    }

    setup();
}
