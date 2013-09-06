//#include "stdafx.h"
#include "mycrypt.h"
#include "pack.h"
#include "file_imp.h"
#include <assert.h>
#include <windows.h>
#define fseek(a,b,c) _fseeki64(a,(__int64)b, c)
#define ftell  (unsigned int)_ftelli64
namespace VFS
{
	size_t myfwrite(void *buffer,
		size_t size,
		size_t count,FILE* f)
	{
		size_t sizeres = fwrite(buffer,size,count,f);
		fflush(f);
		return sizeres;
	}

	CPack::CPack():m_pPackFile(0),m_flags(0)
	{
	}
	CPack::~CPack()
	{
		if (m_pPackFile)
			fclose(m_pPackFile) ;

		m_mapFileOffsets.clear() ;


	}
	FILE* g_LogFile=0 ;
	bool			CPack::Load(const char* sPackName)
	{

		m_flags=0;

		FILE* f = fopen(sPackName,"rb") ;
		if (!f)
			return false ;

		m_strPackPath = sPackName ;

		m_pPackFile = f ;

		if (!fread(&m_PackHeader,sizeof(tFileHeader),1,f))
		{
			assert(!"open pak header error!s") ;
			return false ;
		}

		//version check
		if (m_PackHeader.nVersion!=PACK_VERSION)
		{
			assert(!"Version Checking Error!") ;
			return false ;
		}
		_strlwr(m_PackHeader.sRootDirName) ;
		ReadMD5Map((unsigned int)sizeof(tFileHeader)) ;
		//Sleep(10);

		return true ;
	}
#define VERSION_0_8_5
	extern void MakeMD5(const char* sString,md5_sum& md5) ;
	void CPack::ReadMD5Map(unsigned int pos)
	{
		while( pos)
		{
			//- fseek(m_pPackFile,pos,SEEK_SET) ;
			_fseeki64(m_pPackFile,(__int64)pos,SEEK_SET) ;
			tMapHeader md5map ;
			if (!fread(&md5map,sizeof(md5map),1,m_pPackFile))
			{
				assert(!"open map header error") ;
				return  ;
			}
			//only record first map header
			if (!m_MD5Info.nUsedNum)
				memcpy(&m_MD5Info,&md5map,sizeof(md5map)) ;
			else
			{
				
			}
#ifdef VERSION_0_8_5
			if (md5map.nUsedNum)
			{
				tMD5* md  = new tMD5[md5map.nUsedNum];
				fread(md,sizeof(tMD5)* md5map.nUsedNum,1,m_pPackFile);
				for (unsigned int i = 0; i<md5map.nUsedNum; ++i)
				{
					m_mapFileOffsets[md[i].md] = md[i].nFileInfoOffset ;
				}
				delete []md;
			}
#else
			//每次总是顺序读出map，所以删除map时，需要把删除的map node与最后一个颠倒
			tMD5 md ;
			for (unsigned int i = 0; i<md5map.nUsedNum; ++i)
			{
				if (!fread(&md,sizeof(tMD5),1,m_pPackFile))
				{
					assert(!"open md5 pair error!") ;
				}
				else
				{
					m_mapFileOffsets[md.md] = md.nFileInfoOffset ;
				}

//-				long npos = ftell(m_pPackFile) ;
				__int64 npos = _ftelli64(m_pPackFile) ;
				//- fseek(m_pPackFile,md.nFileInfoOffset,SEEK_SET) ;
				_fseeki64(m_pPackFile,(__int64)md.nFileInfoOffset,SEEK_SET) ;
				tFileInfo info ;
				fread(&info,sizeof(info),1,m_pPackFile) ;
				//- fseek(m_pPackFile,npos,SEEK_SET) ;
				_fseeki64(m_pPackFile,(__int64)npos,SEEK_SET) ;
			}
#endif
			pos = md5map.nNextMapHeader ;
		}
	}

