#include <ansi.h>
#include <weapon.h>

inherit F_WEAPON;

void create()
{
    set_name(HIW"����"NOR, ({ "long sword", "sword" }));
    set_weight(6000);
    setup_weapon(24, 26, 100, 1);
    
    if( !clonep() ) {
        set("wield_as", ({ "righthand", "lefthand" }) );
        set("unit", "��");
        set("value", 5050);
        set("long", "һ�ѽ����ĳ��Ľ����൱������\n" );
    }

    setup();
}
