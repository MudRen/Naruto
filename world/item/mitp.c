/* testpaper.c ���̿��Ծ� write by -Acme- */

inherit ITEM;

mapping test = ([
    1: ([ "msg": "�ڻ�Ӱ���������У��������ߴ��ǣ�\n"
                 "           (1) ������\n"
                 "           (2) ������\n"
                 "           (3) ľҶ��\n"
                 "           (4) ɰ����\n",
          "correct": 3,
          "answer": 0,
       ]),
    2: ([ "msg": "���ߴ���ͨ��������������֯��ͨ����������\n"
                 "           (1) ��Ц��\n"
                 "           (2) ���¼������ɱ��׷ɱ����\n"
                 "           (3) ����ָ����֯\n"
                 "           (4) ��������\n",
          "correct": 2,
          "answer": 0,
       ]),
    3: ([ "msg": "���顰�ٱ����߽�������ǰ�� �� \"��\" �������˫����ʹ����ӡ��\n"
                 "           (1) ��ָ����ֿ� ˫��Ĵָ��ʳָΧ��Բ��\n"
                 "           (2) ʳָ ��ָ Ĵָ\n"
                 "           (3) ʳָ Ĵָ СĴָ\n"
                 "           (4) ���� ʳָ ���� ��סʳָ\n",
          "correct": 1,
          "answer": 0,
       ]),
    4: ([ "msg": "ʲô�ǲ������\n"
                 "           (1) ��һ��ʳ��\n"
                 "           (2) ʹ������������������Դ\n"
                 "           (3) Ǯ�ҵĵ�λ\n"
                 "           (4) ��һ����֯\n",
          "correct": 2,
          "answer": 0,
       ]),
    5: ([ "msg": "ʹ�÷���ǰ����͸��ʲô������������\n"
                 "           (1) ��ů���\n"
                 "           (2) ����\n"
                 "           (3) �ڱ�ʺ\n"
                 "           (4) ��������ӡ\n",
          "correct": 4,
          "answer": 0,
       ]),
    6: ([ "msg": "ľҶ��������Ӱ�ǣ�\n"
                 "           (1) Գ��\n"
                 "           (2) ����Ҳ\n"
                 "           (3) ����\n"
                 "           (4) ����\n",
          "correct": 3,
          "answer": 0,
       ]),
    7: ([ "msg": "������һλ��ľҶ������̣�\n"
                 "           (1) ʨ����\n"
                 "           (2) ������\n"
                 "           (3) С����\n"
                 "           (4) ������\n",
          "correct": 2,
          "answer": 0,
       ]),
    8: ([ "msg": "ʲô��������\n"
                 "           (1) �������ڵĲ���������ѻ��������ʹ�õķ���\n"
                 "           (2) ��һ����٣�ս��ǰʹ�õ�ů���\n"
                 "           (3) �Իþ��������״̬\n"
                 "           (4) �������ѵ�������������\n",
          "correct": 4,
          "answer": 0,
       ]),
    9: ([ "msg": "ʲô��������\n"
                 "           (1) �������ڵĲ���������ѻ��������ʹ�õķ���\n"
                 "           (2) ��һ����٣�ս��ǰʹ�õ�ů���\n"
                 "           (3) �Իþ��������״̬\n"
                 "           (4) �������ѵ�������������\n",
          "correct": 1,
          "answer": 0,
       ]),
   10: ([ "msg": "ʲô�ǻ�����\n"
                 "           (1) �������ڵĲ���������ѻ��������ʹ�õķ���\n"
                 "           (2) ��һ����٣�ս��ǰʹ�õ�ů���\n"
                 "           (3) �Իþ��������״̬\n"
                 "           (4) �������ѵ�������������\n",
          "correct": 3,
          "answer": 0,
       ]),
]);

string do_look_long()
{
    int index, size;
    string msg;
    
    size = sizeof(test);

    msg  = this_object()->query("name") + ", ��ʹ�� answer ָ������\n";
    msg += "������������������������������������������������������������������������\n";

    for(index=1;index<=size;index++) {
        if( undefinedp(test[index]) ) continue;
        msg += sprintf("��%|3s��%2d. %s\n",
               test[index]["answer"] ? test[index]["answer"] + "" : "",
               index,
               test[index]["msg"] );
    }

    msg += "������������������������������������������������������������������������";
    return msg;
}

int do_check()
{
    int index, score = 0 , size;
    size = sizeof(test);

    for(index=1;index<=size;index++) {
        if( undefinedp(test[index]) ) continue;
        if( test[index]["correct"] == test[index]["answer"] ) score += 10;   //���һ���ʮ��
    }

    return score;
}

void create()
{
    set_name("ľҶ�����̿��Ծ�", ({ "muye inferior ninja test paper", "paper"}));
    set_weight(500);
    if( !clonep() ) {
        set("unit", "��");
        set("value", 0);
        set("long", (: do_look_long :) );
    }
    setup();
}

void init()
{
    add_action("do_answer", "answer");
}

int do_answer(string arg)
{
    int no, ans;

    // paper������user���ϲ�����answer����
    if( !userp(environment(this_object())) ) return 0;

    if( !arg ) return notify_fail("ָ���ʽ�� answer <���> <��>\n");

    if( sscanf(arg, "%d %d", no, ans) != 2 ) return notify_fail("ָ���ʽ�� answer <���> <��>\n");

    if( undefinedp(test[no]) ) return notify_fail("û����һ����Ŀ��\n");
    
    tell_object(this_player(), "���ڵ� " + no + " ��Ĵ���д�� " + ans + " ����𰸡�\n");

    test[no]["answer"] = ans;

    this_object()->set("long", do_look_long());

    return 1;
}

string is_test_paper() { return "muye"; }