/********************************************************************
	created:	2012/02/16
	created:	16:2:2012   14:28
	filename: 	e:\x51_svn\2.1.5\vfs_dll\path_util.h
	file path:	e:\x51_svn\2.1.5\vfs_dll
	file base:	path_util
	file ext:	h
	author:		sssa2000
	
	purpose:	和路径字符串相关的函数
*********************************************************************/
#pragma once

#include <string>
#include <vector>
namespace VFS
{
	//将一个路径的目录取出来，返回true表示成功，false表示失败
	bool GetDirNameFromFilePath(std::string& dirname);

	//文件名是否具有指定的扩展名，如果有返回true
	//如果extname=0，无论如何都返回true
	//注意，参数extname不要包含"."
	bool HasExtensionName(const char* fullpath,const char* extname);

	//得到字符串中所有的目录
	//结果放到all_dir中
	void GetAllDirFromPathStr(std::vector<std::string>& all_dir,const char* filepath);

	//!补全路径，如果不是以斜杠结尾，将最后一个字符加上斜杠
	int RepairDirPath(std::string& _name);

	// 检查一个路径是否存在（绝对路径、相对路径，文件或文件夹均可）
	bool FilePathExists(const char* lpPath);
	bool FolderExists(const char* lpPath);
}