inherit ROOM;

void create()
{
	set("short", "��ħ����");
	set("long", @LONG
��������������һ�ɼ�̵İ�ħ��������������������ﳩ��
���ԣ����Բ����ܵ����׼������벿����в��
LONG
	);
	set("exits", ([
		"west" : __DIR__"guildhall",
		"east" : ([ "filename" : __DIR__"wizard.c",
					 "x_axis" : 13,
					 "y_axis" : 5,
				  ]),
	]));

	setup();
	load_object("/daemon/board/propose");
	replace_program(ROOM);
}