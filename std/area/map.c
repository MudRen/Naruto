/* map.c write by -Acme-
   ���︺���ͼ����ʾ��������ƶ��Լ�һЩ�趨�ȵȡ�
*/

#pragma save_binary

#include <area.h>
#include <ansi.h>
#include <dbase.h>

inherit AREA_PROMPT_ICON;

// ������������(���������ͼ��...)
mixed *area;

// Location Of Objects
// ���д���������������һ�����ϣ��Է�����ٲ���
string *LOO = ({});

// ������һ���趨��������Ŀ (������ʽ�ĳ��ڣ�Ҫ���ر�ķ�ʽ�趨)
string *set_data_resist = ({ "objects", "loaded", "loads", "area_exit" });

// ������һ��ɾ����������Ŀ (��������������õ���Ŀ�������ر����Ŀ)
string *delete_data_resist = ({ "objects", "loaded", "loads" });

varargs int do_look(object me, string arg);

////////////////////////////////////////////////////////////
// һ���趨����顢��ѯ
////////////////////////////////////////////////////////////
// ������������ڵ����꼯
string *query_LOO() { return LOO; }
// ����һ������Ԫ����LOO����
void add_LOO(string location) { if( member_array(location, LOO) == -1 ) LOO += ({ location }); }
// ɾ��һ������Ԫ����LOO����
void del_LOO(string location) { if( member_array(location, LOO) != -1 ) LOO -= ({ location }); }

// ʹ��__DIR__֮���·��
string file_path(string dir) { 
    dir = replace_string(dir, "__DIR__", query("file_path") );
    dir = replace_string(dir, "_DIR_AREA_", _DIR_AREA_);
    dir = replace_string(dir, "_DIR_NPC_", _DIR_NPC_);
    dir = replace_string(dir, "_DIR_WP_", _DIR_WP_);
    dir = replace_string(dir, "_DIR_EQ_", _DIR_EQ_);
    dir = replace_string(dir, "_DIR_ITEM_", _DIR_ITEM_);
    dir = replace_string(dir, "_DIR_WORLD_", _DIR_WORLD_);
    return dir;
}

// ���귶Χ���
int check_scope(int x, int y)
{
    if( y < 0 || x < 0 || y >= sizeof(area) || x >= sizeof(area[y]) ) return 0;
    return 1;
}

// ��ѯĳ�������Ѷ
mapping *query_info(int x, int y)
{
    if( !check_scope(x, y) ) return 0;
    return area[y][x];
}

// ��ѯĳ������������
mapping query_loaded(int x, int y)
{
    if( !check_scope(x, y) ) return ([]);
    return area[y][x]["loaded"];
}

// ��ѯ������ĳ����������
mapping *query_inventory(int x, int y)
{
    if( !check_scope(x, y) ) return ({});
    if( undefinedp(area[y][x]["objects"]) ) return ({});
    return area[y][x]["objects"];
}

// �趨�����ͳ���
int set_area_exit(int x, int y, string filename, int exit_x, int exit_y)
{
    if( !check_scope(x, y) ) return 0;
    area[y][x]["area_exit"] = ([]);
    area[y][x]["area_exit"]["filename"] = filename;
    area[y][x]["area_exit"]["x_axis"] = exit_x;
    area[y][x]["area_exit"]["y_axis"] = exit_y;
    return 1;
}

// �趨����ϸ������
int set_area_detail(int x, int y, string title, string detail)
{
    if( !check_scope(x, y) ) return 0;
    if( undefinedp(area[y][x]["detail"]) ) area[y][x]["detail"] = ([]);
    area[y][x]["detail"][title] = detail;
    return 1;
}

// ɾ������ϸ������
int del_area_detail(int x, int y, string title)
{
    if( !check_scope(x, y) ) return 0;
    if( undefinedp(area[y][x]["detail"]) ) return 1;

    map_delete(area[y][x]["detail"], title);

    if( sizeof(area[y][x]["detail"]) < 1 )
        map_delete(area[y][x], "detail");
    return 1;
}

// ȡ������ϸ������
mapping query_area_detail(int x, int y)
{
    if( !check_scope(x, y) ) return 0;
    if( undefinedp(area[y][x]["detail"]) ) return ([]);
    return area[y][x]["detail"];
}

// ȡ��ĳdetail������
string query_area_detail_arug(int x, int y, string title)
{
    if( !check_scope(x, y) ) return 0;
    if( undefinedp(area[y][x]["detail"]) ) return 0;
    if( undefinedp(area[y][x]["detail"][title]) ) return 0;
    return area[y][x]["detail"][title];
}

