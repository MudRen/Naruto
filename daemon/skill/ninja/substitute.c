/* ������������(substitute) */

#include <ansi.h>

inherit SKILL;

string *dodge_msg = ({
    "ֻ������������һ����ԭ��$N��������һ���ľͷ",
    "ɲ�Ǽ�$n����������һ���ľͷ",
    "һ�����̹�ȥ��ԭ��$N��������һ���ľͷ",
});


void create()
{
    seteuid(getuid());
    DAEMON_D->register_skill_daemon("substitute");
    setup();
}

// Ӧ���� dodge ��
int valid_enable(string base_skill) { return base_skill=="dodge"; }

int perform_action(object me, string act, object target)
{
    return notify_fail("����û��������ʽ�����á�\n");
}

// ����
string query_class() { return "ninja"; }

// ��ʾ�ã����� score
int dodge_show(object me, int ability)
{
    if( !me ) return 0;
    if( !me->skill_mapped("dodge") ) return 0;
    if( living(me) ) me->set_temp("evade_message", dodge_msg[random(sizeof(dodge_msg))]);
    return me->query_skill("dodge", 1) + me->query_skill("substitute", 1);
}

int dodge_using(object me, int ability)
{
    int dodge, sub;
    
    if( !me ) return 0;
    if( !me->skill_mapped("dodge") ) return 0;
    
    dodge = SKILL_D("dodge")->dodge_using(me, ability);
    sub = me->query_skill("substitute");

    if( ability && random(200) < random(sub) ) {
        if( living(me) ) me->set_temp("evade_message", dodge_msg[random(sizeof(dodge_msg))]);

        // ���mһЩ�����
        me->damage_stat("mp", 1+random(5), me);

        // ���Ӿ���ֵ
        me->improve_skill("substitute", 1);

        dodge += sub;
    }

    return dodge;
}

// �������Ӿ���, ������������
void skill_improved(object me, string skill) { }

// �������������д˺�ʽ����Ҫ��
void skill_advanced(object me, string skill)
{
    // �������趨�����´���������ľ���

    int lv;

    lv = me->query_skill(skill, 1);	// ���ܵȼ�

    switch( lv ) {
        case 189: me->set_skill_require(skill, 1500000);  break; // �ؿ�3 : 189��190 ��Ҫһ����ʮ����
        case 149: me->set_skill_require(skill, 1000000);  break; // �ؿ�2 : 149��150 ��Ҫһ������
        case  99: me->set_skill_require(skill, 500000);   break; // �ؿ�1 :  99��100 ��Ҫ��ʮ����
        default:  me->set_skill_require(skill, lv*lv*5); break;
    }
}