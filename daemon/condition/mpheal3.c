/* mpheal3.c 
   ״̬ hp ֮�ָ�Ч�ܣ��ָ����ǹ̶�ֵ

   ���Ͻṹ
   ([ "duration": ʱЧ ])
*/
#include <ansi.h>

inherit CONDITION;

private void create()
{
    seteuid(getuid());
    DAEMON_D->register_condition_daemon("mpheal3");
}

void condition_update(object me, string cnd, mixed cnd_data)
{
    if( !mapp(cnd_data) || (cnd_data["duration"]--) < 1 ) {
        me->delete_condition(cnd);
        return;
    }

    // �ָ�MPֵ
    me->heal_stat("mp", 30);
}


void food_condition_apply(object me, mapping cnd_data)
{
    mapping mpheal;
    
    if( !mapp(mpheal = me->query_condition("mpheal3")) )
        mpheal = ([]);
    
    mpheal["duration"] += cnd_data["duration"];
    mpheal["name"] = BMAG"���������"NOR;
    
    if( mpheal["duration"] <= 0 ) mpheal["duration"] = 1;
    else if( mpheal["duration"] > 160 ) mpheal["duration"] = 160;
    
    me->set_condition("mpheal3", mpheal);
}

/*
void condition_apply(object me, string cnd, mapping cnd_data) { }
void condition_unapply(object me, string cnd, mapping cnd_data) { }
*/