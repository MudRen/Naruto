/*  channeld.c - supply moving ability for an object

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
#include <intermud.h>
//#include <net/dns.h>

#pragma save_binary

inherit F_CLEAN_UP;
#define CHANCE 20
 
mapping channels = ([
#if 0
    "gwiz": ([	"msg_speak": HIG "��������ʦ��%s��%s\n" NOR,
		"msg_emote": HIG "��������ʦ��*%s\n" NOR,
		"wiz_only": 1, "intermud": GWIZ, "channel": "CREATOR",
		"filter": 1,
		"intermud_emote": 1
	    ]),
    "es":   ([	"msg_speak": HIG "���������¡�%s��%s\n" NOR,
		"msg_emote": HIG "���������¡�*%s" NOR,
		"intermud": INTERMUD_SERVICE("gchannel"),
		"channel": "es",
		"filter": (: mapp($1) && $1["MUDLIB"]=="Eastern Stories" :)
	    ]),
    "twiz": ([	"msg_speak": HIW "��̨����ʦ��%s��%s\n" NOR,
		"msg_emote": HIW "��̨����ʦ��*%s" NOR,
		"wiz_only":1, "intermud": INTERMUD_SERVICE("gchannel"), "channel": "twiz",
		"filter": (: $1["HOSTADDRESS"][0..2]=="140" :) ]),
#endif
    "sys":  ([	"msg_speak": HIB "��ϵͳ��"NOR"%s��%s\n",
		        "wiz_only": 1,
		        "no_log" : 1,
            ]),
    "wiz":  ([	"msg_speak": HIY "����ʦ��"NOR"%s��%s\n",
		        "msg_emote": HIY "����ʦ��"NOR"*%s",
		        "wiz_only": 1
            ]),

    "chat": ([	"msg_speak": HIC "�����ġ�"NOR"%s��%s\n",
		        "msg_emote": HIC "�����ġ�"NOR"*%s",
		    ]),
		
    "news": ([	"msg_speak": HIG "�����š�"NOR"%s��%s\n",
		        "msg_emote": HIG "�����š�"NOR"*%s",
		    ]),
		
    "rumor":([	"msg_speak": HIM "��ҥ�ԡ�"NOR"%s��%s\n" NOR,
		        "msg_emote": HIM "��ҥ�ԡ�"NOR"*%s" NOR,
		        "anonymous": "ĳ��"
		    ]),
		
    "music":([	"msg_speak": HIW "�����֡�"NOR"%s������%s\n",
		        "msg_emote": HIW "�����֡�"NOR"*%s"
		    ]),
]);
 
private void create() { seteuid(getuid()); }
 
varargs int do_channel(object me, string verb, string arg, int emote)
{
    object *ob;
    string *tuned_ch, who, origin_verb, origin_msg;
 
    // set for log channel msg
    origin_verb = verb;
    origin_msg = arg;

    if( verb== "" ) return 0;
 
    // Check if this is a channel emote.
    if( verb[<1] == '*' ) {
        emote = 1;
        verb = verb[0..<2];
    }
 
    if( !mapp(channels) || undefinedp(channels[verb]) )
        return 0;
 
    if( userp(me) && me->link() ) {
        if(channels[verb]["wiz_only"] && !wizardp(me) )
            return 0;
 
        if (me->query("no_chat") == 1)
        	return notify_fail("��������Ϊ����, ����ֹʹ�ù���Ƶ����\n");

        // add limit on public channel by dragoon
//        if (!wizardp(me) && me->query("level") < 2)
//        	return notify_fail("��ĵȼ����㣬�޷�ʹ�ù���Ƶ����\n");

        // If we speaks something in this channel, then must tune it in.
        tuned_ch = me->link()->query("channels");
        if( !pointerp(tuned_ch) )
            me->link()->set("channels", ({ verb }) );
        else if( member_array(verb, tuned_ch)==-1 )
            me->link()->set("channels", tuned_ch + ({ verb }) );
 
        // Support of channel emote
        if( emote && !channels[verb]["intermud_emote"]) {
            string vb, emote_arg;
 
            if (!arg) 
                arg = (string)me->name(1)+"����������ḻ��\n";
            else {
                if( sscanf(arg, "%s %s", vb, emote_arg)!= 2 ) {
                    vb = arg;
                    emote_arg = "";
                }
                arg = EMOTE_D->do_emote(me, vb, emote_arg, 1);
                if(!arg)
                    if(is_chinese(vb))
                        arg=(string)me->name(1)+vb+" "+emote_arg+"\n";
                    else
                        return 0;
                if( channels[verb]["anonymous"] )
                    arg = replace_string(arg, me->name(1), channels[verb]["anonymous"]);
            }
        }
        if( (string)me->query_temp("last_channel_msg")==(verb + "\n" + arg) )
            return notify_fail("�ý�̸Ƶ��˵���벻Ҫ�ظ���ͬ��ѶϢ��\n");
        // �ݲ�log, ������̫��
        // log_file("channel/CHANNELS", sprintf("%s use %s: %s\n", me->query("id"), origin_verb, origin_msg) );
    }
 
    // Make the identity of speaker.

    if( channels[verb]["anonymous"] && (!userp(me) || wizardp(me) || random(CHANCE)))
        who = channels[verb]["anonymous"];
    else {
		who = me->query("channel_id");
		if( ! stringp(who) )
	    	who = sprintf("%s(%s)", me->name(1), capitalize(me->query("id")));
    }

    // Ok, now send the message to those people listening us.
 
    ob = filter_array( users(), "filter_listener", this_object(), channels[verb] );
    if( !arg ) arg = "...";
 
    if( emote ) {
        string msg;

        if( !channels[verb]["msg_emote"] )
            return notify_fail("���Ƶ������ emote��\n");
        // Support of old behavier of intermud emote.
        if( channels[verb]["intermud_emote"] ) arg = who + " " + arg;
 
        msg = sprintf( channels[verb]["msg_emote"], arg );
	if( strsrch(msg, '\n')==-1 ) msg += "\n";
        message( "channel:" + verb, msg, ob );
    } else {
        string msg = sprintf( channels[verb]["msg_speak"], who, arg );
	if( strsrch(msg, '\n')==-1 ) msg += "\n";
        message( "channel:" + verb, msg, ob );
#if 0
        // Log channel messages for extensions. By Annihilator (12/29/1999)
        if( undefinedp(channels[verb]["no_log"]) )
            log_file("channel/" + verb, msg);
#endif
    }
     
    //��鿴˭��ҥ��
        if (channels[verb]["anonymous"])
        {
                string msg;
                if (userp(me))  who = me->query("name")+"("+capitalize(me->query("id"))+")";
                msg = replace_string(arg, channels["rumor"]["anonymous"], who);
                msg = sprintf(channels["sys"]["msg_speak"], who, arg);
                message( "channel:sys", msg,
                         filter_array(ob, "filter_listener", this_object(),channels["sys"]));

        }

    if( arrayp(channels[verb]["extra_listener"]) ) {
        channels[verb]["extra_listener"] -= ({ 0 });
        if( sizeof(channels[verb]["extra_listener"]) )
            channels[verb]["extra_listener"]->relay_channel(me, verb, arg);
    }
    
    if( !undefinedp(channels[verb]["intermud"])
    &&	me != load_object(channels[verb]["intermud"]) )
        channels[verb]["intermud"]->send(
	    channels[verb]["channel"], me->query("id"), me->name(1), arg, emote,
	    channels[verb]["filter"] );
 
    if( userp(me) ) 
        me->set_temp("last_channel_msg", verb + "\n" + arg);
 
    return 1;
}
 
int filter_listener(object ppl, mapping ch)
{
    // Don't bother those in the login limbo.
    if( !environment(ppl) ) return 0;
    
    if( ch["wiz_only"] ) return wizardp(ppl);
    return 1;
}

// register_relay_channel()
//
// This function allows non-user objects like npcs, daemons, etc to sign-up
// a channel for listenin. The way non-user objects listening channels is
// via a mudlib apply function relay_channel().
 
void register_relay_channel(string channel)
{
    object ob;
 
    ob = previous_object();
    if( undefinedp(channels[channel]) || !ob) return;
    if( arrayp(channels[channel]["extra_listener"]) ) {
        if( member_array(ob, channels[channel]["extra_listener"]) >=0 ) return;
        channels[channel]["extra_listener"] += ({ ob });
    } else
        channels[channel]["extra_listener"] = ({ ob });
}
