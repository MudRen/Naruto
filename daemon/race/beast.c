// beast.c

#define BASE_WEIGHT 10000

#include <ansi.h>
#include <statistic.h>
#include <race.h>

inherit HUMANOID;

mapping *combat_actions = ({
	([ "action": "$N������������$n$lҧȥ", ]),
	([ "action": "$N��$n$l���˹�ȥ�o�����ҧ", ]),
	([ "action": "$N��צ����$n$l�ݺ�һץ", ]),
	([ "action": "$N��������o�ܲ��������$n$l�ݺ�һҧ", ]),
});

mapping query_action()
{
    return combat_actions[random(sizeof(combat_actions))];
}

void create()
{
    ::create();

    seteuid(getuid());

    set("unit", "ֻ");

    set("gender", "male-animal");

    set("limbs", ({ "ͷ��", "����" }) );

    set("default_actions", (: call_other, __FILE__, "query_action" :) );

    DAEMON_D->register_race_daemon("beast");
}

void setup(object ob)
{
    ::setup(ob);

	ob->set_default_object(__FILE__);
	
	if( !ob->query_weight() ) ob->set_weight(BASE_WEIGHT);

	// Natural armor against physical damage.
	ob->set_temp("apply/armor", 3);
}

void initialize(object ob)
{
    seteuid(geteuid(ob));

    if( !ob->query("age") )
	ob->set("age", 1);
    ob->set_default_object(__FILE__);
}
/*
// Ұ���ǲ�����������
int valid_wield(object me, object ob, string skill) { return 1; }

// Ұ���ǲ��ᴩ���ߵ�
int valid_wear(object me, object ob, string part) {	return 1; }
*/