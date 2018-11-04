/* query.c wirte by -Acme-
*/

#include <ansi.h>
//#include <query.h>

inherit "/adm/daemons/itemd.c";

mapping named_apply = ([
    "str": "����", "con": "���", "int": "�ǻ�", "dex": "����",
    "attack": "��������", "armor": "��������", "exact": "��������", "evade": "�ر�����",
    "intimidate": "��������", "wittiness": "��������", "move": "�ƶ�����", "carriage": "��������",
    "ap": "����ֵ", "hp": "����ֵ", "mp": "�����",
    "head_eq": "ͷ��", "face_eq": "����", "neck_eq": "����", "wrist_eq": "����",
    "hand_eq": "�ֲ�", "finger_eq": "��ָ", "cloth": "�·�", "armor": "����",
    "waist_eq": "����", "pants": "����", "leg_eq": "�Ȳ�", "feet_eq": "�㲿",
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
        msg  = "�q���������������������������������������������������������������������������r\n";
        msg += sprintf("��%-74s��\n", "Ŀǰ��Ӱ�������е�" + show + "�б�" );
        msg += "������������������������������������������������������������������������������\n";
        if( type == "npc" ) 
            msg += sprintf("��"BRED"%|4s"NOR" "BYEL"%|4s"NOR" "BBLU"%|16s"NOR" "BMAG"%|42s"NOR"��\n",
                            "���", "  �� ��  ", "����", "����·��");
        else msg += sprintf("��"BRED"%|4s"NOR" "BYEL"%|4s"NOR" "BBLU"%|16s"NOR" "BMAG"%|42s"NOR"��\n",
                           "���", "����/����", "����", "����·��");

        if( sizeof(record) ) {
            foreach(index, data in record) {
                name = uncolor(data["name"]);
                tmp  = sprintf("��%|4d %9d %|16s %-42s��\n",
                           index,
                           (type == "npc") ? data["level"] : data["value"],
                           name,
                           data["file_name"]);
                tmp = replace_string(tmp, name, data["name"]);
                msg += tmp;
            }
        }
        msg += "�t���������������������������������������������������������������������������s";
    } else {
        msg  = "�q���������������������������������r\n";
        msg += sprintf("��%-32s��\n", "Ŀǰ��Ӱ�������е�" + show + "�б�" );
        msg += "������������������������������������\n";
        if( type == "npc" ) 
            msg += sprintf("��"BRED"%|4s"NOR" "BYEL"%|4s"NOR" "BBLU"%|17s"NOR"��\n",
                            "���", "  �� ��  ", "����");
        else msg += sprintf("��"BRED"%|4s"NOR" "BYEL"%|4s"NOR" "BBLU"%|17s"NOR"��\n",
                           "���", "����/����", "����");

        if( sizeof(record) ) {
            foreach(index, data in record) {
                name = uncolor(data["name"]);
                tmp  = sprintf("��%|4d %9d %|17s��\n",
                           index,
                           (type == "npc") ? data["level"] : data["value"],
                           name );
                tmp = replace_string(tmp, name, data["name"]);
                msg += tmp;
            }
        }
        msg += "�t���������������������������������s";
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
        if( file_size(dir) == -2 ) write("Ŀ¼�ǿյġ�\n");
        else write("û�����Ŀ¼��\n");
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

    if( !mapp(record) ) return notify_fail("û�������š�\n");

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
        return notify_fail("��Ŀǰ�޷���ѯ������Ʒ����Ѷ��\n");
    }

    if( select == "npc" ) msg += "�ȼ���" + obj->query("level") + "\n";
    msg += "���ƣ�" + obj->query("name") + "(" + obj->query("id") + ")\n";
    msg += "\n" + obj->query("long") + "\n";

    if( select == "eq" ) {
    	sub_type = obj->query("wear_as");
        msg += sprintf("���߲�λ��%s\n", !undefinedp(named_apply[sub_type]) ? named_apply[sub_type] : sub_type );
        apply = obj->query("apply_armor/" + sub_type);
        if( mapp(apply) ) {
            string prop;
            mixed value;
            
            msg += "�������ԣ�\n";
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
        
        msg += "��ʹ�÷�ʽ��\n";

        if( stringp(sk) ) sk = ({ sk });
        foreach(s in sk) {
            msg += sprintf("\n�� %s (%s)\n", to_chinese(s), s);
            dp = obj->query("damage/" + s);
            msg += sprintf("   �˺����� %d - %d (�˺������� %d%%�������ȼ� %d)\n", 
                dp["lower"], dp["upper"], dp["bonus"], dp["level"] );

            apply = obj->query("apply_weapon/" + s);
            if( mapp(apply) ) {
                string prop;
                mixed value;
            
                msg += "\n   �������ԣ�\n";
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

    if( !arg ) return notify_fail("ָ���ʽ����\n");

    if( wizardp(me) ) {
        if( !sscanf(arg, "scan %s", select) == 1 )
            if( arg == "scan" ) select = me->query("cwd");
        if( stringp(select) ) return do_scan(select);
    }

    if( sscanf(arg, "-%s %d", select, id) == 2 )
        return do_show_detail(select , id);

    if( sscanf(arg, "-%s", select) == 1 ) {
        switch( select ) {
            case "eq": return do_list("װ��", "eq"); break;
            case "wp": return do_list("����", "wp"); break;
            case "npc": return do_list("����", "npc"); break;
            case "food": return do_list("ʳ��", "food"); break;
            case "obj": return do_list("����", "obj"); break;
            default: return notify_fail("ָ���ʽ����\n");
        }
    }

    return 1;
}

int help(object me)
{
    write(@HELP
ָ���ʽ��
  query <ѡ��>           �г�ĳ���͵�������Ʒ
                         �磺 query -eq   ���г�����װ��
  query <ѡ��> <���>    ����ʾ�ñ�ŵ���Ʒ����ϸ����

ѡ�
  -eq    װ��               -wp    ����
  -npc   ����ҽ�ɫ         -food  ʳƷ
  -obj   ����
HELP);

    if( wizardp(me) ) {
    write(@HELP

��ʦָ� query scan [<Ŀ¼>]    ��ĳĿ¼�µ��������ϵͳ�б���
                                  ����Ŀ¼λ�ã�������Ŀǰ����Ŀ¼��
HELP);
    }
    return 1;
}