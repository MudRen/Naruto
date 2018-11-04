/*  corpse.c - the coprse object

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

inherit ITEM;

static int decayed = 0;
static int decay_interval = 120;

void create()
{
    set_name("����ʬ��", ({ "corpse" }) );
    set("long", "����һ������ʬ�塣\n");
    set("unit", "��" );
    setup();
}

void
start_decay(int interval)
{
    if( interval < 1 ) interval = 0;

    decay_interval = interval;

    remove_call_out("decay");
    decayed = 0;
    call_out("decay", decay_interval * 2);
}

int is_corpse() { return decayed < 2; }
int is_character() { return decayed < 1; }

string short() { return name() + "(" + capitalize(query("id")) + ")"; }

void decay()
{
    switch(++decayed) {
        case 1:
            say( query("name") + "��ʼ�����˩o����һ�����ŵĶ����\n" );
            switch(query("gender")) {
                case "male":
                    set_name("���õ���ʬ", ({ "corpse" }) );
                    break;
                case "female":
                    set_name("���õ�Ůʬ", ({ "corpse" }) );
                    break;
                default:
                    set_name("���õ�ʬ��", ({ "corpse" }) );
                    break;
            }
            set("long", "���ʬ����Ȼ�Ѿ�����������һ��ʱ���˩o��ɢ����һ�ɸ�ʬ��ζ����\n");
            call_out("decay", decay_interval);
            break;
        case 2:
            say( query("name") + "���紵Ǭ�˩o���һ�ߺ��ǡ�\n" );
            set_name("��\Ǭ�ĺ���", ({ "skeleton" }) );
            set("long", "�⸱�����Ѿ���������ܾ��ˡ�\n");
            call_out("decay", decay_interval);
            break;
        case 3:
            say( "һ��紵���o��" + query("name") + "���ɹǻҴ�ɢ�ˡ�\n" );
            destruct(this_object());
            break;
    }
}

int receive_object(object oob, int from_inventory) { return 1; }
