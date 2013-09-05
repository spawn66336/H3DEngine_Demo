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
		//简单的输出字符串
		void Log(const char* str);

		//提供格式化输出，并加上前缀（时间、进程id、线程id）
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

		///文件流
		std::ofstream* m_pFile;
		///process id
		int m_ProcessID;

		///lock
		LogLock m_lock;
	};
}