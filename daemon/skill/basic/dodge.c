#include <ansi.h>

inherit SKILL;

string *dodge_msg = ({
    "���Ǻ�$p$lƫ�˼���",
    "���Ǳ�$p����ض㿪��",
    "����$n����һ�࣬���˿�ȥ",
    "���Ǳ�$p��ʱ�ܿ�",
    "����$n����׼�������Ų�æ�Ķ㿪",
});

string query_class() { return "basic"; }

void create()
{
    seteuid(getuid());
    DAEMON_D->register_skill_daemon("dodge");
    setup();
}

// ��ʾ�ã����� score
int dodge_show(object me, int ability)
{
    if( !me ) return 0;
    if( !me->skill_mapped("dodge") ) return 0;
    if( living(me) ) me->set_temp("evade_message", dodge_msg[random(sizeof(dodge_msg))]);
    return me->query_skill("dodge");
}

// ʹ��dodge
int dodge_using(object me, int ability)
{
    if( !me ) return 0;
    if( !me->skill_mapped("dodge") ) return 0;
    if( living(me) ) me->set_temp("evade_message", dodge_msg[random(sizeof(dodge_msg))]);

    // ������Ծ���㼼�ܾ���
    me->improve_skill("dodge", 1);

    return me->query_skill("dodge");
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