	bool CanAddMd5ItemToPak(FILE* f,int num)
	{
		fseek(f,0,SEEK_END) ;
		unsigned int file_size = ftell(f) ;
		unsigned int addsz=sizeof(tMapHeader)+sizeof(tMD5)*num; //32bit机器上 当num=8196时 这个数值=196780字节
		long long int total=file_size+addsz;
		if(total>4000000000) //约等于4g
			return false;
		return true;
	}
	extern void MakeMD5(const char* sString,md5_sum& md5) ;
	int	 CPack::AddMD5Item(tFileInfo &info,unsigned int nFilePos)
	{
		unsigned int pos = m_PackHeader.nFirstMapHeader ;
		tMD5 md5 ;
		MakeMD5(info.sFileName,md5.md) ;
		md5.nFileInfoOffset = nFilePos ;
		FILE* f = m_pPackFile ;
		//- long nLastMapHeaderPos =0 ;
		__int64 nLastMapHeaderPos =0 ;
		while (pos)
		{
			nLastMapHeaderPos = (__int64)pos ;
			tMapHeader mapheader ;
			//- fseek(f,pos,SEEK_SET) ;
			_fseeki64(f,(__int64)pos,SEEK_SET) ;
			fread(&mapheader,sizeof(mapheader),1,f) ;
			if (mapheader.nNumPair>mapheader.nUsedNum)
			{
				unsigned int nmapPos =pos + sizeof(mapheader) +mapheader.nUsedNum * sizeof(tMD5) ;
				//found a empty map node, fill it;
				md5.nFileInfoOffset = nFilePos ;
				//- fseek(f,nmapPos,SEEK_SET) ;
				_fseeki64(f,(__int64)nmapPos,SEEK_SET) ;
				myfwrite(&md5,sizeof(md5),1,f) ;

				mapheader.nUsedNum++ ;
				//- fseek(f,pos,SEEK_SET) ;
				_fseeki64(f,(__int64)pos,SEEK_SET) ;
				myfwrite(&mapheader,sizeof(mapheader),1,f) ;
				return 0;
			}
			pos = mapheader.nNextMapHeader ;
			continue ;
		}
		//can't found a empty map node,add some empty map node to file
		assert(nLastMapHeaderPos) ;//不可能是0;

		fclose(f) ;
		f = fopen(m_strPackPath.c_str(),"a+b") ;
		//下面将要添加N个md5的结点，这里需要判断，添加后保证文件小于4G
		const int num_pair=8196;
		bool b=CanAddMd5ItemToPak(f,num_pair);
		if(!b)
		{
			CVirtualFileManager::instance()->GetLog()->LogEx("Pak文件:%s即将大于4G，添加Md5不成功",m_strPackPath.c_str());
			::MessageBoxA(0,"Pak文件即将大于4G，添加Md5不成功,详细信息见VFS日志",0,0);
			return 1;
		}
		fseek(f,0,SEEK_END) ;
		//- long mapheaderpos = ftell (f) ;
		__int64 mapheaderpos = _ftelli64 (f) ;
		tMapHeader mapheader ;
		mapheader.nNumPair = num_pair ; //by sssa2000 2012 10 29 以前是64
		mapheader.nUsedNum =1 ;
		mapheader.nNextMapHeader=0 ;
		myfwrite(&mapheader,sizeof(mapheader),1,f) ;
		//- long npos = ftell (f) ;
		__int64 npos = _ftelli64 (f) ;
		tMD5* mdbuf = new tMD5[mapheader.nNumPair];
		memcpy(mdbuf,&md5,sizeof(md5));
		size_t nwrite = myfwrite(mdbuf,sizeof(md5),mapheader.nNumPair,f) ;//一次写入8196个;

		fclose(f) ;
		delete []mdbuf;
		m_pPackFile = fopen(m_strPackPath.c_str(),"r+b") ;
		f = m_pPackFile ;
		//连上MAP HEADER;
		//- fseek(f,nLastMapHeaderPos,SEEK_SET) ;
		_fseeki64(f,nLastMapHeaderPos,SEEK_SET) ;
		fread(&mapheader,sizeof(mapheader),1,f) ;
		assert(!mapheader.nNextMapHeader); //一定是0
		if (mapheader.nNextMapHeader)
		{
			
		}
		mapheader.nNextMapHeader = (unsigned int)mapheaderpos ;
		//- fseek(f,nLastMapHeaderPos,SEEK_SET) ;
		_fseeki64(f,nLastMapHeaderPos,SEEK_SET) ;
		myfwrite(&mapheader,sizeof(mapheader),1,f) ;

		return 0;
	}
	void CPack::EraseMapItem(unsigned int pos , md5_sum & erase_md)
	{
		while(pos)
		{
			//fseek(m_pPackFile,pos,SEEK_SET) ;
			_fseeki64(m_pPackFile,(__int64)pos,SEEK_SET) ;
			tMapHeader md5map ;
			if (!fread(&md5map,sizeof(md5map),1,m_pPackFile))
			{
				assert(!"open map header error") ;
				return  ;
			}
			//now search which map node should be erased
			tMD5 md ;
			for (unsigned int i = 0; i<md5map.nUsedNum ;++i)
			{
				//- long nPos  = ftell(m_pPackFile) ;
				__int64 nPos  = _ftelli64(m_pPackFile) ;
				if (!fread(&md,sizeof(tMD5),1,m_pPackFile))
				{
					assert(!"open md5 pair error!") ;
				}
				
				if (md.md == erase_md)
				{
					//- long nLast = (long)(pos + sizeof(tMapHeader)+ (md5map.nUsedNum-1)*sizeof(tMD5)) ;
					__int64 nLast = (__int64)(pos + sizeof(tMapHeader)+ (md5map.nUsedNum-1)*sizeof(tMD5)) ;

					//- fseek(m_pPackFile,nLast,SEEK_SET) ;
					_fseeki64(m_pPackFile,nLast,SEEK_SET) ;
					fread(&md,sizeof(tMD5),1,m_pPackFile) ;

					//- fseek(m_pPackFile,nPos,SEEK_SET) ;
					_fseeki64(m_pPackFile,nPos,SEEK_SET) ;
					myfwrite(&md,sizeof(tMD5),1,m_pPackFile); //到此，交换了最后一个MAP NODE与当前删除的MAP NODE位置

					md5map.nUsedNum--; //减少一个map node下次增加file时，md5直接增加在此map node里

					//- fseek(m_pPackFile,pos,SEEK_SET) ;
					_fseeki64(m_pPackFile,(__int64)pos,SEEK_SET) ;
					myfwrite(&md5map,sizeof(md5map),1,m_pPackFile) ;
					return ;
				}
			}
			pos = md5map.nNextMapHeader ;
		}

		assert(!"can't erase map node while delete a file!") ;
	}

//	extern const char* const MakePurePathString(const char* s,const char* sReserve) ;
	extern void MakeMD5(const char* sString,md5_sum& md5) ;
	CVFile* CPack::OpenFullDirFile(const char* sFileName)
	{
		CVFile* pFile = 0;
		const char* const s = MakePurePathString(sFileName,0);
		char* sRoot =(char*) strstr(s,m_PackHeader.sRootDirName);
		if (!sRoot)
			return 0;
		sRoot+=strlen(m_PackHeader.sRootDirName);
		md5_sum md5 ;
		MakeMD5( sRoot,md5) ;
		MD5_IT it = m_mapFileOffsets.find(md5) ;
		if (it==m_mapFileOffsets.end())
			return 0;
		//- int off = it->second ;
		unsigned int off = it->second ;
		if (!off)
			return 0 ;

		pFile = CVirtualFileManager::instance()->GetEmptyFile();

		if (!pFile->Open(off,this))
		{
			pFile->Release() ;
			return 0 ;
		}
		return pFile;
	}
	bool CPack::FindFile(const char* sFileName)
	{
		if (!sFileName || !sFileName[0])
			return false ;

		md5_sum md5 ;
		MakeMD5( MakePurePathString(sFileName,m_PackHeader.sRootDirName),md5) ;
		MD5_IT it = m_mapFileOffsets.find(md5) ;
		if (it==m_mapFileOffsets.end())
		{
			const char* const s = MakePurePathString(sFileName,0);
			char* sRoot =(char*) strstr(s,m_PackHeader.sRootDirName);
			if (!sRoot)
				return false;
			sRoot+=strlen(m_PackHeader.sRootDirName);
			md5_sum md5 ;
			MakeMD5( sRoot,md5) ;
			MD5_IT it = m_mapFileOffsets.find(md5) ;
			if (it==m_mapFileOffsets.end())
				return false;
			else
				return true;

		}
		else
			return true;
		return false;
	}
	CVFile*			CPack::GetVFile(unsigned int off)
	{
		if (!off)
			return 0 ;

		CVFile* pFile = CVirtualFileManager::instance()->GetEmptyFile();

		if (!pFile->Open(off,this))
		{
			pFile->Release() ;
			return 0 ;
		}

		return pFile;
	}
	CVFile* CPack::OpenFile(const char* sFileName)
	{
		if (!sFileName || !sFileName[0])
			return 0 ;

		CVFile* pFile =0 ;
		md5_sum md5 ;
		MakeMD5( MakePurePathString(sFileName,m_PackHeader.sRootDirName),md5) ;
		MD5_IT it = m_mapFileOffsets.find(md5) ;
		if (it==m_mapFileOffsets.end())
		{
			return OpenFullDirFile(sFileName);
		}
		
		//- int off = it->second ;
		unsigned int off = it->second ;

		
		return GetVFile(off) ;
	}
	void CPack::ReturnVFile(CVFile* f)
	{
		H3D::H3DVector<CVFile*>::iterator it = CVirtualFileManager::instance()->m_vFiles.begin() ;
		while (it!=CVirtualFileManager::instance()->m_vFiles.end())
		{
			if (*it == f)
			{
				CVirtualFileManager::instance()->m_vFiles.erase(it) ;
				CVirtualFileManager::instance()->m_vUnUsedFiles.push_back(f) ;
				return ;
			}
			++it ;
		}

		assert(!"ERROR, return to pack an invalid CVFile!") ;
	}


