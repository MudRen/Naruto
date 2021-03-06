/*  statistic.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <ansi.h>
#include <dbase.h>
#include <login.h>
#include <move.h>
#include <statistic.h>

// variables

static int regenerating = 1;

mapping st_maximum = ([]);
mapping st_current = ([]);
mapping st_notify = ([]);
mapping st_point = ([]);
mapping st_quest = ([]);

static mapping st_regenerator;
static object last_from_ob;
static mapping f_exhausted = ([]);
static mapping f_destroyed = ([]);
static mapping f_notified = ([]);

// implementations

string* query_stat_name() { return mapp(st_maximum) ? keys(st_maximum) : ({}); }
object last_damage_giver() { return last_from_ob; }
mapping query_exhausted() { return f_exhausted; }
mapping query_destroyed() { return f_destroyed; }
mapping query_notified() { return f_notified; }

void clear_statistic_flags()
{
    f_exhausted = ([]);
    f_destroyed = ([]);
    f_notified = ([]);
}

int query_point(string what)
{
	if( mapp(st_point) && !undefinedp(st_point[what]) ) return st_point[what];
}

void add_point(string what, int value)
{
    if( mapp(st_point) ) st_point[what] += value;
}

mapping query_all_quest()
{
	return st_quest;
}

int query_quest(string what, int id)
{
	if( mapp(st_quest) && !undefinedp(st_quest[what]) ) 
        return st_quest[what][id];
}

void add_quest(string what, int id, int value)
{
	if( undefinedp(st_quest[what]) ) st_quest[what] = ([]);
    st_quest[what][id] += value;
}

int delete_quest(string what, int id)
{
	if( !mapp(st_quest) && undefinedp(st_quest[what]) ) return 1;
    map_delete(st_quest[what], id);
    if( !sizeof(st_quest[what]) ) map_delete(st_quest, what);
    return 1;
}

/*  query_stat()
 *
 *  取得某一项基本数值。
 *
 *  每一个基本数值都由三个数字来定义：
 *
 *	目前值 (current)
 *	状态值 (effective)
 *	上限值 (maximum)
 *
 *  因为各种人物的生命型态不同，这三个数字可能有未定义的状态。
 */
int query_stat(string what)
{
    int val;
    if( mapp(st_current) && !undefinedp(val=st_current[what]) ) return val;
    if( mapp(st_maximum) && !undefinedp(val=st_maximum[what]) ) return val;
    return 0;
}

void delete_stat(string what)
{
    if( mapp(st_current) )		map_delete(st_current, what);
    if( mapp(st_maximum) )		map_delete(st_maximum, what);
    if( mapp(st_notify) )		map_delete(st_notify, what);
    if( mapp(st_regenerator) )	map_delete(st_regenerator, what);
}

/*  取得基本数值的资讯
 */
int query_stat_current(string what)
    { return mapp(st_current) ? st_current[what] : 0; }
    
int query_stat_maximum(string what)
    { return mapp(st_maximum) ? st_maximum[what] + this_object()->query_temp("apply/"+what) : 0; }
    
int query_stat_notify(string what)
    { return mapp(st_notify) ? st_notify[what] : 0; }
    
mixed query_stat_regenerate(string what)
    { return mapp(st_regenerator) ? st_regenerator[what] : 0; }

/*  设定基本数值
 */
int set_stat_current(string what, int val)
    { if( val > 9999 ) return 0; return mapp(st_current) ? st_current[what] = val : 0; }

int set_stat_maximum(string what, int val)
    { if( val > 9999 ) return 0; return mapp(st_maximum) ? st_maximum[what] = val : 0; }

int set_stat_notify(string what, int val)
    { if( val > 9999 ) return 0; return mapp(st_notify) ? st_notify[what] = val : 0; }

int advance_stat(string what, int val)
{
    if( !mapp(st_maximum) || undefinedp(st_maximum[what]) ) return 0;
    return set_stat_maximum(what, (st_maximum[what]+val));
}

/*  set_stat_regenerate()
 *
 *  设定基本数值的更新方式。
 */

mixed set_stat_regenerate(string what, mixed val)
{
    int max;

    /* 如果这个基本数值的最大值未定义，则不能设定更新方式。 */
    if( !(max = query_stat_maximum(what)) ) return 0;

    if( functionp(val) ) return (st_regenerator[what] = val);

    switch(val) {
		// 这项基本数值是不会自动更新的
    	case TYPE_STATIC: return (st_regenerator[what] = 0);
    	
		// 这项基本数值是会随时间自动恢复的
    	case TYPE_HEALTH:
			if( undefinedp(query_stat_current(what)) ) set_stat_current(what, max);
			return (st_regenerator[what] = (: health_regenerator :) );
			
		// 这项基本数值是会随时间慢慢减少的
    	case TYPE_WASTING:
			if( undefinedp(query_stat_current(what)) ) set_stat_current(what, max);
			return (st_regenerator[what] = (: wasting_regenerator :) );
			
    	default: error("undefined regenerate method.\n");
    }
    return 0;
}

/* 初始化人物的基本数值 */

