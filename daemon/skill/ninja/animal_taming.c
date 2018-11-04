/* animal_taming.c ������ѱ��֮�� */
    
#include <ansi.h>

#define SKILL_NAME "animal taming"

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

// ��ʼ������
void setup_dog(object me, object pet)
{
    int lv;
   
    // �趨����
    if( me->query("pet/name") ) pet->set("name", me->query("pet/name"));
    
    // �趨ID
    pet->set("id", me->query_id() + "'s dog");
    
    // �趨title
    if( me->query("pet/title") ) pet->set("title", me->query("pet/title"));
    
    // �趨nickname
    if( me->query("pet/nickname") ) pet->set("nickname", me->query("pet/nickname"));

    // �趨long
    if( me->query("pet/long") ) pet->set("long", me->query("pet/long"));
    
    // �趨food
    if( me->query("pet/food") ) pet->food_gain(me->query("pet/food"));
    
    // �趨Level
    if( me->query("pet/level") ) pet->set_level(me->query("pet/level"));
    
    // �趨exp
    if( me->query("pet/exp") )
        pet->exp_gain(me->query("pet/exp"));

    lv = pet->query_level();
    
    // ���ȼ���������
    pet->set_attr("str", lv*2);
    pet->set_attr("con", lv*2);
    pet->set_attr("dex", lv*2);
    pet->set_attr("int", lv*2);
    pet->set_stat_maximum("ap", lv*50);
    pet->set_stat_maximum("hp", lv*50);
    pet->set_stat_maximum("mp", lv*50);
    pet->set_stat_current("ap", lv*50);
    pet->set_stat_current("hp", lv*50);
    pet->set_stat_current("mp", lv*50);
    pet->set_skill("combat", lv*3);
    pet->set_skill("parry", lv*3);
    pet->set_skill("dodge", lv*3);
    pet->set_skill("heavy hit", lv*3);
    pet->set_skill("savage hit", lv*3);
    pet->set_skill("continual hit", lv*3);
    
    pet->do_heal();

    // �趨����Ŀǰ״̬
    if( me->query("pet/ap") ) pet->set_stat_current("ap", me->query("pet/ap"));
    if( me->query("pet/hp") ) pet->set_stat_current("hp", me->query("pet/hp"));
    if( me->query("pet/mp") ) pet->set_stat_current("mp", me->query("pet/mp"));

}

// �г���Ȯ��ʽ
void call_dog(object me)
{
    object pet;

    if( objectp( pet = me->query_temp("pet") ) ) {
        write("�����Ȯ�Ѿ�����г�����...\n");
        return;
    }
    
    if( me->query_skill("animal taming", 1) <= 40 ) {
        write("���ѱ�����������40������ʹ�����ָ�\n");
        return;
    }

    // ӵ�������mһЩ�����
    if( me->query_stat("mp") < 20 ) {
        write("��Ŀǰû���㹻�Ĳ������ʹ�÷�������\n");
        return;
    }

    pet = new(__DIR__"obj/pet.c");
    if( !objectp(pet) ) {
        write("���ƺ�û�취���������Ȯ����...\n");
        destruct(pet);
        return;
    }

    message_vision(HIW"$N�ӿ������ͳ�һ֧���Ӵ��˼��£�����¶�������Ц��...\n"NOR, me);

    // ��ʼ������״̬
    setup_dog(me, pet);

    if( environment(me)->is_area() ) {
        if( !area_move(environment(me), pet, me->query("area_info/x_axis"), me->query("area_info/y_axis")) ) {
            write("�����Ȯ��������һ���̵�������...\n");
            destruct(pet);
            return;
        }
    } else {
        if( !pet->move(environment(me)) ) {
            write("�����Ȯ��������һ���̵�������...\n");
            destruct(pet);
            return;
        }
    }

    if( me->query_skill("animal taming", 1) > 50 ) pet->set_leader(me);     //����leader
    if( me->query_skill("animal taming", 1) > 60 ) pet->guard_ob(me);       //����leader
    pet->set_owner(me);      //�趨owner
    pet->set_attack(1);      //���Թ���
    pet->set_evade(1);       //���Իر�

    me->set_temp("pet", pet);
    
    message_vision("$nһ����$N���˵�������ȥ��������������...\n", me, pet);

    me->damage_stat("mp", 20, me);
    me->start_busy(3);

    // ���Ӽ��ܾ���
    me->improve_skill("animal taming", 1);

    return;
}

// ɾ�����ﺯʽ
void destruct_dog(object me)
{
    mapping petr=([]);
    object pet;
    
    if( !objectp(pet = me->query_temp("pet")) ) {
        write("��Ŀǰû�нг������Ȯ����...\n");
        return;
    }
    
    if( pet->is_fighting() || pet->is_busy() ) {
        write("�����Ȯ�ƺ���æЩʲô��һʱ�Ѳ�����...\n");
        return;
    }
    
    // ����״̬����
    petr["name"] = pet->query("name");
    petr["title"] = pet->query("title");
    petr["nickname"] = pet->query("nickname");
    petr["long"] = pet->query("long");
    petr["exp"] = pet->query_exp();
    petr["food"] = pet->query_food();
    petr["level"] = pet->query_level();
    petr["ap"] = pet->query_stat("ap");
    petr["hp"] = pet->query_stat("hp");
    petr["mp"] = pet->query_stat("mp");    
    
    me->set("pet", petr);
    
    pet->do_destruct();
    me->delete_temp("pet");
    
    write("�����Ȯ�뿪�ˡ�\n");
    
    return;
}

