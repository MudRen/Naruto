/*  user.c - the user body object

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
#include <origin.h>
#include <user.h>
#include <action.h>
#include <login.h>

inherit CHARACTER;
inherit F_AUTOLOAD;
inherit F_EDIT;
inherit F_SAVE;
inherit F_MORE;
inherit F_FLAG;

// private prototypes

private void user_dump(int type);

// variables

static object my_link;
static int last_age_set;

// implementations

void create()
{
    object ob;

    // Let LOGIN_D export proper uid to us.
    seteuid(0);

    if( clonep(this_object()) )
	set_name("������", "noname");
}

private int move_or_destruct( object dest )
{
    if( dest ) move(dest);
    else {
		message("system", "һ��ʱ�յ�Ť�����㴫�͵���һ���ط�....\n", this_object());
		set_temp("last_location", base_name(environment()));
		move(VOID_OB);
    }
}

private void reset()
{
    // To prevent the master copy of user object error out by security
    // violation.
    if( !userp(this_object()) ) return;

    gain_score("survive", (int)query("age") / 2 + 1 );
}

int set_link(object link)
{
    if( origin()==ORIGIN_CALL_OTHER && (geteuid(previous_object()) != ROOT_UID) && (geteuid(previous_object()) != geteuid(this_object())) )
		return 0;
    my_link = link;
    return 1;
}

object link() { return my_link; }

void terminal_type(string term_type)
{
    set_temp("terminal_type", term_type);
    message("system", "�ն˻���̬�趨Ϊ " + term_type + "��\n", this_object());
}

// This is used by F_SAVE to determine the filename to save our data.
string query_save_file()
{
    string id;

    id = geteuid();
    if( !id ) id = getuid();

    return user_data(id);
}

int save()
{
    int res;

    if( objectp(my_link) ) my_link->save();
    save_autoload();
    res = ::save();
    clean_up_autoload();        // To save memory
    return res;
}

// This function updates player's age, called by heart_beat()
private void heart_beat()
{
    ::heart_beat();
    if( !this_object() ) return;

    // Make us older. Note this should not rely on fixed period between
    // each call to heart_beat bcz heart_beat could be stoped for various
    // reason.                                           - Annihilator

    if( !last_age_set ) last_age_set = time();
    add("time_aged", time() - last_age_set);
    last_age_set = time();
    if( (int)query("time_aged") >= 86400 ) {
		add("age", 1);
		delete("time_aged");
    }

    if( objectp(my_link) ) my_link->update_age();

    if( interactive(this_object()) && query_idle(this_object()) >= IDLE_TIMEOUT )
		user_dump(DUMP_IDLE);
}

void setup()
{
    if( ! query("id") ) set("id", getuid());
    ::setup();

    /* �������� F_STATISTIC ������ʹ�����Ƿ����ϴ� quit ֮ǰ�Ѿ���������
     * ��û�б� heart_beat ץ��( �׳� quit �� )
     */
    damage_stat("ap", 0);
    damage_stat("hp", 0);
    damage_stat("mp", 0);

    /* �趨Clinet�Ӵ�ѶϢ add by -Acme- */
    set_temp("window", link()->query_window() );

    restore_autoload();
    restore_condition();
}

private void user_dump(int type)
{
    switch(type)
    {
	    case DUMP_NET_DEAD:
			// change cmd: quit to quit ! -Dragoon
			command("quit !");
			break;
	    case DUMP_IDLE:
			// wiz don't get kicked out (Elon 10-17-95)
			if (wizardp(this_object())) return;
			tell_object( this_object(), "�Բ���o���Ѿ��������� "  + IDLE_TIMEOUT/60 + " �����˩o���´�������\n");
			if( environment()->is_area() )
				tell_area(environment(),
						  this_object()->query("area_info/x_axis"),
						  this_object()->query("area_info/y_axis"),
						  "һ��紵���o�������е�" + query("name") + "��Ϊһ�ѷɻҩo��ʧ�ˡ�\n", ({this_object()}));
			else
				tell_room( environment(), "һ��紵���o�������е�" + query("name") + "��Ϊһ�ѷɻҩo��ʧ�ˡ�\n", ({this_object()}));
			// change cmd: quit to quit ! -Dragoon
			command("quit !");
			break;
	    default: return;
    }
}

/*  net_dead()
 *
 *  ��ʹ���߶���ʱ��driver ��������������
 */
