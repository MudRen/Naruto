#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "��ϰ��(��)");
	set("long", @LONG
����ڷ��˼������ˣ����������������񶷼��ܡ�
LONG);

	set("exits", ([
        "south" : ([ "filename" : __DIR__"wizard.c",
                     "x_axis" : 10,
                     "y_axis" : 39,
                  ]),		
		"north" : __DIR__"prac5",		
		"west" : __DIR__"prac7",		
		"east" : __DIR__"prac9",		
	]));

    set("objects", ([
    	_DIR_NPC_"stake.c": 4,
    ]));

	setup();
}