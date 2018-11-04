// fighter.c

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
            do_chat( name() + "笑著说道﹕嘿嘿 ... 来了帮手吗﹖\n");
            return 1;
        } else if( differ > 5 ) {
            do_chat( name() + "说道﹕嘿嘿 ... " + rank("self") + "今天要来个以寡击众﹗\n");
            return 1;
        } else {
            do_chat( name() + "说道﹕这位" + ob->rank("respectful") + "难道是想倚多为胜﹖\n");
            return 0;
        }
    }

    if( ob->is_fighting() ) {
	do_chat( name() + "说道：切磋武艺，还是单打独斗的好，我先在一旁掠阵吧。\n");
	return 0;
    }

    if( query_stat_current("kee") < query_stat_maximum("kee") ) {
	do_chat( name() + "说道：等会儿，让我喘口气。\n");
	return 0;
    }

    do_chat( name() + "说道﹕不敢﹐请赐招﹗\n");
    return 1;
}

void accept_kill(object ob)
{
    do_chat( name() + "说道﹕" + rank("self") + "与这位" + ob->rank("respectful")
        + "好像无冤无仇吧﹖怎么 ...﹖\n");
}

void accept_negotiation(object who, string key)
{
    switch(key) {
    case "peace":
        if( !is_killing(who) ) {
            do_chat( name() + "躬身还礼，说道：好说，好说。\n");
            remove_enemy(who);
            who->remove_enemy(this_object());
        }
        else do_chat( name() + "说道：废话少说，今日不是你死便是我活！\n");
        return;
    case "mercy":
        if( is_killing(who) ) {
            do_chat( name() + "哈哈大笑，说道：既然如此，今天就饶了你。\n");
            remove_killer(who);
            remove_enemy(who);
            who->remove_killer(this_object());
            who->remove_enemy(this_object());
	    // add by dragoon
	    if( who->query_class() == "fighter" )
                who->gain_score("martial fame", - who->query_level() * 10);
        }
    }
}
