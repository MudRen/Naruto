/* ������ɰ����(sand tie) */

#include <ansi.h>

#define SKILL_NAME "sand tie"

inherit SKILL;

// ����
string query_class() { return "ninja"; }

void create()
{
    seteuid(getuid());
    DAEMON_D->register_skill_daemon(SKILL_NAME);
    setup();
}

// �������Ӿ���, ������������
void skill_improved(object me, string skill) { }

// �������������д˺�ʽ����Ҫ��
void skill_advanced(object me, string skill)
{
    // �������趨�����´���������ľ���

    int lv;

    lv = me->query_skill(skill, 1);					// ���ܵȼ�

    switch( lv ) {
    	case 189: me->set_skill_require(skill, 1500000);  break; // �ؿ�3 : 189��190 ��Ҫһ����ʮ����
    	case 149: me->set_skill_require(skill, 1000000);  break; // �ؿ�2 : 149��150 ��Ҫһ������
    	case  99: me->set_skill_require(skill, 500000);   break; // �ؿ�1 :  99��100 ��Ҫ��ʮ����
    	default:  me->set_skill_require(skill, lv*lv*5); break;
    }
}