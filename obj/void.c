/*  void.c

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

inherit F_DBASE;

string out_path()
{
    return START_ROOM;
}

private void create()
{
    set("short", "����");
    set("long", "������һ��������翵ĵط�...\n");
    set("exits/out", (: out_path :) );
    set("no_fight", 1);
    set("void", 1);
}


int do_look(object me, string arg)
{
    printf( "%s - %s\n    %s", query("short"), wizardp(me)? file_name(this_object()) : "", query("long") );
    return 1;
}

int receive_object(object ob)
{
    return userp(ob);
}
