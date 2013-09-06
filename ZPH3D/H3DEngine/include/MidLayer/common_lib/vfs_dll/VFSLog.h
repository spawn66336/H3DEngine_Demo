/********************************************************************
	created:	2012/03/19
	created:	19:3:2012   17:20
	filename: 	e:\x51_svn\2.1.5\vfs_dll\VFSLog.h
	file path:	e:\x51_svn\2.1.5\vfs_dll
	file base:	VFSLog
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/

#include <fstream>
#include <Windows.h>

namespace VFS
{
	class VFSLog
	{
	public:
		VFSLog();
		~VFSLog();

		bool InitLogFile(const char* fn);
		//�򵥵�����ַ���
		void Log(const char* str);

		//�ṩ��ʽ�������������ǰ׺��ʱ�䡢����id���߳�id��
		void LogEx(const char* str,...);

	private:

		class LogLock
		{
		public:
			LogLock();
			~LogLock();

			void lock();
			void unlock();
		private:
			CRITICAL_SECTION m_cs;
		};

		///�ļ���
		std::ofstream* m_pFile;
		///process id
		int m_ProcessID;

		///lock
		LogLock m_lock;
	};
}