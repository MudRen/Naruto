inherit ROOM;

void create()
{
	set("short", "�ѤW���U�Ӫ��Z����");
	set("long", @LONG
�ѤW�H�����Z�����A�@�Ӭ۷������a��C
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