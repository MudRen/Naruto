/*  okip.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#pragma optimize
#pragma save_binary

#include <command.h>
#include <login.h>

//inherit F_CLEAN_UP;

void create()
{
    seteuid(getuid());
    set_reset(this_object(), 86400);
}


int main(object me, string str)
{
    object link_ob;
    string *args, *okip, ip;

    SECURED_COMMAND;

    if( stringp(str) ) args = explode(str, " ") - ({ "" });
    else args = ({});

    if( wizhood(me)=="(admin)" && sizeof(args) > 0 && args[0][0] != '-' ) {
	link_ob = FINGER_D->acquire_login_ob(args[0]);
	args = args[1..];
    }
    else link_ob = me->link();

    if( ! link_ob ) return notify_fail("�Ҳ������������\n");

    if( stringp(link_ob->query("okip")) )
	okip = explode(link_ob->query("okip"), ":");
    else
	okip = ({});

    if( ! sizeof(args) ) {
	write( sprintf( "%s �� okip Ŀǰ�趨Ϊ��\n\t%s\n",
		link_ob->query("id"),
		implode(okip, "\t\n")) );
	return 1;
    }

    switch(args[0])
    {
    case "-a":
	foreach(ip in args[1..])
	    if( member_array(ip, okip)== -1 ) okip += ({ ip });
	break;
    case "-d":
	okip -= args[1..];
	break;
    default:
	return notify_fail("ָ���ʽ��okip [-a|-d] ����λ�� ...\n");
    }

#ifdef	SAVE_USER
    link_ob->set("okip", implode(okip, ":"));
    link_ob->save();
    if( ! link_ob->body() ) destruct(link_ob);
#endif

    write("Ok��\n");
    return 1;
}

int help()
{
        write(@TEXT
ָ���ʽ��okip [-a|-d ] [IP:IP:...]

���ָ������趨�����Դλַ, �����趨ʮ�鳣�õ�λַ, ����Դλ
ַ����ʱ, ϵͳ���ܾ�������Ҫ��, �Է�ֹ��������Щλַ��Ĳ�����ʿ
�����롣�趨λַʱ��С���趨, ����������ɲ������߽���ʱ, �뱸
����������mail��arch��������ʦ����:
1. ��charƽ�յ�����IP (�ǵõĻ�, ��ð�okip���ʲôҲд��ȥ)
2. ��char������ (��������, ������, ���ܵ�)
3. ��char������

����
okip -a 111.222.333.444 ���������� 111.222.333.444 Ϊ����Դ·��
okip -d 111.222.333.444 ����ɾ���� 111.222.333.444 Ϊ����Դ·��

λַ֮�趨֧Ԯ��*����δ֪����, ��һ��IP��, *ֻ����һ��, ��
okip -a 140.113*	��ʾIP��ͷֻҪ��140.113��λַ��������
okip -a *.hinet.net 	��ʾIPֻҪ��������λַ��hinet.net�ľ�
			������
TEXT
        );
        return 1;
}
