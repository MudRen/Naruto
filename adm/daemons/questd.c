/* questd.c  write by -Acme-
   ������ר���ṩMUD������������б���ͳһ������̵��趨ֵ�ʹ������
   �����ָ�� quest ��ʹ��

   ���Ͻṹ
   quest = ([ "mu": ([ ID: ([ "name": "", "detail": "", "trigger": "", "end":i ]),
                       ID: ([]),
                       ID: ([]),
                       ID: ([]),
                       ID: ([]),
                    ]),
              "yi": ([]),
              "wu": ([]),
              "sn": ([]),
              "yn": ([]),
              "yy": ([]),
              "cn": ([]),
              "pn": ([]),
              "or": ([]),
           ])
*/

#pragma save_binary
#include <ansi.h>

inherit F_CLEAN_UP;
inherit F_SAVE;
inherit F_DBASE;

mapping quest;

void create()
{
    seteuid(getuid());
    if( !restore() && !mapp(quest) ) quest = ([]);
    set("name", "������");
    set("id", "questd");
}

int remove()
{
    save();
    return 1;
}

string query_save_file() { return DATA_DIR + "questd"; }

mapping query_quests() { return quest; }

mapping query_branch(string branch)
{
    if( !stringp(branch) ) return 0;
    if( undefinedp(quest[branch]) ) return 0;
    return quest[branch];
}

mapping query_quest(string branch, int qid)
{
    if( !stringp(branch) || !intp(qid) ) return 0;
    if( undefinedp(quest[branch]) ) return 0;
    if( undefinedp(quest[branch][qid]) ) return 0;
    return quest[branch][qid];
}

int set_quest(string branch, int qid, mapping info)
{
    if( !stringp(branch) || !intp(qid) || !mapp(info) ) return 0;
    if( undefinedp(quest[branch]) ) quest[branch] = ([]);
    quest[branch][qid] = info;
    save();
    return 1;
}

int check_trigger(string branch, int qid, string msg)
{
    if( !stringp(branch) || !intp(qid) ) return 0;
    if( undefinedp(quest[branch]) ) return 0;
    if( undefinedp(quest[branch][qid]) ) return 0;
    if( undefinedp(quest[branch][qid]["trigger"]) ) return 0;
    return quest[branch][qid]["trigger"] == msg;
}

int check_end(object who, string branch, int qid)
{
    if( !objectp(who) || !stringp(branch) || !intp(qid) ) return 0;
    if( undefinedp(quest[branch]) ) return 0;
    if( undefinedp(quest[branch][qid]) ) return 0;
    if( undefinedp(quest[branch][qid]["end"]) || !intp(quest[branch][qid]["end"]) ) return 0;
    return who->query_quest(branch, qid) == quest[branch][qid]["end"];
}