//#include "stdafx.h"
#include "mycrypt.h"
#include "filesystem.h"
#include "pack.h"
#include "file_imp.h"
#include <Windows.h>
#include "path_util.h"

#ifdef _DEBUG
#pragma  comment (lib,"libtomcryptd.lib")
#else
#pragma  comment (lib,"libtomcrypt.lib")
#endif
namespace VFS
{

	//extern const char* const  MakePurePathString(const char* s,const char* sReserve) ;
	extern void MakeMD5(const char* sString,md5_sum& md5) ;
	extern void ReportError(const char* err);
	extern void ReportErrorEx(const char* str,...);


	char* g_tempFileBuffer=0;
	unsigned int g_tempFileSize=0;
	unsigned int g_ProcessId=0; // by sssa2000 20111223 ��¼����id

	CVirtualFileManager* CVirtualFileManager::_inst=0 ;
	CVirtualFileManager::CVirtualFileManager()
	{
		g_ProcessId = ::GetCurrentProcessId();
	}
	CVirtualFileManager::~CVirtualFileManager()
	{
		Clear() ;
	}
	int CVirtualFileManager::Init()
	{
		return 0 ;
	}
	void CVirtualFileManager::Finish()
	{
	}
	void CVirtualFileManager::Clear()
	{
		H3D::H3DMap<H3D::H3DString, CPack*> ::iterator it = m_mapPacks.begin() ;
		while (it!=m_mapPacks.end())
		{
			delete it->second ;
			++it ;
		}
		m_mapPacks.clear() ;
		m_pCurPack=0 ;
		m_curPackPath = "";

		for (size_t i=0 ;i<m_vFiles.size(); ++i)
			delete m_vFiles[i] ;

		for (size_t i=0 ;i<m_vUnUsedFiles.size(); ++i)
			delete m_vUnUsedFiles[i] ;
		m_vFiles.clear() ;
		m_vUnUsedFiles.clear() ;
		if (g_tempFileBuffer)
			delete []g_tempFileBuffer;
		g_tempFileSize=0;
	}

	bool		CVirtualFileManager::	OpenPack(const char* sPackName)
	{
		H3D::H3DString strFileName = MakePurePathString(sPackName,0) ;
		if (strFileName=="")
			return false ;

		if (m_mapPacks.find(strFileName)!=m_mapPacks.end())
		{
			m_pCurPack = m_mapPacks[strFileName] ;
			m_curPackPath = sPackName;
			return true ;
		}

		CPack* pPack = new CPack ;

		if (!pPack->Load(strFileName.c_str()))
		{
			delete pPack;
			CVirtualFileManager::instance()->GetLog()->LogEx("Load Pak Failed:%s",sPackName);
			return false ;
		}

		m_mapPacks[strFileName] = pPack ;
		m_curPackPath = sPackName;

		CVirtualFileManager::instance()->GetLog()->LogEx("Load Pak:%s",sPackName);
		return true ;
	}
	bool		CVirtualFileManager::	ReleasePack(const char* sPackName)
	{
		H3D::H3DString strFileName = sPackName ;
		PACK_IT it = m_mapPacks.find(strFileName) ;
		if (it==m_mapPacks.end())
			return false ;
		delete it->second ;
		m_mapPacks.erase(it) ;
		m_curPackPath = "";
		return true ;
	}

	CPack*		CVirtualFileManager::GetPack(const char* sPackName)
	{
		H3D::H3DString strFileName = MakePurePathString(sPackName,0) ;
		PACK_IT it = m_mapPacks.find(strFileName) ;
		if (it==m_mapPacks.end())
			return 0 ;
		return it->second ;
	}
	CVFile*		CVirtualFileManager::OpenFile(const char* sFileName,const char* sPackName,const char* sFlags)
	{//�����PAK���Ҳ������ļ�����ֱ�����������е��ļ�
		if (!sPackName)
		{
			//���Ѿ��򿪵�����PACK��Ѱ�Ҵ��ļ�
			H3D::H3DMap<H3D::H3DString, CPack*> ::iterator it = m_mapPacks.begin() ;
			while (it!=m_mapPacks.end())
			{
				CVFile* f = it->second->OpenFile(sFileName);
				if (f)
					return f;

				++it;
			}

			//ע��,���ﷵ����,Ŀǰ��֧�ֶ��������ļ�
			return 0;

		}

		H3D::H3DString strPackName = MakePurePathString(sPackName,0) ;
		CPack* pPack =0; 
		PACK_IT pakit = m_mapPacks.find(strPackName); 
		if ( pakit==m_mapPacks.end())
		{
			//try to open a pack
			if (!OpenPack(sPackName))
			{
				//open a disk file
				return 0;
			}

			pakit = m_mapPacks.find(strPackName); 
			if ( pakit==m_mapPacks.end())
				return 0;

		}

		pPack = pakit->second;

		return pPack->OpenFile(sFileName) ;
	}




	CVFile* CVirtualFileManager::GetEmptyFile()
	{
		CVFile* f=0;
		if (CVirtualFileManager::instance()->m_vUnUsedFiles.size())
		{
			f = CVirtualFileManager::instance()->m_vUnUsedFiles[CVirtualFileManager::instance()->m_vUnUsedFiles.size()-1];
			CVirtualFileManager::instance()->m_vUnUsedFiles.pop_back();
			CVirtualFileManager::instance()->m_vFiles.push_back(f) ;

		}
		else
		{
			f = new CVFile;
			CVirtualFileManager::instance()->m_vFiles.push_back(f);
		}

		H3D::H3DVector<CVFile*> ::iterator itor = CVirtualFileManager::instance()->m_vFiles.begin();
		for (size_t i=0;i<CVirtualFileManager::instance()->m_vFiles.size();++i)
			++itor;

		f->SetItor(itor);

		return f;
	}

