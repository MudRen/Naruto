/*  alias.c

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

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    int i;
    mapping alias;
    string verb, replace, *vrbs;
    object link;

    SECURED_COMMAND;

    if( !(link = me->link()) )
	return notify_fail("��û�����������\n");

    if( !arg ) {
	alias = link->query_all_alias();
	if( !sizeof(alias) ) {
	    write("��Ŀǰ��û���趨�κ� alias��\n");
	    return 1;
        } else  {
	    write("��Ŀǰ�趨�� alias �Щs\n");
	    vrbs = keys(alias);
	    for(i=0; i<sizeof(vrbs); i++)
		printf("%-15s = %s\n", vrbs[i], alias[vrbs[i]]);
	    return 1;
	}
    }

    if( sscanf(arg, "%s %s", verb, replace)!=2 )
	link->set_alias(arg, 0);
    else if( verb=="alias" )
	return notify_fail("�㲻�ܽ� \"alias\" ָ���趨������;��\n");
    else if( verb=="" )
	return notify_fail("��Ҫ��ʲô alias�t\n");
    else
	link->set_alias(verb, replace);

    write("Ok.\n");
    return 1;
}

int help (object me)
{
    write(@HELP
ָ���ʽ : alias <���趨ָ֮��> <ϵͳ�ṩָ֮��>
 
��ʱϵͳ���ṩָ֮����Ҫ����ܳ����ִ�, ��ʹ��ʱ(�����Ǿ����õ���)
�����о�������, ��ʱ��(��)�����ô�һָ���趨�����ԭ��ָ֮�
 
����:
    'alias sc score' ���� sc ȡ�� score ָ�
    'alias' �󲻼Ӳ������г������е����ָ�
    'alias sc' ������ sc ������ָ� (���������Ļ�)
 
���п����� $1, $2, $3 .... ��ȡ����һ���ڶ��������������o���� $* ȡ��
���еĲ����o��s
    'alias pb put $1 in $2'

��ÿ����(��)��s
    pb bandage bag

�ͻ�ȡ���ɩs
    put bandage in bag
    
HELP
    );
    return 1;
}
