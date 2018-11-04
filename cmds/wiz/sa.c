/* sa.c
*/

#include <command.h>
#include <ansi.h>

inherit F_CLEAN_UP;

void create()  { seteuid(getuid()); }

void show_info(object me, mapping coord)
{
    int i;
	mixed type;
	mapping info, temp;
	string msg;
	
	info = environment(me)->query_info(coord["x_axis"], coord["y_axis"]);

	if( !sizeof(info) ) return;
	
	msg = sprintf(HIW"========================= %-8s (%2d,%2d) =========================\n"NOR,
		          environment(me)->query("name"), coord["x_axis"], coord["y_axis"] );
    msg += BYEL"[����]"NOR + " " + file_name(environment(me)) + ".c\n";
	
	foreach(type in keys(info))
		switch( type ) {
			case "objects":		// �����������ϵ����
				msg += BRED"[ϵͳ]"NOR" Ŀǰ�����������������(objects)��\n";
				if( (i=sizeof(info["objects"])) ) {
					while(i--)
						msg += "       " + file_name(info["objects"][i]) + "\n";
				}
				break;
			
			case "loaded":		// �Ѿ���������
				msg += BRED"[ϵͳ]"NOR" �Ѿ���������(loaded)��\n";
				if( (i=sizeof(info["loaded"])) ) {
					mixed *k, *v;
					k = keys(info["loaded"]);
					v = values(info["loaded"]);
					while(i--)
						msg += "       " + k[i] + "��" + (objectp(v[i]) ? file_name(v[i]) : "������") + "\n";
				}
				break;
			
			case "loads":		// ����Ҫ����������
				msg += BBLU"[�趨]"NOR" ��������������(loads)��\n";
			    if( (i=sizeof(info["loads"])) ) {
			    	mixed *k, *v;
			    	k = keys(info["loads"]);
			    	v = values(info["loads"]);
			    	while(i--)
			    		msg += "       ������" + k[i] + ", ������" + v[i] + "\n";
			    }
				break;
			
			case "room_exit":	// ����ķ�����ʽ����
				msg += BBLU"[�趨]"NOR" ������ʽ����(room_exit) ������" + info["room_exit"] + "\n";
				break;
			
			case "area_exit":	// �����������ʽ����
				msg += BBLU"[�趨]"NOR" ������ʽ����(area_exit) ������" + info["area_exit"]["filename"];
			    msg += "  ���꣺(" + info["area_exit"]["x_axis"] + "," + info["area_exit"]["y_axis"] + ")\n";
				break;
				
			case "detail":
				msg += BBLU"[�趨]"NOR" ϸ������(detail)��\n";
			    if( (i=sizeof(info["detail"])) ) {
			    	mixed *k, *v;
			    	k = keys(info["detail"]);
			    	v = values(info["detail"]);
			    	while(i--)
			    		msg += "       " + k[i] + "��" + v[i] + "\n";
			    }
				break;
			case "guard":
				break;
			default:			// �����趨
				msg += BGRN"[�趨]"NOR" ";
			    msg += type + "��" + info[type] + "\n";
				break;
		}
	msg += HIW"====================================================================\n"NOR;
	write(msg);
}

