/**
* @file global_body.h
* 这个文件的主要功能是：内部实现文件必须包含的头文件
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.1.25
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
*		获取指定列表中指定名称的元素
* @param
*		element_name：【IN】要获取的元素的名称
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
