/* food.c */

#include <dbase.h>
#include <name.h>

inherit COMBINED_ITEM;

int stuff_ob(object me)
{
	mapping condition;
    string con;

	if( !this_object()->query("condition") ) return 1;

    condition = this_object()->query("condition");

    // ��Ч״̬�趨
    foreach(con in keys(condition))
        CONDITION_D(con)->food_condition_apply(me, condition[con]);

    return 1;
}