#include <ansi.h>
#include <weapon.h>

inherit F_WEAPON;

void create()
{
    set_name(YEL"��"NOR, ({ "big axe", "axe" }));
    set_weight(8000);
    setup_weapon(30, 40, 100, 1);
    
    if( !clonep() ) {
        set("wield_as", ({ "righthand", "twohanded" }) );
        set("unit", "��");
        set("value", 7070);
        set("long", "һ���ƻ����൱���˵ĸ�ͷ��\n" );
    }

    setup();
}
