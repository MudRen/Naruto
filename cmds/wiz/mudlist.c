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

#define SYNTAX	"指令格式﹕mudlist [<Mud 名称>]\n"

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
        return notify_fail("网路精灵并没有被载入，请先将网路精灵载入。\n");

    mudlist = (mapping)INTERMUD_D->query_mudlist();

    if( !mapp(mudlist) )
        return notify_fail( MUD_NAME + "目前尚未和其他 Mud 取得联系。\n");

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
		write("设定 " + mud_name + " 的本地属性：" + name + "\n");
		INTERMUD_D->set_mudinfo(mud_name, ([ prop : str ]) );
		continue;
	    }

	    if( sscanf(name, "%s->%s", prop, str)==2 ) {
		INTERMUD_D->set_mud_alias(prop, str);
		name = prop;
	    }

	    mud_name = name; // = htonn(name);
            if(!mapp( mudinfo = INTERMUD_D->query_mudinfo(name) )) {
                write(MUD_NAME + "并没有和 \"" + name + "\" 取得联系。\n");
		continue;
            }

	    msg = "";
	    foreach(prop, str in mudinfo)
		msg += sprintf("  %15s: %O\n", prop, str);
	    write("有关 " + name + " 的资讯﹕\n" + msg);
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
指令格式：mudlist [-a] [<MUD IP:PORT>|<代号>] {属性=字串}
	  mudlist <代号>-><MUD IP:PORT>

这个指令让你列出目前透过 intermud 协定取得联系的其他 Mud。

如果加上 -a 选项，所有的 Mud 都会被列出；否则只有使用 es2 mudlib 的 Mud
会被列出。

你可以指定单一的 MUD 名称，mudlist 会列出该 MUD 的相关资讯。
mudlist 可以在本地端修改其他 MUD 的属性，典型的用途是设定一个 MUD 的中文编
码如：

> mudlist xajh ENCODING=GB

会将 xajh(笑傲江湖) 这个使用 GB 内码的 MUD 的 ENCODING 设定成 GB，因为有不
少使用旧版 ES2 mudlib 的大陆 MUD 并没有设定 ENCODING 的属性，所以如果你看到
有些 MUD 送过来的 es 频道讯息是乱码，可以试著用上面的指令设定。

这些设定只有在本地端有效，并没有办法改变对方的设定，因此当 intermud 更新该
MUD 的属性时，你可能必须重新设定。釜底抽薪的办法是写信给对方的管理者，请他
们正确设定送出来的 ENCODING 属性。
HELP
    );
    return 1;
}

