/**
* @file global_body.h
* ����ļ�����Ҫ�����ǣ��ڲ�ʵ���ļ����������ͷ�ļ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.1.25
*/

#ifndef _GAME_GRAPHIC_BODY_H_
#define _GAME_GRAPHIC_BODY_H_

#include "game_graphic_header.h"

#include "game_graphic_interface.h"

#ifndef FLOAT_PRESION
#define FLOAT_PRESION 0.000001
#endif

#ifdef _DEBUG
#pragma comment(lib, "mathlib_d.lib")
//#pragma comment(lib, "tinyxml_d.lib")
#pragma comment(lib, "vfsDLLProj.lib")
#pragma comment(lib, "tixml_d.lib")
#else
#pragma comment(lib, "mathlib.lib")
//#pragma comment(lib, "tinyxml.lib")
#pragma comment(lib, "vfsDLLProj.lib")
#pragma comment(lib, "tixml.lib")
#endif

/**
* @brief
*		��ȡָ���б���ָ�����Ƶ�Ԫ��
* @param
*		element_name����IN��Ҫ��ȡ��Ԫ�ص�����
*/
template<class T>
struct name_equal
	: public std::binary_function<T*, std::string, bool>
{	// functor for operator>
	bool operator()(T* obj, std::string obj_name) const
	{
		assert(NULL != obj);
		return (std::string(obj->get_name()) ==  obj_name);
	}
};

#ifdef _DEBUG

class CGameGraphicLog
{
public:
	static void write_log(const char* file_path, const char* log)
	{
		FILE* fp = fopen(file_path, "a+");
		if (NULL == fp)
		{
			return;
		}

		fwrite(log, 1, strlen(log), fp);
		fflush(fp);

		fclose(fp);
	};
};


#endif // _DEBUG
#endif //_GAME_GRAPHIC_BODY_H_
