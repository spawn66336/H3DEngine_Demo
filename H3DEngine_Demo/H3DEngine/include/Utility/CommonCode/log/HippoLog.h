/********************************************************************
	created:	2013/01/23
	created:	23:1:2013   15:14
	filename: 	D:\Code\X52\Engine_MT_Main\TestCode\Utility\CommonCode\log\HippoLog.h
	file path:	D:\Code\X52\Engine_MT_Main\TestCode\Utility\CommonCode\log
	file base:	HippoLog
	file ext:	h
	author:		sssa2000
	
	purpose:	
*********************************************************************/

#include <fstream>
#include <Windows.h>


class HippoFunLogObj
{
public:
	HippoFunLogObj(const char* str);
	~HippoFunLogObj();
private:
	std::string m_content;
};


class HippoLog
{
public:
	HippoLog();
	~HippoLog();

	//提供给一个基本的名字，生成一个文件名，生成规则是：baseName+进程id+时间.log
	static std::string GenerateFileName(const char* baseName);

	bool InitLogFile(const char* fn);

	//简单的输出字符串
	void Log(const char* str);

	//提供格式化输出，并加上前缀（时间、进程id、线程id）
	void LogEx(const char* str,...);

private:

	class HippoLogLock
	{
	public:
		HippoLogLock();
		~HippoLogLock();

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
	HippoLogLock m_lock;
};