#include <room.h>

inherit BANK;

void create()
{
	set("short", "���ϵ�����������");
	set("long", @LONG
�����˼�����У�һ���൱���صĵط���
    ��Ǯ��deposit
    ��Ǯ��withdraw
LONG);
    set("exits", ([
        "east" : ([ "filename" : __DIR__"wizard.c",
                       "x_axis" : 5,
                       "y_axis" : 36,
                  ]),
    ]));
	setup();
}