#include <ansi.h>
#include <combat.h>
inherit F_CLEAN_UP;
void create() { seteuid(ROOT_UID); }

int main(object me, string arg)
{
	object ob;
	mapping score;
	string line;
	int i;

	if(!arg) ob = me;

	else {
		ob = present(arg, environment(me));
		if (!ob) ob = find_player(arg);
		if (!ob) ob = find_living(arg);
		if (!ob) return notify_fail("你要察看谁的状态﹖\n");
		if (!wizardp(me) && !ob->accept_info(me, "score") )
			return notify_fail("只有巫师能察看别人的状态。\n");
	}
	score = ob->query("score");
	if( mapp(score) ) {
		string *c;
		int xp;
        line  = "╭───────────────────────────────────╮\n";
		line += "│人物经验                                                              │\n";
        line += "╰───────────────────────────────────╯\n";
		c = sort_array(keys(score), 1);
		for(i = 0; i < sizeof(c); i++)
			if((xp = (int)ob->query_target_score(c[i]) - score[c[i]]) > 0) {
				line += sprintf("  %11s "HIC"%9d %-11s%s"NOR,
					to_chinese(c[i]),
					score[c[i]],
					"[" + xp + "]",
					i%2 ? "\n" : "" );
			} else {
				line += sprintf("  %11s %9d %-11s%s",
					to_chinese(c[i]),
					score[c[i]],
					"",
					i%2 ? "\n" : "" );
			}
		if( i%2==1 ) line += "\n";
        line += " ────────────────────────────────────\n";
	}
	write(line);
	return 1;
}
int help(object me)
{
        write(@HELP
指令格式 : exp
           exp <对象名称>(巫师专用)

这个指令可以查询人物的经验。 
 
相关指令：score
HELP
    );
    return 1;
}
