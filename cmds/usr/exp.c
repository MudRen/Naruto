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
		if (!ob) return notify_fail("��Ҫ�쿴˭��״̬�t\n");
		if (!wizardp(me) && !ob->accept_info(me, "score") )
			return notify_fail("ֻ����ʦ�ܲ쿴���˵�״̬��\n");
	}
	score = ob->query("score");
	if( mapp(score) ) {
		string *c;
		int xp;
        line  = "�q�����������������������������������������������������������������������r\n";
		line += "�����ﾭ��                                                              ��\n";
        line += "�t�����������������������������������������������������������������������s\n";
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
        line += " ������������������������������������������������������������������������\n";
	}
	write(line);
	return 1;
}
int help(object me)
{
        write(@HELP
ָ���ʽ : exp
           exp <��������>(��ʦר��)

���ָ����Բ�ѯ����ľ��顣 
 
���ָ�score
HELP
    );
    return 1;
}
