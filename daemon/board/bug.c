
inherit BULLETIN_BOARD;

void create()
{
	set_name("����ر���", ({ "bug board", "board" }) );
	set("location", "/world/area/wizard/bug");
	set("board_id", "bug");
	setup();
	set("capacity", 200);
	replace_program(BULLETIN_BOARD);
}
