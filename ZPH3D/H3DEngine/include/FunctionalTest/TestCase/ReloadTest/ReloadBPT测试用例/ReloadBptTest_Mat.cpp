#include "../../FunctionalTest/TestCase/ReloadTest/ReloadTestBase.h"

//#define  RELOAD_BPT_TEST_MAT_TWO _TWO_MAT

//bpt测试基类 针对bpt索引的mat
class ReloadBPTTest_Mat :public ReloadTestBase
{
public:
	ReloadBPTTest_Mat(const char* casename):ReloadTestBase(casename){}
	H3DI::IActor* m_pActor;

#ifdef RELOAD_BPT_TEST_MAT_TWO
	H3DI::IActor* m_pActor1;
#endif

public:

	virtual void GetMatLodAndPath()
	{
		filePath=Make_HippoTestResource_Path("Reload测试用例资源/ReloadBPT/117002001/");
		matLod = GetLod();
	}
	virtual int GetLod(){return 0;}
	virtual void CreatModel()
	{
		m_pActor = m_pScene->CreateActor(false,matLod);
		m_pActor->SetBodyPart(string(dirPath+"117002001.bpt").c_str());

#ifdef RELOAD_BPT_TEST_MAT_TWO
		m_pActor1=m_pScene->CreateActor(false,matLod);
		m_pActor->SetPosition(H3DVec3(-1.5f,0.f,0.f));
		m_pActor1->SetBodyPart(string(dirPath+"117002001.bpt").c_str());
		m_pActor1->SetPosition(H3DVec3(1.5f,0.f,0.f));
#endif
	}
	virtual void ReloadModel()
	{
		m_pActor->ReloadAll();
	}
};

/*备注
断言没有通过，
可能原因：
*/

/*测试逻辑
测试目的：测试bpt索引的mat，mat索引的贴图在reload前后引用计数情况，只有袜子有mat
加载一个actor ，lod0
穿上袜子，记录引用计数s1
渲染5S
修改mat索引的2D贴图
reloadall（）
记录引用计数，并比较
*/
class ReloadMatBPTTest_2DTex1:public ReloadBPTTest_Mat
{
public:
	ReloadMatBPTTest_2DTex1(const char* casename):ReloadBPTTest_Mat(casename){}

	virtual string GetDdsPath(bool bBack)
	{
		if (bBack)
			return string(dirPath+"Texture/backTex/117002001_d.dds");
		else
			return string(dirPath+"Texture/117002001_d.dds");
	}
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
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadMatBPTTest_2DTex1);
/*测试逻辑
lod1 创建一个actor，其他逻辑同上
*/
class ReloadMatBPTTest_2DTex2:public ReloadMatBPTTest_2DTex1
{
public:
	ReloadMatBPTTest_2DTex2(const char* casename):ReloadMatBPTTest_2DTex1(casename){}
		virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadMatBPTTest_2DTex2);

