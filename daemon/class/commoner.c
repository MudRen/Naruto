// commoner.c

void create()
{
	seteuid(getuid());
	DAEMON_D->register_class_daemon("commoner");
}

string query_rank(object ob, string politeness)
{
	switch(politeness) {
		case "self": return "小人";
		case "rude self": return "本大爷";
		case "respectful": return "大爷";
		case "rude": return "恶贼";
		default: return "平民";
	}
}

void initialize(object ob) { seteuid(geteuid(ob)); }
void setup(object ob) { seteuid(geteuid(ob)); }
int valid_wield(object me, object ob, string skill) { return 1; }
int valid_wear(object me, object ob, string part) { return 1; }