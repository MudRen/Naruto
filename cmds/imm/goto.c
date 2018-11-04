/* goto.c */
#include <command.h>

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    int x, y;
    string file, out_msg, in_msg;
    object obj;

    SECURED_COMMAND;

    if( !arg ) return notify_fail("��Ҫȥ����t\n");

    if( sscanf(arg, "goto_out_msg %s", out_msg) == 1 ) {
    	me->set("goto_out_msg", color(out_msg));
    	write("Ok.\n");
    	return 1;
    } else if( sscanf(arg, "goto_in_msg %s", in_msg ) == 1 ) {
    	me->set("goto_in_msg", color(in_msg));
    	write("Ok.\n");
    	return 1;
    }

    if( me->query("goto_out_msg") ) out_msg = me->query("goto_out_msg") + "\n";
    else out_msg = "ֻ��һ���������$N����Ӱ�Ѿ������ˡ�\n";

    if( me->query("goto_in_msg") ) in_msg = me->query("goto_in_msg") + "\n";
    else in_msg = "ֻ��һ���������$N����Ӱ�Ѿ����������\n";

    // goto ������ by -Acme-
    if( sscanf(arg, "%s %d %d", file, x, y) == 3 ) {
        file = resolve_path(me->query("cwd"), file);
        if( !sscanf(file, "%*s.c") ) file += ".c";
        if( file_size(file) >=0 && (obj=load_object(file)) ) {
			if( !obj->is_area() ) return 1;
	    	message_vision(out_msg, me);
			area_move(obj, me, x, y);
		    message_vision(in_msg, me);
		    return 1;
        } else {
        	write("������򵵲����ڡ�\n");
        	return 1;
        }
        
    // ����������������goto
    } else if( sscanf(arg, "%d %d", x, y) == 2 ) {
    	if( !environment(me)->is_area() ) return 1;
    	message_vision(out_msg, me);
		area_move(environment(me), me, x, y);
	    message_vision(in_msg, me);
   		return 1;
    }

    obj = find_player(arg);
    if( !obj ) obj = find_living(arg);
    if( obj == me ) {
    	write("��Ҫ�Ƶ��������ԣ�\n");
    	return 1;
    }

    // �Ƶ�һ�㷿��
    if( !obj || !obj->visible(me)) {
        arg = resolve_path(me->query("cwd"), arg);
        if( !sscanf(arg, "%*s.c") ) arg += ".c";
        if( file_size(arg) >=0 && (obj=load_object(arg)) ) {
            if( obj->is_area() ) {
            	write("�ƶ���������������������������ƶ���\n");
            	return 1;
            }
   	        message_vision(out_msg, me);
            me->move(obj);
   	        message_vision(in_msg, me);
    		return 1;
        } else return notify_fail("û�������ҡ������ط���\n");
    }

    if( !environment(obj) ) return notify_fail("������û�л������� goto��\n");

    if( environment(obj)->is_area() ) {
    	if( area_environment(obj, me) ) return notify_fail("���Ѿ����������ˡ�\n");

        message_vision(out_msg, me);
		if( !area_move(environment(obj), me, obj->query("area_info/x_axis"), obj->query("area_info/y_axis")) ) {
            tell_object(me, "��Ϊĳ�����ɣ���� goto ʧ���ˡ�\n");
		}
    } else {
        message_vision(out_msg, me);
        if( !me->move(environment(obj)) ) {
            tell_object(me, "��Ϊĳ�����ɣ���� goto ʧ���ˡ�\n");
        }
    }

    message_vision(in_msg, me);

    return 1;
}

int help(object me)
{
    write(@HELP
ָ���ʽ : goto <Ŀ��>
 
���ָ��Ὣ�㴫�͵�ָ����Ŀ�ꡣĿ�������һ��living �򷿼�
�ĵ��������Ŀ����living����ᱻ�Ƶ����Ǹ���ͬ���Ļ�����

����������趨goto���뿪ѶϢ������goto�Ľ���ѶϢ��
�磺 goto goto_out_msg $HIW$���������$N�۹�����һ�����뿪�����
     goto goto_in_msg  $HIW$���������$N�۹�����һ�������������
ʹ��ɫ��ϵͳ�������Զ�����$NOR$���Կ��Բ���Ҫ�ټ�һ�Ρ�
HELP);
    return 1;
}
