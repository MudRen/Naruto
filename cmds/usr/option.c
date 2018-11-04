/*  option.h

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
    string term;
    mixed v;

    if( !arg )
    {
        mapping opt = me->query("option");
        string str = "��Ŀǰ�趨��ʹ����ѡ�\n";

        if( !mapp(opt) || !sizeof(opt) )
            str += "    �ޡ�\n";
        else
            foreach(term, v in opt)
                str += sprintf("    %-20s��%O\n", term, v);
        write(str);
        return 1;
    }

    if( sscanf(arg, "%s %d", term, v)==2 || sscanf(arg, "%s %s", term, v)==2 ) {
        if( !v ) me->delete("option/" + term);
        else me->set("option/" + term, v);
        if( term == "map_unfreeze" ) {
            if( environment(me)->is_area() ) {
                if( !v ) {
                    me->set_temp("window/freeze", 1);
                    write(CLR+R(14, me->query_temp("window/height")));
                } else {
                    me->delete_temp("window/freeze");
                    write(SAVEC+R("","")+RESTC);
                }
            } else {
                if( v ) {
                    me->delete_temp("window/freeze");
                    write(SAVEC+R("","")+RESTC);
                }
            }
        }
    } else return notify_fail("ָ���ʽ��option <ѡ��> <�趨ֵ>\n");

    write("Ok.\n");
    return 1;
}

int help()
{
    write(@TEXT
ָ���ʽ��option <ѡ��> <�趨ֵ>

���ָ�������趨һЩ�й�ʹ���߽����ѡ������趨ֵ 0 ��ΪϵͳԤ��ֵ��

ѡ�                ˵����                                  �趨ֵ��
map_unfreeze          ��ͼϵͳ��ȡ�������ͼ�������Ӵ��Ϸ�    (0:��, 1:��)
map_hidden            ��ͼϵͳ�Ƿ����ص�ͼ����                (0:��, 1:��)
map_obj_hidden        ��ͼϵͳ���Ƿ����ػ�����Ʒ              (0:��, 1:��)
map_obj_icon_hidden   ��ͼϵͳ���Ƿ����ؼ�ʱ����Ʒͼʾ        (0:��, 1:��)
map_block             ��ͼϵͳ���Ƿ�ͼʾ���ϰ���������      (0:��, 1:��)
map_through           ��ͼϵͳ���Ƿ����ô�ǽģʽ(��ʦר��)    (0:��, 1:��)
brief_room            �Ƿ�򻯷������ϸ����                  (0:��, 1:��)
brief_short           �Ƿ�ֻ��ʾ��Ʒ���������������          (0:��, 1:��)
TEXT);
    return 1;
}
