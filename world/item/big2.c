/* poker table

    �ӿ��� - ���϶�
	
    ������
    clear(�����ƾ�)	0-δ���ƾ�, 1-�ƾ�����
    start(��ʼ��Ϸ)	0-δ��ʼ, 1-��ʼ
    focus(����Ȩ)	0-δ֪, 1-palyer1����, 2-player2����, 3-player3���ƣ�4-player4����
    play_kind(���Ʒ�ʽ) 0-����, 1-����, 2-����, 3-��«, 4-˳��, 5-��֧, 6-ͬ��˳, 7-��÷����
    owner		�����������
    power		���ӵĴ�С

						write by Acme@ED  2002.1.29 
*/

#include <ansi.h>

inherit ITEM;

mapping poker=([]);
mapping pok=([]);
mapping p1=([]);
mapping p2=([]);
mapping p3=([]);
mapping p4=([]);
mapping score=(["clear":0,"start":0,"focus":0,"play_kind":0,"owner":"","power":0,"person":0]);
mapping board=(["p1":0,"p2":0,"p3":0,"p4":0]);

/* ָ�ʽ */
int do_sit();
int do_reset();
int do_start();
int do_see(string arg);
int do_dump(string arg);
int do_pass();
int do_leave();
int do_rules();
int do_check();
int do_score();

string show_poker(mapping mm,int s);
string show_number(int i);
string show_flower(string str);
string show_score();
string focus_name();
int check_player(string name);	
int check_poker();
int check_over(string name);
void check_board(string winner);
string long();

void create()
{
        set_name("�����϶��� ����", ({ "poker table","table"}));
	set("long", long() );
        set("no_get", 1);
        setup();
}

void init()
{
	add_action("do_sit","sit");
	add_action("do_reset","reset");
	add_action("do_start","start");
	add_action("do_see",".");
	add_action("do_dump","du");
	add_action("do_pass","pa");
	add_action("do_check","ch");
	add_action("do_leave","leave");
	add_action("do_rules","rules");
	add_action("do_score",",");
}

int do_sit()
{
	object me=this_player();
	if( score["start"] ) return notify_fail("�ƾ��Ѿ���ʼ�ˡ�\n");
	if( !check_player(me->query_id()) )
	{
		if( undefinedp(score["p1"]) ) {
			score["p1"]=me->query_id();
			message_vision("$N�����������ı���λ�ӡ�\n",me);
			board["p1"]=100;
			score["person"]+=1;
		}
		else if( undefinedp(score["p2"]) ) {
			score["p2"]=me->query_id();
			message_vision("$N�����������Ķ���λ�ӡ�\n",me);
			board["p2"]=100;
			score["person"]+=1;
		}
		else if( undefinedp(score["p3"]) ) {
			score["p3"]=me->query_id();
			message_vision("$N�������������ϱ�λ�ӡ�\n",me);
			board["p3"]=100;
			score["person"]+=1;
		}
		else if( undefinedp(score["p4"]) ) {
			score["p4"]=me->query_id();
			message_vision("$N����������������λ�ӡ�\n",me);
			board["p4"]=100;
			score["person"]+=1;
		}
		else return notify_fail("�����Ѿ������ˡ�\n");
	}
	else return notify_fail("���Ѿ������������ˡ�\n");
	return 1;
}

int do_reset()
{
	int i;

	if( !check_poker() )
	{
		message_vision("�ɾ��޷�������$N���ƾ������趨...",this_player());
		score=([]);
	}
	else if( !check_player(this_player()->query_id()) )
		return notify_fail("�㲢û�в�����ε��ƾ֡�\n");

	/* ������� */
	score["clear"] = 1;
	score["start"] = 0;
	score["focus"] = 0;
	score["power"] = 0;
	score["owner"] = "";
	score["play_kind"] = 0;

	p1=([]);
	p2=([]);
	p3=([]);
	p4=([]);
	poker["m"]=([]);
	poker["f"]=([]);
	poker["h"]=([]);
	poker["t"]=([]);

	/* �� 52 ���ƣ����趨��С */
	for(i=1;i<=13;i++)
	{
		if( i == 2 ) {		/* �� 2 ��� */
			poker["m"][i]=49;
			poker["f"][i]=50;
			poker["h"][i]=51;
			poker["t"][i]=52;
		}
		else if( i == 1 ) {	/* A �ڶ��� */
			poker["m"][i]=45;
			poker["f"][i]=46;
			poker["h"][i]=47;
			poker["t"][i]=48;
		} else {		/* �����ư���ʽ����С */
			poker["m"][i]=i-2+(i-3)*3;
			poker["f"][i]=i-1+(i-3)*3;
			poker["h"][i]=i+(i-3)*3;
			poker["t"][i]=i+1+(i-3)*3;
		}
	}

	message_vision("$N��������ϴ��һ�Ρ�\n",this_player());
	return 1;
}

