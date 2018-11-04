/* train.c ѵ������ָ�� by -Acme- */

inherit F_CLEAN_UP;

int do_train(object me, object trainer, string skill, int count)
{
	if( !objectp(me) || !objectp(trainer) ) return notify_fail("����û������ˡ�\n");
	
    // ����ѵ��ʦ
	if( !trainer->is_trainer() ) return notify_fail("������ƺ�������㡣\n");
	
    // ����������򻷾��У���ͬ���꣬�����ж���
	if( environment(me)->is_area() )
		if( !area_environment(me, trainer) ) return notify_fail("����û������ˡ�\n");

    // all: �������ߴ�, �����Ƿ���ͬ�����ߴ�
    if( trainer->query("guild") != "all" &&  trainer->query("guild") != me->query("guild") )
        return notify_fail("�Է��ƺ�������㡣\n");
	
	return trainer->train_skill(me, skill, count);
}

int main(object me, string arg)
{
	int count;
	object trainer;
	string skill, who;
	
	if( !arg ) return notify_fail("������˭�ֽ̲�ѵ����ļ��ܣ�\n");
	
	if( sscanf(arg, "%s from %s for %d", skill, who, count) == 3 ) {
        if( !objectp(trainer = present(who, environment(me))) )
            return notify_fail("��ĸ���û������ˡ�\n");
	} else if( sscanf(arg, "%s from %s", skill, who) == 2 ) {
        if( !objectp(trainer = present(who, environment(me))) )
            return notify_fail("��ĸ���û������ˡ�\n");
		count = 1;
	}

    if( count <= 0 ) count = 1;

    return do_train(me, trainer, skill, count);
}

int help()
{
    write(@TEXT
ָ���ʽ�strain <��������> from <����> [for <����>]

���ָ�������ĳ���ֽ̲�ѵ����ļ��ܣ������Է�Ը��Ը����㣬
����������Ծ����ġ�һ�����ǲ������׵Ľ̵������书��������Ը
��̵���Ҳͨ�������������ڡ�
TEXT);
    return 1;
}
