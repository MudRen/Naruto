// container.c

#include <ansi.h>

inherit ITEM;

void setup()
{
	if( !query("exits") )
		set("exits/out", (: environment :));
}

// This function allows the object ob to be moved into the inventory of
// this object by returning 1 on called.
int accept_object(object player, object ob) { return 1; }

// This function prevents players from getting objects from the inventory
// of this object by returning 1 on called.
int hold_object(object ob)
{
	return query("closed") != 0;
}

// This is the look command processor called when player is inside this 
// object.
int do_look(object me, string arg)
{
	string long;

	if( long = query("inside_long") ) write(long);
	else write("����������һ" + query("unit") + name() + "�\n");

	if( !query("closed") && query("exits/out") )
		write("Ҫ�뿪��������� " HIW + "go out" + NOR "��\n");

	if( query("transparent") ) {
		write("����������Կ������棺\n");
		environment()->do_look(me, arg);
	}

	return 1;
}
