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
		unsigned int nNumDataHole ;//�ı��Ѿ��е��ļ����,�����ļ�,ֻ���������ļ�֮��,ɾ���ļ�,
		//ֱ�ӱ�����ݶοն�,���´����ļ�����ʱ,�����ļ���С,���зָ�,�Ͳ������ݶ�
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
		unsigned int nUsedNum ;//����MAP PAIRʱ,ֱ�Ӹı� nNumPair,nSize,
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
		unsigned int nSize ;//�ܴ�С
		unsigned int LocalSize; //�����ݶζ�ʵ�ʴ�С
		unsigned int nNextSeg; //�ļ��¶�����ƫ����
		unsigned int nFlags; //bin or text
		unsigned int nTimeLow,nTimeHigh ;//���һ���޸�ʱ��
		unsigned int nNextFilePos ;//����Ŀ¼��ǰ�ļ�����һ���ļ�ͷ��ַ
		unsigned int nLastFilePos; //����Ŀ¼��ǰ�ļ�����һ���ļ�ͷ��ַ
		unsigned int nNodePos; //ָ���Ŵ��ļ�Ŀ¼�ڵ��λ��,parent node's pos
		char		 sFileName[MAX_PACK_FILE_NAME] ;
		unsigned int nReserve[16] ;
	} ;
	//һ�����ݶο���������״̬ ,����ļ����ݣ��Ϳ���״̬������ļ�����ʱ��DATA SEG��С�����ļ���С��
	//����״̬ʱ����ɾ�����ļ������޸Ĵ�С�����Ŀռ䣬��������DATA SEGռ�ݡ��ȴ��Ժ����ļ����Ӻ��޸�
	//ʱ�����·���
	struct tDataSegHeader
	{
		tDataSegHeader()
		{
			memset(this,0,sizeof(tDataSegHeader)) ;
		} ;
		unsigned int nFlag ;
		unsigned int nSize ;
		//�����ļ���ɾ����,���ݶ�
		//����־����,������ʱ,�����ļ���С,������ݶηָ�.
		//�����¿ռ䲻������һ������,��ֻ�ܷ���
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
		//���Ϊ0���߸���,���Ѵ��ļ���ɾ��,
		//md5ͷ������ָ����һ����md5�Ľṹ���Ա������������
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
		PACK_NO_FILE_IT,//���ܱ���pack files
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


		//��ʼ����־
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