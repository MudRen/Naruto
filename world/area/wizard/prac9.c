#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "练习场(９)");
	set("long", @LONG
这里摆放了几个假人，可以在这里练技格斗技能。
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