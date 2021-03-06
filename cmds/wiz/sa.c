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
    msg += BYEL"[档案]"NOR + " " + file_name(environment(me)) + ".c\n";
	
	foreach(type in keys(info))
		switch( type ) {
			case "objects":		// 存在于座标上的物件
				msg += BRED"[系统]"NOR" 目前存在于这个座标的物件(objects)：\n";
				if( (i=sizeof(info["objects"])) ) {
					while(i--)
						msg += "       " + file_name(info["objects"][i]) + "\n";
				}
				break;
			
			case "loaded":		// 已经载入的物件
				msg += BRED"[系统]"NOR" 已经载入的物件(loaded)：\n";
				if( (i=sizeof(info["loaded"])) ) {
					mixed *k, *v;
					k = keys(info["loaded"]);
					v = values(info["loaded"]);
					while(i--)
						msg += "       " + k[i] + "：" + (objectp(v[i]) ? file_name(v[i]) : "不存在") + "\n";
				}
				break;
			
			case "loads":		// 座标要载入的物件集
				msg += BBLU"[设定]"NOR" 座标载入的物件集(loads)：\n";
			    if( (i=sizeof(info["loads"])) ) {
			    	mixed *k, *v;
			    	k = keys(info["loads"]);
			    	v = values(info["loads"]);
			    	while(i--)
			    		msg += "       档案：" + k[i] + ", 数量：" + v[i] + "\n";
			    }
				break;
			
			case "room_exit":	// 座标的房间型式出口
				msg += BBLU"[设定]"NOR" 房间型式出口(room_exit) 档案：" + info["room_exit"] + "\n";
				break;
			
			case "area_exit":	// 座标的区域型式出口
				msg += BBLU"[设定]"NOR" 区域型式出口(area_exit) 档案：" + info["area_exit"]["filename"];
			    msg += "  坐标：(" + info["area_exit"]["x_axis"] + "," + info["area_exit"]["y_axis"] + ")\n";
				break;
				
			case "detail":
				msg += BBLU"[设定]"NOR" 细节描述(detail)：\n";
			    if( (i=sizeof(info["detail"])) ) {
			    	mixed *k, *v;
			    	k = keys(info["detail"]);
			    	v = values(info["detail"]);
			    	while(i--)
			    		msg += "       " + k[i] + "：" + v[i] + "\n";
			    }
				break;
			case "guard":
				break;
			default:			// 其它设定
				msg += BGRN"[设定]"NOR" ";
			    msg += type + "：" + info[type] + "\n";
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
		write("这指令只能在区域中使用。\n");
		return 1;
	}

    if( !(info = me->query("area_info") ) ) {
    	write("使用指令出现错误。\n");
    	return 1;
    }

	if( !arg ) {
		show_info(me, info);
		return 1;
	}

    if( arg == "save" ) {
    	if( environment(me)->save() ) {
    		write("区域储存成功\。\n");
    		return 1;
    	}
    	write("区域储存失败。\n");
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
   			write("==================== 目前这个区域有载入物件的座标 ====================\n");
   			while(i--) {
   				if( sscanf(lolo[i], "%d,%d", y, x) == 2 ) {
   					write(sprintf("(%2d,%2d) ", x, y));
   					if( i >= 8 && i%8 == 0 ) write("\n");
   				} 
   			}
   			write("\n======================================================================\n");
   		} else write("这个区域目前都还没有要载入的物件。\n");
   		return 1;
    }

	if( sscanf(arg, "%s %d", type, value) != 2 )
		if( sscanf(arg, "%s %s", type, value) != 2) {
			write("指令参数不正确。\n");
			return 1;
		}
	
    if( value == "delete") {
    	if( type == "loads" ) {
    		if( !environment(me)->del_loads(info["x_axis"], info["y_axis"], value) ) {
    			write("删除座标载入物件失败。\n");
    			return 1;
    		}
    		write("删除座标载入物件成功\。\n");
    		return 1;
    	}
    	if( !environment(me)->delete_data(info["x_axis"], info["y_axis"], type) ) {
    		write("删除失败。\n");
    		return 1;
    	}
		write("你将座标 (" + info["x_axis"] + "," + info["y_axis"] + ") 的 " + type + " 资料删除。\n");
    	return 1;
    }

    // 设定区域形式出口
    if( type == "area_exit" ) {
		int x, y;
		string filename;
		if( sscanf(value, "%s %d %d", filename, x, y) != 3 ) {
			write("区域形式出口设定错误。 (指令格式：sa area_exit FILE_NAME X_AXIS Y_AXIS)\n");
			return 1;
		}
		if( !environment(me)->set_area_exit(info["x_axis"], info["y_axis"], filename, x, y) ) {
			write("区域形式出口设定失败。\n");
			return 1;
		} 
		write("区域形式出口设定成功\。\n");
    	return 1;
    }

    // 设定载入物件集
    if( type == "loads" ) {
    	int amount;
    	string filename;
    	if( sscanf(value, "%s %d", filename, amount) != 2 ) {
    		write("区域载入物件设定错误。 (指令格式：sa loads FILE_NAME AMOUNT)\n");
    		return 1;
    	}
    	
    	if( !environment(me)->set_loads(info["x_axis"], info["y_axis"], filename, amount) ) {
    		write("区域载入物件设定失败。\n");
    		return 1;
    	}
    	write("区域载入物件设定成功\。\n");
    	return 1;
    }

    if( type == "detail" ) {
    	string title, detail;
    	if( sscanf(value, "%s %s", title, detail) == 2 ) {
    		if( environment(me)->set_area_detail(info["x_axis"], info["y_axis"], title, detail) )
    			write("设定细节描述成功\。\n");
    	} else write("设定细节描述失败。\n");
    	return 1;
    }

    // 如果是设定icon先转换色码
	if( type == "icon" ) {
		value = color(value);
		if( sizeof(uncolor(value)) != 2 ) {
			write("设定座标上的icon必需是二个字元大小。\n");
			return 1;
		}
	}

    // 其它项目设定
   	if( !environment(me)->set_data(info["x_axis"], info["y_axis"], type, value) ) {
		write("设定失败。\n");
		return 1;
	}
	
	write("你将座标 (" + info["x_axis"] + "," + info["y_axis"] + ") 的 " + type + " 资料设定为 " + value + " 。\n");
	
	return 1;
}

