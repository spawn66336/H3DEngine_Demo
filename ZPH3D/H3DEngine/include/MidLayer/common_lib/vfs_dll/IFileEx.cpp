#include "IFileEx.h"
#include "filesystem.h"
#include "pack.h"

namespace VFS
{
	//到整个vfs文件数量
	VFS_DLL_API unsigned int GetTotalFileNum()
	{
		int pakIdx=0;
		unsigned int res=0;

		unsigned int sz=CVirtualFileManager::instance()->GetPakNum();
		for(unsigned int i=0;i<sz;++i)
		{
			CPack* p= CVirtualFileManager::instance()->GetPackByIdx(pakIdx);
			res+=p->getTotalFileNum();
		}
		return res;
	}

	//从某个pak中，通过一个idx得到一个文件，如果idx太大返回0
	VFS_DLL_API IFile* GetFileByIdx(unsigned int fileIdx)
	{
		int pakIdx=0;
		int local_pakfile_idx=fileIdx;

		CPack* p= CVirtualFileManager::instance()->GetPackByIdx(pakIdx);
		while (p)
		{
			int n=p->getTotalFileNum();
			if(local_pakfile_idx<n)
				return (IFile*)p->getFileByIdx(local_pakfile_idx);
			else
				local_pakfile_idx-=n;
			
			++pakIdx;
			p= CVirtualFileManager::instance()->GetPackByIdx(pakIdx);
		}
		return 0;

	}
}