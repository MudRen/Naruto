/* combat.c
*/

#include <attack.h>
#include <dbase.h>
#include <score.h>
#include <skill.h>
#include <statistic.h>
#include <combat.h>

static string combat_message = "";

void set_combat_message(string s) { combat_message = s; }
void add_combat_message(string s) { combat_message += s; }
string get_combat_message() { return combat_message; }

varargs int query_attr(string,int);

/* ���� */
varargs int query_ability(string application)
{
    if( !living(this_object()) ) return 0;

    switch(application) {
        case "attack":     // ��������
        	return query_attr("str") * 3 + query_temp("apply/attack");
        case "defend":     // ��������
        	return query_attr("con") * 3 + query_temp("apply/armor");
		case "exact":      // ��������
	        return query_attr("int") + query_attr("dex") + query_temp("apply/exact");
        case "evade":      // �ر�����
    	    return query_attr("int") + query_attr("dex") + query_temp("apply/evade");
    	case "intimidate": // ��������
    	    return query_attr("dex") + query_attr("str") + query_temp("apply/intimidate");
    	case "wittiness":  // ��������
    	    return query_attr("int") + query_attr("con") + query_temp("apply/wittiness");
    	case "move":       // �ƶ��ٶ�
    	    return query_attr("dex") + query_temp("apply/move");
        case "carriage":   // ��������
        	 return query_attr("str") * 3000 + query_temp("apply/carriage") * 1000;
	    default: return query_skill(application);
    }
}

/* ����ġ����С�������
*/
varargs int exact()
{
    int counter_ability;
    string sk;

    counter_ability = query_ability("exact");

    if( (sk=skill_mapped("combat")) )
        counter_ability += SKILL_D(sk)->combat_using(this_object(), counter_ability);

    return counter_ability;
}


/* int evade(int ability, object from)
   ��������ġ��رܡ�������
   ����ɫ�ܵ����������ħ������ʱ���ܹ��رܵ�������
   ����ֵ ability Ϊ����������, ��� from Ϊ�����ߡ�
   ���� 0 ���ɫ�޷��رܣ��� 0 ����ֵ��ʾ��ɫ�ɹ��Ļرܴ˴ι�����
*/
varargs int evade(int ability, object from)
{
	int amount, move;
    string sk;
    int counter_ability, chance;
    
	if( !living(this_object()) ) return 0;
	
	// �赹�޷��ر�
	if( this_object()->query_temp("block_msg/all") ) return 0;
	
	switch(from->query("phase")) {
	    case PHASE_PHYSICAL:		// ������
    	case PHASE_MENTAL:			// ������
	    case PHASE_ILLUSION:		// �þ���
    	case PHASE_ELEMENTAL:		// Ԫ����
            // �ر��� = �������� + �Ṧ����
    		counter_ability = query_ability("evade");
			if( (sk=skill_mapped("dodge")) )
	    		counter_ability += SKILL_D(sk)->dodge_using(this_object(), ability);
    		break;
        default: return 0;          // ĩ֪��
	}
	
    // �����������࣬���ͻر���
	move = this_object()->query_ability("move")/10;
    amount = sizeof(this_object()->query_enemy());
    if( amount > move ) counter_ability -= (counter_ability/10)*(amount-move);
	
	// û�о��Ե����л���ԵĻر�
	chance = 50 + (ability-counter_ability)/2;
	if( chance < 10 ) chance = 10;
	else if( chance > 90 ) chance = 90;
	return (random(100) > chance);
}

/* int defend(int ability, object from)
   �����������һ������ġ�������������
   ����ֵ damage Ϊ���ֹ�����, ��� from Ϊ�����ߡ�
*/
varargs int defend(int damage, object from)
{
    int move, amount;
	string sk;
    int counter_ability, chance;

    if( !living(this_object()) ) return damage;

	// �赹�޷�����
	if( this_object()->query_temp("block_msg/all") ) return damage;

	switch(from->query("phase")) {
	    case PHASE_PHYSICAL:		// ������
    	case PHASE_MENTAL:			// ������
	    case PHASE_ILLUSION:		// �þ���
    	case PHASE_ELEMENTAL:		// Ԫ����
            // ������ = �������� + ���м���
			counter_ability = query_ability("defend");
			if( (sk=skill_mapped("parry")) )
				counter_ability += SKILL_D(sk)->parry_using(this_object(), damage);
		    break;
        default: return damage;          // ĩ֪��
	}
    
    // �����������࣬���ͷ�������
	move = this_object()->query_ability("move")/10;
    amount = sizeof(this_object()->query_enemy());
    if( amount > move ) counter_ability -= (counter_ability/10)*(amount-move);

    // û�о��Եķ����򲻿ɷ���
    damage -= counter_ability;
    chance = 50 + damage/2;

    if( damage < 0 ) damage = 0;
	if( chance < 10 ) chance = 10;
	else if( chance > 90 ) chance = 90;

    // ������� ������
    if( damage ) {
    	// ���óɹ�
    	if( random(100) > chance ) return (damage/2)+random(damage/2)+1;
    	// ���ò��ɹ�
    	else return (damage*2/3) + random(damage/3)+1;

    // ��������ȫ��������
    } else {
    	// ���úܳɹ�
    	if( random(100) > chance ) return 0;
        // ���ò�̫�ɹ�
    	else return random(5)+1;
    }
}

// ��ʾ�˺�����(scoreָ����)
varargs int show_damage()
{
    int damage;
    string sk;

    damage = query_ability("attack");

	// ����(savage hit) Ӱ�칥���� �ӳ�
    if( (sk=skill_mapped("savage hit")) ) 
    	damage += SKILL_D(sk)->savage_hit_show(this_object(), damage);

    damage /= 2;

    // �ػ�(heavy hit) Ӱ�칥���� �ȶ���
    if( (sk=skill_mapped("heavy hit")) ) {
    	damage += SKILL_D(sk)->heavy_hit_show(this_object(), damage);
    }

    return damage;
}

// �����˺�(����������˺��̶�)
varargs int inflict_damage()
{
    int damage;
    string sk;

    damage = query_ability("attack");

	// ����(savage hit) Ӱ�칥���� �ӳ�
    if( (sk=skill_mapped("savage hit")) ) {
    	damage += SKILL_D(sk)->savage_hit_using(this_object(), damage);
    }

    damage /= 2;

    // �ػ�(heavy hit) Ӱ�칥���� �ȶ���
    if( (sk=skill_mapped("heavy hit")) ) {
    	damage += SKILL_D(sk)->heavy_hit_using(this_object(), damage);
    } else damage += random(damage);

    return damage;
}

// ���ܹ���
varargs int receive_damage(int damage, object from)
{
    int kill_mode = 0;
    mapping flag;
    string from_type;

    if( damage < 1 ) return 0;

    // ��������
    damage_stat("hp", damage, from);

    // ����ģʽ
    if(	!kill_mode && !from->is_killing(this_object()) && mapp(flag = query_exhausted()) && (flag["hp"] || flag["ap"] || flag["mp"]) ) {
        string stat;
        object ob;

        foreach(stat, ob in flag)
            set_stat_current(stat, 1);

		clear_statistic_flags();
        remove_all_enemy();

        call_out("lose_fight", 0, from);

        return damage;
    }

    return damage;
}