int main(object me, string arg)
{
    mixed value;
    string type;
    mapping info;
    
    SECURED_WIZARD_COMMAND;

	if( !environment(me) ) return 0;
	
	if( !environment(me)->is_area() ) {
		write("��ָ��ֻ����������ʹ�á�\n");
		return 1;
	}

    if( !(info = me->query("area_info") ) ) {
    	write("ʹ��ָ����ִ���\n");
    	return 1;
    }

	if( !arg ) {
		show_info(me, info);
		return 1;
	}

    if( arg == "save" ) {
    	if( environment(me)->save() ) {
    		write("���򴢴�ɹ�\��\n");
    		return 1;
    	}
    	write("���򴢴�ʧ�ܡ�\n");
    	return 1;
    }

    if( arg == "save_ansi") {
    	environment(me)->map_ansi_save();
    	return 1;
    }
    
    if( arg == "save_html") {
    	environment(me)->map_html_save();
    	return 1;
    }
    
    if( arg == "loads") {
  		int x, y, i;
   		string *lolo;
   		lolo = environment(me)->query_LOLO();
   		if( (i=sizeof(lolo)) > 0 ) {
   			write("==================== Ŀǰ���������������������� ====================\n");
   			while(i--) {
   				if( sscanf(lolo[i], "%d,%d", y, x) == 2 ) {
   					write(sprintf("(%2d,%2d) ", x, y));
   					if( i >= 8 && i%8 == 0 ) write("\n");
   				} 
   			}
   			write("\n======================================================================\n");
   		} else write("�������Ŀǰ����û��Ҫ����������\n");
   		return 1;
    }

	if( sscanf(arg, "%s %d", type, value) != 2 )
		if( sscanf(arg, "%s %s", type, value) != 2) {
			write("ָ���������ȷ��\n");
			return 1;
		}
	
    if( value == "delete") {
    	if( type == "loads" ) {
    		if( !environment(me)->del_loads(info["x_axis"], info["y_axis"], value) ) {
    			write("ɾ�������������ʧ�ܡ�\n");
    			return 1;
    		}
    		write("ɾ��������������ɹ�\��\n");
    		return 1;
    	}
    	if( !environment(me)->delete_data(info["x_axis"], info["y_axis"], type) ) {
    		write("ɾ��ʧ�ܡ�\n");
    		return 1;
    	}
		write("�㽫���� (" + info["x_axis"] + "," + info["y_axis"] + ") �� " + type + " ����ɾ����\n");
    	return 1;
    }

    // �趨������ʽ����
    if( type == "area_exit" ) {
		int x, y;
		string filename;
		if( sscanf(value, "%s %d %d", filename, x, y) != 3 ) {
			write("������ʽ�����趨���� (ָ���ʽ��sa area_exit FILE_NAME X_AXIS Y_AXIS)\n");
			return 1;
		}
		if( !environment(me)->set_area_exit(info["x_axis"], info["y_axis"], filename, x, y) ) {
			write("������ʽ�����趨ʧ�ܡ�\n");
			return 1;
		} 
		write("������ʽ�����趨�ɹ�\��\n");
    	return 1;
    }

    // �趨���������
    if( type == "loads" ) {
    	int amount;
    	string filename;
    	if( sscanf(value, "%s %d", filename, amount) != 2 ) {
    		write("������������趨���� (ָ���ʽ��sa loads FILE_NAME AMOUNT)\n");
    		return 1;
    	}
    	
    	if( !environment(me)->set_loads(info["x_axis"], info["y_axis"], filename, amount) ) {
    		write("������������趨ʧ�ܡ�\n");
    		return 1;
    	}
    	write("������������趨�ɹ�\��\n");
    	return 1;
    }

    if( type == "detail" ) {
    	string title, detail;
    	if( sscanf(value, "%s %s", title, detail) == 2 ) {
    		if( environment(me)->set_area_detail(info["x_axis"], info["y_axis"], title, detail) )
    			write("�趨ϸ�������ɹ�\��\n");
    	} else write("�趨ϸ������ʧ�ܡ�\n");
    	return 1;
    }

    // ������趨icon��ת��ɫ��
	if( type == "icon" ) {
		value = color(value);
		if( sizeof(uncolor(value)) != 2 ) {
			write("�趨�����ϵ�icon�����Ƕ�����Ԫ��С��\n");
			return 1;
		}
	}

    // ������Ŀ�趨
   	if( !environment(me)->set_data(info["x_axis"], info["y_axis"], type, value) ) {
		write("�趨ʧ�ܡ�\n");
		return 1;
	}
	
	write("�㽫���� (" + info["x_axis"] + "," + info["y_axis"] + ") �� " + type + " �����趨Ϊ " + value + " ��\n");
	
	return 1;
}

