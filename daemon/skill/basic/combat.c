#include <ansi.h>
inherit SKILL;

mapping *combat_actions = ({
    ([ "action": "$N�����е�$w��$n$l��ȥ", ]),
    ([ "action": "$N�Ӷ�$w����$n$l��ȥ", ]),
    ([ "action": "$N������$w��׼$n$l��ȥ", ]),
    ([ "action": "$N�Ӷ�����$w������$n$l", ]),
    ([ "action": "$NѰ����϶��һ��������ǰ����$w����$n$l����", ]),
});

string *interattack = ({
    "$N�Ż���������\n",
});

string query_class() { return "basic"; }

void create()
{
    seteuid(getuid());
    DAEMON_D->register_skill_daemon("combat");
    setup();
}

// ʹ��ս������ (Ӱ��������)
int combat_using(object me, int ability)
{
    return me->query_skill("combat");
}

void attack_using(object me, object opponent, string skill, object weapon)
{
    int damage,sk;

    if( !me->skill_mapped(skill) ) return 0;

    if( !opponent ) {
        if( me->query_temp("last_attacked_target") )
	    message_vision(CYN + interattack[random(sizeof(interattack))] + NOR, me, weapon);
        return;
    }

    if( !weapon )
        damage = COMBAT_D->fight(me, opponent, skill , me->query("default_actions"), me);
    else
        damage = COMBAT_D->fight(me, opponent, skill , combat_actions[random(sizeof(combat_actions))], weapon);

   if( damage > 0 ) {
     	// ���ܾ���
        me->improve_skill(skill, 1);
        // ��������
        me->gain_score("combat exp", 1);
    }
}

// �������Ӿ���, ������������
void skill_improved(object me, string skill) { }

// �������������д˺�ʽ����Ҫ��
void skill_advanced(object me, string skill)
{
    // �������趨�����´���������ľ���

    int lv;

    lv = me->query_skill(skill, 1);					// ���ܵȼ�

    switch( lv ) {
    	case 189: me->set_skill_require(skill, 3000000); break; // �ؿ�3 : 189��190
    	case 149: me->set_skill_require(skill, 2000000); break; // �ؿ�2 : 149��150
    	case  99: me->set_skill_require(skill, 1000000); break; // �ؿ�1 :  99��100
    	default:  me->set_skill_require(skill, lv*lv*15); break;
    }
}