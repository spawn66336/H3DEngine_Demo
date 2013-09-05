/**
* @file log.cpp
* ����ļ�����Ҫ�����ǣ�
* 
* @author �̳�(Chengchen@h3d.com.cn)
* 
* ������־
* ----------------------------------------------------
* ver 1.0.0
*       by �̳�2010.8.10
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