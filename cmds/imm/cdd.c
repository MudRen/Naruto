// cd.c

inherit F_CLEAN_UP;

int main(object me, string arg)
{
        object env;
        string file,dir;
        int last;
        
        env = environment(me);
        file = base_name(env);
        last = strsrch( file,"/",-1);
        dir = file[0..last];
        me->set("cwd", dir);
        write(dir + "\n");
        return 1;
}

int help(object me)
{
        write(@HELP
ָ���ʽ : cdd

��Ŀǰ�ο���Ŀ¼����Ŀǰ���ڵص�����Ŀ¼��
HELP
        );
        return 1;
}
