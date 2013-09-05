#pragma once

#include <vector>
#include <string>
#include <Windows.h>


bool _FileExist(const char* filename);

bool _DelAFile(const char* filename);

//保证2个文件名都是full name
//如果2个文件的路径不同将会起到copy的效果
bool _ReNameAFile(const char* orgname,const char* destname);


//copy a file 
//when overwrite is false , overwrite the copy path
bool _CopyAFile(const char* orgfile,const char* dest,bool overwrite=false);


bool _MoveAFile(const char* src,const char* dest, bool overwrite=false);


bool _MoveAFileWhenNextBoot(const char* src,const char* dest,bool overwrite=false);


DWORD _GetFileSize(const char* file_full_name);

int _GetFileLastWriteTime(const char* filename,FILETIME* time);
//////////////////////////////////////////////////////////////////////////
// Dir op
bool _CreateADir(const char* dirfullname);

int _CreateADir(const char* path,const char* dirname);

//as same as the _RenameAFile;
bool _RenameADir(const char* orgname,const char* newname);

bool _DeleteAEmptyDir(const char* dirname);

//在一个路径中给定一个字符串，在这个路径中按照这个字符串 创建各级子目录
//例如 想在c:\1\ 建立 "a\b\c\d"  调用该函数即可
int _CreateDirbyAuto(const std::string& baseDir,const std::string& otherDirs);


//////////////////////////////////////////////////////////////////////////

/**
找到一个路径下所有的目录
参数pDirStoreVec，是存放找到的direct的容器
参数pfiltervec是筛选列表。筛选列表的目录字符串不能以'\'结尾,而且不能是full path
*/
void _FindAllSubDirect(const char* path,std::vector<std::string>* pDirStoreVec,std::vector<std::string>* pfiltervec=0,bool bRecur=true);

/**
找一个目录下的文件，不递归查找子目录
vres存放结果
*/
int _FindFileNotRecur(const char* path,const char* strfindfile,std::vector<std::string>* vres);

int FindFileRecursion(std::vector<std::string>* _result,const char* path,const char* filename,std::vector<std::string>* pFilterVec=0);

int GetATypeFile(std::vector<std::string> * Finalres,const char* dir,const char* filetype,int num,bool bRecur=true);


//////////////////////////////////////////////////////////////////////////
//path string process

void GetFileName(const std::string& fullName, std::string& fileName);
void GetFileNameNoExtenion(const std::string& fullName, std::string& fileName);
void GetDirName(const std::string& fullName, std::string& DirName);


int FindLastSalsh(std::string & name,int end=-1);//反方向查找第n个斜杠的位置
std::string GetLastNPath(std::string & _name,unsigned int last_level);//获得反方向的第n个斜杠的字符串
std::string GetNPath(std::string & _name,unsigned int last_level,bool single); //获得路径中第N个斜杠的字符串，器中single=false表示获得从0-N个斜杠的字符串

int FindSalsh(std::string & name,int end=-1);//获得正方向的第n个斜杠的位置
int GetSalshNumber(std::string& _name);//获得路径中斜杠的数目
int TrasformSalsh1(std::string& _name);//把所有的斜杠都统一为一种 \\

int TrasformSalsh2(std::string& _name);//把所有的斜杠都统一为一种 /

int RepairPath(std::string& _name,bool isIncludeFileName=false);//补全路径，如果不是以斜杠结尾，将最后一个字符加上斜杠
int ChangeExtenionName(std::string& _name,const char* extenion); //修改扩展名

//获得一个路径的一部分字符串，例如c:\resource\media\shaders\1.txt 只想要resource之后的字符串，那么调用GetSubPath，subdir参数为“resource”
std::string GetSubPath(std::string& filename,std::string& subdir); 

/*得到一个字符串位于整个路径的第几级
例如c:\QQX5\resources\media\shaders\role\actions\role.xml
resources位于第三级

返回值：
0表示没有找到
其他值表示 级别
*/
int GetStrInPathLevel(std::string& path,std::string& str);

/*
得到一个路径中，第几个级别之间的字符串
例如c:\QQX5\resources\media\shaders\role\actions\role.xml
要得到resources之前的所有路径即，c:\QQX5 调用GetPathBeforeLevel(path,3)
参数beglevel，endlevel指示想要获得哪几个级别之间的字符串
其中，endlevel为-1表示获取beglevel之后的所有

*/
std::string GetPathMidLevel(std::string& path,unsigned int beglevel,int endlevel);


//得到一个路径中所有的目录，不包括文件名
 int GetPathAllDir(std::string& path,std::vector<std::string>* pVecRes);

int string_to_lower(std::string* _str);
int string_to_upper(std::string* _str);


//
bool TranslateAKindFile(const char* from_dir,const char* to_dir,const char* gettokens,std::vector<std::string>* pFilter=0);
bool MoveAKindFile(const char* from_dir,const char* to_dir,const char* gettokens,std::vector<std::string>* pFilter=0);
bool CleanAKindFile(const char* from_dir,const char* gettokens,std::vector<std::string>* pFilter=0);
bool SetDirectoryStructure(const char* src_base_dir,const char* dest_base_dir,bool lowercase=false);