int do_start()
{
	int i, point,value;
	string kind;

	if( !check_player(this_player()->query_id()) )
		return notify_fail("�㲢û�в�����ε��ƾ֡�\n");

	if( !check_poker() )
		return notify_fail("�������㡢�����������ڷ��������ߣ��޷���ʼ��\n");

	if( !score["clear"] ) return notify_fail("��û��ϴ�ơ�\n");
	if( score["start"] ) return notify_fail("�Ѿ���ʼ�ˡ�\n");

	/* �������� */
	point=1;
	for(i=1;i<=52;i++)
	{
		/* ��ɫ����ʱ��ɾ����ɫ */
		if( sizeof(poker["m"]) < 1 ) map_delete(poker,"m");
		if( sizeof(poker["f"]) < 1 ) map_delete(poker,"f");
		if( sizeof(poker["h"]) < 1 ) map_delete(poker,"h");
		if( sizeof(poker["t"]) < 1 ) map_delete(poker,"t");
		
		/* ����ȡ�� */
		kind=keys(poker)[random(sizeof(keys(poker)))];
		value=keys(poker[kind])[random(sizeof(values(poker[kind])))];
		
		switch( point )
		{
			case 1:
				if( undefinedp(p1[kind]) ) p1[kind]=([]);
				p1[kind][value]=poker[kind][value];
				break;
			case 2:
				if( undefinedp(p2[kind]) ) p2[kind]=([]);
				p2[kind][value]=poker[kind][value];
				break;
			case 3:
				if( undefinedp(p3[kind]) ) p3[kind]=([]);
				p3[kind][value]=poker[kind][value];
				break;
			case 4:
				if( undefinedp(p4[kind]) ) p4[kind]=([]);
				p4[kind][value]=poker[kind][value];
				break;
		}

		map_delete(poker[kind],value);

		if( kind == "m" && value==3 ) {
			score["focus"]=point;
			/* ��������������÷�����ߣ���һ���� */
			if( score["person"]==3 ) point--;
		}
		point++;
		if( score["person"]==3 && point > 3 ) point =1;
		else if( point>4 ) point=1;
	}
	kind=focus_name();
	message_vision(HIW"$N����ϴ��ϴ������"+chinese_number(score["person"])+"��... ÷������"+kind+"���ϣ���"+kind+"���г��ơ�\n"NOR,this_player());
	score["start"] = 1;
	score["play_kind"] = 7;
	return 1;
}

int do_see(string arg)
{
	if( !check_player(this_player()->query_id()) )
		return notify_fail("�㲢û�в�����ε��ƾ֡�\n");

	if( !score["start"] ) 
		return notify_fail("�ƾֻ�û��ʼ��\n");

	/* ��ʦ����ר�ã�������Ҫ�����������߰ɣ�...:P
	   ��Ȼÿ�θ�����涼���֣��񲻶�����....Hehe....	
	 */
	if( arg && !wizardp(this_player()) )
		return notify_fail("�㲻��͵�����˵��ƣ�\n");
	if( !arg ) arg=this_player()->query_id();

	switch( check_player(arg) )
	{
		case 1:	write( "��"+score["p1"]+"��"+show_poker(p1,1) ); break;
		case 2: write( "��"+score["p2"]+"��"+show_poker(p2,1) ); break;
		case 3:	write( "��"+score["p3"]+"��"+show_poker(p3,1) ); break;
		case 4:	write( "��"+score["p4"]+"��"+show_poker(p4,1) ); break;
		default: return notify_fail(arg+"û�м������ƾ֡�\n"); break;
	}
	return 1;
}