string status_color(int current, int max)
{
    int ratio = max ? (current * 100) / max : 100;

    switch( ratio ) {
        case  0..25: return HIR;
        case 26..50: return HIM;
        case 51..75: return HIY;
        case 76..100: return HIG;
        default: return HIC;
    }
}

void show_state(object me)
{
    object pet;
    string msg;
    
    if( !objectp(pet = me->query_temp("pet")) ) {
        write("���Ƚг������Ȯ��\n");
        return;
    }

    if( me->query_skill("animal taming", 1) <= 20 ) {
        write("���ѱ�����������20������ʹ�����ָ�\n");
        return;
    }

    printf("[%s] �ȼ� %d  ����ֵ %2.2f%s  ʳ���� %s%d/%d\n"NOR,
        pet->query("name"),
        pet->query("level"),
        (pet->query_exp()*10.0 / (pet->query("level") * pet->query("level"))),
        "%",
        status_color(pet->query_food(), pet->query_food_maximum()),
        pet->query_food(),
        pet->query_food_maximum() );

    return;
}


void feed_dog(object me, string item)
{
    int stuff, heal;
    object food, pet;
    
    if( !objectp(pet = me->query_temp("pet")) ) {
        write("���Ƚг������Ȯ��\n");
        return;
    }
    
    if( me->query_skill("animal taming", 1) <= 30 ) {
        write("���ѱ�����������20������ʹ�����ָ�\n");
        return;
    }

    food = present(item, me);
    if( !food ) food = present(item, environment(me));
    if( !food ) {
        write("����û���������� ...\n");
        return;
    }

    stuff = food->query("food_stuff");
    if( !stuff || !food->stuff_ob(pet)) {
        write("���������ƺ����ܳ�...\n");
        return;
    }
    // ����ʳ����
    pet->food_gain(stuff);

    message_vision("$N������һ" + ( food->query("base_unit") ? food->query("base_unit") : food->query("unit") ) + "$n��\n", pet, food);
    
    food->add_amount(-1);
    if( food->query_amount() <= 0 ) destruct(food);

    // �Զ��������Ӿ���ֵ
    pet->exp_gain(stuff);
    
    return;
}

void set_data(object me, string key, string value)
{
    object pet;
    
    if( !objectp(pet = me->query_temp("pet")) ) {
        write("���Ƚг������Ȯ��\n");
        return;
    }
    
    if( me->query_skill("animal taming", 1) <= 100 ) {
        write("���ѱ�����������100������ʹ�����ָ�\n");
        return;
    }

    if( key == "name" ) {
        if( strlen(value) > 10 ) {
            write("��ֻ��Ϊ�����Ȯȡ��������ִ�С���ڵ����֡�\n");
            return;
        }
        write("��������Ȯȡ��Ϊ " +value + "��\n");
        pet->set("name", value);
        return;
    }
    
    if( key == "title" ) {
        if( strlen(value) > 14 ) {
            write("��ֻ��Ϊ�����Ȯȡ�߸������ִ�С���ڵ�ͷ�Ρ�\n");
            return;
        }
        write("��������Ȯ��ͷ��ȡΪ " +value + "��\n");
        pet->set("title", value);
        return;
    }

    if( key == "nickname") {
        if( strlen(value) > 14 ) {
            write("��ֻ��Ϊ�����Ȯȡ�߸������ִ�С���ڵ���š�\n");
            return;
        }
        write("��������Ȯ�����ȡΪ " +value + "��\n");
        pet->set("nickname", value);
        return;
    }
    
    if( key == "long") {
        if( strlen(value) > 200 ) {
            write("��ֻ��Ϊ�����Ȯ�趨һ�ٸ������ִ�С���ڵ�������\n");
            return;
        }
        write("��������Ȯ��������Ϊ " +value + "��\n");
        pet->set("long", value);
        return;
    }

    write("��Ŀǰ�ܸĸ���Ȯ����Ѷ�� name, title, nickname, long ...\n");
    return;
}

// ʹ�þ���
int perform_action(object me, string act, object target)
{
    string k, v;
    if( !objectp(me) ) return 0;
    if( me->is_busy() ) return notify_fail("������û�п�ʹ��ѱ��֮����\n");

    switch( act ) {
        case     "call": call_dog(me); break;
        case    "leave": destruct_dog(me); break;
        case    "state": show_state(me); break;
        default:
            if( sscanf(act, "feed %s", k) == 1 ) {
                feed_dog(me, k);
            } else if( sscanf(act, "set %s %s", k, v) == 2 ) {
                set_data(me, k, v);
            } else return notify_fail("Ŀǰѱ��֮���� feed, call, leave, state, set ���ֹ�\�ܡ�\n"); break;
        break;
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