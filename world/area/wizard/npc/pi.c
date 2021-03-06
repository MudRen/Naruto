#include <npc.h>

inherit F_SOLDIER;

void create()
{
    set_name("宕搁辟", ({ "dang ge pi", "pi" , "__GUARD__" }) );

    set_class("superior");  // 上忍
    set_level(55);

    set("title", "神兽之森守卫");

    set("long", "一个守护的神兽之森的上忍，防止不知情的人误闯进去，也保护人们\n"
                "不受神兽之森里的野兽攻击。\n");

    // 守著出口
    set("guard_exit", ({"east", "northeast", "southeast"}));

    // 世仇：神界
    set("vendetta_mark", "wizard");

    setup();

    // 人物本身的技能设定
    set_skill("dodge", 200);               // 基本： 回避技巧
    set_skill("parry", 200);               // 基本： 防御技巧
    set_skill("combat", 200);              // 基本： 格斗技巧
    set_skill("savage hit", 200);          // 基本： 暴击
    set_skill("heavy hit", 200);           // 基本： 重击
    set_skill("force", 200);               // 基本： 体术
    set_skill("ninja", 10);                // 基本： 忍术
    set_skill("magic", 10);                // 基本： 幻术
}

int do_guard_exit(int x, int y, string exit)
{
	message_vision("$n伸手将$N拦住。\n", this_player(), this_object());
    do_chat((: command, "say 神兽森林里的野兽凶恶得很，你最好别进去。":));
    return 1;
}

int accept_fight(object ob)
{
    do_chat((: command, "say 比武？我可没空，你去找别人试试吧。":));
    return 0;
}
