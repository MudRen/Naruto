#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "��ϰ��(��)");
	set("long", @LONG
����ڷ��˼������ˣ����������������񶷼��ܡ�
LONG);

	set("exits", ([
		"west" : __DIR__"prac2",
		"south" : __DIR__"prac6",
	]));

    set("objects", ([
    	_DIR_NPC_"stake.c": 4,
    ]));

	setup();
}