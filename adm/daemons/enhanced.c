/*  enhanced.c

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

string enhance_weapon(object ob, int bonus, string weapon_type)
{
    int major = random(200), minor = random(1000);
    string new_name = "";
    string id = ob->query("id");
    mapping apply = ([]);

    if( major < 3 )
        switch( random(14) ) {
        case 0:
            new_name += CYN "ÇàÍ­"; apply["defense"] = 10;
            id = "bronze " + id;
            break;
        case 1:
            new_name += RED "ïÙÌú"; apply["damage"] = 3;
            id = "iron " + id;
            break;
        case 2:
            new_name += HIW "µã¸Ö"; apply["attack"] = 5; apply["damage"] = 2;
            id = "steel " + id;
            break;
        case 3:
            new_name += HIB "Çà¸Ö"; apply["attack"] = 5; apply["defense"] = 5;
            id = "steel " + id;
            break;
        case 4:
            new_name += HIK "ºÚÌú"; apply["damage"] = 2; apply["defense"] = 5;
            id = "iron " + id;
            break;
        case 5:
            new_name += MAG "ËÉÎÆ"; apply["str"] = 1; apply["attack"] = 5;
            id = "fine " + id;
            break;
        case 6:
            new_name += HIM "µñ»¨"; apply["dex"] = 1; apply["defense"] = 5;
            id = "fine " + id;
            break;
        case 7:
            new_name += HIK "²ø²¼"; apply["cps"] = 1; apply["armor"] = 3;
            id = "wrapped-hilt " + id;
            break;
        case 8:
            new_name += HIK "Ìú¼¬"; apply["str"] = 1; apply["damage"] = 2;
            id = "thorny " + id;
            break;
        case 9:
            new_name += HIW  "Ì«¼«"; apply["wis"] = 1; apply["defense"] = 5;
            id = "taiji " + id;
            break;
        case 10:
            new_name += HIW  "ÒõÑô"; apply["int"] = 1; apply["attack"] = 5;
            id = "yinyang " + id;
            break;
        case 11:
            new_name += HIR "ÖìÉ°"; apply["magic"] = 5; apply["spells"] = 5;
            id = "cinnabar " + id;
            break;
        case 12:
            new_name += HIY "ÈÕÔÂ"; apply["con"] = 1; apply["magic"] = 5;
            id = "glowing " + id;
            break;
        case 13:
            new_name += HIK "Ñá»ð"; apply["con"] = 1; apply["damage"] = 2;
            id = "yenholdish " + id;
            break;
    }
    else if( major < 6 )
        switch( random(22) ) {
        case 0:
            new_name += HIW "¾«¸Ö"; apply["attack"] = 10; apply["damage"] = 3;
            id = "highsteel " + id;
            break;
        case 1:
            new_name += HIW "°×Òø"; apply["defense"] = 15;
            id = "silver " + id;
            break;
        case 2:
            new_name += HIK "ºÚ¸Ö"; apply["attack"] = 15;
            id = "darksteel " + id;
            break;
        case 3:
            new_name += YEL "¹Å¶§"; apply["defense"] = 10; apply["cps"] = 1;
            id = "darkmetal " + id;
            break;
        case 4:
            new_name += RED "³ûÌú"; apply["attack"] = 10; apply["defense"] = 10;
            id = "iron" + id;
            break;
        case 5:
            new_name += HIR "Í®ÔÆ"; apply["wis"] = 1; apply["cor"] = 1;
            id = "cloudy " + id;
            break;
        case 6:
            new_name += MAG "×Ïµç"; apply["dex"] = 1; apply["str"] = 1;
            id = "thunder " + id;
            break;
        case 7:
            new_name += HIY "½ðÏâ"; apply["con"] = 1; apply["int"] = 1;
            id = "golden " + id;
            break;
        case 8:
            new_name += HIW "Ì«Ðþ"; apply["spi"] = 1; apply["spells"] = 10;
            id = "mystic " + id;
            break;
        case 9:
            new_name += HIB "Ì«Çå"; apply["wis"] = 1; apply["spells"] = 10;
            id = "mystic " + id;
            break;
        case 10:
            new_name += HIC "Ì«ÒÒ"; apply["int"] = 1; apply["spells"] = 10;
            id = "mystic " + id;
            break;
        case 11:
            new_name += HIK "Ì«Òõ"; apply["int"] = 2;
            id = "moon " + id;
            break;
        case 12:
            new_name += HIY "Ì«Ñô"; apply["con"] = 2;
            id = "sun " + id;
            break;
        case 13:
            new_name += HIB "±ÙÑý"; apply["magic"] = 15;
            id = "demonbane " + id;
            break;
        case 14:
            new_name += BLU "±ÙÐ°"; apply["spells"] = 15;
            id = "devilbane " + id;
            break;
        case 15:
            new_name += HIK "Õò¹í"; apply["magic"] = 10; apply["spells"] = 10;
            id = "nether " + id;
            break;
        case 16:
            new_name += RED "³àÑª"; apply["damage"] = 5;
            id = "blood " + id;
            break;
        case 17:
            new_name += HIR "±ÌÑª"; apply["armor"] = 5;
            id = "blood " + id;
            break;
        case 18:
            new_name += CYN "ÎÞ³¦"; apply["int"] = 3; apply["con"] = -1;
            id = "woochanian " + id;
            break;
        case 19:
            new_name += HIG "½¹½Ä"; apply["dex"] = 3; apply["str"] = -1;
            id = "jiaojao " + id;
            break;
        case 20:
            new_name += HIM "×ÏÑô"; apply["cps"] = 2;
            id = "dawn " + id;
            break;
        case 21:
            new_name += HIC "ÇàÑô"; apply["wis"] = 2;
            id = "shining " + id;
            break;
    }

    if( new_name=="" ) return ob->name();
    
    if( minor < 3 )
        switch( random(13) ) {
        case 4:
            new_name += HIR "»ðÑ×";
            id += " of flame";
            break;
        case 5:
            new_name += HIB "º®±ù";
            id += " of freeze";
            break;
        case 6:
            new_name += HIC "·ç²¨";
            id += " of wind";
            break;
        case 7:
            new_name += HIB "ÕðÀ×";
            id += " of lightning";
            break;
        case 8:
            new_name += YEL "ÂÞºº";
            id += " of guardian";
            break;
        case 9:
            new_name += HIW "ÌìÁú";
            id += " of wyvern";
            break;
        case 10:
            new_name += HIR "³àÁú";
            id += " of red dragon";
            break;
        case 11:
            new_name += HIY "½ðÓ¥";
            id += " of eagle";
            break;
        case 12:
            new_name += HIC "ÌìÓ¥";
            id += " of falcon";
            break;
    }

    if( new_name != "" ) {
        mixed wield_as;
        string wield_skill;
        new_name += weapon_type + NOR;
        ob->set("name", new_name);
        ob->set("id", id);
        // Apply enchantment to the weapon.
        wield_as = ob->query("wield_as");
        if( arrayp(wield_as) ) 
            foreach(wield_skill in wield_as)
                ob->set("apply_weapon/" + wield_skill, apply);
        else
            ob->set("apply_weapon/" + wield_as, apply);
        return new_name;
    }

    return ob->name();
}
