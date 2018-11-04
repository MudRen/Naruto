/*  suicide.c

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
#include <command.h>

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

int main(object me, string arg)
{
    object link_ob;
    string passwd;

    SECURED_COMMAND;

    if( !(link_ob = me->link()) )
        return notify_fail("���������������˩o�����޷���ɱ��\n");

    if( !arg ) return notify_fail("ָ���ʽ�ssuicide <����>\n");

    passwd = link_ob->query("password");
    if( crypt(arg, passwd) != passwd )
        return notify_fail("�������u\n");

    log_file("static/SUICIDE",
        sprintf("[%s] %s committed suicide from %s\n",
	    ctime(time()), geteuid(me), query_ip_name(me)) );
    CHANNEL_D->do_chnnel(me, "sys", me->name() + "��ɱ�ˡ�\n");

    rm( link_ob->query_save_file() );
    rm( me->query_save_file() );
    
    write("�ðɣ�ϣ�����ܺܿ��� ES2 �п�������\n");
    tell_room(environment(me), me->name() +
        "��ɱ�˩o�Ժ�����Ҳ������������ˡ�\n", ({me}));

    destruct(link_ob);
    destruct(me);

    return 1;
}

int help (object me)
{
    write(@HELP
ָ���ʽ�ssuicide <����>
 
�����Ϊĳ��ԭ���㲻����˩o������������ɱָ����ɾ���������o��ɱ
�Ժ�Ὣ�������浵��ȫ��Ӳ���������o�޷��Ȼ��˩o������ɱǰ�������
˼��

���ָ����������������������в��ʦ�� mud�����ߵĩo��ɱһ�������
��Ҹ���ʦ���ԩo��������������ĩo��Ϊ�����ʵ�����еġ���������ĳ��
�������Ǿ�����ͬ����ĩo���ѡ����ɱ��;��֮ǰ����ȷ�����Ѿ��������
ȷʵ�������ָ��Ĺ��á�
HELP
    );
    return 1;
}
