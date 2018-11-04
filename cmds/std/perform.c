// cast.c

#include <skill.h>

inherit F_CLEAN_UP;

int main(object me, string arg)
{
    object weapon;
    string martial_skill, act, trg;
    object target;

    if( me->is_busy() ) return notify_fail("������û�пթu\n");

    if( !arg ) return notify_fail("ָ���ʽ�sperform <����.����> on <Ŀ��>\n");
    
    if( sscanf( arg, "%s.%s on %s", martial_skill, act, trg) != 3 &&
        sscanf( arg, "%s.%s", martial_skill, act) != 2 )
        return notify_fail("ָ���ʽ�sperform <����.����> [on <Ŀ��>]\n");

    notify_fail("�������� enable ָ��ѡ����Ҫʹ�õļ��ܡ�\n");

    if( (martial_skill != me->skill_mapped( martial_skill ))
    &&	(me->skill_mapped(martial_skill) != 0))
        return SKILL_D(me->skill_mapped(martial_skill))->perform_action(me, act, target);

    return 0;

}

int help (object me)
{
    write(@HELP
ָ���ʽ�sperform <����.����> [on <ʩ�ж���>]

��ս����ʹ����ѧ���ܵľ�������ʩ�ж����ս�����п��Բο�
����ļ������ṩ��HELP��
HELP);
    return 1;
}
