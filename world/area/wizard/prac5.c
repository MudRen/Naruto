#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "��ϰ��(��)");
	set("long", @LONG
����ڷ��˼������ˣ����������������񶷼��ܡ�
LONG);

	set("exits", ([
		"north" : __DIR__"prac2",		
		"east" : __DIR__"prac6",		
		"south" : __DIR__"prac8",		
		"west" : __DIR__"prac4",		
	]));

    set("objects", ([
    	_DIR_NPC_"stake.c": 4,
    ]));

	setup();
}