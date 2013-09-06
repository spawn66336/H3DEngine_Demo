/********************************************************************
	created:	2012/12/20
	created:	20:12:2012   14:13
	filename: 	D:\Code\X52_VFS\vfs_dll\IFileEx.h
	file path:	D:\Code\X52_VFS\vfs_dll
	file base:	IFileEx
	file ext:	h
	author:		sssa2000
	
	purpose:	该接口仅仅用来测试用例使用,使用该接口前应调用initvfs
*********************************************************************/
#pragma once

#include "ifile.h"

namespace VFS
{
	//到整个vfs文件数量
	VFS_DLL_API unsigned int GetTotalFileNum();
	//通过一个idx得到一个文件，如果idx太大返回0
	VFS_DLL_API IFile* GetFileByIdx(unsigned int fileIdx);
}