	const char* const  CPack::PrepareFileOp(const char* sFileName,md5_sum& md,int func)
	{
		const char* const sPath = MakePurePathString(sFileName,m_PackHeader.sRootDirName,func) ;


		if (!sPath)
		{
		//	assert(sPath) ; 相对路径可以是0，说明func=1时，所给文件目录不是shaders/ 以下的
			return 0 ;
		}

		MakeMD5(sPath,md) ;

		return sPath ;
	}

	void StreamIn(FILE* fobj, unsigned int nLength,FILE* f)
	{
		unsigned int i=0 ;
		//by sssa2000 2012 12 17
		//这个原来是一次读8k 拷贝文件的时候太慢了 我给改成1M了
		const  unsigned int nStep = 1024*1024;//8192 ;
		unsigned int  nStep1  = nStep ;
		char* s = new char[nStep] ;
		while (i < nLength)
		{
			if ( i +nStep > nLength)
			{
				nStep1 = nLength - i ;
			}

			size_t nS = fread(s,nStep1,1,fobj) ;
			if (nS>0)
				myfwrite(s,nStep1,1,f) ;
			else
			{
				assert(0) ;
			}

			i += nStep1 ;
		}
		delete []s ;
	}
	unsigned int CPack::FindValidHole(unsigned int nPos,unsigned int nSize,tDataSegHeader& dataseg)
	{
		while (nPos)
		{
			//- fseek(m_pPackFile,nPos,SEEK_SET) ;
			_fseeki64(m_pPackFile,(__int64)nPos,SEEK_SET) ;
			fread(&dataseg,sizeof(dataseg),1,m_pPackFile) ;
			if (dataseg.nSize>=nSize)
				return nPos ;

			if(nPos==dataseg.nNextDataSeg)
				return 0;

			nPos = dataseg.nNextDataSeg ;
		}
		return 0 ;
	}
	bool GetFileTime(const char* sFileName ,FILETIME * fileTime)
	{
		HANDLE hFile = ::CreateFile (sFileName,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_READONLY,
			NULL) ;

		if(!::GetFileTime(hFile,	NULL,NULL,fileTime))
		{
			fileTime->dwHighDateTime=0;
			fileTime->dwLowDateTime =0;
			return false ;
		}
		::CloseHandle(hFile) ;
		return true ;
	}

