
#include "FileOperation.h"
#include <iostream>
#include <Windows.h>
#include <io.h>
#include <assert.h>
#include <algorithm>
#pragma warning(disable : 4800) 
#pragma warning(disable : 4018) 
#pragma warning(disable : 4996)

bool _FileExist(const char* filename)
{
	if(_access(filename,0)!=-1)
		return true;
	return false;
}


bool _DelAFile(const char* filename)
{
	assert(_FileExist(filename));
	return DeleteFileA(filename);
}


//��֤2���ļ�������full name
//���2���ļ���·����ͬ������copy��Ч��
bool _ReNameAFile(const char* orgname,const char* destname)
{
	assert(orgname);
	assert(destname);
	return (rename(orgname,destname)==0);
}

//copy a file 
//when overwrite is false , overwrite the copy path
bool _CopyAFile(const char* orgfile,const char* dest,bool overwrite)
{
	assert(orgfile);
	assert(dest);
	return (CopyFileA(orgfile,dest,overwrite)!=0);
}

bool _MoveAFile(const char* src,const char* dest, bool overwrite)
{
	assert(src);
	assert(dest);

	if(overwrite)
		return (MoveFileExA(src,dest,MOVEFILE_REPLACE_EXISTING)!=0);
	else
		return (MoveFileA(src,dest)!=0);
}

bool _MoveAFileWhenNextBoot(const char* src,const char* dest,bool overwrite)
{
	assert(src);
	assert(dest);
	if(overwrite)
		return (MoveFileExA(src,dest,MOVEFILE_DELAY_UNTIL_REBOOT|MOVEFILE_REPLACE_EXISTING)!=0);
	else 
		return (MoveFileExA(src,dest,MOVEFILE_DELAY_UNTIL_REBOOT)!=0);

}

