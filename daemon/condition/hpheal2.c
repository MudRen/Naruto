/* hpheal2.c 
   ״̬ hp ֮�ָ�Ч�ܣ��ָ����ǹ̶�ֵ

   ���Ͻṹ
   ([ "duration": ʱЧ ])
*/
#include <ansi.h>

inherit CONDITION;

private void create()
{
    seteuid(getuid());
    DAEMON_D->register_condition_daemon("hpheal2");
}

void condition_update(object me, string cnd, mixed cnd_data)
{
    if( !mapp(cnd_data) || (cnd_data["duration"]--) < 1 ) {
        me->delete_condition(cnd);
        return;
    }

    // �ָ�HPֵ
    me->heal_stat("hp", 20);
}


void food_condition_apply(object me, mapping cnd_data)
{
    mapping hpheal;
    
    if( !mapp(hpheal = me->query_condition("hpheal2")) )
        hpheal = ([]);
    
    hpheal["duration"] += cnd_data["duration"];
    hpheal["name"] = BRED"�¼�������"NOR;
    
    if( hpheal["duration"] <= 0 ) hpheal["duration"] = 1;
    else if( hpheal["duration"] > 180 ) hpheal["duration"] = 180;
    
    me->set_condition("hpheal2", hpheal);
}

/*
void condition_apply(object me, string cnd, mapping cnd_data) { }
void condition_unapply(object me, string cnd, mapping cnd_data) { }
*/