/*  gain.c

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
    string term, txt;
    int g;
    mapping score_g, skill_g;

    score_g = me->query_score_gain();
    me->reset_score_gain();
    skill_g = me->query_skill_gain();
    me->reset_skill_gain();

    txt = "�����ϴ�ʹ�� gain ָ�������Ϊֹ���㹲��ã�\n\n";
    
    txt += "���������\n";
    if( !sizeof(score_g) )
	txt += "    �ޡ�\n";
    else {
	foreach(term, g in score_g)
	    txt += sprintf("    %-16s %+d ��\n",
//		to_chinese("score of " + term), score_g[term] );
		to_chinese(term), score_g[term] );
    }

    txt += "\n���ܵ�����\n";
    if( !sizeof(skill_g) )
	txt += "    �ޡ�\n";
    else {
	foreach(term, g in skill_g)
	    txt += sprintf("    %-16s %+d ��\n",
		to_chinese(term), skill_g[term] );
    }

    write(txt);
    return 1;    
}

int help()
{
    write(@TEXT
ָ���ʽ��gain

���ָ��������������������ɳ���ÿ������������κξ���������ܵ�����ϵ
ͳ���¼����õ�ֵ������ʹ�� gain ָ�����ʾ��Щֵ��Ȼ�������Щ��¼��ͷ��
ʼ���������ս����ʼǰ���� gain �����Ȼ����ս���������� gain ������������
�ոյ�ս���л�Ķ��ٽ�����
TEXT
    );
    return 1;
}
