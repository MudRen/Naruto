#include <ansi.h>
#include <weapon.h>

inherit F_WEAPON;

void create()
{
    set_name("�̽�", ({ "short sword", "sword" }));
    set_weight(3500);
    setup_weapon(10, 14, 100, 1);
    
    if( !clonep() ) {
        set("wield_as", ({ "righthand", "lefthand" }) );
        set("unit", "��");
        set("value", 2424);
        set("long", "һ�ѷ����Ķ̽���\n" );
    }

    setup();
}
