#include "HippoLog.h"
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include "../CommonCode/HippoUtil/HippoGlobal.h"

HippoLog::HippoLogLock::HippoLogLock()
{
	InitializeCriticalSection(&m_cs);
}
HippoLog::HippoLogLock::~HippoLogLock()
{
	DeleteCriticalSection(&m_cs);
}

void HippoLog::HippoLogLock::lock()
{
	EnterCriticalSection(&m_cs);
}
void HippoLog::HippoLogLock::unlock()
{
	LeaveCriticalSection(&m_cs);
}

//////////////////////////////////////////////////////////////////////////

HippoLog::HippoLog()
{
	m_pFile=0;
	m_ProcessID = ::GetCurrentProcessId();
}

HippoLog::~HippoLog()
{
	if (m_pFile)
	{
		m_pFile->close();
		delete m_pFile;
		m_pFile=0;
	}
}

//该函数不能多线程访问 只能主线程访问
bool HippoLog::InitLogFile(const char* fn)
{
	m_pFile=new std::ofstream;
	m_pFile->open(fn);

	return true;
}

void HippoLog::Log(const char* str)
{

	if(!m_pFile)
		return;

	m_lock.lock();
	(*m_pFile)<<str<<std::endl;
	m_lock.unlock();
}

void HippoLog::LogEx(const char* szString,...)
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

std::string HippoLog::GenerateFileName(const char* baseName)
{
	DWORD pid = ::GetCurrentProcessId();

	char szParsedString[128];
	time_t ttime;
	tm* pCurTime;
	time( &ttime );
	pCurTime= localtime( &ttime );
	strftime( szParsedString, 32, "%H_%M_%S", pCurTime );

	char buff[256];
	sprintf_s(buff,256,"%s_%d_%s.log",baseName,pid,szParsedString);
	return std::string(buff);
}

HippoFunLogObj::HippoFunLogObj(const char* str)
{
	m_content=str;
	Hippo_WriteConsoleAndLog(CC_WHITE,"%s Begin",m_content.c_str());
}
HippoFunLogObj::~HippoFunLogObj()
{
	Hippo_WriteConsoleAndLog(CC_WHITE,"%s End",m_content.c_str());
}