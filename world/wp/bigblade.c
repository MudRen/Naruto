#include <ansi.h>
#include <weapon.h>

inherit F_WEAPON;

void create()
{
    set_name(HIW"��"NOR, ({ "big blade", "blade" }));
    set_weight(8000);
    setup_weapon(20, 30, 100, 1);
    
    if( !clonep() ) {
        set("wield_as", ({ "righthand", "twohanded" }) );
        set("unit", "��");
        set("value", 5050);
        set("long", "һ�Ѵ��͵ĵ�������ʮ����ƻ�����\n" );
    }

    setup();
}
