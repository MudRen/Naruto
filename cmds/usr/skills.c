/* skills.c */

#include <ansi.h>
#include <skill.h>

inherit F_CLEAN_UP;

string *skill_level_desc = ({
    "��ѧէ��",
    "��ͨƤë",
    "��������",
    "������",
    "�������",
    "�������",
    "����似",
    "¯����",
    "�����뻯",
    "�Ƿ��켫",
    "��ɲ�"
});

string *knowledge_level_desc = ({
    "��ѧէ��",
    "�����ž�",
    "��֪һ��",
    "������",
    "����С��",
    "����Ӧ��",
    "׿Ȼ�г�",
    "�������",
    "��������",
    "һ����ʦ",
    "��ɲ�"
});

string skill_level(string, int);

mapping _skill;

string skill_class(string sk)
{
	switch( SKILL_D(sk)->query_class() ) {
		case "basic": return BGRN"����"NOR; break;
		case "force": return BRED"����"NOR; break;
		case "ninja": return BBLU"����"NOR; break;
		case "magic": return BCYN"����"NOR; break;
        case "seal":  return BWHT+BLK"��ӡ"NOR; break;
		default: return BYEL"����"NOR; break;
	}
}

string skill_ability(int i)
{
	switch( i ) {
		case 10: return HIW"[S]"NOR; break;
		
		case  9:
		case  8: return HIY+"["+i+"]"+NOR;break;
		
		case  7:
		case  6: return HIC+"["+i+"]"+NOR;break;
		
		case  5:
		case  4: return HIG+"["+i+"]"+NOR;break;
		
		case  1:
		case  2: return HIK+"["+i+"]"+NOR;break;
		default: return HIK+"["+i+"]"+NOR;break;
	}
}

int main(object me, string arg)
{
    object ob;
    mapping skl, lrn, map, flag, ability, require;
    string *sname, *mapped, s, old_s, line, option;
    int show_all = 0;

    seteuid(getuid());

    if( !arg )  ob = me;
    else {
        ob = present(arg, environment(me));
        if (!ob) ob = find_player(arg);
        if (!ob) ob = find_living(arg);
        if (!ob) return notify_fail("��Ҫ�쿴˭�ļ��ܩt\n");
        if( !wizardp(me)
         && !ob->is_apprentice_of(me)
         && !me->is_apprentice_of(ob)
         && !ob->accept_info(me, "skills") )
                return notify_fail("ֻ����ʦ����ʦͽ��ϵ�����ܲ쿴���˵ļ��ܡ�\n");
    }

    flag = ob->query_skill_flags();
    if( !mapp(flag) ) flag = ([]);

    skl = ob->query_skills();
    if( !mapp(skl) ) skl = ([]);

    lrn = ob->query_learned();
    if( !mapp(lrn) ) lrn = ([]);

    ability = ob->query_skills_ability();
    if( !mapp(ability) ) ability = ([]);

    require = ob->query_skills_require();
    if( !mapp(require) ) require = ([]);

    if(!sizeof(skl) && !sizeof(lrn)) {
        write( (ob==me ? "��" : ob->name()) + "Ŀǰ��û��ѧ���κμ��ܡ�\n");
        return 1;
    }

    line = "�q�������������������������������������������������������������������������r\n";
    line += sprintf("��%-72s��\n", (ob==me ? "��" : ob->name()) + "Ŀǰ��ϰ�õļ����б�");

    // This is a very tricky code that setup a mapping of effective skill
    // level and an array of skill names (>0).
    _skill = ([]);
    sname = filter_array(keys(skl) + keys(lrn), (: undefinedp($3[$1]) ? $3[$1] = $2->query_skill($1) : 0 :), ob, _skill );

    map = ob->query_skill_map();

    if( mapp(map) ) mapped = values(map);
    else mapped = ({});

    sname = sort_array( keys(_skill) , (: _skill[$2] - _skill[$1] :));

    line += "����������������������������������������������������������������������������\n";
    foreach(s in sname ) {
    	line += sprintf("��%s%s %-37s %s %3s %-6s%7s��\n",
                        skill_class(s),
                        skill_ability(ability[s]),
                        to_chinese(s) + "(" + s + ")",
                        skill_level(SKILL_D(s)->type(), skl[s]),
                        ob->skill_mapped(s) ?  ""+_skill[s] : HIK"---"NOR,
                        ( ob->skill_mapped(s) && _skill[s] != skl[s]) ? (sprintf("(%+d)", _skill[s] - skl[s])) : "",
                        (( flag[s] & SKILL_FLAG_ABANDONED ) ? HIK" ------"NOR : 
                            ( !require[s] ? "00.00%" : sprintf("%2.2f",lrn[s]*100.0/require[s]) +"%" )) );
                
    }
    line += "�t�������������������������������������������������������������������������s";
    this_player()->start_more(line);
    return 1;
}

string skill_level(string type, int level)
{
    int grade;

    grade = sqrt(level/3);

    switch(type) {
        case "knowledge":
            if( grade >= sizeof(knowledge_level_desc) )
                grade = sizeof(knowledge_level_desc)-1;
            return knowledge_level_desc[grade];
        default:
            if( grade >= sizeof(skill_level_desc) )
                grade = sizeof(skill_level_desc)-1;
            return skill_level_desc[grade];
    }
}

int help(object me)
{
    write(@HELP
ָ���ʽ : skills [<ĳ��>]

���ָ����������ѯѧ���ļ��ܼ�����ֵ����ɲ�֪������ʦͽ��ϵ�Ķ���֮��
��״����

��ʦ���Բ�ѯ�κ��˻� NPC �ļ���״����

disable �ļ����ڼ��ܵȼ��Ĳ����� --- ��ʾ
abandon �ļ����ڼ��ܾ���Ĳ����� --- ��ʾ

���ָ�enable��disable��abandon
HELP
    );
    return 1;
}
