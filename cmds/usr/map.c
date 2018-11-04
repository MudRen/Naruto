/* map.c */

#include <command.h>
#include <ansi.h>

inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

int main(object me, string arg)
{
	string file, text;
	object area;
	
	if( !(area = environment(me)) ) return 0;
	if( !area->is_area() ) return notify_fail("���ָ��ֻ���������򻷾��С�\n");
	
	// ����󣬷ֳ����Ҷ����ֵ�ͼ
	if( area->query("x_axis_size") > 49 ) {
		if( me->query("area_info/x_axis") <= 49 ) file = file_name(area) + "_left.ansi";
		else file = file_name(area) + "_right.ansi";
	} else file = file_name(area) + ".ansi";

    if( file_size(file) <= 0 ) return notify_fail("Ŀǰ�������û�е�ͼ����\n");
    
    if( !(text = read_file(file)) ) text = "";

    me->start_more(text);

	return 1;
}

int help(object me)
{
    write(@HELP
ָ���ʽ�� map

���ָ���������鿴���򻷾���ȫͼ��ֻ���������򻷾��У�һ��
���价���޷�ʹ�á�
HELP);
	return 1;
}