/*  logind.c

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

#include <ansi.h>
#include <command.h>
#include <login.h>

inherit F_CLEAN_UP;
inherit F_DBASE;

int wiz_lock_level = WIZ_LOCK_LEVEL;

/*
string *user_race = ({
    "human",
});
*/

string *banned_name = ({
    "你", "你", "我", "他", "她", "它", "它",
});

string *banned_rude_name = ({
    "干", "操", "娘", "妈", "屌", "屄", "插", "奸", "穴",
});

string *banned_ip = ({
// 暂时取消, 记录先留著, 若是情况仍相同再考虑永久ban掉 -dragoon
//	"163.19.7.%*s", 	// mutli, dragoon
//	"140.96.148.%*s",	// mutli, dragoon
});

string *banned_hostname = ({
//	"%*s.Dorm2.nsysu.edu.tw",	// mutli, rude words, dragoon
});

#ifdef ENABLE_ANTISPAM
mapping spammer_player = ([]);
mapping spammer_ip = ([]);
string *penalty_attr = ({
	"str", "dex", "int", "con"
});
#endif	/* ENABLE_ANTISPAM */

private void get_id(string arg, object ob);
private void confirm_id(string yn, object ob);
private void get_email(string email, object ob);
object make_body(object ob);
private void init_new_body(object link, object user);
varargs void enter_world(object ob, object user, int silent);
varargs void reconnect(object ob, object user, int silent);
object find_body(string name);
int check_legal_id(string arg);
int check_legal_name(string arg);
private int list_user_race(object link);
private void increment_visitor_count();
private int check_ip(object link);

private void create() 
{
    seteuid(getuid());
    set("channel_id", "连线精灵");
}

private void reset()
{
    object room, ob;

#if 0
    /* 清空在 void 里的断线玩家。 */
    room = find_object(VOID_OB);
    seteuid(getuid());
    if( objectp(room) )
	foreach(ob in all_inventory(room)) {
#ifdef	SAVE_USER
	    if( userp(ob) ) ob->save();
#endif
	    destruct(ob);
	}
#endif

    /* 记录线上使用者人数 */
    log_file("USRGRAPH", sprintf("[%s] %d users\n",
	ctime(time()), sizeof(users())));

#ifdef ENABLE_ANTISPAM
    spammer_player = ([]);
    spammer_ip = ([]);
#endif
}

void logon(object ob)
{
    object *usr;
    int i, wiz_cnt, ppl_cnt, login_cnt;

#ifdef ENABLE_ANTISPAM
    if( spammer_ip[query_ip_number(ob)] >= 10 ) {
        write("从您连线的主机创造的人物太多了﹐您的主机将被拒绝往来一段时间。\n");
        destruct(ob);
        return;
    }
#endif

    seteuid(getuid());
    efun::write(read_file(WELCOME) + "");
//    UPTIME_CMD->main();
    VISITOR_CMD->main();

    usr = users();
    wiz_cnt = 0;
    ppl_cnt = 0;
    login_cnt = 0;
    // invis wizard count in ppl in stead of wiz, by grain (03/25/1998)
    for(i=0; i<sizeof(usr); i++) {
        if( !environment(usr[i]) ) login_cnt++;
        else if( wizardp(usr[i]) ) {
            if( !usr[i]->link()->query("invis") ) wiz_cnt++;
        }
        else ppl_cnt++;
    }
    printf("目前共有 %d 位巫师、%d 位玩家在线上﹐以及 %d 位使用者尝试连线中。\n\n",
        wiz_cnt, ppl_cnt, login_cnt );

    write("您的使用者代号：");
    input_to( "get_id", ob );
}

