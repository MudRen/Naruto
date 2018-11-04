// write by -Acme-
/* shadow.c
   �������ṥ��������ر�
   Ӱ�����ṥ������ر�
*/

#include <npc.h>

inherit F_VILLAGER;

object owner;
int attack = 0;        // �ɹ������
int evade = 0;         // �ɻر����

void set_attack(int i) { attack = i; }
void set_evade(int i) { attack = i; }

// ����
varargs int attack(object opponent)
{
    if( !attack ) return 0;
    return ::attack(opponent);
}

// �ر�
varargs int evade(int ability, object from)
{
	if( !evade ) return 0;
	return ::evade(ability, from);
}

// �趨ӵ����
int set_owner(object obj)
{
    if( !objectp(obj) ) return 0;
    owner = obj;
}

// ��ѯӵ����
object query_owner()
{
    if( !objectp(owner) ) return 0;
    return owner;
}

// ɾ��Ӱ����
void do_destruct()
{
    message_vision("��������������һ����$N����ʧ������...\n", this_object());
    destruct(this_object());
	return;
}

// ��ʱ���ӵ�����Ƿ����㹻�������������ȡ
void do_check()
{
    // ӵ������ʧ����Ӱ������ʧ
    if( !objectp(owner) ) {
    	do_destruct();
        return;
    }
    
    // ӵ���ߵĲ��������
    if( owner->query_stat("mp") < this_object()->query_level() ) {
    	do_destruct();
        return;
    }

    // ӵ�������mһЩ�����
    owner->damage_stat("mp", this_object()->query_level() , owner);

    // ӵ�������Ӽ��ܾ���
    if( this_object()->query("skill") )
        owner->improve_skill(this_object()->query("skill"), 1);

    return;
}

void die()
{
    message_vision("��������������һ����$N����ʧ������...\n", this_object());
    return;
}

// �Ƿ��������
int accept_order(object who, string cmd)
{
    return 0;  // �����ǲ��ܿ��Ƶ�
}

void create()
{
    set_name("Ӱ����", ({ "shadow" }));
    set_race("human");

    // ƽʱ���������owner���ڲ��ڣ�����ȡ������
    set("chat_chance", 3);
    set("chat_msg", ({
        (: do_check :),
    }));

    // ս��ʱ���������owner���ڲ��ڣ�����ȡ������
    set("chat_chance_combat", 5);
    set("chat_msg_combat", ({
        (: do_check :),
    }) );

    setup();
}