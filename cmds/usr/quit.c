/* quit.c */

#include <command.h>
#include <login.h>
#include <action.h>

inherit F_DBASE;
inherit F_CLEAN_UP;

void create() 
{
    seteuid(getuid());
    set("channel_id", "����ָ��");
}

int main(object me, string arg)
{
    int i;
    object link_ob, ob, ob2;

    if( me->is_busy() ) {
        write("����æ����, ������ͣ����æ���¡�\n");
        return 1;
    }

    if( me->query_team() ) me->dismiss_team();

    // ��������Ƿ��д��ӣ����У���������Ķ����Ƶ�����
    foreach(ob in all_inventory(me))
        if( ob->accept_object() ) // �Ǹ�����
            foreach(ob2 in all_inventory(ob))
            	if( !ob2->move(me) ) DROP_CMD->do_drop(me, ob2);

    link_ob = me->link();

    // We might be called on a link_dead player, so check this.
    if( link_ob ) {

        // Are we possessing in others body ?
        if( link_ob->is_character() ) {
            write("��Ļ��ǻص�" + link_ob->name(1) + "�����ϡ�\n");
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

    // �趨���߽��뿪ʼλ��
    if( environment(me) && !environment(me)->query("void") && !environment(me)->is_character() )
		me->set("startroom", file_name(environment(me)) );

    // if pking, set a pker mark before quit -dragoon
    if( time() - (int)me->query("last_pk_time") < 60 * 60 ) me->set("pker", 1);

    write("��ӭ�´������u\n");
    message_vision("$N(" + me->query("id") + ") �뿪��Ϸ��\n", me);
//    message("system", me->name() + "(" + me->query("id") + ") �뿪��Ϸ��\n", environment(me), me);

    CHANNEL_D->do_channel(this_object(), "sys",    me->name() + "("+ me->query("id") +") �뿪��Ϸ�ˡ�");

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
ָ���ʽ : quit

����(��)����ʱ�뿪ʱ, �����ô�һָ�
HELP
    );
    return 1;
}