private void get_id(string arg, object ob)
{
    object ppl;

    arg = lower_case(arg);
    if( !check_legal_id(arg)) {
	write("您的使用者代号：");
	input_to("get_id", ob);
	return;
    }

    /* 设定连线物件的权限等级 */
    seteuid(arg);
    export_uid(ob);
    seteuid(getuid());
    ob->set("id", arg);

#ifdef MAX_USERS
    if( wizhood(arg)=="(player)" && sizeof(users()) >= MAX_USERS )
    {
	ppl = find_body(arg);
	// Only allow reconnect an interactive player when MAX_USERS exceeded.
	if( !ppl || !interactive(ppl) ) {
	    write("对不起﹐" + MUD_NAME + "的使用者已经太多了﹐请待会再来。\n");
	    destruct(ob);
	    return;
	}
    }
#endif

#ifdef ENABLE_BAN_SITE
    // Rework by Annihilator (11/10/1999), support IP address and hostname
    if( wizhood(arg)=="(player)" )
    {
	string ip, pattern;

	ip = query_ip_number(ob);
	foreach(pattern in banned_ip)
	    if( ip==pattern || sscanf(ip, pattern) ) {
		write("对不起，您的连线位置目前不接受使用者登入。\n");
		destruct(ob);
		return;
	    }
	ip = query_ip_name(ob);
	foreach(pattern in banned_hostname)
	    if( ip==pattern || sscanf(ip, pattern) ) {
		write("对不起，您的连线位置目前不接受使用者登入。\n");
		destruct(ob);
		return;
	    }
    }
#endif	/* ENABLE_BAN_SITE */

#ifdef WIZ_LOCK_LEVEL
    if( (int)wiz_level(arg) < (int)wiz_lock_level ) {
        write("对不起﹐" + MUD_NAME + "目前限制巫师等级 " + WIZ_LOCK_LEVEL
            + " 以上的人才能连线。\n");
        destruct(ob);
        return;
    }
#endif

    if( arg=="guest" ) {
        // If guest, let them create the character.
        get_email( "guest@" + query_ip_name(ob), ob);
        return;
    }

    if( file_size(login_data(arg)) != -1 ) {
        if( ob->restore() ) {
            write("请输入密码﹕");
            input_to("get_passwd", 1, ob);
            return;
        }
        write("对不起﹐您的人物储存挡出了一些问题﹐请利用 guest 人物通知巫师处理。\n");
        destruct(ob);
        return;
    }

    write("使用 " + (string)ob->query("id") + " 这个名字将会创造一个新的人物﹐您确定吗(y/n)﹖");
    input_to("confirm_id", ob);
}

private void get_passwd(string pass, object ob)
{
    string my_pass;
    object user;

    write("\n");

    if( !check_ip(ob) ) {
	write("对不起，您的连线位置不正确。\n");
	destruct(ob);
	return;
    }

    my_pass = ob->query("password");
    if( crypt(pass, my_pass) != my_pass ) {
        write("密码错误！\n");
        destruct(ob);
        return;
    }

    user = find_body(ob->query("id"));
    if (user) {
	/* 断线的使用者重新连线 */
        if( !user->link() ) {
            reconnect(ob, user);
            return;
        }
        write("您要将另一个连线中的相同人物赶出去﹐取而代之吗﹖(y/n)");
        input_to("confirm_relogin", ob, user);
        return;
    }

    user = make_body(ob);
    if( ! user ) {
	/* 如果没有办法制造使用者物件，直接切断连线 */
	destruct(ob);
	return;
    }

    if( user->restore() )
    {
	log_file( "USAGE", sprintf("[%s] %s login from %s\n",
	    ctime(time()), (string)user->query("id"), query_ip_name(ob) ) );
/*
	if( wizhood(ob)=="(admin)") {
	    if( (query_ip_name(ob) != "localhost") && (query_ip_number(ob) != "127.0.0.1") ) {
			write("安全检查失败....自动登出。\n");
			destruct(user);
			destruct(ob);
			return;
	    }
	    write("安全检查通过。\n");
	}
*/	enter_world(ob, user);
	return;
    } else {
	if( file_size(user->query_save_file())==-1 ) {
	    write(@NOTICE
系统找不到您的人物资料，可能的原因包括您在创造人物或人物在投胎转世时
断线，或者因为违反规定您的人物资料被删除了。如果您确定这个人物并没有
上述这些情况，请用 guest 帐号洽线上巫师查询。

NOTICE
	    );
	    destruct(user);
	    write(HIY "您要重新创造这个人物吗？(y/n) " NOR);
	    input_to("confirm_reincarnate", ob);
	} else {
	    write(@NOTICE
系统目前无法读取您的人物资料，可能的原因包括系统正在备分或整理使用者
资料，请稍候再试。
NOTICE
	    );
	    destruct(user);
	    destruct(ob);
	}
    }
}

