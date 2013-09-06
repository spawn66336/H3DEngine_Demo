/********************************************************************
	created:	2012/07/22
	created:	22:7:2012   15:13
	filename: 	F:\EngineX52MT\���̲߳�����������\Common\HippoTestManager\HippoTestManager.h
	file path:	F:\EngineX52MT\���̲߳�����������\Common\HippoTestManager
	file base:	HippoTestManager
	file ext:	h
	author:		sssa2000
	
	purpose:	������������������google test�Ĵ���
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
{   //�Զ����� ���� ĳ���� ָ������
	AUTO_RUN_ALL,
	AUTO_RUN_A_TYPE,
	AUTO_RUN_SPECIFIED,
	//�ֶ����� ���� ĳ���� ָ������
	MANUAL_RUN_ALL,
	MANUAL_RUN_A_TYPE,
	MANUAL_RUN_SPECIFIED
};

enum CASE_STATE
{
	CASE_INIT=0,		//��ʼ��״̬����ӦInitScene����
	CASE_BEFORE_EXECING,//׼��ִ�в���(Ŀǰ��ʱû��)
	CASE_EXECING,		//ִ�в����У���ӦAutoRunExec����ManualRunExec����
	CASE_END_EXECING,	//����ִ����ϣ�������ӦCleanUpScene����
};

#define SAFE_RELEASE(x)\
	if(x)\
{\
	x->Release();\
	x = 0;\
	};\


class HippoTestManager;
class HippoTestCase_RefCompare;
//��������
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

	// �Զ�ִ�в��԰����ķ���������true��ʾ�Ѿ����
	virtual bool AutoRunExec()=0;

	// �ֶ�ִ�в��Եķ���������true��ʾ�Ѿ����
	virtual bool ManualRunExec()=0;

	//��ת n ���룬����true��ʾʱ�乻��
	virtual bool Spin_N_Ms(DWORD timeMs);
	virtual bool Spin_N_Frame(DWORD timeMs);

	virtual void AfterRender()	{}

	//!�Զ������Ϣ������������0��ʾ�Ը���Ϣ������Ȥ������1��ʾ�Ѿ���������ҪĬ����Ϣ�������ٴ���
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
	std::string testcase_name; // ���԰�������


private:
	//����2�������������ģ�Ϊ���ܹ���testcase��ʼ��ĩβ��һЩ����
	virtual bool InitTestCase();
	virtual bool CleanUpTestCase();
};


//�������ڲ��������Ļ��Զ��Ƚϲ�����������ǰ�Ͳ�����������ʱ�� ������Դ���ü�������
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

	//����2�������������ģ�Ϊ���ܹ���testcase��ʼ��ĩβ��һЩ����
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

//���Կ��ʵ��
class HippoTestManager
{
public:
	HippoTestManager();
	~HippoTestManager();

	// ����
	static HippoTestManager* GetInstance(); 

	//
	bool GetCurrentRunMode(){return runMode==MANUAL_RUN_SPECIFIED||runMode==MANUAL_RUN_ALL || runMode == MANUAL_RUN_A_TYPE; }

	//��ʼ��
	void InitWithArg(int argc, char* argv[]);

	// ע����԰���
	HippoTestCaseBase* RegisterTestCase(const char* type,HippoTestCaseBase* pCase);

	// �õ���ǰ����ִ�е�test case
	HippoTestCaseBase* GetCurrentTestCase();

	// ִ�е�Ԫ���ԣ���frame begin��end֮�����
	int RunAllTest();

	void BeforeRender();

	void AfterRender();

	MouseKeyCallback GetMouseLeftDownCallback();
	MouseKeyCallback GetMouseMoveCallback();
	MouseKeyCallback GetMouseLeftUpCallback();
	MouseWheelCallback GetMouseWheelCallback();
	KeyCallback GetKeyDownCallback();


	//----------�м�����level��
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


	unsigned int m_TestCaseIdx;//��ǰ���е��ĸ���������
	HippoTestCaseBase* m_pCurrentTestCase; // ��¼��ǰִ�еĲ��԰���

	//****************
	typedef std::vector<std::pair<std::string,HippoTestCaseBase*> > AllCaseCon;
	typedef AllCaseCon::iterator AllCaseConItr;
	AllCaseCon m_all_testcase;

	void Release();
	RUN_MODE runMode;

	//�м��level���� ��Ҫһ�����----------
	CameraBase* m_pCamera;

};


//������
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


//д����������ʱ��ʹ������꼴��
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
	Hippo_WriteConsoleAndLog(CC_GREEN,"���Զ���ͨ��");\
}
