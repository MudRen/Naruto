// commoner.c

void create()
{
	seteuid(getuid());
	DAEMON_D->register_class_daemon("commoner");
}

string query_rank(object ob, string politeness)
{
	switch(politeness) {
		case "self": return "С��";
		case "rude self": return "����ү";
		case "respectful": return "��ү";
		case "rude": return "����";
		default: return "ƽ��";
	}
}

void initialize(object ob) { seteuid(geteuid(ob)); }
void setup(object ob) { seteuid(geteuid(ob)); }
int valid_wield(object me, object ob, string skill) { return 1; }
int valid_wear(object me, object ob, string part) { return 1; }