// �趨ĳ�����guard
int set_area_guard(int x, int y, string exit, object ob)
{
    if( !check_scope(x, y) ) return 0;
    if( !objectp(ob) ) return 0;
    if( undefinedp(area[y][x]["guard"]) ) area[y][x]["guard"] = ([]);
    if( undefinedp(area[y][x]["guard"][exit]) ) area[y][x]["guard"][exit] = ({});
    area[y][x]["guard"][exit] += ({ ob });
    return 1;
}

// ��ѯĳ�����Ƿ�guard
int query_area_guard(int x, int y, string exit)
{
	int index;
	object guard;
    if( !check_scope(x, y) ) return 0;
    if( undefinedp(area[y][x]["guard"]) ) return 0;
    if( undefinedp(area[y][x]["guard"][exit]) ) return 0;
    if( !(index=sizeof(area[y][x]["guard"][exit])) ) return 0;
    while(index--) {
    	if( objectp(guard=area[y][x]["guard"][exit][index]) ) {
    		if( guard->query("area_info/x_axis") == x && guard->query("area_info/y_axis") == y ) {
    			guard->do_guard_exit(x, y, exit);
    		    return 1;
    		}
        }
    }
    return 0;
}

// �趨һ������
int set_data(int x, int y, string type, mixed value)
{
    if( member_array(type, set_data_resist) != -1 ) return 0;
    if( !check_scope(x, y) ) return 0;
    area[y][x][type] = value;
    return 1;
}

// ɾ��һ������
int delete_data(int x, int y, string type)
{
    if( member_array(type, delete_data_resist) != -1 ) return 0;
    if( !check_scope(x, y) ) return 0;
    if( undefinedp(area[y][x][type]) ) return 1;
    map_delete(area[y][x], type);
    return 1;
}

// ȡ��һ������
mixed query_data(int x, int y, string type)
{
    if( member_array(type, delete_data_resist) != -1 ) return 0;
    if( !check_scope(x, y) ) return 0;
    if( undefinedp(area[y][x][type]) ) return 0;
    return area[y][x][type];
}

////////////////////////////////////////////////////////////
// �ƶ�
////////////////////////////////////////////////////////////

// ���ĳ�����Ƿ����ϰ���Ƿ�ɴ�͸
int is_move(int x, int y)
{
    // �����ʦ�趨Ϊ��ǽģʽ
    if( wizardp(this_player()) && this_player()->query("option/map_through") ) return 1;
    if( undefinedp(area[y][x]["block"]) ) return 1;
    if( !area[y][x]["block"] ) return 1;
    return 0;
}

// �������ĳ���괦��
int move_in(int x, int y, object ob)
{
    // ���������С��Χ
    if( !check_scope(x, y) ) return 0;

    // ������û�������(objects)�����������Ϊ��
    if( undefinedp(area[y][x]["objects"]) ) area[y][x]["objects"] = ({});

    // һ���������30�����
    if( sizeof(area[y][x]["objects"]) >= 30 ) {
        write("�Ǹ�����̫�ණ���ˡ�\n");
        return 0;
    }

    // ����Ѿ����������(objects)�У��Ͳ���Ҫ�ټ��������
    if( member_array( ob, area[y][x]["objects"] ) != -1 ) return 1;

    // ��������������
    area[y][x]["objects"] += ({ ob });
    
    // �ı伴ʱ��ͼʾ
    set_icon_weight(x, y, get_icon_weight(ob));

    // ����LOO��Ѱ��
    add_LOO((string)y + "," + (string)x);

    return 1;
}

// ����Ƴ�ĳ���괦��
int move_out(int x, int y, object ob)
{
    // ���������С��Χ
    if( !check_scope(x, y) ) return 0;

    // ������û�������(objects)
    if( undefinedp(area[y][x]["objects"]) ) return 1;

    // �ı伴ʱ��ͼʾ
    set_icon_weight(x, y, -get_icon_weight(ob));

    // ����������������(objects)�У��Ͳ���Ҫ���Ƴ������
    if( member_array( ob, area[y][x]["objects"] ) == -1 ) return 1;

    // ����Ƴ������
    area[y][x]["objects"] -= ({ ob });
     
    // ������ȫû���ʱ��ɾ�������
    if( sizeof(area[y][x]["objects"]) < 1 ) {
        map_delete(area[y][x], "objects");
        // ɾ��LOO��Ѱ��
        del_LOO((string)y + "," + (string)x);
    }

    return 1;
}

