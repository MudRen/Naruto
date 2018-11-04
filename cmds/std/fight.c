/*  fight.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <ansi.h>
#include <char.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object obj, old_target;
    int rg, rk, rgm, rkm;

    if( environment(me)->query("no_fight") )
        return notify_fail("�����ֹս����\n");
 
    if( !arg
    ||	!objectp(obj = present(arg, environment(me)))
    ||	!obj->is_character()
    ||	obj->is_corpse() )
	return notify_fail("���빥��˭�t\n");

    if( obj->query("no_fight") ) return notify_fail("��������ﲻ���� Fight ָ�\n");

    if( !me->visible(obj) )
	return notify_fail( obj->name() + "�������㣬��֪������������ս��\n");

    if( me->is_apprentice_of(obj) )
	return notify_fail( obj->name() + "�����ʦ���u\n");

    if( obj->is_fighting(me) )
	return notify_fail("���ͩu���ͩu���ͩu\n");

    if( !living(obj) && inherits(F_ATTACK, obj) )
	return notify_fail( "��" + obj->name() + "ս����\n"); 

    if( obj==me ) return notify_fail("�㲻�ܹ����Լ���\n");

    if( userp(obj) && (object)obj->query_temp("pending/fight")!=me ) {
	message_vision("\n$N����$n˵���s" 
	    + me->rank("self") + me->name() + "�������λ"
	    + obj->rank("respectful") + "�ĸ��Щu\n\n", me, obj);

	if( objectp(old_target = me->query_temp("pending/fight"))
	&&  obj != old_target
	&&  environment(old_target)==environment(me) )
	    message_vision("\n$N��$n��ȭ��Ҿ��˵����"
		+ me->rank("self") + "����������̡�\n", me, old_target);
	me->set_temp("pending/fight", obj);

	tell_object(obj, YEL "�����Ը��ͶԷ����б��ԣ�����Ҳ��" + me->name() + "("+(string)me->query("id")+")"+ "��һ�� fight ָ�\n" NOR);
	write(YEL "���ڶԷ�������ҿ��Ƶ���������ȶԷ�ͬ����ܽ��б��ԡ�\n" NOR);
	return 1;
    }

    if( obj->query("civilized") ) {
	if( me->is_fighting() )
	    message_vision( "\n$N����$n˵����һ�������λ"
		+ obj->rank("respectful") + "�ĸ��Щu\n\n", me, obj);
	else
	    message_vision( "\n$N����$n˵���s" 
		+ me->rank("self") + me->name() + "�������λ"
		+ obj->rank("respectful") + "�ĸ��Щu\n\n", me, obj);

	if( obj->query_temp("pending/fight")==me ) {
	    obj->start_busy(1);
//	delete else, ��˫����busy��������fightָ�����busy only -dragoon
//	else 
	    me->start_busy(1);
	}

	if( !userp(obj) && !obj->accept_fight(me) ) {
	    write("������" + obj->name() + "��������������\n");
	    return 1;
	}

	me->fight_ob(obj);
	obj->fight_ob(me);
    } else {
	message_vision("\n$N���һ������ʼ��$n���������u\n\n", me, obj);
	me->fight_ob(obj);
	obj->kill_ob(me);
    }

    return 1;
}

int help(object me)
{
    write(@HELP
ָ���ʽ : fight <����>
 
���ָ��������һ������ֽ̡������ǡ��д����ա��o������ʽ��ս��������
�㵽Ϊֹ�o���ֻ�����������o����������˩o���ǲ��������е�  NPC ��ϲ��
��ܩo��������״����ı���Ҫ��ᱻ�ܾ���
 
�������ָ��: kill

PS. ����Է���Ը����������ս�o����Ȼ���������� kill ָ�ʼս����
HELP);
    return 1;
}
 
