/* advance */

#include <ansi.h>
#include <attribute.h>

inherit F_CLEAN_UP;

int add_attribute(object me, string attr, int amount)
{
    if( me->query_point("attribute") < amount ) {
        write("������Ե������㣬�޷������������ԡ�\n");
        return 1;
    }
    
    if( me->query_attr(attr, 1) + amount > ATTRVAL_MAX ) {
        write("������� " + attr + " �޷�������ô�࣬����Ϊ"+ chinese_number(ATTRVAL_MAX) +"��\n");
        return 1;
    }
    
    me->add_point("attribute", -amount);
    me->set_attr(attr, me->query_attr(attr, 1) + amount);
    
    write(HIY"������� " + attr + " ����" + chinese_number(amount) + "���ˣ�\n"NOR);
    
    return 1;
}

int add_state(object me, string state, int amount)
{
    if( me->query_point("score") < amount ) {
        write("���Ǳ�ܵ������㣬�޷���������״̬��\n");
        return 1;
    }
    
    if( me->query_stat_maximum(state) + amount > 9999 ) {
        write("���״̬ " + state + " �޷�������ô�࣬����Ϊ��ǧ�Űپ�ʮ�š�\n");
        return 1;
    }
    
    me->add_point("score", -amount);
    me->advance_stat(state, amount);
    me->heal_stat(state, amount);

    write(HIY"���״̬ " + state + " ����"+ chinese_number(amount) +"���ˣ�\n"NOR);

    return 1;
}

int add_skill(object me, string skill, int amount)
{
    int i;
    if( me->query_point("skill") < amount ) {
        write("��ļ��ܵ������㣬�޷��������＼�����ޡ�\n");
        return 1;
    }

    if( me->query_skill_ability(skill) + amount > 10 ) {
        write("��ļ��� " + skill + " �޷�������ô�࣬����Ͷ�����Ϊʮ�㡣\n");
        return 1;
    }
    
    me->add_point("skill", -amount);

    for(i=1;i<=amount;i++) me->add_skill_ability(skill);
    
    write(HIY"��ļ��� " + skill + " ��������" + chinese_number(amount*20) + "���ˣ�\n"NOR);
    
    return 1;
}

int main(object me, string arg)
{
    int amount;

    if( !arg ) return notify_fail("�������������ʲô������\n");
    
    if( sscanf(arg, "%s for %d", arg, amount) != 2 ) amount = 1;

    if( amount < 0 ) return notify_fail("���뽵�������������\n");

    // ������������
    if( arg == "str" || arg == "con" || arg == "dex" || arg == "int" ) {
        add_attribute(me, arg, amount);
        return 1;
    }
    
    // ��������״̬
    if( arg == "ap" || arg == "hp" || arg == "mp" ) {
        add_state(me, arg, amount);
        return 1;
    }
    
    // �������＼��
    if( me->query_skill(arg) ) {
        add_skill(me, arg, amount);
        return 1;
    }
    
    write("û�� " + arg + " �������ԡ�״̬���ܣ����޷������κ�������\n");

    return 1;
}

int help()
{
    write(@TEXT
ָ���ʽ�� advance <���ԡ�״̬����> [for <����>]

���ָ��������������������

����
advance str          �����������������1�㣬ʹ�ú�۳�һ�����Ե���
advance con for 3    ����������������3�㣬ʹ�ú�۳��������Ե���
advance dex          �����������������1�㣬ʹ�ú�۳�һ�����Ե���
advance int for 2    ����������ǻ�����2�㣬ʹ�ú�۳��������Ե���

advance ap           �������ﾫ��ֵ״̬1�㣬ʹ�ú�۳�һ��Ǳ�ܵ���
advance hp for 50    ������������ֵ״̬50�㣬ʹ�ú�۳���ʮ��Ǳ�ܵ���
advance mp           ������������״̬1�㣬ʹ�ú�۳�һ��Ǳ�ܵ���

advance combat      ����combatͶһ�㣬������������20��
advance dodge for 3 ����dodgeͶ���㣬������������60��
TEXT);
    return 1;
}