private void confirm_reincarnate(string yn, object ob)
{
    if( yn=="" ) {
        write(HIY "您要重新创造这个人物吗？(y/n) " NOR);
        input_to("confirm_reincarnate", ob);
        return;
    }

    if( yn[0]!='y' && yn[0]!='Y' ) {
        write("好吧﹐欢迎下次再来。\n");
        destruct(ob);
        return;
    }

    if( !list_user_race(ob) ) {
	/*  业力不足，无法投胎转世的状况。 */
#ifdef	SAVE_USER
	rm(ob->query_save_file());
#endif
        destruct(ob);
        return;
    }

    input_to("get_race", ob);
}

private void confirm_relogin(string yn, object ob, object user)
{
    object old_link;

    if( yn=="" ) {
        write("您要将另一个连线中的相同人物赶出去﹐取而代之吗﹖(y/n)");
        input_to("confirm_relogin", ob, user);
        return;
    }       

    if( yn[0]!='y' && yn[0]!='Y' ) {
        write("好吧﹐欢迎下次再来。\n");
        destruct(ob);
        return;
    } else {
        tell_object(user, "有人从别处( " + query_ip_number(ob)
            + " )连线取代您所控制的人物。\n");
        log_file( "USAGE", sprintf("[%s] %12s replaced @ %s\n",
            ctime(time()),
            (string)user->query("id"),
            query_ip_name(ob),  ) );
    }

    // Kick out tho old player.
    old_link = user->link();
    if( old_link ) {
	seteuid(getuid());
	if( interactive(user) ) exec(old_link, user);
        destruct(old_link);
    }

    reconnect(ob, user);    
}

private void confirm_id(string yn, object ob)
{
    if( yn=="" ) {
        write("使用这个名字将会创造一个新的人物﹐您确定吗(y/n)﹖");
        input_to("confirm_id", ob);
        return;
    }       

    if( yn[0]!='y' && yn[0]!='Y' ) {
        write("好吧﹐那么请重新输入您的英文名字﹕");
        input_to("get_id", ob);
        return;
    }

#ifdef ENABLE_ANTISPAM
    if( spammer_player[ob->query("id")] ) spammer_player[ob->query("id")]++;
    else spammer_player[ob->query("id")] = 1;
    if( spammer_ip[query_ip_number(ob)] ) spammer_ip[query_ip_number(ob)]++;
    else spammer_ip[query_ip_number(ob)] = 1;
#endif
    write("请设定您的密码﹕");
    input_to("new_password", 1, ob);
}

private void new_password(string pass, object ob)
{
    write("\n");
    if( strlen(pass)<5 ) {
        write("密码的长度至少要五个字元﹐请重设您的密码﹕");
        input_to("new_password", 1, ob);
        return;
    }
#ifdef	ENABLE_MD5_PASSWORD
    ob->set("password", crypt(pass, sprintf("$1$%d", random(99999999))));
#else
    ob->set("password", crypt(pass, 0) );
#endif
    write("请再输入一次您的密码﹐以确认您没记错﹕");
    input_to("confirm_password", 1, ob);
}

private void
confirm_password(string pass, object ob)
{
    string old_pass;

    write("\n");
    old_pass = ob->query("password");
    if( crypt(pass, old_pass)!=old_pass ) {
        write("您两次输入的密码并不一样﹐请重新设定一次密码﹕");
        input_to("new_password", 1, ob);
        return;
    }

    write(@TEXT
为了避免您的人物遭人盗用，Naruto 将采用 email 认证方式保护
人物所有权，所以请您输入一个可供接收认证用电子邮件的地址。
这个电子邮件地址除了巫师以外，不会被其他使用者看到。
TEXT
    );
    write("您的电子邮件地址：");
    input_to("get_email",  ob);
}

