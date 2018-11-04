/*  help.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include "/doc/help.h"

inherit F_CLEAN_UP;

static string *default_search = DEFAULT_SEARCH_PATHS;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string file, *search;

    if( !arg ) {
        cat(HELP_DIR + "help/topics");
        return 1;
    }

    arg = replace_string(arg, " ", "_");

    // Else, try if a command name is specified.
    seteuid(getuid());
    if( stringp(file = me->find_command(arg)) ) {
        notify_fail("�����ָ����کo���ǲ�û����ϸ��˵���ļ���\n");
        return file->help(me);
    }

    // Support efun/lfun help with same name as other topics such as
    // ed() and ed command.
    sscanf(arg, "%s()", arg);

    search = DEFAULT_SEARCH_PATHS;
    if( wizardp(me) )
        search += WIZARD_SEARCH_PATHS;
        
    foreach(string path in search)
    {
	if( file_size(path + arg) < 0 ) continue;
	if( wizardp(me) )
	    write("˵���ļ���" + path + arg
		+ "\n---------------------------------------------------------------------\n");
	me->start_more( read_file(path + arg) ); 
	return 1;
    }

    return notify_fail("û��������������˵���ļ���\n");
}

int help(object me)
{
    write(@HELP
ָ���ʽ�shelp <����>              ����s> help all
          help <��������>()        ����s> help call_out()

���ָ���ṩ�����ĳһ�������ϸ˵���ļ��o���ǲ�ָ������o���ṩ���й�����
���ļ��o���е������뺯�����ƽ�ΪӢ�ġ�
HELP
    );
    if( wizardp(me) )
        write(@HELP
��ָ�������Ѱ�����Ƿ�Ϊ command, ( ��������Ϊ��ʦ, �������Ѱ /doc/help.h
�е� WIZARD_SEARCH_PATHS ), ����Ѱ DEFAULT_SEARCH_PATHS�� 
����ͬ����֮�ļ�������Щ��Ѱλ����, ��λ�ں���·���߽�������Ѱ, ��ֻ������
ǰ��Ѱ֮ doc, �������ļ�ʱ�����⡣
HELP
        );
    return 1;
}
