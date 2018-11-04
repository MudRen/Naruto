/*  reply.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <ansi.h>
//#include <net/dns.h>

#define MIN_REPLY_INTERVAL       3
#define MAX_REPLY_MESSAGE        1024

inherit F_CLEAN_UP;

int help(object me);

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string target, mud, uid, *ignore;
    object obj;
    int idle_time, tmp;

    if( !arg || arg=="" )
	return notify_fail("��Ҫ�ش�ʲô�t\n");

    if( !stringp(target = me->query_temp("reply")) )
	return notify_fail("�ղ�û���˺���˵������\n");

    if( time() - (int)me->query_temp("last_reply_time") < MIN_REPLY_INTERVAL )
        return notify_fail("�������Ϣһ�²����ٶ�ʹ�����ָ�\n");

    me->set_temp("last_reply_time", time());

    if( strlen(arg) > MAX_REPLY_MESSAGE )
        return notify_fail("���ѶϢ̫���ˣ��볤����˵��\n");

/*
    if( sscanf(target, "%s@%s", target, mud)==2 ) {
	GTELL->send_gtell(lower_case(mud), lower_case(target), me, arg);
	write("��·ѶϢ���ͳ��o����Ҫ�Ժ���ܵõ���Ӧ��\n");
	return 1;
    }
*/
    if( (!obj = find_player(target)) || (!interactive(obj)) )
	return notify_fail("�ղź���˵�����������޷�������o�����Ѿ��뿪��Ϸ�ˡ�\n");

    uid = getuid(me);
    ignore = obj->query("ignore");

    // add by dragoon to let tell work in sneak
    if( me->query_temp("pending/hidden") ) {
        tmp = me->query_temp("pending/hidden");
        me->delete_temp("pending/hidden");
    }

        if (pointerp(ignore) && member_array(uid, ignore) != -1  )
         return notify_fail(sprintf("%s��������Ļش�\n",obj->name(1)));
        if( obj->query_temp("netdead") )
          return notify_fail(sprintf(HIG"%s������...\n"NOR,obj->name(1)));
        write("��ش�" + obj->name(1) + "(" + obj->query("id") + ")�s"
                + HIG + arg + "\n" NOR);

    if ((idle_time = query_idle(obj)/60) > 2
    || in_input(obj)
    || in_edit(obj) ) {
                write(GRN+obj->name(1)+
                (idle_time>2 ? "�Ѿ����� "+idle_time+" ������" : "")+
                (in_input(obj) ? " <����������> " : "")+
                (in_edit(obj) ? " <�༭������> " : "")+
                ", ���ܲ������ϻش����...\n"+NOR);
    }

        tell_object(obj, sprintf("%s(%s)�ش���s" + HIG + "%s\n"NOR,
                me->name(1), me->query("id"), arg ));

    // reset hidden if it exist before use tell, -Dragoon
    if( tmp > 0 ) {
        me->set_temp("pending/hidden", tmp); }

        obj->set_temp("reply", me->query("id"));
        return 1;
}

int help(object me)
{
        write(@HELP
ָ���ʽ�sreply <ѶϢ>

����������ָ��͸ղ��� tell �� reply ����˵����ʹ����˵�����Է���������
��������, ������������״̬, ��༭����״̬, ��ϵͳ����ʾ���㡣

see also : tell
HELP
        );
        return 1;
}
