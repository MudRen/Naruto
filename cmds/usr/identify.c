/*  identify.c

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
#include <combat.h>

inherit F_CLEAN_UP;

mapping named_apply = ([
    "str" : "����",
    "con" : "���",
    "int" : "�ǻ�",
    "dex" : "����",
    "attack" : "��������",
    "armor" : "��������",
    "exact"  : "��������",
    "evade"  : "�ر�����",
    "intimidate" : "��������",
    "wittiness" : "��������",
    "move" : "�ƶ�����",
    "carriage" : "��������",
    "ap" : "����ֵ",
    "hp" : "����ֵ",
    "mp" : "�����",
    "head_eq" : "ͷ��",
    "face_eq" : "����",
    "neck_eq" : "����",
    "wrist_eq" : "����",
    "hand_eq" : "�ֲ�",
    "finger_eq" : "��ָ",
    "cloth" : "�·�",
    "armor" : "����",
    "waist_eq" : "����",
    "pants" : "����",
    "leg_eq" : "�Ȳ�",
    "feet_eq" : "�㲿",
]);

int main(object me, string arg)
{
    object ob;
    string type, sub_type;
    mapping dp, apply;

    if( me->is_busy() ) return 0;

    if( !arg ) return notify_fail("��Ҫ����ʲô����?\n");
    ob = present(arg, me);
    if( !ob ) return notify_fail("��Ҫ����ʲô����?\n");
    if( !(type = ob->query("equipped")) )
        return notify_fail("�������װ��" + ob->name() + "���ܼ���.\n");
    if( sscanf(type, "%s/%s", type, sub_type) != 2 )
        return notify_fail("������Ʒ�޷�����.\n");

    write(ob->long() + "\n");

    if( type=="weapon" ) {
        string s;
        mixed sk = ob->query("wield_as");
        
        write("��ʹ�÷�ʽ��\n");
        if( stringp(sk) ) sk = ({ sk });
        foreach(s in sk)
            printf("%s%s (%s)\n", ((s==sub_type) ? HIY"��"NOR : "  "), to_chinese(s), s);

        dp = ob->query("damage/" + sub_type);
        printf("\n�˺����� %d - %d (�˺������� %d%%�������ȼ� %d)\n", 
            dp["lower"], dp["upper"], dp["bonus"], dp["level"] );

        apply = ob->query("apply_" + type + "/" + sub_type);
        if( mapp(apply) ) {
            string prop;
            mixed value;
            
            write("\n�������ԩs\n");
            foreach(prop, value in apply) {
                if( !undefinedp(named_apply[prop]) )
                    prop = named_apply[prop];
                else
                    prop = to_chinese(prop);
                printf("  %-30s %O\n", prop, value);
            }
        }
    }
    if( type=="armor" ) {
        apply = ob->query("apply_" + type + "/" + sub_type);
        printf("���߲�λ��%s\n", !undefinedp(named_apply[sub_type]) ? named_apply[sub_type] : sub_type );
        if( mapp(apply) ) {
            string prop;
            mixed value;
            
            write("�������ԣ�\n");
            foreach(prop, value in apply) {
                if( !undefinedp(named_apply[prop]) )
                    prop = named_apply[prop];
                printf("  %-30s %O\n", to_chinese(prop), value);
            }
        }
    }

    return 1;        
}

int help(object ob)
{
    write(@TEXT
ָ���ʽ: identify <���>

���ָ������������һЩ����װ������Ʒ��
TEXT
    );
    return 1;
}
