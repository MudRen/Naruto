/*  okip.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#pragma optimize
#pragma save_binary

#include <command.h>
#include <login.h>

//inherit F_CLEAN_UP;

void create()
{
    seteuid(getuid());
    set_reset(this_object(), 86400);
}


int main(object me, string str)
{
    object link_ob;
    string *args, *okip, ip;

    SECURED_COMMAND;

    if( stringp(str) ) args = explode(str, " ") - ({ "" });
    else args = ({});

    if( wizhood(me)=="(admin)" && sizeof(args) > 0 && args[0][0] != '-' ) {
	link_ob = FINGER_D->acquire_login_ob(args[0]);
	args = args[1..];
    }
    else link_ob = me->link();

    if( ! link_ob ) return notify_fail("找不到连线物件。\n");

    if( stringp(link_ob->query("okip")) )
	okip = explode(link_ob->query("okip"), ":");
    else
	okip = ({});

    if( ! sizeof(args) ) {
	write( sprintf( "%s 的 okip 目前设定为：\n\t%s\n",
		link_ob->query("id"),
		implode(okip, "\t\n")) );
	return 1;
    }

    switch(args[0])
    {
    case "-a":
	foreach(ip in args[1..])
	    if( member_array(ip, okip)== -1 ) okip += ({ ip });
	break;
    case "-d":
	okip -= args[1..];
	break;
    default:
	return notify_fail("指令格式：okip [-a|-d] 连线位置 ...\n");
    }

#ifdef	SAVE_USER
    link_ob->set("okip", implode(okip, ":"));
    link_ob->save();
    if( ! link_ob->body() ) destruct(link_ob);
#endif

    write("Ok。\n");
    return 1;
}

int help()
{
        write(@TEXT
指令格式：okip [-a|-d ] [IP:IP:...]

这个指令可以设定你的来源位址, 最多可设定十组常用的位址, 当来源位
址不符时, 系统将拒绝该连线要求, 以防止被来自这些位址外的不明人士
所侵入。设定位址时请小心设定, 若是设错而造成不能连线进入时, 请备
妥下列资料mail给arch级以上巫师处理:
1. 该char平日的上线IP (记得的话, 最好把okip设成什么也写上去)
2. 该char的特徵 (包括属性, 精气神, 技能等)
3. 该char的密码

范例
okip -a 111.222.333.444 就是增加了 111.222.333.444 为你来源路径
okip -d 111.222.333.444 就是删除了 111.222.333.444 为你来源路径

位址之设定支援以*代表未知的码, 但一组IP里, *只能用一次, 如
okip -a 140.113*	表示IP开头只要是140.113的位址均可连入
okip -a *.hinet.net 	表示IP只要最后两码的位址是hinet.net的均
			可连入
TEXT
        );
        return 1;
}