int do_dump(string arg)
{
	int i, len, p, num, d=0;
	string flo;
	mapping p_num=([]), p_flo=([]);

	if( !arg ) return notify_fail("��Ҫ�������ƣ�\n");

	if( !check_player(this_player()->query_id()) )
		return notify_fail("�㲢û�в�����ε��ƾ֡�\n");

	if( !score["start"] ) 
		return notify_fail("�ƾֻ�û��ʼ��\n");

	if( (flo=focus_name())=="??" )
		return notify_fail("���ƾֻ�û���衢��δ���ơ�\n");

	if( flo != this_player()->query_id() )
		return notify_fail("�������ֵ�"+flo+"���ƣ���û���㣡\n");

	len=strlen(arg);
	if( len > 19 ) return notify_fail("�㶪̫�����ˣ����ֻ�ܶ������ƣ�\n");

	/* ץȡ������ */
	for(i=0;i<len;i++)
	if( arg[i..i] == " " || i==len-1 ) {

		if( i!=len-1 ) {
			if( sscanf(arg[p..i-1], "%d%s", num, flo) != 2 )
				return notify_fail("�㶪����������Ŷ��\n");
		} else {
			if( sscanf(arg[p..len], "%d%s", num, flo) != 2 )
				return notify_fail("�㶪����������Ŷ��\n");
		}

		if( num < 1 || num > 13 )
			return notify_fail("�㶪���ƴ�С���ˣ�\n");
		if( flo != "t" && flo != "h" && flo != "f" && flo != "m" )
			return notify_fail("�㶪���ƻ�ɫ������Ŷ��\n");

		/* ��鶪���������ظ� */
		if( !undefinedp(p_flo[flo]) && !undefinedp(p_flo[flo][num]) )
			return notify_fail("�㶪�����ظ���Ŷ��\n");

		/* �ж��������޴���..&& ���ƶ���mapping������Ժ��ж�
		   && �趨��С
		*/
		switch( score["focus"] ) {
			case 1:
				if( undefinedp(p1[flo]) || undefinedp(p1[flo][num]) )
					return notify_fail("������û��"+show_flower(flo)+show_number(num)+"�����ơ�\n");
				if(undefinedp(p_num[num])) p_num[num]=([]);
				if(undefinedp(p_flo[flo])) p_flo[flo]=([]);
				p_num[num][flo]=p1[flo][num];
				p_flo[flo][num]=p1[flo][num];
				break;
			case 2:
				if( undefinedp(p2[flo]) || undefinedp(p2[flo][num]) )
					return notify_fail("������û��"+show_flower(flo)+show_number(num)+"�����ơ�\n");
				if(undefinedp(p_num[num])) p_num[num]=([]);
				if(undefinedp(p_flo[flo])) p_flo[flo]=([]);
				p_num[num][flo]=p2[flo][num];
				p_flo[flo][num]=p2[flo][num];
				break;
			case 3:
				if( undefinedp(p3[flo]) || undefinedp(p3[flo][num]) )
					return notify_fail("������û��"+show_flower(flo)+show_number(num)+"�����ơ�\n");
				if(undefinedp(p_num[num])) p_num[num]=([]);
				if(undefinedp(p_flo[flo])) p_flo[flo]=([]);
				p_num[num][flo]=p3[flo][num];
				p_flo[flo][num]=p3[flo][num];
				break;
			case 4:
				if( undefinedp(p4[flo]) || undefinedp(p4[flo][num]) )
					return notify_fail("������û��"+show_flower(flo)+show_number(num)+"�����ơ�\n");
				if(undefinedp(p_num[num])) p_num[num]=([]);
				if(undefinedp(p_flo[flo])) p_flo[flo]=([]);
				p_num[num][flo]=p4[flo][num];
				p_flo[flo][num]=p4[flo][num];
				break;
		}	
		d++;
		p=i+1;
	}
		
	/* �ƾ�һ��ʼ���붪��÷���� */
	if( score["play_kind"]==7 && ( undefinedp(p_flo["m"]) || undefinedp(p_flo["m"][3]) ) )
		return notify_fail("�ƾ�һ��ʼ���붪��÷������\n");

	if( score["owner"] == this_player()->query_id() ) {
		score["power"] = 0;
		score["play_kind"] = 0;
	}

	switch( d ) {
		case 1: /* ���� */
			if( score["play_kind"] != 1 && score["play_kind"] != 0 && score["play_kind"] != 7 )
				return notify_fail("���ڲ���������ʱ��\n");

			/* check power */
			p=0;
			foreach( flo in keys(p_flo) )
			foreach( i in keys(p_flo[flo]) )
				p=p_flo[flo][i];
			if( p < score["power"] ) 
				return notify_fail("�㶪���Ʋ�����Ŷ��\n");

			score["play_kind"] = 1;
			score["power"]=p;
			message_vision("$N˵����"HIW"�� "NOR+YEL"������"HIW"��"NOR"\n",this_player());
			break;

		case 2: /* ���� */
			if( score["play_kind"] != 2 && score["play_kind"] != 0 && score["play_kind"] != 7 )
				return notify_fail("���ڲ��Ƕ����ӵ�ʱ��\n");

			/* check�Ƿ�Ϊ���� */
			if( sizeof( keys(p_num) ) > 1 )
				return notify_fail("��������ʱ�����趪���Ӳ��С�\n");

			/* check power */
			p=0;
			foreach( flo in keys(p_flo) )
			foreach( i in keys(p_flo[flo]) )
				if( p < p_flo[flo][i] ) p=p_flo[flo][i];
			if( p < score["power"] ) 
				return notify_fail("�㶪�Ķ��Ӳ�����Ŷ��\n");
			score["play_kind"] = 2;
			score["power"]=p;
			message_vision("$N˵����"HIW"�� "NOR+MAG"���ӣ�"HIW"��"NOR"\n",this_player());
			break;

		case 3: return notify_fail("û��������ϵ���Ŷ��\n"); break;
		case 4:	return notify_fail("û��������ϵ���Ŷ��\n"); break;

		case 5: 
			num=sizeof(keys(p_num));
			/* ˳�ӡ�ͬ��˳ */
			if( num == 5) {
				int *check, x, y, swap;
				check=keys(p_num);

				/* ���� */
			        for(x=4;x>0;x--)
		                for(y=1;y<=x;y++)
				if( check[y-1] < check[y] )
				{
					swap=check[y-1];
					check[y-1]=check[y];
					check[y]=swap;
                		}
				
				/* check�Ƿ�Ϊ˳��,1 10,11,12,13 Ҳ��˳�� */
				if( check[0]!=13 || check[1]!=12 || check[2]!=11 || check[3]!=10 || check[4]!=1 ) {
					y=check[0];
					for(x=1;x<5;x++) {
						if( y-check[x] != 1 )
							return notify_fail("û��������ϵ��ưɣ�(5)\n");
						y=check[x];
					}
				}

				/* check power 1~5��С */
				if( check[0]==5 && check[1]==4 && check[2]==3 && check[3]==2 && check[4]==1 ) {
					foreach( flo in keys(p_num[5]) )
						p=p_num[5][flo];
				} else {
					p=0;
					foreach( flo in keys(p_flo) )
					foreach( i in keys(p_flo[flo]) )
						if( p < p_flo[flo][i] ) p=p_flo[flo][i];
				}

				/* check �Ƿ�ͬ�� */
				num=0;
				foreach( flo in keys(p_flo) )
				if( num < sizeof(keys(p_flo[flo])) ) num = sizeof(keys(p_flo[flo]));

				if( num==5 ) {
					if( score["play_kind"] == 6 && p < score["power"] )
						return notify_fail("�㶪��ͬ��˳������Ŷ��\n");
					score["play_kind"]=6;
					message_vision("$N��Ц����"HIW"�� "HIY"�ۣ���������ͬ��˳��"HIW"��"NOR"\n",this_player());
				} else {
					if( score["play_kind"] != 4 && score["play_kind"] != 0 && score["play_kind"] != 7 )
						return notify_fail("���ڲ��ǳ�˳�ӵ�ʱ��\n");
					if( p < score["power"] ) 
						return notify_fail("�㶪��˳�Ӳ�����Ŷ��\n");
					score["play_kind"]=4;
					message_vision("$N�е���"HIW"�� "HIM"˳�ӣ�"HIW"��"NOR"\n",this_player());
				}
				score["power"]=p;

			/* ��֧����« */
			} else if( num == 2 ) {
				i=0; len=0; p=0;
				foreach( num in keys(p_num) )
				if( i < sizeof(keys(p_num[num])) ) {
					i=sizeof(keys(p_num[num]));
					len=num;
				}

				/* ��« */
				if( i == 3 ) {
					if( score["play_kind"] != 3 && score["play_kind"] != 0 && score["play_kind"] != 7 )
						return notify_fail("���ڲ��ǳ���«��ʱ��\n");
					foreach( flo in keys(p_num[len]) )
						if( p < p_num[len][flo] ) p = p_num[len][flo];
					if( score["power"] > p )
						return notify_fail("�㶪�ĺ�«������Ŷ��\n");
					score["play_kind"]=3;
					message_vision("$N������"HIW"�� "HIG"��«��"HIW"��"NOR"\n",this_player());
				/* ��֧ */
				} else if( i==4 ) {
					if( score["play_kind"] == 6 )
						return notify_fail("���ڲ��ǳ���֧��ʱ��\n");
					foreach( flo in keys(p_num[len]) )
						if( p < p_num[len][flo] ) p = p_num[len][flo];
					if( score["play_kind"] == 5 && score["power"] > p )
						return notify_fail("�㶪����֧������Ŷ��\n");
					score["play_kind"]=5;
					message_vision("$N����Ц����"HIW"�� "HIC"����������֧��"HIW"��"NOR"\n",this_player());

				} else return notify_fail("����������������Ŷ��\n");

				score["power"]=p;

			} else return notify_fail("û��������ϵ���Ŷ��\n"); break;

			break;
		default: return notify_fail("�㶪����̫�����ˣ�������š�\n"); break;
	}

	pok=p_flo;
	/* �������ƴ�������� */
	foreach( flo in keys(p_flo) )
	foreach( i in keys(p_flo[flo]) )
	switch( score["focus"] )
	{
		case 1: map_delete(p1[flo],i); break;
		case 2: map_delete(p2[flo],i); break;
		case 3: map_delete(p3[flo],i); break;
		case 4: map_delete(p4[flo],i); break;
	}
	score["owner"]=this_player()->query_id();

	message_vision(show_poker(p_flo,0),this_player());

	if( !check_over(this_player()->query_id()) ) {
		score["focus"] += 1;
		if( score["person"]==3 && score["focus"] > 3 ) score["focus"]=1;
		else if( score["focus"] > 4 ) score["focus"]= 1;
		message_vision("... �ֵ�"+focus_name()+"���ơ�\n",this_player());
	} else {
		message_vision("\n$N���˿��Ц����"HIW"�� "HIY"�ڹ磡�ںϣ��ںϣ��ںϣ� ��  Ӯ  �ˣ�����"HIW"��\n\n"NOR,this_player());

		check_board(this_player()->query_id());
		do_reset();
	}

	return 1;
}

