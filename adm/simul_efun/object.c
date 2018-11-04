/*  object.c

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

varargs int getoid(object ob)
{
    int id;

    if (!ob) ob = previous_object();
    sscanf(file_name(ob), "%*s#%d", id);
    return id;
}

// Get the owner of a file.  Used by log_error() in master.c.
string file_owner(string file)
{
    string name, rest, dir;

    if (file[0] != '/') file = "/" + file;
    if (sscanf(file, "/u/%s/%s/%s", dir, name, rest) == 3) {
        return name;
    }
    return 0;
}

/*  creator_file()
 *
 *  ��������� ES2 ��ȫϵͳ�ĺ��ģ�ÿ��һ������� load ��ʱ��ϵͳ��
 *  ���������ĵ������� simul_efun �е� creator_file() ������������
 *  ��������Ĵ���ֵ��Ϊ��һ����� uid���� ES2 �İ�ȫϵͳ�У������
 *  uid ��ʾ�����������Ȩ��( ������Ծ��� seteuid ����߻򽵵��Լ�
 *  ��Ȩ�� )��������Ȩ���ǵ���������ĵ��������ģ������Ҳ����˵ES2
 *  �İ�ȫϵͳ�Ǹ����ڵ�����дȨ�Ĺ����ϡ�
 *
 *  ES2 Ŀǰ�� uid �ķ��ŷ�ʽ��ο���ʽ���е�ע�⡣
 */
string creator_file(string file)
{
    string *path, euid;

    path = explode(file, "/") - ({ "" });
    if( ! sizeof(path) )
	return 0;	/* ������Ӧ�ò��ᷢ����ֻ��Ԥ����һ */

    switch(path[0])
    {
	// ----------------------------------------------------------
	// �� /adm �� /cmds �µ����ӵ����ߵ�Ȩ�� ROOT_UID����������
	// ���¡�
	// ----------------------------------------------------------
	case "adm":
	case "world":
	case "cmds":	return ROOT_UID;

	// -----------------------------------------------------------
	// �� /u �� /open �µ�����������ʹ�������룬����ʹ���ߵ���
	// ͬȨ�ޣ����������ʹ�������룬���ú��������������ͬ��Ȩ
	// �ޡ�
	// -----------------------------------------------------------
	case "u":
	case "open":
	    if( this_player(1) ) euid = getuid(this_player(1));
	    if( !euid ) euid = geteuid(previous_object(1));
	    if( euid && euid != ROOT_UID ) return euid;
            return "NONAME";

	// ------------------------------------------------------------
	// �� /obj �µ���������Ȩ��������������Ȩ��(euid)����������
	// ��˵�ɾ��� ROOT_UID Ȩ�޵��������� /obj ������;��� ROOT
	// Ȩ�ޣ���ʹ�������������;���ʹ���ߵ�Ȩ�ޡ�һ����� ROOT Ȩ
	// �޵�������Ҫ���� /obj �����ͨ������ seteuid ���ʵ���Ȩ��
	// �ٽ������롣
	// ------------------------------------------------------------
	case "obj":
	    if( previous_object(1) ) return geteuid(previous_object(1));
	    return "NONAME";

	// ------------------------------------------------------------
	// ����Ŀ¼�µ���������� MUDLIB_UID ��Ȩ��
	// ------------------------------------------------------------
	case "daemon":
	default:
	    return MUDLIB_UID;
    }
}

void destruct(object ob)
{
    string err;

    if( !ob ) return;

    err = catch(ob->remove());

    if( err ) log_file("destruct", sprintf("[%s] %O: %s", ctime(time()), ob, err));

    if( userp(ob) && (geteuid(previous_object())!=ROOT_UID) && (geteuid(previous_object())!=geteuid(ob)) ) {
		log_file("destruct", sprintf("[%s] %O: Destructed by %s denied\n", ctime(time()), ob, err));
		error("Destruction denied.\n");
    }

    // ����ʹ�� by -Acme-
    if( environment(ob) && environment(ob)->is_area() ) {
    	environment(ob)->move_out(ob->query("area_info/x_axis"),
    		                      ob->query("area_info/y_axis"), ob);
    }

    efun::destruct(ob);
}
