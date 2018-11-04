// villager.c

// prototypes
mixed do_chat(mixed chat);
varargs string name(int raw);
void kill_ob(object ob);
varargs int is_fighting(object ob);

int accept_fight(object ob)
{
	if( is_fighting() ) {
		do_chat( name() + "说道﹕哼 ... 倚多为胜不是好汉﹐待会儿再陪你玩﹗\n");
		return 0;
	}
	do_chat( name() + "说道﹕哈﹗看来你是活得不耐烦了﹐让我来教训教训你﹗\n");
	kill_ob(ob);
	return 1;
}

void accept_kill(object ob)
{
	do_chat( name() + "“哼”地一声﹐说道﹕那得瞧你有没有这个本事﹗\n");
}
