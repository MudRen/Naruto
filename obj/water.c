// water.c

inherit LIQUID_ITEM;
inherit F_DRINK;

void create()
{
	set_name("��ˮ", ({ "water" }));
	if( !clonep() ) {
		set("long", "... ˮ����ˮ�oûɶ�ÿ��ġ�\n");
		set("density", 1);
	}
	setup();
	set_volume(1000);
}
