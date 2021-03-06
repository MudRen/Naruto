/* vendor.c 商人 */

#include <ansi.h>
#include <query.h>

mapping goods = ([]);

// 设定贩卖的物品
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
    	return this_object()->query("name") + "目前没有贩卖任何物品。\n";

    msg  = "╭──────────────────────────────────╮\n";
    msg += sprintf("│%-68s│\n", this_object()->query("name") + "目前的商品列表");
    msg += "├──────────────────────────────────┤\n";
    msg += "│编号    数量  单价/银两  名称                                       │\n";
    msg += "├──────────────────────────────────╯\n";

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

        msg += sprintf("│%4d %7s %10d  %-s\n", 
                       index,
                       ( goods[index]["amount_max"] == -1 ? "oo" : "" + goods[index]["amount"] ),
                       record["value"],
                       record["name"] );
    }

    msg += "├──────────────────────────────────╮\n";
    msg += "│输入 help trade 瞭解如何使用交易指令。                              │\n";
    msg += "╰──────────────────────────────────╯";

    return msg;
}

int to_sell(object who, int number, int amount)
{
    int value;
    object item;
    mapping record;

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

    // 非无限量供应之商品的数量检查
    if( !amount ) amount = 1;
    if( goods[number]["amount_max"] != -1 && goods[number]["amount"] < amount ) {
        tell_object(who, "这个商品数量没有这么多。\n");
        return 1;
    }

    switch( goods[number]["type"] ) {
        case "eq":   record = ITEM_D->do_query("eq", goods[number]["id"]); break;
        case "wp":   record = ITEM_D->do_query("wp", goods[number]["id"]); break;
        case "food": record = ITEM_D->do_query("food", goods[number]["id"]); break;
        case "obj": record = ITEM_D->do_query("obj", goods[number]["id"]); break;
        default:
            tell_object(who, "店家发现这个商品有问题后随即下架了，你无法购买。\n");
            // 或许档案已经不存在，从列表中删除
            map_delete(goods, number);
            return 1;
    }
    if( !mapp(record) ) {
        tell_object(who, "店家发现这个商品有问题后随即下架了，你无法购买。\n");
        // 或许档案已经不存在，从列表中删除
        map_delete(goods, number);
        return 1;
    }

    value = record["value"] * amount;

    // 银行钱不够
    if( who->query("bank") < value ) {
        tell_object(who, "你银行里的钱不够，无法购买此商品。\n");
        return 1;
    }

    // 无法建立该商品
    if( !objectp(item = new(record["file_name"])) ) {
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
    
    if( !item->move(who) ) {
        tell_object(who, "店家发现你似乎拿不动这样商品，便不卖你了。\n");
        // 删除物品
        destruct(item);
        return 1;
    }

    message_vision("$N花了" + chinese_number(value) + "两银子向" + this_object()->query("name") + "买下" +
                   chinese_number(amount) + "个" + record["name"] + "。\n", who);

    // 从银行扣钱
    who->add("bank", -value);

    // 非无限量供应的商品数量需改变
    if( goods[number]["amount_max"] != -1 )
        goods[number]["amount"] -= amount;

    return 1;
}

// 恢复商品的数量
void reset()
{
	int index, maximum;
	maximum = sizeof(goods);
    for(index=1;index<=maximum;index++) {
        if( undefinedp(goods[index]) ) continue;
        goods[index]["amount"] = goods[index]["amount_max"];
    }
}