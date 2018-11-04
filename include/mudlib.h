/*  mudlib.h

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#ifndef __MUDLIB_H
#define __MUDLIB_H

/* ����� mudlib �����ƣ���������ѯ��ԭ���ߵ�ͬ�⣬�����㲻Ӧ���޸����
 * ���ơ�������Ƴ�������ʶ�� mudlib ��ϵͳ֮�⣬Ҳ���� intermud Э����
 * �� es Ƶ������Ϊ�� MUD �乵ͨѶϢ֮�á�
 */
#define MUDLIB_NAME		"Eastern Stories"

/* ����� mudlib �İ汾���룬ES2 mudlib ���ͳ��°��ʱ���������汾����
 * �е����ֲ��֣����ֺ���ӵ���ĸ���������£�
 *	p: ԭ�Ͱ� (prototype)��ʵ���á�
 *	a: �ڲ����԰� (alpha)���ͳ����м�վ�����߲����á�
 *	b: �������԰� (beta)���ͳ���һ��ʹ���߲����á�
 *	(��): ��ʽ�棬�� mudlib ����˵����������
 */
#define MUDLIB_VERSION_NUMBER	"2.1.3a"

#define MUDLIB_VERSION		(MUDLIB_NAME + " " + MUDLIB_VERSION_NUMBER)

/* ������� MUD ʹ�õ����ı���ϵͳ��Ԥ��Ϊ̨��������õ� BIG5����½����ͨ
 * ��ʹ�� GB �룬ES2 mudlib �ͳ�ʱ�� BIG5 ��Ϊ�����������Ը��ת��� GB ��
 * ��ӭ�������硣
 */
#define MUDLIB_LOCALE		"BIG5"

/* ����������� intermud �ϵ� mud ���ƣ����� intermud ��͹����ϵ����� mud
 * ��ͨ������������ʹ�� ASCII �������Ԫ(������)��
 */
#define INTERMUD_MUD_NAME	"Celestial Empire"

#endif	/* __MUDLIB_H */
