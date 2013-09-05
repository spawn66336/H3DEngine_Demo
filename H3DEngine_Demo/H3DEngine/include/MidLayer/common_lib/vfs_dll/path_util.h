/********************************************************************
	created:	2012/02/16
	created:	16:2:2012   14:28
	filename: 	e:\x51_svn\2.1.5\vfs_dll\path_util.h
	file path:	e:\x51_svn\2.1.5\vfs_dll
	file base:	path_util
	file ext:	h
	author:		sssa2000
	
	purpose:	��·���ַ�����صĺ���
*********************************************************************/
#pragma once

#include <string>
#include <vector>
namespace VFS
{
	//��һ��·����Ŀ¼ȡ����������true��ʾ�ɹ���false��ʾʧ��
	bool GetDirNameFromFilePath(std::string& dirname);

	//�ļ����Ƿ����ָ������չ��������з���true
	//���extname=0��������ζ�����true
	//ע�⣬����extname��Ҫ����"."
	bool HasExtensionName(const char* fullpath,const char* extname);

	//�õ��ַ��������е�Ŀ¼
	//����ŵ�all_dir��
	void GetAllDirFromPathStr(std::vector<std::string>& all_dir,const char* filepath);

	//!��ȫ·�������������б�ܽ�β�������һ���ַ�����б��
	int RepairDirPath(std::string& _name);

	// ���һ��·���Ƿ���ڣ�����·�������·�����ļ����ļ��о��ɣ�
	bool FilePathExists(const char* lpPath);
	bool FolderExists(const char* lpPath);
}