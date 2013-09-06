#include "path_util.h"
#include <Windows.h>

namespace VFS
{
	//��һ��·����Ŀ¼ȡ����������true��ʾ�ɹ���false��ʾʧ��
	bool GetDirNameFromFilePath(std::string& dirname)
	{
		size_t pos=dirname.rfind('\\');
		if (pos!=std::string::npos)
		{
			dirname=dirname.substr(0,pos+1); //+1������Ҫ��б�ܼӽ���
			return true;
		}
		return false;
	}


	//�ļ����Ƿ����ָ������չ��������з���true
	//���extname=0��������ζ�����true
	//ע�⣬����extname��Ҫ����"."
	bool HasExtensionName(const char* fullpath,const char* extname)
	{
		if(!extname)
			return true;

		char* ext =(char*) strrchr(fullpath,'.');
		if (ext)
		{
			++ext;
			if (!_stricmp(ext,extname)) 
				return true;
		}
		return false;
	}

	//�õ��ַ��������е�Ŀ¼
	//����ŵ�all_dir��
	void GetAllDirFromPathStr(std::vector<std::string>& all_dir,const char* filepath)
	{
		for (size_t i=0;i<strlen(filepath);++i)
		{
			if(filepath[i]=='\\')
			{
				all_dir.push_back(std::string(filepath,i+1)); //��filepath��ǰ��i+1���ַ�����,+1��ԭ����Ҫ��б�����ȥ
			}
		}
	}

	//!��ȫ·�������������б�ܽ�β�������һ���ַ�����б��
	/*ע�⣬����Ѿ����ļ����ˣ��Ͳ�Ҫ����
	return:
	1:��ʾ�Ѿ��޸�
	2:��ʾ����Ҫ�޸�
	*/
	int RepairDirPath(std::string& _name)
	{
		char& last=_name.at(_name.length()-1);
		if(last=='/')
		{
			//�滻Ϊ��б��
			last='\\';
		}

		else if(last!='\\')
		{
			_name.append("\\");
			return 1;
		}

		return 2;
	}


	bool FindFirstFileExists(const char* lpPath, DWORD dwFilter)
	{
		WIN32_FIND_DATA fd;
		HANDLE hFind = FindFirstFile(lpPath, &fd);
		BOOL bFilter = (FALSE == dwFilter) ? TRUE : fd.dwFileAttributes & dwFilter;
		BOOL RetValue = ((hFind != INVALID_HANDLE_VALUE) && bFilter) ? TRUE : FALSE;
		FindClose(hFind);
		return RetValue==1;
	}

	// ���һ��·���Ƿ���ڣ�����·�������·�����ļ����ļ��о��ɣ�
	bool FilePathExists(const char* lpPath)
	{
		return FindFirstFileExists(lpPath, FALSE);
	}

	// ���һ���ļ����Ƿ���ڣ�����·�������·�����ɣ�
	bool FolderExists(const char* lpPath)
	{
		return FindFirstFileExists(lpPath, FILE_ATTRIBUTE_DIRECTORY);
	}


}
