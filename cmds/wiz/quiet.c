// quiet command..

inherit F_CLEAN_UP;

int main(object me, string str)
{
        object ob;
        int i;

        seteuid(getuid(me));
        if(!str) return notify_fail("指令格式: quiet <使用者>\n");
        ob = LOGIN_D->find_body(str);
        if(!ob) return notify_fail("线上的使用者中没有这个人。\n");

	seteuid(geteuid(ob));
	if (ob->query("no_chat") == 1) {
		ob->set("no_chat", 0) ; 
		tell_object(ob, "巫师("+me->query("id")+")取消你的禁言\n");
		tell_object(me,"你取消了玩家("+ob->query("id")+")的禁言\n");
		}
	else {
		ob->set("no_chat", 1) ;
		tell_object(ob, "巫师("+me->query("id")+")禁止你使用公共频道\n");
                tell_object(me,"你启动玩家("+ob->query("id")+")的禁言\n");
		}
        return 1;
}

int help(object me)
{
write(@HELP
指令格式 : quiet <某人>

被禁言者再次使用quite 就可以取消禁言
此指令可让你(你)将某人禁止使用公用频道。
HELP
    );
    return 1;
}
