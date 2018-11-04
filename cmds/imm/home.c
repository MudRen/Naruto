// home.c

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
	string id, msg;
	object home;

    id = me->query_id();
    
    if( arg ) {
    	if( !objectp( home = load_object("/u/" + arg[0..0] + "/" + arg + "/workroom.c") ) ) {
    		write(arg + "�Ĺ���Ŀ¼��û��workroom�Ĵ��ڣ����޷��ƶ���\n");
    		return 1;
    	}
		message_vision("$N�����˻ӻ��֣���" + arg + "�Ҵ�����ȥ��\n", me);
		if( me->move(home) )
			message("tell_room", "һ�����̹�ȥ��" + me->name() + "���������\n", environment(me), me);
		return 1;
    }

	if( !objectp( home = load_object("/u/" + id[0..0] + "/" + id + "/workroom.c") ) ) {
		message_vision("$N�����˻ӻ��֣��ؼ�ȥ�ˡ�\n", me);
		me->move("/world/area/wizard/guildhall");
		message("tell_room", me->name() + "�ص����Լ��ļ��С�\n", environment(me), me);
		return 1;
	}

	if( stringp(msg = me->query("env/msg_home")) )
		message_vision(msg + "\n", me);
	else
		message_vision("$N�����˻ӻ��֣��ؼ�ȥ�ˡ�\n", me);
	me->move(home);
	message("tell_room", me->name() + "�ص����Լ��ļ��С�\n", environment(me), me);
	return 1;
}

int help(object me)
{
  write(@HELP
ָ���ʽ : home [id]

���ô�һָ���ֱ�ӻص��Լ��Ĺ����һ�����ʦ�Ĺ����ң��� home directory ��
�� workroom.c ������ص� wiz guild������� workroom.c ���� 'msg_home' ���
�趨, ���ڳ����˶��ῴ���Ǹ�ѶϢ��
HELP
    );
    return 1;
}
