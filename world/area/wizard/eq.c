inherit ROOM;

void create()
{
	set("short", "���ϵ������ķ��ߵ�");
	set("long", @LONG
�����˼�ķ��ߵ꣬һ���൱���صĵط���
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