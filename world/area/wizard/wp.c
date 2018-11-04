inherit ROOM;

void create()
{
	set("short", "天上掉下來的武器店");
	set("long", @LONG
天上人間的武器店，一個相當神秘的地方。
LONG);
    set("exits", ([
        "east" : ([ "filename" : __DIR__"wizard.c",
                       "x_axis" : 5,
                       "y_axis" : 32,
                  ]),
    ]));
    set("objects", ([
        __DIR__"npc/wper.c": 1,
    ]));
	setup();
}