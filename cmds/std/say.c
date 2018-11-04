// say.c

#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
	object *ob;
	if (!arg) return notify_fail("��Ҫ˵ʲô�t\n");

	write("��˵���s" + HIG + arg + "\n" NOR);

	// The mudlib interface of say
	// ��������ػ�Ӧ, ���������ж� by -Acme-
	if( environment(me)->is_area() ) {
		mapping info;
		info = me->query("area_info");
		ob = environment(me)->query_inventory(info["x_axis"], info["y_axis"]);
        tell_area(environment(me), info["x_axis"], info["y_axis"], me->name() + "˵���s" + HIG + arg + "\n" NOR, ({me}));
	} else {
        message("sound", me->name() + "˵���s" + HIG + arg + "\n" NOR, environment(me), me);
	    ob = all_inventory(environment(me));
	}

//    message("sound", me->name() + "˵���s" + HIG + arg + "\n" NOR, environment(me), me);
	
	ob -= ({ me });
	ob->relay_say(me, arg);

	return 1;
}

int help (object me)
{
	write(@HELP
ָ���ʽ: say <ѶϢ>
 
˵���o���и�����ͬһ��������˶���������˵�Ļ���
 
ע: ��ָ����� ' ȡ��.
 
HELP
	);
	return 1;
}
