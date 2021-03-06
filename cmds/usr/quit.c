/* quit.c */

#include <command.h>
#include <login.h>
#include <action.h>

inherit F_DBASE;
inherit F_CLEAN_UP;

void create() 
{
    seteuid(getuid());
    set("channel_id", "离线指令");
}

int main(object me, string arg)
{
    int i;
    object link_ob, ob, ob2;

    if( me->is_busy() ) {
        write("你正忙著呢, 请试著停下所忙的事。\n");
        return 1;
    }

    if( me->query_team() ) me->dismiss_team();

    // 检查身上是否有袋子，若有，将袋子里的东西移到身上
    foreach(ob in all_inventory(me))
        if( ob->accept_object() ) // 是个袋子
            foreach(ob2 in all_inventory(ob))
            	if( !ob2->move(me) ) DROP_CMD->do_drop(me, ob2);

    link_ob = me->link();

    // We might be called on a link_dead player, so check this.
    if( link_ob ) {

        // Are we possessing in others body ?
        if( link_ob->is_character() ) {
            write("你的魂魄回到" + link_ob->name(1) + "的身上。\n");
            exec(link_ob, me);
            link_ob->setup();
            return 1;
        }

        link_ob->set("last_on", time());
        link_ob->set("last_from", query_ip_name(me));
#ifdef SAVE_USER
        link_ob->save();
#endif
        destruct(link_ob);
    }

    // 设定连线进入开始位置
    if( environment(me) && !environment(me)->query("void") && !environment(me)->is_character() )
		me->set("startroom", file_name(environment(me)) );

    // if pking, set a pker mark before quit -dragoon
    if( time() - (int)me->query("last_pk_time") < 60 * 60 ) me->set("pker", 1);

    write("欢迎下次再来﹗\n");
    message_vision("$N(" + me->query("id") + ") 离开游戏。\n", me);
//    message("system", me->name() + "(" + me->query("id") + ") 离开游戏。\n", environment(me), me);

    CHANNEL_D->do_channel(this_object(), "sys",    me->name() + "("+ me->query("id") +") 离开游戏了。");

#ifdef SAVE_USER
    me->save();
#endif
    log_file( "USAGE", sprintf("[%s] %12s quit\n", ctime(time()), (string)me->query("id")));

    destruct(me);

    return 1;
}

int help(object me)
{
    write(@HELP
指令格式 : quit

当你(你)想暂时离开时, 可利用此一指令。
HELP
    );
    return 1;
}
