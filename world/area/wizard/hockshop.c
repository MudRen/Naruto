#include <room.h>

inherit HOCKSHOP;

void create()
{
	set("short", "���ϵ������ĵ���");
	set("long", @LONG
�����˼�ĵ��̣�һ���൱���صĵط���
LONG);
    set("exits", ([
        "east" : ([ "filename" : __DIR__"wizard.c",
                       "x_axis" : 5,
                       "y_axis" : 40,
                  ]),
    ]));

	setup();
}