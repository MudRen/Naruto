#include <ansi.h>

inherit BULLETIN_BOARD;

void create()
{
	set_name(HIW"Î×Ê¦¹«¸æ°å"NOR, ({ "wizard board", "board" }) );
	set("location", "/world/area/wizard/guildhall");
	set("board_id", "wizard");
	set("NO_PLAYER_POST", 1);
	setup();
	set("capacity", 200);
	replace_program(BULLETIN_BOARD);
}
