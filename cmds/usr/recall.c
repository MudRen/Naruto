/* recall.c */

#include <ansi.h>
#include <login.h>

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    if( !objectp(me) ) return 0;

    if( me->is_fighting() )
        return notify_fail("�㻹��ս���У��޷�ʹ�� recall ָ�\n");

    if( me->busy() )
        return notify_fail("����æ�����޷�ʹ�� recall ָ�\n");

    if( environment(me) && environment(me)->query("no_recall") )
        return notify_fail("�����ڵĵط��޷�ʹ�� recall ָ�\n");

    if( me->query_temp("no_recall") )
        return notify_fail("�������޷�ʹ�� recall ָ�\n");

    message_vision(HIM"$N�����۾����������в�С��(Jolin)�����ţ������ر�һ���߲������ס�ˡ�\n"NOR, me);

    me->start_busy(1);

    call_out("do_move", 2, me, arg);

    return 1;
}

int do_move(object me, string arg)
{
    string tmp;
    object room;

    if( arg == "start" ) tmp = START_ROOM;
    else {
    	if( !(tmp = me->query("recoverroom")) ) tmp = START_ROOM;
    }

    if( !objectp(room=load_object(tmp)) ) 
        if( !objectp(room=load_object(START_ROOM)) )
            tell_object(me, "ͻȻʱ�շ���һ����ң����޷��ƶ���\n");

    message_vision(HIG"ֻ��$N"HIG"�����廯��ƬƬѩ������ʧ�ڰ����....\n"NOR,me);

    me->move(room);

    message_vision(HIW"һ����紵���������$N����Ӱ����������ͩu\n"NOR,me);

    return 1;
}

int help(object me)
{
    write(@HELP
ָ���ʽ�s recall [start]

���ָ���������Ѹ�ٷ��������㡣���Ӳ��� start ���ص���ʦ��
HELP);
    return 1;
}