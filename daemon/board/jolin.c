inherit BULLETIN_BOARD;

void create()
{
        set_name("°ÚÀÏ´óÊ÷", ({ "old tree","tree" }) );
        set("location", "/u/j/jolin/workroom");
        set("board_id", "jolin");
        set("board_owner","jolin");
        setup();
        set("capacity", 100);
        replace_program(BULLETIN_BOARD);
}
