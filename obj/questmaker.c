/* write by -Acme */
#include <ansi.h>

inherit ITEM;

void create()
{
    set_name("����༭��", ({ "quest maker", "maker" }));
    set_weight(1);
    if( !clonep() ) {
        set("unit", "��");
        set("value", 0);
        set("no_drop", 1);
        set("long", "���ָ�\n"
        	        HIW"1. sq\n\n"NOR
        	        "��ѯĿǰ���趨״̬\n\n"
                    HIW"2. sq <���> <����>\n\n"NOR
                    "�趨�����������\n"
                    "����趨��\n"
                    "name: �趨��������\n"
                    "detail: �趨����ϸ�ڣ���ϸ�ڽϳ���������� to ָ��� to sq detail\n"
                    "trigger: �趨������䣬�ҵ�����NPCʱ������˵��仰�㿪ʼ����\n"
                    "end: �����ڵڼ������������׫д����ʱ���жϸ������Ƿ����\n\n"
                    HIW"3. create <���> <���ߴ�ѡ��>\n\n"NOR
                    "<���ߴ�ѡ��> ������ help maker\n"
                    "<���> �����ָ�� quest ��ѯ��ţ����������\n"
                    "������ǰ�趨��������Ѷ�������������ṩ����ϵͳʹ�á�\n\n"
                    HIW"4. help maker\n\n"NOR
                    "������Ƶ����˵��" );
    }
    setup();
}

void init()
{
    if( !environment()->is_area() ) {
	    add_action("do_set", "sq");
	    add_action("do_help", "help");
	    add_action("do_create", "create");
	}
}

int show_help()
{
    string msg;

    msg = @LONG
���ߴ�ѡ��
  -mu ľҶ��(muye)             -yi ������(yinyin)
  -wn ������(wuyin)            -sn ɰ����(shayin)
  -yn ������(yuyin)            -yy ������(yunyin)
  -cn ������(caoyin)           -pn ������(puyin)
  -or ����(other)

������ƾ���

�����趨

sq name ľҶ���ﹷ����
sq detail ľҶ��ļ���С�㣬��Ϊ����Ҫ�������Ҫ���˰�æ�ﹷ��
sq trigger ����С���Ұ����ﹷ
sq end 3

����������趨���ϵ�ľҶ���ﹷ��������id Ϊ1, ������ľҶ��(mu)�ġ�
Ҳ�����������������������ṩ(����ĳNPC�ṩ)��

1. �����ȥ�����������ľҶ���ﹷ����ʱ���㽫��� palyer->add_quest("mu", 1, 1);
   �����趨���ȡ�� ľҶ��(mu) ������ IDΪ1 ��������Ŀǰ�Ĳ���� 1 
   Ŀǰ������Ϊ 1

2. �ҵ�����С����� say ����С���Ұ����ﹷ ʱ�����ڼ���С�������ж�player��ָ��say
   �������Ƿ��봥�������ͬ QUEST_D->check_trigger("mu", 1, "�����˵�Ļ�") ��Ϊ��ֵ
   ���ʾ�Ǵ�����䣬���ڼ���С�������ж� player->query_quest("mu", 1)
   ľҶ��(mu) �� ���� IDΪ1 �������Ƿ�Ϊ 1, Ҳ�����о���������1, ���ǣ��򽫹���player��
   �� player->add_quest("mu", 1, 1); �ٽ������ 1
   Ŀǰ������Ϊ 2

3. �������깷�� �� player->add_quest("mu", 1, 1); �ٽ������ 1
   Ŀǰ������Ϊ 3, (������������е� end �趨)

4. ��ȥ�Ҽ���С�㣬�ڼ���С�������ж� QUEST_D->check_end(player, "mu", 1) �Ƿ�Ϊ��ֵ����Ϊ
   ���ʾ�����Ѿ�����������ƽ�����Ķ�����
LONG;

   this_player()->start_more(msg);
   return 1;
}

int do_help(string arg)
{
	if( arg == "maker" ) return show_help();
	return 0;
}

int do_set_quest(string kind, mixed value)
{
	mapping m;
	if( !mapp(m=this_object()->query("sq")) ) {
	   m = ([]);
	   this_object()->set("sq", m);
	}
    m[kind] = value;
    this_object()->set("sq", m);
    write("Ok.\n");
    return 1;
}

int do_create(string arg)
{
	string kind;
	int iv;
    if( sscanf(arg, "%d -%s", iv, kind) == 2 ) {
    	mapping sq;
        if( !mapp(sq = this_object()->query("sq")) ) return notify_fail("Ŀǰû���κε��趨��\n");
        QUEST_D->set_quest(kind, iv, sq);
        this_object()->delete("sq");
        write("Ok.\n");
        return 1;
    }
    return notify_fail("ָ���ʽ����\n");
    return 1;
}

int do_set(string arg)
{
	int iv;
	string kind, sv;

    if( !arg ) {
    	mapping sq;
        if( !mapp(sq = this_object()->query("sq")) ) return notify_fail("Ŀǰû���κε��趨��\n");
        write("name:" + sq["name"] + "\n");
        write("detail:" + sq["detail"] + "\n");
        write("trigger:" + sq["trigger"] + "\n");
        write("end:" + sq["end"] + "\n");
        return 1;
    }

    if( sscanf(arg, "%s %d", kind, iv) == 2 ) return do_set_quest(kind, iv);
    if( sscanf(arg, "%s %s", kind, sv) == 2 ) return do_set_quest(kind, sv);

    return notify_fail("ָ���ʽ����ȷ��\n");

	return 1;
}