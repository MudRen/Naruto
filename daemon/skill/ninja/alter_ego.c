/* alter_ego.c ������������ */
    
#include <ansi.h>

#define SKILL_NAME "alter ego"
#define SHADOW_NUM 5

inherit SKILL;

void create()
{
    seteuid(getuid());
    DAEMON_D->register_skill_daemon(SKILL_NAME);
    setup();
}


// Ӧ���� ����(ninja)
int valid_enable(string base_skill) { return base_skill == "ninja"; }

// ����
string query_class() { return "ninja"; }

// �������ʽ
void construct_shadow(object me)
{
    int i, count;
    object shadow;
    mapping map;
    
    if( me->query_skill(SKILL_NAME, 1) < 40 ) {
        write("����������������ܵȼ����㣬ÿ����ʮ���ɶ�����һֻ����\n");
        return;
    }

    // ӵ�������mһЩ�����
    if( me->query_stat("mp") < 20 ) {
        write("��Ŀǰû���㹻�Ĳ������ʹ�÷�������\n");
        return;
    }

    map = me->query_temp("shadow");
    count = sizeof(map);
    
    // Ѱ�ҿ��ñ�� count
    if( count ) {
        count = 0;
        for(i=0;i<SHADOW_NUM;i++)
            if( objectp(map["shadow"+i]) ) count++;
    }

    if( count >= SHADOW_NUM || count >= me->query_skill(SKILL_NAME, 1)/40 ) {
        write("��ķ��������Ѿ������ˣ��޷��ٴ������ķ���\n");
        return;
    }
    
    shadow = new(__DIR__"obj/shadow.c");
    if( !objectp(shadow) ) {
        write("���ƺ��޷��������...\n");
        destruct(shadow);
        return;
    }

    message_vision("$N˫�ֿ��ٵĽ�ӡ��ʩչ��"HIW"������"NOR"����\n", me);

    me->damage_stat("mp", 20, me);
    me->start_busy(3);

    if( environment(me)->is_area() ) {
        if( !area_move(environment(me), shadow, me->query("area_info/x_axis"), me->query("area_info/y_axis")) ) {
            write("��ķ������������ʧ������...\n");
            destruct(shadow);
            return;
        }
    } else {
        if( !shadow->move(environment(me)) ) {
            write("��ķ������������ʧ������...\n");
            destruct(shadow);
            return;
        }
    }

    // �趨����, ID
    shadow->set_name(me->query("name"), ({ me->query_id() + "'s alter ego",  "ego" }));
    shadow->set("nickname", me->query("nickname"));
    shadow->set("title", me->query("title"));

    // ��ͬ�ȼ�
    shadow->set("level", me->query_level());

    // ��ͬ����
    shadow->set_attr("str", me->query_attr("str"));
    shadow->set_attr("con", me->query_attr("con"));
    shadow->set_attr("int", me->query_attr("int"));
    shadow->set_attr("dex", me->query_attr("dex"));

    // ��ͬ״̬
    shadow->set_stat_current("ap", me->query_stat_maximum("ap"));
    shadow->set_stat_maximum("ap", me->query_stat_maximum("ap"));
    shadow->set_stat_current("hp", me->query_stat_maximum("hp"));
    shadow->set_stat_maximum("hp", me->query_stat_maximum("hp"));
    shadow->set_stat_current("mp", me->query_stat_maximum("mp"));
    shadow->set_stat_maximum("mp", me->query_stat_maximum("mp"));

    shadow->guard_ob(me);       //����leader
    shadow->set_leader(me);     //����leader
    shadow->set_owner(me);      //�趨owner

    shadow->set("skill", "alter ego");  // shadow����alter ego���ܽг�

    me->set_temp("shadow/shadow"+count, shadow);
    
    // ���Ӽ��ܾ���
    me->improve_skill("alter ego", 1);

    return;
}

// ɾ������ʽ
void destruct_shadow(object me)
{
    object shadow, *obs;
    
    if( !me->query_temp("shadow") ) {
        write("��Ŀǰû���κεķ�����ڡ�\n");
        return;
    }
    
    if( sizeof(obs = values(me->query_temp("shadow"))) <= 0 ) {
        write("��Ŀǰû���κεķ�����ڡ�\n");
        return;
    }

    foreach(shadow in obs) if( objectp(shadow) ) shadow->do_destruct();
    
    me->delete_temp("shadow");
    
    write("������з�����ʧ�ˡ�\n");
    
    return;
}

// ʹ�þ���
int perform_action(object me, string act, object target)
{
    if( !objectp(me) ) return 0;
    if( me->is_busy() ) return notify_fail("������û�п�ʹ�÷�������\n");

    switch( act ) {
        case   "shadow": construct_shadow(me); break;
        case "destruct": destruct_shadow(me);  break;
        default: return notify_fail("Ŀǰ������ shadow, destruct ���ֹ�\�ܡ�\n"); break;
    }
    
    return 1;
}

// �������Ӿ���, ������������
void skill_improved(object me, string skill) { }

// �������������д˺�ʽ����Ҫ��
void skill_advanced(object me, string skill)
{
    // �������趨�����´���������ľ���

    int lv;

    lv = me->query_skill(skill, 1);    // ���ܵȼ�

    switch( lv ) {
        case 189: me->set_skill_require(skill, 1500000);  break; // �ؿ�3 : 189��190 ��Ҫһ����ʮ����
        case 149: me->set_skill_require(skill, 1000000);  break; // �ؿ�2 : 149��150 ��Ҫһ������
        case  99: me->set_skill_require(skill, 500000);   break; // �ؿ�1 :  99��100 ��Ҫ��ʮ����
        default:  me->set_skill_require(skill, lv*lv*5); break;
    }
}