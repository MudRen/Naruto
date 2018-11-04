/* heavy_hit.c �ػ�
*/

#include <ansi.h>
inherit SKILL;

string query_class() { return "basic"; }

void create()
{
    seteuid(getuid());
    DAEMON_D->register_skill_daemon("heavy hit");
    setup();
}

// ��ʾ��
int heavy_hit_show(object me, int ability)
{
	int stable;

    if( !me ) return 0;
    if( !me->skill_mapped("heavy hit") ) return 0;

    stable = me->query_skill("heavy hit");

    ability = (stable * ability/200) + ((stable * ability/2+1)/200);

    return ability;
}

// ʹ���ػ�
int heavy_hit_using(object me, int ability)
{
	int stable;
	
    if( !me ) return 0;
    if( !me->skill_mapped("heavy hit") ) return 0;

    stable = me->query_skill("heavy hit");
    
    ability = (stable * ability/200) + ((stable * random(ability)+1)/200);

    // ���Ӿ���
    me->improve_skill("heavy hit", 1);

    return ability;
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