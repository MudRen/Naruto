#include <npc.h>

inherit F_VENDOR;

void create()
{
    set_name("ҩ��ʦ", ({ "pill master", "master" }));
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

    set_goods(1, "food", 1, -1);
    set_goods(2, "food", 2, -1);
    set_goods(3, "food", 3, -1);
    set_goods(4, "food", 4, -1);
    set_goods(5, "food", 5, -1);
    set_goods(6, "food", 6, -1);
    set_goods(7, "food", 7, -1);
    set_goods(8, "food", 8, -1);
    set_goods(9, "food", 9, -1);

    setup();
}
