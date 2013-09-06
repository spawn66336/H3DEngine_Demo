#include "ReloadTestBase.h"


 void ReloadTestBase::CompareSnapShot()
 {
	
	 //HippoResStateSSDiff* diff=RefCountCompare::GetSnapShotDiff(s2Tex,s1Tex);
	 //HippoResStateSSDiff* difff=RefCountCompare::GetSnapShotDiff(s1Mat,s2Mat);
	 //HippoResStateSSDiff* diffff=RefCountCompare::GetSnapShotDiff(s1Tpl,s2Tpl);
	 

	 //com
	 HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(s1Mat,s2Mat),true);
	 HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(s1Tex,s2Tex),true);
	 HIPPO_EXPECT_EQ(RefCountCompare::SnapShotEqual(s1Tpl,s2Tpl),true);
	 //release
	 s1Mat->Release();
	 s1Tex->Release();
	 s1Tpl->Release();
	 s2Mat->Release();
	 s2Tex->Release();
	 s2Tpl->Release();
 }

 void ReloadTestBase::Init()
 {
	 GetMatLodAndPath();
	 std::string::size_type idx = filePath.find_last_of('/');
	 if (idx==-1)
	 {
		 Hippo_WriteConsole(CC_WHITE,"输入路径格式不正确，请检查");
		 return;
	 }
	 dirPath = filePath.substr(0,idx+1);
 }

 SCSTATE ReloadTestBase::CreateModel_0()
 {
	 Hippo_WriteConsole(CC_WHITE,"创建模型");
	 CopyTestFileOrDel(true);
	 CreatModel();
	 return STATE_CHANGE;
 }


 SCSTATE ReloadTestBase::Render5S_1()
 {
	 if (Spin_N_Ms(1500))
	 {
		 Hippo_WriteConsole(CC_WHITE,"渲染5S完成");
		 return STATE_CHANGE;
	 }
	 return STATE_NOT_CHANGE;
 }

 SCSTATE ReloadTestBase::SnapShot1_2()
 {
	 Hippo_WriteConsole(CC_WHITE,"引用计数快照S1");
	 s1Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
	 s1Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
	 s1Tpl = RefCountCompare::GetResState(HIPPO_TPL_RES);
	 return STATE_CHANGE;
 }
 SCSTATE ReloadTestBase::ReloadFile_3()
 {
	 RenameTestFile();//reaname test file
	 ReloadModel();
	  Hippo_WriteConsole(CC_WHITE,"ReloadAll()完成");
	 CopyTestFileOrDel(false);//保证执行后的测试文件正确性
	 return STATE_CHANGE;
 }

 SCSTATE ReloadTestBase::SnapShot2AndCom_4()
 {
	 Hippo_WriteConsole(CC_WHITE,"记录引用计数快照S2并比较");
	 Hippo_GetIRender()->ClearMaterialLib();
	 //get s2
	 s2Mat = RefCountCompare::GetResState(HIPPO_MAT_RES);
	 s2Tex = RefCountCompare::GetResState(HIPPO_TEX_RES);
	 s2Tpl = RefCountCompare::GetResState(HIPPO_TPL_RES);
	 CompareSnapShot();
	 bAutoEnd = true;
	 return STATE_CHANGE;
 }

 void ReloadTestBase::InitScene()
 {
	 m_pScene = Hippo_GetSceneManager()->CreateEmptyLevel();
	 bAutoEnd = false;
	 stataMachine.AddState(0,std::tr1::bind(&ReloadTestBase::CreateModel_0,this));
	 stataMachine.AddState(1,std::tr1::bind(&ReloadTestBase::Render5S_1,this));
	 stataMachine.AddState(2,std::tr1::bind(&ReloadTestBase::SnapShot1_2,this));
	 stataMachine.AddState(3,std::tr1::bind(&ReloadTestBase::ReloadFile_3,this));
	 stataMachine.AddState(4,std::tr1::bind(&ReloadTestBase::Render5S_1,this));
	 stataMachine.AddState(5,std::tr1::bind(&ReloadTestBase::SnapShot2AndCom_4,this));

	 Init();
 }
void ReloadTestBase::CleanUpScene()
 {
	 m_pScene->CleanScene();
	 Hippo_GetSceneManager()->DelScene(m_pScene);
 }
 bool ReloadTestBase::ManualRunExec()
 {
	 stataMachine.Run();
	 return false;
 }
 bool ReloadTestBase::AutoRunExec()
 {
	 if (bAutoEnd)
	 {
		 return true;
	 }
	 return ManualRunExec();
 }
 int ReloadTestBase::OnKeyDown(unsigned int key)
 {
	 if (HippoTestManager::GetInstance()->GetCurrentRunMode())
	 {
		 switch (key)
		 {
		 case VK_RETURN:
			 m_state = CASE_END_EXECING;
			 return 0;
		 }
	 }
	 return -1;
 }
