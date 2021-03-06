/*  who.c write by -Acme-
*/

#include <ansi.h>

inherit F_CLEAN_UP;

object *all, *target;
string msg;
int i, j;

void msg_format(string arg);

void create() { seteuid(getuid()); }

mixed main(object me, string arg, int remote)
{
    int center_lv;
    string tmp;
    target = ({});
    msg = "";

    /* 先过滤尚未登入完成的使用者，与隐形的上级巫师。
     * 注意在被 rwho_q 呼叫的状况下，this_player() 为 0。
     */
    if( this_player(1) )
        all = filter(users(), (: objectp(environment($1)) &&
                                 (!wizardp($1) || wiz_level($2)>wiz_level($1) || $1->visible($2)) :), this_player(1));
    else all = filter(users(), (: objectp(environment($1)) && !wizardp($1) :));

    i = sizeof(all);
    
    if( !arg || arg=="" ) {
	    center_lv = me->query_level();
        while( i-- ) {
            if( all[i]->query_level() - center_lv < 6 && center_lv - all[i]->query_level() < 6 && !wizardp(all[i]) )
                target += ({ all[i] });
        }
        msg_format("1");
    } else {
        switch(arg){
            case "-mu":
        		target = filter(all, (: $1->query("guild")=="muye" :));     // 木叶村
                msg_format("5");
                break;
            case "-yi":
        		target = filter(all, (: $1->query("guild")=="yinyin" :));   // 音隐村
                msg_format("5");
                break;
            case "-wn":
        		target = filter(all, (: $1->query("guild")=="wuyin" :));    // 雾隐村
                msg_format("5");
                break;
            case "-sn":
        		target = filter(all, (: $1->query("guild")=="shayin" :));   // 砂隐村
                msg_format("5");
                break;
            case "-yn":
        		target = filter(all, (: $1->query("guild")=="yuyin" :));    // 雨隐村
                msg_format("5");
                break;
            case "-yy":
        		target = filter(all, (: $1->query("guild")=="yunyin" :));   // 云隐村
                msg_format("5");
                break;
            case "-cn":
        		target = filter(all, (: $1->query("guild")=="caoyin" :));   // 草隐村
                msg_format("5");
                break;
            case "-pn":
        		target = filter(all, (: $1->query("guild")=="puyin" :));    // 瀑隐村
                msg_format("5");
                break;
        	case "-cr":
        		target = filter(all, (: $1->query_class()=="commoner" :));  // 平民
                msg_format("5");
        		break;
            case "-ir":
                target = filter(all, (: $1->query_class()=="inferior" :));  // 下忍
                msg_format("5");
                break;
            case "-me":
                target = filter(all, (: $1->query_class()=="middle" :));    // 中忍
                msg_format("5");
                break;
            case "-sr":
                target = filter(all, (: $1->query_class()=="superior" :));  // 上忍
                msg_format("5");
                break;
            case "-an":
                target = filter(all, (: $1->query_class()=="assassin" :));  // 暗忍
                msg_format("5");
                break;
            case "-pe":
                target = filter(all, (: $1->query_class()=="police" :));    // 警部
                msg_format("5");
                break;
            case "-mh":
                target = filter(all, (: $1->query_class()=="monarch" :));   // 影忍
                msg_format("5");
                break;
            case "-l":
                target = all;
                msg_format("5");
                break;
            case "-w":
	            target = filter(all, (: wizardp :));
                msg_format("5");
                break;
            case "-i":
                msg_format("41");
                break;
            case "-n":
                msg_format("42");
                break;
	        default :
	            return notify_fail("请输入 help who 了解指令格式。\n");
        }
    }

    tmp = msg;
//    tmp += sprintf("│共列出 %3d/%3d 位使用者  系统负荷：%-37s│\n",  sizeof(target), sizeof(all), query_load_average());
    tmp += sprintf("│%-72s│\n", "共列出 " + sizeof(target) + "/" + sizeof(all) + " 位使用者  系统负荷：" + query_load_average());
    tmp += "╰────────────────────────────────────╯";
    
    if( remote ) return tmp;

    me->start_more(tmp);
    return 1;
}

int sort_user(object ob1, object ob2)
{
    int ord;

    ord = wiz_level(ob1) - wiz_level(ob2);
    if( ord==0 )
        ord = (int)ob1->query_level() - (int)ob2->query_level();
    return ord;
}

