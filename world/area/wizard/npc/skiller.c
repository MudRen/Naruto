#include <npc.h>

inherit F_VENDOR;

void create()
{
    set_name("����ʦ", ({ "skill master", "master" }));
    set_race("human");
    set_level(50);
    set("age", random(50)+18);

    set("long", "�޼鲻���̣�ʮ��ļ��������ϼ����ˡ�\n");

    // �趨��������������Ʒ
    // set_goods(����, ����, ���, ����)
    // �����ܷ����Ķ���ֻ���� query ָ����� eq, wp, food �ܲ��������Ʒ
    // �� set_goods(1, "food", 1, 10) ��ʾ���� query ָ��������food���Ϊ1��ʳƷʮ��
    // �������趨 -1 ��ʾ��������Ӧ
    // ���������� buy ָ���ϣ��� buy 1 ������������´���Ϊ1����Ʒ

    setup();
}
