#include "../../FunctionalTest/TestCase/ReloadTest/ReloadTestBase.h"

//#define  RELOAD_BPT_TEST_CHR_TWO _TWO_CHR
//bpt测试基类 针对bpt索引的chr
class ReloadBPTTest_Chr :public ReloadTestBase
{
public:
	ReloadBPTTest_Chr(const char* casename):ReloadTestBase(casename){}
	H3DI::IActor* m_pActor;

#ifdef RELOAD_BPT_TEST_CHR_TWO
	H3DI::IActor* m_pActor1;
#endif

public:
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadBPT/116112001/");
		matLod = GetLod();
	}
	virtual int GetLod(){return 0;}
	virtual void CreatModel()
	{
		m_pActor = m_pScene->CreateActor(false,matLod);
		m_pActor->SetBodyPart(string(dirPath+"116112001.bpt").c_str());

#ifdef RELOAD_BPT_TEST_CHR_TWO
		 m_pActor1=m_pScene->CreateActor(false,matLod);
		 m_pActor->SetPosition(H3DVec3(-1.5f,0.f,0.f));
		 m_pActor1->SetBodyPart(string(dirPath+"116112001.bpt").c_str());
		 m_pActor1->SetPosition(H3DVec3(1.5f,0.f,0.f));
#endif
	}
	virtual void ReloadModel()
	{
		m_pActor->ReloadAll();
	}
};


/*备注
断言都没用通过，问题集中在引用计数的比较,主要出错点是 117001001法线，固有色和高光贴图 前后有差异

引用计数统计在 资源目录下 33.txt 和44.txt ，这两个文件分别保存了前后引用计数的统计情况
可能的原因：
*/


/*测试逻辑
测试目的：测试BPT索引的CHR，CHR索引的mat，mat索引的贴图被修改后reload
Lod0加载一个Actor，并穿上一件服装
渲染5S
引用计数快照S1
通过代码修改mat索引的贴图/（2D贴图之间替换）
调用reloadAll
渲染5S，查看变化
Clearmatlib，引用计数快照S2，并比较两次引用计数
*/
class ReloadMatOfChrBPTTest_2DTex1:public ReloadBPTTest_Chr
{
public:
	ReloadMatOfChrBPTTest_2DTex1(const char* casename):ReloadBPTTest_Chr(casename){}
	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(GetDdsPath(true).c_str(),GetDdsPath(false).c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/backTex/changed_d.dds").c_str(),string(dirPath+"Texture/changed_d.dds").c_str(),TRUE);
		}
		else
		{
			DeleteFile(GetDdsPath(false).c_str());
			DeleteFile(string(dirPath+"Texture/back_d.dds").c_str());

			CopyFile(GetDdsPath(true).c_str(),GetDdsPath(false).c_str(),TRUE);
		}
	}

	virtual void RenameTestFile()
	{
		rename(GetDdsPath(false).c_str(),string(dirPath+"Texture/back_d.dds").c_str());
		rename(string(dirPath+"Texture/changed_d.dds").c_str(),GetDdsPath(false).c_str());
	}

	virtual string GetDdsPath(bool bBack)
	{
		if (bBack)
			return string(dirPath+"Texture/backTex/116112001_d.dds");
		else
			return string(dirPath+"Texture/116112001_d.dds");
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadMatOfChrBPTTest_2DTex1);

/*
lod1 加载一个actor 并 穿上一个bpt
*/
class ReloadMatOfChrBPTTest_2DTex2 : public ReloadMatOfChrBPTTest_2DTex1
{
public:
	ReloadMatOfChrBPTTest_2DTex2(const char* casename):ReloadMatOfChrBPTTest_2DTex1(casename){}
	virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadMatOfChrBPTTest_2DTex2);

