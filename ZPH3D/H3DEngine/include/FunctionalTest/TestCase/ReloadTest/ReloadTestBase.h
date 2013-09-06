#include "../../HippoTestManager/HippoTestManager.h"
#include "../CommonCode/HippoUtil/HippoGlobal.h"
#include "../CommonCode/HippoUtil/ActorUtil.h"
#include "../../CommonCode/HippoUtil/HippoScene.h"
#include "../../CommonCode/HippoUtil/HippoStateMachine.h"
#include "../../CommonCode/HippoUtil/RefCountCompare.h"
#include "engine_interface.h"
#include "../idMath/dMathHeader.h"
#include "../../FunctionalTest/functional_test/HippoTestConfig.h"
#include <Windows.h>
#include "EffectCore_dx.h"
using namespace std;

class ReloadTestBase : public HippoTestCase_RefCompare
{
public:
	ReloadTestBase(const char* casename):HippoTestCase_RefCompare(casename){}
	HippoLevelScene* m_pScene;

	Hippo_StateMachine stataMachine;
	bool bAutoEnd;

	HippoResStateSnapShot* s1Tex;
	HippoResStateSnapShot* s1Mat;
	HippoResStateSnapShot* s1Tpl;

	HippoResStateSnapShot* s2Tex;
	HippoResStateSnapShot* s2Mat;
	HippoResStateSnapShot* s2Tpl;

	std::string filePath;
	std::string dirPath;
	int matLod;
public:
	virtual void GetMatLodAndPath(){};
	//virtual int GetLod(){return 0;}

	//传入参数影响了执行的操作
	//true 拷贝测试文件和被修改的文件到执行目录
	//false 删除执行目录中与测试文件相关的文件 并拷贝测试文件到执行目录
	virtual void CopyTestFileOrDel(bool bcopy){};
	virtual void RenameTestFile(){};
	virtual void CreatModel() {};
	virtual void ReloadModel(){}


    void CompareSnapShot();
	void Init();////初始化 lod filepath 和 dirpath三个参数


	SCSTATE CreateModel_0();
	SCSTATE Render5S_1();
	SCSTATE SnapShot1_2();
	SCSTATE ReloadFile_3();
	SCSTATE SnapShot2AndCom_4();



	virtual void InitScene();
	virtual void CleanUpScene();
	virtual bool AutoRunExec();
	virtual bool ManualRunExec();
	virtual int OnKeyDown(unsigned int key);

	

};