	int addfile_ref = 0;
	int				CPack::AddFile(const char* sFileName,const char *sFileDiskPath)
	{
		OutputDebugStringA("===VFS: add file start===\n");
		addfile_ref ++;
		if (!sFileDiskPath)
			sFileDiskPath = sFileName;//如果没提供，就用第一个。
		//find location
		//check same file
		//add to end of package
		//adjust chain in package
		md5_sum md5sum ;
		const char* const sPath = PrepareFileOp(sFileName,md5sum,1) ;
		if (!sPath)
		{
			CVirtualFileManager::instance()->GetLog()->LogEx("	Error! PrepareFileOp失败!");
			return PACK_NO_DIR ;
		}

		MD5_IT it = m_mapFileOffsets.find(md5sum) ;
		if (it!=m_mapFileOffsets.end())
		{
			//has a same file, delete it first
			int res = ReplaceFileVFS(sFileName,sFileDiskPath) ;
			return res ;
		}

		//add file
		//1, find emtpy map node, data seg,
		//2, if space is big enough, put file in
		//3, if space is not big enough or there's no empty data hole, add a map node, file info, and file data at the end of file
		FILE* f = fopen(sFileDiskPath,"rb") ;//fix1

		if (!f)
		{
			assert(!"无法打开磁盘对应文件");
			CVirtualFileManager::instance()->GetLog()->LogEx("	Error!无法打开磁盘对应文件%s",sFileDiskPath);
			return -1 ;
		}
		//- fseek(f,0,SEEK_END) ;
		_fseeki64 (f,0,SEEK_END) ;
		//- unsigned int nSize = (unsigned int)ftell(f) ;
		unsigned int nSize = (unsigned int)_ftelli64(f) ;
		tDataSegHeader dataseg ;
		unsigned int nPos =0 ;

		//by sssa2000 2012 1217
		//去掉这句话，让添加文件时变快一些
		//if (m_PackHeader.nFirstDataHole)
		//	nPos = FindValidHole(m_PackHeader.nFirstDataHole,nSize,dataseg) ;
		
		//close pack file then re open it
		ReOpenPackage("r+b") ;
		tFileInfo info ;
		info.LocalSize = info.nSize  = nSize ;
		const char* const sPureName = MakePurePathString(sFileName,m_PackHeader.sRootDirName) ;
		assert (sPureName) ;
		strcpy(info.sFileName ,sPureName) ;
		FILETIME fileTime ;
		GetFileTime(sFileName,&fileTime) ;
		info.nTimeLow = fileTime.dwLowDateTime ;
		info.nTimeHigh = fileTime.dwHighDateTime ;
		unsigned int nNewNodePos=nPos ;
		if (1)
		{
			if (!nPos)
			{
				//- fseek(m_pPackFile,0,SEEK_END) ;
				_fseeki64(m_pPackFile,0,SEEK_END) ;
				//- nNewNodePos = (unsigned int)ftell(m_pPackFile) ;
				nNewNodePos = (unsigned int)_ftelli64(m_pPackFile) ;
			}

			//append data to package
			//1,find location
			//2,write data
			//3,change chain of filenode

			if (!nPos )
			{//no empty hole valid, append the package file!
				ReOpenPackage("ab") ;
				myfwrite(&info,sizeof(info),1,m_pPackFile) ;
				dataseg.nSize = info.nSize ;
				myfwrite(&dataseg,sizeof(dataseg),1,m_pPackFile) ;
				rewind(f) ;
				//fseek(f,0,SEEK_END);
				StreamIn(f,nSize,m_pPackFile) ;
				ReOpenPackage("r+b") ;
				OutputDebugString("---append the file !\n") ;
			}
			else
			{//there's a empty hole valid,fill file in,and process data seg chain

				//modify at 2008-3-26, write file first
				//- fseek(m_pPackFile,nPos+sizeof(info)+sizeof(tDataSegHeader),SEEK_SET) ;
				_fseeki64(m_pPackFile,(__int64)(nPos+sizeof(info)+sizeof(tDataSegHeader)),SEEK_SET) ;
				rewind(f);
				StreamIn(f,nSize,m_pPackFile) ;

				//把这个空洞拿掉。假如在这个时候断开，空洞链表紊乱，不会影响目录系统
				ProcessDataseg(dataseg,nSize,nPos) ;

				//fill the file info
				//- fseek(m_pPackFile,nPos,SEEK_SET) ;
				_fseeki64(m_pPackFile,(__int64)nPos,SEEK_SET) ;
				info.LocalSize = nSize ;
				myfwrite(&info,sizeof(info),1,m_pPackFile) ;

				OutputDebugString("---over write the file !\n") ;
			}


			//改变文件MD5索引表,如果这个时候断电，文件无法正确加入，索引表结构被破坏!
			int nMD5FilePos = AddMD5Item(info,nNewNodePos) ;
		}

		m_mapFileOffsets[md5sum] = nNewNodePos ;
		//- fseek(m_pPackFile,0,SEEK_SET) ;
		_fseeki64(m_pPackFile,0,SEEK_SET) ;
		myfwrite(&m_PackHeader,sizeof(m_PackHeader),1,m_pPackFile) ;
		fclose(f) ;

		--addfile_ref;
		
		printf("===VFS: add file finished===\n");
		OutputDebugStringA("===VFS: add file finished===\n");
	//	ReOpenPackage("rb") 
		return 0 ;
		

	}
	void CPack::DelDataHole(tDataSegHeader& dataseg,tDataSegHeader& lastSeg,tDataSegHeader& nextSeg)
	{
		if (!dataseg.nLastDataSeg)
		{//it's the first of the data hole in file
			m_PackHeader.nFirstDataHole = dataseg.nNextDataSeg ;
		}
		else
		{//it has last node
			lastSeg.nNextDataSeg = dataseg.nNextDataSeg ;
			//- fseek(m_pPackFile,dataseg.nLastDataSeg,SEEK_SET) ;
			_fseeki64(m_pPackFile,(__int64)dataseg.nLastDataSeg,SEEK_SET) ;
			myfwrite(&lastSeg,sizeof(lastSeg),1,m_pPackFile) ;
		}

		if (dataseg.nNextDataSeg)
		{
			nextSeg.nLastDataSeg = dataseg.nLastDataSeg ;
			//- fseek(m_pPackFile,dataseg.nNextDataSeg,SEEK_SET) ;
			_fseeki64(m_pPackFile,(__int64)dataseg.nNextDataSeg,SEEK_SET) ;
			myfwrite(&nextSeg,sizeof(nextSeg),1,m_pPackFile) ;
		}

	}
	void CPack::InsertDataHole(tDataSegHeader& dataseg , unsigned int nPos)
	{
		if (m_PackHeader.nFirstDataHole)
		{
			dataseg.nNextDataSeg = m_PackHeader.nFirstDataHole ;
			tDataSegHeader segFirst ;
			//- fseek(m_pPackFile,m_PackHeader.nFirstDataHole,SEEK_SET) ;
			_fseeki64(m_pPackFile,(__int64)m_PackHeader.nFirstDataHole,SEEK_SET) ;
			fread(&segFirst,sizeof(segFirst),1,m_pPackFile)  ;
			segFirst.nLastDataSeg = nPos ;
			//- fseek(m_pPackFile,m_PackHeader.nFirstDataHole,SEEK_SET) ;
			_fseeki64(m_pPackFile,(__int64)m_PackHeader.nFirstDataHole,SEEK_SET) ;
			myfwrite(&segFirst,sizeof(segFirst),1,m_pPackFile)  ;
		}

		
		m_PackHeader.nFirstDataHole = nPos ;
		dataseg.nLastDataSeg =0 ;
		//- fseek(m_pPackFile,nPos,SEEK_SET) ;
		_fseeki64(m_pPackFile,(__int64)nPos,SEEK_SET) ;
		myfwrite(&dataseg,sizeof(dataseg),1,m_pPackFile)  ;

	}
	void CPack::ProcessDataseg(tDataSegHeader& dataseg,unsigned int nNewFileSize,unsigned int nHolePos)
	{
		tDataSegHeader segLast,segNext ;
		if (dataseg.nLastDataSeg)//it's the first data hole
		{
			//- fseek(m_pPackFile,dataseg.nLastDataSeg,SEEK_SET) ;
			_fseeki64(m_pPackFile,(__int64)dataseg.nLastDataSeg,SEEK_SET) ;
			fread(&segLast,sizeof(segLast),1,m_pPackFile) ;
		}
		if (dataseg.nNextDataSeg)
		{
			//- fseek(m_pPackFile,dataseg.nNextDataSeg,SEEK_SET) ;
			_fseeki64(m_pPackFile,(__int64)dataseg.nNextDataSeg,SEEK_SET) ;
			fread(&segNext,sizeof(segNext),1,m_pPackFile) ;
		}

		//change the hole chain first
		DelDataHole(dataseg,segLast,segNext) ;

		unsigned int nSizeLeft = dataseg.nSize - nNewFileSize ;
		if (nSizeLeft < sizeof(tFileInfo) + sizeof(tDataSegHeader) + 1024)
		{
			//can't split the dataseg ,write it directly
		}
		else
		{
			//split the dataseg
			unsigned int nextPos = nHolePos + sizeof(tFileInfo)+sizeof(tDataSegHeader)+nNewFileSize ;
			dataseg.nSize = nNewFileSize ;
			tDataSegHeader newDataHole  ;
			newDataHole.nSize = nSizeLeft - sizeof(tFileInfo) - sizeof(tDataSegHeader) ;

			//generate a new dataseg,and insert the data hole chain
			InsertDataHole(newDataHole , nextPos) ;
		}

		//write the current datahole
		
		//- fseek(m_pPackFile,nHolePos + sizeof(tFileInfo),SEEK_SET) ;
		_fseeki64(m_pPackFile,(__int64)(nHolePos + sizeof(tFileInfo)),SEEK_SET) ;
		myfwrite(&dataseg,sizeof(dataseg),1,m_pPackFile) ;
	}




