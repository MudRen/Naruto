#include <ansi.h>
#include <weapon.h>

inherit F_WEAPON;

void create()
{
    set_name(HIR"˫�и�"NOR, ({ "double axe", "axe" }));
    set_weight(10000);
    setup_weapon(40, 45, 100, 1);
    
    if( !clonep() ) {
        set("wield_as", ({ "twohanded" }) );
        set("unit", "��");
        set("value", 4242);
        set("long", "һ�����˽������еľ��͸�ͷ��\n" );
    }

    setup();
}
