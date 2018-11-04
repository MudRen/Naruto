#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "游戏室(２)");
	set("long", @LONG
这是一间休闲娱乐场所，附近可能摆放一些好玩的东西
LONG);

	set("exits", ([
        "north" : ([ "filename" : __DIR__"wizard.c",
                     "x_axis" : 26,
                     "y_axis" : 33,
                  ]),		
		"west" : __DIR__"game1",
		"south" : __DIR__"game5",
		"east" : __DIR__"game3",
	]));

    set("objects", ([
    	_DIR_ITEM_"big2.c" : 1,
    ]));

    set("no_fight", 1);
	setup();
}