int help(object me)
{
    string msg = "����ָ���������趨��������������ϵ�ָ��(Ŀǰ����������)��\n"
BBLU"\n1. ָ���ʽ�� sa\n"NOR
"   �����г�Ŀǰ��������������Ѷ��\n"
BBLU"\n2. ָ���ʽ�� sa loads\n"NOR
"   ��ѯ�����л�������������������б�\n"
BBLU"\n3. ָ���ʽ�� sa loads FILE_NAME AMOUNT\n"NOR
"   ���ָ������趨Ŀǰ�������꣬���Զ�����ĳ����ڴ������С�\n"
"   ���� sa loads __DIR__npc 2      �������������Ŀ¼·���еĵ���npc��ֻ�������ϡ�\n"
"        sa loads __DIR__npc/mob 4  �������������·����npcĿ¼�µ�mob������ֻ�������ϡ�\n"
BBLU"\n4. ָ���ʽ�� sa area_exit FILE_NAME X_AXIS Y_AXIS\n"NOR
"   ���ָ����趨������ʽ�ĳ��ڡ�\n"
"   ���� sa area_exit __DIR__area 50 20  �������Ƶ�����ʱ�����Զ�ת�Ƶ�area��������(50,20)λ�á�\n"
BBLU"\n5. ָ���ʽ�� sa room_exit FILE_NAME\n"NOR
"   ���ָ������趨������ʽ�ĳ��ڡ�\n"
"   ���� sa room_exit __DIR__room  �������Ƶ�����ʱ�����Զ�ת�Ƶ�����room�С�\n"
BBLU"\n6. ָ���ʽ�� sa detail TITLE DETAIL\n"NOR
"   ���ָ���������趨�����ϸ�������ģ�֧Ԯsearch, lookָ�Ӧ����quest��\n"
"   ���� sa detail ���� ����һ�ô���������\�������(climb)��ȥ��\n"
BBLU"\n7. ָ���ʽ�� sa TYPE VALUE\n"NOR
"   ���ָ���������趨������\�ܵ��趨��\n"
"   ���� sa icon $YEL$ʺ$NOR$  �趨ͼʾ, ��ָ��֧Ԯɫ�롣\n"
"        sa no_fight 1         ��ս��\n"
"        sa nonprompt_icon 1   ���ἴʱ�ı�ͼʾ���趨(��˼���������Ƶ�������ʱͼʾ����ı�)\n"
"        sa short ��·         �趨����Ķ�����\n"
"        sa long xxxxxxxx      �趨����ĳ����������̫��������toָ��, �磺 to sa long\n"
BBLU"\n8. ָ���ʽ�� sa TYPE delete\n"NOR
"   ��������ɾ���������ϵ�ָ�\n"
"   ���� sa loads delete      ɾ����������������\n"
"        sa area_exit delete  ɾ�������������ʽ����\n"
"        sa room_exit delete  ɾ������ķ�����ʽ����\n"
"        sa icon delete       ɾ�������ͼʾ\n"
BBLU"\n9. ָ���ʽ�� sa save\n"NOR
"   ���Ǵ������򵵰���ָ�����������һЩ����󣬱���ʹ�ô�ָ�������档\n"
BBLU"\n10. ָ���ʽ�� sa save_ansi\n"NOR
"   ���Ǵ�������ȫͼ��ָ��������mapָ�\n"
BBLU"\n11. ָ���ʽ�� sa save_html\n"NOR
"   ���Ǵ�������ȫͼ��ָ�������ҳ�ϡ�\n"
"\nע�����������ʹ�õĵ���·����\n"
"      __DIR__     ���·���������Ŀ¼���� __DIR__ ��ָ����\n"
"      _DIR_AREA_  ָ�� /world/area/ Ŀ¼��\n"
"      _DIR_NPC_   ָ�� /world/npc/ Ŀ¼��\n"
"      _DIR_WP_    ָ�� /world/wp/ Ŀ¼��\n"
"      _DIR_EQ_    ָ�� /world/eq/ Ŀ¼��\n"
"      _DIR_ITEM_  ָ�� /world/item/ Ŀ¼��\n"
"      _DIR_WORLD_ ָ�� /world/ Ŀ¼��\n";
    me->start_more(msg);
    return 1;
}
