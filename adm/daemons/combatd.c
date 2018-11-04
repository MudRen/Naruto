/*  combatd.c ս������
*/

#pragma save_binary

#include <ansi.h>
#include <combat.h>
#include <skill.h>

inherit F_CLEAN_UP;
inherit F_DBASE;

void create()
{
    seteuid(getuid());
    set("name", "ս������");
    set("id", "combatd");
}

string damage_msg(int damage)
{
	switch( damage ) {
	    case   0..2: return "���ֻ����ǿ���һ����΢�˺���"; break;
	    case   3..5: return "��������΢���˺���"; break;
	    case  6..10: return "������һ���˺���"; break;
	    case 11..20: return "������һ�������˺���"; break;
	    case 21..30: return "��������Ϊ���ص��˺���"; break;
	    case 31..40: return "�������൱���ص��˺���"; break;
	    case 41..60: return "������ʮ�����ص��˺���"; break;
	    case 61..80: return "�����ɼ������ص��˺���"; break;
	    default: return "�����ɷǳ����µ������˺���"; break;
	}
}

string status_msg(int ratio)
{
    switch( ratio ) {
		case    100: return HIG "( $N"HIG"��������û�����ˡ� )\n" NOR; break;
		case 96..99: return HIG "( $N"HIG"�ƺ����˵����ˣ�������������������� )\n" NOR; break;
		case 91..95: return HIG "( $N"HIG"�������������˵����ˡ� )\n" NOR; break;
		case 81..90: return HIG "( $N"HIG"���˼����ˣ������ƺ��������¡� )\n" NOR; break;
		case 61..80: return HIY "( $N"HIY"���˲��ᣬ������״������̫�á� )\n" NOR; break;
		case 41..60: return HIY "( $N"HIY"��Ϣ���أ�������ʼɢ�ң��������ܵ�����ʵ���ᡣ )\n" NOR; break;
		case 31..40: return HIY "( $N"HIY"�Ѿ��˺����ۣ���������֧����������ȥ�� )\n" NOR; break;
		case 21..30: return HIR "( $N"HIR"�����൱�ص��ˣ�ֻ�»�������Σ�ա� )\n" NOR; break;
		case 11..20: return HIR "( $N"HIR"����֮���Ѿ�����֧�ţ��ۿ���Ҫ���ڵ��ϡ� )\n" NOR; break;
		case  5..10: return HIR "( $N"HIR"���˹��أ��Ѿ�����һϢ�����ڵ�Ϧ�ˡ� )\n" NOR; break;
		default:     return HIR "( $N"HIR"���˹��أ��Ѿ�������в�����ʱ�����ܶ����� )\n" NOR; break;
    }    
}

varargs void report_status(object ob)
{
    if( !ob->query_stat_maximum("hp") ) return;
    message_vision( status_msg((int)ob->query_stat("hp")*100/(int)ob->query_stat_maximum("hp")), ob);
}

varargs int fight(object me, object victim, string skill, mapping action, object weapon)
{
    int ability;
    string msg;

    if( environment(me)->query("no_fight") && !victim->query("unprotect_mark") && !me->query("unprotect_mark") )
        return 0;

    // ս�����̶��ߣ�ֹͣս��
    if( ( userp(me) && !interactive(me) ) || ( userp(victim) && !interactive(victim) ) ) return 0;

    // �趨ս��ѶϢ
    me->set_combat_message(action["action"]);
    me->set_temp("last_action", action);

    me->set_temp("evade_message", 0);
    me->set_temp("defend_message", 0);
    
    // ������ = �������� + �������ܵȼ� + ��ʽ������
    ability = me->exact() + action["exact"];
    
    // ���������Ƿ�رܳɹ�
    if( !victim->is_busy() && victim->evade(ability, weapon ? weapon : me) ) {
        // �رܳɹ������رܵ�ѶϢ����ս��ѶϢ��
        string evade_msg;
        evade_msg = victim->query_temp("evade_message");
        me->add_combat_message("��" + (stringp(evade_msg) ? evade_msg : "����û��")  );

        // �����������m����
        if( userp(victim) && ability >= 40 ) victim->damage_stat("ap", random(ability/40)+1, me);
        else victim->damage_stat("ap", 1, me);

        ability = -1;
    } else {
    	// �ر�ʧ�ܣ����з���
    	string defend_msg;

        // �����˺���
        if( weapon ) ability = weapon->inflict_damage();
        else ability = me->inflict_damage();

        ability = victim->defend(ability, weapon ? weapon : me);

        // ���������m����
        if( userp(me) && ability >= 10) me->damage_stat("ap", random(ability/10) + 1, me);
        else me->damage_stat("ap", 1, me);

        // �������ѶϢ
        defend_msg = victim->query_temp("defend_message");
        me->add_combat_message("��" + (stringp(defend_msg) ? defend_msg : "$n�ٱ��м�") );
    }
    
    if( ability > 0 ) {
        victim->receive_damage(ability, me);
        // ���˺��̶ȵ�ѶϢ����ս��ѶϢ��
        me->add_combat_message("��" + damage_msg(ability) + "(" + ability+ ")\n");
    } else me->add_combat_message("��\n");

    // �ͳ�ս��ѶϢ��
    msg = me->get_combat_message();
    if( stringp(msg) ) {
        string *limbs = victim->query("limbs");
        if( !arrayp(limbs) || !sizeof(limbs) ) msg = replace_string(msg, "$l", "����");
        else msg = replace_string(msg, "$l", limbs[random(sizeof(limbs))]);
        if( weapon ) msg = replace_string(msg, "$w", weapon->name());
        message_vision( msg, me, victim);
        if( ability > 0 ) report_status(victim);
    }

    // ���ط���ֵ(�˺���)��ʾ�����߿ɻ�þ���
    if( living(victim) ) return ability;
    return 0;
}

