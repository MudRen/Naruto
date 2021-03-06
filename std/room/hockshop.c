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

    // goods 不是一个物件
    if( !objectp(item) ) return 0;

    if( base_name(item)[0..6] != "/world/" ) return 0;

    // 商品列表已满
    if( sizeof(goods) >= GOODS_MAXIMUM ) {
        // 移除前半段的商品
        for(index=1;index<=GOODS_MAXIMUM/2;index++) {
            if( undefinedp(goods[index]) ) continue;
            map_delete(goods, index);
        }
    }

    amount = item->query_amount();
    if( !amount ) amount = 1;

    file = base_name(item);

    changed = item->query("changed");

    // 若货品是没有经过“改变”也就是货品的设定与原始档案设定相同，则
    // 检查是否与商品列表中的某商品相同，如果相同，即增加数量便可。
    if( !changed ) {
        for(index=1;index<=GOODS_MAXIMUM;index++) {
            if( undefinedp(goods[index]) ) continue;

            // 找到相同的档案名称，增加其数量
            if( goods[index]["file"] == file ) {
                goods[index]["amount"] += amount;
                destruct(item);
                return 1;
            }
        }
    }

    // 寻找可用的商品编号
    for(index=1;index<=GOODS_MAXIMUM;index++) {
        if( undefinedp(goods[index]) ) break;
    }

    // 编号超过当铺商品数量上限
    if( index > GOODS_MAXIMUM ) return 0;

    // 设定储存该商品的资料
    goods[index] = ([]);
    goods[index]["file"] = file;
    goods[index]["amount"] = amount;
    goods[index]["name"] = item->query("name");
    goods[index]["id"] = item->query("id");

    if( item->query("base_value") ) goods[index]["value"] = item->query("base_value");
    else goods[index]["value"] = item->query("value");

    if( item->query("wear_as") ) goods[index]["class"] = BBLU"防具"NOR;
    else if( item->query("wield_as") ) goods[index]["class"] = BRED"武器"NOR;
    else if( function_exists("stuff_ob", item) ) goods[index]["class"] = BGRN"食品"NOR;
    else goods[index]["class"] = BYEL"其它"NOR;

    if( item->query("base_unit") ) goods[index]["unit"] = item->query("base_unit");
    else if( item->query("unit") ) goods[index]["unit"] = item->query("unit");
    else goods[index]["unit"] = "个";


    // 经过修改过的物品，储存完整的物品资料
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
        tell_object(who, "你想要买几个商品？\n");
        return 1;
    }

    // 没有这个商品编号
    if( undefinedp(goods[number]) ) {
        tell_object(who, "没有这个商品编号。\n");
        return 1;
    }

    // 数量检查
    if( !amount ) amount = 1;
    if( goods[number]["amount"] < amount ) {
        tell_object(who, "这个商品数量没有这么多。\n");
        return 1;
    }
    
    value = goods[number]["value"] * amount;
    // 银行钱不够
    if( who->query("bank") < value ) {
        tell_object(who, "你银行里的钱不够，无法购买此商品。\n");
        return 1;
    }
    
    // 无法建立该商品
    if( !objectp(item = new(goods[number]["file"])) ) {
        tell_object(who, "店家发现这个商品有问题后随即下架了，你无法购买。\n");
        // 或许档案已经不存在，从列表中删除
        map_delete(goods, number);
        return 1;
    }
    
    if( !function_exists("set_amount", item) && amount > 1) {
        tell_object(who, "这个东西一次只能买一个。\n");
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
        tell_object(who, "店家发现你似乎拿不动这样商品，便不卖你了。\n");
        // 删除物品
        destruct(item);
        return 1;
    }

    message_vision("$N花了" + chinese_number(value) + "两银子向" + this_object()->query("short") + "买下" +
                   chinese_number(amount) + goods[number]["unit"] + goods[number]["name"] + "。\n", who);

    // 从银行扣钱
    who->add("bank", -value);

    // 商品数量改变
    goods[number]["amount"] -= amount;

    // 卖完了，从列表中删除
    if( goods[number]["amount"] <= 0 ) map_delete(goods, number);

    return 1;
}

string do_list()
{
    int index;
    string msg;
    
    if( !sizeof(goods) ) return "目前没有任何商品。\n";
    
    msg  = "╭──────────────────────────────────╮\n";
    msg += sprintf("│%-68s│\n", this_object()->query("short") + "目前的商品列表" );
    msg += "├──────────────────────────────────┤\n";
    msg += "│编号  类别    数量  单价/银两  名称                                 │\n";
    msg += "├──────────────────────────────────╯\n";
    for(index=1;index<=GOODS_MAXIMUM;index++) {
        if( undefinedp(goods[index]) ) continue;
        msg += sprintf("│%4d  %s %7d %10d  %-s\n", 
                       index,
                       goods[index]["class"],
                       goods[index]["amount"],
                       goods[index]["value"],
                       goods[index]["name"] + "(" + goods[index]["id"] + ")" );
    }
    msg += "├──────────────────────────────────╮\n";
    msg += "│输入 help trade 瞭解如何使用交易指令。                              │\n";
    msg += "╰──────────────────────────────────╯";
    
    return msg;
}

// 储存当铺资料的路径位置
string query_save_file() { return file_name(this_object()); }

// 这是当铺
int is_hockshop() { return 1; }

// 在reset()时才进行save()动作
//void reset() { save(); }

// 在remove()时才进行save()动作
int remove()
{
    save();
    return 1;
}

void setup()
{
    if( !restore() ) goods = ([]);
}