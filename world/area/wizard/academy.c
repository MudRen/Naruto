
inherit ROOM;

void create()
{
	set("short", "��ʦѧԺ");
	set("long", @LONG
��������ʦ���о������������δ���ĳ���������Կ��������
ʦ���ֻ��ŵظ�̸���������ѵ��뷨����Ҳ����һͬ�������ǵ�����
���������������á�
LONG
	);
	set("exits", ([
		"south" : __DIR__"guildhall",
	]));

	setup();
	load_object("/daemon/board/design");
	replace_program(ROOM);
}