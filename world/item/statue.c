/* statue.c write by -Acme-
*/

#include <login.h>
#include <ansi.h>

inherit ITEM;

void create()
{
    set_name(HIW"����"NOR, ({ "statue"}));
    set_weight(3);
    if( !clonep() ) {
        set("unit", "��");
        set("value", 0);
        set("no_get", 1);
        set("long", "һ����·�����ĵ��񣬲�֪Ϊ�λ����������ط�������Է��ֵ���\n"
                    "����ǳ��⻬�����񳣳�����ȥ����(touch)�����ӡ�");
    }
    setup();
}

void init()
{
    add_action("do_touch", "touch");
}

int do_touch(string arg)
{
    object room;

    if( environment(this_object())->is_area() )
        if( !area_environment(this_object(), this_player()) ) return 0;

    if( this_player()->is_busy() || this_player()->is_fighting() ) 
        return notify_fail("������û�пա�\n");

    if( !arg ) return notify_fail("���봥��ʲô������\n");

    if( !this_object()->id(arg) ) return notify_fail("���봥��ʲô������\n");

    if( !objectp(room = load_object(START_ROOM)) ) {
        message_vision("$N������$n�����˼��Σ�������������������Ц��...\n", this_player(), this_object());
        return 1;
    }

    message_vision("$N������$n�����˼��Σ�������������������Ц������������ʧ�ˡ�\n", this_player(), this_object());

    this_player()->move(room);
    
    message_vision("$NͻȻ�����ϵ�������...\n", this_player());

    this_player()->start_busy(3);

    return 1;
}