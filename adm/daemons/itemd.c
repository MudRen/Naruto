/* itemd.c write by -Acme-
   ��������Mud�����е�item�������ϣ������ѯ
*/

#pragma save_binary
#include <ansi.h>

inherit F_CLEAN_UP;
inherit F_SAVE;

mapping item;
mapping serial;

void create()
{
    seteuid(getuid());
    if( !restore() && !mapp(item) ) {
        serial = ([]);
        item = ([]);
    }
}

string query_save_file() { return DATA_DIR + "itemd"; }

int do_add(string type, object ob)
{
    string base;
    int index, tmp, *kk;

    if( undefinedp(serial[type]) ) serial[type] = 0;
    if( undefinedp(item[type]) ) item[type] = ([]);

    base = base_name(ob);

    for(index=1;index<=serial[type];index++) {
    	if( undefinedp(item[type][index]) ) continue;
        if( item[type][index]["file_name"] == base ) break;
    }

    if( index > serial[type] ) serial[type]++;

    item[type][index] = ([]);
    item[type][index]["file_name"] = base;
    item[type][index]["name"] = ob->name();
    
    if( type == "npc" ) item[type][index]["level"] = ob->query_level();
    else item[type][index]["value"] = ob->query("value");

    return 1;
}

int do_del(string type, int index)
{
    if( undefinedp(item[type]) ) return 1;
    if( undefinedp(item[type][index]) ) return 1;
    map_delete(item[type], index);
    return 1;
}

mapping do_query_all(string type)
{
    if( undefinedp(item[type]) ) return 0;
    return item[type];
}

mapping do_query(string type, int index)
{
    if( undefinedp(item[type]) ) return 0;
    return item[type][index];
}

int add_item(object ob)
{
    if( !objectp(ob) ) return 0;
    
    if( ob->is_character() ) return do_add("npc", ob);
    else if( ob->query("wear_as") ) return do_add("eq", ob);
    else if( ob->query("wield_as") ) return do_add("wp", ob);
    else if( function_exists("stuff_ob", ob) ) return do_add("food", ob);
    else return do_add("obj", ob);

    return 0;
}