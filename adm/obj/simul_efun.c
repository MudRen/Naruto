//#pragma optimize all

#include "/adm/simul_efun/atoi.c"
#include "/adm/simul_efun/chinese.c"
#include "/adm/simul_efun/file.c"
#include "/adm/simul_efun/gender.c"
#include "/adm/simul_efun/object.c"
#include "/adm/simul_efun/path.c"
#include "/adm/simul_efun/wizard.c"

// This must be after gender.c
#include "/adm/simul_efun/message.c"

// �������
#include "/adm/simul_efun/area.c"

// ɫ��ת��
#include "/adm/simul_efun/color.c"

void create()
{
	seteuid(ROOT_UID);
	write("simul_efun loaded successfully.\n");
}
