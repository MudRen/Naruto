#include <ansi.h>
#include <weapon.h>

inherit F_WEAPON;

void create()
{
    set_name("����", ({ "blade" }));
    set_weight(5000);
    setup_weapon(14, 18, 100, 1);
    
    if( !clonep() ) {
        set("wield_as", ({ "righthand", "lefthand" }) );
        set("unit", "��");
        set("value", 3232);
        set("long", "һ�ѻ��������϶���������ĵ�����\n" );
    }

    setup();
}
