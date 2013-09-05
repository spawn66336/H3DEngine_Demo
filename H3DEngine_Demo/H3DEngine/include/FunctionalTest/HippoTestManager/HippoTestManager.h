/********************************************************************
	created:	2012/07/22
	created:	22:7:2012   15:13
	filename: 	F:\EngineX52MT\多线程测试用例代码\Common\HippoTestManager\HippoTestManager.h
	file path:	F:\EngineX52MT\多线程测试用例代码\Common\HippoTestManager
	file base:	HippoTestManager
	file ext:	h
	author:		sssa2000
	
	purpose:	测试用例管理器，简化google test的代码
*********************************************************************/
#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include "../HippoAppBase/Win32MsgUtil.h"

#include "../CommonCode/HippoUtil/RefCountCompare.h"
#include "../CommonCode/HippoUtil/camera/CameraBase.h"

enum RUN_MODE
{   //自动运行 所有 某类型 指定用例
	AUTO_RUN_ALL,
	AUTO_RUN_A_TYPE,
	AUTO_RUN_SPECIFIED,
	//手动运行 所有 某类型 指定用例
	MANUAL_RUN_ALL,
	MANUAL_RUN_A_TYPE,
	MANUAL_RUN_SPECIFIED
};

enum CASE_STATE
{
	CASE_INIT=0,		//初始化状态，对应InitScene函数
	CASE_BEFORE_EXECING,//准备执行测试(目前暂时没用)
	CASE_EXECING,		//执行测试中，对应AutoRunExec或者ManualRunExec函数
	CASE_END_EXECING,	//测试执行完毕，清理。对应CleanUpScene函数
};

#define SAFE_RELEASE(x)\
	if(x)\
{\
	x->Release();\
	x = 0;\
	};\


class HippoTestManager;
class HippoTestCase_RefCompare;
//用例基类
class HippoTestCaseBase
{
public:
	friend class HippoTestManager;
	friend class HippoTestCase_RefCompare;

	HippoTestCaseBase(const char* case_name) : testcase_name(case_name)
	{
		m_state=CASE_INIT;
	}
	//
	virtual void InitScene(){}
	virtual void CleanUpScene()=0;

	virtual void BeforeRender()	{}

	// 自动执行测试案例的方法，返回true表示已经完成
	virtual bool AutoRunExec()=0;

	// 手动执行测试的方法，返回true表示已经完成
	virtual bool ManualRunExec()=0;

	//空转 n 毫秒，返回true表示时间够了
	virtual bool Spin_N_Ms(DWORD timeMs);
	virtual bool Spin_N_Frame(DWORD timeMs);

	virtual void AfterRender()	{}

	//!自定义的消息处理函数，返回0表示对该消息不感兴趣，返回1表示已经处理，不需要默认消息处理函数再处理
	virtual int OnKeyDown(unsigned int key){return 0;}
	virtual int OnLeftDown(HippoMouseEvent& e){	return 0;}
	virtual int OnLeftUp(HippoMouseEvent& e){	return 0;}
	virtual int OnRightDown(HippoMouseEvent& e){	return 0;}
	virtual int OnRightUp(HippoMouseEvent& e){	return 0;}
	virtual int OnWheel(HippoWheelEvent& e){	return 0;}

	CASE_STATE GetState(){return m_state;}
	void SetState(CASE_STATE s){m_state=s;}
	const char* GetTestCaseName(){return testcase_name.c_str();}
protected:
	CASE_STATE m_state;
	//DWORD m_timer;
	std::string testcase_name; // 测试案例名称


private:
	//以下2个函数是新增的，为了能够在testcase开始和末尾做一些事情
	virtual bool InitTestCase();
	virtual bool CleanUpTestCase();
};


//以下类在测试用例的会自动比较测试用例开启前和测试用例结束时的 引擎资源引用计数快照
class HippoTestCase_RefCompare:public HippoTestCaseBase
{
public:
	HippoTestCase_RefCompare(const char* case_name) : HippoTestCaseBase(case_name)
	{
		m_pShot_Begin_Mat=0;
		m_pShot_Begin_TPL=0;
		m_pShot_Begin_Tex=0;

		m_pShot_End_Mat=0;
		m_pShot_End_TPL=0;
		m_pShot_End_Tex=0;
	}

	//以下2个函数是新增的，为了能够在testcase开始和末尾做一些事情
	virtual bool InitTestCase();
	virtual bool CleanUpTestCase();

protected:
	HippoResStateSnapShot* m_pShot_Begin_Mat;
	HippoResStateSnapShot* m_pShot_Begin_TPL;
	HippoResStateSnapShot* m_pShot_Begin_Tex;