	int			CPack::	DelFileVFS(const char* sFileName)
	{
		OutputDebugStringA("===VFS: del file start===\n");
		md5_sum md5sum ;
		const char* const sPath  = PrepareFileOp(sFileName,md5sum) ;

		MD5_IT it = m_mapFileOffsets.find(md5sum) ;
		if (it==m_mapFileOffsets.end())
		{
			CVirtualFileManager::instance()->GetLog()->LogEx("	Error! DelFile失败!没有该文件索引:%s",sFileName);
			return PACK_NO_FILE ;
		}

		ReOpenPackage("r+b") ;

		//- int nEraseFilePos = it->second ;
		unsigned int nEraseFilePos = it->second ;
		fseek(m_pPackFile,it->second,SEEK_SET) ;
		tFileInfo info ;
		fread(&info,sizeof(info),1,m_pPackFile) ;
		tDataSegHeader orgData ;
		fread(&orgData,sizeof(orgData),1,m_pPackFile) ;

		//now, change the chain in file, erase all info about this file from map header,pack header,and other...
		//file header
		m_PackHeader.nNumFile-- ;
		//map header;
		m_mapFileOffsets.erase(it) ;
		EraseMapItem( (unsigned int)sizeof(tFileHeader),md5sum) ;

		//data seg

		tDataSegHeader dataseg ;
		dataseg.nFlag =1 ;//empty;
		dataseg.nSize = orgData.nSize;   //原数据块大小，此数据段实际能存放最大的尺寸

		InsertDataHole(dataseg,nEraseFilePos) ;

		//write pack header
		fseek(m_pPackFile,0,SEEK_SET) ;
		myfwrite(&m_PackHeader,sizeof(m_PackHeader),1,m_pPackFile) ;
		//merge data hole
		//把相连接的数据空洞融合起来，形成更大的数据空洞
		//MergeDataHole() 

		OutputDebugStringA("===VFS: del file finsihed===\n");
		return 0 ;
	}

