#include <ansi.h>
#include <weapon.h>

inherit F_WEAPON;

void create()
{
    set_name(GRN"����"NOR, ({ "rattan" }));
    set_weight(2000);
    setup_weapon(4, 8, 100, 1);
    
    if( !clonep() ) {
        set("wield_as", ({ "righthand", "lefthand" }) );
        set("unit", "��");
        set("value", 1212);
        set("long", "һ��������������Ӧ�ÿ����������ˡ�\n" );
    }

    setup();
}
