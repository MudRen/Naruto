#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "��Ϸ��(��)");
	set("long", @LONG
����һ���������ֳ������������ܰڷ�һЩ����Ķ���
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