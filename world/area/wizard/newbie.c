inherit ROOM;

void create()
{
	set("short", "����ѧ��");
	set("long", @LONG
���������ֽ�ѧѧ�õĴ��ſڣ�����Կ����������������֣�ÿ
�������϶��������ڴ���������ѧ�����̵�һЩ��Ϸ�еĻ�����
ʶ�Ͳ�������������ָ���������������д�����
LONG
	);
	set("exits", ([
        "west" : ([ "filename" : __DIR__"wizard.c",
                    "x_axis" : 15,
                    "y_axis" : 5,
                  ]),		
	]));

    set("objects", ([
    	__DIR__"npc/zhang.c" : 1,
    ]));

	setup();

	load_object("/daemon/board/newbie");

	replace_program(ROOM);
}
