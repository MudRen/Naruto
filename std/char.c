/*  char.c - standard character body

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#pragma save_binary

#include <login.h>
#include <action.h>
#include <ansi.h>
#include <char.h>
#include <command.h>
#include <condition.h>
#include <dbase.h>
#include <move.h>
#include <name.h>
#include <skill.h>
#include <user.h>

inherit F_ACTION;
inherit F_ATTACK;
inherit F_ATTRIBUTE;
inherit F_COMBAT;
inherit F_COMMAND;
inherit F_CONDITION;
inherit F_DBASE;
inherit F_FINANCE;
inherit F_MESSAGE;
inherit F_MASTER;
inherit F_MOVE;
inherit F_NAME;
inherit F_SCORE;
inherit F_SKILL;
inherit F_STATISTIC;
inherit F_TEAM;

// variables

static int tick;

// implementations

int is_character() { return 1; }

void die();

/* setup()
 *
 * �����ʽ�������һ������ĳ�ʼ������������������������Խ��ܵ��¼�������
 * ����(heart beat)��ָ��(command)�ȡ�
 * setup ������� CHAR_D ����� game-specific �������ʼ����
 */

void setup()
{
    seteuid(getuid());

    // ֻ�и��Ƶ������Ҫ�� heart beat �����µ��趨��������ʡ CPU time
    if( !clonep(this_object()) ) return;

    enable_player();
    init_command();
    CHAR_D->setup_char( this_object() );

    set_heart_beat(1);
    tick = 5 + random(10);
}

/* heart_beat()
 *
 * �����������ʽ�������ʽÿ�����ӻᱻ driver ����һ�Σ���������Խ���
 * ս���ȶ�����ʹ�����Լ� NPC ���еĹ��ֱܷ����� NPC �� USER �������
 * ��
 *
 * ���������ʽ��ִ�еĴ��������� mudlib ����ߵģ�����������൱�����
 * ���������׵���Ч�ܵĴ�����ͣ������� LPC ��û�к��죬�벻Ҫ�������
 * �����ʽ�����ݡ�
 */

static void heart_beat()
{
    mapping f;
    int f_altered = 0;

    if( mapp(f=query_destroyed()) && sizeof(f) ) {
		f_altered++;
		RACE_D(query_race())->statistic_destroyed(this_object(), f);
        if( !this_object() ) return;
    }

    if( mapp(f=query_exhausted()) && sizeof(f) ) {
		f_altered++;
		RACE_D(query_race())->statistic_exhausted(this_object(), f);
        if( !this_object() ) return;
    }

    if( mapp(f=query_notified()) && sizeof(f) ) {
		f_altered++;
		this_object()->notify_stat(f);
        if( !this_object() ) return;
    }

    if( f_altered ) clear_statistic_flags();

    /* ս���غϵĿ��� */
    if( living(this_object()) )
    {
		if( is_fighting() ) clean_up_enemy();
		if( is_busy() ) continue_action();
		else if( is_fighting() ) attack();
    }

    /* ������ tick ʱ�䵽��ʱ��Ż�ִ�еĲ��֡� */
    if( tick-- ) return;

    /* ��һ�� tick ��ʱ����ȡ����������������� heart beat ����������
     * ��ϵͳ�ĸ������ڼ������ض��� tick��
     */
    tick = 20 + random(10);

    update_condition();
    regenerate();
}

void lose_fight(object opponent)
{
    if( opponent ) opponent->win_fight(this_object());
}

int notify_stat(mapping flag)
{
    // ������趨Ԥ�����ܷ������ȳ���������������ܣ��������ѡһ�������ܡ�
    string dir;
    if( !living(this_object()) || !environment() ) return 0;
    if( stringp(dir = query("wimpy_dir")) ) return command("go "+ dir);
    else command("go escape");
}

