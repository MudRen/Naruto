/*  mailbox.c

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

inherit ITEM;
inherit F_SAVE;

/* 这个 mapping 储存载入的邮件内容，宣告为 private 以防止被其他物件以
 * 继承方式存取。
 */
private mapping *mails;

void get_mail_text(mapping mail, string str);

private void
create()
{
    set_name("信札", "mail folder", "mail", "folder" );
    set("long", @LONG
这是一个用来整理你信件的信札﹐你可以用以下指令来处理你的信件：

mail <某人>		寄信给别人﹐除非你是巫师﹐否则你必须在驿站才能寄信。
forward <编号> <某人>	将某封信转寄给别人﹐同样必须在驿站才有用。
from [-a]		查看信札中的信件，预设只显示尚未读取的信件，加上 -a
			会显示全部的信件。
read <编号>		阅读一封信件。
discard <编号>		丢弃一封信件。
LONG
    );
    set("unit", "个");
    set("no_drop", "私人的信札请不要随便丢弃。\n");
    set("no_insert", 1);
    setup();

    seteuid(0);	/* 清除 euid，让 SMTP daemon 指定 uid 给这个物件 */
}

varargs string
short(int raw)
{
    if( arrayp(mails) && sizeof(mails) )
	return ::short(raw) + "[ " + sizeof(mails) + " 封信 ]";
    return ::short(raw) + "[ 空无一物 ]";
}

nomask void
retire()
{
    destruct(this_object());
}

nomask private void
init()
{
    object ob;

    if( environment()!=this_player() ) {
	/* 安全措施：若 mailbox 被移动到 this_player() 以外的地方，立刻
	 * 销毁物件中载入的邮件内容，并尽快销毁这个物件。
	 */
	mails = 0;
	call_out("retire", 0);
	return;
    }

    seteuid(getuid());		/* 设定 euid，用来载入 SMTP daemon */
    ob = load_object(SMTP_D);
    seteuid(0);			/* 清除 euid，让 SMTP 指定 uid 给这个物件 */
    ob->restore_mailbox(this_object());

    if( objectp(ob = this_player()->query_temp("mbox_ob")) )
	ob->retire();

    this_player()->set_temp("mbox_ob", this_object());

    set("owner_ob", this_player());
    add_action("do_mail", "mail");
    add_action("do_forward", "forward");
    add_action("do_from", "from");
    add_action("do_read", "read");
    add_action("do_discard", "discard");
}

nomask string
query_save_file()
{
    return mail_data(getuid());
}

void receive_mail(mapping mail)
{
    /* 只接受来自具有 ROOT 权限的物件所送的信 */
if (!previous_object()) error("no previous object\n");
    if( geteuid(previous_object()) != ROOT_UID )
	error("Permission denied.\n");

    if( !pointerp(mails) ) mails = ({ mail });
    else mails += ({ mail });
}

private int
do_mail(string arg)
{
    mapping mail;

    if( this_player()!=environment() ) return 0;
    if( !wizardp(this_player())
    &&  !environment(this_player())->query("post_office") )
	return notify_fail("你必须找一间驿站才能寄信。\n");

    if( !arg || arg=="" )
	return notify_fail("你要寄信给谁﹖\n");

    mail = ([
	"from": this_player()->name(1) + "(" + this_player()->query("id") + ")",
	"unread": 1 ,
	"title": "无题",
	"to": arg,
	"time": time(),
	"text": ""
    ]);
    write("信件标题﹕");
    input_to("get_mail_title", mail);
    return 1;
}

private void
get_mail_title(string str, mapping mail)
{
    if( str!="" ) mail["title"] = str;
    write("信件内容﹕\n");
    this_player()->edit( (: get_mail_text, mail :) );
}

void get_mail_text(mapping mail, string str)
{
    mail["text"] = str;
    write("您自己要留一份备份吗(y/n)﹖[n]");
    input_to("confirm_copy", mail);
}

private void
confirm_copy(string yn, mapping mail)
{
    if( yn[0]=='y' || yn[0]=='Y' ) receive_mail(mail);
    if( SMTP_D->send_mail(getuid(), mail["to"], mail) )
	write("Ok.\n");
    else
	write("寄信失败！\n");
}

private int
do_from(string arg)
{
    int i, n_listed = 0, all = 0;
    string txt = "--------------------------------------------------\n";

    if( !arrayp(mails) || !sizeof(mails) ) {
	write("你的信札中目前没有任何信件。\n");
	return 1;
    }

    if( arg=="-a" ) all = 1;
    for(i=sizeof(mails)-1; i>=0; i--)
    {
	mapping msg = mails[i];

	if( !all && !msg["unread"] ) continue;
	n_listed++;
	if( msg["unread"] == 1 )
	    txt += sprintf("%2d " HIC "%-46s 来自: %-20s\n" NOR,
		i+1, msg["title"], msg["from"]);
	else
	    txt += sprintf("%2d %-46s 来自: %-20s\n",
		i+1, msg["title"], msg["from"]);
    }
    if( all ) txt = "你现在共有 " + sizeof(mails) + " 封信件﹕\n" + txt;
    else txt = sprintf("你共有 %d/%d 封未读的信：\n%s", n_listed,
	sizeof(mails), txt);
    this_player()->start_more(txt);

    return 1;
}

private int
do_read(string arg)
{
    int num;

    if( !arg || !sscanf(arg, "%d", num) )
	return notify_fail("你要读哪一封信﹖\n");

    if( !pointerp(mails) || num < 1 || num > sizeof(mails) )
	return notify_fail("没有这个编号的信件。\n");

    num--;

    printf(HIC "信件标题﹕%s\n寄信人﹕%s" NOR "\n\n%s\n",
	mails[num]["title"], mails[num]["from"], mails[num]["text"] );

	mails[num]["unread"] = 0;
	save();
    return 1;
}

private int
do_discard(string arg)
{
    int num;

    if( !arg || !sscanf(arg, "%d", num) )
	return notify_fail("你要丢弃哪一封信﹖\n");

    if( !pointerp(mails) || num > sizeof(mails) )
	return notify_fail("没有这个编号的信件。\n");

    num --;

    mails = mails[0..num-1] + mails[num+1..sizeof(mails)-1];
    save();
    write("Ok.\n");

    return 1;
}

private int
do_forward(string arg)
{
    string dest;
    int num;
    mapping m;

    if( !wizardp(this_player())
    &&  !environment(this_player())->query("post_office") )
	return notify_fail("你必须找一间驿站才能寄信。\n");

    if( !arg || sscanf(arg, "%d %s", num, dest)!=2 )
	return notify_fail("你要将哪一封信转寄给别人﹖\n");

    if( !pointerp(mails) || num > sizeof(mails) )
	return notify_fail("没有这个编号的信件。\n");

    num --;

    m = ([]);
    m["title"] = mails[num]["title"];
    m["text"] = mails[num]["text"];
    m["unread"] = 1;
    m["time"] = mails[num]["time"];
    m["from"] = mails[num]["from"] + "/转寄自" + this_player()->query("name");
    m["to"] = dest;
    SMTP_D->send_mail( getuid(), dest, m );
    write("Ok.\n");

    return 1;
}

nomask void
owner_is_killed()
{
    retire();
}

