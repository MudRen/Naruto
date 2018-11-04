#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "游戏室(８)");
	set("long", @LONG
这是一间休闲娱乐场所，附近可能摆放一些好玩的东西
LONG);

	set("exits", ([
        "south" : ([ "filename" : __DIR__"wizard.c",
                     "x_axis" : 26,
                     "y_axis" : 39,
                  ]),		
		"north" : __DIR__"game5",		
		"west" : __DIR__"game7",		
		"east" : __DIR__"game9",		
	]));

    set("objects", ([
    	_DIR_ITEM_"big2.c" : 1,
    ]));

    set("no_fight", 1);
	setup();
}