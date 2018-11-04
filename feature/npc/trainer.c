/* trainer.c
   ����ѵ����Ҽ��ܵ�npc
*/

#include <skill.h>
#include <ansi.h>

mapping train = ([]);

/* guild: muye   ľҶ���ߴ�
	      yinyin �������ߴ�
	      wuyin  �������ߴ�
	      shayin ɰ�����ߴ�
	      yuyin  �������ߴ�
	      yunyin �������ߴ�
	      caoyin �������ߴ�
	      puyin  �������ߴ�
*/

int is_trainer() { return 1; }

mapping query_train() { return train; }

string skill_class(string skill)
{
    switch( SKILL_D(skill)->query_class() ) {
        case "basic": return BGRN"����"NOR; break;
        case "force": return BRED"����"NOR; break;
        case "ninja": return BBLU"����"NOR; break;
        case "magic": return BCYN"����"NOR; break;
        case "seal":  return BWHT+BLK"��ӡ"NOR; break;
        default: return BYEL"����"NOR; break;
    }
}

// �趨��ѵ���ļ���
void set_train(string skill, int learn, int require, int upperbound)
{
    train[skill] = ([]);
    train[skill]["learn"] = learn;                // �����û�иü���ʱ���Ƿ��ֱ��ϰ�ô˼���lv1
    	                                          // ��lv�ļ��ܲſ���Ͷ�����������
    	                                          // ���������⼼��Ҫ���յõ�lv�󷽿�ѵ����
    train[skill]["require"] = require;			  // ѧϰ�˼�����Ҫ�ĵȼ�
    train[skill]["upperbound"] = upperbound;      // �˼��ܿ�ѧϰ��������
}

string do_list()
{
	string msg, sk, *sks;

    if( this_object()->query("guild") != "all" ) {
        if( this_object()->query("guild") != this_player()->query("guild") )
            return "�㲻�ܹۿ�" + this_object()->query("name") + "��ѵ�������б�\n";
    }

	msg = "  " + this_object()->query("name") + "�ṩѵ���ļ����б�\n";
	msg += "�q�������������������������������������������������������������������������r\n";
	msg += "�����  ϰ�ô˼�  ����ȼ�  ����                                  ѵ�����ީ�\n";
    msg += "����������������������������������������������������������������������������\n";

    sks = sort_array( keys(train) , (: train[$1]["require"] - train[$2]["require"] :));

//    foreach( sk in keys(train) ) {
    foreach( sk in sks ) {
    	msg += sprintf("��%s  %|8s  %|8d  %-37s %|8d��\n",
    		           skill_class(sk),
    		           train[sk]["learn"] ? HIW"   ��   "NOR : HIR"   ��   "NOR,
    		           train[sk]["require"],
    		           to_chinese(sk) + "(" + sk + ")",
    		           train[sk]["upperbound"] );
    }

    msg += "�t�������������������������������������������������������������������������s\n";

    return msg;
}

int train_skill(object who, string sk, int time)
{
	int point, sklv, require, learn;

    if( !objectp(who) ) return 0;

  	if( undefinedp(train[sk]) ) {
  		tell_object(who, this_object()->query("name")+"û���ڰ���ѵ�� " + sk + " ������ܡ�\n");
	    return 1;
   	}

	if( who->query_level() < train[sk]["require"] ) {
		tell_object(who, "��ĵȼ����㣬�޷�ѧϰ������ܡ�\n");
		return 1;
	}

    if( !who->query_skill(sk) ) {
    	if( !train[sk]["learn"] ) {
    		tell_object(who, "����ȫ���� " + sk + " ������ܣ�����" + this_object()->query("name") + "�������������ܡ�\n");
    		return 1;
    	}
    	tell_object(who, "��Ŀǰ����ûѧ��������ܣ���"+ this_object()->query("name") +"�Ľ̵������ס����������ˡ�\n");
        tell_object(who, "(��һ��ѧ��������û�취ѵ���ģ���ǵ�ʹ�� advance ָ��Ͷ���ܵ������ſɽ���ѵ����)\n");
    	who->set_skill(sk, 1);
    	return 1;
    }

    point = who->query_point("learn");
	if( point < time ) {
		tell_object(who, "���ѧϰ�������㡣\n");
		return 1;
	}
		
	sklv = who->query_skill(sk, 1);
	if( sklv >= train[sk]["upperbound"] ) {
		tell_object(who, this_object()->query("name")+"�Ѿ��޷��ٽ̵��� " + to_chinese(sk) + " ������ܡ�\n");
	    return 1;
	}

    learn = who->query_learn(sk);
    require = who->query_skill_require(sk);

    if( require < learn + time*who->query_attr("int") ) {
        time = (require-learn) / who->query_attr("int");
        who->add_point("learn", -time );
        time ++;
    } else who->add_point("learn", -time);

	who->improve_skill(sk, time);

    sklv = who->query_skill(sk, 1);

    if( sklv >= (who->query_skill_ability(sk)*20) ) {
        message_vision("$N��$nѧϰ�й�"+to_chinese(sk)+"�����ƺ�������ƿ����\n", who, this_object() );    
        tell_object(who, "(��ļ���Ͷ�㲻������ѧϰ������ܡ�)\n");
    } else if( who->query_skill_abandon(sk) ) {
        message_vision("$N��$nѧϰ�й�"+to_chinese(sk)+"�����ƺ�û�취�˽⡣\n", who, this_object() );
        tell_object(who, "(���Ѿ�ֹͣѧϰ����ܣ������޷�����κξ��顣)\n");
    } else message_vision("$N��$nѧϰ�й�"+to_chinese(sk)+"���ƺ������ĵõġ�\n", who, this_object() );

	return 1;
}