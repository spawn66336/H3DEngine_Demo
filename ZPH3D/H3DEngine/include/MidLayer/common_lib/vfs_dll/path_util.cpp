#include "path_util.h"
#include <Windows.h>

namespace VFS
{
	//将一个路径的目录取出来，返回true表示成功，false表示失败
	bool GetDirNameFromFilePath(std::string& dirname)
	{
		size_t pos=dirname.rfind('\\');
		if (pos!=std::string::npos)
		{
			dirname=dirname.substr(0,pos+1); //+1是由于要把斜杠加进来
			return true;
		}
		return false;
	}


	//文件名是否具有指定的扩展名，如果有返回true
	//如果extname=0，无论如何都返回true
	//注意，参数extname不要包含"."
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

	//得到字符串中所有的目录
	//结果放到all_dir中
	void GetAllDirFromPathStr(std::vector<std::string>& all_dir,const char* filepath)
	{
		for (size_t i=0;i<strlen(filepath);++i)
		{
			if(filepath[i]=='\\')
			{
				all_dir.push_back(std::string(filepath,i+1)); //用filepath的前面i+1个字符构造,+1的原因是要把斜杠算进去
			}
		}
	}

	//!补全路径，如果不是以斜杠结尾，将最后一个字符加上斜杠
	/*注意，如果已经有文件名了，就不要补偿
	return:
	1:表示已经修复
	2:表示不需要修复
	*/
	int RepairDirPath(std::string& _name)
	{
		char& last=_name.at(_name.length()-1);
		if(last=='/')
		{
			//替换为反斜杠
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

	// 检查一个路径是否存在（绝对路径、相对路径，文件或文件夹均可）
	bool FilePathExists(const char* lpPath)
	{
		return FindFirstFileExists(lpPath, FALSE);
	}

	// 检查一个文件夹是否存在（绝对路径、相对路径均可）
	bool FolderExists(const char* lpPath)
	{
		return FindFirstFileExists(lpPath, FILE_ATTRIBUTE_DIRECTORY);
	}


}
