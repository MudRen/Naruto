/*  chpass.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#include <login.h>
#include <command.h>

inherit F_CLEAN_UP;

int
main(object me, string arg)
{
    object ob;
    string userid, new_passwd;

#ifndef SAVE_USER
    return notify_fail("ϵͳĿǰ������ʹ�������ϡ�\n");
#endif

    SECURED_ADMIN_COMMAND;

    if( previous_object() != this_player(1) ) {
        log_file("static/CHPASS", sprintf("[%s] %s call_other chpass command\n",
            ctime(time()), file_name(previous_object())));
        return notify_fail("���ָ��ܱ���Ӻ��С�\n");
    }

    // Write every attempt to this command.
    log_file("static/CHPASS",
        sprintf("[%s] %s %s\n", ctime(time()), geteuid(this_player(1)), arg));

    if( sscanf(arg, "%s %s", userid, new_passwd) != 2 )
        return notify_fail("ָ���ʽ��chpass <ʹ���ߴ���> <����>\n");

    if( wizhood(userid)=="(admin)" )
        return notify_fail("�����޸�ϵͳ�����ߵ����롣\n");

    if( find_player(userid) )
        return notify_fail("�����޸������е�ʹ�������롣\n");

    seteuid(getuid());
    ob = new(LOGIN_OB);
    ob->set("id", userid);
    if( file_size(ob->query_save_file()) < 0 )
        return notify_fail("û�����ʹ���ߡ�\n");

    if( ! ob->restore() )
        return notify_fail("ʹ�������϶�ȡʧ�ܡ�\n");

    if(    ob->set("password", crypt(new_passwd, 0))
    &&    ob->save() )
        write("ʹ���� " + userid + " �������ɹ�\��\n");
    else
        write("ʹ�������ϴ���ʧ�ܡ�\n");

    return 1;
}

int help(object me)
{
    write(@HELP
ָ���ʽ : chpass <ʹ���ߴ���> <����>
 
���ָ����ϵͳ������ר�õģ������޸ĳ��˹��������⣬�����˵����롣���г���
ִ�����ָ��Ķ��������۳ɹ���ʧ�ܣ����ᱻ log ������
 
HELP
    );
    return 1;
}
 
