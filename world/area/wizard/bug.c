inherit ROOM;

void create()
{
	set("short", "��ħ����");
	set("long", @LONG
����������ܲ�����ǿ��ĺ�ħ�����������ƺ���Ϊ��նɱ����
�Ҵܵĳ���룬������ǰ�����ѻ���ɽ�����к����ɼ����ħ����
�����൱���á�
LONG
	);
	set("exits", ([
		"east" : __DIR__"guildhall",
	]));

	setup();
	load_object("/daemon/board/bug");
	replace_program(ROOM);
}
