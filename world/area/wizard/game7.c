#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "��Ϸ��(��)");
	set("long", @LONG
����һ���������ֳ������������ܰڷ�һЩ����Ķ���
LONG);

	set("exits", ([
		"north" : __DIR__"game4",		
		"east" : __DIR__"game8",		
	]));

    set("objects", ([
    	_DIR_ITEM_"big2.c" : 1,
    ]));

    set("no_fight", 1);
	setup();
}