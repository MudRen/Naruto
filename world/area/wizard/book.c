inherit ROOM;

void create()
{
    set("short", "���ϵ������ľ����");
    set("long", @LONG
�����˼�ľ���꣬һ���൱���صĵط���
LONG);
    set("exits", ([
        "east" : ([ "filename" : __DIR__"wizard.c",
                       "x_axis" : 5,
                       "y_axis" : 20,
                  ]),
    ]));
    set("objects", ([
        __DIR__"npc/skiller.c" : 1,
    ]));
    setup();
}