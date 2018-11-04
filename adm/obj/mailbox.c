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

/* ��� mapping ����������ʼ����ݣ�����Ϊ private �Է�ֹ�����������
 * �̳з�ʽ��ȡ��
 */
private mapping *mails;

void get_mail_text(mapping mail, string str);

private void
create()
{
    set_name("����", "mail folder", "mail", "folder" );
    set("long", @LONG
����һ�������������ż��������o�����������ָ������������ż���

mail <ĳ��>		���Ÿ����˩o����������ʦ�o�������������վ���ܼ��š�
forward <���> <ĳ��>	��ĳ����ת�ĸ����˩oͬ����������վ�����á�
from [-a]		�鿴�����е��ż���Ԥ��ֻ��ʾ��δ��ȡ���ż������� -a
			����ʾȫ�����ż���
read <���>		�Ķ�һ���ż���
discard <���>		����һ���ż���
LONG
    );
    set("unit", "��");
    set("no_drop", "˽�˵������벻Ҫ��㶪����\n");
    set("no_insert", 1);
    setup();

    seteuid(0);	/* ��� euid���� SMTP daemon ָ�� uid �������� */
}

varargs string
short(int raw)
{
    if( arrayp(mails) && sizeof(mails) )
	return ::short(raw) + "[ " + sizeof(mails) + " ���� ]";
    return ::short(raw) + "[ ����һ�� ]";
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
	/* ��ȫ��ʩ���� mailbox ���ƶ��� this_player() ����ĵط�������
	 * ���������������ʼ����ݣ�������������������
	 */
	mails = 0;
	call_out("retire", 0);
	return;
    }

    seteuid(getuid());		/* �趨 euid���������� SMTP daemon */
    ob = load_object(SMTP_D);
    seteuid(0);			/* ��� euid���� SMTP ָ�� uid �������� */
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
    /* ֻ�������Ծ��� ROOT Ȩ�޵�������͵��� */
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
	return notify_fail("�������һ����վ���ܼ��š�\n");

    if( !arg || arg=="" )
	return notify_fail("��Ҫ���Ÿ�˭�t\n");

    mail = ([
	"from": this_player()->name(1) + "(" + this_player()->query("id") + ")",
	"unread": 1 ,
	"title": "����",
	"to": arg,
	"time": time(),
	"text": ""
    ]);
    write("�ż�����s");
    input_to("get_mail_title", mail);
    return 1;
}

private void
get_mail_title(string str, mapping mail)
{
    if( str!="" ) mail["title"] = str;
    write("�ż����ݩs\n");
    this_player()->edit( (: get_mail_text, mail :) );
}

void get_mail_text(mapping mail, string str)
{
    mail["text"] = str;
    write("���Լ�Ҫ��һ�ݱ�����(y/n)�t[n]");
    input_to("confirm_copy", mail);
}

private void
confirm_copy(string yn, mapping mail)
{
    if( yn[0]=='y' || yn[0]=='Y' ) receive_mail(mail);
    if( SMTP_D->send_mail(getuid(), mail["to"], mail) )
	write("Ok.\n");
    else
	write("����ʧ�ܣ�\n");
}

private int
do_from(string arg)
{
    int i, n_listed = 0, all = 0;
    string txt = "--------------------------------------------------\n";

    if( !arrayp(mails) || !sizeof(mails) ) {
	write("���������Ŀǰû���κ��ż���\n");
	return 1;
    }

    if( arg=="-a" ) all = 1;
    for(i=sizeof(mails)-1; i>=0; i--)
    {
	mapping msg = mails[i];

	if( !all && !msg["unread"] ) continue;
	n_listed++;
	if( msg["unread"] == 1 )
	    txt += sprintf("%2d " HIC "%-46s ����: %-20s\n" NOR,
		i+1, msg["title"], msg["from"]);
	else
	    txt += sprintf("%2d %-46s ����: %-20s\n",
		i+1, msg["title"], msg["from"]);
    }
    if( all ) txt = "�����ڹ��� " + sizeof(mails) + " ���ż��s\n" + txt;
    else txt = sprintf("�㹲�� %d/%d ��δ�����ţ�\n%s", n_listed,
	sizeof(mails), txt);
    this_player()->start_more(txt);

    return 1;
}

private int
do_read(string arg)
{
    int num;

    if( !arg || !sscanf(arg, "%d", num) )
	return notify_fail("��Ҫ����һ���ũt\n");

    if( !pointerp(mails) || num < 1 || num > sizeof(mails) )
	return notify_fail("û�������ŵ��ż���\n");

    num--;

    printf(HIC "�ż�����s%s\n�����˩s%s" NOR "\n\n%s\n",
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
	return notify_fail("��Ҫ������һ���ũt\n");

    if( !pointerp(mails) || num > sizeof(mails) )
	return notify_fail("û�������ŵ��ż���\n");

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
	return notify_fail("�������һ����վ���ܼ��š�\n");

    if( !arg || sscanf(arg, "%d %s", num, dest)!=2 )
	return notify_fail("��Ҫ����һ����ת�ĸ����˩t\n");

    if( !pointerp(mails) || num > sizeof(mails) )
	return notify_fail("û�������ŵ��ż���\n");

    num --;

    m = ([]);
    m["title"] = mails[num]["title"];
    m["text"] = mails[num]["text"];
    m["unread"] = 1;
    m["time"] = mails[num]["time"];
    m["from"] = mails[num]["from"] + "/ת����" + this_player()->query("name");
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

