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

    /* �ȹ�����δ������ɵ�ʹ���ߣ������ε��ϼ���ʦ��
     * ע���ڱ� rwho_q ���е�״���£�this_player() Ϊ 0��
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
        		target = filter(all, (: $1->query("guild")=="muye" :));     // ľҶ��
                msg_format("5");
                break;
            case "-yi":
        		target = filter(all, (: $1->query("guild")=="yinyin" :));   // ������
                msg_format("5");
                break;
            case "-wn":
        		target = filter(all, (: $1->query("guild")=="wuyin" :));    // ������
                msg_format("5");
                break;
            case "-sn":
        		target = filter(all, (: $1->query("guild")=="shayin" :));   // ɰ����
                msg_format("5");
                break;
            case "-yn":
        		target = filter(all, (: $1->query("guild")=="yuyin" :));    // ������
                msg_format("5");
                break;
            case "-yy":
        		target = filter(all, (: $1->query("guild")=="yunyin" :));   // ������
                msg_format("5");
                break;
            case "-cn":
        		target = filter(all, (: $1->query("guild")=="caoyin" :));   // ������
                msg_format("5");
                break;
            case "-pn":
        		target = filter(all, (: $1->query("guild")=="puyin" :));    // ������
                msg_format("5");
                break;
        	case "-cr":
        		target = filter(all, (: $1->query_class()=="commoner" :));  // ƽ��
                msg_format("5");
        		break;
            case "-ir":
                target = filter(all, (: $1->query_class()=="inferior" :));  // ����
                msg_format("5");
                break;
            case "-me":
                target = filter(all, (: $1->query_class()=="middle" :));    // ����
                msg_format("5");
                break;
            case "-sr":
                target = filter(all, (: $1->query_class()=="superior" :));  // ����
                msg_format("5");
                break;
            case "-an":
                target = filter(all, (: $1->query_class()=="assassin" :));  // ����
                msg_format("5");
                break;
            case "-pe":
                target = filter(all, (: $1->query_class()=="police" :));    // ����
                msg_format("5");
                break;
            case "-mh":
                target = filter(all, (: $1->query_class()=="monarch" :));   // Ӱ��
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
	            return notify_fail("������ help who �˽�ָ���ʽ��\n");
        }
    }

    tmp = msg;
//    tmp += sprintf("�����г� %3d/%3d λʹ����  ϵͳ���ɣ�%-37s��\n",  sizeof(target), sizeof(all), query_load_average());
    tmp += sprintf("��%-72s��\n", "���г� " + sizeof(target) + "/" + sizeof(all) + " λʹ����  ϵͳ���ɣ�" + query_load_average());
    tmp += "�t�������������������������������������������������������������������������s";
    
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
        msg  = "�q�������������������������������������������������������������������������r\n";
        msg += sprintf("��%-72s��\n", MUD_NAME+"��������б�" );
        msg += "����������������������������������������������������������������������������\n";
        msg += "��"BYEL"�ȼ�"NOR" "BRED"  ְ    ҵ  "NOR" "BGRN"  ͷ      ��  "NOR" "BMAG"  ��      ��  "NOR" "BBLU"        ����(ID)        "NOR"��\n";

        while( j-- ) {
        	if( target[j]->query("nickname") ) ntmp = uncolor(target[j]->query("nickname"));
        	else ntmp = "";
            tmp = sprintf("��%|4d %s%|12s"NOR" %|14s %|14s %12s%-12s��\n"NOR,
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
            msg = sprintf("%s��%|4d %s%|12s"NOR" %|14s %|14s %12s%-12s��\n"NOR,
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
        msg += "����������������������������������������������������������������������������\n";

        break;
    case "41":
        target = sort_array(all, "sort_user", this_object());
        j = sizeof(target);
        msg  = "�q�������������������������������������������������������������������������r\n";
        msg += sprintf("��%-72s��\n", MUD_NAME+"��������б�" );
        msg += "�t�������������������������������������������������������������������������s\n";

        while( j-- ) {
            msg = sprintf("%s  %-10s%s", msg, capitalize(target[j]->query("id")), (( i - j ) % 6 ) ? "" : "\n" );
        }
        if( i % 6 ) msg += "\n";
        msg += "�q�������������������������������������������������������������������������r\n";
        break;
    case "42":
        target = sort_array(all, "sort_user", this_object());
        j = sizeof(target);
        msg  = "�q�������������������������������������������������������������������������r\n";
        msg += sprintf("��%-72s��\n", MUD_NAME+"��������б�" );
        msg += "�t�������������������������������������������������������������������������s\n";
        while( j-- ) {
            msg = sprintf("%s  %-12s%s", msg, target[j]->query("name", 1), (( i - j ) % 6 ) ? "" : "\n" );
        }
        if( i % 6 ) msg += "\n";
        msg += "�q�������������������������������������������������������������������������r\n";
        break;
    default:
        msg = "������ help who �˽�ָ���ʽ��\n";
    }
}

int help(object me)
{
    write(@HELP
ָ���ʽ : who [ѡ��]

���ָ������г������ϵ�ʹ���߼�����������Ѷ��
ѡ�������м���, һ��ֻ��ʹ��һ��ѡ��, �� who -i -w ��ͬ who -i��
��ѡ��ʱ, �г�����ȼ���� 5 ������֮ʹ���ߡ�

һ��ѡ�
  -w  ֻ�г���ʦ                     -l  �г�����ʹ����
  -n  ֻ�г���������                 -i  ֻ�г�Ӣ��ID

ְҵѡ�
  -cr �г�����ƽ��(commoner)         -ir �г���������(inferior)
  -me �г���������(middle)           -sr �г���������(superior)
  -an �г����а���(assassin)         -pe �г����о���(police)
  -mh �г�����Ӱ��(monarch)

���ߴ�ѡ�
  -mu �г�����ľҶ������(muye)       -yi �г���������������(yinyin)
  -wn �г���������������(wuyin)      -sn �г�����ɰ��������(shayin)
  -yn �г���������������(yuyin)      -yy �г���������������(yunyin)
  -cn �г����в���������(caoyin)     -pn �г���������������(puyin)

���ָ� finger
HELP);
    return 1;
}
