#pragma once

#include <vector>
#include <string>
#include <Windows.h>


bool _FileExist(const char* filename);

bool _DelAFile(const char* filename);

//��֤2���ļ�������full name
//���2���ļ���·����ͬ������copy��Ч��
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

//��һ��·���и���һ���ַ����������·���а�������ַ��� ����������Ŀ¼
//���� ����c:\1\ ���� "a\b\c\d"  ���øú�������
int _CreateDirbyAuto(const std::string& baseDir,const std::string& otherDirs);


//////////////////////////////////////////////////////////////////////////

/**
�ҵ�һ��·�������е�Ŀ¼
����pDirStoreVec���Ǵ���ҵ���direct������
����pfiltervec��ɸѡ�б�ɸѡ�б��Ŀ¼�ַ���������'\'��β,���Ҳ�����full path
*/
void _FindAllSubDirect(const char* path,std::vector<std::string>* pDirStoreVec,std::vector<std::string>* pfiltervec=0,bool bRecur=true);

/**
��һ��Ŀ¼�µ��ļ������ݹ������Ŀ¼
vres��Ž��
*/
int _FindFileNotRecur(const char* path,const char* strfindfile,std::vector<std::string>* vres);

int FindFileRecursion(std::vector<std::string>* _result,const char* path,const char* filename,std::vector<std::string>* pFilterVec=0);

int GetATypeFile(std::vector<std::string> * Finalres,const char* dir,const char* filetype,int num,bool bRecur=true);


//////////////////////////////////////////////////////////////////////////
//path string process

void GetFileName(const std::string& fullName, std::string& fileName);
void GetFileNameNoExtenion(const std::string& fullName, std::string& fileName);
void GetDirName(const std::string& fullName, std::string& DirName);


int FindLastSalsh(std::string & name,int end=-1);//��������ҵ�n��б�ܵ�λ��
std::string GetLastNPath(std::string & _name,unsigned int last_level);//��÷�����ĵ�n��б�ܵ��ַ���
std::string GetNPath(std::string & _name,unsigned int last_level,bool single); //���·���е�N��б�ܵ��ַ���������single=false��ʾ��ô�0-N��б�ܵ��ַ���

int FindSalsh(std::string & name,int end=-1);//���������ĵ�n��б�ܵ�λ��
int GetSalshNumber(std::string& _name);//���·����б�ܵ���Ŀ
int TrasformSalsh1(std::string& _name);//�����е�б�ܶ�ͳһΪһ�� \\

int TrasformSalsh2(std::string& _name);//�����е�б�ܶ�ͳһΪһ�� /

int RepairPath(std::string& _name,bool isIncludeFileName=false);//��ȫ·�������������б�ܽ�β�������һ���ַ�����б��
int ChangeExtenionName(std::string& _name,const char* extenion); //�޸���չ��

//���һ��·����һ�����ַ���������c:\resource\media\shaders\1.txt ֻ��Ҫresource֮����ַ�������ô����GetSubPath��subdir����Ϊ��resource��
std::string GetSubPath(std::string& filename,std::string& subdir); 

/*�õ�һ���ַ���λ������·���ĵڼ���
����c:\QQX5\resources\media\shaders\role\actions\role.xml
resourcesλ�ڵ�����

����ֵ��
0��ʾû���ҵ�
����ֵ��ʾ ����
*/
int GetStrInPathLevel(std::string& path,std::string& str);

/*
�õ�һ��·���У��ڼ�������֮����ַ���
����c:\QQX5\resources\media\shaders\role\actions\role.xml
Ҫ�õ�resources֮ǰ������·������c:\QQX5 ����GetPathBeforeLevel(path,3)
����beglevel��endlevelָʾ��Ҫ����ļ�������֮����ַ���
���У�endlevelΪ-1��ʾ��ȡbeglevel֮�������

*/
std::string GetPathMidLevel(std::string& path,unsigned int beglevel,int endlevel);


//�õ�һ��·�������е�Ŀ¼���������ļ���
 int GetPathAllDir(std::string& path,std::vector<std::string>* pVecRes);

int string_to_lower(std::string* _str);
int string_to_upper(std::string* _str);


//
bool TranslateAKindFile(const char* from_dir,const char* to_dir,const char* gettokens,std::vector<std::string>* pFilter=0);
bool MoveAKindFile(const char* from_dir,const char* to_dir,const char* gettokens,std::vector<std::string>* pFilter=0);
bool CleanAKindFile(const char* from_dir,const char* gettokens,std::vector<std::string>* pFilter=0);
bool SetDirectoryStructure(const char* src_base_dir,const char* dest_base_dir,bool lowercase=false);
