#include <ansi.h>
#include <armor.h>

inherit F_HEAD_EQ;

void create()
{
    set_name(YEL"���ñ"NOR, ({ "shit hat", "hat" }) );

    set_weight(3000);

    setup_head_eq();

    if( !clonep() ) {
        set("unit", "��");
        set("value", 700);
        set("long", "һ��������͵�ñ�ӣ��ƺ���������Ц�õġ�");
        set("wear_as", "head_eq");
        set("apply_armor/head_eq", ([
            "int": 1,
        ]));
    }

    setup();
}