/*
lod2 加载一个actor 并 穿上一个bpt
*/
class ReloadMatOfChrBPTTest_2DTex3 : public ReloadMatOfChrBPTTest_2DTex1
{
public:
	ReloadMatOfChrBPTTest_2DTex3(const char* casename):ReloadMatOfChrBPTTest_2DTex1(casename){}
	virtual int GetLod(){return 2;}
	virtual string GetDdsPath(bool bBack)
	{
		if (bBack)
			return string(dirPath+"Texture/backTex/116112001_d_LOD2.dds");
		else
			return string(dirPath+"Texture/116112001_d_LOD2.dds"); 
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadMatOfChrBPTTest_2DTex3);


/*测试逻辑
其他逻辑同上，这里使用立方体贴图替换立方体贴图
*/
class ReloadMatOfChrBPTTest_Cube1 : public ReloadBPTTest_Chr
{
public:
	ReloadMatOfChrBPTTest_Cube1(const char* casename):ReloadBPTTest_Chr(casename){}

	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(string(dirPath+"Texture/backTex/hdr.dds").c_str(),string(dirPath+"Texture/hdr.dds").c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/backTex/changed_hdr.dds").c_str(),string(dirPath+"Texture/changed_hdr.dds").c_str(),TRUE);
		}
		else
		{
			DeleteFile(string(dirPath+"Texture/hdr.dds").c_str());
			DeleteFile(string(dirPath+"Texture/back_hdr.dds").c_str());

			CopyFile(string(dirPath+"Texture/backTex/hdr.dds").c_str(),string(dirPath+"Texture/hdr.dds").c_str(),TRUE);
		}
	}

