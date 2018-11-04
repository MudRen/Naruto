/* look.c write by -Acme-
*/

#pragma save_binary
#include <ansi.h>

inherit F_CLEAN_UP;

mapping named_apply = ([
    "head_eq": HIK"["NOR"ͷ��"HIK"]"NOR,
    "face_eq": HIK"["NOR"����"HIK"]"NOR,
    "neck_eq": HIK"["NOR"����"HIK"]"NOR,
    "wrist_eq": HIK"["NOR"����"HIK"]"NOR,
    "hand_eq": HIK"["NOR"�ֲ�"HIK"]"NOR,
    "finger_eq": HIK"["NOR"��ָ"HIK"]"NOR,
    "cloth": HIK"["NOR"�·�"HIK"]"NOR,
    "armor": HIK"["NOR"����"HIK"]"NOR,
    "waist_eq": HIK"["NOR"����"HIK"]"NOR,
    "pants": HIK"["NOR"����"HIK"]"NOR,
    "leg_eq": HIK"["NOR"�Ȳ�"HIK"]"NOR,
    "feet_eq": HIK"["NOR"�㲿"HIK"]"NOR,
    "lefthand": HIK"["NOR"����"HIK"]"NOR,
    "righthand": HIK"["NOR"����"HIK"]"NOR,
    "twohanded": HIK"["NOR"˫��"HIK"]"NOR,
]);

string status_msg(string who, int ratio)
{
    switch( ratio ) {
		case    100: return HIG + who + "��������û�����ˡ�\n" NOR; break;
		case 96..99: return HIG + who + "�ƺ����˵����ˣ��������������������\n" NOR; break;
		case 91..95: return HIG + who + "�������������˵����ˡ�\n" NOR; break;
		case 81..90: return HIG + who + "���˼����ˣ������ƺ��������¡�\n" NOR; break;
		case 61..80: return HIY + who + "���˲��ᣬ������״������̫�á�\n" NOR; break;
		case 41..60: return HIY + who + "��Ϣ���أ�������ʼɢ�ң��������ܵ�����ʵ���ᡣ\n" NOR; break;
		case 31..40: return HIY + who + "�Ѿ��˺����ۣ���������֧����������ȥ��\n" NOR; break;
		case 21..30: return HIR + who + "�����൱�ص��ˣ�ֻ�»�������Σ�ա�\n" NOR; break;
		case 11..20: return HIR + who + "����֮���Ѿ�����֧�ţ��ۿ���Ҫ���ڵ��ϡ�\n" NOR; break;
		case  5..10: return HIR + who + "���˹��أ��Ѿ�����һϢ�����ڵ�Ϧ�ˡ�\n" NOR; break;
		default:     return HIR + who + "���˹��أ��Ѿ�������в�����ʱ�����ܶ�����\n" NOR; break;
    }    
}

string inventory_look(object obj, int flag)
{
    string str, type, sub_type;

    str = obj->short();
    if( type = obj->query("equipped") ) {
        if( sscanf(type, "%s/%s", type, sub_type) == 2 ) {
            str = sprintf("  %s %s",
            	          !undefinedp(named_apply[sub_type]) ? named_apply[sub_type] : sub_type,
            	          str );
        } else str = HIY "  ��"NOR + str;
    } else if( !flag ) str = "    " + str;
    else return 0;

    return str;
}

int look_living(object me, object obj)
{
    int age;
    string msg, race, pro;
    mixed *inv;

    race = obj->query_race();
    pro = (obj==me) ? gender_self(obj->query("gender")) : gender_pronoun(obj->query("gender"));
    age = obj->query("age");

    if( me != obj ) message("vision", me->name() + "�������㿴�o��֪����Щʲô���⡣\n", obj);

    msg = sprintf("%s������%s�ꡣ\n", pro, age > 10 ? "Լ" + chinese_number(age/10*10) + "��" : "����ʮ" );
    msg += obj->long();
    if( obj->query_stat_maximum("hp") )
        msg += status_msg(pro, obj->query_stat_current("hp")*100/obj->query_stat_maximum("hp"));

    inv = all_inventory(obj);
    if( sizeof(inv) ) {
        inv = map_array(inv, "inventory_look", this_object(), obj->is_corpse() ? 0 : 1 );
        inv -= ({ 0 });
        if( sizeof(inv) )
            msg += sprintf( obj->is_corpse() ? "%s�������Щs\n%s\n" : "%s���ϴ����s\n%s\n", pro, implode(inv, "\n") );
    }

    message("vision", msg, me);

    return 1;
}

int look_item(object me, object obj)
{
    mixed *inv;

    me->start_more(obj->long());
    inv = all_inventory(obj);
    if( sizeof(inv) ) {
        if( sizeof(inv) < 30 ) {
            inv = map_array(inv, "inventory_look", this_object() );
            message("vision", sprintf("�����Щs\n  %s\n", implode(inv, "\n  ") ), me);
        }
        else tell_object(me, YEL"���������Ӱ˵ķ�����ණ��..\n"NOR);
    }
    return 1;
}

int main(object me, string arg)
{
    object obj;

    if( !environment(me) )
        return notify_fail("������ʲôҲ��������\n");

    if( me->query_temp("block_msg/vision") )
        return notify_fail("������ʲôҲ��������\n");

    if( !arg ) return environment(me)->do_look(me, arg);
    
    if( (obj = present(arg, me)) || (obj = present(arg, environment(me)))) {
        if( obj->is_character() ) return look_living(me, obj);
            else return look_item(me, obj);
    } else return environment(me)->do_look(me, arg);
    
    return 1;
}

int help (object me)
{
    write(@HELP
ָ���ʽ: look [<��Ʒ>|<����>]
 
���ָ������鿴�����ڵĻ�����ĳ����Ʒ�����
 
HELP);
    return 1;
}