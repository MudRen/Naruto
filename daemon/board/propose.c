
inherit BULLETIN_BOARD;

void create()
{
	set_name("�������۰�", ({ "propose board", "board" }) );
	set("location", "/world/area/wizard/propose");
	set("board_id", "propose");
	setup();
	set("capacity", 200);
	replace_program(BULLETIN_BOARD);
}