private void net_dead()
{
    set_heart_beat(0);

    if( objectp(my_link) ) {
	my_link->save();
	destruct(my_link);
    }

    // Stop fighting, we'll be back soon.
    remove_all_enemy();
    if( environment() )
	set_temp("last_location", base_name(environment()) );

    if( is_busy() ) interrupt(this_object(), INTR_LINKDEAD);

    if( userp(this_object()) ) {
		call_out("user_dump", NET_DEAD_TIMEOUT, DUMP_NET_DEAD);
		if( environment()->is_area() )
			tell_area(environment(),
					  this_object()->query("area_info/x_axis"),
					  this_object()->query("area_info/y_axis"),
					  "ʱ��һ�󲨶�, " + query("name") + "˲�������������ʧ...\n");
		else tell_room(environment(), "ʱ��һ�󲨶�, " + query("name") + "˲�������������ʧ...\n");
		LOGIN_D->net_dead(this_object());
    } else 
	command("quit");
}

// reconnect: called by the LOGIN_D when a netdead player reconnects.
void reconnect()
{
    string last_loc;

    if( geteuid(previous_object()) != ROOT_UID )
		error("Permission denied.\n");

    set_heart_beat(1);
    remove_call_out("user_dump");
    tell_object(this_object(), "����������ϡ�\n");
    if( stringp(last_loc = query_temp("last_location")) ) {
        object room;
        room = load_object(last_loc);
        if( !room ) room = load_object(START_ROOM);
        if( room->is_area() )
            area_move(room, this_object(), this_object()->query("area_info/x_axis"),
                                           this_object()->query("area_info/y_axis") );
        else move(room);
    }
}

static string last_cmd;
static int last_cmd_time;
static int last_cmd_count;

string process_input(string str)
{
#ifdef	MAX_COMMAND_PER_SECOND
//	if( !wizardp(this_player()) ) {
    	if( last_cmd_time == time() ) {
			last_cmd_count++;
			if( last_cmd_count > MAX_COMMAND_PER_SECOND ) return "";
			if( last_cmd_count == MAX_COMMAND_PER_SECOND ) {
				write("\r����ָ����ٶ�̫���ˣ��������Ƶ������޷�ִ����ô�����\n");
		    	return "";
			}
    	} else {
			last_cmd_time = time();
			last_cmd_count = 1;
    	}
//	}
#endif	/* MAX_COMMAND_PER_SECOND */

    if( str[0..1]=="!!" )
		return last_cmd + str[2..];

    if( objectp(my_link) )
		return last_cmd = my_link->process_alias(str);

    return 0;
}

void lose_fight(object opponent)
{
    ::lose_fight(opponent);
//    tell_object(this_object(), "��һ�����޷��мܣ�������ⳡ���ԡ�\n");
}

void win_fight(object opponent)
{
    message_vision("$n�мܲ�ס��$NӮ���ⳡ���ԡ�\n", this_object(), opponent);
//    tell_object(this_object(), opponent->name() + "�мܲ�ס����Ӯ���ⳡ���ԡ�\n");
    remove_all_enemy();
}

/***********************************************************************
                            USER DATA PROTECTIONS
 ***********************************************************************/
 
nomask mixed set(string prop, mixed data)
{
    USER_PROTECT();
    return ::set(prop, data);
}

nomask mixed add(string prop, mixed data)
{
    USER_PROTECT();
    return ::add(prop, data);
}

nomask void delete(string prop)
{
    USER_PROTECT();
    ::delete(prop);
}

nomask varargs mixed query(string prop, int raw)
{
    mixed v = ::query(prop, raw);
    if( mapp(v) || arrayp(v) ) USER_PROTECT();
    return v;
}

nomask void set_skill(string skill, int amount)
{
    USER_PROTECT();
    ::set_skill(skill, amount);
}

nomask void advance_skill(string skill, int amount)
{
    USER_PROTECT();
    ::advance_skill(skill, amount);
}

nomask void set_learn(string skill, int lrn)
{
    USER_PROTECT();    
    ::set_learn(skill, lrn);
}

nomask void improve_skill(string skill, int amount)
{
    USER_PROTECT();
    ::improve_skill(skill, amount);
}

nomask int set_attr(string what, int value)
{
    USER_PROTECT();
    return ::set_attr(what, value);
}

nomask int set_stat_maximum(string what, int val)
{
    USER_PROTECT();
    return ::set_stat_maximum(what, val);
}
