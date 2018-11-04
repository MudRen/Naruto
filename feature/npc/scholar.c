/*  scholar.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <attack.h>
#include <statistic.h>

// prototypes
mixed do_chat(mixed chat);
varargs string name(int raw);
void kill_ob(object ob);
varargs int is_fighting(object ob);
int query_level();
string rank(string politeness);

int accept_fight(object ob)
{
    if( is_fighting() ) {
        int differ;

        differ = query_level() - (int)ob->query_level();
        if( differ > 10 ) {
            do_chat( name() + "Ц��˵����" + ob->rank("respectful")
        	+ "���ٴ���" + rank("self") + "�Ե���̡�\n");
            return 0;
        } else {
            do_chat( name() + "˵���s��λ" + ob->rank("respectful") + "�ѵ������ж�Ϊʤ�t\n");
            return 0;
        }
    }

    if( ob->is_fighting() ) {
	do_chat( name() + "˵�����д����գ����ǵ�������ĺã�������һ������ɡ�\n");
	return 0;
    }

    if( query_stat_current("kee") < query_stat_maximum("kee") ) {
	do_chat( name() + "˵�����Ȼ�������Ҵ�������\n");
	return 0;
    }

    do_chat( name() + "˵���s���ҩo����Щu\n");
    return 1;
}

void
accept_kill(object ob)
{
    do_chat( name() + "˵���s��֪" + rank("self") + "��ε�������λ"
	+ ob->rank("respectful") + "��Ϊʲô��Ҫ���ֲ����أ�\n");
}

void accept_negotiation(object who, string key)
{
    switch(key) {
    case "peace":
        if( !is_killing(who) ) {
            do_chat( name() + "������˵������˵����˵��\n");
            remove_enemy(who);
            who->remove_enemy(this_object());
        }
        else do_chat( name() + "˵�����ϻ���˵�����ղ������������һ\n");
        return;
    case "mercy":
        if( is_killing(who) ) {
            do_chat( name() + "������Ц��˵������Ȼ��ˣ�����������㡣\n");
            remove_killer(who);
            remove_enemy(who);
            who->remove_killer(this_object());
            who->remove_enemy(this_object());
            who->gain_score("martial fame", - who->query_level() * 10);
        }
    }
}

void
win_fight(object ob)
{
    do_chat( name() + "����˵�������ã���λ" + ob->rank("respectful")
	+ "�������飬" + rank("self") + "����ʢ�顣\n");
}
