/* transfer.c write by -Acme- */
#include <ansi.h>

inherit ROOM;

mapping record = ([
    1: ([ "name": HIR"��֮��"NOR+BGRN"ľҶ��"NOR,
          "file": _DIR_AREA_"muye/inn.c",
         "price": 10 ]),
/*
    2: ([ "name": "ˮ֮�����̴�",
          "file": _DIR_AREA_"muye/inn.c",
         "price": 100 ]),
*/
]);

string do_list()
{
    int index, size;
    string msg;

    msg  = "�q�������������������������������������������������r\n";
    msg += "�����д����ر����                                ��\n";
    msg += "����������������������������������������������������\n";
    msg += "�����  ����/����  ����                           ��\n";
    msg += "���������������������������������������������������s\n";

    if( (size=sizeof(record)) )
        for(index=1;index<=size;index++)
            msg += sprintf("��%4d  %9d  %s\n", index, record[index]["price"], record[index]["name"]);

    msg += "���������������������������������������������������r\n";
    msg += "������ transfer <���> ���д��͡�                 ��\n";
    msg += "�t�������������������������������������������������s";

    return msg;
}

void create()
{
    set("short", "���д���");
    set("long", "��������һ��ǿ�ҵĲ������������ĳλ�����ڴ˲����ر��\n"
                "����������Է������ܹ������������ᣬ���������ǰ������һ�Ŵ�\n"
                "���ӣ���ͷ���߰����д��һ�����֡�\n" );

    set("detail", ([
        "����" : (: do_list :),
    ]));

    set("exits", ([
        "east" : ([ "filename" : __DIR__"wizard.c",
                       "x_axis" : 21,
                       "y_axis" : 20,
                  ]),
        "south" : ([ "filename" : __DIR__"wizard.c",
                       "x_axis" : 18,
                       "y_axis" : 23,
                  ]),
        "west" : ([ "filename" : __DIR__"wizard.c",
                       "x_axis" : 15,
                       "y_axis" : 20,
                  ]),
        "north" : ([ "filename" : __DIR__"wizard.c",
                       "x_axis" : 18,
                       "y_axis" : 17,
                  ]),
    ]));

    setup();
}

void init()
{
    add_action("do_transfer", "transfer");
}

int do_transfer(string arg)
{
    int number;
    object me = this_player(), room;

    if( me->is_busy() || me->is_fighting() ) 
        return notify_fail("������û�пա�\n");

    if( !arg )
        return notify_fail("����Ҫ���͵����\n");

    if( sscanf(arg, "%d", number) != 1 )
        return notify_fail("����Ҫ���͵����\n");

    if( undefinedp(record[number]) )
        return notify_fail("����Ҫ���͵����\n");

    if( me->query("bank") < record[number]["price"] )
        return notify_fail("���������Ǯ���㣬�޷����͡�\n");

    if( !objectp(room = load_object(record[number]["file"])) )
        return notify_fail("�������޷����͵��Ǹ��ط���\n");

    message_vision("����ͻȻ������һ��ǿ�磬$Nһ˲��ͱ�������...\n", me);

    // �ƶ��ɹ����踶Ǯ
    if( me->move(room) ) me->add("bank", -record[number]["price"]);

    message_vision("����ͻȻ������һ��ǿ�磬$N��ǿ�����������...\n", me);

    me->start_busy(3);

    return 1;
}