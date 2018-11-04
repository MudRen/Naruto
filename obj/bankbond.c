// bankbond.c

inherit ITEM;

int balance = 0;

void create()
{
	set_name("Ǯׯ����", ({ "bankbond" }));
	set_weight(1);
	if( !clonep() ) {
		set("unit", "��");
		set("long",
			"����һ��Ǯׯ�������ظ��ͻ���Ǯ�����Ľ����o������˫���Ļ�Ѻ��\n"
			"���ࡣ\n");
        set("no_sell", 1);
	        set("value", 1);
	}
	set("keep_this", 1);
	setup();
}

string query_autoload() { return balance + ""; }

void autoload(string param)
{
	if( sscanf(param, "%d", balance) )
		set("owner_id", geteuid(previous_object()));
}

void transact(int amount)
{
	if( environment()->query("bank_account") != balance ) return;

	balance = environment()->query("bank_account") + amount;
	environment()->set("bank_account", balance);
}

void set_balance(int b) { balance = b; environment()->set("bank_account", b); }
int query_balance() { return balance; }

string long(int raw)
{
	string str;
	int amount;

	str = ::long(raw);
	if( balance > 0 ) {
		str += "�����ܹ�����s";
		amount = balance;
	} else if( balance < 0 ) {
		str += "�����ڻ���ǷǮׯ�s";
		amount = - balance;
	}
	if( balance != 0 ) {
		if( amount >= 100 ) {
			str += chinese_number(amount/100) + "��";
			if( amount%100 ) str += "��";
		}
		if( amount%100!=0 )
			str += chinese_number(amount%100) + "��Ǯ��";
		str += "\n";
	}

	return str;	
}
