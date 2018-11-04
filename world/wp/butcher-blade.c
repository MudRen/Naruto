#include <ansi.h>
#include <weapon.h>

inherit F_WEAPON;

void create()
{
    set_name("ɱ��", ({ "butcher's blade" ,"blade" }));
    set_weight(1000);
    setup_weapon(10, 20, 100, 1);
    
    if( !clonep() ) {
        set("wield_as", ({ "twohanded" }) );
        set("unit", "��");
        set("value", 5815);
        set("long", "һ����ͨ����������ĵ�����˵��һλ��̽�������칦�����ٿ�����ܽ�ǹ�͡�\n" );
    }

    set("apply_weapon/twohanded", 
        ([ "str" : 3,            // ���ԣ�����
           "attack" : 2,         // ��������
           "armor" : 2,        // ��������
    ]) );

    setup();
}

