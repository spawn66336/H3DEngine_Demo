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

	//�ṩ��һ�����������֣�����һ���ļ��������ɹ����ǣ�baseName+����id+ʱ��.log
	static std::string GenerateFileName(const char* baseName);

	bool InitLogFile(const char* fn);

	//�򵥵�����ַ���
	void Log(const char* str);

	//�ṩ��ʽ�������������ǰ׺��ʱ�䡢����id���߳�id��
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

	///�ļ���
	std::ofstream* m_pFile;
	///process id
	int m_ProcessID;

	///lock
	HippoLogLock m_lock;
};