int do_pass()
{
	string who;
	object me=this_player();

	if( !check_player(me->query_id()) )
		return notify_fail("�㲢û�в�����ε��ƾ֡�\n");

	if( !score["start"] ) 
		return notify_fail("�ƾֻ�û��ʼ��\n");

	if( (who=focus_name())=="??" )
		return notify_fail("���ƾֻ�û���衢��δ���ơ�\n");

	if( who != me->query_id() )
		return notify_fail("�������ֵ�"+who+"���ƣ���û���㣡\n");

	if( score["play_kind"]==7 )
		return notify_fail("��������Ҫ��һ��÷����������PassŶ��\n");

	if( score["owner"] == me->query_id() )
		return notify_fail("�㲻�ܹ�Pass�������㶪�����ưɡ�\n");

	score["focus"] += 1;
	if( score["person"] == 3 && score["focus"] > 3 )  score["focus"]=1;
	else if( score["focus"] > 4 ) score["focus"]= 1;

	message_vision("$N����˵����"HIW"�� "HIY"�У���"HIW"��"NOR"... �ֵ�"+focus_name()+"���ơ�\n",me);
	return 1;
}

int do_check()
{
	string msg;
	
	if( !check_player(this_player()->query_id()) )
		return notify_fail("�㲢û�в�����ε��ƾ֡�\n");

	if( !score["start"] ) 
		return notify_fail("�ƾֻ�û��ʼ��\n");

	if( focus_name()=="??" )
		return notify_fail("���ƾֻ�û���衢��δ���ơ�\n");

	if( sizeof(pok) < 1 )
		return notify_fail("Ŀǰ������û���ƣ�\n");

	msg = "��"+score["owner"]+"�������ƣ�";
	msg += show_poker(pok,0);
	msg += "��������"+focus_name()+"���ơ�\n";
	write(msg);
	return 1;
}

