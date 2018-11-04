/*  team.c

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

int
main(object me, string arg)
{
    object *t;

    if( !arg ) {
	if( !pointerp(t = me->query_team()) )
	    return notify_fail("�����ڲ�û�вμ��κζ��顣\n");
	write("�����ڶ����еĳ�Ա�Щs\n  " + implode(t->short(1), "\n  ") + "\n");
	return 1;
    }

    if( arg=="dismiss" ) {
        if( !pointerp(t = me->query_team()) )
            return notify_fail("�����ڲ�û�вμ��κζ��顣\n");
        if( me->is_team_leader() ) {
            message("team", me->name(1) + "�������ɢ�ˡ�\n", t, me);
            write("�㽫�����ɢ�ˡ�\n");
        } else {
            message("team", me->name(1) + "����������顣\n", t, me);
            write("����������Ķ��顣\n");
        }
        me->dismiss_team();
        return 1;
    }

    if( sscanf(arg, "with %s", arg)==1 ) {
        object ob;

        if( !(ob = present(arg, environment(me)))
        ||    !living(ob)
        ||    !ob->is_character()
        ||    ob==me )
            return notify_fail("�����˭��Ϊ���t\n");

        if( !pointerp(me->query_team()) || me->is_team_leader() ) {
            if( me==(object)ob->query_temp("pending/team") ) {
                if( !pointerp(me->query_team()) ) {
                    ob->add_team_member(me);
                    message_vision("$N��������$n�Ķ��顣\n", me, ob);
                } else {
                    me->add_team_member(ob);
                    message_vision("$N������$n������顣\n", me, ob);
                }
                ob->dellete_temp("pending/team");
                return 1;
            } else {
                message_vision("$N����$n����$P�Ķ��顣\n", me, ob);
                tell_object(ob, YEL "�����Ը�����o���� team with " + me->query("id") + "��\n" NOR);
                me->set_temp("pending/team", ob);
                return 1;
            }
        } else
            return notify_fail("ֻ�ж����������������˼��롣\n");
    }

    if( sscanf(arg, "talk %s", arg)==1 ) {
        if( !pointerp(t=me->query_team()) )
            return notify_fail("�����ڲ�û�кͱ�����ɶ��顣\n");
        message("team", HIW "�����顿" + me->name(1) + "�s" + arg + "\n" NOR, t);
        return 1;
    }

    if( sscanf(arg, "form %s", arg)==1 ) {
        if( !pointerp(t=me->query_team()) || !me->is_team_leader() )
            return notify_fail("�������һ����������������֯���Ρ�\n");
        if( !me->query_skill(arg, 1) )
            return notify_fail("����������ûѧ����\n");
        return SKILL_D(arg)->form_array(me);
    }
}

int help(object me)
{
    write(@HELP
����ָ��ʹ�÷���:

team with <ĳ��> - ��ĳ����ɶ���. ����Ҫ˫����ͬ�����Ż���Ч��
team dismiss     - �뿪����. ���´�ָ��������, ������������ɢ��
team talk <ѶϢ> - ����������������̸��, ������ tt <ѶϢ> �����
team form <��> - �����������ѧ���󷨵Ļ��o���Խ������еĳ�Ա��֯��
                   ĳ�����Ωo������ε�״ֻ̬Ҫ����һ�ƶ��ͻ�����

ֱ���� team ָ��ʱ�����ʾ��Ŀǰ�Ƿ��м�����鼰��Ա����, ��һ�������䡣
 quit ʱ���Զ��������, ���� quit ʱ�����������ɢ��

ע: team �� follow �Ƕ����ĩo�㲻һ��Ҫ follow ��������䡣
HELP
    );
    return 1;
}