	int			CPack::	ReplaceFileVFS(const char* sFileName,const char*sFileDiskPath)
	{
		int res1 = DelFileVFS(sFileName);
		int res2 = AddFile(sFileName,sFileDiskPath);
		return res1 |res2 ;
	}
	int			CPack::	MvFile(const char* sFileName,const char* sDstFileName)
	{
		return 0 ;
	}
	int			CPack::	CheckFile(const char* sFileName,int& size,unsigned int nLow,unsigned int nHigh)
	{
		return 0 ;
	}


	void			CPack::ReOpenPackage(const char* sOpenFlag)
	{
		fclose(m_pPackFile) ;
		m_pPackFile = fopen(m_strPackPath.c_str(),sOpenFlag) ;
	}

	const char res1[] = "无效文件路径!" ;
	const char res3[] = "无法从磁盘打开比较文件:" ;
	const char res4[] = "磁盘文件大小不相等:" ;
	const char res5[] = "磁盘文件读取大小与PACKAGE中不符:" ;
	const char res6[] = "文件内容比较不符：" ;
	const char res2[] = "无法在PACKAGE中找到相应文件!" ;
	const char res7[] = "文件比较成功:" ;
	void FillResString(const char* s1,const char* s2,char* sRes,size_t nMaxLength,size_t& nCurPos)
	{
		if (!sRes)
			return;

		char* sResStr = sRes + nCurPos ;
		if (!s1)
			return ;

		size_t len = strlen(s1) ;
		if (s2)
			len+=strlen(s2) ;

		len +=2 ;

		if (nMaxLength<= len+ nCurPos)
			return ;

		if(s2)
			sprintf(sResStr,"%s%s\n\r",s1,s2) ;
		else 
			sprintf(sResStr,"%s\n\r",s1) ;

		FILE* f = fopen("c:/vfs_compare.log","a");
		if (f)
		{
			fprintf(f,sResStr);
			fclose(f);
		}

		nCurPos+=(int)len ;
	}
	int CPack::CompareFile(const char* sItemName,const char* sDiskDir,char* sResString,size_t nMaxLength,size_t& nCurPos)
	{
		m_nCompRes[1] ++; //add error first cos this func can exit anytime
		char *sResStringCur  = sResString+nCurPos ;
		const char* const s = MakePurePathString(sItemName,0) ;
		if (!s)
		{
			FillResString(res1,sItemName,sResString,nMaxLength,nCurPos) ;
			return 1 ;
		}

		md5_sum md ;
		MakeMD5(s,md) ;
		MD5_IT it= m_mapFileOffsets.find(md) ;
		if (it==m_mapFileOffsets.end())
		{
			FillResString(res2,s,sResString,nMaxLength,nCurPos) ;
			return 2 ;
		}

		fseek(m_pPackFile,it->second,SEEK_SET) ;
		tFileInfo info ;
		fread(&info,sizeof(info),1,m_pPackFile) ;
		//move to file content position
		fseek(m_pPackFile,sizeof(tDataSegHeader),SEEK_CUR) ;

		char sfullpath[256] ;
		strcpy(sfullpath,sDiskDir) ;
		strcat(sfullpath,s) ;
		FILE* ff  = fopen(sfullpath,"rb") ;
		if (!ff)
		{
			FillResString(res3,sfullpath,sResString,nMaxLength,nCurPos) ;
			return 3 ;
		}


		char *buf =0 ;
		if (info.nSize)
			buf = new char[info.nSize] ;

		fseek(ff,0,SEEK_END) ;
		//- long file_size = ftell(ff) ;
		unsigned int file_size = ftell(ff) ;
		fseek(ff,0,SEEK_SET) ;
		if (file_size!=info.nSize)
		{
			FillResString(res4,sItemName,sResString,nMaxLength,nCurPos) ;
			fclose(ff) ;
			return 5 ;
		}

		if (!info.nSize)
		{
			//same!,but size=0
			m_nCompRes[1]-- ;
			m_nCompRes[0]++ ;
	//		FillResString(res7,sItemName,sResString,nMaxLength,nCurPos) 
			fclose(ff) ;
			return 0 ;
		}


		size_t size = fread(buf,info.nSize,1,m_pPackFile) ;
		if (!size)
		{
			FillResString(res5,sfullpath,sResString,nMaxLength,nCurPos) ;
			fclose(ff) ;
			return 2 ;
		}


		char* buf1 = new char[file_size] ;
		fread(buf1,file_size,1,ff) ;
		size_t i=  0;
		for (i=  0; i<file_size; ++i)
		{
			if (buf[i]!=buf1[i])
			{
				FillResString(res6,sItemName,sResString,nMaxLength,nCurPos) ;
		//		Beep(440,2) 
				break ;
			}
		}

		if (i==file_size)
		{
			//same!
			m_nCompRes[1]-- ;
			m_nCompRes[0]++ ;
	//		FillResString(res7,sItemName,sResString,nMaxLength,nCurPos) 
		}

		delete []buf ;
		delete []buf1 ;
		fclose(ff) ;

		return 0 ;
	}
	static int nLayerStart=0 ;
	int				CPack::Compare(const char* sItemName,const char* sDiskDir,char* sResString,size_t nMaxLength)
	{
		if (nMaxLength>0 && nMaxLength<128)
		{
			assert(!"比较磁盘文件函数所给结果字串长度太短！必须大于128") ;
			return -1 ;
		}
		memset(m_nCompRes,0,16*sizeof(int)) ;
	 
		int nRes =0 ;
		size_t  nCurPos=0 ;
		nLayerStart=0 ;
		if(sResString)
			sResString[0]=0 ;

		return r_Compare(MakePurePathString(sItemName,0),sDiskDir,sResString,(size_t)nMaxLength,nCurPos) ;
	}

