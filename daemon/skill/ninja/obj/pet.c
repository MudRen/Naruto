// write by -Acme-
/* pet.c �����Ȯ */

#include <ansi.h>
#include <npc.h>

inherit F_VILLAGER;

object owner;

int attack = 0;         // �ɹ������
int evade = 0;          // �ɻر����
int food_maximum = 999; // ʳ��������
int food = 0;           // Ŀǰʳ����
int exp = 0;            // ����ֵ

void set_attack(int i) { attack = i; }
void set_evade(int i) { attack = i; }

int query_food_maximum() { return food_maximum; }
int query_food() { return food; }
int query_exp() { return exp; }

// ʳ��������
int food_gain(int fo)
{
	if( food + fo > food_maximum ) return 0;
	food += fo;
	return 1;
}

// ����ֵ���
void exp_gain(int ex)
{
	if( this_object()->query_level() >= 50 ) return;

	exp += ex;
	
    // �������
	if( this_object()->query_level() * this_object()->query_level() * 10 <= exp ) {
        exp = 0;
        this_object()->set_level(this_object()->query_level()+1);
        tell_object(owner, HIY"�����Ȯ " + this_object()->query("name") + " �ĵȼ������� " + this_object()->query_level() + " ����\n"NOR);
	}

	return;
}

// ����
varargs int attack(object opponent)
{
    if( !attack ) return 0;

    // �л������Ӿ���ֵ
    if( random(100) > random(100) ) exp_gain(1);

    return ::attack(opponent);
}

// �ر�
varargs int evade(int ability, object from)
{
	if( !evade ) return 0;

    // �л������ʳ����
    if( random(100) > random(100) ) food_gain(-1);

	return ::evade(ability, from);
}

// �趨����
int set_owner(object obj)
{
    if( !objectp(obj) ) return 0;
    owner = obj;
}

// ��ѯ����
object query_owner()
{
    if( !objectp(owner) ) return 0;
    return owner;
}

// ɾ����Ȯ
void do_destruct()
{
    message_vision("$Nһ���̵ľ��ܵ���Ӱ������...\n", this_object());
    destruct(this_object());
	return;
}

// ��ʱ��������Ƿ����
void do_check()
{
    // ������ʧ������ȮҲ��ʧ
    if( !objectp(owner) ) {
    	do_destruct();
        return;
    }
    
    // ʳ�������
    if( food <= 0 ) {
    	tell_object(owner, HIR"�����Ȯ " + this_object()->query("name") + " �ƺ����Ӷ��ú�...\n"NOR);
        this_object()->damage_stat("ap", 10, this_object()); // �۾���
        this_object()->damage_stat("hp", 10, this_object()); // ������
        this_object()->damage_stat("mp", 10, this_object()); // �۲����
    	return;
    }
    
    // ����ʳ����
    food -= random(3) + 1;
    
    // ��Ȯ�ָ�״̬�ȽϿ�
    this_object()->heal_stat("ap", this_object()->query_level());
    this_object()->heal_stat("hp", this_object()->query_level());
    this_object()->heal_stat("mp", this_object()->query_level());

    return;
}

void die()
{
    message_vision("$N�ҽС��á����á�������������...\n", this_object());
    exp = 0;
    return;
}

// �Ƿ��������
int accept_order(object who, string cmd)
{
	if( owner != who ) return 0;

	// ѱ�����������150�����Ż��ܿ���
	if( who->query_skill("animal taming", 1) <= 150 ) return 0;

	return 1;
}
	
void create()
{
    set_name("��Ȯ", ({ "dog" }));
    set_race("beast");

    // ƽʱ���������owner���ڲ���
    set("chat_chance", 1);
    set("chat_msg", ({
        (: do_check :),
    }));

    // ս��ʱ���������owner���ڲ���
    set("chat_chance_combat", 1);
    set("chat_msg_combat", ({
        (: do_check :),
    }) );

    setup();
}