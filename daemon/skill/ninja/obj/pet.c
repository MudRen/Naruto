// write by -Acme-
/* pet.c 宠物：忍犬 */

#include <ansi.h>
#include <npc.h>

inherit F_VILLAGER;

object owner;

int attack = 0;         // 可攻击与否
int evade = 0;          // 可回避与否
int food_maximum = 999; // 食物量上限
int food = 0;           // 目前食物量
int exp = 0;            // 经验值

void set_attack(int i) { attack = i; }
void set_evade(int i) { attack = i; }

int query_food_maximum() { return food_maximum; }
int query_food() { return food; }
int query_exp() { return exp; }

// 食物量增减
int food_gain(int fo)
{
	if( food + fo > food_maximum ) return 0;
	food += fo;
	return 1;
}

// 经验值获得
void exp_gain(int ex)
{
	if( this_object()->query_level() >= 50 ) return;

	exp += ex;
	
    // 升级检查
	if( this_object()->query_level() * this_object()->query_level() * 10 <= exp ) {
        exp = 0;
        this_object()->set_level(this_object()->query_level()+1);
        tell_object(owner, HIY"你的忍犬 " + this_object()->query("name") + " 的等级提升至 " + this_object()->query_level() + " 级。\n"NOR);
	}

	return;
}

// 攻击
varargs int attack(object opponent)
{
    if( !attack ) return 0;

    // 有机会增加经验值
    if( random(100) > random(100) ) exp_gain(1);

    return ::attack(opponent);
}

// 回避
varargs int evade(int ability, object from)
{
	if( !evade ) return 0;

    // 有机会减少食物量
    if( random(100) > random(100) ) food_gain(-1);

	return ::evade(ability, from);
}

// 设定主人
int set_owner(object obj)
{
    if( !objectp(obj) ) return 0;
    owner = obj;
}

// 查询主人
object query_owner()
{
    if( !objectp(owner) ) return 0;
    return owner;
}

// 删除忍犬
void do_destruct()
{
    message_vision("$N一溜烟的就跑得无影无踪了...\n", this_object());
    destruct(this_object());
	return;
}

// 随时检查主人是否存在
void do_check()
{
    // 主人消失，则忍犬也消失
    if( !objectp(owner) ) {
    	do_destruct();
        return;
    }
    
    // 食物量检查
    if( food <= 0 ) {
    	tell_object(owner, HIR"你的忍犬 " + this_object()->query("name") + " 似乎肚子饿得很...\n"NOR);
        this_object()->damage_stat("ap", 10, this_object()); // 扣精神
        this_object()->damage_stat("hp", 10, this_object()); // 扣体力
        this_object()->damage_stat("mp", 10, this_object()); // 扣查克拉
    	return;
    }
    
    // 减少食物量
    food -= random(3) + 1;
    
    // 忍犬恢复状态比较快
    this_object()->heal_stat("ap", this_object()->query_level());
    this_object()->heal_stat("hp", this_object()->query_level());
    this_object()->heal_stat("mp", this_object()->query_level());

    return;
}

void die()
{
    message_vision("$N惨叫“该～～该～～”后倒了下来...\n", this_object());
    exp = 0;
    return;
}

// 是否接受命令
int accept_order(object who, string cmd)
{
	if( owner != who ) return 0;

	// 驯兽术必需大于150级，才会受控制
	if( who->query_skill("animal taming", 1) <= 150 ) return 0;

	return 1;
}
	
void create()
{
    set_name("忍犬", ({ "dog" }));
    set_race("beast");

    // 平时动作：检查owner还在不在
    set("chat_chance", 1);
    set("chat_msg", ({
        (: do_check :),
    }));

    // 战斗时动作：检查owner还在不在
    set("chat_chance_combat", 1);
    set("chat_msg_combat", ({
        (: do_check :),
    }) );

    setup();
}