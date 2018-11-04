/* vendor.c ���� */

#include <ansi.h>
#include <query.h>

mapping goods = ([]);

// �趨��������Ʒ
void set_goods(int number, string type, int id, int amount)
{
    goods[number] = ([]);
    goods[number]["type"] = type;
    goods[number]["id"] = id;
    goods[number]["amount"] = amount;
    goods[number]["amount_max"] = amount;
}

string do_list()
{
	int index, maximum;
	string msg;
    mapping record;

    if( (maximum=sizeof(goods)) <= 0 )
    	return this_object()->query("name") + "Ŀǰû�з����κ���Ʒ��\n";

    msg  = "�q���������������������������������������������������������������������r\n";
    msg += sprintf("��%-68s��\n", this_object()->query("name") + "Ŀǰ����Ʒ�б�");
    msg += "������������������������������������������������������������������������\n";
    msg += "�����    ����  ����/����  ����                                       ��\n";
    msg += "�����������������������������������������������������������������������s\n";

    for(index=1;index<=maximum;index++) {
        if( undefinedp(goods[index]) ) continue;
        switch( goods[index]["type"] ) {
            case "eq":   record = ITEM_D->do_query("eq", goods[index]["id"]); break;
            case "wp":   record = ITEM_D->do_query("wp", goods[index]["id"]); break;
            case "food": record = ITEM_D->do_query("food", goods[index]["id"]); break;
            case "obj": record = ITEM_D->do_query("obj", goods[index]["id"]); break;
            default: continue;
        }
        if( !mapp(record) ) continue;

        msg += sprintf("��%4d %7s %10d  %-s\n", 
                       index,
                       ( goods[index]["amount_max"] == -1 ? "oo" : "" + goods[index]["amount"] ),
                       record["value"],
                       record["name"] );
    }

    msg += "�����������������������������������������������������������������������r\n";
    msg += "������ help trade �t�����ʹ�ý���ָ�                              ��\n";
    msg += "�t���������������������������������������������������������������������s";

    return msg;
}

int to_sell(object who, int number, int amount)
{
    int value;
    object item;
    mapping record;

    if( !objectp(who) ) return 0;

    if( amount <= 0 ) {
        tell_object(who, "����Ҫ�򼸸���Ʒ��\n");
        return 1;
    }

    // û�������Ʒ���
    if( undefinedp(goods[number]) ) {
        tell_object(who, "û�������Ʒ��š�\n");
        return 1;
    }

    // ����������Ӧ֮��Ʒ���������
    if( !amount ) amount = 1;
    if( goods[number]["amount_max"] != -1 && goods[number]["amount"] < amount ) {
        tell_object(who, "�����Ʒ����û����ô�ࡣ\n");
        return 1;
    }

    switch( goods[number]["type"] ) {
        case "eq":   record = ITEM_D->do_query("eq", goods[number]["id"]); break;
        case "wp":   record = ITEM_D->do_query("wp", goods[number]["id"]); break;
        case "food": record = ITEM_D->do_query("food", goods[number]["id"]); break;
        case "obj": record = ITEM_D->do_query("obj", goods[number]["id"]); break;
        default:
            tell_object(who, "��ҷ��������Ʒ��������漴�¼��ˣ����޷�����\n");
            // �������Ѿ������ڣ����б���ɾ��
            map_delete(goods, number);
            return 1;
    }
    if( !mapp(record) ) {
        tell_object(who, "��ҷ��������Ʒ��������漴�¼��ˣ����޷�����\n");
        // �������Ѿ������ڣ����б���ɾ��
        map_delete(goods, number);
        return 1;
    }

    value = record["value"] * amount;

    // ����Ǯ����
    if( who->query("bank") < value ) {
        tell_object(who, "���������Ǯ�������޷��������Ʒ��\n");
        return 1;
    }

    // �޷���������Ʒ
    if( !objectp(item = new(record["file_name"])) ) {
        tell_object(who, "��ҷ��������Ʒ��������漴�¼��ˣ����޷�����\n");
        // �������Ѿ������ڣ����б���ɾ��
        map_delete(goods, number);
        return 1;
    }
    
    if( !function_exists("set_amount", item) && amount > 1) {
        tell_object(who, "�������һ��ֻ����һ����\n");
        destruct(item);
        return 1;
    }

    item->set_amount(amount);
    
    if( !item->move(who) ) {
        tell_object(who, "��ҷ������ƺ��ò���������Ʒ���㲻�����ˡ�\n");
        // ɾ����Ʒ
        destruct(item);
        return 1;
    }

    message_vision("$N����" + chinese_number(value) + "��������" + this_object()->query("name") + "����" +
                   chinese_number(amount) + "��" + record["name"] + "��\n", who);

    // �����п�Ǯ
    who->add("bank", -value);

    // ����������Ӧ����Ʒ������ı�
    if( goods[number]["amount_max"] != -1 )
        goods[number]["amount"] -= amount;

    return 1;
}

// �ָ���Ʒ������
void reset()
{
	int index, maximum;
	maximum = sizeof(goods);
    for(index=1;index<=maximum;index++) {
        if( undefinedp(goods[index]) ) continue;
        goods[index]["amount"] = goods[index]["amount_max"];
    }
}