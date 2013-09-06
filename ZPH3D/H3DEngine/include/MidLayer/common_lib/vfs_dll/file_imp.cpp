//#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>

#include "file_imp.h"
#include "pack.h"
#include "windows.h"
#include <assert.h>
#define fseek(a,b,c) _fseeki64(a,(__int64)b,c)


namespace VFS
{
	extern unsigned int g_ProcessId;

	H3D::H3DVector<CVFile*> CVFile::vZERO ;
	unsigned int CVFile::nTempFileCount=0;
	CVFile::CVFile()
	{
		//m_itor = ;//vZERO.end();
		m_nNull=0;
		m_nLastPos=0;
		m_pFile = 0;
		m_nCurPos = 0;
		m_nPos = 0;
		m_nNull = 0;
		m_pPack=0 ;
		memset(&m_FileInfo,0,sizeof(m_FileInfo)) ;
		m_nFlags=OPEN_BIN|OPEN_READ ;
		m_pTempFile = 0;
	}
	CVFile::~CVFile()
	{
		//if (m_itor!=vZERO.end())
		if (m_nNull)
		{
			H3D::H3DVector<CVFile*>::iterator it = CVirtualFileManager::instance()->m_vFiles.begin() ;
			while (it!=CVirtualFileManager::instance()->m_vFiles.end())
			{
				if (*it == this)
				{
					CVirtualFileManager::instance()->m_vFiles.erase(it) ;
					//	m_itor=vZERO.end();
					m_nNull=0;
					break ;
				}
				++it ;
			}

			//	assert(m_itor == vZERO.end()) ;
			assert(!m_nNull);

			if (m_pFile && !m_nPos)
				fclose(m_pFile);
		}
		m_nNull=0;
	}
	extern unsigned int g_tempFileSize;
	extern char* g_tempFileBuffer;
	FILE* CVFile::GetCFileHandle()
	{
		//add by 2008-12-2
		bool validExtension = false;

		if (strstr(m_FileInfo.sFileName,".png") ||
			strstr(m_FileInfo.sFileName,".jpg") ||
			strstr(m_FileInfo.sFileName,".jpeg") ||
			strstr(m_FileInfo.sFileName,".dds") ||
			strstr(m_FileInfo.sFileName,".bmp") ||
			strstr(m_FileInfo.sFileName,".tga") ||
			strstr(m_FileInfo.sFileName,".bp3") ||
			strstr(m_FileInfo.sFileName,".ch3") ||
			strstr(m_FileInfo.sFileName,".dm3") ||
			strstr(m_FileInfo.sFileName,".an3") ||
			strstr(m_FileInfo.sFileName,".at3") ||
			strstr(m_FileInfo.sFileName,".ac3")	||
			strstr(m_FileInfo.sFileName,".bp6") ||
			strstr(m_FileInfo.sFileName,".ch6") ||
			strstr(m_FileInfo.sFileName,".dm6") ||
			strstr(m_FileInfo.sFileName,".rm6") ||
			strstr(m_FileInfo.sFileName,".an6") ||
			strstr(m_FileInfo.sFileName,".at6") ||
			strstr(m_FileInfo.sFileName,".ac6") ||
			strstr(m_FileInfo.sFileName,".ma6") ||
			strstr(m_FileInfo.sFileName,".tp6") ||
			strstr(m_FileInfo.sFileName,".ge6") ||
			strstr(m_FileInfo.sFileName,".cl6") ||
			strstr(m_FileInfo.sFileName,".sp6") ||
			strstr(m_FileInfo.sFileName,".ef6") ||
			strstr(m_FileInfo.sFileName,".xml")
			)
		{
			validExtension = true;
		}
		if(m_nPos >=  0x7EFFFFFF && validExtension)
		{
			//create temp file
			if (g_tempFileSize<m_FileInfo.nSize)
			{
				delete []g_tempFileBuffer;
				g_tempFileBuffer = new char[GetSize()];
				g_tempFileSize = m_FileInfo.nSize;
			}
			//生成一个临时磁盘文件
			char tempName[16];
			nTempFileCount++;
			_itoa(nTempFileCount,tempName,10);
			//add by liren  2011-5-20 临时文件名加上进程ID
			char FileNameAndProecssID[32];
			_itoa(g_ProcessId,FileNameAndProecssID,10);
			strcat(FileNameAndProecssID,"_");
			strcat(FileNameAndProecssID,tempName);
			//add by liren  2011-5-20 临时文件名加上进程ID
			
			
			m_pTempFile = 0;

			m_pTempFile =fopen(FileNameAndProecssID,"w+bDT");
			if (!m_pTempFile)
			{
				//严重异常,
				FILE* flog = fopen("vfs.log","a");
				fprintf(flog,"error!生成临时文件异常 %s\n",GetFileName());
				fclose(flog);
				//NOW, OPEN FILE IN local directory

				m_pTempFile =fopen(tempName,"wb");
				if (!m_pTempFile)
				{
					flog = fopen("vfs.log","a");
					fprintf(flog,"error!本地磁盘生成临时文件异常 %s\n",GetFileName());
					fclose(flog);
					return 0;
				}
			}

			//read to buffer
			Read(g_tempFileBuffer,m_FileInfo.nSize,1);
			fwrite(g_tempFileBuffer,m_FileInfo.nSize,1,m_pTempFile);
			rewind(m_pTempFile);
		//	size_t size =  fread(g_tempFileBuffer,m_FileInfo.nSize,1,m_pFile);

		//	fwrite(g_tempFileBuffer,m_FileInfo.nSize,1,m_pTempFile);

			return m_pTempFile;

		}
		else
		{
			if(!validExtension)
			{
				MessageBoxA(NULL,"尝试从PAK获取一个不支持的文件类型!","X52",MB_OK);
			}
			return m_pFile ;
		}
	}
	size_t CVFile::Read(  void *buffer,	size_t size,size_t count)
	{
		//fseek(m_pFile,m_nCurPos,SEEK_SET);
		_fseeki64(m_pFile,(__int64)m_nCurPos,SEEK_SET);
		size_t c = fread(buffer,size,count,m_pFile) ;
		//m_nCurPos = ftell(m_pFile);
		m_nCurPos = (unsigned int)_ftelli64(m_pFile);
		return c;
	}
	size_t CVFile::Write(void* buffer,size_t size,size_t count)
	{
		//fseek(m_pFile,m_nCurPos,SEEK_SET);
		_fseeki64(m_pFile,(__int64)m_nCurPos,SEEK_SET);
		if (!m_nPos && m_pFile)
		{
			size_t c = fwrite(buffer,size,count,m_pFile);
			//m_nCurPos = ftell(m_pFile);
			m_nCurPos = (unsigned int)_ftelli64(m_pFile);
			return c;
		}
		else
		{
			assert(!"Can't write to virtual file now !");
			return 0;
		}

		if (m_nFlags & OPEN_APPEND)
		{
			__int64 n = _ftelli64(m_pFile) ;
			fclose(m_pFile) ;
			fopen(m_pPack->m_strPackPath.c_str(),"r+b") ;
			_fseeki64(m_pFile,n,SEEK_SET) ;
		}
		m_nFlags &= ~OPEN_APPEND ;
		m_nFlags |= OPEN_WRITE ;

		return fwrite(buffer,size,count,m_pFile) ;
	}
	size_t CVFile::GetSize()
	{
		//	assert(m_FileInfo.nSize ==m_nCurPos);
		return m_FileInfo.nSize ;
	}
	__int64 CVFile::GetTime()
	{
		__int64 hi,lo ;
		hi   = (__int64)m_FileInfo.nTimeHigh  ;
		lo = (__int64)m_FileInfo.nTimeLow ;
		hi = hi<<32 ;
		hi |= lo ;
		return hi ;
	}
	size_t CVFile::CurPosition()
	{
		return (unsigned int)_ftelli64(m_pFile) - m_nPos;
	}
	void CVFile::Release()
	{
		//FIXME
		if (m_nNull)
		{
			m_pPack->ReturnVFile(this) ;
		}
		//if (m_itor!=vZERO.end())
		//{
		//	m_pPack->ReturnVFile(this) ;
		//	m_itor = vZERO.end() ;
		//}

		m_pPack=0 ;
		memset(&m_FileInfo,0,sizeof(m_FileInfo)) ;
		m_nFlags=OPEN_BIN|OPEN_READ ;

		if (m_pFile && !m_nPos)
			fclose(m_pFile);

		if (m_pFile && m_nPos)
		{
			if (m_nLastPos)
				//fseek(m_pFile,m_nLastPos,SEEK_SET);
				_fseeki64(m_pFile,(__int64)m_nLastPos,SEEK_SET);
		}
		m_nLastPos=0;
		m_pFile = 0;
		m_nCurPos = 0;
		m_nPos = 0;
		m_nNull = 0;
		if (m_pTempFile)
			fclose(m_pTempFile);
		m_pTempFile = 0;
	}
	//bool CVFile::Open(int off,CPack* pPack)
	bool CVFile::Open(unsigned int off,CPack* pPack)
	{
		m_pPack = pPack ;
		if (!pPack)
			return false ;

		FILE* f = pPack->GetHandle() ;
		if (!f)
		{
			assert(!"error,CVFile get FILE handle error!") ;
			return false ;
		}
		m_pFile = f ;
		//m_nLastPos = ftell(f);
		m_nLastPos = (unsigned int)_ftelli64(f);
		m_nPos = off + sizeof(tFileInfo)+sizeof(tDataSegHeader) ;
		//if (fseek(f,off,SEEK_SET))
		if (_fseeki64(f,(__int64)off,SEEK_SET))
		{
			assert(!"error , CVFile seek header error!") ;
		}

		if (fread(&m_FileInfo,sizeof(m_FileInfo),1,f)!=1)
		{
			assert(!"error, CVFile read header error!") ;
		}

		//if (fseek(f,m_FileInfo.nNodePos,SEEK_SET))
		if (_fseeki64(f,(__int64)m_FileInfo.nNodePos,SEEK_SET))
		{
			assert(!"error , CVFile seek node error!") ;
		}


		//fseek(f,m_nPos,SEEK_SET);
		_fseeki64(f,(__int64)m_nPos,SEEK_SET);
		m_nCurPos = m_nPos;
		return true 					  ;
	}

