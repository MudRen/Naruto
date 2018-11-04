#include <ansi.h>
#include <weapon.h>

inherit F_WEAPON;

void create()
{
    set_name(HIW"长剑"NOR, ({ "long sword", "sword" }));
    set_weight(6000);
    setup_weapon(24, 26, 100, 1);
    
    if( !clonep() ) {
        set("wield_as", ({ "righthand", "lefthand" }) );
        set("unit", "把");
        set("value", 5050);
        set("long", "一把剑身颇长的剑，相当锐利。\n" );
    }

    setup();
}
