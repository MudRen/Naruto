/* continual_hit.c ����
*/

#include <ansi.h>
inherit SKILL;

string query_class() { return "basic"; }

void create()
{
    seteuid(getuid());
    DAEMON_D->register_skill_daemon("continual hit");
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
    	case 189: me->set_skill_require(skill, 3000000); break; // �ؿ�3 : 189��190
    	case 149: me->set_skill_require(skill, 2000000); break; // �ؿ�2 : 149��150
    	case  99: me->set_skill_require(skill, 1000000); break; // �ؿ�1 :  99��100
    	default:  me->set_skill_require(skill, lv*lv*15); break;
    }
}