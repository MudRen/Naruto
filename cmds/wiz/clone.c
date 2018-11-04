/*  clone.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <command.h>
#include "/doc/help.h"

inherit F_CLEAN_UP;

int main(object me, string file)
{
    int amount;
    object obj;
    string err, msg;

    SECURED_WIZARD_COMMAND;

    /* ʹ���������ߵ�Ȩ�޸������ */
    seteuid(geteuid(this_player(1)));

    if (!file) file = me->query("cwf");
    if (!file) return notify_fail("��Ҫ����ʲô����t\n");

    if(sscanf(file, "%d %s", amount, file)==2 && amount < 1 )
	return notify_fail("������Ʒ���������ٱ����� 1��\n");

    file = resolve_path(me->query("cwd"), file);
    if( sscanf(file, "%*s.c") != 1 ) file += ".c";

    if( file_size(file) < 0 )
	return notify_fail("û���������(" + file + ")��\n");

    if( !amount ) me->set("cwf", file);
    else me->set("cwf", amount + " " + file);

    if( !find_object(file) ) {
	err = catch(load_object(file));
	if (err) {
	    printf("����ʧ�ܣ�%O\n", err);
	    return 1;
	}
    }

    err = catch(obj = new(file));
    if( err || !obj ) {
	printf("����ʧ�ܣ�%O\n", err);
	return 1;
    }

    if( amount ) {
	if( function_exists("set_amount", obj) )
	    obj->set_amount(amount);
	else if( function_exists("set_volume", obj) )
	    obj->set_volume(amount);
	else
	    write("( " + obj->name() + "���Ǹ�������� )\n");
    }

    if( !stringp(msg = me->query("env/msg_clone")) )
	msg = "ֻ��$N�������һָ�o�����$n��\n";

    // re-set by dragoon
    log_file("static/CLONE", 
	sprintf("[%s] %s(%s) ���� %s at %s\n",
	ctime(time()), me->name(1), geteuid(me), file, 
	file_name(environment(me)) ) );

    notify_fail("�޷����Ʋ����ƶ������(" + file + ")��\n");

    if( !obj->query("no_get") && !obj->is_character() && !living(obj) && obj->move(me) ) {
		write(obj->query("name") + "���Ƴɹ�\�o���������Ʒ����\n");
		message_vision(msg + "\n", me, obj);
		return 1;
    }
    
    // add by -Acme-
    if( environment(me)->is_area() ) {
    	if( area_move(environment(me), obj, me->query("area_info/x_axis"), me->query("area_info/y_axis")) ) {
    	    write(obj->query("name") + "���Ƴɹ�\�o����������䡣\n");
		    message_vision(msg + "\n", me, obj);
		    return 1;
    	}
    }
    
    if( obj->move(environment(me)) ) {
    	write(obj->query("name") + "���Ƴɹ�\�o����������䡣\n");
		message_vision(msg + "\n", me, obj);
		return 1;
    }

    destruct(obj);
    return 1;
}

int help(object me)
{
    write(@HELP
ָ���ʽ : clone <����>

���ô�ָ��ɸ����κ����ƶ�֮���(������)��
HELP
    );
    return 1;
}
