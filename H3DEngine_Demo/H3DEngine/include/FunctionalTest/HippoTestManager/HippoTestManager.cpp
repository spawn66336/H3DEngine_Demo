#include "HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include "engine_interface.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>
using namespace std;
using namespace boost;

const char* EXE_TYPE="";

HippoTestManager::HippoTestManager()
{
	m_pCurrentTestCase=0;
	m_TestCaseIdx=0;
	m_pCamera=0;
}

void HippoTestManager::Release()
{
	AllCaseConItr itr=m_all_testcase.begin();
	while(itr!=m_all_testcase.end())
	{
		HippoTestCaseBase* p=itr->second;
		delete p;
		p=0;
		++itr;
	}
	m_all_testcase.clear();
}
HippoTestManager::~HippoTestManager()
{
	Release();
}


// 单件
HippoTestManager* HippoTestManager::GetInstance()
{
	static HippoTestManager instance;
	return &instance;
}

//分析参数
bool HippoTestManager::ParseArg(int argc, char* argv[])
{
	char* runManual="-m";
	char* runAll="-all";
	char* runType="-t";

	int count_all = 0;
	int count_type = 0;
	int count_m = 0;
	int indexType=0;

	for (int i=0;i<argc;i++)
	{
		if (strcmp(argv[i],runManual)==0)
		    count_m++;
		if (strcmp(argv[i],runAll)==0)
			count_all++;
		if (strcmp(argv[i],runType)==0)
			{count_type++;indexType=i;}
	}
	//出现同样的参数 返回false 
	if ((count_all>1) || (count_m>1) ||(count_type>1))
	{
		Hippo_WriteConsoleAndLog(CC_RED,"同一个参数被指定两次，请检测！");
		Release();
		return false;
	}

	if (count_all)
	{
		runMode=count_m?MANUAL_RUN_ALL:AUTO_RUN_ALL;
	}
	else if (count_type)//参数 -t
	{
		runMode=count_m?MANUAL_RUN_A_TYPE:AUTO_RUN_A_TYPE;
		if (indexType+1>argc || strcmp(argv[indexType+1],runManual)==0 )
		{
			Hippo_WriteConsoleAndLog(CC_RED,"请指定要运行用例的类型！");
			Release();
			return false;
		}
		EXE_TYPE=argv[indexType+1];
	}
	else // -all -t both no
	{
		runMode=count_m?MANUAL_RUN_SPECIFIED:AUTO_RUN_SPECIFIED;
		for (int i=2;i<argc;i++)
		{
			if (strcmp(argv[i],runManual)!=0)
			{
				m_ExecTest.push_back(argv[i]);
			}
		}
	}
	return true;
}

void HippoTestManager::CreateTestCases()
{
	if (runMode == AUTO_RUN_A_TYPE || runMode ==MANUAL_RUN_A_TYPE)
	{
		AllCaseCon temp;
		auto itr =m_all_testcase.begin();
		while(itr!=m_all_testcase.end())
		{
			if (strcmp((itr->first).c_str(),EXE_TYPE)!=0)//如果不是指定的类型 删除
			{
				delete itr->second;
				itr = m_all_testcase.erase(itr);
				continue;
			}
			++itr;
		}

		Hippo_WriteConsoleAndLog(CC_WHITE,"当前运行 runtest -t %s",EXE_TYPE);
		if (m_all_testcase.size()==0)
		{
			Hippo_WriteConsoleAndLog(CC_RED,"不存在%s类型的实例",EXE_TYPE);
		}
	}
	else if (runMode == AUTO_RUN_SPECIFIED || runMode ==MANUAL_RUN_SPECIFIED)
	{
		auto itr=m_all_testcase.begin();
		while (itr!=m_all_testcase.end())//如果m_ExecTest同时包含了已定义和未定义的用例，则忽略未定义的用例
		{
			HippoTestCaseBase* pCase=itr->second;
			if(std::find(m_ExecTest.begin(),m_ExecTest.end(),pCase->GetTestCaseName())==m_ExecTest.end())
			{
				delete pCase;
				itr=m_all_testcase.erase(itr);
				continue;
			}
			++itr;
		}
		Hippo_WriteConsoleAndLog(CC_WHITE,"当前运行：rutest ");
		if (m_all_testcase.size()==0)
		{
			Hippo_WriteConsoleAndLog(CC_RED,"请指定要运行的用例！");
		}
	}
	else
	{
		Hippo_WriteConsole(CC_WHITE,"当前运行runtest -all");
	}

}
//初始化
void HippoTestManager::InitWithArg(int argc, char* argv[])
{
	if(argc>1 && ParseArg(argc,argv))
		CreateTestCases();
}
// 注册测试案例
HippoTestCaseBase* HippoTestManager::RegisterTestCase(const char* type,HippoTestCaseBase* pCase)
{
	m_all_testcase.push_back(std::make_pair(type,pCase));
	return pCase;
}