	int CPack::r_Compare(const char* sItemName,const char* sDiskDir,char* sResString,size_t nMaxLength,size_t & nCurPos)
	{

		return 0 ;
	}

	int CPack::r_CompareDisk(const char* sDirName,char* sResString,size_t nMaxLength,size_t& nCurPos)
	{

		return 0 ;
	}
	int CPack::CompareDisk(const char* sDirName,char* sResString,size_t nMaxLength,size_t& nCurPos)
	{
		sResString[0]=0 ;
		memset(m_nCompRes,0,16*sizeof(int)) ;
		r_CompareDisk(sDirName,sResString,nMaxLength,nCurPos) ;
		return 0 ;
	}

	int	CPack::FragmentClean()
	{
		return 0;
	}



	int				CPack::getTotalFileNum()
	{
		return (int)m_mapFileOffsets.size();
	}
	CVFile*  		CPack::getFirstFile(int &res)
	{
		
		m_map_it = m_mapFileOffsets.begin();
		if (m_map_it!=m_mapFileOffsets.end())
		{
			m_flags|=PACK_CAN_FILE_IT;
			res = PACK_NO_ERROR;
			return GetVFile(m_map_it->second);
		}
		else
		{
			m_flags&=~PACK_CAN_FILE_IT;
			res =PACK_NO_FILE;
		}
		
		return 0;
	}
	CVFile*		CPack::getNextFile(int & res)
	{
		if (!(m_flags&PACK_CAN_FILE_IT))
		{
			res = PACK_NO_FILE_IT;
			return 0;
		}

		++m_map_it;
		if (m_map_it==m_mapFileOffsets.end())
		{
			m_flags&=~PACK_CAN_FILE_IT;
			res = PACK_NO_FILE_IT;
			return 0;
		}
			
		res =PACK_NO_ERROR;
		return GetVFile(m_map_it->second);
		
	}

