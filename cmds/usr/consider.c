/* consider.c */

inherit F_CLEAN_UP;

string consider(int ability)
{
	if( ability < -150 ) return "Զʤ����";
	if( ability < -80 ) return "�߳�������";
	if( ability < -40 ) return "�߳���һ��";
	if( ability < -10 ) return "������ʤһ��";
	if( ability < 10 ) return "����ֻ�ڲ���֮��";
	if( ability < 40 ) return "��ѷ��һ��";
	if( ability < 80 ) return "����������һ��";
	if( ability < 150 ) return "���������Ϊ����";
	return "Զѷ����";
}

int main(object me, string arg)
{
    object ob, weapon;
	int me_ability, ob_ability;
    mapping wielded_weapon;
    string msg, sk, art, skill;

	if( !arg ) return notify_fail("��������˭��ʵ����\n");
	
	ob = present(arg, environment(me));
	if( !ob || !ob->is_character() || !ob->visible(me) )
	    return notify_fail("����û�� " + arg + " ����ˡ�\n");

    msg = "������" + ob->query("name") + "��ʵ����\n";
    
    // ��������
    me_ability = 0;
    wielded_weapon = me->query_temp("weapon");
    if( mapp(wielded_weapon) && sizeof(wielded_weapon) ) {
        foreach(skill, weapon in wielded_weapon) {
            art = me->skill_mapped("combat");
            if( ! art ) continue;
            me_ability += weapon->show_damage();
        }
    } else me_ability = me->show_damage();

    ob_ability = 0;
    wielded_weapon = ob->query_temp("weapon");
    if( mapp(wielded_weapon) && sizeof(wielded_weapon) ) {
        foreach(skill, weapon in wielded_weapon) {
            art = ob->skill_mapped("combat");
            if( ! art ) continue;
            ob_ability += weapon->show_damage();
        }
    } else ob_ability = ob->show_damage();

    msg += "��������" + consider(me_ability-ob_ability);

    // ������
    me_ability = me->query_ability("defend");
    if( (sk=me->skill_mapped("parry")) ) me_ability += SKILL_D(sk)->parry_show(me, 0);

    ob_ability = ob->query_ability("defend");
    if( (sk=ob->skill_mapped("parry")) ) ob_ability += SKILL_D(sk)->parry_show(ob, 0);

    msg += "����������" + consider(me_ability-ob_ability) + "��\n";

    // ��������
    me_ability = me->query_ability("intimidate"),
    ob_ability = ob->query_ability("intimidate"),

    msg += "��������" + consider(me_ability-ob_ability);

    // ��������
    me_ability = me->query_ability("wittiness"),
    ob_ability = ob->query_ability("wittiness"),

    msg += "����������" + consider(me_ability-ob_ability) + "��\n";

    // ������
    me_ability = me->exact();
    ob_ability = ob->exact();

    msg += "��������" + consider(me_ability-ob_ability);

    // �ر���
    me_ability = me->query_ability("evade");
    if( (sk=me->skill_mapped("dodge")) ) me_ability += SKILL_D(sk)->dodge_show(me, 0);

    ob_ability = ob->query_ability("evade");
    if( (sk=ob->skill_mapped("dodge")) ) ob_ability += SKILL_D(sk)->dodge_show(ob, 0);

    msg += "���ر�����" + consider(me_ability-ob_ability) + "��\n";

    write(msg);

    return 1;
}

int help()
{
    write(@TEXT
ָ���ʽ��consider ����

���ָ�������������һ�����ֵ�ʵ����
TEXT);
    return 1;
}