// 得到当前正在执行的test case
HippoTestCaseBase* HippoTestManager::GetCurrentTestCase()
{
	if (m_TestCaseIdx>=0 && m_TestCaseIdx<m_all_testcase.size())
	{
		return m_all_testcase.at(m_TestCaseIdx).second;
	}
	return 0;
}

// 执行单元测试
int HippoTestManager::RunAllTest()
{
	int nTestResult = 1;
	int nPassed=0;
	int nFailed=0;
	while(m_TestCaseIdx<m_all_testcase.size())
	{
		HippoTestCaseBase* testcase = m_all_testcase.at(m_TestCaseIdx).second;
		m_pCurrentTestCase = testcase;
		CASE_STATE s=m_pCurrentTestCase->GetState();
		if(s==CASE_INIT)
		{
			Hippo_WriteConsoleAndLog(CC_WHITE,"*****Begin A Test:%s*****",m_pCurrentTestCase->GetTestCaseName());
			m_pCurrentTestCase->InitTestCase();
		}
		else if (s==CASE_BEFORE_EXECING)
		{

		}
		else if (s==CASE_EXECING)
		{
			bool b=false;
			//如果有参数-m 则运行manualrun 否则运行autorun
			if(runMode== MANUAL_RUN_A_TYPE || runMode==MANUAL_RUN_ALL ||runMode==MANUAL_RUN_SPECIFIED)
				m_pCurrentTestCase->ManualRunExec();
			else
				b=m_pCurrentTestCase->AutoRunExec();
			if(b)
				m_pCurrentTestCase->SetState(CASE_END_EXECING);

			break;
		}
		else if (s==CASE_END_EXECING)
		{
			m_pCurrentTestCase->CleanUpTestCase();
			Hippo_WriteConsoleAndLog(CC_WHITE,"*****End A Test:%s*****",m_pCurrentTestCase->GetTestCaseName());
			++m_TestCaseIdx;
		}

	}

	return 1;
}

void HippoTestManager::BeforeRender()
{
	if(!m_pCurrentTestCase)
		return;

	if (m_pCurrentTestCase->GetState() != CASE_EXECING)
	{
		return;
	}

	m_pCurrentTestCase->BeforeRender();
}

void HippoTestManager::AfterRender()
{
	if(!m_pCurrentTestCase)
		return;

	if (m_pCurrentTestCase->GetState() != CASE_EXECING)
	{
		return;
	}

	m_pCurrentTestCase->AfterRender();
}




int HippoTestManager::OnKeyDown(unsigned int key)
{
	if(!m_pCurrentTestCase)
		return 0;
	if(GetCurrentRunMode()  && key==VK_RETURN)
	{
		//手动运行模式下 如果按回车表示切换到下一个用例
		m_pCurrentTestCase->SetState(CASE_END_EXECING);
		return 1;

	}
	return m_pCurrentTestCase->OnKeyDown(key);
}

