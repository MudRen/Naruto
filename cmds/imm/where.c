#include <ansi.h>

inherit F_CLEAN_UP;

int main(object me, string str)
{
	object ob, where, *ob_list;
	string msg="", dir_str;
	int i;

	if (!str) {
		ob_list = sort_array( users(), (:
			strcmp( environment($1) ? file_name(environment($1)) : "",
					environment($2) ? file_name(environment($2)) : "" )
		:) );
        msg  = "�q�����������������������������������������������������������������������������r\n";
        msg += "��"BBLU"        ����(ID)        "NOR" "BGRN"                       ��   ��                     "NOR"��\n";
		foreach(ob in ob_list) {
			if( !environment(ob) || !ob->visible(this_player()) ) continue;
			msg += sprintf("��%12s%-12s %-51s��\n",
				ob->name(),
				"("+ ob->query_id() +")",
				file_name(environment(ob)) + 
				( environment(ob)->is_area() ? " (" + ob->query("area_info/x_axis") + "," + ob->query("area_info/y_axis") + ")" : "")
			);
		}
        msg += "�t�����������������������������������������������������������������������������s";
		me->start_more(msg);
		return 1;
	}

	ob = find_player(str);
	if( !ob ) ob = find_living(str);
	if( !ob ) ob = LOGIN_D->find_body(str);
	if( ob && ob->visible(me) ) {
		where = environment(ob);
		if (!where) return notify_fail("����˲�֪��������Ү...\n");
		printf("%s(%s) ������ %s (%s), %s\n",
			ob->name(),
			ob->query("id"),
			where->is_area() ? where->query("name") + "("+ob->query("area_info/x_axis")+","+ob->query("area_info/y_axis")+")": where->query("short"),
			file_name(where),
			ob->link() ? ( living(ob) ? "���� " + query_idle(ob)/60 + " ����" : "�赹��") : (userp(ob) ? "������":"NPC ��"), 
		);
		return 1;
	}

	// locate the children of object
	dir_str = resolve_path(me->query("cwd"), str);
	if( sizeof(ob_list = children(dir_str)) ) {
		for(i=0; i<sizeof(ob_list); i++) {
			if( !ob = environment(ob_list[i]) ) continue;
			msg += sprintf("%O\n", ob );
			// show until ob is room
			while( ob = environment(ob) )
				msg += sprintf("%O\n", ob );
			msg += "\n";
		}
		msg += "Object " + dir_str + ".c located.\n";
		this_player()->start_more(msg);
		return 1;
	}
	if (!ob) return notify_fail("����û " + str + " ������� (include ppl and npc), Ҳû�� " + dir_str + ".c �Ĵ���.\n");
}

int help(object me)
{
	write(@HELP
ָ���ʽ: where <����>

������Ϊ:
��          �г����� users ����λ�á�
grain       ��ʹ���� "grain" . ����, ���г�ȫ��Ϊ "grain" ֮ npcλ��, �һ�
            ��� "����", "�赹", "����", "NPC" ��״̬��ʾ��
kao yeu     �г�ȫ�� ( set_name �е�һ�� string ), Ϊǰ��֮ npcλ�á�
/obj/bottle �г����е���Ϊǰ��֮���λ��, �һ�һ������, ֱ����������Ϊ
            room Ϊֹ��
bottle      ͬ����, ��ֻ��Ѱ��ǰĿ¼ (current directory)��
bottle.c    ͬ���С�
HELP);
	return 1;
}
