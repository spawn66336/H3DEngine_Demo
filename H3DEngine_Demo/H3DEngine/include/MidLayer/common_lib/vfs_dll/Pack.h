#ifndef _PACK
#define _PACK
#include "H3DSTLHeader.h"
#include <memory.h>
#include "stdio.h"
#include "ifile.h"
#include "filesystem.h"

using namespace std ;
namespace VFS
{
	enum
	{
		PACK_CAN_FILE_IT=1<<0,
	};
	class CVFile;
	#define  MAX_RES_NUM 16
	class CPack
	{
	public:
		CPack() ;
		~CPack() ;

		bool			Load(const char* sPackName) ;
		inline FILE*	GetHandle(){return m_pPackFile ;} ;
		CVFile*			OpenFile(const char* sFileName) ;
		CVFile*			OpenFullDirFile(const char* sFileName);
		void			ReturnVFile(CVFile* f) ;


		int				AddFile(const char* sFileName,const char *sFileDiskPath=0) ;
		int				DelFileVFS(const char* sFileName) ;
		int				ReplaceFileVFS(const char* sFileName,const char*sFileDiskPath) ;
		int				MvFile(const char* sFileName,const char* sDstFileName) ;
		int				CheckFile(const char* sFileName,int& size,unsigned int nLow,unsigned int nHigh) ;
		int				AddDir(const char* sDirName) ;
		int				DelDir(const char* sDirName) ;
		int				DelDirNoSelf(const char* sDirName);
		int				DirExists(const char* sPackDirName);
		//compare pack's file and dir with disk
		int				Compare(const char* sItemName,const char* sDiskDir,char* sResString,size_t nMaxLength) ;
		//compare pack's file with disk
		int				CompareFile(const char* sItemName,const char* sDiskDir,char* sResString,size_t nMaxLength,size_t&nCurPos) ;
		H3D::H3DString						m_strPackPath ;
		const char*		GetRootName(){return m_PackHeader.sRootDirName ;} ;

		//compare a disk dir and all files&subdir with pack
		int				CompareDisk(const char* sDirName,char* sResString,size_t nMaxLength,size_t & nCurPos) ;
		int m_nCompRes[MAX_RES_NUM] ;//0, same 1,not same, 2, can't find, 3, can't find node;

		int				FragmentClean();

		bool			FindFile(const char* sFileName);
		int				getTotalFileNum();
		CVFile*			getFirstFile(int & res);
		CVFile*			getNextFile(int& res);
		CVFile*			getFileByIdx(unsigned int idx);
		int				GetFileInfoByIdx(unsigned int idx,tFileInfo& s);
		int				GetFileInfoByPos(unsigned int pos,tFileInfo& s);
		unsigned int	GetFilePosByIdx(unsigned int idx);
		unsigned int	GetFilePosByFileName(const char* sFileName);

	protected:
		CVFile*			GetVFile(unsigned int off);
		void			ReadMD5Map(unsigned int pos) ;
		void			EraseMapItem(unsigned int pos, md5_sum & erase_md) ;
		const char* const  PrepareFileOp(const char* sFileName,md5_sum& md,int func=0) ;
		unsigned int	FindValidHole(unsigned int nPos,unsigned int  nSize,tDataSegHeader& dataseg) ;
		void			ProcessDataseg(tDataSegHeader& dataseg,unsigned int nNewFileSize,unsigned int nHolePos) ;
		void			DelDataHole(tDataSegHeader& dataseg,tDataSegHeader& lastSeg,tDataSegHeader& nextSeg) ;
		void			InsertDataHole(tDataSegHeader& dataseg , unsigned int nPos) ;
		unsigned int	AddFileInfo(tFileInfo& info,unsigned int nPos) ;
		int				AddMD5Item(tFileInfo &info,unsigned int nFilePos) ;

		int				r_Compare(const char* sItemName,const char* sDiskDir,char* sResString,size_t nMaxLength,size_t& nCurPos) ;
		int				r_CompareDisk(const char* sDirName,char* sResString,size_t nMaxLength,size_t& nCurPos) ;
		void			ReOpenPackage(const char* sOpenFlag) ;
		FILE*						m_pPackFile ;
		tFileHeader					m_PackHeader ;
		tMapHeader					m_MD5Info ;
		H3D::H3DMap<md5_sum,unsigned int,md5_less>	m_mapFileOffsets ;

		int							m_FileCount;
		H3D::H3DMap<md5_sum,unsigned int,md5_less>::iterator m_map_it;
		int							m_flags;
	public:
		int GetMapHeader_ForUT(int idx,tMapHeader& md5map);
	} ;
}//namespace VFS
#endif