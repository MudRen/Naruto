#include <ansi.h>
#include <weapon.h>

inherit F_WEAPON;

void create()
{
    set_name(YEL"�ܲ�"NOR, ({ "radish" }));
    set_weight(2000);
    setup_weapon(1, 3, 100, 1);
    
    if( !clonep() ) {
        set("wield_as", ({ "righthand", "lefthand" }) );
        set("unit", "��");
        set("value", 404);
        set("long", "һ��Ǭ�Ƶ��ܲ���������Ӳ���ģ�Ӧ�ÿ����������ˡ�\n" );
    }

    setup();
}
