/**
* @file log.h
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.8.10
*/

#ifndef _MIDDLELAYER_LOG_H_
#define _MIDDLELAYER_LOG_H_

class CMiddleLayerLog
{
public:
	static void write_log(const char* file_path, const char* log);
};

#endif //
