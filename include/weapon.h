// weapon.h

#ifndef __WEAPON_H__
#define __WEAPON_H__

#define F_WEAPON "/feature/weapon/weapon.c"
	
#ifndef __ARMOR_H__
inherit ITEM;
inherit F_ATTRIBUTE;
inherit F_EQUIP;
inherit F_STATISTIC;
#endif

#endif
