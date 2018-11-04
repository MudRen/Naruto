/* ������ʨ������(lion attack) */

#include <ansi.h>

#define SKILL_NAME "lion attack"

inherit SKILL;

void create()
{
    seteuid(getuid());
    DAEMON_D->register_skill_daemon(SKILL_NAME);
    setup();
}

// Ӧ���� force ��
int valid_enable(string base_skill) { return base_skill=="force"; }

int perform_action(object me, string act, object target)
{
    return notify_fail("����û��������ʽ�����á�\n");
}

// ����
string query_class() { return "force"; }

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