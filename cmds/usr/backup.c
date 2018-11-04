/*  backup.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <command.h>
#include <login.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    int last_backup, delta_t;
    string save_file;

    SECURED_COMMAND;

    seteuid(getuid());

    if( (last_backup = me->query_temp("last_backup"))
    &&  (delta_t = (time()/60 - last_backup)) < 5 ) {
	write( sprintf("���ü���%d ����ǰ��Ÿձ��ݹ���%d ���Ӻ����԰ɡ�\n",
		delta_t, (5-delta_t)) );
	return 1;
    }

#ifdef SAVE_USER
    save_file = me->query_save_file();
    if( cp(save_file, save_file+".backup") ) {
	write("������ϡ�\n");
	me->set_temp("last_backup", (int)(time()/60) );
    }
    else
	write("����ʧ�ܣ�\n");
    return 1;
#else
    return notify_fail("ϵͳĿǰ������ʹ�������ϡ�\n");
#endif
}

int help(object me)
{
    write(@HELP
ָ���ʽ�sbackup

��������ܶ��Ľ������������Ϊ������ϵͳЧ�ʣ�Ŀǰ���� backup ֮ʱ������
��Ϊ����ӡ�

���ָ���Ŀ����Ϊ�˱���ʹ�������ܵ� <�������ԭ�����޷����������> ������
���޷��ֲ�֮�ش���ʧ��

������ʦ������Ϊ�б�Ҫʱ���Ὣ��ı��ݵ�ȡ�ء�
HELP
    );
    return 1;
}
