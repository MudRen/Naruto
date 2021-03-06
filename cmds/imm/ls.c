/*  ls.c

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

int
main(object me, string arg)
{
    int i, w, col;
    string dir, listing;
    mixed *file, *dirent;
    object ob;

    SECURED_COMMAND;
    seteuid(geteuid(this_player(1)));

    dir = resolve_path(me->query("cwd"), arg);
    if( file_size(dir)==-2 && dir[<1] != '/' ) dir += "/";

    file = get_dir(dir, -1);
    if( !sizeof(file) ) {
	if (file_size(dir) == -2) write("目录是空的。\n");
	else write("没有这个目录。\n");
	return 1;
    }

    i = sizeof(file);
    w = 0;
    while(i--) {
	/* 如果列出的是目录，加上一个 '/' */
	if( file[i][1]==-2 ) file[i][0] += "/";
	/* 找出最长的档名长度 */
	if( strlen(file[i][0]) > w ) w = strlen(file[i][0]) + 1;
    }

    listing = sprintf("目录：%s\n", dir );
    col = 70 / (w+6);
    i = 0;
    foreach( dirent in file ) {
	ob = find_object( dir + dirent[0] );
	listing += sprintf("%4d ", 1 + dirent[1]/1024);
	if( ob ) listing += HIG;
	else if( dirent[1]==-2 ) listing += HIB;
	listing += sprintf("%-*s" NOR, 1+w, dirent[0]);
	listing += (++i%col) ? "  " : "\n";
    }

    if( listing[<1] != '\n' ) listing += "\n";
    me->start_more( listing );

    return 1;    
}

int
help(object me)
{
    write(@HELP
指令格式: ls [<路径名>]
 
列出目录下所有的子目录及档案, 如果没有指定目录, 则列出所在目录
的内容﹐所列出的档案中前面标示 * 号的是已经载入的物件。
 
范例:
'ls /' 会列出所有位于根目录下的档案及子目录.
 
HELP
    );
    return 1;
}
