#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "��֮��");
	set("long", @LONG
������������һЩ����˼�����������˵��������ʩչһЩ��
��Ľ������������Ըı����������������ʵ����̫����˼����
��������������ŵ����գ��㲻�ɵ���Ͻ��뿪��
LONG
	);

	set("exits", ([
		"north" : __DIR__"guildhall",
	]));

	setup();

    load_object("/daemon/board/chat");
}