private void get_email(string email, object ob)
{
    int c, delim=0, err=0;

    if( strlen(email) > 64 ) {
		write("电子邮件地址最多可以有 64 个字元。\n");
		write("您的电子邮件地址：");
		input_to("get_email",  ob);
		return;
    }

    foreach(c in email) {
	if( c=='@' && !delim ) { delim = 1; continue; }
	if( strsrch("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.", c) < 0 )
	    { err = 1; break; }
    }
    if( !delim || err ) {
		write("您的电子邮件格式错误，请输入正确的电子邮件地址。\n");
		write("您的电子邮件地址：");
		input_to("get_email",  ob);
		return;
    }

    ob->set("email", email);

//  If ONE_GUEST is defined in /include/login.h, only permit one guest login.
#ifdef ONE_GUEST
    if ((string)ob->query("id") == "guest") {
        write("抱歉, 线上已经有一个 Guest 了.\n");
        destruct(ob);
        return;            }
#endif /* ONE_GUEST */

    // Complete non-body-specific initialization of new user here.
//    ob->set("karma", 20);
//    list_user_race(ob);
//    input_to("get_race", ob);

    write("您要扮演男性(m)的角色或女性(f)的角色﹖");
    input_to("get_gender", ob, "human");
}

/*
private void get_race(string race, object ob)
{
    int kar;
    string choice;
    if( sscanf(race, "? %s", race) ) {
        write( read_file(HELP_DIR + "help/" + race) );
        // add by ueiren ... 
        // list_user_race(ob);
        input_to("get_race", ob);
        return;
    }
    if( member_array(race, user_race)==-1 ) {
        write("没有这种种族﹐请您重新选择﹕");
        input_to("get_race", ob);
        return;
    }

    write("您要扮演男性(m)的角色或女性(f)的角色﹖");
    input_to("get_gender", ob, race);
}
*/

private void get_gender(string gender, object ob, string race)
{
    object body;
    string body_file;

    if( gender=="" ) {
        write("您要扮演男性(m)的角色或女性(f)的角色﹖");
        input_to("get_gender", ob, race);           
        return;
    }

    if( gender[0]=='m' || gender[0]=='M' )      gender = "male";
    else if( gender[0]=='f' || gender[0]=='F' )     gender = "female";
    else {
        write("对不起﹐您只能选择男性(m)或女性(f)的角色﹕");
        input_to("get_gender", ob, race);
        return;
    }

    if( !stringp(body_file = RACE_D(race)->query("default_body")) ) body_file = USER_OB;

    ob->set("body", body_file);
    if( !(body = make_body(ob)) ) {
        destruct(ob);
        return;
    }

    body->set("gender", gender);
    body->set_race(race);
    init_new_body(ob, body);

    // Remember it so we can dest it if we go linkdead before finishing
    // chraracter creation.
    ob->set_temp("temp_body", body);

    write("您的中文名字﹕");
    input_to("get_name", ob, body);
}

private void get_name(string arg, object ob, object user)
{
    if( !check_legal_name(arg) ) {
        write("您的中文名字﹕");
        input_to("get_name", ob, user);
        return;
    }

    ob->set("name", arg);
    user->set("name", arg);
    if( !ob->query("creation_time") ) {
        log_file( "USAGE", sprintf("[%s] %12s created  @ %s\n",
            ctime(time()),
            user->query("id"),
            query_ip_name(ob), ) );
            ob->set("creation_time", time());
            // default open chat and rumor channel -- by dragoon
            ob->set("channels", ({"chat","rumor"}));
    }
    
    // 新角色 lv1 可获得一些能力点数
    user->add_point("learn", 100);
    user->add_point("score", 50);
    user->add_point("attribute", 2);
    user->add_point("skill", 2);
    
    user->init_attribute((["str": 20, "con": 20, "dex": 20, "int": 20, ]));
    user->init_statistic((["ap": 80, "hp": 80, "mp": 80, ]));

    enter_world(ob, user);
}

object make_body(object link_ob)
{
    string err;
    object user;
    int n;

