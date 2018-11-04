inherit ROOM;

void create()
{
	set("short", "白魔法阵");
	set("long", @LONG
在这个房间充满著一股坚固的白魔法防御术，你可以在这里畅所
欲言，绝对不会受到穷凶极恶的蟑螂部队威胁。
LONG
	);
	set("exits", ([
		"west" : __DIR__"guildhall",
		"east" : ([ "filename" : __DIR__"wizard.c",
					 "x_axis" : 13,
					 "y_axis" : 5,
				  ]),
	]));

	setup();
	load_object("/daemon/board/propose");
	replace_program(ROOM);
}