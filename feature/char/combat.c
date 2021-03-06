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

/* 能力 */
varargs int query_ability(string application)
{
    if( !living(this_object()) ) return 0;

    switch(application) {
        case "attack":     // 攻击力量
        	return query_attr("str") * 3 + query_temp("apply/attack");
        case "defend":     // 防御力量
        	return query_attr("con") * 3 + query_temp("apply/armor");
		case "exact":      // 命中能力
	        return query_attr("int") + query_attr("dex") + query_temp("apply/exact");
        case "evade":      // 回避能力
    	    return query_attr("int") + query_attr("dex") + query_temp("apply/evade");
    	case "intimidate": // 攻击技巧
    	    return query_attr("dex") + query_attr("str") + query_temp("apply/intimidate");
    	case "wittiness":  // 防御技巧
    	    return query_attr("int") + query_attr("con") + query_temp("apply/wittiness");
    	case "move":       // 移动速度
    	    return query_attr("dex") + query_temp("apply/move");
        case "carriage":   // 负重能力
        	 return query_attr("str") * 3000 + query_temp("apply/carriage") * 1000;
	    default: return query_skill(application);
    }
}

/* 人物的“命中”能力。
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
   定义人物的“回避”能力。
   当角色受到人物、武器、魔法攻击时，能够回避的能力。
   传入值 ability 为对手命中率, 物件 from 为攻击者。
   传回 0 表角色无法回避，非 0 的数值表示角色成功的回避此次攻击。
*/
varargs int evade(int ability, object from)
{
	int amount, move;
    string sk;
    int counter_ability, chance;
    
	if( !living(this_object()) ) return 0;
	
	// 昏倒无法回避
	if( this_object()->query_temp("block_msg/all") ) return 0;
	
	switch(from->query("phase")) {
	    case PHASE_PHYSICAL:		// 物理类
    	case PHASE_MENTAL:			// 精神类
	    case PHASE_ILLUSION:		// 幻觉类
    	case PHASE_ELEMENTAL:		// 元素类
            // 回避率 = 人物能力 + 轻功技能
    		counter_ability = query_ability("evade");
			if( (sk=skill_mapped("dodge")) )
	    		counter_ability += SKILL_D(sk)->dodge_using(this_object(), ability);
    		break;
        default: return 0;          // 末知类
	}
	
    // 敌人数量过多，降低回避率
	move = this_object()->query_ability("move")/10;
    amount = sizeof(this_object()->query_enemy());
    if( amount > move ) counter_ability -= (counter_ability/10)*(amount-move);
	
	// 没有绝对的命中或绝对的回避
	chance = 50 + (ability-counter_ability)/2;
	if( chance < 10 ) chance = 10;
	else if( chance > 90 ) chance = 90;
	return (random(100) > chance);
}

/* int defend(int ability, object from)
   这个函数定义一个人物的“防御”能力。
   传入值 damage 为对手攻击力, 物件 from 为攻击者。
*/
varargs int defend(int damage, object from)
{
    int move, amount;
	string sk;
    int counter_ability, chance;

    if( !living(this_object()) ) return damage;

	// 昏倒无法防御
	if( this_object()->query_temp("block_msg/all") ) return damage;

	switch(from->query("phase")) {
	    case PHASE_PHYSICAL:		// 物理类
    	case PHASE_MENTAL:			// 精神类
	    case PHASE_ILLUSION:		// 幻觉类
    	case PHASE_ELEMENTAL:		// 元素类
            // 防御力 = 人物能力 + 拆招技能
			counter_ability = query_ability("defend");
			if( (sk=skill_mapped("parry")) )
				counter_ability += SKILL_D(sk)->parry_using(this_object(), damage);
		    break;
        default: return damage;          // 末知类
	}
    
    // 敌人数量过多，降低防御力道
	move = this_object()->query_ability("move")/10;
    amount = sizeof(this_object()->query_enemy());
    if( amount > move ) counter_ability -= (counter_ability/10)*(amount-move);

    // 没有绝对的防御或不可防御
    damage -= counter_ability;
    chance = 50 + damage/2;

    if( damage < 0 ) damage = 0;
	if( chance < 10 ) chance = 10;
	else if( chance > 90 ) chance = 90;

    // 如果还有 攻击力
    if( damage ) {
    	// 防得成功
    	if( random(100) > chance ) return (damage/2)+random(damage/2)+1;
    	// 防得不成功
    	else return (damage*2/3) + random(damage/3)+1;

    // 攻击力完全被消减掉
    } else {
    	// 防得很成功
    	if( random(100) > chance ) return 0;
        // 防得不太成功
    	else return random(5)+1;
    }
}

// 显示伤害能力(score指令用)
varargs int show_damage()
{
    int damage;
    string sk;

    damage = query_ability("attack");

	// 暴击(savage hit) 影响攻击力 加乘
    if( (sk=skill_mapped("savage hit")) ) 
    	damage += SKILL_D(sk)->savage_hit_show(this_object(), damage);

    damage /= 2;

    // 重击(heavy hit) 影响攻击力 稳定度
    if( (sk=skill_mapped("heavy hit")) ) {
    	damage += SKILL_D(sk)->heavy_hit_show(this_object(), damage);
    }

    return damage;
}

// 给予伤害(计算产生的伤害程度)
varargs int inflict_damage()
{
    int damage;
    string sk;

    damage = query_ability("attack");

	// 暴击(savage hit) 影响攻击力 加乘
    if( (sk=skill_mapped("savage hit")) ) {
    	damage += SKILL_D(sk)->savage_hit_using(this_object(), damage);
    }

    damage /= 2;

    // 重击(heavy hit) 影响攻击力 稳定度
    if( (sk=skill_mapped("heavy hit")) ) {
    	damage += SKILL_D(sk)->heavy_hit_using(this_object(), damage);
    } else damage += random(damage);

    return damage;
}

// 接受攻击
varargs int receive_damage(int damage, object from)
{
    int kill_mode = 0;
    mapping flag;
    string from_type;

    if( damage < 1 ) return 0;

    // 消耗体力
    damage_stat("hp", damage, from);

    // 比武模式
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