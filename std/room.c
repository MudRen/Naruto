/* room.c */

#pragma save_binary

#include <ansi.h>
#include <dbase.h>
#include <room.h>

inherit F_DBASE;
inherit F_CLEAN_UP;

int receive_object(object ob, int from_inventory) { return 1; }

int clean_up(int inherit_flag)
{
    mapping items;
    string file;
    object ob;

    if( mapp(items = query_temp("objects")) )
	    foreach(file, ob in items) {
	        if( objectp(ob) && ob->is_character() && environment(ob)!=this_object() ) return 1;
	}

    return ::clean_up(inherit_flag);
}

void remove()
{
    mapping items;
    string file;
    object ob;
    int cnt = 0;

    if( !mapp(items = query_temp("objects")) ) return;

    foreach(file, ob in items) {
        if( objectp(ob) && ob->is_character() && environment(ob)!=this_object() ) {
	        message("vision", "һ��ǿ�ҵ������Ȼ���֣���û��" + ob->name() + "��\n", environment(ob));
	        destruct(ob);
	        cnt++;
	    }
    }

    if( cnt && this_player() )
	    write("���棺" + cnt + "���ɴ˷����������������ɾ����\n");
}

object make_inventory(string file)
{
    object ob;

    ob = new(file);

    // Support for uniqueness
    if( ob->violate_unique() ) ob = ob->create_replica();
    if( !ob ) return 0;

    ob->move(this_object());

    return ob;
}

void reset()
{
    mapping ob_list, ob;
    string file;
    int amount;
    object obj, *inv;

    ob_list = query("objects");
    if( mapp(ob_list) ) {
	    if( !mapp(ob = query_temp("objects")) ) ob = ([]);

	    foreach(file, amount in ob_list) {
        // ��鷿����Ҫ�������������Ƿ��㹻
	        if( amount==1 ) {
		        if( !objectp(ob[file]) ) ob[file] = make_inventory(file);
	        } else {
		        int i;
		        for(i=amount-1; i>=0; i--) {
		            if( objectp(ob[file + " " + i]) ) continue;
		            ob[file + " " + i] = make_inventory(file);
		        }
	        }
	    }
	    set_temp("objects", ob);
    }

    inv = all_inventory(this_object());
    foreach(obj in inv) if( interactive(obj) || !clonep(obj) ) return;
    if( mapp(query("resource")) ) set_temp("resource", copy(query("resource")) );
}

int valid_leave(object me, string dir)
{
	return 1;
}

varargs int do_look(object me, string arg)
{
    int i;
    object *inv, ob;
    mapping exits;
    string str, *dirs;

    // Look specific object in the room.
    if( arg ) {
		if( str = query("detail/" + arg) ) {
		    me->start_more(str);
	    	return 1;
		}
		if( strsrch(query("long"), arg) >= 0 )
	    	return notify_fail("�㿴���������" + arg + "��ʲô�ر�ġ�\n");
		return notify_fail("��Ҫ��ʲô�t\n");
    }

    if( previous_object() && previous_object()->query("option/brief_room") )
		str = query("short") + "��";
    else
		str = sprintf( "%s - %s\n    %s%s    ",
	    	query("short"),
		    wizardp(me)? file_name(this_object()) : "",
		    query("long"),
	    	query("outdoors") ? NATURE_D->outdoor_room_description() : "" );

    if( mapp(exits = query("exits")) )
		dirs = keys(exits);

    if( sizeof(dirs)==0 )
		str += "����û���κ����Եĳ�·��\n";
    else if( sizeof(dirs)==1 )
		str += "����Ψһ�ĳ����� " BOLD + dirs[0] + NOR "��\n";
    else
		str += sprintf("�������Եĳ����� " BOLD "%s" NOR " �� " BOLD "%s" NOR "��\n", implode(dirs[0..<2], "��"), dirs[<1]);

    inv = all_inventory(this_object()) - ({ me });
    foreach(ob in inv) {
		if( !ob->visible(me) ) continue;
		str = sprintf("%s  %s\n", str, ob->short() );
    }

    message("vision", str, me);
    return 1;
}

void init()
{
    int explore_id;
    if( !undefinedp(explore_id = query("site_explore")) ) {
	if( !this_player()->recognize(explore_id, 1) ) {
	    this_player()->gain_score("survive", 100);
	    this_player()->gain_score("explorer fame", 1);
	}
    }
}

void setup()
{
    seteuid(getuid());
    this_object()->reset();
}