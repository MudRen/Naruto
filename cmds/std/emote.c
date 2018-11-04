// emote.c (Mon  09-04-95)

#include <ansi.h>
inherit F_CLEAN_UP;
 
int main(object me, string str)
{
	mapping info;
	
	info = me->query("area_info");
	
    if (!str) {
        write(CYN"�㿴��������ḻ��\n"NOR);
        if( environment(me)->is_area() ) 
        	tell_area(environment(me), info["x_axis"], info["y_axis"], CYN+(string)me->name()+"����������ḻ��\n"+NOR, ({me}));
        else  tell_room(environment(me), CYN+(string)me->name()+"����������ḻ��\n" +NOR, me);
        return 1;
    }
    write(CYN"��"+str+"\n"NOR);
    if( environment(me)->is_area() )
      	tell_area(environment(me), info["x_axis"], info["y_axis"], CYN+(string)me->name()+str+"\n"+NOR, ({me}));
    else tell_room(environment(me), CYN+(string)me->name()+ str+"\n"+NOR, me);
    return 1;
}
 
int help(object me)
{
	write(@HELP
ָ���ʽ: emote <������>

���ָ�����������һ��ϵͳû��Ԥ��Ķ�����, ϵͳ�Ὣ��������
�����ּ��������������ʾ��������ͬһ���ط������￴. Ϊ�˱�����
������, ���������Ĵʾ�ǰ������ -> �Թ�����. (��ʦ���ڴ�
��.)

����: emote ����������
��ῴ��: ������������
�����˻ῴ��: ->��������������

����, ���Ӿ����������.

���ָ��: semote
HELP
	);
        return 1;
}
