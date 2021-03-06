// write by -Acme-
/* shadow.c
   分身：不会攻击，不会回避
   影分身：会攻击，会回避
*/

#include <npc.h>

inherit F_VILLAGER;

object owner;
int attack = 0;        // 可攻击与否
int evade = 0;         // 可回避与否

void set_attack(int i) { attack = i; }
void set_evade(int i) { attack = i; }

// 攻击
varargs int attack(object opponent)
{
    if( !attack ) return 0;
    return ::attack(opponent);
}

// 回避
varargs int evade(int ability, object from)
{
	if( !evade ) return 0;
	return ::evade(ability, from);
}

// 设定拥有者
int set_owner(object obj)
{
    if( !objectp(obj) ) return 0;
    owner = obj;
}

// 查询拥有者
object query_owner()
{
    if( !objectp(owner) ) return 0;
    return owner;
}

// 删除影分身
void do_destruct()
{
    message_vision("附近传来“崩”一声，$N就消失不见了...\n", this_object());
    destruct(this_object());
	return;
}

// 随时检查拥有者是否有足够查克拉，并且吸取
void do_check()
{
    // 拥有者消失，则影分身消失
    if( !objectp(owner) ) {
    	do_destruct();
        return;
    }
    
    // 拥有者的查克拉不足
    if( owner->query_stat("mp") < this_object()->query_level() ) {
    	do_destruct();
        return;
    }

    // 拥有者消秏一些查克拉
    owner->damage_stat("mp", this_object()->query_level() , owner);

    // 拥有者增加技能经验
    if( this_object()->query("skill") )
        owner->improve_skill(this_object()->query("skill"), 1);

    return;
}

void die()
{
    message_vision("附近传来“崩”一声，$N就消失不见了...\n", this_object());
    return;
}

// 是否接受命令
int accept_order(object who, string cmd)
{
    return 0;  // 分身是不受控制的
}

void create()
{
    set_name("影分身", ({ "shadow" }));
    set_race("human");

    // 平时动作：检查owner还在不在，并吸取其查克拉
    set("chat_chance", 3);
    set("chat_msg", ({
        (: do_check :),
    }));

    // 战斗时动作：检查owner还在不在，并吸取其查克拉
    set("chat_chance_combat", 5);
    set("chat_msg_combat", ({
        (: do_check :),
    }) );

    setup();
}