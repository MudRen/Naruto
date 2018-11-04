/*  save.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <login.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object link_ob;
    int last_save, delta_t;
    
    seteuid(getuid());

    if( !objectp(link_ob = me->link()) )
        return notify_fail("�㲻�Ǿ����������߽���o���ܴ��档\n");

    if( !environment(me) ) 
    	return notify_fail("�㲻֪��������������޷����档\n");

    if( environment(me)->query("recoverroom") ) {
        me->set("recoverroom", base_name(environment(me)));
        write("�����꽫���ڴ�������\n");
    }

    if( !wizardp(me) && (last_save = me->query_temp("last_save")) && (delta_t = (time()/60 - last_save)) < 5 )
        return notify_fail("���ü�, " +delta_t+ " ����ǰ��Ÿմ����, " +(5 - delta_t)+ " ���Ӻ����԰ɡ�\n");

    me->set("startroom", base_name(environment(me)));

#ifdef SAVE_USER
    if( (int)me->save() ) {
        write("����������ϡ�\n");
        me->set_temp("last_save", (int)(time()/60) );
        return 1;
    } else {
        write("����ʧ�ܡ�\n");
        return 0;
    }
#else
    return notify_fail("ϵͳĿǰ������ʹ�������ϡ�\n");
#endif
}

int help(object me)
{
    write(@HELP
ָ���ʽ�ssave

��������ܶ��Ľ����������Ϊ������ϵͳЧ��, Ŀǰ���� save ֮ʱ�������Ϊ
����ӡ� quit ʱϵͳ���������� save��
see also: quit, backup
HELP
    );
    return 1;
}