    seteuid(getuid());
    err = catch(user = new(USER_OB));
    if(err || !user) {
        write("现在可能有巫师正在修改使用者物件的程式﹐请稍候再试。\n");
        return 0;
    }

    /* 设定使用者的权限 */
    seteuid(getuid(link_ob));
    export_uid(user);
    seteuid(getuid());

    user->set("id", link_ob->query("id"));
    user->set_name( link_ob->query("name"), link_ob->query("id") );

    return user;
}

private int check_ip(object link)
{
    string okip, cur_ip, cur_ip_num, ip_part, num_part, be_checked;
    int len, ed;

    cur_ip = query_ip_name(link);
    cur_ip_num = query_ip_number(link);

    okip = link->query("okip");
    if( !arrayp(okip) || !sizeof(okip) ) return 1;

    foreach(string ip in explode(okip, ":") - ({ "" })) {
		if (sscanf(ip, "%s*", be_checked) && be_checked!="") {
		    len = strlen(be_checked);
		    ip_part = cur_ip[0..len-1];
	    	num_part = cur_ip_num[0..len-1];
		} else if (sscanf(ip, "*%s", be_checked) && be_checked!="") {
		    len = strlen(be_checked);
		    ed = strlen(cur_ip);
	    	ip_part = cur_ip[ed-len..<1];
		    num_part = cur_ip_num[ed-len..<1];
		} else {
		    be_checked = ip;
		    ip_part = cur_ip;
	    	num_part = cur_ip_num;
		}
       
		if (lower_case(be_checked) == lower_case(ip_part) || lower_case(be_checked) == lower_case(num_part)) {
	    	write(HIW "检查通过。\n" NOR);
	    	return 1;
		}
    }

    return 0;
}

private void init_new_body(object link, object user)
{
#ifdef ENABLE_ANTISPAM
    int penalty;
    string a;
#endif

    user->set("birthday", time() );
    user->set_class("commoner");
    user->set_level(1);

    CHAR_D->setup_char(user);
}

varargs void enter_world(object ob, object user, int silent)
{
    object room;
//    string startroom, err, mot;
    string err, mot;

    /* 将使用者从连线物件 exec 到使用者物件上 */
    user->set_link(ob);
    ob->set_body(user);
    exec(user, ob);

    if( !silent ) write("目前权限﹕" + wizhood(user) + "\n");

    user->setup();
    increment_visitor_count();

#ifdef SAVE_USER
    /* 在这里储存，让新创造的人物被存下来 */
    user->save();
#endif

    if( silent ) return;

    if( !mot = read_file(MOTD) ) mot = "";
    user->start_more(mot);
//    cat(MOTD);

    IDENT_D->query_userid((string)user->query("id"));

    /* 以安全的方法载入使用者登入的房间 */
   	err = catch(room=load_object(user->query("startroom")));
   	if( !room || !room->is_area() ||
   		!area_move(room, user, user->query("area_info/x_axis"), user->query("area_info/y_axis") ) )
    {
    	err = catch(room = load_object(user->query("recoverroom")));
    	if( !room || !user->move(room) ) {
	        if( !room ) err = catch(room = load_object(START_ROOM));
	        if( !room ) err = catch(room = load_object(VOID_OB));
  		
    	    if( !room || !user->move(room) ) {
        	    write("对不起，目前系统正在整修一些登入地点的程式，请稍候再试。\n");
			    destruct(ob);
			    destruct(user);
	 	    }
	 	}
  	}

    /* 检查使用者是否有未读的信件 */
    if( ob->query("new_mail") ) {
		write( HIW "\n有您的信！请到驿站来一趟 ...\n\n" NOR);
		ob->delete("new_mail");
    }
 
    // if detect mark: pker, set the time mark -dragoon
    if( user->query("pker") ) {
		user->set("last_pk_time", time());
		user->delete("pker");
    }

    /* 宣告使用者登入的消息 */
    if( !wizardp(user) && !user->query("invis") ) {
		message_vision("$N连线进入这个世界。\n", user);
		CHANNEL_D->do_channel( this_object(), "sys",
			sprintf("%s(%s)由%s连线进入。",
			user->name(1),
			user->query("id"),
			query_ip_name(user)) );
    }
}

