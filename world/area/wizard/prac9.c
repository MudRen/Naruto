#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "��ϰ��(��)");
	set("long", @LONG
����ڷ��˼������ˣ����������������񶷼��ܡ�
LONG);

	set("exits", ([
		"north" : __DIR__"prac6",
		"west" : __DIR__"prac8",
	]));

    set("objects", ([
    	_DIR_NPC_"stake.c": 4,
    ]));

	setup();
}