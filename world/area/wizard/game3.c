#include <ansi.h>

inherit ROOM;

void create()
{
	set("short", "��Ϸ��(��)");
	set("long", @LONG
����һ���������ֳ������������ܰڷ�һЩ����Ķ���
LONG);

	set("exits", ([
		"west" : __DIR__"game2",
		"south" : __DIR__"game6",
	]));

    set("objects", ([
    	_DIR_ITEM_"big2.c" : 1,
    ]));

    set("no_fight", 1);
	setup();
}