int help(object me)
{
    string msg = "这是指令是用来设定区域座标相关资料的指令(目前所处的座标)。\n"
BBLU"\n1. 指令格式： sa\n"NOR
"   可以列出目前所处座标的相关资讯。\n"
BBLU"\n2. 指令格式： sa loads\n"NOR
"   查询区域中会载入物件的所有座标列表。\n"
BBLU"\n3. 指令格式： sa loads FILE_NAME AMOUNT\n"NOR
"   这个指令可以设定目前身处的座标，会自动载入某物件于此座标中。\n"
"   例： sa loads __DIR__npc 2      这样会载入相对目录路径中的档名npc二只在座标上。\n"
"        sa loads __DIR__npc/mob 4  这样会载入相对路径中npc目录下的mob档案四只在座标上。\n"
BBLU"\n4. 指令格式： sa area_exit FILE_NAME X_AXIS Y_AXIS\n"NOR
"   这个指令可设定区域型式的出口。\n"
"   例： sa area_exit __DIR__area 50 20  当人物移到这里时，会自动转移到area这个区域的(50,20)位置。\n"
BBLU"\n5. 指令格式： sa room_exit FILE_NAME\n"NOR
"   这个指令可以设定房间型式的出口。\n"
"   例： sa room_exit __DIR__room  当人物移到这里时，会自动转移到房间room中。\n"
BBLU"\n6. 指令格式： sa detail TITLE DETAIL\n"NOR
"   这个指令是用来设定房间的细节描述的，支援search, look指令，应用于quest。\n"
"   例： sa detail 大树 这是一棵大树，或许\你可以爬(climb)上去。\n"
BBLU"\n7. 指令格式： sa TYPE VALUE\n"NOR
"   这个指令是用来设定其它功\能的设定。\n"
"   例： sa icon $YEL$屎$NOR$  设定图示, 此指令支援色码。\n"
"        sa no_fight 1         非战区\n"
"        sa nonprompt_icon 1   不会即时改变图示的设定(意思是人物、物件移到这座标时图示不会改变)\n"
"        sa short 道路         设定座标的短序述\n"
"        sa long xxxxxxxx      设定座标的长序述，如果太长最好配合to指令, 如： to sa long\n"
BBLU"\n8. 指令格式： sa TYPE delete\n"NOR
"   这是用来删除区域资料的指令。\n"
"   例： sa loads delete      删除座标的载入物件集\n"
"        sa area_exit delete  删除座标的区域型式出口\n"
"        sa room_exit delete  删除座标的房间型式出口\n"
"        sa icon delete       删除座标的图示\n"
BBLU"\n9. 指令格式： sa save\n"NOR
"   这是储存区域档案的指令，当区域做了一些变更后，必需使用此指令来储存。\n"
BBLU"\n10. 指令格式： sa save_ansi\n"NOR
"   这是储存区域全图的指令，用来配合map指令。\n"
BBLU"\n11. 指令格式： sa save_html\n"NOR
"   这是储存区域全图的指令，用于网页上。\n"
"\n注：在区域里可使用的档案路径：\n"
"      __DIR__     相对路径，区域的目录在哪 __DIR__ 便指向哪\n"
"      _DIR_AREA_  指向 /world/area/ 目录下\n"
"      _DIR_NPC_   指向 /world/npc/ 目录下\n"
"      _DIR_WP_    指向 /world/wp/ 目录下\n"
"      _DIR_EQ_    指向 /world/eq/ 目录下\n"
"      _DIR_ITEM_  指向 /world/item/ 目录下\n"
"      _DIR_WORLD_ 指向 /world/ 目录下\n";
    me->start_more(msg);
    return 1;
}