// �����ƶ�
// ����Ƿ�Ϸ������������ƶ�
int valid_leave(object ob, string dir)
{
    int x, y, x_past, y_past;
    x_past = ob->query("area_info/x_axis");
    y_past = ob->query("area_info/y_axis");
    
    // ����ת��
    switch( dir ) {
        case     "north":  y = y_past-1; x = x_past;   break;
        case      "east":  y = y_past;   x = x_past+1; break;
        case     "south":  y = y_past+1; x = x_past;   break;
        case      "west":  y = y_past;   x = x_past-1; break;
        case "northeast":  y = y_past-1; x = x_past+1; break;
        case "southeast":  y = y_past+1; x = x_past+1; break;
        case "southwest":  y = y_past+1; x = x_past-1; break;
        case "northwest":  y = y_past-1; x = x_past-1; break;
        default: return 0; break;
    }
    
    // �޷��ƶ���������(���ϰ���)
    if( !check_scope(x, y) || !is_move(x, y) ) {
        write("�������û�г�·");
        return 0;
    }

    // �޷���ĳ�����ƶ���Ϊ��ĳ��guard
    if( query_area_guard(x_past, y_past, dir) ) return 0;
	
    //////////////////////////////
    // ������ķ���
    //////////////////////////////
    if( !undefinedp(area[y][x]["room_exit"]) ) {
        object room;
        if( !objectp(room = load_object(file_path(area[y][x]["room_exit"]))) ) {
            write("�������ĳ��������⣬��֪ͨ������������\n");
            return 0;
        }
        if( room->is_area() ) {
            write("�������ĳ��������⣬��֪ͨ������������\n");
            return 0;
        } else {
        	// ����ɹ��Ƶ�����, move ���Զ�����ǰ��area����move_out����
            if( ob->move(room) ) return 1;
            else {
                write("�������ĳ��������⣬��֪ͨ������������\n");
                return 0;
            }
        }
        return 0;
    }

    //////////////////////////////
    // �����������
    //////////////////////////////
    if( !undefinedp(area[y][x]["area_exit"]) ) {
        object room;
        if( !objectp(room = load_object(file_path(area[y][x]["area_exit"]["filename"]))) ) {
            write("�������ĳ��������⣬��֪ͨ������������\n");
            return 0;
        }
        if( !room->is_area() ) {
            write("�������ĳ��������⣬��֪ͨ������������\n");
            return 0;
        }

        if( !area_move(room, ob, area[y][x]["area_exit"]["x_axis"], area[y][x]["area_exit"]["y_axis"]) ) {
        	write("�������ĳ��������⣬��֪ͨ������������\n");
        	return 0;
        }
        return 1;
    }

    //////////////////////////////
    // ���������ƶ� 
    //////////////////////////////
    // ����Ƴ�������
    if( move_out(x_past, y_past, ob) ) {
        // �������������
        if( move_in(x, y, ob) ) {
            ob->set("area_info/x_axis", x);
            ob->set("area_info/y_axis", y);
            ob->set("area_info/x_axis_old", x);
            ob->set("area_info/y_axis_old", y);
        } else { 
            // �������ʧ�ܣ��˻�ԭ����
            move_in(x_past, y_past, ob);
            return 0;
        }
        if( userp(ob) ) do_look(ob, 0);
    } else return 0;

    return 1;
}



////////////////////////////////////////////////////////////
// �������򻷾�
////////////////////////////////////////////////////////////

