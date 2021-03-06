/*  to.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

// inherit F_CLEAN_UP;
// Don't want this command be cleaned up while typing long string.

int main(object me, string arg)
{
    me->edit( (: call_other, __FILE__, "do_to", me, arg :) );
    return 1;
}

void do_to(object me, string arg, string str)
{
    seteuid(getuid());
    if(arg) me->force_me(arg + " " + str);
    else me->force_me(str);
}

int help(object me)
{
write(@HELP
指令格式 : to <指令>

可以一次输出多行以上的文字, <指令>通常是输出讯息的指令如
say , tell , describe , chat , reple ........等。
范例 : 
 to say
 结束离开用 '.'﹐取消输入用 '~q'﹐使用内建列编辑器用 '~e'。
 ─────────────────────────────
 东方神教教主 : 安尼席洛特尔
 教主大夫人暨发呆部部长 : Elon
 教主小老婆暨摸鱼部部长 : 梦忆柔
 大弟子暨打混部部长 : 风火血舞
 .

输出到萤幕上的结果则是 :

你说道﹕东方神教教主 : 安尼席洛特尔
教主大夫人暨发呆部部长 : Elon
教主小老婆暨摸鱼部部长 : 梦忆柔
大弟子暨打混部部长 : 风火血舞
HELP
    );
    return 1;
}

