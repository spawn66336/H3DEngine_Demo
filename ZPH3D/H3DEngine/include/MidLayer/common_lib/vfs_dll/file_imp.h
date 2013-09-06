#ifndef _VFILE
#define _VFILE


#include "filesystem.h"
using namespace std;
namespace VFS
{
	extern size_t myfwrite(void *buffer,
		size_t size,
		size_t count,FILE* f);
	enum//open flags
	{
		OPEN_BIN=1<<0,
		OPEN_TEXT=1<<1,
		OPEN_READ=1<<2,
		OPEN_WRITE=1<<3,
		OPEN_APPEND=1<<4,
	} ;
	class CPack ;
	class CVFile :public IFile
	{
		friend class CPack;
	public :
		CVFile() ;
		~CVFile() ;

		FILE*			GetCFileHandle() ;
		size_t			Read(  void *buffer,	size_t size,size_t count) ;
		size_t			Write(void* buffer,size_t size,size_t count) ;
		size_t			GetSize() ;
		__int64			GetTime() ;
		size_t			CurPosition() ;
		bool			Open(unsigned int off,CPack* pPack) ;
		bool			Open(const char* sFileName,const char* sFlags);
		void			Release() ;
		size_t			Append(void* buffer,size_t size,size_t count) ;
		long			Tell() ;
		int				Seek(long offset,int origin) ;
		int				Close(IFile** pThis) ;
		void			SetItor(H3D::H3DVector<CVFile*>::iterator & itor){m_itor = itor;m_nNull=1;};
		int				Eof();
		char *			GetS( char *string,	int n ) ;
		int				GetC( ) ;
		void 			Printf(char* format,...);
		void			SetCurPosAfterScan();
		CPack*			GetPack()const {return m_pPack;};
		const char*		GetFileName()const;
	protected:
		CPack*			m_pPack ;
		tFileInfo		m_FileInfo ;
		unsigned int	m_nPos ;
		unsigned int	m_nCurPos;
		int				m_nFlags; //open flags;
		FILE*			m_pFile ;
		FILE*			m_pTempFile;
		unsigned int	m_nNull;
		unsigned int	m_nLastPos;
		H3D::H3DVector<CVFile*>::iterator m_itor ;
		static H3D::H3DVector<CVFile*> vZERO ;
		static unsigned int nTempFileCount;
	} ;
}
#endif