DWORD _GetFileSize(const char* file_full_name)
{
	if(file_full_name)
	{
		HANDLE hFile = INVALID_HANDLE_VALUE;
		hFile = CreateFileA(file_full_name,// file to open
			GENERIC_READ,						// open for reading
			FILE_SHARE_READ,					// share for reading
			NULL,								// default security
			OPEN_EXISTING,						// existing file only
			FILE_ATTRIBUTE_NORMAL,				// normal file
			NULL);								// no attr. template
		if (hFile != INVALID_HANDLE_VALUE) 
		{ 
			DWORD res=GetFileSize(hFile,0);
			CloseHandle(hFile);
			return res;
		}
		else
		{
			//���ļ�ʧ�ܣ������ļ�������
			//��������������Ŀ¼ �������ļ�Ҳ��ת���˴�
			//InfoLog::GetSingleton().Put(LOG_FAILURE,"%s,�޷���ȡ�ļ���С",file_full_name);
			::MessageBoxA(0,"���ļ�ʧ�ܣ��޷���ȡ�ļ���С","Error!",0);
		}
	}

	return 0;
}
int _GetFileLastWriteTime(const char* filename,FILETIME* time)
{
	if(filename)
	{
		HANDLE hFile = INVALID_HANDLE_VALUE;
		hFile = CreateFileA(filename,// file to open
			GENERIC_READ,						// open for reading
			FILE_SHARE_READ,					// share for reading
			NULL,								// default security
			OPEN_EXISTING,						// existing file only
			FILE_ATTRIBUTE_NORMAL,				// normal file
			NULL);								// no attr. template
		if (hFile != INVALID_HANDLE_VALUE) 
		{ 
			bool res=GetFileTime(hFile,0,0,time);
			CloseHandle(hFile);
			return res?1:-1;
		}
		else
		{
			//���ļ�ʧ�ܣ������ļ�������
			//��������������Ŀ¼ �������ļ�Ҳ��ת���˴�
			//InfoLog::GetSingleton().Put(LOG_FAILURE,"%s,�޷���ȡ�ļ�ʱ��",filename);
			::MessageBoxA(0,"���ļ�ʧ�ܣ��޷���ȡ�ļ�ʱ��","Error!",0);
			return -1;
		}
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////
//����Ŀ¼������·��
bool _CreateADir(const char* dirfullname)
{
	return ((::CreateDirectoryA(dirfullname,NULL))!=0);
}

//����Ŀ¼
//����1����Ŀ¼������2��Ҫ������Ŀ¼��
int _CreateADir(const char* path,const char* dirname)
{
	std::string fullname=path;
	int len=fullname.length();
	if(fullname[len]!='\\')
	{
		fullname+="\\";
	}
	fullname+=dirname;

	if(_FileExist(fullname.c_str()))
		return 0;
	else
	{
		 bool res=_CreateADir((fullname.c_str()));
		 return res?1:0;
	}
	return 1;
}

//as same as the _RenameAFile
bool _RenameADir(const char* orgname,const char* newname)
{
	assert(orgname);
	assert(newname);
	return (rename(orgname,newname)==0);
}

bool _DeleteAEmptyDir(const char* dirname)
{
	assert(dirname);
	if(_FileExist(dirname))
		return (RemoveDirectoryA(dirname)!=0);
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////

/**
�ҵ�һ��·�������е�Ŀ¼
����pDirStoreVec���Ǵ���ҵ���direct������
����pfiltervec��ɸѡ�б�ɸѡ�б��Ŀ¼�ַ���can not��'\'��β,���� can not be full path

*/
void _FindAllSubDirect(const char* path,std::vector<std::string>* pDirStoreVec,std::vector<std::string>* pfiltervec,bool bRecur)
{
	assert(pDirStoreVec);

	bool usefilter=false;
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;


	std::string fullname=path;

	int len=fullname.length();
	if(fullname[len-1]!='\\')
	{
		fullname+='\\';
	}
	//���pfileervec��Ϊ�գ���ô����ɸѡ.������ݽ����ĸ�Ŀ¼ λ��ɸѡ�б��� ֱ���˳�
	if(pfiltervec && (!pfiltervec->empty()))
	{
		for(int i=0;i<pfiltervec->size();++i)
		{
			size_t i2=fullname.rfind('\\',fullname.length()-2);
			if(i2!=std::string::npos)
			{
				std::string tempstr1=pfiltervec->at(i);
				int str_len=tempstr1.length();
				if(tempstr1[str_len-1]!='\\')
				{
					tempstr1+='\\';
				}
				std::string tempstr2=fullname.substr(i2+1,fullname.length()-i2+1);
				if(tempstr2==tempstr1)
					return;
			}
		}
	}
	fullname+='*';
	std::string dirname=fullname;
	//////////////////////////////////////////////////////////////////////////
	//������direction�г���
	hFind=FindFirstFileA(dirname.c_str(),&FindFileData);
	if(hFind!=INVALID_HANDLE_VALUE)
	{
		//���Ȱ��״δ���˺�����path ����
		do
		{

			if( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				&& strcmp(FindFileData.cFileName,".")
				&& strcmp(FindFileData.cFileName,".."))
			{
				std::string temp=path;
				if(temp[temp.length()-1]!='\\')
				{
					temp+='\\';
				}
				temp+=FindFileData.cFileName;
				temp+="\\";

				//ɸѡ��Ŀ¼
				if(pfiltervec && (!pfiltervec->empty()))
				{
					for(int i=0;i<pfiltervec->size();++i)
					{
						size_t i2=fullname.rfind("\\",temp.length()-1);
						if(i2!=std::string::npos)
						{
							std::string tempstr1=pfiltervec->at(i);
							int str_len=tempstr1.length();
							if(tempstr1[str_len-1]!='\\')
							{
								tempstr1+='\\';
							}
							std::string tempstr2=temp.substr(i2+1,temp.length()-i2+1);
							if(tempstr2==tempstr1)
							{
								usefilter=true;
								break;
							}
						}
					}
					if(usefilter)
					{
						usefilter=!usefilter;
						continue;
					}
				}
				pDirStoreVec->push_back(temp);
				if(bRecur)
					_FindAllSubDirect(temp.c_str(),pDirStoreVec,pfiltervec);
			}
		}
		while(FindNextFileA(hFind,&FindFileData));
	}

	FindClose(hFind);
}

/**
��һ��Ŀ¼�µ��ļ������ݹ������Ŀ¼
vres��Ž��
*/
int _FindFileNotRecur(const char* path,const char* strfindfile,std::vector<std::string>* vres)
{
	assert(strfindfile);
	assert(path);

	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError;

	std::string fullname=path;
	int len=fullname.length();
	if(fullname[len-1]!='\\')
	{
		fullname+="\\";
	}
	fullname+=strfindfile;

	hFind = FindFirstFileA(fullname.c_str(), &FindFileData);

	//std::vector<std::string> vres;
	std::string res=path;


	if (hFind == INVALID_HANDLE_VALUE) 
	{
		return -1;
	} 
	else 
	{
		if(res[res.length()-1]!='\\')
		{
			res+='\\';
		}
		res+=FindFileData.cFileName;
		if(strlen(FindFileData.cFileName)>4)
		if(!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
			vres->push_back(res);
		while (FindNextFileA(hFind, &FindFileData) != 0) 
		{
			res=path;
			if(res[res.length()-1]!='\\')
			{
				res+='\\';
			}

			res+=FindFileData.cFileName;
			//ֻ���ļ� ����Ŀ¼��
			if(!(FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
			//if(strlen(FindFileData.cFileName)>4 && res[res.length()-4]=='.')
				vres->push_back(res);
		}

		dwError = GetLastError();
		FindClose(hFind);
	}
	return 1;
}


int FindFileRecursion(std::vector<std::string>* _result,const char* path,const char* filename,std::vector<std::string>* pFilterVec)
{		
	if(path && filename && _result)
	{
		std::vector<std::string> dirname;
		std::string rootdir=path;
		TrasformSalsh1(rootdir);

		dirname.push_back(rootdir);
		_FindAllSubDirect(rootdir.c_str(),&dirname,pFilterVec);

		for(int i=0;i<dirname.size();++i)
		{
			_FindFileNotRecur(dirname[i].c_str(),filename,_result);
		}	
		return 1;
	}
	return -1;
}
/*
��ȡһ��Ŀ¼�����е� ĳһ���͵��ļ�
num��ʾ��Ҫ���ٸ���������С��num,����Num=-1����ȫ������

*/
int GetATypeFile(std::vector<std::string>* Finalres,const char* dir,const char* filetype,int num,bool bRecur)
{
	//std::vector<std::string> res;
	if(num<=0 && num!=-1)
		return -1;

	if (!Finalres)
	{
		return -1;
	}

	if(bRecur)
	{
		FindFileRecursion(Finalres,dir,filetype,0);
	}
	else
	{
		_FindFileNotRecur(dir,filetype,Finalres);
	}
	if(num>=Finalres->size() ||num==-1)
		return 1;
	else
	{
		Finalres->resize(num);
	}
	return 1;
}

void GetFileName(const std::string& fullName, std::string& fileName)
{
	size_t i = fullName.rfind("\\");
	if (i!=std::string::npos)
	{
		fileName = fullName.substr(i+1, fullName.length()-i-1);
	}
	else
	{
		i=fullName.rfind("/");
		if (i!=std::string::npos)
		{
			fileName = fullName.substr(i+1, fullName.length()-i-1);
			return;
		}
		fileName = fullName;
	}
}

void GetFileNameNoExtenion(const std::string& fullName, std::string& fileName)
{
	GetFileName(fullName, fileName);
	size_t i = fileName.find(".");
	if (i!=std::string::npos)
	{
		fileName = fileName.substr(0, i);
	}
}

void GetDirName(const std::string& fullName, std::string& DirName)
{
	size_t i = fullName.rfind("\\");
	if (i!=std::string::npos)
	{
		DirName = fullName.substr(0, i);
	}
	else
	{
		i=fullName.rfind("/");
		if (i!=std::string::npos)
		{
			DirName = fullName.substr(0, i);
			return;
		}
		DirName = fullName;
	}
}


//////////////////////////////////////////////////////////////////////////
//string helper

int FindLastSalsh(std::string & name,int end)
{
	size_t index1=(end==-1)?name.find_last_of('/'):name.find_last_of('/',end);
	size_t index2=(end==-1)?name.find_last_of('\\'):name.find_last_of('\\',end);
	size_t index;

	if(index1!=std::string::npos && index2!=std::string::npos)
	{
		index=(index1>index2)?index1:index2;
	}
	else if(index1==std::string::npos &&  index2!=std::string::npos)
	{
		index=index2;
	}
	else if(index2==std::string::npos &&  index1!=std::string::npos)
	{
		index=index1;
	}
	else
	{
		return -1;
	}
	return index;
}
int FindSlash(std::string&name,int end)
{
	size_t index1=(end==-1)?name.find_first_of('/'):name.find_first_of('/',end);
	size_t index2=(end==-1)?name.find_first_of('\\'):name.find_first_of('\\',end);
	size_t index;

	if(index1!=std::string::npos && index2!=std::string::npos)
	{
		index=(index1>index2)?index1:index2;
	}
	else if(index1==std::string::npos &&  index2!=std::string::npos)
	{
		index=index2;
	}
	else if(index2==std::string::npos &&  index1!=std::string::npos)
	{
		index=index1;
	}
	else
	{
		return -1;
	}
	return index;
}
std::string GetLastNPath(std::string & _name,unsigned int last_level)
{
	std::string res="";
	int index=std::string::npos;
	int last_index=index;
	std::string name=_name;
	for(int i=0;i<last_level;++i)
	{
		last_index=index-1;

		index=FindLastSalsh(name,-1);
		if(index==-1)
		{
			return res;
		}

		name=name.substr(0,index-1);
	}
	res=_name.substr(index+1,last_index-index);
	return res;
}
// ������������ʾ�ǵ����ĵ�n��б��֮������ݻ��Ǵ�0-n��б�ܵ�����
std::string GetNPath(std::string& _name,unsigned int level,bool single)
{

	size_t size=_name.length();
	std::string res="";
	int num_slash=0;
	int slash_index=-1;
	int last_slash_index=-1;
	for(size_t i=0;i<size;++i)
	{
		if(_name.at(i)=='\\' || _name.at(i)=='/')
		{
			++num_slash;
			last_slash_index=slash_index;
			slash_index=i;
		}
		if(num_slash>=level)
			break;
	}

	size_t beg_index=0;
	size_t len=slash_index;
	if(single)  //���ֻ��Ҫ�����ĵ�n��б�ܵ��ַ���
	{
		beg_index=last_slash_index+1;
		len=slash_index-last_slash_index;
	}

	if(last_slash_index>0)
	{
		res=_name.substr(beg_index,len); 
	}
	else if(last_slash_index<0)
	{
		res=_name.substr(0,len); 
	}

	return res;
	
}

int GetSalshNumber(std::string& _name)
{
	size_t size=_name.size();
	int res=0;
	for(size_t i=0;i<size;++i)
	{
		char t_char=_name.at(i);
		if(t_char=='\\' ||t_char=='/')
		{
			++res;
		}
	}

	return res;
}

int TrasformSalsh1(std::string& _name)
{
	size_t size=_name.size();
	int res=0;
	for(size_t i=0;i<size;++i)
	{
		char t_char=_name.at(i);
		if(t_char=='/')
		{
			_name.at(i)='\\';
			++res;
		}
	}

	return res;
}
int TrasformSalsh2(std::string& _name)
{
	size_t size=_name.size();
	int res=0;
	for(size_t i=0;i<size;++i)
	{
		char t_char=_name.at(i);
		if(t_char=='\\')
		{
			_name.at(i)='/';
			++res;
		}
	}

	return res;
}
//��ȫ·�������������б�ܽ�β�������һ���ַ�����б��
//ע�⣬����Ѿ����ļ����ˣ��Ͳ�Ҫ����
/*
return:
0: ��ʾ�Ǵ����ļ���������·����
1:��ʾ�Ѿ��޸�
2:��ʾ����Ҫ�޸�
*/
int RepairPath(std::string& _name,bool isIncludeFileName)
{
	if(isIncludeFileName)
	{
		if(_name.rfind('.')!=std::string::npos)
		{
			return 0;
		}
	}
	char& last=_name.at(_name.length()-1);
	if(last!='/' && last!='\\')
	{
		_name.append("\\");
		return 1;
	}

	return 2;
}

/*
return:
-1 ��ʾ�ַ��в����� �ļ�
1��ʾ�ɹ�

*/
int ChangeExtenionName(std::string& _name,const char* extenion)
{
	if(_name.rfind('.')==std::string::npos)
	{
		return -1;
	}
	size_t len=_name.length();
	_name.at(len-3)=extenion[0];
	_name.at(len-2)=extenion[1];
	_name.at(len-1)=extenion[2];
	return 1;

}


int string_to_lower(std::string* _str)
{
	std::transform (_str->begin(),_str->end(), _str->begin(), tolower); 
	return 1;
}
int string_to_upper(std::string* _str)
{
	std::transform (_str->begin(),_str->end(), _str->begin(), toupper); 
	return 1;

}


//���һ��·����һ�����ַ���������c:\resource\media\shaders\1.txt ֻ��Ҫresource֮����ַ���"media\shaders\1.txt"����ô����GetSubPath��subdir����Ϊ��resource��
std::string GetSubPath(std::string& filename,std::string& subdir)
{
	std::string res="";
	size_t pos=filename.find(subdir,0); //find���ص�һ��ƥ�䵽�Ľ��
	if (pos!=std::string::npos)
	{
		size_t len=subdir.length();
		size_t beginpos=pos+len+1;
		size_t sublen=filename.length()-beginpos;
		res=filename.substr(beginpos,sublen);
	}

	return res;

}


bool CleanAKindFile(const char* from_dir,const char* gettokens,std::vector<std::string>* pFilter)
{
	bool final_result=true;
	if(from_dir )
	{
		std::vector<std::string> filearray;
		FindFileRecursion(&filearray,from_dir,gettokens,pFilter);
		size_t totalsize=filearray.size();
		float inv_totalsize=1.0f;
		if(totalsize>0)
			inv_totalsize=1.0f/totalsize;

		std::vector<std::string> failed_res;
		for(int i=0;i<totalsize;++i)
		{
			bool res=_DelAFile(filearray[i].c_str());
			//if failed, record the file
			if(!res)
			{
				DWORD info=GetLastError();
				char errorcode[128];
				sprintf(errorcode,"   Error Code: %d",info);

				std::string failed_reson=filearray[i];
				failed_reson+=errorcode;
				//InfoLog::GetSingleton().Put(LOG_FAILURE,"%s",failed_reson.c_str());
				//failed_res.push_back(failed_reson);
				final_result=false;
			}
		}

	}
	return final_result;
}
/**
gettokens��ʾ���ҵĺ���ͨ������ַ���
*/
bool TranslateAKindFile(const char* from_dir,const char* to_dir,const char* gettokens,std::vector<std::string>* pFilter)
{
	SetDirectoryStructure(from_dir,to_dir);
	bool final_result=true;
	if(from_dir && to_dir)
	{
		std::vector<std::string> filearray;
		FindFileRecursion(&filearray,from_dir,gettokens,pFilter);
		size_t totalsize=filearray.size();
		float inv_totalsize=1.0f;
		if(totalsize>0)
			inv_totalsize=1.0f/totalsize;
		int fromdirlen=strlen(from_dir);
		int todirlen=strlen(to_dir);
		std::vector<std::string> failed_res;
		for(int i=0;i<totalsize;++i)
		{
			std::string newfilepath=to_dir;
			if(newfilepath[newfilepath.length()-1]!='\\')
			{
				newfilepath+='\\';
			}
			newfilepath+=filearray[i].substr(fromdirlen,filearray[i].length()-fromdirlen);
			//m_str_nowfile=filearray[i].c_str();
			bool res=_CopyAFile(filearray[i].c_str(),newfilepath.c_str(),false);
			//if failed, record the file
			if(!res)
			{
				DWORD info=GetLastError();
				char errorcode[128];
				sprintf(errorcode,"   Error Code: %d",info);

				std::string failed_reson=filearray[i];
				failed_reson+='\n';
				failed_reson+=newfilepath;
				failed_reson+=errorcode;
				//InfoLog::GetSingleton().Put(LOG_FAILURE,"%s",failed_reson.c_str());
				//failed_res.push_back(failed_reson);
				final_result=false;
			}
		}

	}
	return final_result;
}
/**
temp

move some file
��ΪSDK MoveFile ��������̣�����Ҫ��������
*/
bool MoveAKindFile(const char* from_dir,const char* to_dir,const char* gettokens,std::vector<std::string>* pFilter)
{
	SetDirectoryStructure(from_dir,to_dir);
	bool final_result=true;
	if(from_dir && to_dir)
	{
		std::vector<std::string> filearray;
		FindFileRecursion(&filearray,from_dir,gettokens,pFilter);
		size_t totalsize=filearray.size();		
		float inv_totalsize=1.0f;
		if(totalsize>0)
			inv_totalsize=1.0f/totalsize;

		int fromdirlen=strlen(from_dir);
		int todirlen=strlen(to_dir);
		std::vector<std::string> failed_res;
		for(int i=0;i<totalsize;++i)
		{

			std::string newfilepath=to_dir;
			int tpos=newfilepath.length();
			if(newfilepath[tpos-1]!='\\')
			{
				newfilepath+='\\';
			}
			newfilepath+=filearray[i].substr(fromdirlen,filearray[i].length()-fromdirlen);
			//m_str_nowfile=filearray[i].c_str();

			bool res=_MoveAFile(filearray[i].c_str(),newfilepath.c_str(),true);

			//if failed, record the file
			if(!res)
			{
				DWORD info=GetLastError();
				char errorcode[128];
				sprintf(errorcode,"   Error Code: %d",info);

				std::string failed_reson=filearray[i];
				failed_reson+='\n';
				failed_reson+=newfilepath;
				failed_reson+=errorcode;
				failed_reson+=errorcode;
				//failed_res.push_back(failed_reson);
				//InfoLog::GetSingleton().Put(LOG_FAILURE,"%s",failed_reson.c_str());

				final_result=false;
			}
		}
		//WriteRecord(failed_res,m_logfile2,MOVE_OP);
	}
	return final_result;
}

/**
����Ŀ¼�ṹ���൱��dos ���� xcopy /t/e
c:\123\456\789\
*/
bool SetDirectoryStructure(const char* src_base_dir,const char* dest_base_dir,bool lowercase)
{

	if(src_base_dir && dest_base_dir)
	{
		//if dest do not exist,create the directory
		if(!_FileExist(dest_base_dir))
		{
			std::string dest_base_dir_lowcase=dest_base_dir;
			if (lowercase )
			{
				string_to_lower(&dest_base_dir_lowcase);
			}
			if(!_CreateADir(dest_base_dir_lowcase.c_str()))
			{
				::MessageBoxA(0,"Can not Set Directory Structrue!","error",0);
				return false;
			}
		} 
		std::vector<std::string> allsubdir;
		_FindAllSubDirect(src_base_dir,&allsubdir);
		if(!allsubdir.empty())
		{

			for(int i=0;i<allsubdir.size();++i)
			{
				std::string srcdir=allsubdir.at(i);
				int src_base_len=strlen(src_base_dir);
				std::string destdir=dest_base_dir;
				if(destdir[destdir.length()-1]!='\\')
				{
					destdir+='\\';
				}
				destdir+=srcdir.substr(src_base_len,srcdir.length()-src_base_len);
				if(!_FileExist(destdir.c_str()))
				{
					std::string dest_base_dir_lowcase=destdir;
					if (lowercase )
					{
						string_to_lower(&dest_base_dir_lowcase);
					}

					if(!_CreateADir(dest_base_dir_lowcase.c_str()))
					{
						::MessageBoxA(0,"Can not Set Directory Structrue!","error",0);
						return false;
					}
				}
			}
		}
		return true;
	}

	return false;
}

/*�õ�һ���ַ���λ������·���ĵڼ���
����c:\QQX5\resources\media\shaders\role\actions\role.xml
resourcesλ�ڵ�����

ע�⣬�����в���Ҫ����б��
����ֵ��
0��ʾû���ҵ�
����ֵ��ʾ ����

*/
int GetStrInPathLevel(std::string& path,std::string& str)
{
	size_t size=path.size();

	std::vector<std::string> level_str;
	size_t last_index=0;
	for(size_t i=0;i<size;++i)
	{
		if(path.at(i)=='/' || path.at(i)=='\\')
		{
			std::string substr=path.substr(last_index,i-last_index);
			level_str.push_back(substr);
			last_index=i+1;
		}
	}
	//�����������ļ��������ļ�������
	if(last_index<size)
	{
		level_str.push_back(path.substr(last_index,size-last_index));
	}
	std::vector<std::string>::const_iterator itr=find(level_str.begin(),level_str.end(),str);
	if(itr!=level_str.end())
	{
		size_t level_size=level_str.size();
		size_t pos=0;
		for(size_t i=0;i<level_size;++i)
		{
			if(str==level_str.at(i))
			{
				pos=i;
			}
		}

		return pos+1;
	}
	else
	{
		return 0;
	}
}


/*
�õ�һ��·���У��ڼ�������֮ǰ���ַ���
����c:\QQX5\resources\media\shaders\role\actions\role.xml
Ҫ�õ�resources֮ǰ������·������c:\QQX5 ����GetPathBeforeLevel(path,3)

beglevel ��0��ʼ
*/
std::string GetPathMidLevel(std::string& path,unsigned int beglevel,int endlevel)
{
	size_t size=path.size();

	std::vector<std::string> level_str;
	size_t last_index=0;
	for(size_t i=0;i<size;++i)
	{
		if(path.at(i)=='/' || path.at(i)=='\\')
		{
			std::string substr=path.substr(last_index,i-last_index);
			level_str.push_back(substr);
			last_index=i+1;
		}
	}
	//�����������ļ��������ļ�������
	if(last_index<size)
	{
		level_str.push_back(path.substr(last_index,size-last_index));
	}

	std::string res;
	size_t level_size=level_str.size();
	if(endlevel==-1)
		endlevel=level_size;
	if(beglevel>level_size ||endlevel>level_size)
		return res;
	for(size_t i=beglevel;i<endlevel;++i) 
	{
		if(i!=beglevel)
			res+="\\";
		res+=level_str.at(i);

	}
	return res;
}
//�õ�һ��·�������е�Ŀ¼���������ļ���
int GetPathAllDir(std::string& path,std::vector<std::string>* pVecRes)
{
	if(!pVecRes)
		return -1;

	size_t last_index=0;
	size_t size=path.size();
	for(size_t i=0;i<size;++i)
	{
		if(path.at(i)=='/' || path.at(i)=='\\')
		{
			std::string substr=path.substr(last_index,i-last_index);
			pVecRes->push_back(substr);
			last_index=i+1;
		}
	}

	return 1;
}


//��һ��·���и���һ���ַ����������·���а�������ַ��� ����������Ŀ¼
//���� ����c:\1\ ���� "a\b\c\d"  ���øú�������
int _CreateDirbyAuto(const std::string& baseDir,const std::string& otherDirs)
{

	std::vector<std::string> VecRes;
	size_t last_index=0;
	size_t size=otherDirs.size();
	for(size_t i=0;i<size;++i)
	{
		if(otherDirs.at(i)=='/' || otherDirs.at(i)=='\\')
		{
			std::string substr=otherDirs.substr(last_index,i-last_index);
			VecRes.push_back(substr);
			last_index=i+1;
		}
	}
	
	size_t VecResSize=VecRes.size();
	std::string totalPath=baseDir;
	RepairPath(totalPath);
	for(size_t i=0;i<VecResSize;++i)
	{
		std::string& subdir=VecRes.at(i);
		int res=_CreateADir(totalPath.c_str(),subdir.c_str());
		//����0��ʾ�Ѿ�����
		if(res<0)
		{
			return -1;
		}
		totalPath=totalPath+subdir;
		RepairPath(totalPath);


	}
	
	return 1;
}