	long CVFile::Tell()
	{

		//return ftell(m_pFile) - m_nPos ;
		return (long)m_nCurPos-m_nPos;
	}
	int CVFile::Seek(long offset,int origin)
	{
		if (origin == SEEK_SET)
		{
			m_nCurPos = m_nPos + (unsigned int)offset;
		}
		else if (origin == SEEK_END && offset<=0)
		{
			m_nCurPos = m_nPos + m_FileInfo.nSize + (unsigned int)offset;
		}
		else if (origin == SEEK_CUR)
		{
			m_nCurPos+=(unsigned int)offset;
		}
		else
			return -1;

		if (m_nCurPos >m_nPos +(int) m_FileInfo.nSize ||
			m_nCurPos <m_nPos )
			return -1;

		//int c = fseek(m_pFile,m_nCurPos,SEEK_SET) ;
		__int64 c = _fseeki64(m_pFile,(__int64)m_nCurPos,SEEK_SET) ;
		//m_nCurPos = ftell(m_pFile);
		m_nCurPos = (unsigned int)_ftelli64(m_pFile);

		//这里返回的是不准确的, 返回值是大PAK的寻址数值,没有意义
		return (int) c;

	}
	int CVFile::Close(IFile** pThis)
	{
		((CVFile*)(*pThis))->Release() ;
		*pThis = 0 ;
		return 0 ;
	}
	size_t CVFile::Append(void* buffer,size_t size,size_t count)
	{
		//fseek(m_pFile,m_nCurPos,SEEK_SET);
		_fseeki64(m_pFile,(__int64)m_nCurPos,SEEK_SET);
		if (!m_nPos && m_pFile)
		{
			size_t  c = fwrite(buffer,size,count,m_pFile);
			//m_nCurPos = ftell(m_pFile);
			m_nCurPos = (unsigned int)_ftelli64(m_pFile);
			return c;
		}
		else
		{
			assert(!"Can't write to virtual file now !");
			return 0;
		}

		if (m_nFlags & OPEN_WRITE)
		{
			fclose(m_pFile) ;
			fopen(m_pPack->m_strPackPath.c_str(),"ab") ;
		}
		m_nFlags &= ~OPEN_WRITE ;
		m_nFlags |= OPEN_APPEND ;

		return fwrite(buffer,size,count,m_pFile) ;
	}

