inherit ROOM;

void create()
{
	set("short", "天上掉下来的防具店");
	set("long", @LONG
天上人间的防具店，一个相当神秘的地方。
LONG);
    set("exits", ([
        "east" : ([ "filename" : __DIR__"wizard.c",
                       "x_axis" : 5,
                       "y_axis" : 28,
                  ]),
    ]));
    set("objects", ([
        __DIR__"npc/eqer.c" : 1,
    ]));
	setup();
}