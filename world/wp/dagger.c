#include <ansi.h>
#include <weapon.h>

inherit F_WEAPON;

void create()
{
    set_name(HIK"����"NOR, ({ "dagger" }));
    set_weight(2500);
    setup_weapon(12, 12, 100, 1);
    
    if( !clonep() ) {
        set("wield_as", ({ "righthand", "lefthand" }) );
        set("unit", "��");
        set("value", 2424);
        set("long", "һ������ר�õ�Ѱ���̾ߡ�\n" );
    }

    setup();
}
