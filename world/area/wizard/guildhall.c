/* write by -Acme- */

#include <room.h>

// ���INN�ߣ����Զ���Ϊ��ս����������������
inherit INN;

void create()
{
    set("short", "��ʦ���");
    set("long", @LONG
����ס��һȺӵ�и߼�ħ������ʦ����ʦ��ӵ�е���������һ��
�˿��Բt��ģ�����Կ���ǰ�����ʦ�����㵸�أ�����ʱɢ����
�߲ʹ�â����֪��ʹʲôħ����
LONG
    );
    set("exits", ([
        "west" : __DIR__"bug",
        "north" : __DIR__"academy",
        "east" : __DIR__"propose",
        "up" : __DIR__"meetingroom",
        "south": __DIR__"power",
    ]));

    set("objects", ([
        __DIR__"npc/annihilator" : 1,
        __DIR__"npc/luky" : 1,
        __DIR__"npc/shengsan" : 1,
        __DIR__"npc/tmr" : 1, 
        __DIR__"npc/acme" : 1, 
    ]));

    setup();
    load_object("/daemon/board/wizard");
    replace_program(ROOM);
}