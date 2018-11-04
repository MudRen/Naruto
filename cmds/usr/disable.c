/*  disable.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    object ob;
    mapping m;
    string sk, sk_m;

    if( !arg || !(ob = SKILL_D(arg)) || ob==SKILL_D("none") )
        return notify_fail("��Ҫͣ��ʲô���ܣ�\n");

    if( me->is_busy() )
        return notify_fail("������æ�������, �޷�ר�ġ�\n");

//    if (!me->query_learn(arg)) 
    if (!me->query_skill(arg,1)) 
        return notify_fail("��Ŀǰ��������ܡ�\n");

    if( !function_exists("valid_enable", ob) ) {
        me->map_skill(arg, "none");
        write("�������ʱֹͣʹ����ѧ�йء�" + to_chinese(arg) + "���ļ��ɡ�\n");
    } else {
	int c = 0;
	m = me->query_skill_map();
	foreach(sk, sk_m in m) {
	    if( sk_m==arg ) {
		me->map_skill(sk);
		write("�����ֹͣ�ڡ�" + to_chinese(sk) + "������ֹͣʹ����ѧ�йء�" + to_chinese(arg) + "���ļ��ɡ�\n");
		c++;
	    }
	}
	if( !c ) {
	    write("�����ֹͣʹ����ѧ�йء�" + to_chinese(arg) + "���ļ��ɡ�\n");
	    me->map_skill(arg, "none");
	}
    }

    return 1;
}

int help()
{
    write(@TEXT
ָ���ʽ��disable <����>

���ָ������ֹͣʹ��ĳ�ּ��ܡ�ֻ��ֹͣʹ�ö��ѣ������������������ܡ�

�������ָ�enable��skills��abandon
TEXT);
    return 1;
}