// ��ѯĳ����ĳ���
string *query_exits(int x, int y, int option)
{
    string *exits=({});

    if( !option ) {
        // ����������·
        if( y-1 >= 0 ) {
            if( x-1 >= 0 && is_move(x-1, y-1) ) exits += ({ "northwest" });
            if( x >= 0 && is_move(x, y-1) ) exits += ({ "north" });
            if( x+1 < sizeof(area[0]) && is_move(x+1, y-1) ) exits += ({ "northeast" });
        }

        // ���Ͽ�����·
        if( y+1 < sizeof(area) ) {
            if( x-1 >= 0 && is_move(x-1, y+1) ) exits += ({ "southwest" });
            if( x >= 0 && is_move(x, y+1) ) exits += ({ "south" });
            if( x+1 < sizeof(area[0]) && is_move(x+1, y+1) ) exits += ({ "southeast" });
        }

        // ����������·
        if( x+1 < sizeof(area[0]) && is_move(x+1, y) ) exits += ({ "east" });
    
        // ����������·
        if( x-1 >= 0 && is_move(x-1, y) ) exits += ({ "west" });
    } else {
        // ����������·
        if( y-1 >= 0 ) {
            if( x-1 >= 0 && is_move(x-1, y-1) ) exits += ({ "����("HIK"nw"NOR")" });
            if( x >= 0 && is_move(x, y-1) ) exits += ({ "��("HIK"n"NOR")" });
            if( x+1 < sizeof(area[0]) && is_move(x+1, y-1) ) exits += ({ "����("HIK"ne"NOR")" });
        }

        // ���Ͽ�����·
        if( y+1 < sizeof(area) ) {
            if( x-1 >= 0 && is_move(x-1, y+1) ) exits += ({ "����("HIK"sw"NOR")" });
            if( x >= 0 && is_move(x, y+1) ) exits += ({ "��("HIK"s"NOR")" });
            if( x+1 < sizeof(area[0]) && is_move(x+1, y+1) ) exits += ({ "����("HIK"se"NOR")" });
        }

        // ����������·
        if( x+1 < sizeof(area[0]) && is_move(x+1, y) ) exits += ({ "��("HIK"e"NOR")" });
    
        // ����������·
        if( x-1 >= 0 && is_move(x-1, y) ) exits += ({ "��("HIK"w"NOR")" });
    }
    return exits;
}

// ��ͼ��ʾ
varargs string show_area(int x, int y, int type)
{
    int i, j, x_start, y_start, x_size, y_size;
    string msg;

    // ������ʾ��ͼ��Y�����, X�����
    y_size = sizeof(area);
    x_size = sizeof(area[0]);

    if( y <= 5 || y_size <= 11 ) y_start = 0;
    else if( y >= y_size-6 ) y_start = y_size-11;
    else y_start = y-5;

    if( x <= 9 || x_size <= 19 ) x_start = 0;
    else if( x >= x_size-9 ) x_start = x_size-19;
    else x_start = x-9;

    // ������ʱ��ͼ
    msg = sprintf(BBLU"�v"LINE" %-21s %s (%2d,%2d) "NOR+BBLU"�u\n"NOR,
            query("name") + (area[y][x]["short"] ? " - " + area[y][x]["short"] : ""),
            (area[y][x]["no_fight"] ? "��ս��" : "ս����"), x, y, );
    
    for(i=y_start;i<y_size && i<y_start+11;i++) {
        msg += BBLU"��"NOR;
        for(j=x_start;j<x_size && j<x_start+19;j++) {
            if( y == i && x == j ) msg += HIY"��"NOR;
            else if( undefinedp(area[i][j]["icon"]) && 
                    (!undefinedp(area[i][j]["room_exit"]) || !undefinedp(area[i][j]["area_exit"])) )
                        msg += HIW"��"NOR;
            else {
                int check=1;
                if( check && (type & 2) == 2 ) {
                    // �������費��ʱ���ͼʾ
                    if( !area[i][j]["nonprompt_icon"] && check_icon(j, i) ) {
                        msg += get_icon(j, i);
                        check = 0;
                    }
                }
                if( check && (type & 4) == 4 ) {
                    if( area[i][j]["block"] ) {
                        msg += "��";
                    } else msg += "  ";
                    check = 0;     // ͼ�����趨�����
                }
                // ͼ��δ��
                if( check ) msg += area[i][j]["icon"] ? area[i][j]["icon"] : "  ";
            }
        }
        msg += BBLU"��"NOR;
        msg += "\n";
    }
    msg += BBLU"�u���������������������������������������v\n"NOR;
    
    if( (type & 1) == 1 ) return msg;
    else return SAVEC+HOME+msg+RESTC;
}

varargs string show_objects(int x, int y, int type)
{
    string str = "";
    object ob;
    
    if( undefinedp(area[y][x]["objects"]) ) return "";

    foreach(ob in area[y][x]["objects"]) {
        if( ob == this_player() ) continue;
    	if( !objectp(ob) || environment(ob) != this_object() ||
            ob->query("area_info/y_axis") != y || ob->query("area_info/x_axis") != x ) {
                area[y][x]["objects"] -= ({ ob });
                continue;
        }
        str += sprintf("  %s\n", ob->short() );
    }
    
/*  debug ��
    whos = all_inventory(this_object());
    str += sprintf("\n====== Environment_Objects (%d) ======\n", sizeof(whos)) ;
    for(i=0;i<sizeof(whos);i++) {
        str += sprintf("  %s(%s) -> (%d,%d)\n",
            whos[i]->query("name"),
            whos[i]->query("id"),
            whos[i]->query("area_info/x_axis"),
            whos[i]->query("area_info/y_axis") );
    }
*/
    return str;
}

