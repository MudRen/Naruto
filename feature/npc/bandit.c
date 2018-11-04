// villager.c

// prototypes
mixed do_chat(mixed chat);
varargs string name(int raw);
void kill_ob(object ob);
varargs int is_fighting(object ob);

int accept_fight(object ob)
{
	if( is_fighting() ) {
		do_chat( name() + "˵���s�� ... �ж�Ϊʤ���Ǻú��o�������������u\n");
		return 0;
	}
	do_chat( name() + "˵���s���u�������ǻ�ò��ͷ��˩o��������ѵ��ѵ��u\n");
	kill_ob(ob);
	return 1;
}

void accept_kill(object ob)
{
	do_chat( name() + "���ߡ���һ���o˵���s�ǵ�������û��������©u\n");
}
