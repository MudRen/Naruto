#include <npc.h>

inherit F_VENDOR;

void create()
{
    set_name("����ʦ", ({ "armor master", "master" }));
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

    set_goods(1, "eq", 1, 10);
    set_goods(2, "eq", 2, 10);
    set_goods(3, "eq", 3, 10);
    set_goods(4, "eq", 4, 10);
    set_goods(5, "eq", 5, 10);
    set_goods(6, "eq", 6, 10);
    set_goods(7, "eq", 7, 10);
    set_goods(8, "eq", 8, 10);
    set_goods(9, "eq", 9, 10);
    set_goods(10, "eq", 10, 10);
    set_goods(11, "eq", 11, 10);
    set_goods(12, "eq", 12, 10);
    set_goods(13, "eq", 13, 10);
    set_goods(14, "eq", 14, 10);
    set_goods(15, "eq", 15, 10);
    set_goods(16, "eq", 16, 10);
    set_goods(17, "eq", 17, 10);
    set_goods(18, "eq", 18, 10);
    set_goods(19, "eq", 19, 10);
    set_goods(20, "eq", 20, 10);
    set_goods(21, "eq", 21, 10);
    set_goods(22, "eq", 22, 10);
    set_goods(23, "eq", 23, 10);
    set_goods(24, "eq", 24, 10);
    set_goods(25, "eq", 25, 10);

    setup();
}
