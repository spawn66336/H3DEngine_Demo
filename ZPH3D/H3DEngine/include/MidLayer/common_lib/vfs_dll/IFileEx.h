/********************************************************************
	created:	2012/12/20
	created:	20:12:2012   14:13
	filename: 	D:\Code\X52_VFS\vfs_dll\IFileEx.h
	file path:	D:\Code\X52_VFS\vfs_dll
	file base:	IFileEx
	file ext:	h
	author:		sssa2000
	
	purpose:	�ýӿڽ���������������ʹ��,ʹ�øýӿ�ǰӦ����initvfs
*********************************************************************/
#pragma once

#include "ifile.h"

namespace VFS
{
	//������vfs�ļ�����
	VFS_DLL_API unsigned int GetTotalFileNum();
	//ͨ��һ��idx�õ�һ���ļ������idx̫�󷵻�0
	VFS_DLL_API IFile* GetFileByIdx(unsigned int fileIdx);
}