int HippoTestManager::OnLeftDown(HippoMouseEvent& e)
{
	if(!m_pCurrentTestCase)
		return 0;

	return m_pCurrentTestCase->OnLeftDown(e);
}
int HippoTestManager::OnLeftUp(HippoMouseEvent& e)
{
	if(!m_pCurrentTestCase)
		return 0;

	return m_pCurrentTestCase->OnLeftUp(e);
}
int HippoTestManager::OnRightDown(HippoMouseEvent& e)
{
	if(!m_pCurrentTestCase)
		return 0;

	return m_pCurrentTestCase->OnRightDown(e);
}
int HippoTestManager::OnRightUp(HippoMouseEvent& e)
{
	if(!m_pCurrentTestCase)
		return 0;

	return m_pCurrentTestCase->OnRightUp(e);
}
int HippoTestManager::OnWheel(HippoWheelEvent& e)
{
	if(!m_pCurrentTestCase)
		return 0;

	return m_pCurrentTestCase->OnWheel(e);
}

bool HippoTestCaseBase::Spin_N_Frame(DWORD frameNum)
{
	static DWORD frameCount=0;
	++frameCount;
	if(frameNum<=frameCount)
	{
		frameCount=0;
		return true;
	}
	return false;
}

bool HippoTestCaseBase::Spin_N_Ms(DWORD timeMs)
{
	static DWORD _start=0;
	if(_start==0)
		_start=GetTickCount();

	DWORD now=GetTickCount();
	if((now-_start)>=timeMs)
	{
		_start=0;
		return true;
	}
	return false;
}
MouseKeyCallback HippoTestManager::GetMouseLeftDownCallback()
{
	MouseKeyCallback cb= boost::bind(&HippoTestManager::OnLeftDown,this,_1);
	return cb;
}

MouseKeyCallback HippoTestManager::GetMouseLeftUpCallback()
{
	MouseKeyCallback cb= boost::bind(&HippoTestManager::OnLeftUp,this,_1);
	return cb;
}
MouseWheelCallback HippoTestManager::GetMouseWheelCallback()
{
	MouseWheelCallback cb= boost::bind(&HippoTestManager::OnWheel,this,_1);
	return cb;
}
KeyCallback HippoTestManager::GetKeyDownCallback()
{
	KeyCallback cb= boost::bind(&HippoTestManager::OnKeyDown,this,_1);
	return cb;
}



bool HippoTestCaseBase::InitTestCase()
{
	InitScene();
	m_state=CASE_EXECING;
	return true;
}
bool HippoTestCaseBase::CleanUpTestCase()
{
	CleanUpScene();
	m_state=CASE_END_EXECING;
	return true;
}


bool HippoTestCase_RefCompare::InitTestCase()
{
	Hippo_GetIRender()->OpenActionLib("../resources/art/role/actions/role.xml");
	m_pShot_Begin_Mat=RefCountCompare::GetResState(HIPPO_MAT_RES);
	m_pShot_Begin_TPL=RefCountCompare::GetResState(HIPPO_TPL_RES);
	m_pShot_Begin_Tex=RefCountCompare::GetResState(HIPPO_TEX_RES);

	return  HippoTestCaseBase::InitTestCase();
}
bool HippoTestCase_RefCompare::CleanUpTestCase()
{
	bool b=HippoTestCaseBase::CleanUpTestCase();
	m_pShot_End_Mat=RefCountCompare::GetResState(HIPPO_MAT_RES);
	m_pShot_End_TPL=RefCountCompare::GetResState(HIPPO_TPL_RES);
	m_pShot_End_Tex=RefCountCompare::GetResState(HIPPO_TEX_RES);

	HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(m_pShot_Begin_Mat,m_pShot_End_Mat),true);
	HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(m_pShot_Begin_TPL,m_pShot_End_TPL),true);
	HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(m_pShot_Begin_Tex,m_pShot_End_Tex),true);

	m_pShot_Begin_Mat->Release();
	m_pShot_Begin_TPL->Release();
	m_pShot_Begin_Tex->Release();
	m_pShot_End_Mat->Release();
	m_pShot_End_TPL->Release();
	m_pShot_End_Tex->Release();

	return b;
}
