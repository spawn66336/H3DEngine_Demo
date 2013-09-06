#ifndef _FILESYSTEM
#define _FILESYSTEM
#include "H3DSTLHeader.h"
#include <memory.h>
#include "stdio.h"
#include "ifile.h"

#include "VFSLog.h"

using namespace std ;
namespace VFS
{

	extern const char* const  MakePurePathString(const char* s,const char* sReserve,int func=0);
	struct tFileHeader
	{
		tFileHeader():nVersion(1),nNumFile(0)
		{
			memset(this,0,sizeof(tFileHeader)) ;
		} ;
		unsigned int nVersion ;
		unsigned int nNumFile ;
		unsigned int nNumNode ;
		unsigned int nNumDir ;
		unsigned int nNumDataSeg ;
		unsigned int nNumDataHole ;//改变已经有的文件结果,增加文件,只增加在总文件之后,删除文件,
		//直接标记数据段空洞,当下次有文件填入时,根据文件大小,进行分割,和补充数据段
		unsigned int nFirstNode ;
		unsigned int nFirstEmptyNode ;
		unsigned int nFirstSegHeader ;
		unsigned int nFirstDataHole ;
		unsigned int nFirstMapHeader ;
		unsigned int nFirstEmptyMapHeader ;
		char sRootDirName[MAX_PACK_FILE_NAME] ;
		unsigned int nReserve[16] ;
	} ;
	struct tMapHeader
	{
		tMapHeader():nNextMapHeader(0),nNumPair(0),nUsedNum(0)
		{
			memset(this,0,sizeof(tMapHeader)) ;
		} ;
		unsigned int nNumPair ;
		unsigned int nUsedNum ;//减少MAP PAIR时,直接改变 nNumPair,nSize,
		//nUsedSize
		unsigned int nNextMapHeader ;
		unsigned int nReserve[16] ;
	} ;



	struct tFileInfo
	{
		tFileInfo()
		{
			memset(this,0,sizeof(tFileInfo)) ;
		};
		unsigned int nSize ;//总大小
		unsigned int LocalSize; //本数据段段实际大小
		unsigned int nNextSeg; //文件下段数据偏移量
		unsigned int nFlags; //bin or text
		unsigned int nTimeLow,nTimeHigh ;//最后一次修改时间
		unsigned int nNextFilePos ;//本级目录当前文件下面一个文件头地址
		unsigned int nLastFilePos; //本级目录当前文件上面一个文件头地址
		unsigned int nNodePos; //指向存放此文件目录节点的位置,parent node's pos
		char		 sFileName[MAX_PACK_FILE_NAME] ;
		unsigned int nReserve[16] ;
	} ;
	//一个数据段可以有两种状态 ,存放文件数据，和空置状态。存放文件数据时，DATA SEG大小就是文件大小，
	//空置状态时，被删除的文件或者修改大小后多余的空间，被整个空DATA SEG占据。等待以后有文件增加和修改
	//时被重新分配
	struct tDataSegHeader
	{
		tDataSegHeader()
		{
			memset(this,0,sizeof(tDataSegHeader)) ;
		} ;
		unsigned int nFlag ;
		unsigned int nSize ;
		//当有文件被删除后,数据段
		//被标志空闲,再填入时,根据文件大小,会把数据段分割.
		//当余下空间不够放置一个数据,则只能放弃
		unsigned int nNextDataSeg ;//if the data seg is empty, then this param point to next unused seg space
		unsigned int nLastDataSeg ;//if the data seg is empty, then this param point to next unused seg space
		unsigned int nReserve[16] ;
	} ;
	struct md5_sum
	{
		long long low,high ;
		bool operator==(md5_sum& b)const
		{
			return (low == b.low && high == b.high) ;
		} ;
	} ;
	struct md5_less
	{
		bool operator ()(const md5_sum &a,const  md5_sum& b)const
		{
			if(a.high!=b.high)
				return a.high<b.high ;
			return a.low<b.low ;
		};
	} ;
	struct tMD5
	{
		tMD5()
		{
			memset(this,0,sizeof(tMD5)) ;
		};
		md5_sum md ;
		//如果为0或者负数,则已此文件经删除,
		//md5头被用作指向下一个空md5的结构，以便快速浏览和填充
		unsigned int nFileInfoOffset; 
	} ;


	enum//ERROR_CODE
	{
		PACK_NO_ERROR,
		PACK_NO_PACKAGE,
		PACK_NO_DIR,
		PACK_NO_FILE,
		PACK_NO_DISK_FILE,
		PACK_NO_DIRNODE_POS,
		PACK_NO_FILE_IT,//不能遍历pack files
	} ;
	class CPack ;
	class CVFile ;
	typedef H3D::H3DMap<md5_sum,unsigned int,md5_less> MAP_MD5 ;
	typedef MAP_MD5::iterator MD5_IT ;
	typedef H3D::H3DMap<H3D::H3DString, CPack*> MAP_PACK ;
	typedef MAP_PACK::iterator PACK_IT ;


	class CVirtualFileManager
	{
		friend class CPack;
		friend class CVFile;
		static CVirtualFileManager* _inst ;
		CVirtualFileManager() ;
		~CVirtualFileManager() ;
	public:
		
		static CVirtualFileManager* instance()
		{
			if (!_inst)
				_inst = new CVirtualFileManager ;
			return _inst ;
		} ;
		static void destroy()
		{
			if (_inst)
				delete _inst ;
			_inst=0;
		} ;
		int				Init() ;
		void			Finish() ;
		void			Clear() ;
		bool			OpenPack(const char* sPackName) ;
		bool			ReleasePack(const char* sPackName) ;
		//bool			CreatePack(const char* sPackPath);
		CPack*			GetPack(const char* sPackName) ;
		CVFile*			OpenFile(const char* sFileName,const char* sPackName,const char* sFlags=0) ;

		//add, delete, replace,move,
		CPack*		FindFile(const char* sFileName);
		int			AddFile(const char* sFileName,const char* sPackName,const char* sFileDiskPath=0) ;
		int			DelFile(const char* sFileName,const char* sPackName) ;
		int			ReplaceFileVFS(const char* sFileName,const char* sPackName) ;
		int			MvFile(const char* sFileName,const char* sDstFileName,const char* sPackName) ;
		int			CheckFile(const char* sFileName,const char* sPackName,int& size,unsigned int nLow,unsigned int nHigh) ;

		int			ComparePackFileWithDisk(const char* sFileName,const char* sDiskDir,const char* sPackName);
		int			CompareDiskFileWithPack(const char* sFileName,const char* sPackName);
		H3D::H3DString		GetCurrPackPath();


		//初始化日志
		int InitVFSLog(const char* logfilename);

		VFSLog* GetLog(){return &m_log;}

		unsigned int GetPakNum();
		CPack* GetPackByIdx(unsigned int idx);
	protected:

		H3D::H3DMap<H3D::H3DString, CPack*> m_mapPacks ;
		CPack*				m_pCurPack ;
		H3D::H3DString				m_curPackPath;

		H3D::H3DVector<CVFile*>	m_vFiles ;//a vfile pool, to generate file object
		H3D::H3DVector<CVFile*>	m_vUnUsedFiles;  // a vfile to collect un-used file object


		CVFile* GetEmptyFile();

		//about find file
		CPack*				m_pCurFindPack;
		bool				m_findinSpecPack;
		H3D::H3DString				m_findDir;
		VFSLog m_log;
	} ;
}//namespace VFS
#endif