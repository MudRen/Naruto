/* query.c wirte by -Acme-
*/

#include <ansi.h>
//#include <query.h>

inherit "/adm/daemons/itemd.c";

mapping named_apply = ([
    "str": "力量", "con": "体格", "int": "智慧", "dex": "敏捷",
    "attack": "攻击力量", "armor": "防御力量", "exact": "命中能力", "evade": "回避能力",
    "intimidate": "攻击技巧", "wittiness": "防御技巧", "move": "移动能力", "carriage": "负重能力",
    "ap": "精神值", "hp": "体力值", "mp": "查克拉",
    "head_eq": "头部", "face_eq": "脸部", "neck_eq": "颈部", "wrist_eq": "手腕",
    "hand_eq": "手部", "finger_eq": "手指", "cloth": "衣服", "armor": "铠甲",
    "waist_eq": "腰部", "pants": "裤子", "leg_eq": "腿部", "feet_eq": "足部",
]);

int sort_record(object ob1, object ob2)
{
    int ord;

    ord = wiz_level(ob1) - wiz_level(ob2);
    if( ord==0 )
        ord = (int)ob1->query_level() - (int)ob2->query_level();
    return ord;
}

int do_list(string show, string type)
{
    int index;
    string msg, tmp, name;
    mapping record, data;

    record = ITEM_D->do_query_all(type);

    if( wizardp(this_player()) ) {
        msg  = "╭─────────────────────────────────────╮\n";
        msg += sprintf("│%-74s│\n", "目前火影忍者所有的" + show + "列表" );
        msg += "├─────────────────────────────────────┤\n";
        if( type == "npc" ) 
            msg += sprintf("│"BRED"%|4s"NOR" "BYEL"%|4s"NOR" "BBLU"%|16s"NOR" "BMAG"%|42s"NOR"│\n",
                            "编号", "  等 级  ", "名称", "档案路径");
        else msg += sprintf("│"BRED"%|4s"NOR" "BYEL"%|4s"NOR" "BBLU"%|16s"NOR" "BMAG"%|42s"NOR"│\n",
                           "编号", "单价/银两", "名称", "档案路径");

        if( sizeof(record) ) {
            foreach(index, data in record) {
                name = uncolor(data["name"]);
                tmp  = sprintf("│%|4d %9d %|16s %-42s│\n",
                           index,
                           (type == "npc") ? data["level"] : data["value"],
                           name,
                           data["file_name"]);
                tmp = replace_string(tmp, name, data["name"]);
                msg += tmp;
            }
        }
        msg += "╰─────────────────────────────────────╯";
    } else {
        msg  = "╭────────────────╮\n";
        msg += sprintf("│%-32s│\n", "目前火影忍者所有的" + show + "列表" );
        msg += "├────────────────┤\n";
        if( type == "npc" ) 
            msg += sprintf("│"BRED"%|4s"NOR" "BYEL"%|4s"NOR" "BBLU"%|17s"NOR"│\n",
                            "编号", "  等 级  ", "名称");
        else msg += sprintf("│"BRED"%|4s"NOR" "BYEL"%|4s"NOR" "BBLU"%|17s"NOR"│\n",
                           "编号", "单价/银两", "名称");

        if( sizeof(record) ) {
            foreach(index, data in record) {
                name = uncolor(data["name"]);
                tmp  = sprintf("│%|4d %9d %|17s│\n",
                           index,
                           (type == "npc") ? data["level"] : data["value"],
                           name );
                tmp = replace_string(tmp, name, data["name"]);
                msg += tmp;
            }
        }
        msg += "╰────────────────╯";
    }

    this_player()->start_more(msg);

    return 1;
}

int do_scan(string dir)
{
    int i;
    object ob;
    mixed *file, *dirent;
    file = get_dir(dir, -1);

    if( !sizeof(file) ) {
        if( file_size(dir) == -2 ) write("目录是空的。\n");
        else write("没有这个目录。\n");
        return 1;
    }

    foreach( dirent in file ) {
        write(dirent[0] + "\n");
        ob = new(dir + dirent[0]);
        if( objectp(ob) ) {
            ITEM_D->add_item(ob, 0);
            destruct(ob);
        }
    }

    ITEM_D->save();

    write("Ok.\n");

    return 1;
}

