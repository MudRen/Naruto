// inn.c
// update by -Acme-

inherit ROOM;

void setup()
{
    ::setup();
    set("valid_startroom", 1);
    set("no_fight", 1);
    set("recoverroom", 1);   // ��ɫ�ڴ�room save�󣬽�ɫ�������ɴ�����
}