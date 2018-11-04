/*  kill.c

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

inherit F_CLEAN_UP;

void do_kill(object, object);

int main(object me, string arg)
{
    object obj;
    string *killer, callname;

    if( !arg ) return notify_fail("����ɱ˭�t\n");

    if( !(obj = present(arg, environment(me)))
    ||	!obj->is_character()
    ||	obj->is_corpse() )
	return notify_fail("����û������ˡ�\n");

    if( userp(obj) ) return notify_fail("�㲻�ܶ������ Kill ָ�\n");

    if( environment(me)->query("no_fight") ) {
	    if( !obj->query("unprotect_mark") ) {
	        tell_object(me, "���ﲻ׼ս����\n");
	        return 1;
	    }
    }

    if( obj->query("no_fight") ) return notify_fail("��������ﲻ���� Kill ָ�\n");

    if( me->is_busy() ) return notify_fail("�㱻���˹�����æ���� ...\n");

    if( obj==me ) return notify_fail("����ô������\n");

    me->set_temp("pending/hidden", 0);      // be visible
        
    if( ! me->is_fighting(obj)
    ||	! me->is_killing(obj) ) {
	switch( random(3) )
	{
	case 0:
	    message_vision("\n$N����$n�ȵ��s��" + obj->rank("rude")
		+ "�u���ղ������������һ�u��\n\n", me, obj);
	    break;
	case 1:
	    message_vision("\n$N����$n�ȵ��s��" + obj->rank("rude")
		+ "������������\n\n", me, obj);
	    break;
	case 2:
	    message_vision("\n$N����$n�ȵ��s��" + obj->rank("rude")
		+ "������Ľ��������ļ��գ���\n\n", me, obj);
	    break;
	}
    }

    do_kill(me, obj);

    return 1;
}

void do_kill(object me, object obj)
{
    me->kill_ob(obj);

    if( obj->is_fighting(me) && obj->is_killing(me) ) return;

    if( userp(obj) )
    {
	/* ����ҵ� kill ָ�� */
	obj->fight_ob(me);
	tell_object(obj, HIR "�����Ҫ��" + me->name() 
	    + "�����ಫ������Ҳ���������һ�� kill ָ�\n" NOR);
    }
    else {
	/* �� NPC �� kill ָ�� */
	if( function_exists("accept_kill", obj)
	&&  ! obj->is_killing(me)
	&&  obj->accept_kill(me) ) {
	    return; }

	if( ! obj->is_killing(me) ) obj->kill_ob(me);
	else if( ! obj->is_fighting(me) ) obj->fight_ob(me);
    }
}

int help(object me)
{
  write(@HELP
ָ���ʽ : kill <����>
 
���ָ������������ʼ����һ������o���ҳ���ɱ���Է��okill �� fight ����
��ͬ����˫�������浶ʵǹ�ش򶷩oҲ����˵�o��������ˡ����� kill ֻ�赥��
��һ����Ը�Ϳ��Գ����o�������κ���ʹ�� kill ָ��Ὺʼս���oͨ�����
�Է��� NPC �Ļ��o����Ҳ��ͬ������ʹ�� kill��

�������ָ��: fight
HELP);
    return 1;
}
 