/*测试逻辑
lod2 创建一个actor，其他逻辑同上
*/
class ReloadMatBPTTest_2DTex3:public ReloadMatBPTTest_2DTex1
{
public:
	ReloadMatBPTTest_2DTex3(const char* casename):ReloadMatBPTTest_2DTex1(casename){}
	virtual int GetLod(){return 2;}
	virtual string GetDdsPath(bool bBack)
	{
		if (bBack)
			return dirPath+"Texture/backTex/117002001_d_LOD2.dds";
		else
			return dirPath+"Texture/117002001_d_LOD2.dds";
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadMatBPTTest_2DTex3);

/*测试逻辑
lod0 创建一个actor，这里修改mask贴图，其他逻辑同测试1
*/
class ReloadMatBPTTest_Mask4:public ReloadBPTTest_Mat
{
public:
	ReloadMatBPTTest_Mask4(const char* casename):ReloadBPTTest_Mat(casename){}

	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(string(dirPath+"Texture/backTex/117002001_m.dds").c_str(),string(dirPath+"Texture/117002001_m.dds").c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/backTex/changed_m.dds").c_str(),string(dirPath+"Texture/changed_m.dds").c_str(),TRUE);
		}
		else
		{
			DeleteFile(string(dirPath+"Texture/117002001_m.dds").c_str());
			DeleteFile(string(dirPath+"Texture/back_m.dds").c_str());

			CopyFile(string(dirPath+"Texture/backTex/117002001_m.dds").c_str(),string(dirPath+"Texture/117002001_m.dds").c_str(),TRUE);
		}
	}
	virtual void RenameTestFile()
	{
		rename(string(dirPath+"Texture/117002001_m.dds").c_str(),string(dirPath+"Texture/back_m.dds").c_str());
		rename(string(dirPath+"Texture/changed_m.dds").c_str(),string(dirPath+"Texture/117002001_m.dds").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadMatBPTTest_Mask4);
/*测试逻辑
lod1 创建一个actor，其他逻辑同上
*/
class ReloadMatBPTTest_Mask5:public ReloadMatBPTTest_Mask4
{
public:
	ReloadMatBPTTest_Mask5(const char* casename):ReloadMatBPTTest_Mask4(casename){}
	virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadMatBPTTest_Mask5);
/*测试逻辑
lod2创建一个人物，其他逻辑同上
*/
class ReloadMatBPTTest_Mask6:public ReloadMatBPTTest_Mask4
{
public:
	ReloadMatBPTTest_Mask6(const char* casename):ReloadMatBPTTest_Mask4(casename){}
	virtual int GetLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadMatBPTTest_Mask6);

/*测试逻辑
lod0 创建一个actor，这里使用cube贴图替换，其他逻辑同测试1
*/
class ReloadMatBPTTest_Cube7:public ReloadBPTTest_Mat
{
public: 
	ReloadMatBPTTest_Cube7(const char* casename):ReloadBPTTest_Mat(casename){}

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
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadMatBPTTest_Cube7);
/*测试逻辑
lod1 加载一个actor，其他逻辑同上
*/
class ReloadMatBPTTest_Cube8:public ReloadMatBPTTest_Cube7
{
public:
	ReloadMatBPTTest_Cube8(const char* casename):ReloadMatBPTTest_Cube7(casename){}
	virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadMatBPTTest_Cube8);
/*测试逻辑
lod2加载一个actor，其他逻辑同上
*/
class ReloadMatBPTTest_Cube9:public ReloadMatBPTTest_Cube7
{
public:
	ReloadMatBPTTest_Cube9(const char* casename):ReloadMatBPTTest_Cube7(casename){}
	virtual int GetLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadMatBPTTest_Cube9);

/*测试逻辑
lod0 加载一个actor，同时修改三种贴图，比较前后引用计数
其他逻辑同测试1
*/

class ReloadMatBlendBPTTest10:public ReloadBPTTest_Mat
{
public:
	ReloadMatBlendBPTTest10(const char* casename):ReloadBPTTest_Mat(casename){}

	virtual string GetDdsPath(bool bBack)
	{
		if (bBack)
			return string(dirPath+"Texture/backTex/117002001_d.dds");
		else
			return string(dirPath+"Texture/117002001_d.dds");
	}

	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(GetDdsPath(true).c_str(),GetDdsPath(false).c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/backTex/changed_d.dds").c_str(),string(dirPath+"Texture/changed_d.dds").c_str(),TRUE);

			CopyFile(string(dirPath+"Texture/backTex/117002001_m.dds").c_str(),string(dirPath+"Texture/117002001_m.dds").c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/backTex/changed_m.dds").c_str(),string(dirPath+"Texture/changed_m.dds").c_str(),TRUE);

			CopyFile(string(dirPath+"Texture/backTex/hdr.dds").c_str(),string(dirPath+"Texture/hdr.dds").c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/backTex/changed_hdr.dds").c_str(),string(dirPath+"Texture/changed_hdr.dds").c_str(),TRUE);
		}
		else
		{
			DeleteFile(GetDdsPath(false).c_str());
			DeleteFile(string(dirPath+"Texture/back_d.dds").c_str());
			CopyFile(GetDdsPath(true).c_str(),GetDdsPath(false).c_str(),TRUE);

			DeleteFile(string(dirPath+"Texture/117002001_m.dds").c_str());
			DeleteFile(string(dirPath+"Texture/back_m.dds").c_str());
			CopyFile(string(dirPath+"Texture/backTex/117002001_m.dds").c_str(),string(dirPath+"Texture/117002001_m.dds").c_str(),TRUE);

			DeleteFile(string(dirPath+"Texture/hdr.dds").c_str());
			DeleteFile(string(dirPath+"Texture/back_hdr.dds").c_str());
			CopyFile(string(dirPath+"Texture/backTex/hdr.dds").c_str(),string(dirPath+"Texture/hdr.dds").c_str(),TRUE);
		}
	}

	virtual void RenameTestFile()
	{
		rename(GetDdsPath(false).c_str(),string(dirPath+"Texture/back_d.dds").c_str());
		rename(string(dirPath+"Texture/changed_d.dds").c_str(),GetDdsPath(false).c_str());

		rename(string(dirPath+"Texture/117002001_m.dds").c_str(),string(dirPath+"Texture/back_m.dds").c_str());
		rename(string(dirPath+"Texture/changed_m.dds").c_str(),string(dirPath+"Texture/117002001_m.dds").c_str());

		rename(string(dirPath+"Texture/hdr.dds").c_str(),string(dirPath+"Texture/back_hdr.dds").c_str());
		rename(string(dirPath+"Texture/changed_hdr.dds").c_str(),string(dirPath+"Texture/hdr.dds").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadMatBlendBPTTest10);

/*测试逻辑
lod1 加载一个actor，其他逻辑同测试1
*/
class ReloadMatBlendBPTTest11:public ReloadMatBlendBPTTest10
{
public:
	ReloadMatBlendBPTTest11(const char* casename):ReloadMatBlendBPTTest10(casename){}
	virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadMatBlendBPTTest11);
/*测试逻辑
lod2 加载一个actor，其他逻辑同测试1
*/
class ReloadMatBlendBPTTest12:public ReloadMatBlendBPTTest10
{
public:
	ReloadMatBlendBPTTest12(const char* casename):ReloadMatBlendBPTTest10(casename){}
	virtual int GetLod(){return 2;}
	virtual string GetDdsPath(bool bBack)
	{
		if (bBack)
			return dirPath+"Texture/backTex/117002001_d_LOD2.dds";
		else
			return dirPath+"Texture/117002001_d_LOD2.dds";
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadMatBlendBPTTest12);