/*  human.c - human race

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#define BASE_WEIGHT 60000

#include <ansi.h>
#include <statistic.h>
#include <race.h>

inherit HUMANOID;

mapping *combat_actions = ({
    ([ "action": "$N��ȭ����$n��$l", ]),
    ([ "action": "$N��$n��$lһץ", ]),
    ([ "action": "$N��$n��$l�ݺݵ�����һ��", ]),
    ([ "action": "$N����ȭͷ��$n��$l��ȥ", ]),
    ([ "action": "$N��׼$n��$l�����ӳ�һȭ", ]),
});

mapping query_action()
{
    return combat_actions[random(sizeof(combat_actions))];
}

void create()
{
    ::create();

    seteuid(getuid());
	
    // ����������
    set("civilized", 1);    // human is a civilized humanoid tribe.

//    set("commoner_score_base", 100);    // score base for gaining level

    set("limbs", ({
        "ͷ��", "����", "�ؿ�", "����", "���", "�Ҽ�", "���", "�ұ�",
        "����", "����", "����", "С��", "����", "����", "���", "�ҽ�"
    }) );

    set("default_actions", (: call_other, __FILE__, "query_action" :) );

    DAEMON_D->register_race_daemon("human");
}


void setup(object ob)
{
    ::setup(ob);

    ob->set_default_object(__FILE__);

    // ��������
    if( !ob->query_weight() || ob->query_weight() < 0 ) ob->set_weight(BASE_WEIGHT);

    // ��������
    ob->add_temp("apply/armor", 5);
}

void initialize(object ob)
{
    ::initialize(ob);

    if( !ob->query("age") ) ob->set("age", 18);

    ob->set_default_object(__FILE__);
}