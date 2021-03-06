/*  visitior.c

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

int main(object me, string arg)
{
    string s;
    int cnt, t;

    s = read_file("/adm/etc/visitor.cnt");
    if( !s ) return 0;
    sscanf(s, "%d %d", t, cnt);
    printf("从 %s 以来共有 %d 使用者人次。\n", ctime(t), cnt);
    return 1;
}

int help()
{
    write(@TEXT
指令格式：visitor

这个指令会显示系统目前统计的使用者人次资料。
TEXT
    );
    return 1;
}
