//#include "stdafx.h"
#include <windows.h>
#include "ifile.h"
#include "filesystem.h"
#include "pack.h"
#include "file_imp.h"
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//////========================     C runtime file lib   ========================///////////


namespace VFS
{

	class SectionLock
	{
	public:	
		SectionLock()
		{
			InitializeCriticalSection(&m_cs);
		}

		~SectionLock()
		{
			DeleteCriticalSection(&m_cs);
		}

		void Lock()
		{
			EnterCriticalSection(&m_cs);
		}

		void Unlock()
		{
			LeaveCriticalSection(&m_cs);
		}

	private:
		CRITICAL_SECTION		m_cs;
	};

	template<class LockType>
	class ScopedLock
	{
	public:
		ScopedLock(LockType* pLock):m_pLock(pLock)
		{
			m_pLock->Lock();
		}

		~ScopedLock()
		{
			m_pLock->Unlock();
		}

	private:
		LockType* m_pLock;

	};

	typedef ScopedLock<SectionLock> VFSLock;


	VFS_DLL_API const char* g_vfs_dir = "..\\resources\\";
	static int nOpened = 0;
	SectionLock g_pLock;

	VFS_DLL_API int InitVFS()
	{
		if (nOpened)
			return 0;

		int res=CVirtualFileManager::instance()->InitVFSLog(0);

		//find all *.pak files under g_vfs_dir
		//open them all
		char a[128];
		strcpy(a,g_vfs_dir);
		strcat(a,"*.pak");
		int n=0;
		WIN32_FIND_DATA finddata;
		HANDLE h = ::FindFirstFile(a,&finddata);
		if (!h || h==INVALID_HANDLE_VALUE)
			return 0;
		if (finddata.cFileName && finddata.cFileName[0])
		{
	
			strcpy(a,g_vfs_dir);
			strcat(a,finddata.cFileName);
		}
		if (CVirtualFileManager::instance()->OpenPack(a))
			++n;
		
		while(::FindNextFile(h,&finddata))
		{
			strcpy(a,g_vfs_dir);
			strcat(a,finddata.cFileName);
			if ( CVirtualFileManager::instance()->OpenPack(a) )
			{
				++n;
			}
		}
		FindClose(h);

		nOpened = 1;

		return n;
	} 
	VFS_DLL_API void ShutdownVFS()
	{
		CVirtualFileManager::instance()->GetLog()->LogEx("VFS Shutdown");
		CVirtualFileManager::instance()->destroy();
		nOpened = 0;
	}
	VFS_DLL_API IFile* CreateIFile(const char* sPackFileName,const char* sFullPath,const char* sPackageFullPath) 
	{
		VFSLock lock(&g_pLock);
		return 0;
	}
	VFS_DLL_API IFile* Hfopen(const char* sname,const char* sflag,const char* sPakName) 
	{
		VFSLock lock(&g_pLock);
		if (!sPakName)
		{
			return (IFile*)CVirtualFileManager::instance()->OpenFile(sname,0,sflag);
		}
		IFile* f = (IFile*) CVirtualFileManager::instance()->OpenFile(sname,sPakName,sflag);
		return f;
	}

	VFS_DLL_API int Hfclose(IFile* f) 
	{
		VFSLock lock(&g_pLock);
		f->Release();
		return 0;
	}

	VFS_DLL_API size_t Hfread( void *buffer,
				size_t size,
				size_t count,IFile* f)
	{
		VFSLock lock(&g_pLock);
		return f->Read(buffer,size,count);
	}
	//VFS_DLL_API size_t Hfwrite( void *buffer,
	//			size_t size,
	//			size_t count,IFile* f)
	//{
	//	return f->Write(buffer,size,count);
	//}
	VFS_DLL_API int Hfseek(IFile* f,size_t offset,int flag)
	{
		VFSLock lock(&g_pLock);
		return f->Seek((long)offset,flag);
	}
	VFS_DLL_API int Hfeof(IFile* f)
	{
		VFSLock lock(&g_pLock);
		return f->Eof();
	}

	VFS_DLL_API int Hftell(IFile* f)
	{
		VFSLock lock(&g_pLock);
		return f->Tell();
	}
	VFS_DLL_API char *Hfgets( char *string,	int n,	IFile *stream ) 
	{
		VFSLock lock(&g_pLock);
		return stream->GetS(string ,n);

	}
	VFS_DLL_API int Hfgetc( IFile *stream ) 
	{
		VFSLock lock(&g_pLock);
		return stream->GetC();

	}
	VFS_DLL_API int Hfscanf(IFile *f, const char *format, ...) 
	{
		VFSLock lock(&g_pLock);
		//	return f->Scan(format,...);
		return 0;
	}


	VFS_DLL_API bool FileExist(const char* sFileName)
	{
		VFSLock lock(&g_pLock);
		if (CVirtualFileManager::instance()->FindFile(sFileName))
			return true;
		return false;
	}
	VFS_DLL_API int AddFile(const char* sFileName,const char* sDiskFileName,const char* sPackFileName)
	{
		VFSLock lock(&g_pLock);
		CVirtualFileManager::instance()->GetLog()->LogEx("AddFile sFileName=%s sDiskFileName=%s,sPackFileName=%s",sFileName,sDiskFileName,sPackFileName);
		CPack* pack = CVirtualFileManager::instance()->FindFile(sFileName);
		if (!pack)//不存在
		{
			return CVirtualFileManager::instance()->AddFile(sFileName,sPackFileName,sDiskFileName);
		}
		else
		{
			return pack->AddFile(sFileName,sDiskFileName);
		}
		return 0;
	}
	VFS_DLL_API int  DelFile(const char* sPackFileName,const char* sPackageFullPath) 
	{
		VFSLock lock(&g_pLock);
		CPack* pack = CVirtualFileManager::instance()->FindFile(sPackFileName);
		if (!pack)//不存在
		{
			return CVirtualFileManager::instance()->DelFile(sPackFileName,sPackageFullPath);
		}
		else
		{
			return pack->DelFileVFS(sPackFileName);
		}
		return 0;
	}
	VFS_DLL_API IFile* FindFileInDir(const char* sdir,const char* sExt,const char* spack)
	{
		

		return 0;

	}

	VFS_DLL_API IFile* FindNextFileInDir(IFile* f,const char* sExt)
	{
		
		return 0;
		
	}


}//namespace
