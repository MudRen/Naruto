// Fighter class daemon.
void create()
{
	seteuid(getuid());
	DAEMON_D->register_class_daemon("inferior");
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

    // ����ֻ������lv20
    if( lv < 20 ) {
		// ����ʮ������Ҫ�����E������
    	if( lv > 10 ) ob->set_target_score("elevel", lv-10);
        ob->set_target_score("combat exp", lv*lv*100);
    } else ob->delete("target_score");
}

void advance_level(object ob)
{
    int lv;
	lv = ob->query_level();
	if( !lv ) return;
	
	// �趨�´��������辭��
	if( lv < 20 ) {
		// ����ʮ������Ҫ�����E������
    	if( lv > 10 ) ob->set_target_score("elevel", lv-10);
	    ob->set_target_score("combat exp", lv*lv*100);
	} else ob->delete("target_score");
	
	// �趨ѧϰ������Ǳ�ܵ��������Ե��������ܵ���;
	ob->add_point("learn", 100);
	ob->add_point("score", 50);
	ob->add_point("attribute", 2);
	ob->add_point("skill", 2);
}