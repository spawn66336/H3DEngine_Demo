/**
* @file game_graphic_header.h
* ����ļ�����Ҫ�����ǣ�����������ͷ�ļ�����������ļ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.1.25
*/

#ifndef _GAME_GRAPHIC_HEADER_H_
#define _GAME_GRAPHIC_HEADER_H_

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
//#	include <WinSock2.h>
#include <winsock.h>
#endif

// �������Ҫ��Ե�������ָ���汾��ƽ̨�����޸����ж��塣
// �йز�ͬƽ̨��Ӧֵ��������Ϣ����ο� MSDN��
#ifndef _WIN32_WINNT            // ָ��Ҫ������ƽ̨�� Windows Vista��
#define _WIN32_WINNT 0x0600     // ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif

#include <stdio.h>
#include <tchar.h>

#include <string>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <set>

#ifndef _STLPORT
#include <xutility>
#endif

#include "third_party/tixml/tinyutil.h"
#include "third_party/tixml/tinyxmlhelper.h"
#include "platform/common/data_struct.h"
#include "common_lib/mathlib/dMathHeader.h"
#include "platform/h3d/EffectCore_dx.h"

#ifndef FORBIDDEN_EVIL_CONSTRUCTOR
#define FORBIDDEN_EVIL_CONSTRUCTOR(classname) \
private:\
	classname();\
	classname(const classname& c);\
	classname& operator=(const classname& c)
#endif //FORBIDDEN_EVIL_CONSTRUCTOR

enum LOD_LEVEL
{
	LL_NULL = -0x1,
	LL_ZERO = 0x0,
	LL_ONE = 0x1,
	LL_TWO = 0x2,
	LL_NUM,
};

//#ifndef _LOCATION_SET_OPTIMIZE_
//#define _LOCATION_SET_OPTIMIZE_
//#endif

#endif //_GAME_GRAPHIC_HEADER_H_
