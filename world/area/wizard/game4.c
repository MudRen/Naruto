#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "��Ϸ��(��)");
	set("long", @LONG
����һ���������ֳ������������ܰڷ�һЩ����Ķ���
LONG);

	set("exits", ([
        "west" : ([ "filename" : __DIR__"wizard.c",
                     "x_axis" : 23,
                     "y_axis" : 36,
                  ]),				
		"north" : __DIR__"game1",		
		"east" : __DIR__"game5",		
		"south" : __DIR__"game7",		
	]));

    set("objects", ([
    	_DIR_ITEM_"big2.c" : 1,
    ]));

    set("no_fight", 1);
	setup();
}