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
    		write(arg + "的工作目录下没有workroom的存在，你无法移动。\n");
    		return 1;
    	}
		message_vision("$N向众人挥挥手，到" + arg + "家串门子去。\n", me);
		if( me->move(home) )
			message("tell_room", "一阵轻烟过去后，" + me->name() + "来到了这里。\n", environment(me), me);
		return 1;
    }

	if( !objectp( home = load_object("/u/" + id[0..0] + "/" + id + "/workroom.c") ) ) {
		message_vision("$N向众人挥挥手，回家去了。\n", me);
		me->move("/world/area/wizard/guildhall");
		message("tell_room", me->name() + "回到了自己的家中。\n", environment(me), me);
		return 1;
	}

	if( stringp(msg = me->query("env/msg_home")) )
		message_vision(msg + "\n", me);
	else
		message_vision("$N向众人挥挥手，回家去了。\n", me);
	me->move(home);
	message("tell_room", me->name() + "回到了自己的家中。\n", environment(me), me);
	return 1;
}

int help(object me)
{
  write(@HELP
指令格式 : home [id]

利用此一指令可直接回到自己的工作室或别的巫师的工作室，若 home directory 中
无 workroom.c 档，则回到 wiz guild。如果你 workroom.c 中有 'msg_home' 这个
设定, 则在场的人都会看到那个讯息。
HELP
    );
    return 1;
}