varargs int do_look(object me, string arg)
{
    int i=0, op=0;
    string str="", *exits;
    mapping info, option;

    if( !userp(me) ) return 0;

    info = me->query("area_info");
    option = me->query("option");

    if( !info ) return 0;
    if( !option ) option = ([]);
    if( !check_scope(info["x_axis"], info["y_axis"]) ) return 0;

    if( arg ) {
    	if( (str = query_area_detail_arug(info["x_axis"], info["y_axis"], arg)) )
            message("vision", str + "\n", me);
        else return notify_fail("��Ҫ��ʲô��\n");
        return 1;
    }

    // �Ƿ�ȡ�����ἴʱ��ͼ
    if( option["map_unfreeze"] ) op = op | 1;

    // ��ʾ��ʱͼ�α任
    if( !option["map_obj_icon_hidden"] ) op = op | 2;

    // ��ԭ���ĵ�ͼͼ�α任�ɶ��� ���ϰ��� �� û�ϰ���
    if( option["map_block"] ) op = op | 4;

    // ��ʾ��ͼ
    if( !option["map_hidden"] ) str += show_area(info["x_axis"], info["y_axis"], op);
    
    // ������ʾ
    exits = query_exits(info["x_axis"], info["y_axis"], 1);
    if( (i = sizeof(exits)) ) {
    	str += "����ĳ����� ";
    	while( i-- ) str += exits[i] + ( i ? "��" : "��\n" );
    } else str += "����û���κγ��ڡ�\n";
    
    // ��ʾ���
    if( !option["map_obj_hidden"] ) str += show_objects(info["x_axis"], info["y_axis"], 0);
	
    message("vision", str, me);

    return 1;
}

varargs int map_ansi_save()
{
    int i, j, x, y;
    string file, msg="", msg2="";

    y = sizeof(area);
    x = sizeof(area[0]);

    file = base_name(this_object());

    for(i=0;i<y;i++) {
        for(j=0;j<x;j++) {
        	if( j <= 49 ) {
	            if( undefinedp(area[i][j]["icon"]) ) msg += "  ";
    	        else msg += area[i][j]["icon"];
    	    } else {
	            if( undefinedp(area[i][j]["icon"]) ) msg2 += "  ";
    	        else msg2 += area[i][j]["icon"];
    	    }
        }
        msg += "\n";
        if( x > 49 ) msg2 += "\n";
    }
    
    if( x <= 49 ) {
	    if( !write_file(file + ".ansi" , msg, 1) ) write("����ANSI��ʧ�ܡ�\n");
    	else write("����ANSI���ɹ�\(" + file + ".ansi)��\n");
    } else {
    	write("��Ϊ��area��ȳ�����ʮ���ִ�С���ʷֳ����Ҷ���ͼ�����档\n");
	    if( !write_file(file + "_left.ansi" , msg, 1) ) write("��������ANSI��ʧ�ܡ�\n");
    	else write("��������ANSI���ɹ�\(" + file + "_left.ansi)��\n");
	    if( !write_file(file + "_right.ansi" , msg2, 1) ) write("�����Ұ��ANSI��ʧ�ܡ�\n");
    	else write("�����Ұ��ANSI���ɹ�\(" + file + "_right.ansi)��\n");
    }
    
    return 1;
}

varargs int map_html_save()
{
    int i, j, x, y;
    string file, msg="", tmp;

    y = sizeof(area);
    x = sizeof(area[0]);

    file = base_name(this_object());

    msg += "<html>\n";
    msg += "<head>\n";
    msg += "<title>" + this_object()->query("name") + "</title>\n";
    msg += "</head>\n";
    msg += "<body bgcolor=000000><font size=2 face=ϸ����>\n";
    for(i=0;i<y;i++) {
        for(j=0;j<x;j++) {
            if( undefinedp(area[i][j]["icon"]) ) msg += "��";
   	        else {
   	        	tmp = replace_string(area[i][j]["icon"], "  ", "��");
   	            msg += color_to_html(tmp);
   	            msg += "</span>";
   	        }
        }
        msg += "\n<br>";
    }
    msg += "</font></body>\n</html>";

    if( !write_file(file + ".html" , msg, 1) )
        write("����HTML��ʧ�ܡ�\n");

    write("����HTML���ɹ�\��\n");

    return 1;
}