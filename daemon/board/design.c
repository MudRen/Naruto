
inherit BULLETIN_BOARD;

void create()
{
	set_name("������۰�", ({ "design board", "board" }) );
	set("location", "/world/area/wizard/academy");
	set("board_id", "design");
	setup();
	set("capacity", 200);
	replace_program(BULLETIN_BOARD);
}