void auto_fight(object me, object obj, string type)
{
    if( !userp(me) && !userp(obj) ) return;
    call_out( "start_" + type, 0, me, obj);
}

/* ��� */
void start_hatred(object me, object obj)
{
    if( !me || !obj ) return;

    if( !living(me) || (userp(obj) && !interactive(obj)) ) return;
    if( environment(me)->is_area() ) {
        if( !area_environment(me, obj) ) return;
    } else {
        if( environment(me) != environment(obj) ) return;
        if( environment(me)->query("no_fight") ) return;
    }

    if( me->is_fighting(obj) && obj->is_fighting(me) ) return;  // add by -Acme-

    message_vision( HIW"$N"HIW"��$n"HIW"������������ۺ죬���̴���������\n"NOR, me, obj);

    me->kill_ob(obj);
    if( userp(obj) ) obj->fight_ob(me);
    else obj->kill_ob(me);
//    me->attack(obj);
}

/* ���� */
void start_vendetta(object me, object obj)
{
    if( !me || !obj ) return;

    if( !living(me) || (userp(obj) && !interactive(obj)) ) return;
    if( environment(me)->is_area() ) {
        if( !area_environment(me, obj) ) return;
    } else {
        if( environment(me) != environment(obj) ) return;
        if( environment(me)->query("no_fight") ) return;
    }

    if( me->is_fighting(obj) && obj->is_fighting(me) ) return;  // add by -Acme-

    message_vision( HIW"$N"HIW"��$n"HIW"������������ۺ죬���̴���������\n"NOR, me, obj);

    me->kill_ob(obj);
    if( userp(obj) ) obj->fight_ob(me);
    else obj->kill_ob(me);
//    me->attack(obj);
}

/* ���� */
void start_aggressive(object me, object obj)
{
    int def;

    if( !me || !obj ) return;

    if( !living(me) || (userp(obj) && !interactive(obj)) ) return;
    if( environment(me)->is_area() ) {
        if( !area_environment(me, obj) ) return;
    } else {
        if( environment(me) != environment(obj) ) return;
        if( environment(me)->query("no_fight") ) return;
    }

    message_vision( HIW"$N"HIW"һ����$n"HIW"������˵�����̾ʹ���������\n" NOR, me, obj);

    if( me->is_fighting(obj) && obj->is_fighting(me) ) return;  // add by -Acme-

    me->kill_ob(obj);
    if( userp(obj) ) obj->fight_ob(me);
    else obj->kill_ob(me);
//    me->attack(obj);
}

void announce(object ob, string event)
{
    switch(event) {
	    case "dead":
    		message_vision("\n$N�����˼������ȳ�һ����Ѫ�����������\n\n", ob);
	        break;
	    case "unconcious":
    	    message_vision("\n$N����һ�����ȩo���ڵ���һ��Ҳ�����ˡ�\n\n", ob);
        	break;
	    case "revive":
    	    message_vision("\n$N���������۾��o�����˹�����\n\n", ob);
        	break;
    }
}