void msg_format(string arg)
{
	string tmp, ntmp;
    if( !arg || arg=="" )    return;

    j = sizeof(target);
    target = sort_array(target, "sort_user", this_object());

    switch(arg){
    case "1":
    case "5":
    case "50":
        msg  = "╭────────────────────────────────────╮\n";
        msg += sprintf("│%-72s│\n", MUD_NAME+"线上玩家列表" );
        msg += "├────────────────────────────────────┤\n";
        msg += "│"BYEL"等级"NOR" "BRED"  职    业  "NOR" "BGRN"  头      衔  "NOR" "BMAG"  匿      称  "NOR" "BBLU"        名称(ID)        "NOR"│\n";

        while( j-- ) {
        	if( target[j]->query("nickname") ) ntmp = uncolor(target[j]->query("nickname"));
        	else ntmp = "";
            tmp = sprintf("│%|4d %s%|12s"NOR" %|14s %|14s %12s%-12s│\n"NOR,
                  target[j]->query_level(),
                  wizardp(target[j]) ? BOLD : NOR,
                  target[j]->rank(),
                  target[j]->query("title") ? target[j]->query("title") : "",
                  ntmp,
                  target[j]->query("name", 1),
                  "(" + capitalize(target[j]->query("id")) + ")" );
            if( ntmp != "" ) tmp = replace_string(tmp, ntmp, target[j]->query("nickname"));
            msg += tmp;
/*
            msg = sprintf("%s│%|4d %s%|12s"NOR" %|14s %|14s %12s%-12s│\n"NOR,
                  msg,
                  target[j]->query_level(),
                  wizardp(target[j]) ? BOLD : NOR,
                  target[j]->rank(),
                  target[j]->query("title") ? target[j]->query("title") : "",
                  target[j]->query("nickname") ? target[j]->query("nickname") : "",
                  target[j]->query("name", 1),
                  "(" + capitalize(target[j]->query("id")) + ")" );
*/
        }
        msg += "├────────────────────────────────────┤\n";

        break;
    case "41":
        target = sort_array(all, "sort_user", this_object());
        j = sizeof(target);
        msg  = "╭────────────────────────────────────╮\n";
        msg += sprintf("│%-72s│\n", MUD_NAME+"线上玩家列表" );
        msg += "╰────────────────────────────────────╯\n";

        while( j-- ) {
            msg = sprintf("%s  %-10s%s", msg, capitalize(target[j]->query("id")), (( i - j ) % 6 ) ? "" : "\n" );
        }
        if( i % 6 ) msg += "\n";
        msg += "╭────────────────────────────────────╮\n";
        break;
    case "42":
        target = sort_array(all, "sort_user", this_object());
        j = sizeof(target);
        msg  = "╭────────────────────────────────────╮\n";
        msg += sprintf("│%-72s│\n", MUD_NAME+"线上玩家列表" );
        msg += "╰────────────────────────────────────╯\n";
        while( j-- ) {
            msg = sprintf("%s  %-12s%s", msg, target[j]->query("name", 1), (( i - j ) % 6 ) ? "" : "\n" );
        }
        if( i % 6 ) msg += "\n";
        msg += "╭────────────────────────────────────╮\n";
        break;
    default:
        msg = "请输入 help who 了解指令格式。\n";
    }
}

int help(object me)
{
    write(@HELP
指令格式 : who [选项]

这个指令可以列出在线上的使用者及其等它相关资讯。
选项有下列几种, 一次只可使用一种选项, 即 who -i -w 视同 who -i。
无选项时, 列出与你等级相差 5 级以内之使用者。

一般选项：
  -w  只列出巫师                     -l  列出所有使用者
  -n  只列出中文名称                 -i  只列出英文ID

职业选项：
  -cr 列出所有平民(commoner)         -ir 列出所有下忍(inferior)
  -me 列出所有中忍(middle)           -sr 列出所有上忍(superior)
  -an 列出所有暗部(assassin)         -pe 列出所有警部(police)
  -mh 列出所有影忍(monarch)

忍者村选项：
  -mu 列出所有木叶村忍者(muye)       -yi 列出所有音隐村忍者(yinyin)
  -wn 列出所有雾隐村忍者(wuyin)      -sn 列出所有砂隐村忍者(shayin)
  -yn 列出所有雨隐村忍者(yuyin)      -yy 列出所有云隐村忍者(yunyin)
  -cn 列出所有草隐村忍者(caoyin)     -pn 列出所有瀑隐村忍者(puyin)

相关指令： finger
HELP);
    return 1;
}
