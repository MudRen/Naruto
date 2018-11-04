/* area.c write by -Acme-
   ���︺��������������롢���������ȹ�����
*/

#pragma save_binary

#include <area.h>
#include <dbase.h>

inherit F_DBASE;
inherit F_CLEAN_UP;
inherit F_SAVE;

inherit AREA_MAP;

// ��������������
// Location Of Loaded Objects
// ������������֮������������һ�����ϣ��Է�����ٲ���
string *LOLO = ({});

////////////////////////////////////////////////////////////
// һ���趨����顢��ѯ
////////////////////////////////////////////////////////////
// ������������������꼯
string *query_LOLO() { return LOLO; }
// ����һ������Ԫ����LOLO����
void add_LOLO(string location) { if( member_array(location, LOLO) == -1 ) LOLO += ({ location }); }
// ɾ��һ������Ԫ����LOLO����
void del_LOLO(string location) { if( member_array(location, LOLO) != -1 ) LOLO -= ({ location }); }

// �����������ϵ�·��λ��
string query_save_file() { return file_name(this_object()); }

// ���Խ�����������������
int receive_object(object ob, int from_inventory) { return 1; }

// ������
int is_area() { return 1; }

// �趨ĳ����Ҫ����������
int set_loads(int x, int y, string filename, int amount)
{
	if( !check_scope(x, y) ) return 0;
	if( amount <= 0 ) return 0;
	if( undefinedp(area[y][x]["loads"]) )
		area[y][x]["loads"] = ([]);
	area[y][x]["loads"][filename] = amount;
	
	// ���������Ѱ��
	add_LOLO((string)y+","+(string)x);
	return 1;
}

// ��ĳ����Ҫ�����������Ƴ�
int del_loads(int x, int y)
{
	if( !check_scope(x, y) ) return 0;
	if( undefinedp(area[y][x]["loads"]) ) return 1;
	map_delete(area[y][x], "loads");
	
	// �Ƴ�������Ѱ��
	del_LOLO((string)y+","+(string)x);
	return 1;
}

// ��ĳ���������������Ƴ�
int del_loaded(int x, int y)
{
	if( !check_scope(x, y) ) return 0;
	if( undefinedp(area[y][x]["loaded"]) ) return 1;
	map_delete(area[y][x], "loaded");
	return 1;
}

////////////////////////////////////////////////////////////
// �������������롢���
////////////////////////////////////////////////////////////
object make_inventory(string file, int x, int y)
{
	string *exits;
    object ob;

    file = file_path(file);
    ob = new(file);

    // Support for uniqueness
    if( ob->violate_unique() ) ob = ob->create_replica();
    if( !ob ) return 0;

    ob->set("area_info/x_axis", x);
    ob->set("area_info/y_axis", y);
    ob->set("area_info/x_axis_old", x);
    ob->set("area_info/y_axis_old", y);

    // ������Ƶ������������Ƿ���guardĳ����
	if( move_in(x, y, ob) ) {
		if( (exits=ob->query("guard_exit")) ) {
			int i = sizeof(exits);
			while(i--) this_object()->set_area_guard(x, y, exits[i], ob);
		}
	    ob->move(this_object());
	}
	else destruct(ob);

    return ob;
}

void reset_callout(int temp)
{
    int i, j, x, y, amount, t=0;
    mapping ob;
	string file;

	if( !sizeof(LOLO) || temp >= sizeof(LOLO) ) return;
	
	for(i=temp;i<sizeof(LOLO);i++) {
        if( sscanf(LOLO[i], "%d,%d", y, x) != 2 ) continue;
        if( !check_scope(x, y) ) continue;
        if( undefinedp(area[y][x]["loads"]) ) continue;
        if( !mapp(ob = area[y][x]["loaded"]) ) ob = ([]);
        foreach( file, amount in area[y][x]["loads"] ) {
            for(j=amount;j>0;j--) {
                if( objectp(ob[file + " " + j]) ) continue;
                ob[file + " " + j] = make_inventory(file, x, y);
            }
            area[y][x]["loaded"] = ob;
        }
        t++;
        if( t >= 5 ) {
            call_out("reset_callout", 1+random(3), i+1);
            return;
        }
    }
	return;
}

