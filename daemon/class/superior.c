// Fighter class daemon.
void create()
{
	seteuid(getuid());
	DAEMON_D->register_class_daemon("superior");
}

string query_rank(object ob, string politeness)
{
	switch(politeness) {
		case "self": return "����";
		case "rude self": return "����";
		case "respectful": return "����";
		case "rude": return "����";
		default: return "����";
	}
}

void setup(object ob) {}

void initialize(object ob)
{
    int lv;
	seteuid(geteuid(ob));
	lv = ob->query_level();
	// �趨�´��������辭��
	if( lv < 60 ) {
		if( lv > 50 )
			ob->set_target_scor("alevel", lv-50);
		else ob->set_target_score("blevel", lv-40);
	    ob->set_target_score("combat exp", lv*lv*1000);
	} else ob->delete("target_score");
}

void advance_level(object ob)
{
    int lv;
	lv = ob->query_level();
	if( !lv ) return;
	
	// �趨�´��������辭��
	if( lv < 60 ) {
		if( lv > 50 )
			ob->set_target_scor("alevel", lv-50);
		else ob->set_target_score("blevel", lv-40);
	    ob->set_target_score("combat exp", lv*lv*1000);
	} else ob->delete("target_score");
	
	// �趨ѧϰ������Ǳ�ܵ��������Ե��������ܵ���;
	ob->add_point("learn", 100);
	ob->add_point("score", 150);
	ob->add_point("attribute", 4);
	ob->add_point("skill", 2);
}