	virtual void RenameTestFile()
	{
		rename(string(dirPath+"Texture/hdr.dds").c_str(),string(dirPath+"Texture/back_hdr.dds").c_str());
		rename(string(dirPath+"Texture/changed_hdr.dds").c_str(),string(dirPath+"Texture/hdr.dds").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadMatOfChrBPTTest_Cube1);


class ReloadMatOfChrBPTTest_Cube2 : public ReloadMatOfChrBPTTest_Cube1
{
public:
	ReloadMatOfChrBPTTest_Cube2(const char* casename):ReloadMatOfChrBPTTest_Cube1(casename){}
	virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadMatOfChrBPTTest_Cube2);

class ReloadMatOfChrBPTTest_Cube3:public ReloadMatOfChrBPTTest_Cube1
{
public:
	ReloadMatOfChrBPTTest_Cube3(const char* casename):ReloadMatOfChrBPTTest_Cube1(casename){}
	virtual int GetLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadMatOfChrBPTTest_Cube3);

/*测试逻辑
其他逻辑同上，这里使用mask贴图 替换mask贴图
*/
class ReloadMatOfChrBPTTest_Mask1:public ReloadBPTTest_Chr
{
public:
	ReloadMatOfChrBPTTest_Mask1(const char* casename):ReloadBPTTest_Chr(casename){}

	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(string(dirPath+"Texture/backTex/116112001_m.dds").c_str(),string(dirPath+"Texture/116112001_m.dds").c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/backTex/changed_m.dds").c_str(),string(dirPath+"Texture/changed_m.dds").c_str(),TRUE);
		}
		else
		{
			DeleteFile(string(dirPath+"Texture/116112001_m.dds").c_str());
			DeleteFile(string(dirPath+"Texture/back_m.dds").c_str());

			CopyFile(string(dirPath+"Texture/backTex/116112001_m.dds").c_str(),string(dirPath+"Texture/116112001_m.dds").c_str(),TRUE);
		}
	}
	virtual void RenameTestFile()
	{
		rename(string(dirPath+"Texture/116112001_m.dds").c_str(),string(dirPath+"Texture/back_m.dds").c_str());
		rename(string(dirPath+"Texture/changed_m.dds").c_str(),string(dirPath+"Texture/116112001_m.dds").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadMatOfChrBPTTest_Mask1);
/*测试逻辑
	lod1创建一个actor ，其他逻辑同上
*/
class ReloadMatOfChrBPTTest_Mask2: public ReloadMatOfChrBPTTest_Mask1
{
public:
	ReloadMatOfChrBPTTest_Mask2(const char* casename):ReloadMatOfChrBPTTest_Mask1(casename){}
	virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadMatOfChrBPTTest_Mask2);
/*测试逻辑
	lod2创建一个actor，其他逻辑同上
*/
class ReloadMatOfChrBPTTest_Mask3:public ReloadMatOfChrBPTTest_Mask1
{
public:
	ReloadMatOfChrBPTTest_Mask3(const char* casename):ReloadMatOfChrBPTTest_Mask1(casename){}
	virtual int GetLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadMatOfChrBPTTest_Mask3);

/*测试逻辑
	测试目的：测试BPT索引的CHR，CHR索引的蒙皮ase，
	lod0 创建一个actor，穿上bpt，比较reload前后引用计数
*/
class ReloadChrBPTTest_skin10:public ReloadBPTTest_Chr
{
public:
	ReloadChrBPTTest_skin10(const char* casename):ReloadBPTTest_Chr(casename){}
	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
		CopyFile(string(dirPath+"backAse/116112_M.ase").c_str(),string(dirPath+"116112_M.ase").c_str(),TRUE);
		CopyFile(string(dirPath+"backAse/changed_M.ase").c_str(),string(dirPath+"changed_M.ase").c_str(),TRUE);
		}
		else
		{
		DeleteFile(string(dirPath+"116112_M.ase").c_str());
		DeleteFile(string(dirPath+"back_M.ase").c_str());
		CopyFile(string(dirPath+"backAse/116112_M.ase").c_str(),string(dirPath+"116112_M.ase").c_str(),TRUE);
		}
	}
	virtual void RenameTestFile()
	{
		rename(string(dirPath+"116112_M.ase").c_str(),string(dirPath+"back_M.ase").c_str());
		rename(string(dirPath+"changed_M.ase").c_str(),string(dirPath+"116112_M.ase").c_str());
	}

private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadChrBPTTest_skin10);


/*测试逻辑
	lod1 创建一个actor，穿上bpt，比较reload前后引用计数
*/
class ReloadChrBPTTest_skin11:ReloadChrBPTTest_skin10
{
public:
	ReloadChrBPTTest_skin11(const char* casename):ReloadChrBPTTest_skin10(casename){}
	virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadChrBPTTest_skin11);
/*测试逻辑
	lod2 创建一个actor，穿上bpt，比较reload前后引用计数
*/
class ReloadChrBPTTest_skin12 :public ReloadChrBPTTest_skin10
{
public:
	ReloadChrBPTTest_skin12(const char* casename):ReloadChrBPTTest_skin10(casename){}
	virtual int GetLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadChrBPTTest_skin12);


/*测试逻辑
测试目的：测试BPT索引的CHR，CHR索引的动作ase，
lod0 创建一个actor，穿上bpt，比较reload前后引用计数
*/
class ReloadChrBPTTest_action13:public ReloadBPTTest_Chr
{
public:
	ReloadChrBPTTest_action13(const char* casename):ReloadBPTTest_Chr(casename){}

	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(string(dirPath+"backAse/116112_MAnm.ase").c_str(),string(dirPath+"116112_MAnm.ase").c_str(),TRUE);
			CopyFile(string(dirPath+"backAse/changed_MAnm.ase").c_str(),string(dirPath+"changed_MAnm.ase").c_str(),TRUE);
		}
		else
		{
			DeleteFile(string(dirPath+"116112_MAnm.ase").c_str());
			DeleteFile(string(dirPath+"back_MAnm.ase").c_str());

			CopyFile(string(dirPath+"backAse/116112_MAnm.ase").c_str(),string(dirPath+"116112_MAnm.ase").c_str(),TRUE);
		}
	}
	virtual void RenameTestFile()
	{
		rename(string(dirPath+"116112_MAnm.ase").c_str(),string(dirPath+"back_MAnm.ase").c_str());
		rename(string(dirPath+"changed_MAnm.ase").c_str(),string(dirPath+"116112_MAnm.ase").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadChrBPTTest_action13);

/* 测试逻辑
lod1 加载一个actor，其他逻辑同上
*/
class ReloadChrBPTTest_action14:public ReloadChrBPTTest_action13
{
public:
	ReloadChrBPTTest_action14(const char* casename):ReloadChrBPTTest_action13(casename){}
	virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST",ReloadChrBPTTest_action14);
/*测试逻辑
lod2加载一个actor，其他逻辑同上
*/
class ReloadChrBPTTest_action15: public ReloadChrBPTTest_action13
{
public:
	ReloadChrBPTTest_action15(const char* casename):ReloadChrBPTTest_action13(casename){}
	virtual int GetLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadChrBPTTest_action15);

/*测试逻辑
测试目的：测试BPT索引的CHR，CHR索引的布料，
lod0 创建一个actor，穿上bpt，比较reload前后引用计数
*/
class ReloadChrBPTTest_cloth16:public ReloadBPTTest_Chr
{
public:
	ReloadChrBPTTest_cloth16(const char* casename):ReloadBPTTest_Chr(casename){}

	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(string(dirPath+"backAse/CM_116112_M.ase").c_str(),string(dirPath+"CM_116112_M.ase").c_str(),TRUE);
			CopyFile(string(dirPath+"backAse/CM_changed_M.ase").c_str(),string(dirPath+"CM_changed_M.ase").c_str(),TRUE);
		}
		else
		{
			DeleteFile(string(dirPath+"CM_116112_M.ase").c_str());
			DeleteFile(string(dirPath+"CM_back_M.ase").c_str());

			CopyFile(string(dirPath+"backAse/CM_116112_M.ase").c_str(),string(dirPath+"CM_116112_M.ase").c_str(),TRUE);
		}
	}
	virtual void RenameTestFile()
	{
		rename(string(dirPath+"CM_116112_M.ase").c_str(),string(dirPath+"CM_back_M.ase").c_str());
		rename(string(dirPath+"CM_changed_M.ase").c_str(),string(dirPath+"CM_116112_M.ase").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadChrBPTTest_cloth16);

/*测试逻辑
lod1 创建一个actor，穿上bpt，比较reload前后引用计数,其他逻辑同上
*/
class ReloadChrBPTTest_cloth17:public ReloadChrBPTTest_cloth16
{
public: 
	ReloadChrBPTTest_cloth17(const char* casename):ReloadChrBPTTest_cloth16(casename){}
	virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadChrBPTTest_cloth17);
/*测试逻辑
lod2 创建一个actor，其他逻辑同上
*/
class ReloadChrBPTTest_cloth18:public ReloadChrBPTTest_cloth16
{
public:
	ReloadChrBPTTest_cloth18(const char* casename):ReloadChrBPTTest_cloth16(casename){}
	virtual int GetLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadChrBPTTest_cloth18);

/*测试逻辑
以下三个测试用例综合了上述几个用例，在这里对贴图，蒙皮，动作，布料都进行了相应替换
测试逻辑和上面的一样
*/
class ReloadBlendBPTTest19:public ReloadBPTTest_Chr
{
public:
	ReloadBlendBPTTest19(const char* casename):ReloadBPTTest_Chr(casename){}

	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			//2D
			CopyFile(GetDdsPath(true).c_str(),GetDdsPath(false).c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/backTex/changed_d.dds").c_str(),string(dirPath+"Texture/changed_d.dds").c_str(),TRUE);
			//cube
			CopyFile(string(dirPath+"Texture/backTex/hdr.dds").c_str(),string(dirPath+"Texture/hdr.dds").c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/backTex/changed_hdr.dds").c_str(),string(dirPath+"Texture/changed_hdr.dds").c_str(),TRUE);
			//mask
			CopyFile(string(dirPath+"Texture/backTex/116112001_m.dds").c_str(),string(dirPath+"Texture/116112001_m.dds").c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/backTex/changed_m.dds").c_str(),string(dirPath+"Texture/changed_m.dds").c_str(),TRUE);
			//skin
			CopyFile(string(dirPath+"backAse/116112_M.ase").c_str(),string(dirPath+"116112_M.ase").c_str(),TRUE);
			CopyFile(string(dirPath+"backAse/changed_M.ase").c_str(),string(dirPath+"changed_M.ase").c_str(),TRUE);
			//action
			CopyFile(string(dirPath+"backAse/116112_MAnm.ase").c_str(),string(dirPath+"116112_MAnm.ase").c_str(),TRUE);
			CopyFile(string(dirPath+"backAse/changed_MAnm.ase").c_str(),string(dirPath+"changed_MAnm.ase").c_str(),TRUE);
			//cloth
			CopyFile(string(dirPath+"backAse/CM_116112_M.ase").c_str(),string(dirPath+"CM_116112_M.ase").c_str(),TRUE);
			CopyFile(string(dirPath+"backAse/CM_changed_M.ase").c_str(),string(dirPath+"CM_changed_M.ase").c_str(),TRUE);
		}
		else
		{
			//2d
			DeleteFile(GetDdsPath(false).c_str());
			DeleteFile(string(dirPath+"Texture/back_d.dds").c_str());
			CopyFile(GetDdsPath(true).c_str(),GetDdsPath(false).c_str(),TRUE);
			//cube
			DeleteFile(string(dirPath+"Texture/hdr.dds").c_str());
			DeleteFile(string(dirPath+"Texture/back_hdr.dds").c_str());
			CopyFile(string(dirPath+"Texture/backTex/hdr.dds").c_str(),string(dirPath+"Texture/hdr.dds").c_str(),TRUE);
			//mask
			DeleteFile(string(dirPath+"Texture/116112001_m.dds").c_str());
			DeleteFile(string(dirPath+"Texture/back_m.dds").c_str());
			CopyFile(string(dirPath+"Texture/backTex/116112001_m.dds").c_str(),string(dirPath+"Texture/116112001_m.dds").c_str(),TRUE);
			//skin
			DeleteFile(string(dirPath+"116112_M.ase").c_str());
			DeleteFile(string(dirPath+"back_M.ase").c_str());
			CopyFile(string(dirPath+"backAse/116112_M.ase").c_str(),string(dirPath+"116112_M.ase").c_str(),TRUE);
			//action
			DeleteFile(string(dirPath+"116112_MAnm.ase").c_str());
			DeleteFile(string(dirPath+"back_MAnm.ase").c_str());
			CopyFile(string(dirPath+"backAse/116112_MAnm.ase").c_str(),string(dirPath+"116112_MAnm.ase").c_str(),TRUE);
			//cloth
			DeleteFile(string(dirPath+"CM_116112_M.ase").c_str());
			DeleteFile(string(dirPath+"CM_back_M.ase").c_str());
			CopyFile(string(dirPath+"backAse/CM_116112_M.ase").c_str(),string(dirPath+"CM_116112_M.ase").c_str(),TRUE);
		}
	}

	virtual void RenameTestFile()
	{
		//
		rename(GetDdsPath(false).c_str(),string(dirPath+"Texture/back_d.dds").c_str());
		rename(string(dirPath+"Texture/changed_d.dds").c_str(),GetDdsPath(false).c_str());
		//
		rename(string(dirPath+"Texture/hdr.dds").c_str(),string(dirPath+"Texture/back_hdr.dds").c_str());
		rename(string(dirPath+"Texture/changed_hdr.dds").c_str(),string(dirPath+"Texture/hdr.dds").c_str());
		//
		rename(string(dirPath+"Texture/116112001_m.dds").c_str(),string(dirPath+"Texture/back_m.dds").c_str());
		rename(string(dirPath+"Texture/changed_m.dds").c_str(),string(dirPath+"Texture/116112001_m.dds").c_str());
		//
		rename(string(dirPath+"116112_M.ase").c_str(),string(dirPath+"back_M.ase").c_str());
		rename(string(dirPath+"changed_M.ase").c_str(),string(dirPath+"116112_M.ase").c_str());
		//
		rename(string(dirPath+"116112_MAnm.ase").c_str(),string(dirPath+"back_MAnm.ase").c_str());
		rename(string(dirPath+"changed_MAnm.ase").c_str(),string(dirPath+"116112_MAnm.ase").c_str());
		//
		rename(string(dirPath+"CM_116112_M.ase").c_str(),string(dirPath+"CM_back_M.ase").c_str());
		rename(string(dirPath+"CM_changed_M.ase").c_str(),string(dirPath+"CM_116112_M.ase").c_str());
	}

	virtual string GetDdsPath(bool bBack)
	{
		if (bBack)
			return string(dirPath+"Texture/backTex/116112001_d.dds");
		else
			return string(dirPath+"Texture/116112001_d.dds");
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadBlendBPTTest19);
/*测试逻辑
lod1 加载一个actor ，其他逻辑同上
*/
class ReloadBlendBPTTest20: public ReloadBlendBPTTest19
{
public:
	ReloadBlendBPTTest20(const char* casename):ReloadBlendBPTTest19(casename){}
	virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELAOD_BPT_TEST",ReloadBlendBPTTest20);
/*测试逻辑
lod2 加载一个actor ，其他逻辑同上
*/
class ReloadBlendBPTTest21: ReloadBlendBPTTest19
{
public:
	ReloadBlendBPTTest21(const char* casename):ReloadBlendBPTTest19(casename){}
	virtual int GetLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_CHR",ReloadBlendBPTTest21);