void reset()
{
    int i, amount, x, y;
    mapping ob;
    string file, location;

    // ���LOO���꼯
    LOO = ({});

    // ������뼯������, �ݻ��ӳٺ��в���mob����cost����
    call_out("reset_callout", 1, 0);
    return;
}

int clean_up(int inherit_flag)
{
    int i, y, x;
    mapping items;
    string file;
    object ob;

    i = sizeof(LOLO);
    while(i--)
		if( sscanf(LOLO[i], "%d,%d", y, x) == 2 ) {
        	if( !mapp(items = query_loaded(x, y)) ) continue;
        	foreach(file, ob in items)
	    		if( objectp(ob) && ob->is_character() && environment(ob) != this_object() )
					return 1;
    	}

    return ::clean_up(inherit_flag);
}

// ��������ʱ���д˺�ʽ
void remove()
{
    int i, y, x, cnt;
    mapping items;
    string file, *objects;
    object ob;

	i = sizeof(LOLO);
	while(i--)
		if( sscanf(LOLO[i], "%d,%d", y, x) == 2 ) {
			if( !mapp(items = query_loaded(x, y)) ) continue;
			foreach(file, ob in items) {
				if( objectp(ob) ) {
					message("vision", ob->name() + "������ƮƮ�İ�����ɢ�ˡ�\n", environment(ob));
					destruct(ob);
					cnt++;
			}
		}
		// �������������������������
		map_delete(area[y][x], "objects");
		
        // ����������������֮���ϣ������������ 
        map_delete(area[y][x], "loaded");
    }

    if( cnt && this_player() ) write("���棺" + cnt + " ��NPC�ɴ������첢��ǿ��ɾ����\n");
}

int save()
{
	int i, j, x_size, y_size;
    string *LOO_BAK;
    mapping ICON_BAK;
    mixed *AREA_BAK;
    
	// �浵ǰ������Ҫ��ȡ�ı�����գ������������޷�Ԥ�ڵĴ���
	LOO_BAK = LOO;
	ICON_BAK = icon;
	AREA_BAK = area;
	
	LOO = ({});
	icon = ([]);
	
	x_size = this_object()->query("x_axis_size");
	y_size = this_object()->query("y_axis_size");
	
    // ÿ�������е���ʱ����ҲҪ���
	for(i=0;i<y_size;i++) {
		for(j=0;j<x_size;j++) {
            map_delete(area[i][j], "loaded");
            map_delete(area[i][j], "objects");
        }
    }
	
	if( ::save() ) {
		LOO = LOO_BAK;
		icon = ICON_BAK;
		area = AREA_BAK;
		return 1;
	}
	
	return 0;
}

void setup()
{
	int i;
    string file;

	seteuid(getuid());

	// û�д��浵
	if( !restore() ) {
    	int j, y_size, x_size;

        // ���������ϱ��� area ����ʼ��
        y_size = this_object()->query("y_axis_size");
        x_size = this_object()->query("x_axis_size");
        if( !y_size || y_size<AREA_YAXIS_MIN || y_size>AREA_YAXIS_MAX ) y_size = DEFAULT_YAXIS_SIZE;
        if( !x_size || x_size<AREA_XAXIS_MIN || x_size>AREA_XAXIS_MAX ) x_size = DEFAULT_XAXIS_SIZE;

        // Ҫ�������
        area = allocate( y_size );
        for(i=0;i<sizeof(area);i++) {
            area[i] = allocate( x_size );
            for(j=0;j<sizeof(area[i]);j++) area[i][j]=([]);
        }
        
        // �����������
        LOO = ({});
        LOLO = ({});
        
        // ����
        save();
	}

    // update ���·��
    file = base_name(this_object());
    i = strlen(file);
    while(i--) if( file[i..i] == "/" ) break;
    set("file_path", file[0..i] );
    
	this_object()->reset();

}