void init_statistic(mapping base)
{
    mapping st;
    string s;
    int val;

    if( mapp(st = query("statistic/maximum")) ) {
		st_maximum = st;
		delete("statistic/maximum");
    }

    if( mapp(st = query("statistic/current")) ) {
		st_current = st;
		delete("statistic/current");
    }

    if( mapp(st = query("statistic/notify")) ) {
		st_notify = st;
		delete("statistic/notify");
    }

    if( mapp(st = query("statistic")) ) delete("statistic");

    if( !mapp(st_maximum) ) st_maximum = allocate_mapping(NUM_STATISTICS);
    if( !mapp(st_current) ) st_current = allocate_mapping(NUM_STATISTICS);
    if( !mapp(st_notify) ) st_notify = allocate_mapping(NUM_STATISTICS);
    if( !mapp(st_regenerator) ) st_regenerator = allocate_mapping(NUM_STATISTICS);

    if( mapp(base) && sizeof(base) )
	foreach(s, val in base)
	    if( undefinedp(st_maximum[s]) ) st_maximum[s] = val;
}

varargs int damage_stat(string type, int damage, object who)
{
    if( damage < 0 ) error("damage less than zero.\n");
    if( damage == 0 ) return 0;

    /* 若未指定这个伤害来自哪一个生物，使用以下规则猜测 */
    if( !objectp(who) ) {
        if( this_player() ) who = this_player();
        else if( previous_object() && previous_object()->is_character() ) who = previous_object();
        else who = this_object();
    }

    /* 如果这种基本数值不存在，直接返回以节省时间 */
    if( !mapp(st_current) || undefinedp(st_current[type]) || f_exhausted[type] ) return 0;

    last_from_ob = who;
    st_current[type] -= damage;

    /* 若这个基本数值的目前值已经耗尽，设定记号等 heart_beat 时处理 */
    if( st_current[type] < 0 ) {
        st_current[type] = 0;
        f_exhausted[type] = who;
    }

    /* 若低于设定警示的百分比，设定记号等 heart_beat 时处理 */
    if( who && mapp(st_notify) && !undefinedp(st_notify[type])) {
        if( mapp(st_maximum) && !undefinedp(st_maximum[type]) &&
            st_maximum[type] > 0 &&
            st_current[type]*100/query_stat_maximum(type) < st_notify[type] )
            //st_current[type]*100/st_maximum[type] < st_notify[type] )
            f_notified[type] = who;
    }
    start_regenerate();
    return damage;
}

int heal_stat(string type, int heal)
{
    int old_stat;

    if( heal < 0 ) error("heal less than zero.\n");

    if( !mapp(st_current) || undefinedp(st_current[type]) ) return 0;

    old_stat = st_current[type];
    st_current[type] += heal;

    if( mapp(st_maximum) && st_current[type] > query_stat_maximum(type) )
	    st_current[type] = query_stat_maximum(type);

    start_regenerate();
    return st_current[type] - old_stat;
}

// start_regenerate()
//
// This function starts regenerating manually. The regenerating is on by
// default when characters inherits F_STATISTIC was created. It can be
// turned off by regenerate() eventually. If you want force regenerating
// to start, call this function.
void start_regenerate() { regenerating = 1; }

/*  regenerate()
 *
 *  基本数值的更新函式。在人物的 heart_beat 中每当一个 tick 的时间到
 *  了，就会呼叫这个函式来更新人物的基本数值。
 *
 *  当人物的所有基本数值被设定的更新函式都传回 0 的时候，regenerate
 *  会自动把自己关掉，直到 start_regenerate 被呼叫为止。
 */

int regenerate()
{
    string st;
    mixed regenerator;
    int n_updated = 0;

    if( !regenerating ) return 0;

    if( !mapp(st_regenerator) || !mapp(st_maximum) ) return 0;

    foreach(st, regenerator in st_regenerator) {
		if( evaluate( regenerator, this_object(), st, query_stat_maximum(st), mapp(st_current) ? st_current[st] : 0) )
			n_updated++;
		if( mapp(st_current) && st_current[st] < 0 && !f_exhausted[st])
	    	f_exhausted[st] = this_object();
    }

    return regenerating = (n_updated > 0);
}

// 随著时间恢复
//int health_regenerator(object me, string stat, int max, int eff, int cur)
int health_regenerator(object me, string stat, int max, int cur)
{
    if( me->is_fighting() ) return 0;   // 战斗中不会恢复

    // 恢复目前值
    if( cur < max ) {
		switch(stat) {
			case "ap": return me->heal_stat(stat, (int)me->query_attr("dex")/5 );
			case "hp": return me->heal_stat(stat, (int)me->query_attr("con")/5 );
			case "mp": return me->heal_stat(stat, (int)me->query_attr("int")/5 );
			default:   return me->heal_stat(stat, (int)me->query_attr("int")/5 );
		}
    }

    if( cur > max ) {
        switch(stat) {
			case "ap": return me->set_stat_current(stat, max);
			case "hp": return me->set_stat_current(stat, max);
			case "mp": return me->set_stat_current(stat, max);
			default:   return me->set_stat_current(stat, max);
		}
    }
    
    return 0;
}

// 随著时间减少
int wasting_regenerator(object me, string stat, int max, int cur)
{
    if( !userp(me) ) return 0;
    return me->damage_stat(stat, 1 + (me->is_fighting() ? 2 : 0), me);
}
