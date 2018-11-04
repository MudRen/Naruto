/*  chinesed.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>

    Update By -Acme-
*/

// chinese_d.c
// A minor chinese-english or english-chinese converter.
// by Annihilator@Eastern.Stories 09-27-93
// adapted for ES2-lib 01-18-95

#pragma save_binary

#include <ansi.h>
#include <localtime.h>

#define DICTIONARY DATA_DIR + "chinese"

inherit F_CLEAN_UP;
inherit F_SAVE;

static string *c_digit = ({ "��","ʮ","��","ǧ","��","��","��" });
static string *c_num = ({"��","һ","��","��","��","��","��","��","��","��","ʮ"});

mapping dict = ([]);

void add_translate( string key, string chinz );
void remove_translate( string key );
mapping query_translate();	// Add by -Acme-

void create()
{
    seteuid(getuid());
    restore();
}

void remove() { save(); }

string chinese_number(int i)
{
    if( i<0 ) return "��" + chinese_number(-i);
    if( i<11 ) return c_num[i];
    if( i<20 ) return c_num[10] + c_num[i-10];
    if( i<100 ) {
        if( i%10 ) return c_num[i/10] + c_digit[1] + c_num[i%10];
        else return c_num[i/10] + c_digit[1];
    }
    if( i<1000 ) {
        if( i%100==0 )
            return c_num[i/100] + c_digit[2];
        else if( i%100 < 10 )
            return c_num[i/100] + c_digit[2] + c_num[0] + chinese_number(i%100);
        else if( i%100 < 20 )
            return c_num[i/100] + c_digit[2] + c_num[1] + chinese_number(i%100);
        else return c_num[i/100] + c_digit[2] + chinese_number(i%100);
    }
    if( i<10000 ) {
        if( i%1000==0 )
            return c_num[i/1000] + c_digit[3];
        else if( i%1000 < 100 )
            return c_num[i/1000] + c_digit[3] + c_digit[0] + chinese_number(i%1000);
        else
            return c_num[i/1000] + c_digit[3] + chinese_number(i%1000);
    }
    if( i<100000000 ) {
        if( i%10000==0 )
            return chinese_number(i/10000) + c_digit[4];
        else if( i%10000 < 1000 )
            return chinese_number(i/10000) + c_digit[4] + c_digit[0] + chinese_number(i%10000);
        else
            return chinese_number(i/10000) + c_digit[4] + chinese_number(i%10000);
    }
    if( i<1000000000000 ) {
        if( i%100000000==0 )
            return chinese_number(i/100000000) + c_digit[5];
        else if( i%100000000 < 10000000 )
            return chinese_number(i/100000000) + c_digit[5] + c_digit[0] + chinese_number(i%100000000);
        else
            return chinese_number(i/100000000) + c_digit[5] + chinese_number(i%100000000);
    }
        if( i%1000000000000==0 )
            return chinese_number(i/1000000000000) + c_digit[6];
        else if( i%1000000000000 < 100000000000 )
            return chinese_number(i/1000000000000) + c_digit[6] + c_digit[0] + chinese_number(i%1000000000000);
        else
            return chinese_number(i/1000000000000) + c_digit[6] + chinese_number(i%1000000000000);
}

string query_save_file() { return DICTIONARY; }

// This is called by to_chinese() simul_efun
string chinese(string str)
{
    if( !undefinedp( dict[str] ) ) return dict[str];
    else return str;
}

mapping query_translate() { return dict; }

void remove_translate( string key )
{
    map_delete( dict, key );
    save();
}

void add_translate( string key, string chinz )
{
    dict[key] = chinz;
    save();
}

void dump_translate()
{
    string *k, str;
    int i;

    str = "";
    k = keys(dict);
    for(i=0; i<sizeof(k); i++)
        str += sprintf("%-30s %s\n", k[i], dict[k[i]]);
    write_file("/CHINESE_DUMP", str);
}

string chinese_date_brief(int date)
{
    mixed *local;

    local = localtime(date);

    return sprintf("%d/%s/%s %s:%s",
	               local[LT_YEAR],
	               local[LT_MON]+1 < 10 ? "0" + (local[LT_MON]+1) : (local[LT_MON]+1) + "",
	               local[LT_MDAY] < 10 ? "0" + local[LT_MDAY] : local[LT_MDAY] + "",
	               local[LT_HOUR] < 10 ? "0" + local[LT_HOUR] : local[LT_HOUR] + "",
	               local[LT_MIN] < 10 ? "0" + local[LT_MIN] : local[LT_MIN] +"" );
}

string chinese_date(int date)
{
    mixed *local;

    local = localtime(date);

    return sprintf("%d��%d��%d��%dʱ%d��",
	               local[LT_YEAR],
	               local[LT_MON]+1,
	               local[LT_MDAY],
	               local[LT_HOUR],
	               local[LT_MIN] );
}

string chinese_period(int t)
{
    int y, n, d, h, m, s;
    string time;

    s = t % 60;        t /= 60;
    m = t % 60;        t /= 60;
    h = t % 24;        t /= 24;
    d = t % 30;        t /= 30;
    n = t % 12;        t /= 12;
    y = t;

    time = "";

    if(y) time = chinese_number(y) + "����";
    if(n) time += chinese_number(n) + "����"    + (strlen(time)?"":"��");
    if(d) time += chinese_number(d) + "��"        + (strlen(time)?"":"��");
    if(h) time += chinese_number(h) + "Сʱ"    + (strlen(time)?"":"��");
    if(m) time += chinese_number(m) + "��"        + (strlen(time)?"":"��");
    time += chinese_number(s) + "��";

    return time;
}

int ncstrlen(string msg)
{
    int i=0, j=0, t=0;
    for(i=0;i<=strlen(msg);i++) {
	if( msg[i..i] == "[" ) j=i-1;
	else if( msg[i..i] == "m" ) {
	    t+=i-j+1;
	    j=i;
	}
   }
   return strlen(msg)-t;
}