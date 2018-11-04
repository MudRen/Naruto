
inherit ROOM;

void create()
{
	set("short", "巫师学院");
	set("long", @LONG
这里是巫师们研究讨论这个世界未来的场所，你可以看到许多巫
师比手划脚地高谈阔论著自已的想法，你也可以一同参与他们的讨论
，让这个世界更美好。
LONG
	);
	set("exits", ([
		"south" : __DIR__"guildhall",
	]));

	setup();
	load_object("/daemon/board/design");
	replace_program(ROOM);
}