	CPack*	CVirtualFileManager::FindFile(const char* sFileName)
	{
		PACK_IT it = m_mapPacks.begin();
		while(it!=m_mapPacks.end())
		{
			if ( it->second->FindFile(sFileName) )
				return it->second;
			++it;
		}
		return 0;
	}
	int CVirtualFileManager::AddFile(const char* sFileName,const char* sPackName,const char* sFileDiskPath)
	{
		if (!m_mapPacks.size())
		{
			CVirtualFileManager::instance()->GetLog()->LogEx("Error! AddFileʧ��,ϵͳû�м����κ�Pak");
			return PACK_NO_PACKAGE;
		}
		//find all pack in dir
		CPack* pPack =FindFile(sFileName);

		if (!pPack)
		{
			if (!sPackName)
			{
				pPack = m_mapPacks.begin()->second;
			}
			else
			{
				H3D::H3DString strFileName = MakePurePathString(sPackName,0) ;
				if (m_mapPacks.find(strFileName)==m_mapPacks.end())
				{
					CVirtualFileManager::instance()->GetLog()->LogEx("Error! AddFileʧ��,�޷��ҵ�����=%s��Pak",strFileName.c_str());
					return PACK_NO_PACKAGE ;
				}
				pPack = m_mapPacks[strFileName] ;
			}
		}

		if (pPack)
			return pPack->AddFile(sFileName,sFileDiskPath);
		else
		{
			CVirtualFileManager::instance()->GetLog()->LogEx("Error! AddFileʧ��,�޷��ҵ���Ӧ��Pak");
			return PACK_NO_PACKAGE;
		}
	}
	int	CVirtualFileManager::DelFile(const char* sFileName,const char* sPackName)
	{
		CPack* pPack =FindFile(sFileName);
		/*
		if (!sPackName)
		{
		if (!m_mapPacks.size())
		return PACK_NO_PACKAGE;

		pPack = m_mapPacks.begin()->second;
		}
		else	
		{
		string strFileName = MakePurePathString(sPackName,0) ;
		if (m_mapPacks.find(strFileName)==m_mapPacks.end())
		return PACK_NO_PACKAGE ;
		pPack = m_mapPacks[strFileName] ;
		}
		*/
		if (!pPack)
			return PACK_NO_PACKAGE;

		return pPack->DelFileVFS(sFileName) ;
	}
	int	CVirtualFileManager::ReplaceFileVFS(const char* sFileName,const char* sPackName)
	{
		return 0 ;
	}
	int	CVirtualFileManager::MvFile(const char* sFileName,const char* sDstFileName,const char* sPackName)
	{
		return 0 ;
	}
	int CVirtualFileManager::CheckFile(const char* sFileName,const char* sPackName,int& size,unsigned int nLow,unsigned int nHigh)
	{
		return 0 ;
	}

	int			CVirtualFileManager::ComparePackFileWithDisk(const char* sFileName,const char* sDiskDir,const char* sPackName)
	{
		int nRes = 0;
		H3D::H3DString strFileName = MakePurePathString(sPackName,0) ;
		PACK_IT it = m_mapPacks.find(strFileName);
		if (it==m_mapPacks.end())
			return PACK_NO_PACKAGE ;

		nRes = it->second->Compare(sFileName,sDiskDir,0,0);

		return nRes;
	}
	int			CVirtualFileManager::CompareDiskFileWithPack(const char* sFileName,const char* sPackName)
	{
		int nRes = 0;
		H3D::H3DString strFileName = MakePurePathString(sPackName,0) ;
		PACK_IT it = m_mapPacks.find(strFileName);
		if (it==m_mapPacks.end())
			return PACK_NO_PACKAGE ;

		unsigned int pos=0;
		nRes = it->second->CompareDisk(sFileName,0,0,pos);

		return nRes;
	}
	H3D::H3DString		CVirtualFileManager::GetCurrPackPath()
	{
		return m_curPackPath;
	}



	int CVirtualFileManager::InitVFSLog(const char* logfilename)
	{
		const char* vfslog_dir="EngineLog";
		std::string log_filename="";
		//���logfilenameΪ�� �Զ������ļ���
		if(!logfilename)
		{
			//����ļ��в����� �����ļ���
			if (!FolderExists(vfslog_dir))
			{
				BOOL b=::CreateDirectory(vfslog_dir,0);
				if(b==FALSE)
				{
					char buff[256];
					GetCurrentDirectoryA(256,buff);
					ReportErrorEx("����Ŀ¼logsʧ��");
					return -1;
				}
			}

			char szParsedString[256];
			SYSTEMTIME sys;
			GetLocalTime( &sys );
			sprintf_s(szParsedString,256,"./%s/VFSLog_%d_%d_%d_%d_%d_%d_%d.log",
				vfslog_dir,
				sys.wYear,
				sys.wMonth,
				sys.wDay,
				sys.wHour,
				sys.wMinute,
				sys.wSecond,
				sys.wMilliseconds);

			log_filename=szParsedString;
		}
		else
			log_filename=logfilename;

		//����־
		m_log.InitLogFile(log_filename.c_str());

		return 1;
	}

	unsigned int CVirtualFileManager::GetPakNum()
	{
		return m_mapPacks.size();
	}

	CPack* CVirtualFileManager::GetPackByIdx(unsigned int idx)
	{
		if(idx>=m_mapPacks.size())
			return 0;

		auto itr=m_mapPacks.begin();
		std::advance(itr,idx);
		return itr->second;
	}
}