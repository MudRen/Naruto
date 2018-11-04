/* quest.c wirte by -Acme-

   ����ĸ�ʽ
   ID = ([ "name":                  // ��������
           "detail":                // ����ϸ��˵��
           "trigger":               // ���񴥷����
           "end":                   // ���������Ҫ�ﵽ������ֵ
        ]);
*/

#include <ansi.h>
//#include <quest.h>

inherit "/adm/daemons/questd.c";

int do_list_quest(string branch, string name)
{
    int index, size;
    mapping bran;
    string msg;

    msg  = "�q�����������������������������������������������������������r\n";
    msg += sprintf("��%-58s��\n", name+"�����б�" );
    msg += "��������������������������������������������������������������\n";
    msg += sprintf("��"BRED"%6s"NOR" "BBLU"%|51s"NOR"��\n", " ��� ", " �������� ");

    if( mapp(bran = QUEST_D->query_branch(branch)) && (size = sizeof(bran)) ) {
        for(index=1;index<=size;index++) {
            if( undefinedp(bran[index]) ) continue;
            msg += sprintf("��%|6d %-51s��\n", index, bran[index]["name"] );
        }
    }

    msg += "�t�����������������������������������������������������������s";

    this_player()->start_more(msg);

    return 1;
}

int do_show_detail(string branch, int id)
{
    string msg;
    mapping quest;

    if( !mapp(quest = QUEST_D->query_quest(branch, id)) )
        return notify_fail("û����������š�\n");

    msg  = sprintf(BRED"%|6d"NOR" "BBLU"%|55s"NOR"\n", id,  quest["name"] );
    msg += "��������������������������������������������������������������\n";
    msg += quest["detail"] + "\n";
    msg += "��������������������������������������������������������������";
    if( wizardp(this_player()) ) {
        msg += "\n������: " + quest["trigger"] + "\n";
        msg += "��������: " + quest["end"];
    }

    this_player()->start_more(msg);

    return 1;
}

int main(object me, string arg)
{
    int id;
    string select;

    if( !arg ) return notify_fail("������ help quest �˽�ָ���ʽ��\n");

    if( sscanf(arg, "-%s %d", select, id) == 2 )
        return do_show_detail(select , id);

    if( sscanf(arg, "-%s", select) == 1 ) {
        switch( select ) {
            case "mu": return do_list_quest("mu", "ľҶ��"); break;  // ľҶ
            case "yi": return do_list_quest("yi", "������"); break;  // ����
            case "wn": return do_list_quest("wn", "������"); break;  // ����
            case "sn": return do_list_quest("sn", "ɰ����"); break;  // ɰ��
            case "yn": return do_list_quest("yn", "������"); break;  // ����
            case "yy": return do_list_quest("yy", "������"); break;  // ����
            case "cn": return do_list_quest("cn", "������"); break;  // ����
            case "pn": return do_list_quest("pn", "������"); break;  // ����
            case "or": return do_list_quest("or", "����"); break;    // ����
            default: return notify_fail("�������������� help quest �˽�ָ���ʽ��\n");
        }
    }

    return 1;
}

int help(object me)
{
    write(@HELP
ָ���ʽ��
  quest <���ߴ�ѡ��>           �г�ĳ���ߴ����������
                               �磺 quest -mu ���г�����ľҶ��������б�
  quest <���ߴ�ѡ��> <���>    �磺 quest -mu 1 ����ʾľҶ����Ϊ1���������˵��

���ߴ�ѡ�
  -mu ľҶ��(muye)             -yi ������(yinyin)
  -wn ������(wuyin)            -sn ɰ����(shayin)
  -yn ������(yuyin)            -yy ������(yunyin)
  -cn ������(caoyin)           -pn ������(puyin)
  -or ����(other)
HELP);
    return 1;
}