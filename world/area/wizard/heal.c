inherit ROOM;

void create()
{
    set("short", "���ϵ�������ҽҩ��");
    set("long", @LONG
�����˼��ҽҩ�꣬һ���൱���صĵط���
LONG);
    set("exits", ([
        "east" : ([ "filename" : __DIR__"wizard.c",
                       "x_axis" : 5,
                       "y_axis" : 24,
                  ]),
    ]));
    
    set("objects", ([
        __DIR__"npc/piller.c" : 1,
    ]));
    setup();
}