	int CPack::GetMapHeader_ForUT(int idx,tMapHeader& md5map)
	{
#ifdef VFS_UNIT_TEST
		unsigned int pos = m_PackHeader.nFirstMapHeader ;
		_fseeki64(m_pPackFile,pos,SEEK_SET) ;
		fread(&md5map,sizeof(md5map),1,m_pPackFile);

		while (idx>=0 && md5map.nNextMapHeader > 0)
		{
			_fseeki64(m_pPackFile,(__int64)md5map.nNextMapHeader,SEEK_SET) ;
			fread(&md5map,sizeof(md5map),1,m_pPackFile);
			--idx;
		}

		if(idx>0) 
			return 0;
		return 1;
#endif
		return 0;
	}

	CVFile*	CPack::getFileByIdx(unsigned int idx)
	{
		if(idx>=m_mapFileOffsets.size())
			return 0;

		auto itr=m_mapFileOffsets.begin();
		std::advance(itr,idx);
		return GetVFile(itr->second);
	}

	int	CPack::GetFileInfoByPos(unsigned int pos,tFileInfo& s)
	{
		FILE* f = GetHandle() ;
		if (_fseeki64(f,(__int64)pos,SEEK_SET))
		{
			assert(!"error , CVFile seek header error!") ;
			return 0;
		}
		if (fread(&s,sizeof(s),1,f)!=1)
		{
			assert(!"error, CVFile read header error!") ;
			return 0;
		}
		return 1;
	}

	int	CPack::GetFileInfoByIdx(unsigned int idx,tFileInfo& s)
	{
		if(idx>=m_mapFileOffsets.size())
			return 0;

		auto itr=m_mapFileOffsets.begin();
		std::advance(itr,idx);
		unsigned int pos=itr->second;

		FILE* f = GetHandle() ;
		if (_fseeki64(f,(__int64)pos,SEEK_SET))
		{
			assert(!"error , CVFile seek header error!") ;
			return 0;
		}
		if (fread(&s,sizeof(s),1,f)!=1)
		{
			assert(!"error, CVFile read header error!") ;
			return 0;
		}
		return 1;
	}

	unsigned int CPack::GetFilePosByIdx(unsigned int idx)
	{
		if(idx>=m_mapFileOffsets.size())
			return 0;

		auto itr=m_mapFileOffsets.begin();
		std::advance(itr,idx);
		unsigned int pos=itr->second;

		return pos;
	}

	unsigned int CPack::GetFilePosByFileName(const char* sFileName)
	{
		md5_sum md5 ;
		MakeMD5( MakePurePathString(sFileName,m_PackHeader.sRootDirName),md5) ;

		auto it = m_mapFileOffsets.find(md5) ;
		if(it==m_mapFileOffsets.end())
		{
			CVirtualFileManager::instance()->GetLog()->LogEx("	Error! 该pak中无该文件:%s",sFileName);
			return 0 ;
		}
		return it->second;
	}
}//namespace