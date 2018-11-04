/*  soldier.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

mixed do_chat(mixed chat);
varargs string name(int raw);
int is_fighting(object);
void kill_ob(object);

int accept_fight(object ob)
{
    switch(random(5)) {
        case 0: do_chat((: command, "shake" :));    break;
        case 1: do_chat(name() + "说道﹕走开走开﹗\n");        break;
        case 2: do_chat((: command, "hehe" :));    break;
    }
    return 0;
}

void accept_kill(object ob)
{
    if( is_fighting(ob) ) return;
    do_chat( name() + "说道﹕咦﹖造反啦﹖去死吧 ...\n");
    
    // 增加区域判断 by -Acme-
    if( environment()->is_area() ) {
    	mapping info;
    	info = this_object()->query("area_info");
    	if( !info ) return;
    	environment()->query_inventory(info["x_axis"], info["y_axis"])->soldier_assist(this_object(), ob);
    }
    else all_inventory(environment())->soldier_assist(this_object(), ob);
}

void soldier_assist(object alert, object ob)
{
    if( alert==this_object() || !living(this_object()) || is_fighting(ob) )
        return;

    switch(random(5)) {
	    case 0: do_chat(name() + "说道﹕大胆刁民﹗竟敢公然逞凶﹗\n"); break;
    	case 1: do_chat(name() + "喝道﹕反了反了﹗兄弟们﹐拿下这个匪徒﹗\n"); break;
	    case 2: command("say 喂﹗你想干什么﹖"); break;
    	case 3: command("say 来人哪﹗拿下那个可疑的家伙﹗"); break;
    }
    
    kill_ob(ob);
    ob->fight_ob(this_object());
}