varargs void reconnect(object ob, object user, int silent)
{
    /* 将使用者从连线物件 exec 到使用者物件上 */
    user->set_link(ob);
    ob->set_body(user);
    exec(user, ob);

    user->reconnect();
    IDENT_D->query_userid((string)user->query("id"));   

    if( silent ) return;

    // if detect pking, reset time mark to now -dragoon
    if( time() - (int)user->query("last_pk_time") < 60 * 60 )
	user->set("last_pk_time", time());

    /* 宣告使用者重新连线的消息 */
    if( !wizardp(user) && !user->query("invis") ) {
		message_vision("$N重新连线回到这个世界。\n", user);
		CHANNEL_D->do_channel( this_object(), "sys",
			sprintf("%s(%s)由%s重新连线进入。",
			user->name(1),
			user->query("id"),
			query_ip_name(user))
		);
    }
}

void net_dead(object ob)
{
    CHANNEL_D->do_channel( this_object(), "sys", sprintf("%s(%s)断线了。", ob->name(1), ob->query("id")));
    ob->move("/obj/void");
}

int check_legal_id(string id)
{
    int i;

    i = strlen(id);
    if( (strlen(id) < 3) || (strlen(id) > 10 ) ) {
        write("对不起﹐您的英文名字必须是 3 到 10 个英文字母。\n");
        return 0;
    }
    while(i--)
        if( id[i]<'a' || id[i]>'z' ) {
            write("对不起﹐您的英文名字只能用英文字母。\n");
            return 0;
        }

    return 1;
}

int check_legal_name(string name)
{
    int i;
    string bname;

    i = strlen(name);

    if( (strlen(name) < 2) || (strlen(name) > 12 ) ) {
        write("对不起，您的中文名字必须是 1 到 6 个中文字。\n");
        return 0;
    }
    while(i--) {
        if( name[i]<=' ' ) {
            write("对不起﹐您的中文名字不能用控制字元。\n");
            return 0;
        }
        if( i%2==0 && !is_chinese(name[i..<0]) ) {
            write("对不起﹐请您用“中文”取名字。\n");
            return 0;
        }
        if( name[i..i+1]=="　" ) {
            write("对不起﹐请不要用空白字元取名字。\n");
            return 0;
        }
    }
    if( member_array(name, banned_name)!=-1 ) {
        write("对不起﹐这种名字会造成其他人的困扰。\n");
        return 0;
    }
    foreach(bname in banned_rude_name)
        if( strsrch( name, bname )>-1 ) {
            write("对不起, 这种名子会引起不必要的误会。\n");
            return 0;
        }
    return 1;
}

object find_body(string name)
{
    object ob, *body;

    if( objectp(ob = find_player(name)) ) return ob;
    foreach(ob in children(USER_OB))
        if( clonep(ob)
        &&  userp(ob)
        &&  ob->is_character()
        &&  getuid(ob) == name )
            return ob;

    return 0;
}

int set_wizlock(int level)
{
    if( wiz_level(this_player(1)) <= level ) return 0;
    if( geteuid(previous_object()) != ROOT_UID ) return 0;

    wiz_lock_level = level;
    return 1;
}

/*
static int list_user_race(object link)
{
    string msg, race;

    msg = "";
    foreach(race in user_race) {
		msg += sprintf("%-25s\n",
			to_chinese(race) + "(" + race + ")" );
    }

    msg += "您的选择(用 '? <种族名>' 可查阅\说明)﹕";

    write(msg);
    return 1;
}
*/

#define VISITOR_COUNTER_FILE	"/adm/etc/visitor.cnt"

private void increment_visitor_count()
{
    int t, cnt;
    string s = read_file(VISITOR_COUNTER_FILE);

    if( !s ) 
        s = sprintf("%d 1", time());
    else {
        sscanf(s, "%d %d", t, cnt);
        if( ! t ) { t = time(); cnt = 0; }
        s = sprintf("%d %d", t, cnt+1);
    }
    write_file(VISITOR_COUNTER_FILE, s, 1);
}
