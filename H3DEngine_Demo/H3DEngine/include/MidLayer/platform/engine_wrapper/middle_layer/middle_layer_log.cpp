/**
* @file log.cpp
* 这个文件的主要功能是：
* 
* @author 程陈(Chengchen@h3d.com.cn)
* 
* 更改日志
* ----------------------------------------------------
* ver 1.0.0
*       by 程陈2010.8.10
*/

#include "middle_layer_body.h"
#include "middle_layer_log.h"

void CMiddleLayerLog::write_log(const char* file_path, const char* log)
{
	FILE* fp = fopen(file_path, "a+");
	if (NULL == fp)
	{
		return;
	}

	fwrite(log, 1, strlen(log), fp);
	fflush(fp);

	fclose(fp);
}