int do_leave()
{
	object me=this_player();

	if( !check_player(me->query_id()) )
		return notify_fail("�㲢û�в�����ε��ƾ֡�\n");

	if( score["p1"] == me->query_id() ) 
		map_delete(score,"p1");
	else if( score["p2"] == me->query_id() )
		map_delete(score,"p2");
	else if( score["p3"] == me->query_id() )
		map_delete(score,"p3");
	else {
		if( undefinedp(score["p4"]) ) return 0;
		if( score["p4"] == me->query_id() )
		map_delete(score,"p4");
	}

	message_vision("$N�뿪�����������ˡ�\n",me);
	score["person"]-=1;
	if( score["start"] ) do_reset();
	return 1;
}

int do_rules()
{
	write(@rules
	�X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[
	�U               �ӿ���-���϶� ��Ϸ����˵��	        �U
	�d�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�g
	�U  ��Ҷ�֪����... ���ô��� ccc..                      �U
	�U                                                      �U
	�U  ��Ҫע����ǣ�                                      �U
	�U       һ��û�С����ޡ�                               �U
	�U       ����û�С�������                               �U
	�U       ����˳��1~5����С��                            �U
	�U       �ġ�ֻҪ����һ����������û�Ƽ�Ӯ��             �U
	�U       �塢������Ҳ�����棬������÷�������˶�һ����   �U
	�U                                                      �U
	�U  �Ʒַ�ʽ��                                          �U
	�U       ���ƾֽ���������ʣ�����ƾͿۼ������         �U
	�U       �������ʮ����˶�������д��϶�Ҳ�˶�         �U
	�U       �����˿۵ĵ�����Ӹ�Ӯ���ˡ�                   �U
	�U                                                      �U
	�U                             write by -Acme@ED-       �U
	�^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a
rules);

	return 1;
}

int do_score()
{
	if( !score["start"] ) 
		return notify_fail("�ƾֻ�û��ʼ��\n");

	if( focus_name()=="??" )
		return notify_fail("���ƾֻ�û���衢��δ���ơ�\n");

	write(show_score()+"\n");

	return 1;
}

/* ��ʾĿǰ������֮���� */
string focus_name()
{
	if( undefinedp(score["focus"]) ) return "??";
	switch( score["focus"] )
	{
		case 1: if( !undefinedp(score["p1"]) ) return score["p1"]; break;
		case 2: if( !undefinedp(score["p2"]) ) return score["p2"]; break;
		case 3: if( !undefinedp(score["p3"]) ) return score["p3"]; break;
		case 4: if( !undefinedp(score["p4"]) ) return score["p4"]; break;
	}
	return "??";
}

/* ��ʾ���ϵ��� */
string show_poker(mapping mm,int s)
{
	int i, t;
	string msg="", msg2="",bar_up,bar_down, num;

	t=sizeof(mm["t"])+sizeof(mm["h"])+sizeof(mm["f"])+sizeof(mm["m"])-1;
	bar_up = "\n�X";
	bar_down = "�^";
	while( t-- ) {
		bar_up += "�T�j";
		bar_down += "�T�m";
	}
	bar_up += "�T�[\n";
	bar_down += "�T�a\n";

	/* ���մ�С���� */
	for(i=13;i>0;i--)
	{
		num=show_number(i);
		if( member_array("t",keys(mm))!=-1 && !undefinedp(mm["t"][i]) ) {
			msg+=sprintf("�U%s",num);
			msg2+="�U"+HIK"��"NOR;
		}
		if( member_array("h",keys(mm))!=-1 && !undefinedp(mm["h"][i]) ) {
			msg+=sprintf("�U%s",num);
			msg2+="�U"+HIR"��"NOR;
		}
		if( member_array("f",keys(mm))!=-1 && !undefinedp(mm["f"][i]) ) {
			msg+=sprintf("�U%s",num);
			msg2+="�U"+HIR"��"NOR;
		}
		if( member_array("m",keys(mm))!=-1 && !undefinedp(mm["m"][i]) ) {
			msg+=sprintf("�U%s",num);
			msg2+="�U"+HIK"÷"NOR;
		}
	}
	if( s ) {
		msg+="�U\tt->"HIK"����"NOR", h->"HIR"����"NOR"\n";
		msg2+="�U\tf->"HIR"����"NOR", m->"HIK"÷��"NOR"\n";
	}
	else {
		msg+="�U\n";
		msg2+="�U\n";
	}
	return bar_up+msg+msg2+bar_down;
}

/* ������ */
string show_number(int i)
{
	switch( i ) {
		case 1: return "��"; break;
		case 2: return HIW"��"NOR; break;
		case 3: return "��"; break;
		case 4: return "��"; break;
		case 5: return "��"; break;
		case 6: return "��"; break;
		case 7: return "��"; break;
		case 8: return "��"; break;
		case 9: return "��"; break;
		case 10: return "10"; break;
		case 11: return "��"; break;
		case 12: return "��"; break;
		case 13: return "��"; break;
	}
	return "??";
}

/* �㻨ɫ */
string show_flower(string str)
{
	switch( str ) {
		case "t": return "����"; break;
		case "h": return "����"; break;
		case "f": return "����"; break;
		case "m": return "÷��"; break;
	}
	return "??";
}

string show_score()
{
	string msg="";

	if( !undefinedp(board["p1"]) && !undefinedp(score["p1"]) )
		msg+=score["p1"]+"ĿǰΪ"+board["p1"]+"��  ";

	if( !undefinedp(board["p2"]) && !undefinedp(score["p2"]) )
		msg+=score["p2"]+"ĿǰΪ"+board["p2"]+"��  ";

	if( !undefinedp(board["p3"]) && !undefinedp(score["p3"]) )
		msg+=score["p3"]+"ĿǰΪ"+board["p3"]+"��  ";

	if( !undefinedp(board["p4"]) && !undefinedp(score["p4"]) )
		msg+=score["p4"]+"ĿǰΪ"+board["p4"]+"��  ";

	return msg;
}

/* ����Ƿ���������ǰ */
int check_player(string name)
{
	if( name == score["p1"] ) return 1;
	else if( name == score["p2"] ) return 2;
	else if( name == score["p3"] ) return 3;
	else {
		if( undefinedp(score["p4"]) ) return 0;
		if( name == score["p4"] ) return 4;
	}
	return 0; 
}

/* ����ƾ��Ƿ���Լ���ִ�� */
int check_poker()
{
	object user;
	if( !undefinedp(score["p1"]) && 
	    /* ����뿪�����߻򷢴����޷����� */
	    user = present(score["p1"], environment(this_object()))) {
		if(!interactive(user)) return 0;
		if(query_idle(user) > 180) return 0;
	}
	else return 0;

	if( !undefinedp(score["p2"]) && 
	    user = present(score["p2"], environment(this_object()))) {
		if(!interactive(user)) return 0;
		if(query_idle(user) > 180) return 0;
	}
	else return 0;

	if( !undefinedp(score["p3"]) && 
	    user = present(score["p3"], environment(this_object()))) {
		if(!interactive(user)) return 0;
		if(query_idle(user) > 180) return 0;
	}
	else return 0;

	/* ����Ҳ���� */
	if( score["person"] == 3 ) return 1;
	if( !undefinedp(score["p4"]) && 
	    user = present(score["p4"], environment(this_object()))) {
		if(!interactive(user)) return 0;
		if(query_idle(user) > 180) return 0;
	}
	else return 0;
	return 1;
}

/* ����ƾ��Ƿ���� */
int check_over(string name)
{
	int i;
	i=check_player(name);
	switch( i )
	{
		case 1: if( sizeof(p1["m"]) < 1 && sizeof(p1["f"]) < 1 
			&& sizeof(p1["h"]) < 1 && sizeof(p1["t"]) < 1 ) return 1; break;
		case 2: if( sizeof(p2["m"]) < 1 && sizeof(p2["f"]) < 1 
			&& sizeof(p2["h"]) < 1 && sizeof(p2["t"]) < 1 ) return 1; break;
		case 3: if( sizeof(p3["m"]) < 1 && sizeof(p3["f"]) < 1 
			&& sizeof(p3["h"]) < 1 && sizeof(p3["t"]) < 1 ) return 1; break;
		case 4: 
			if( undefinedp(score["p4"]) ) return 0;
			if( sizeof(p4["m"]) < 1 && sizeof(p4["f"]) < 1 
			&& sizeof(p4["h"]) < 1 && sizeof(p4["t"]) < 1 ) return 1; break;
	}
	return 0;
}

void check_board(string winner)
{
	int num=0, x, i;
	string flo;
	object me=this_player();

	message_vision("�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T\n",me);
	message_vision("                              ��      ��                              \n",me);
	message_vision("�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T\n",me);
	i=x=0;
	if( winner != score["p1"] ) {
		message_vision(score["p1"]+"������ʣ�µ��ƣ�"+show_poker(p1,0),me);
		foreach( flo in keys(p1) ) {
			i+=sizeof(keys(p1[flo]));
			if( !undefinedp(p1[flo][2]) ) {
				message_vision(show_flower(flo)+"��δ��(x2) ",me);
				x+=2;
			}
		}
		if( i > 9 ) {
			message_vision("ʣ�µ��Ƴ���ʮ��(x2)��",me);
			x+=2;
		}
		if( !x ) x=1;
		message_vision(score["p1"]+"�ܹ�����"+i+"*"+x+"="+i*x+"�������\n",me);
		i=i*x;
		board["p1"]-=i;
		num+=i;
		message_vision("\n\n",me);
	}

	i=x=0;
	if( winner != score["p2"] ) {
		message_vision(score["p2"]+"������ʣ�µ��ƣ�"+show_poker(p2,0),me);
		foreach( flo in keys(p2) ) {
			i+=sizeof(keys(p2[flo]));
			if( !undefinedp(p2[flo][2]) ) {
				message_vision(show_flower(flo)+"��δ��(x2) ",me);
				x+=2;
			}
		}
		if( i > 9 ) {
			message_vision("ʣ�µ��Ƴ���ʮ��(x2)��",me);
			x+=2;
		}
		if( !x ) x=1;
		message_vision(score["p2"]+"�ܹ�����"+i+"*"+x+"="+i*x+"�������\n",me);
		i=i*x;
		board["p2"]-=i;
		num+=i;
		message_vision("\n\n",me);
	}

	i=x=0;
	if( winner != score["p3"] ) {
		message_vision(score["p3"]+"������ʣ�µ��ƣ�"+show_poker(p3,0),me);
		foreach( flo in keys(p3) ) {
			i+=sizeof(keys(p3[flo]));
			if( !undefinedp(p3[flo][2]) ) {
				message_vision(show_flower(flo)+"��δ��(x2) ",me);
				x+=2;
			}
		}
		if( i > 9 ) {
			message_vision("ʣ�µ��Ƴ���ʮ��(x2)��",me);
			x+=2;
		}
		if( !x ) x=1;
		message_vision(score["p3"]+"�ܹ�����"+i+"*"+x+"="+i*x+"�������\n",me);
		i=i*x;
		board["p3"]-=i;
		num+=i;
		message_vision("\n\n",me);
	}

	i=x=0;
	if( !undefinedp(score["p4"]) && winner != score["p4"] ) {
		message_vision(score["p4"]+"������ʣ�µ��ƣ�"+show_poker(p4,0),me);
		foreach( flo in keys(p4) ) {
			i+=sizeof(keys(p4[flo]));
			if( !undefinedp(p4[flo][2]) ) {
				message_vision(show_flower(flo)+"��δ��(x2) ",me);
				x+=2;
			}
		}
		if( i > 9 ) {
			message_vision("ʣ�µ��Ƴ���ʮ��(x2)��",me);
			x+=2;
		}
		if( !x ) x=1;
		message_vision(score["p4"]+"�ܹ�����"+i+"*"+x+"="+i*x+"�������\n",me);
		i=i*x;
		board["p4"]-=i;
		num+=i;
		message_vision("\n\n",me);
	}

	if( score["p1"] == winner ) {
		message_vision(HIY+score["p1"]+"Ӯ���ⳡ������"+num+"�֡�\n"NOR,me);
		board["p1"]+=num;
	} else if( score["p2"] == winner ) {
		message_vision(HIY+score["p2"]+"Ӯ���ⳡ������"+num+"�֡�\n"NOR,me);
		board["p2"]+=num;
	} else if( score["p3"] == winner ) {
		message_vision(HIY+score["p3"]+"Ӯ���ⳡ������"+num+"�֡�\n"NOR,me);
		board["p3"]+=num;
	} else if( !undefinedp(score["p4"]) && score["p4"] == winner ) {
		message_vision(HIY+score["p4"]+"Ӯ���ⳡ������"+num+"�֡�\n"NOR,me);
		board["p4"]+=num;
	}
	message_vision("�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T\n",me);
	message_vision(show_score()+"\n",me);
	message_vision("�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T\n",me);

	return;
}

string long()
{
	string msg="";
		
	msg += "�X�T�T�T�T�T�T�[\n";
	msg += "�U ���϶����� �U\n";
	msg += "�d�T�T�T�T�T�T�m�T�T�T�T�T�T�T�T�T�j�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[\n";
	msg += "�U                                �U                              �U\n";
	msg += "�U     rules     �ۿ�����         �U                              �U\n";
	msg += "�U     leave     �뿪����         �U                              �U\n";
	msg += sprintf("�U     sit	 ����λ��         �U%13s%-17s�U\n","",
			!undefinedp(score["p1"]) ? score["p1"] : "" );

	msg += "�U     reset	 �����¾֡�ϴ��   �U            �X�T�[            �U\n";

	msg += sprintf("�U     start	 ��ʼ�ƾ�         �U%12s�U���U%-12s�U\n",
			!undefinedp(score["p4"]) ? score["p4"] : "",
			!undefinedp(score["p2"]) ? score["p2"] : "" );

	msg += "�U     .         �ۿ����е���     �U            �^�T�a            �U\n";

	msg += sprintf("�U     du	 ����             �U%13s%-17s�U\n","",
			!undefinedp(score["p3"]) ? score["p3"] : "" );

	msg += "�U     pa        ����Ȩ�ø��¼�   �U                              �U\n";
	msg += "�U     ch        ����Ŀǰ��ʲô�� �U                              �U\n";
	msg += "�U     ,         ����Ŀǰ����	  �U                              �U\n";
	msg += "�U                                �U                              �U\n";
	msg += "�d�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�m�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�g\n";
	msg += "�U     ������t, ������h, ������f, ÷����m                         �U\n";
	msg += "�U     �ϣ���13, ����12, ����11, ����1                            �U\n";
	msg += "�U     duʱΪdu 10t 3h, ��������ʱ�Կհ׸���                      �U\n";
	msg += "�U     ������K,Q,J,A��ʽ�������������ֲ���                        �U\n";
	msg += "�^�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�a\n";

	return msg;
}