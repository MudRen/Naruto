#include <npc.h>

inherit F_VENDOR;

void create()
{
    set_name("���ʦ", ({ "weapon master", "master" }));
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

    set_goods(1, "wp", 1, 10);
    set_goods(2, "wp", 2, 10);
    set_goods(3, "wp", 3, 10);
    set_goods(4, "wp", 4, 10);
    set_goods(5, "wp", 5, 10);
    set_goods(6, "wp", 6, 10);
    set_goods(7, "wp", 7, 10);
    set_goods(8, "wp", 8, 10);
    set_goods(9, "wp", 9, 10);
    set_goods(10, "wp", 10, 10);
    set_goods(11, "wp", 11, 10);
    set_goods(12, "wp", 12, 10);

    setup();
}
