/*  more.c - the more command

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

inherit F_CLEAN_UP;

int help(object me);

int main(object me, string arg)
{
    string file, text;
    object ob;

    seteuid(geteuid(this_player(1)));
    if (!arg)
	return notify_fail("ָ���ʽ : more <����>|<�����> \n");
    file = resolve_path(me->query("cwd"), arg);
    if( file_size(file) < 0 ) {
        ob = present(arg, me);
        if( !ob ) ob = present(arg, environment(me));
        if( !ob ) return notify_fail("û�� "+file+" ���������\n");
        file = base_name(ob) + ".c";
    }
    if( !text = read_file(file) ) text = "";
    write("������" + file + "\n");
    me->start_more(text);
    return 1;
}

int help(object me)
{
write(@HELP
ָ���ʽ : more <����>|<�����>
 
��ҳ��ʾ�������ݡ�
 
more ģʽ�µ�ָ��:
q      : �뿪 more.
b      : ��ʾǰһҳ������.
[ENTER]: ��ʾ��һҳ������.

HELP
    );
    return 1;
}
