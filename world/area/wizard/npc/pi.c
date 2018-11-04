#include <npc.h>

inherit F_SOLDIER;

void create()
{
    set_name("崸��", ({ "dang ge pi", "pi" , "__GUARD__" }) );

    set_class("superior");  // ����
    set_level(55);

    set("title", "����֮ɭ����");

    set("long", "һ���ػ�������֮ɭ�����̣���ֹ��֪������󴳽�ȥ��Ҳ��������\n"
                "��������֮ɭ���Ұ�޹�����\n");

    // ��������
    set("guard_exit", ({"east", "northeast", "southeast"}));

    // �������
    set("vendetta_mark", "wizard");

    setup();

    // ���ﱾ��ļ����趨
    set_skill("dodge", 200);               // ������ �رܼ���
    set_skill("parry", 200);               // ������ ��������
    set_skill("combat", 200);              // ������ �񶷼���
    set_skill("savage hit", 200);          // ������ ����
    set_skill("heavy hit", 200);           // ������ �ػ�
    set_skill("force", 200);               // ������ ����
    set_skill("ninja", 10);                // ������ ����
    set_skill("magic", 10);                // ������ ����
}

int do_guard_exit(int x, int y, string exit)
{
	message_vision("$n���ֽ�$N��ס��\n", this_player(), this_object());
    do_chat((: command, "say ����ɭ�����Ұ���׶�úܣ�����ñ��ȥ��":));
    return 1;
}

int accept_fight(object ob)
{
    do_chat((: command, "say ���䣿�ҿ�û�գ���ȥ�ұ������԰ɡ�":));
    return 0;
}
