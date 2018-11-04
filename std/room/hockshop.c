/* hockshop.c
   write by -Acme-
*/

#pragma save_binary

#define GOODS_MAXIMUM 100

#include <dbase.h>
#include <ansi.h>

inherit F_SAVE;
inherit ROOM;

mapping goods;

mapping query_goods() { return goods; }

int to_stock(object item)
{
    int index, amount, changed;
    string file;

    // goods ����һ�����
    if( !objectp(item) ) return 0;

    if( base_name(item)[0..6] != "/world/" ) return 0;

    // ��Ʒ�б�����
    if( sizeof(goods) >= GOODS_MAXIMUM ) {
        // �Ƴ�ǰ��ε���Ʒ
        for(index=1;index<=GOODS_MAXIMUM/2;index++) {
            if( undefinedp(goods[index]) ) continue;
            map_delete(goods, index);
        }
    }

    amount = item->query_amount();
    if( !amount ) amount = 1;

    file = base_name(item);

    changed = item->query("changed");

    // ����Ʒ��û�о������ı䡱Ҳ���ǻ�Ʒ���趨��ԭʼ�����趨��ͬ����
    // ����Ƿ�����Ʒ�б��е�ĳ��Ʒ��ͬ�������ͬ��������������ɡ�
    if( !changed ) {
        for(index=1;index<=GOODS_MAXIMUM;index++) {
            if( undefinedp(goods[index]) ) continue;

            // �ҵ���ͬ�ĵ������ƣ�����������
            if( goods[index]["file"] == file ) {
                goods[index]["amount"] += amount;
                destruct(item);
                return 1;
            }
        }
    }

    // Ѱ�ҿ��õ���Ʒ���
    for(index=1;index<=GOODS_MAXIMUM;index++) {
        if( undefinedp(goods[index]) ) break;
    }

    // ��ų���������Ʒ��������
    if( index > GOODS_MAXIMUM ) return 0;

    // �趨�������Ʒ������
    goods[index] = ([]);
    goods[index]["file"] = file;
    goods[index]["amount"] = amount;
    goods[index]["name"] = item->query("name");
    goods[index]["id"] = item->query("id");

    if( item->query("base_value") ) goods[index]["value"] = item->query("base_value");
    else goods[index]["value"] = item->query("value");

    if( item->query("wear_as") ) goods[index]["class"] = BBLU"����"NOR;
    else if( item->query("wield_as") ) goods[index]["class"] = BRED"����"NOR;
    else if( function_exists("stuff_ob", item) ) goods[index]["class"] = BGRN"ʳƷ"NOR;
    else goods[index]["class"] = BYEL"����"NOR;

    if( item->query("base_unit") ) goods[index]["unit"] = item->query("base_unit");
    else if( item->query("unit") ) goods[index]["unit"] = item->query("unit");
    else goods[index]["unit"] = "��";


    // �����޸Ĺ�����Ʒ��������������Ʒ����
    if( changed ) {
        mapping dbase = item->query_entire_dbase();
        if( mapp(dbase) )
            goods[index]["dbase"] = dbase;
    }

    destruct(item);

    return 1;
}

int to_sell(object who, int number, int amount)
{
    int value;
    object item;

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

    // �������
    if( !amount ) amount = 1;
    if( goods[number]["amount"] < amount ) {
        tell_object(who, "�����Ʒ����û����ô�ࡣ\n");
        return 1;
    }
    
    value = goods[number]["value"] * amount;
    // ����Ǯ����
    if( who->query("bank") < value ) {
        tell_object(who, "���������Ǯ�������޷��������Ʒ��\n");
        return 1;
    }
    
    // �޷���������Ʒ
    if( !objectp(item = new(goods[number]["file"])) ) {
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
    
    if( !undefinedp(goods[number]["dbase"]) ) {
        string *key;
        int j;
        key = keys(goods[number]["dbase"]);
        j = sizeof(key);
        while(j--) item->set(key[j], goods[number]["dbase"][key[j]]);
        item->delete("equipped");
    }

    if( !item->move(who) ) {
        tell_object(who, "��ҷ������ƺ��ò���������Ʒ���㲻�����ˡ�\n");
        // ɾ����Ʒ
        destruct(item);
        return 1;
    }

    message_vision("$N����" + chinese_number(value) + "��������" + this_object()->query("short") + "����" +
                   chinese_number(amount) + goods[number]["unit"] + goods[number]["name"] + "��\n", who);

    // �����п�Ǯ
    who->add("bank", -value);

    // ��Ʒ�����ı�
    goods[number]["amount"] -= amount;

    // �����ˣ����б���ɾ��
    if( goods[number]["amount"] <= 0 ) map_delete(goods, number);

    return 1;
}

string do_list()
{
    int index;
    string msg;
    
    if( !sizeof(goods) ) return "Ŀǰû���κ���Ʒ��\n";
    
    msg  = "�q���������������������������������������������������������������������r\n";
    msg += sprintf("��%-68s��\n", this_object()->query("short") + "Ŀǰ����Ʒ�б�" );
    msg += "������������������������������������������������������������������������\n";
    msg += "�����  ���    ����  ����/����  ����                                 ��\n";
    msg += "�����������������������������������������������������������������������s\n";
    for(index=1;index<=GOODS_MAXIMUM;index++) {
        if( undefinedp(goods[index]) ) continue;
        msg += sprintf("��%4d  %s %7d %10d  %-s\n", 
                       index,
                       goods[index]["class"],
                       goods[index]["amount"],
                       goods[index]["value"],
                       goods[index]["name"] + "(" + goods[index]["id"] + ")" );
    }
    msg += "�����������������������������������������������������������������������r\n";
    msg += "������ help trade �t�����ʹ�ý���ָ�                              ��\n";
    msg += "�t���������������������������������������������������������������������s";
    
    return msg;
}

// ���浱�����ϵ�·��λ��
string query_save_file() { return file_name(this_object()); }

// ���ǵ���
int is_hockshop() { return 1; }

// ��reset()ʱ�Ž���save()����
//void reset() { save(); }

// ��remove()ʱ�Ž���save()����
int remove()
{
    save();
    return 1;
}

void setup()
{
    if( !restore() ) goods = ([]);
}