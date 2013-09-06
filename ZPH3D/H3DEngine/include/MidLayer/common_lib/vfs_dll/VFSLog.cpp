#include "VFSLog.h"
#include <time.h>
#include <stdio.h>
#include <stdarg.h>

namespace VFS
{
	VFSLog::LogLock::LogLock()
	{
		InitializeCriticalSection(&m_cs);
	}
	VFSLog::LogLock::~LogLock()
	{
		DeleteCriticalSection(&m_cs);
	}

	void VFSLog::LogLock::lock()
	{
		EnterCriticalSection(&m_cs);
	}
	void VFSLog::LogLock::unlock()
	{
		LeaveCriticalSection(&m_cs);
	}

	//////////////////////////////////////////////////////////////////////////

	VFSLog::VFSLog()
	{
		m_pFile=0;
		m_ProcessID = ::GetCurrentProcessId();
	}

	VFSLog::~VFSLog()
	{
		if (m_pFile)
		{
			m_pFile->close();
			delete m_pFile;
			m_pFile=0;
		}
	}

	//该函数不能多线程访问 只能主线程访问
	bool VFSLog::InitLogFile(const char* fn)
	{
		m_pFile=new std::ofstream;
		m_pFile->open(fn);

		return true;
	}

	void VFSLog::Log(const char* str)
	{
		
		if(!m_pFile)
			return;

		m_lock.lock();
		(*m_pFile)<<str<<std::endl;
		m_lock.unlock();
	}

	void VFSLog::LogEx(const char* szString,...)
	{
		if(!m_pFile)
			return;

		m_lock.lock();
		static char szParsedString[1024];

		//时间
		//get time
		time_t ttime;
		tm* pCurTime;
		time( &ttime );
		pCurTime= localtime( &ttime );
		strftime( szParsedString, 32, "%H:%M:%S", pCurTime );
		(*m_pFile)<<szParsedString;

		//进程id
		(*m_pFile)<<",ProcessID="<<m_ProcessID;
		
		//thread id
		DWORD threadid=GetCurrentThreadId();
		(*m_pFile)<<",ThreadID="<<threadid<<" :";

		//实际的日志内容
		va_list va;
		va_start( va, szString );
		vsprintf( szParsedString, szString, va );
		va_end( va );

		(*m_pFile)<<szParsedString<<std::endl;
		
		m_lock.unlock();
	}
}