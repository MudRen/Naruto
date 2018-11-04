// stake.c ���� 
// 2002/04/13 -Tmr
// update by -Acme-

inherit NPC;

#define LIFE 10000

void create()
{
    set_name("����", ({ "wood stake", "stake", "_OBJ_STAKE_" }) );
    set_race("human");
    set("long", "һ��ľ�Ƶļ��ˣ�ר�Ÿ��������\�õġ�\n");
    // ���˵�����ֵ
    set("stake_life", LIFE);
    setup();
    set_attr("str", 10);
    set_attr("con", 10);
    set_attr("dex", 10);
    set_attr("int", 10);
    delete_stat("hp");
}

// ���Խ��ܹ���
int accept_fight(object ob) { return 1; }

// ���˲���ر�
varargs int evade(int ability, object from) { return 0; }

// �����޷�����
varargs int defend(int damage, object from) { return damage; }

//���˲��ṥ��
varargs int attack(object opponent) { return 0; }

varargs int receive_damage(int damage, object from, object attacker)
{
    // û�й�����
    if( !attacker ) return 0;

    // ���˵����� �ж�
    add("stake_life",-damage);
    if( query("stake_life") < 0 ) {
        message_vision("�������������죬$N��ɻ�����Ƭ��ɢ�������ˡ�\n",this_object());
        remove_all_killer();
        call_out( (:destruct($1):),1,this_object());
    }       
    return damage;
}
