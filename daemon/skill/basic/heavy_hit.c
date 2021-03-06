/* heavy_hit.c 重击
*/

#include <ansi.h>
inherit SKILL;

string query_class() { return "basic"; }

void create()
{
    seteuid(getuid());
    DAEMON_D->register_skill_daemon("heavy hit");
    setup();
}

// 显示用
int heavy_hit_show(object me, int ability)
{
	int stable;

    if( !me ) return 0;
    if( !me->skill_mapped("heavy hit") ) return 0;

    stable = me->query_skill("heavy hit");

    ability = (stable * ability/200) + ((stable * ability/2+1)/200);

    return ability;
}

// 使用重击
int heavy_hit_using(object me, int ability)
{
	int stable;
	
    if( !me ) return 0;
    if( !me->skill_mapped("heavy hit") ) return 0;

    stable = me->query_skill("heavy hit");
    
    ability = (stable * ability/200) + ((stable * random(ability)+1)/200);

    // 增加经验
    me->improve_skill("heavy hit", 1);

    return ability;
}

// 技能增加经验, 不需做其它事
void skill_improved(object me, string skill) { }

// 技能升级后会呼叫此函式，必要！
void skill_advanced(object me, string skill)
{
    // 在这里设定技能下次升级所需的经验

    int lv;

    lv = me->query_skill(skill, 1);					// 技能等级

    switch( lv ) {
    	case 189: me->set_skill_require(skill, 3000000); break; // 关卡3 : 189升190
    	case 149: me->set_skill_require(skill, 2000000); break; // 关卡2 : 149升150
    	case  99: me->set_skill_require(skill, 1000000); break; // 关卡1 :  99升100
    	default:  me->set_skill_require(skill, lv*lv*15); break;
    }
}