int do_show_detail(string select, int id)
{
    string msg = "", sub_type;
    object obj;
    mapping record, apply;

    switch( select ) {
        case "eq": record = ITEM_D->do_query("eq", id); break;
        case "wp": record = ITEM_D->do_query("wp", id); break;
        case "npc": record = ITEM_D->do_query("npc", id); break;
        case "food": record = ITEM_D->do_query("food", id); break;
        case "obj": record = ITEM_D->do_query("obj", id); break;
    }

    if( !mapp(record) ) return notify_fail("没有这个编号。\n");

    if( !objectp(obj = new(record["file_name"])) ) {
        if( file_size(record["file_name"]) == -1 ) {
            switch( select ) {
                case "eq": record = ITEM_D->do_del("eq", id); break;
                case "wp": record = ITEM_D->do_del("wp", id); break;
                case "npc": record = ITEM_D->do_del("npc", id); break;
                case "food": record = ITEM_D->do_del("food", id); break;
                case "obj": record = ITEM_D->do_del("obj", id); break;
            }
        }
        return notify_fail("你目前无法查询这项物品的资讯。\n");
    }

    if( select == "npc" ) msg += "等级：" + obj->query("level") + "\n";
    msg += "名称：" + obj->query("name") + "(" + obj->query("id") + ")\n";
    msg += "\n" + obj->query("long") + "\n";

    if( select == "eq" ) {
    	sub_type = obj->query("wear_as");
        msg += sprintf("护具部位：%s\n", !undefinedp(named_apply[sub_type]) ? named_apply[sub_type] : sub_type );
        apply = obj->query("apply_armor/" + sub_type);
        if( mapp(apply) ) {
            string prop;
            mixed value;
            
            msg += "护具特性：\n";
            foreach(prop, value in apply) {
                if( !undefinedp(named_apply[prop]) ) prop = named_apply[prop];
                msg += sprintf("  %-30s %O\n", to_chinese(prop), value);
            }
        }
    }
    
    if( select == "wp" ) {
        string s;
        mapping dp;
        mixed sk = obj->query("wield_as");
        
        msg += "可使用方式：\n";

        if( stringp(sk) ) sk = ({ sk });
        foreach(s in sk) {
            msg += sprintf("\n☉ %s (%s)\n", to_chinese(s), s);
            dp = obj->query("damage/" + s);
            msg += sprintf("   伤害力： %d - %d (伤害力修正 %d%%，武器等级 %d)\n", 
                dp["lower"], dp["upper"], dp["bonus"], dp["level"] );

            apply = obj->query("apply_weapon/" + s);
            if( mapp(apply) ) {
                string prop;
                mixed value;
            
                msg += "\n   武器特性：\n";
                foreach(prop, value in apply) {
                    if( !undefinedp(named_apply[prop]) ) prop = named_apply[prop];
                    else prop = to_chinese(prop);
                    msg += sprintf("     %-30s %O\n", prop, value);
                }
            }
        }
    }

    this_player()->start_more(msg);

    destruct(obj);

    return 1;
}

int main(object me, string arg)
{
    int id;
    string select;

    if( !arg ) return notify_fail("指令格式错误。\n");

    if( wizardp(me) ) {
        if( !sscanf(arg, "scan %s", select) == 1 )
            if( arg == "scan" ) select = me->query("cwd");
        if( stringp(select) ) return do_scan(select);
    }

    if( sscanf(arg, "-%s %d", select, id) == 2 )
        return do_show_detail(select , id);

    if( sscanf(arg, "-%s", select) == 1 ) {
        switch( select ) {
            case "eq": return do_list("装备", "eq"); break;
            case "wp": return do_list("武器", "wp"); break;
            case "npc": return do_list("人物", "npc"); break;
            case "food": return do_list("食物", "food"); break;
            case "obj": return do_list("其它", "obj"); break;
            default: return notify_fail("指令格式错误。\n");
        }
    }

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式：
  query <选项>           列出某类型的所有物品
                         如： query -eq   会列出所有装备
  query <选项> <编号>    会显示该编号的物品的详细资料

选项：
  -eq    装备               -wp    武器
  -npc   非玩家角色         -food  食品
  -obj   其它
HELP);

    if( wizardp(me) ) {
    write(@HELP

巫师指令： query scan [<目录>]    将某目录下的物件加入系统列表中
                                  不加目录位置，则是依目前工作目录下
HELP);
    }
    return 1;
}