varargs void revive(int quiet)
{
    object ob;
    string arg, arg2;

    remove_call_out("revive");

    // ���������ﱻ��������Я�������ƶ���Я���ߵ�����
    while( environment()->is_character() )
        this_object()->move(environment(environment()));

    enable_player();
    init_command();		// Add by -Acme- �赹����ʱ��û�취ʹ��ָ��

    delete_temp("block_msg/all");

    if( !quiet ) {
        COMBAT_D->announce(this_object(), "revive");
        message("system", HIY "\n������������������֪��....\n\n" NOR, this_object());
    }
}

/*  ���������
 */
void die()
{
    object corpse, ob;

    // ֹͣһ�е�ս��������������ѡ��Ĺ�ϵ
    remove_all_killer();
    guard_ob(0);
    charge_ob(0);
    dismiss_team();

    // ֹͣ�κ����ڽ��еĶ���
    if( is_busy() ) interrupt(this_object(), INTR_DEATH);

    // ������е� condition
    clear_condition();

    // ����������������
    COMBAT_D->announce(this_object(), "dead");

    // ������������ڻ����У������������ء��ѹ����������ſ��õ��Լ�������ѶϢ
    if( !living(this_object()) ) revive(1);

    tell_object(this_object(), HIR "\n�����ˣ���\n\n" NOR);
    CHAR_D->make_corpse(this_object());

    if( userp(this_object()) ) {
    	int lv;
        mapping skills;
        string err, sk, *sname;
        object room;
        
        // �����ͷ������м��ܽ���һ��
        skills = this_object()->query_skills();
        if( !mapp(skills) ) skills = ([]);
        sname = keys(skills);
        
        foreach( sk in sname ) {
        	lv = this_object()->query_skill(sk, 1);
        	if( lv > 1 ) this_object()->set_skill(sk, lv-1);
        }

        // �������������е�����ɾ��
        this_object()->delete("vendetta");
        
	    // �����Ƶ�������
        err = catch(room = load_object(this_object()->query("recoverroom")));
        if( !room ) err = catch(room = load_object(START_ROOM));
        if( !room ) err = catch(room = load_object(VOID_OB));
        if( room && this_object()->move(room) )
            message_vision(HIW"һ��׹��ȥ��$N����Ӵ����һ����ͻȻ������е������ˣ����˸�����ʺ��\n"NOR, this_object());
    }

    return;
}

varargs void unconcious(string reason)
{
    object ob, me=this_object();

    // �Ѿ����� ?
    if( !living(me) ) return;

    // ��ʦ���ܻ���
    if ( wizardp(this_object()) ) {
        message("system", HIW "\n�����ʦ��������������ȫ�����˺� ....\n\n" NOR, me);
        me->set_stat_current("ap", me->query_stat_maximum("ap"));
        me->set_stat_current("hp", me->query_stat_maximum("hp"));
        me->set_stat_current("mp", me->query_stat_maximum("mp"));
        return;
    }

    // ֹͣ���� fight �еĵ���
    remove_all_enemy();

    // �ж����ڽ��еĶ���
    if( is_busy() ) interrupt(me, INTR_UNCONCIOUS);

    message("system", HIR "\n�����ǰһ�ڣ�����ʲôҲ��֪����....\n\n" NOR, this_object());

    disable_player(stringp(reason) ? reason : " <���Բ���>");
    set_temp("block_msg/all", 1);

    COMBAT_D->announce(this_object(), "unconcious");

    // �趨һ���Ӻ�����
    remove_call_out("revive");
    call_out("revive", 60);
}

// overrides
int receive_object(object ob, int from_inventory)
{
    if( from_inventory ) return 1;

    if( query_encumbrance() + ob->weight() > query_ability("carriage") )
        return notify_fail(ob->name() + "���ڶ�����˵̫���ˡ�\n");

    if( sizeof(all_inventory(this_object())) >= 30 )
        return notify_fail("�������Ѿ�û�п�λ��Я�����������ˡ�\n");

    return 1;
}

