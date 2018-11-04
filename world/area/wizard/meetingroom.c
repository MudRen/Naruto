#include <ansi.h>
#include <command.h>

inherit ROOM;

string filename;

void create()
{
	set("short", "������");
	set("long", @LONG
�����������������ش�����ĳ�����Բ�εķ��䣬��ɫ��ǽ����
����ɫ�ĵ�̺������Ķ����⍀��һ�����صľ�ʯ�����߾ʹӾ�ʯɢ
���������������������䣬����ȴ�����ۡ����ŵ�ǽ�߸���һ����ɫ
����壬�����Ͽ���Щ�֡�
LONG
	);

	set("exits", ([
		"down" : __DIR__"guildhall"
	]));

	set("detail", ([
		"���" : "һ��Ʈ���ڰ���е��ϴ����, �������Щ��:

�����¼�ķ���:
start <filename> :	��ʼ��¼�������, filename �ǻ����¼�ĵ�����
over :			ֹͣ��¼������̡�

",
	]));
    set("no_fight", 1);
	setup();
	load_object("/daemon/board/wizard_talk");
}

void init()
{
	add_action("say", ({"say", "'"}) );
	add_action("start", "start");
	add_action("over", "over");
}

int start(string str)
{
	if( !wizardp(this_player()) )
	return notify_fail("��û��Ȩ�������顣\n");

	if( !str )
	return notify_fail("��Ҫ�������¼д���Ǹ���??\n");

	if ( (int)query("meeting_trigger") != 0 )
	return notify_fail("Ŀǰ���ڿ����С�\n");

	set("meeting_trigger", 1);
	message_vision(HIW"\n$N�������ڻ�����ʽ��ʼ��\n\n"NOR, this_player());
	filename = str;
	log_file(filename, sprintf("[%s] %s (%s) �������顣\n",
		ctime(time()), this_player()->name(1), geteuid(this_player())));
	return 1;
}

int say(string msg)
{
	if( !msg ) return 0;
	else
	SAY_CMD->main(this_player(), msg);
	if ((int)query("meeting_trigger") == 1)
	log_file(filename,
	sprintf("%s˵���s" + msg + "\n",this_player()->name(1)) );
	return 1;
}

int over()
{
	if( !wizardp(this_player()) )
        return notify_fail("��û��Ȩ���������顣\n");

	delete("meeting_trigger");
	log_file(filename, sprintf("[%s] %s (%s) �������顣\n",
		ctime(time()), this_player()->name(1), geteuid(this_player())));
	message_vision(HIW"\n$N�������������\n\n"NOR, this_player());
	return 1;
}