	HippoResStateSnapShot* m_pShot_End_Mat;
	HippoResStateSnapShot* m_pShot_End_TPL;
	HippoResStateSnapShot* m_pShot_End_Tex;
};

//测试框架实现
class HippoTestManager
{
public:
	HippoTestManager();
	~HippoTestManager();

	// 单件
	static HippoTestManager* GetInstance(); 

	//
	bool GetCurrentRunMode(){return runMode==MANUAL_RUN_SPECIFIED||runMode==MANUAL_RUN_ALL || runMode == MANUAL_RUN_A_TYPE; }

	//初始化
	void InitWithArg(int argc, char* argv[]);

	// 注册测试案例
	HippoTestCaseBase* RegisterTestCase(const char* type,HippoTestCaseBase* pCase);

	// 得到当前正在执行的test case
	HippoTestCaseBase* GetCurrentTestCase();

	// 执行单元测试，在frame begin和end之间调用
	int RunAllTest();

	void BeforeRender();

	void AfterRender();

	MouseKeyCallback GetMouseLeftDownCallback();
	MouseKeyCallback GetMouseMoveCallback();
	MouseKeyCallback GetMouseLeftUpCallback();
	MouseWheelCallback GetMouseWheelCallback();
	KeyCallback GetKeyDownCallback();


	//----------中间层更新level用
	inline void  SetCamera(CameraBase* pPam){m_pCamera = pPam;}
	inline CameraBase* GetCamera()
	{
		//if (m_pCamera)
			return m_pCamera;
	}

protected:

	virtual int OnKeyDown(unsigned int key);
	virtual int OnLeftDown(HippoMouseEvent& e);
	virtual int OnLeftUp(HippoMouseEvent& e);
	virtual int OnRightDown(HippoMouseEvent& e);
	virtual int OnRightUp(HippoMouseEvent& e);
	virtual int OnWheel(HippoWheelEvent& e);

	bool ParseArg(int argc, char* argv[]);
	void CreateTestCases();


	//std::vector<std::string> m_OnlyRun;
	//std::vector<std::string> m_NotRun;
	std::vector<std::string> m_ExecTest;


	unsigned int m_TestCaseIdx;//当前运行到哪个测试用例
	HippoTestCaseBase* m_pCurrentTestCase; // 记录当前执行的测试案例

	//****************
	typedef std::vector<std::pair<std::string,HippoTestCaseBase*> > AllCaseCon;
	typedef AllCaseCon::iterator AllCaseConItr;
	AllCaseCon m_all_testcase;

	void Release();
	RUN_MODE runMode;

	//中间层level更新 需要一个相机----------
	CameraBase* m_pCamera;

};


//宏声明
#define TESTCASE_NAME(testcase_type,testcase_name) \
	testcase_name##_TEST

#define HIPPO_TEST_(testcase_type,testcase_name) \
class TESTCASE_NAME(testcase_type,testcase_name) : public HippoTestCaseBase \
{ \
public: \
	TESTCASE_NAME(testcase_type,testcase_name)(const char* case_name) : HippoTestCaseBase(case_name){}; \
	virtual void Run(); \
private: \
	static HippoTestCaseBase* const testcase_; \
}; \
	\
	HippoTestCaseBase* const TESTCASE_NAME(testcase_type,testcase_name) \
	::testcase_ = HippoTestManager::GetInstance()->RegisterTestCase( \
	new TESTCASE_NAME(testcase_type,testcase_name)(#testcase_name)); \
	void TESTCASE_NAME(testcase_type,testcase_name)::Run()


//写测试用例的时候使用这个宏即可
#define HIPPOTEST(testcase_type,testcase_name) \
	HIPPO_TEST_(testcase_type,testcase_name)


#define ADD_TESTCASE(testcase_type,testcase_name)\
	HippoTestCaseBase* const testcase_name \
	::testcase_ = HippoTestManager::GetInstance()->RegisterTestCase( \
	(testcase_type),new testcase_name(#testcase_name));


//__FILE__,__LINE__,__FUNCTION__
#define HIPPO_EXPECT_EQ(m, n) \
	if (m != n) \
{ \
	HippoTestCaseBase* p=HippoTestManager::GetInstance()->GetCurrentTestCase(); \
	Hippo_WriteConsoleAndLog(CC_RED,"Failed! test=%s,\nfilename=%s,\nline=%d",p->GetTestCaseName(),__FILE__,__LINE__);\
	if(IsDebuggerPresent())\
	{\
		__debugbreak();\
	}\
}\
	else\
{\
	Hippo_WriteConsoleAndLog(CC_GREEN,"测试断言通过");\
}
