// Fighter class daemon.
void create()
{
	seteuid(getuid());
	DAEMON_D->register_class_daemon("monarch");
}

string query_rank(object ob, string politeness)
{
	switch(politeness) {
		case "self": return "����";
		case "rude self": return "����";
		case "respectful": return "����";
		case "rude": return "����";
		default: return "Ӱ��";
	}
}

void setup(object ob) {}

void initialize(object ob)
{
    int lv;
	seteuid(geteuid(ob));
	lv = ob->query_level();
	// �趨�´��������辭��
	if( lv < 99 ) ob->set_target_score("combat exp", lv*lv*3000);
    else ob->delete("target_score");
}

void advance_level(object ob)
{
    int lv;
	lv = ob->query_level();
	if( !lv ) return;
	
	// �趨�´��������辭��
	if( lv < 99 ) ob->set_target_score("combat exp", lv*lv*3000);
    else ob->delete("target_score");

	// �趨ѧϰ������Ǳ�ܵ��������Ե��������ܵ���;
	ob->add_point("learn", 100);
	ob->add_point("score", 100);
	ob->add_point("attribute", 2);
	ob->add_point("skill", 2);
}