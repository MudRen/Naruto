/*  roommaker.c - A room editing tool for quick start.

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#define RMK_VERSION " v1.1"

inherit ITEM;

string *special_props = ({
    "short", "long" });

int save_room_file(string yn, string file, string str);

void create()
{
    set_name("����༭��" + RMK_VERSION, ({ "roommaker", "maker" }) );
    set_weight(100);
    set("unit", "��" );
    set("long", @LONG
����һ�����������١����췿��Ĺ��ߩo����ʱ�䲻���������  lag ����ʦ
����������������򵥵ķ���o���������������Լ����򷿼��Ʒ�ʩo���
��Ҫ̫������������o�����ʹ�����¼���ָ��s

mkroom <���䵵��>

    ���һ�����ա�����o֮������� goto ����������� rset �趨�����
    ���������ԡ�

rset <��������> <��������>

    �趨��Ŀǰ���ڷ�������ԩo�� short�olong �ȡ�

connect <����> <���䵵��>

    ����Ŀǰ���ڵķ�������һ�����ڵ���һ�����䡣

saveroom

    ����Ŀǰ���ڵķ���浵��
LONG
    );
}

int query_autoload() { return 1; }

void init()
{
    seteuid(getuid(environment()));
    if( wizardp(environment()) ) {
	add_action("do_mkroom", "mkroom");
	add_action("do_rset", "rset");
	add_action("do_connect", "connect");
	add_action("do_saveroom", "saveroom");
    }
}

int do_mkroom(string arg)
{
    string file, dir, code;

    if( !arg ) return notify_fail("ָ���ʽ�smkroom <���䵵��> [<����>]\n");
    if( sscanf(arg, "%s %s", file, dir)!=2 )
	file = arg;
    seteuid(geteuid(this_player()));
    file = resolve_path(this_player()->query("cwd"), file) + ".c";
    if( file_size(file)!=-1 )
        return notify_fail("���� " + file + " �Ѿ������ˡ�\n");
    if( !write_file(file, @ROOM_CODE
// This is a room made by roommaker.

inherit ROOM;

void create()
{
    set("short", "�շ���");
    set("long", "����һ��ʲôҲû�еĿշ��䡣\n");
    setup();
    replace_program(ROOM);
}
ROOM_CODE
    ) ) return notify_fail("��û��д�뵵��(" + file +")��Ȩ����\n");
    message_vision("ֻ����$N�ķ���༭�����١���һ���o�������������ֱ����һ�㡣\n", this_player());
    return 1;
}

int do_rset(string arg)
{
    string prop, str, file;
    mixed data;

    if( !arg || sscanf(arg, "%s %s", prop, str)!=2 )
        return notify_fail("ָ���ʽ�srset <��������> <��������>\n");

    if( sscanf(str, "%d", data)!=1 )
        data = str;

    file = file_name(environment(this_player(1))) + ".c";
    if( ! SECURITY_D->valid_write(file, this_player(1), "set") )
	return notify_fail("��û���޸�����������Ե�Ȩ�ޡ�");

    environment(this_player())->set(prop, str);
    write("Ok.\n");
    return 1;
}

int do_connect(string arg)
{
    mapping exits;
    string dir, file;

    if( !arg || sscanf(arg, "%s %s", dir, file)!=2 )
        return notify_fail("ָ���ʽ�sconnect <����> <���䵵��>\n");
    file = resolve_path(this_player()->query("cwd"), file);
    exits = environment(this_player())->query("exits");
    if( mapp(exits) )
        exits[dir] = file;
    else
        environment(this_player())->set("exits", ([ dir: file ]) );
    write("Ok.\n");
    return 1;
}

int do_saveroom(string arg)
{
    object env;
    string str, *name, file;
    mapping prop;
    int i;

    env = environment(this_player());
    file = file_name(env) + ".c";
    str = sprintf("// File: %s\n// Updated by %s (%s)\n",
	file, geteuid(this_player(1)), ctime(time()) );
    str += @ROOM_CODE

inherit ROOM;

void create()
{
ROOM_CODE
    ;
    str += sprintf("    set(\"short\", %O);\n", env->query("short") );
    str += sprintf("    set(\"long\", @LONG\n%sLONG\n    );\n", env->query("long") );
    prop = env->query_entire_dbase();
    name = keys(prop);
    for(i=0; i<sizeof(name); i++) {
        if( member_array(name[i], special_props)!=-1 ) continue;
        str += sprintf("    set(\"%s\", %O);\n", name[i], prop[name[i]] );
    }
    str += "\n\tsetup();\n    replace_program(ROOM);\n}\n";

    // Replace current directory with __DIR__ macro to make relocatable code.
    i = strlen(file)-1;
    while( (i > 0) && (file[i]!='/')) i--;
    if( i>0 ) {
        write("ĿǰĿ¼�s" + file[0..i] + " �� __DIR__ ȡ����\n");
        str = replace_string(str, "\"" + file[0..i], "__DIR__\"");
    }

    // Indent structured constants with tab char
    str = replace_string(str, "\n  ", "\n\t");

    if( file_size(file)!=-1 ) {
        write("���� " + file + " �Ѵ��کoҪɾ���ɵ���t[y/n]");
        input_to("save_room_file", file, str);
        return 1;
    }

    return save_room_file("y", file, str);
}

int save_room_file(string yn, string file, string str)
{
    if( strlen(yn)<1 || yn[0]!='y' ) return 1;

    seteuid(geteuid(this_player(1)));
    write("�浵 " + file + "....");
    if( write_file(file, str, 1) ) {
        write("Ok.\n");
	seteuid(getuid());
        return 1;
    } else {
	write("��û��д���������(" + file + ")��Ȩ����\n");
	seteuid(getuid());
	return 1;
    }
}

void owner_is_killed() { destruct(this_object()); }
