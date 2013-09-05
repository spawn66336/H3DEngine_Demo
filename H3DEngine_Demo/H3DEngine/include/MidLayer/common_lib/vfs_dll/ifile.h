#ifndef __IFILE__
#define __IFILE__
#ifdef VFS_DLL_EXPORTS
#define VFS_DLL_API __declspec(dllexport)
#else
//#define VFS_DLL_API __declspec(dllimport)
#define VFS_DLL_API 
#endif

#define PACK_VERSION 1
#define MAX_PACK_FILE_NAME 128
#include "stdio.h"

typedef long long int64;
namespace VFS
{


extern VFS_DLL_API const char* g_vfs_dir ;//寻找*.pak的缺省位置,一般在 ../resources/目录下
class VFS_DLL_API IFile
{
public :
	virtual ~IFile()=0{};
	virtual FILE* GetCFileHandle()=0 ;
	virtual  size_t Read(  void *buffer,
		size_t size,
		size_t count)=0 ;
//	virtual size_t Write(void* buffer,size_t size,size_t count)=0 ;
//	virtual size_t Append(void* buffer,size_t size,size_t count)=0 ;
	virtual size_t GetSize()=0 ;
	virtual int64 GetTime()=0 ;//最后被修改的时间,可以通过crt time函数转换成可看懂的文字
	virtual size_t CurPosition()=0 ;//此文件目前被读取的位置
	virtual long Tell()=0 ;
	virtual int Seek(long offset,int origin)=0 ;
	virtual int	Close(IFile** pThis)=0 ;//关闭文件
	virtual int Eof()=0;
	virtual char *	GetS( char *string,	int n ) =0;//读取一行
	virtual int		GetC()=0;//读取一个char
	virtual void Release()=0;//释放虚拟文件
//	virtual void Printf(char* format,...)=0;
	virtual void SetCurPosAfterScan()=0;//对文件进行sscan操作后, 需要手动调整文件当前读取位置
	virtual const char*		GetFileName()const=0;//得到文件名字
} ;
//add a file to package, pack name : dir1\dir2\dir3\filename.extname
//
VFS_DLL_API IFile* CreateIFile(const char* sPackFileName,const char* sFullPath,const char* sPackageFullPath) ;
VFS_DLL_API int InitVFS();
VFS_DLL_API void ShutdownVFS();
VFS_DLL_API int AddFile(const char* sFileName,const char* sDiskFileName,const char* sPackFileName=0);
VFS_DLL_API int DelFile(const char* sPackFileName,const char* sPackageFullPath) ;
VFS_DLL_API bool CreateDirectory(const char* sPackDirName,const char* sPackageFullPath) ;
VFS_DLL_API bool RemoveDirectory(const char* sPackDirName,const char* sPackageFullPath) ;
VFS_DLL_API bool MvFile(const char* sSrcFilePath,const char* sDstFilePath,const char* sPackageFullPath) ;
VFS_DLL_API bool ReplaceFile(const char* sDstFile,const char* sSrcFile,const char* sPackageFullPath) ;
VFS_DLL_API unsigned int GetFileSize(IFile* f) ;
VFS_DLL_API void GetFileTime(IFile* f, int64 &nTime) ;
VFS_DLL_API bool CloseHandle(IFile* hObject) ;
VFS_DLL_API bool DirExists(const char* sPackDirName,const char* sPackageFullPath) ;
VFS_DLL_API bool FileExist(const char* sFileName);

VFS_DLL_API IFile* Hfopen(const char* sname,const char* sflag,const char* sPakName) ;
VFS_DLL_API int Hfclose(IFile* f) ;
VFS_DLL_API size_t Hfread( void *buffer,
					 size_t size,
					 size_t count,IFile* f) ;
VFS_DLL_API size_t Hfwrite( void *buffer,
					  size_t size,
					  size_t count,IFile* f) ;
VFS_DLL_API int Hfseek(IFile* f,size_t offset,int flag) ;
VFS_DLL_API int Hfeof(IFile* f) ;
VFS_DLL_API int Hftell(IFile* f) ;
VFS_DLL_API char *Hfgets( char *string,	int n,	IFile *stream ) ;
VFS_DLL_API int Hfgetc( IFile *stream ) ;
VFS_DLL_API IFile* FindFileInDir(const char* sdir,const char* sExt,const char* spack);
VFS_DLL_API IFile* FindNextFileInDir(IFile* f,const char* sExt);
}
#endif