/* bank.c write by -Acme */

inherit ROOM;

void init()
{
	add_action("do_withdraw", "withdraw");
	add_action("do_deposit", "deposit");
}

int do_withdraw(string arg)
{
    int amount;
    string money;
    object money_ob, bond;

    seteuid(getuid());

    if( !arg || sscanf(arg, "%d %s", amount, money)!=2 )
        return notify_fail("ָ���ʽ�swithdraw <����> <��������>��\n");

    if( amount < 0 )
        return notify_fail("�㲻�����������µĻ��ҡ�\n");

    if( amount > 30000) 
        return notify_fail("�㲻��һ����̫�ࡣ\n");    

    if( file_size("obj/money/" + money + ".c") < 0 )
	    return notify_fail("��Ҫ������һ��Ǯ�t\n");

    if( catch(money_ob = new("/obj/money/" + money)) ) return 0;

    money_ob->set_amount(amount);
    if( this_player()->query("bank") < money_ob->value() ) {
        destruct(money_ob);
        return notify_fail("��Ļ�ͷ��û����ô��Ǯ��\n");
    }

    if( !money_ob->move(this_player()) ) {
        destruct(money_ob);
        return notify_fail("�����ϴ���������\��Ǯ�o����һ��ɡ�\n");
    }

    // ���ʻ���Ǯ
    this_player()->add("bank", -money_ob->value());

    write("����������" + chinese_number(amount) + money_ob->query("base_unit") + money_ob->name() + "��\n");

	return 1;
}

int do_deposit(string arg)
{
    int amount;
    string money;
    object money_ob;

    seteuid(getuid());

    if( !arg || sscanf(arg, "%d %s", amount, money) != 2 )
        return notify_fail("ָ���ʽ�sdeposit <����> <��������>��\n");

    if( amount < 0 )
        return notify_fail("�㲻�ܴ��������µ�Ǯ�ҡ�\n");
        
    if( !money_ob = present(money + "_money", this_player()) )
        return notify_fail("������û������Ǯ�ҡ�\n");

    if( money_ob->query_amount() < amount )
        return notify_fail("������û����ô���" + money_ob->name() + "��\n");

    write("�㽫" + chinese_number(amount) + money_ob->query("base_unit") + money_ob->name() + "�������С�\n");

    // ����������
    this_player()->add("bank", money_ob->query("base_value") * amount);

    // ���ϵ�Ǯ��������
    money_ob->add_amount( -amount );

    if( money_ob->query_amount() <= 0 ) destruct(money_ob);

    this_player()->save_autoload();
    this_player()->save();

	return 1;
}