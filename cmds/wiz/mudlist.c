/*  mudlist.c - list muds that are in contact with us over intermud

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <intermud.h>

inherit F_CLEAN_UP;

#define SYNTAX	"ָ���ʽ�smudlist [<Mud ����>]\n"

private void create() { seteuid(getuid()); }

int
main(object me, string arg)
{
    mapping mudlist;
    mixed *muds;
    string output;
    string mud;
    int list_all = 0;

    if( !find_object(INTERMUD_D) )
        return notify_fail("��·���鲢û�б����룬���Ƚ���·�������롣\n");

    mudlist = (mapping)INTERMUD_D->query_mudlist();

    if( !mapp(mudlist) )
        return notify_fail( MUD_NAME + "Ŀǰ��δ������ Mud ȡ����ϵ��\n");

    muds = sort_array( keys(mudlist), 1);

    if( arg ) {
	string* args, mud_name;

	args = explode(arg, " ") - ({ "" });

	foreach(string name in args)
	{
	    string prop, str, msg;
	    mapping mudinfo;

	    if( mud_name && mapp(mudinfo)
	    &&	sscanf(name, "%s=%s", prop, str)==2 ) {
		write("�趨 " + mud_name + " �ı������ԣ�" + name + "\n");
		INTERMUD_D->set_mudinfo(mud_name, ([ prop : str ]) );
		continue;
	    }

	    if( sscanf(name, "%s->%s", prop, str)==2 ) {
		INTERMUD_D->set_mud_alias(prop, str);
		name = prop;
	    }

	    mud_name = name; // = htonn(name);
            if(!mapp( mudinfo = INTERMUD_D->query_mudinfo(name) )) {
                write(MUD_NAME + "��û�к� \"" + name + "\" ȡ����ϵ��\n");
		continue;
            }

	    msg = "";
	    foreach(prop, str in mudinfo)
		msg += sprintf("  %15s: %O\n", prop, str);
	    write("�й� " + name + " ����Ѷ�s\n" + msg);
        }
	if( mud_name ) return 1;
    }

    output = "";

    // Loop through mud list and store one by one
    foreach(mud in muds)
    {
//	if( !list_all && mudlist[mud]["MUDLIB"] != "Eastern Stories" )
//	    continue;
	output += sprintf("%-20s %-10s %s (%s)\n",
		mud,
		mudlist[mud]["ALIAS"] ? mudlist[mud]["ALIAS"] : "",
		mudlist[mud]["NAME"],
		mudlist[mud]["MUDLIB"]);
    }

    //    Display dumped mudlist output through user's more pager
    this_player()->start_more( output );

    return 1;
}

int help()
{
    write( @HELP
ָ���ʽ��mudlist [-a] [<MUD IP:PORT>|<����>] {����=�ִ�}
	  mudlist <����>-><MUD IP:PORT>

���ָ�������г�Ŀǰ͸�� intermud Э��ȡ����ϵ������ Mud��

������� -a ѡ����е� Mud ���ᱻ�г�������ֻ��ʹ�� es2 mudlib �� Mud
�ᱻ�г���

�����ָ����һ�� MUD ���ƣ�mudlist ���г��� MUD �������Ѷ��
mudlist �����ڱ��ض��޸����� MUD �����ԣ����͵���;���趨һ�� MUD �����ı�
���磺

> mudlist xajh ENCODING=GB

�Ὣ xajh(Ц������) ���ʹ�� GB ����� MUD �� ENCODING �趨�� GB����Ϊ�в�
��ʹ�þɰ� ES2 mudlib �Ĵ�½ MUD ��û���趨 ENCODING �����ԣ���������㿴��
��Щ MUD �͹����� es Ƶ��ѶϢ�����룬���������������ָ���趨��

��Щ�趨ֻ���ڱ��ض���Ч����û�а취�ı�Է����趨����˵� intermud ���¸�
MUD ������ʱ������ܱ��������趨�����׳�н�İ취��д�Ÿ��Է��Ĺ����ߣ�����
����ȷ�趨�ͳ����� ENCODING ���ԡ�
HELP
    );
    return 1;
}

