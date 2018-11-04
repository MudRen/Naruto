/* area.c write by -Acme-
*/

#pragma optimize
#pragma save_binary

// �Ƚ϶�������Ƿ�����ͬ������������
int area_environment(object ob1, object ob2)
{
    if( environment(ob1) != environment(ob2) ) return 0;
    else {
        mapping info1, info2;
        if( !(info1 = ob1->query("area_info")) ) return 0;
        if( !(info2 = ob2->query("area_info")) ) return 0;
        if( info1["x_axis"] != info2["x_axis"] || info1["y_axis"] != info2["y_axis"] ) return 0;
    }
    return 1;
}

int area_move(object area, object who, int x, int y)
{
    mapping info;
    int env_area = 0;

    if( !objectp(who) || !objectp(area) ) return 0;

    if( environment(who) && environment(who)->is_area() ) {
        info = who->query("area_info");
        env_area = 1;
    }

    // ���who move_in���µ�area��ʧ��
    if( !area->move_in(x, y, who) ) return 0;

    who->set("area_info/x_axis", x);
    who->set("area_info/y_axis", y);
    // �ɹ�����area, move() ���Զ���area���Ƴ�
    if( who->move(area) ) {
        who->set("area_info/x_axis_old", x);
        who->set("area_info/y_axis_old", y);
        return 1;
    }
    // ����areaʧ�ܣ����ԭ������area�����У�����������һ��
    else {
        // ����ǰ�Ƶ��µ�area�����Ƴ�
        area->move_out(x, y, who);
        if( env_area ) {
            // �����ǰ�� x, y 
            who->set("area_info/x_axis", info["x_axis_old"]);
            who->set("area_info/y_axis", info["y_axis_old"]);
            if( !environment(who)->move_in(info["x_axis_old"], info["y_axis_old"]) ) {
                tell_object(who, "��Ϊĳ��ԭ����Ľ�ɫ�������ƶ�ʱ�����˴���...\n");
                destruct(who);
                return 0;
            }
        }
    }
    return 0;
}

// ��дmudos present��ʽ
object present(mixed arg, object ob)
{
    if( ob->is_area() ) {
        int p=0, index;
        object *obs, t;
        mapping area_info;

        area_info = this_player()->query("area_info");
    
        if( sscanf(arg, "%s %d", arg, index) != 2 ) index = 1;

        obs = ob->query_inventory(area_info["x_axis"], area_info["y_axis"]);

        foreach(t in obs) {
            if( !objectp(t) ) continue;
            if( t->id(arg) ) {
                p += 1;
                if( p == index ) return t;
            }
        }
        return 0;
    } else return efun::present(arg, ob);
}
