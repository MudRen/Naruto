/*  dest.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <command.h>

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    string option, target, msg;
    object obj;

    SECURED_WIZARD_COMMAND;

    seteuid(geteuid(this_player(1)));

    if (!arg) return notify_fail("ָ���ʽ : dest <���֮���ƻ���>\n" );

    if( sscanf(arg, "-%s %s", option, target)!=2 ) target = arg;

    if( option == "r" ) {
	if( wizhood(me)=="(admin)" ) seteuid(ROOT_UID);
	else return notify_fail("��û�����Ȩ�ޡ�\n");
    }

    obj = find_object(target);
    if (!obj) obj = present(target, me);
    if (!obj) obj = present(target, environment(me));
    if (!obj) obj = find_object( resolve_path(me->query("cwd"), target) );
    if (!obj) return notify_fail("û���������....��\n");

    if( environment(me)==environment(obj) )
    if( !stringp(msg = me->query("env/msg_dest")) )
        msg = "$N�ٻ���һ���ڶ��o��$n��û�ˡ�\n";
    else
        message_vision(msg + "\n", me, obj);

    destruct(obj);
    if(obj) write("���޷���������ǿ�ƴݻ١�\n");
    else write("Ok.\n");

    return 1;
}

int help(object me)
{
    write(@HELP
ָ���ʽ : dest [-r] <���֮���ƻ���>

���ô�һָ��ɽ�һ�����(object)���������(class)�Ӽ�����������o�������
������(���sָ�������������)����һ�βο�����������ʱ������½������롣

���� (admin) ��ֵ���ʦ������ -r ѡ���� ROOT_UID ���������������������
ʹ���ߡ�

������ڱ���������з����˴���o��ʹ��������������Ȼ�ᱻǿ�����( ʹ����
���ֻ����ʹ�� ROOT_UID ���ʱ����ǿ����� )�o�������ѶϢ�ᱻд��
/log/destruct �С�
  
�ο����ϩs destruct()
HELP
    );
    return 1;
}
