// write by -Acme-

#include <npc.h>

inherit F_FIGHTER;
inherit F_TRAINER;    // ѵ������

/* guild: muye   ľҶ���ߴ�
          yinyin �������ߴ�
          wuyin  �������ߴ�
          shayin ɰ�����ߴ�
          yuyin  �������ߴ�
          yunyin �������ߴ�
          caoyin �������ߴ�
          puyin  �������ߴ�
*/

void create()
{
    set_name("���ĳ�", ({ "sin sin zhang master", "master" }) );
    set_race("human");
    set_level(40);
    set_class("superior");    //����
    set("age", 35);
    set("title", "���ֵ�ʦ");
    set("long", "����һλ����ѧԺ��רƸ�Ľ�ʦ���̵�����һЩ�������ܣ��������\n"
                "�� train ָ����ѧϰһЩ�����ļ��ܡ�\n"
                "�� list ָ���ѯ����ѧϰ�ļ����б�\n");

    // �趨���ߴ壬��ͬ�����ߴ�Ż��, �趨 all �������ߴ�
    set("guild", "all");

    // �趨��ѵ���ļ���
    //
    // set_train(������, �Ƿ��ֱ��ϰ��, ѧϰ�ü�������Ҫ�Ľ�ɫ�ȼ�, �˼��ܿ�ѧϰ�������޵ȼ�)
    //
    // �����û�иü���ʱ����"�Ƿ��ֱ��ϰ��"����趨ֵΪ����ֵʱ�������ѵ����ѧϰʱ��ɼ�ס
    // ������ܵ�lv1������lv�ļ��ܲſ���Ͷ������������ʿ��������⼼��Ҫ���յõ�lv�󷽿�ѵ����
    //
    set_train("combat", 1, 1, 100);               // �񶷼���             F��
    set_train("dodge", 1, 1, 100);                // ���㼼��             F��
    set_train("parry", 1, 1, 100);                // ������             F��
    set_train("heavy hit", 1, 5, 100);            // �ػ�֮��             F��
    set_train("savage hit", 1, 5, 100);           // ����֮��             F��
    set_train("continual hit", 1, 5, 100);        // ����֮��             F��
    set_train("force", 1, 10, 100);               // ����                 F��
    set_train("ninja", 1, 10, 100);               // ����                 F��
    set_train("magic", 1, 10, 100);               // ����                 F��
/*
    set_train("alter ego", 0, 20, 100);           // ������������         
    set_train("substitute", 0, 15, 100);          // ������������         
    set_train("shadow", 0, 50, 100);              // ������Ӱ������       
    set_train("thunder cut", 0, 60, 100);         // ����������           S��
    set_train("reincarnation", 0, 80, 100);       // ����������ת��       S��
    set_train("moon dance", 0, 55, 100);          // ������������         A��
    set_train("wind blade", 0, 60, 100);          // ����������           A��
    set_train("snake kill", 0, 65, 100);          // ������˫����ɱ��     A��
    set_train("water-water fall", 0, 50, 100);    // ������ˮ�ݡ����ٲ��� A��
    set_train("water-water dragon", 0, 40, 100);  // ������ˮ�ݡ�ˮ������ B��
    set_train("plover", 0, 55, 100);              // ������ǧ��           A��
    set_train("soul dies", 0, 45, 100);           // ������������         B��
    set_train("puppet", 0, 35, 100);              // ������������         C��
    set_train("whirlwind", 0, 30, 100);           // ������������         C��
    set_train("fire-dragon fire", 0, 30, 100);    // ��������ݡ�������   C��
    set_train("fire-bold fireball", 0, 25, 100);  // ��������ݡ��������� C��
    set_train("fire-phoenix fire", 0, 20, 100);   // ��������ݡ����ɻ��� C��
    set_train("animal taming", 0, 10, 100);       // ������ѱ����         E��
    set_train("bad worm", 0, 40, 100);            // �������Ļ�����       -��
    set_train("sand burial", 0, 40, 100);         // ������ɰ������       -��
    set_train("sand tie", 0, 30, 100);            // ������ɰ����         -��
    set_train("sand armor", 0, 50, 100);          // ������ɰ����         -��
    set_train("sand shield", 0, 60, 100);         // ������ɰ��           -��

    set_train("inside lotus", 0, 60, 100);        // ������������         A��
    set_train("outside lotus", 0, 50, 100);       // ������������         B��
    set_train("shadow leaf", 0, 30, 100);         // ������Ӱ��Ҷ         C��
    set_train("lion attack", 0, 30, 100);         // ������ʨ������       C��
    set_train("muscle tank", 0, 25, 100);         // �������ⵯս��       C��
    set_train("thousand year kill", 0, 20, 100);  // ������ǧ��ɱ         E��
    set_train("double physique", 0, 30, 100);     // ������������         -
    set_train("sixty palms", 0, 50, 100);         // ������������ʮ����   -
    set_train("sky palm", 0, 40, 100);            // �����������ƻ���     -

    set_train("five elements seal", 0, 70, 100);  // ��ӡ�����з�ӡ��     A��
    set_train("five elements unseal", 0, 70, 100);// ��ӡ�����н�ӡ��     A��
*/
    setup();

    // ���ﱾ��ļ����趨
    set_skill("dodge", 200);               // ������ �رܼ���
    set_skill("parry", 200);               // ������ ��������
    set_skill("combat", 200);              // ������ �񶷼���
    set_skill("savage hit", 200);          // ������ ����
    set_skill("heavy hit", 200);           // ������ �ػ�
    set_skill("continual hit", 200);       // ������ ����
    set_skill("force", 180);               // ������ ����
    set_skill("ninja", 200);               // ������ ����
    set_skill("magic", 160);               // ������ ����
    set_skill("substitute", 180);          // ������ ������

    map_skill("dodge", "substitute");      // ʹ�ü��ܡ����������ڻرܼ�����
}