varargs int move(mixed dest, int silent)
{
    if( interactive(this_object()) && is_busy() )
	return notify_fail("�������޷��ƶ��u\n");

    if( ::move(dest, silent) ) {
		if( interactive(this_object()) && !silent )
	    	environment()->do_look(this_object());
		return 1;
    }

    return 0;
}

string query_id()
{
    mixed* applied;

    if( arrayp(applied = query_temp("apply/id")) && sizeof(applied) && arrayp(applied[<1]) && stringp(applied[<1][0]) )
        return applied[<1][0];

    return query("id");
}

int id(string str)
{
    mixed* applied;

    if( arrayp(applied = query_temp("apply/id")) && sizeof(applied) && arrayp(applied[<1]) )
		return member_array(str, applied[<1]) >= 0;
    return ::id(str);
}

varargs string name(int raw)
{
    string *applied_name;

    if( !raw && arrayp(applied_name = query_temp("apply/name")) && sizeof(applied_name) && stringp(applied_name[<1]) )
		return applied_name[<1];
    return ::name(raw);
}

/*  short()
 *
 *  ���������������ļ��������ʹ������ look һ�������У��������������
 *  ��ʾ�ľ�������ִ���
 */
varargs string short(int raw)
{
    string str, title, nick, *applied;

    str = ((title = query("title")) ? title : "")
        + ((nick = query("nickname")) ? ("��"+nick+"��") : (title?" ":""))
        + ::short(1);

    if( raw ) return str;

    // ���ӵĶ�����
    if(	arrayp(applied = query_temp("apply/short")) && sizeof(applied) && stringp(applied[<1]) ) {
		str = applied[<1];
		if( !this_player()->query("option/BRIEF_ID") )
	    	str += "(" + capitalize(query_id()) + ")";
    }

    if( !living(this_object()) ) str += HIR + query("disable_type") + NOR;

    if( query_temp("pending/hidden") )
		str = GRN + " <Ǳ��> " + str + NOR;

    if( this_object()->is_fighting() ) str += HIR" <ս��>"NOR;

    if( !userp(this_object()) ) return str;

    if( !interactive(this_object()) ) str += HIG " <����>" NOR;
    else if( query_idle( this_object() ) > 120 ) str += HIM " <����>" NOR;

    if( in_input() ) str += HIC " <����>" NOR;
    if( in_edit() ) str += HIY " <�༭>" NOR;

    return str;
}

varargs string long(int raw)
{
    string str, *applied;

    if( !raw && arrayp(applied = query_temp("apply/long"))  && sizeof(applied) && stringp(applied[<1]) )
		str = applied[<1];
    else str = ::long(raw);

    if( !raw && arrayp(applied = query_temp("apply/description")) && sizeof(applied) && stringp(applied[<1]) ) {
		str += implode(applied, "\n");
		str = replace_string(str, "$N", this_object()->name(raw));
    }

    return str;
}

varargs string rank(string politeness, int raw)
{
    string c, r;

    if( c = this_object()->query_class() )
        r = CLASS_D(c)->query_rank(this_object(), politeness);
    else
        r = ::rank(politeness, raw);

    if( !politeness && !raw ) {
        switch(wizhood(this_object())) {
        	case "(admin)":      return "����";
        	case "(arch)":       return "����";
        	case "(wizard)":     return "����";
        	case "(apprentice)": return "����";
        	case "(immortal)":   return "Ұ��";
        }
    }
    return r;
}

nomask int visible(object ob)
{
    int a, b;

    // �κ�������Զ�����Կ����Լ�
    if( ob==this_object() ) return 1;

    // ��ʦ��Զ���Կ�����ʦ�ȼ����Լ��͵����
    a = wiz_level(ob);
    b = wiz_level(this_object());
    if( wizardp(ob) && a>b ) return 1;
    if( query("invis") && a<=b ) return 0;

    if( !::visible(ob) ) return 0;

    // ���ص������Ҫ�㹻���������ܷ���
    if( query_temp("pending/hidden") > ob->query_ability("awareness") )
		return 0;

    return 1;
}