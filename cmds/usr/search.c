// search.c

#include <ansi.h>

inherit F_CLEAN_UP;

int searching(object me, object env);
int find_drug(object me,int drug_num);

int main(object me, string arg)
{
	int i;
    object env;
    string msg="", *exits, *detail;

	if( me->is_fighting() || me->is_busy() )
		return notify_fail("������æ��, �������� halt ֹͣ��æ���¡�\n");

    if( !objectp(env=environment(me)) ) return 0;

	message_vision("$N¶�����������飬��ʼ�Ĵ��������ҡ�\n\n", me);
	me->start_busy(1);

    if( env->is_area() ) {
    	string temp;
        mapping info, detail_a;
        
        info = me->query("area_info");
        
        // ������
        msg = env->query("name");
        
        // short ����
        if( (temp=env->query_data(info["x_axis"], info["y_axis"], "short")) ) 
            msg += " - " + temp + "\n";
        else msg += "\n";

        // long ����
        if( (temp=env->query_data(info["x_axis"], info["y_axis"], "long")) ) 
            msg += "    " + temp + "\n";
        else msg += "\n";
        
        // ������ʾ
        exits = env->query_exits(info["x_axis"], info["y_axis"]);
        if( (i = sizeof(exits)) ) {
    	    msg += "�������Եĳ����� ";
    	    while( i-- ) msg += HIW+exits[i]+NOR + ( i ? "��" : "��\n" );
        } else msg += "  ����û���κγ��ڡ�\n";

        // ϸ����ʾ
    	detail_a = env->query_area_detail(info["x_axis"], info["y_axis"]);
    	if( (i=sizeof(detail_a)) ) {
    		msg += "\n����������������У�\n";
    		detail = keys(detail_a);
    		while(i--) msg += "  " + detail[i] + "\n";
    	}
    } else {
    	msg = env->query("short") + " -\n";
    	msg += "    " + env->query("long");
        // ������ʾ
        if( (i = sizeof(env->query("exits"))) ) {
            exits = keys(env->query("exits"));
    	    msg += "    �������Եĳ����� ";
    	    while( i-- ) msg += HIW+exits[i]+NOR + ( i ? "��" : "��\n" );
        } else msg += "  ����û���κγ��ڡ�\n";
    	
    	if( (i=sizeof(env->query("detail"))) ) {
    		msg += "\n����������������У�\n";
    		detail = keys(env->query("detail"));
    		while(i--) msg += "  " + detail[i] + "\n";    		
    	}
    }

    me->start_more(msg);

	return 1;
}

int help(object me)
{
	write(@HELP
ָ���ʽ�ssearch

���ָ���������������Ļ������Ƿ�������Ķ������������񡢰�����������Ʒ
�ȿ��� search �����һЩ���߻�Ѱ������Ķ�����

HELP
	);
	return 1;
}