inherit F_CLEAN_UP;

int main(object me, string arg)
{
	string key, chinz;

	if( !arg || arg=="" )
		return notify_fail("ָ���ʽ�schinese <Ӣ��>==<����>\n");

	if( sscanf(arg, "%s==%s", key, chinz)==2 ) {
		CHINESE_D->add_translate(key, chinz);
		write( key + " == " + chinz + "\nOk.\n");
		return 1;
	}

	return 0;
}

int help()
{
	write ( @HELP
ָ���ʽ�schinese <Ӣ��>==<����>
HELP
	);
	return 1 ;
}
