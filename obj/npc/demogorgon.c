// demongorgon.c

#include <ansi.h>

inherit NPC;

void create()
{
        set_name( HIW "����" NOR, ({ "demogorgon" }) );
        set_level(100);
        set_race("human"); 
       set("long",
                "    ����������վ������ǰ�����ߴ��������һ��������Ϣ��\n"
                "����ɱ����ѹ���㼸����������������������ƺ���һ�������ڸ�\n"
                "����s�� ���뿪 ������\n");

        set("age", 500);

        advance_stat("kee",900);
        advance_stat("gin",900);

        set_attr("str",35);
        set_attr("dex",42);
        set_attr("con",35);
         set_attr("spi",45);
        set_attr("cor",45);

        setup();

}

void start_shutdown()
{
        if( geteuid(previous_object()) != ROOT_UID ) return;

        message("system",
                HIR "\n����������д���һ����µ����� ....\n\n"
                HIW     "����" HIR "���ͳ�������������Ķ��ڣ�\n\n"
                        "\t\t��Ҫ���ģ����ǻ�������ӿ��ԸϿ��뿪�ɣ���\n\n"
                        "\t\t��Ȼ�Ļ���������и��𡫡�\n\n" NOR,
                users() );
        call_out("countdown", 240, 61);
}

private void countdown(int sec)
{
        sec--;
        if( sec > 59) {
                message("system",
                        HIW "\n����" HIR "���ͳ�������������Ķ��ڣ�\n\n"
                "\t\t���ǻ���" + chinese_number(sec) + "���ӿ����뿪��\n\n"
                    "\t\t��Ҫ����������Կ��ɣ���\n\n" NOR,
                        users() );
                call_out("countdown", 49, 11);
        }else if( sec > 0) {
                message("system",
                        HIW "\n����" HIR "���ͳ�������������Ķ��ڣ�\n\n"
                                "\t\t���ǻ���" + chinese_number(sec) + "���ӵ�ʱ������ӣ���\n\n"
                                "\t\t�������ľ͸Ͽ��Ӱɣ�����\n\n" NOR,
                        users() );
                call_out("countdown", 1, sec);
        }
        else {
                message("system",
                        HIW "\n����" HIR "�÷�ŭ��������������\n\n"
                            "\t\t�찡�� �ذ���  �񰡡� \n\n" NOR,
                        users() );
                call_out("do_shutdown", 2);
        }
}

private void do_shutdown()
{
        message("system",
                HIW "\n\n����" HIR "�÷�ŭ�������ȵ��s\n\n"
                HIW "\t\tͨ ͨ �� �� һ �� �� �� �� �� �������� �u\n\n"
                HIR "\t\t����һ�ϣ�������̱��Ѫ�죬��....����ǰ��һ�ж����Ѫ��...\n\n"
                        "\t\t���е�һ�ж���˲�������....\n\n"
                        "\t\tȻ�������ǰ��һƬ�ڰ�....��ֹ���ĺڰ�....\n\n" NOR,
                users() );
        shutdown(0);
}

void die()
{
        object ob;

        if( !ob = query_temp("last_damage_from") )
                ob = this_player(1);

        if( !ob ) return;

        message("system",
                HIR "\n\n������һ�������ߺޡ����֡���Ұ�ް���������쳹������ա�\n\n"
                HIW "����" HIR "˻�Ƶغ����s�ɶ��" + ob->short() + "�o��һ���ᱨ��ġ�����\n\n"
                        "Ȼ��һ����ɫ�����Х�������ƶ˩o����ָֻ���������\n\n" NOR,
                users() );
        
        destruct(this_object());
}