	bool			CVFile::Open(const char* sFileName,const char* sFlags)
	{
		m_pFile = fopen(sFileName,sFlags);
		if (!m_pFile)
			return false;

		fseek(m_pFile,0,SEEK_END);
		m_FileInfo.nSize = (unsigned int )ftell(m_pFile);
		fseek(m_pFile,0,SEEK_SET);
		m_nPos = 0;
		m_nCurPos = 0;

		return true;
	}

	int CVFile::Eof()
	{
		//fseek(m_pFile,m_nCurPos,SEEK_SET);
		_fseeki64(m_pFile,m_nCurPos,SEEK_SET);

		if (!m_nPos && m_pFile)
			return feof(m_pFile);
		else if (CurPosition()>=m_FileInfo.nSize)
			return 1;
		else 
			return 0;
	}

	char *CVFile::GetS( char *string,	int n ) 
	{
		if (m_nCurPos - m_nPos >= m_FileInfo.nSize)
		{
			return 0;
		}
		//fseek(m_pFile,m_nCurPos,SEEK_SET);
		_fseeki64(m_pFile,m_nCurPos,SEEK_SET);
		char *s = fgets(string,n,m_pFile);
		//m_nCurPos = ftell(m_pFile);
		m_nCurPos = (unsigned int)_ftelli64(m_pFile);
		return s;
	}
	int			CVFile::GetC( ) 
	{
		//fseek(m_pFile,m_nCurPos,SEEK_SET);
		_fseeki64(m_pFile,m_nCurPos,SEEK_SET);
		int c = fgetc(m_pFile);
		//m_nCurPos = ftell(m_pFile);
		m_nCurPos = (unsigned int)_ftelli64(m_pFile);
		return c;
	}

	void CVFile::Printf(char* format, ...)
	{
		//fseek(m_pFile,m_nCurPos,SEEK_SET);
		_fseeki64(m_pFile,m_nCurPos,SEEK_SET);
		va_list		argptr;
		va_start(argptr,format);
		vfprintf(m_pFile,format,argptr);
		va_end(argptr);
		//m_nCurPos = ftell(m_pFile);
		m_nCurPos = (unsigned int)_ftelli64(m_pFile);
	}

	void CVFile::SetCurPosAfterScan()
	{
		//m_nCurPos = ftell(m_pFile);
		m_nCurPos = (unsigned int)_ftelli64(m_pFile);
	}

	const char*		CVFile::GetFileName()const
	{
		return m_FileInfo.sFileName;
	}
}//namepsace VFS