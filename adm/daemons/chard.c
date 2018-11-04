/*  chard.c - protected character operation daemon

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#pragma save_binary

#include <login.h>
#include <statistic.h>

inherit F_CLEAN_UP;

private void create() { seteuid(getuid()); }

// setup_char()
//
// This function is called when characters need setup (after create() is
// called). The major task of this function is to keep consistency of
// user data and NPCs. Because the game system may change with development
// of mudlib, there might be minor or major changes in creating character
// objects. Also, this function serves as default creation function of 
// NPCs that saves wizards' work on initializing NPCs.

/*  setup()
 *
 *  һ���Ե������趨����������������������������һЩ��ʼ�����趨��
 */
void setup_char(object ob)
{
    mapping apply;

    // �������û���趨���塢ְҵ���ȼ�����һ��Ԥ��ֵ
    if( !ob->query_race() ) ob->set_race("human");
    if( !ob->query_class() ) ob->set_class("commoner");
    if( !ob->query_level() ) ob->set_level(1);

    ob->init_statistic();
    ob->init_attribute();

    // ��ʼ���������ʱ״̬
    if( mapp(apply = ob->query("perm_apply") ) ) ob->set_temp("apply", copy(apply));
    else ob->set_temp("apply", ([]));

/* 
    ��������������̬(life_form)��ִ��ϵͳԤ����趨
    TYPE_STATIC        ���������ֵ�ǲ����Զ����µ�
    TYPE_HEALTH     ���������ֵ�ǻ���ʱ���Զ��ָ���
    TYPE_WASTING    ���������ֵ�ǻ���ʱ���������ٵ�
*/
    ob->set_stat_regenerate("ap", TYPE_HEALTH);
    ob->set_stat_regenerate("hp", TYPE_HEALTH);
    ob->set_stat_regenerate("mp", TYPE_HEALTH);

    // ִ�����弰ְҵ�ĳ�ʼ���������ְҵ��������޸�ǰ���ʼ���������ԡ�
    RACE_D(ob->query_race())->setup(ob);
    CLASS_D(ob->query_class())->setup(ob);
    ob->add_path("/daemon/race/" + ob->query_race() + "/");
    ob->add_path("/daemon/class/" + ob->query_class() + "/");
}

varargs object make_corpse(object victim, object killer)
{
    int amount;
    mapping bounty;
    object corpse, inv;
    string score, corpse_ob;

    // ��ʦ����Ҫ����ʬ��
    if( wizardp(victim) ) return 0;

    // Notify inventory item to save data if nessessary.
    all_inventory(victim)->owner_is_killed(killer);

    // Check if using custom corpse object.
    corpse_ob = victim->query("corpse_ob");
    if( !corpse_ob ) corpse_ob = CORPSE_OB;

    seteuid(MUDLIB_UID);
    catch(corpse = new(corpse_ob));
    seteuid(getuid());

    corpse->set_name( victim->name(1) + "��ʬ��", "corpse of " + capitalize(victim->query("id")), "corpse" );
    corpse->set("long", "һ��" + victim->name(1) + "��ʬ�壬����������Ť�˵��Ҳ���ɢ���������\n");
    corpse->set("age", victim->query("age"));
    corpse->set("gender", victim->query("gender"));
    corpse->set("victim_name", victim->name(1));
    corpse->set("level", victim->query_level());
    corpse->set("userp", userp(victim));

    corpse->set_weight( victim->query_weight() );
    corpse->set_max_encumbrance( victim->query_max_encumbrance() );
    corpse->start_decay( userp(victim) ? 120 : 60 );

    if( environment(victim)->is_area() ) {
        area_move(environment(victim), corpse, victim->query("area_info/x_axis"), victim->query("area_info/y_axis"));
    } else corpse->move(environment(victim));

    // ����������Ʒת�Ƶ�ʬ����
    foreach(inv in all_inventory(victim)) inv->move(corpse);

    victim->set_temp("corpse", corpse);

    if( !killer ) killer = victim->last_damage_giver();

    if( objectp(killer) ) {
        object *team;

        // ɱ������npc����ĳ����ӵ�У���ɱ���߼���ĳ��
        if( !userp(killer) && objectp(killer->query_owner()) ) killer = killer->query_owner();

        team = killer->query_team();

        // ��¼������
        if( victim->query("vendetta_mark") ) killer->add("vendetta/" + victim->query("vendetta_mark"), 1);

        // ���ɱ��������Ļ���ɱҵ 1
        if( victim->query("humanoid") ) killer->gain_score("kills", 1);

        // ��þ��� (���victim���趨bounty�Ļ�)
        if( mapp(bounty = victim->query("bounty")) ) {
            foreach(score, amount in bounty) {
                if( team ) {
                    object ob;
                    if( environment(killer)->is_area() ) {
                        foreach(ob in team) if( area_environment(killer, ob) )
                            ob->gain_score(score, amount);
                    } else {
                        foreach(ob in team) if( environment(killer) == environment(ob) )
                            ob->gain_score(score, amount);
                    }
                }
                else killer->gain_score(score, amount);
            }
        }

        // ����ѧϰ����
        if( team ) {
            object ob;
            if( environment(killer)->is_area() ) {
                foreach(ob in team) if( area_environment(killer, ob) )
                    ob->add_point("learn", victim->query_level());
            } else {
                foreach(ob in team) if( environment(killer) == environment(ob) )
                    ob->add_point("learn", victim->query_level());
            }
        } else killer->add_point("learn", victim->query_level());
        
        // Finally, let class daemon check this kill.
        //    CLASS_D(killer->query_class())->kill_check(killer, victim, corpse);
    }        
    return corpse;
}