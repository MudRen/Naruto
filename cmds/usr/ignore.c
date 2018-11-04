
inherit F_CLEAN_UP;

void create() { seteuid(getuid()); }

int main( object me, string name )
{
  string * ignore;
  int i;

  // add by dragoon
  if( name == "all" ) {
	me->set_temp("ignore/all", 1);
	tell_object(me, "�㿪ʼ�ܾ������˶���˵����\n");
	return 1;
  }
  if( name == "cancel" ) {
	me->delete_temp("ignore/all");
	tell_object(me, "�㿪ʼ���������˶���˵����\n");
	return 1;
  }

  seteuid(getuid(me));
  ignore = me->query( "ignore" );
  if( !pointerp( ignore ) ) ignore = ({ });
  if( !name )
  {
    if( !sizeof( ignore ) ) write( "������û�оܾ����˸��㽻̸��\n" );
    else write( "�����ھܾ����㽻̸�����У�" + implode( ignore, ", " ) + "\n");
    return 1;
  }
  if( -1 == member_array( name, ignore ) )
  {
    write( "�����ڿ�ʼ����  " + name + "  ���㽻̸��\n" );
    ignore += ({ name });
  } else {
    write( "�����ڿ�ʼ��  " + name + "  ���㽻̸��\n" );
    ignore -= ({ name });
  }
  me->set( "ignore", ignore );
  return 1;
}

int help()
{
        write(@HELP
ָ���ʽ�signore < player > | all | cancel

���㲻���ĳ�˽�̸ʱ�Ϳ��������ָ�

�����s
    ignore cuteweca             �ܾ����� cuteweca ��������
    ignore cuteweca             �� cuteweca �����ٸ���˵���ˡ�
    ignore all			�ܾ������κ��˶��㽲����
    ignore cancel		ȡ�� ignore all ��Ч�á�

HELP
        );
  return 1;
}
