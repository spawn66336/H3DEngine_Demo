#include "../../FunctionalTest/TestCase/ReloadTest/ReloadTestBase.h"
#define RELOAD_SPE_TEST_
class ReloadSPeTest :public ReloadTestBase
{
public:
	ReloadSPeTest(const char* casename):ReloadTestBase(casename){}
	ISpecialEffect* m_Spe;
	//H3DI::ISkeletonModel* mpa;
	//H3DI::IModel* m_pd;

#ifdef RELOAD_SPE_TEST_
	ISpecialEffect* m_Spe1;
#endif

public:
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadSpe/Spe/4115008001/4115008.spe");
		matLod = GetLod();
	}
	virtual int GetLod(){return 0;}
	virtual void CreatModel()
	{
		m_Spe = m_pScene->CreateSpe(filePath.c_str(),matLod);
#ifdef RELOAD_SPE_TEST_
	m_Spe1=m_pScene->CreateSpe(filePath.c_str(),matLod);
	m_Spe1->SetPosition(H3DVec3(-2.5f,0.f,0.f));
	m_Spe->SetPosition(H3DVec3(2.5f,0.f,0.f));
#endif
	}
	virtual void ReloadModel()
	{
		m_Spe->ReloadAll();
	}
};

/*备注
spe的reload不会导致重新加载修改过的资源，如，使用修改后的蒙皮文件替换，spe的reload后蒙皮并未被改变
当chr 的reload的时候，可以看到蒙皮被改变了
eff 索引的mat的测试2和3没有通过断言，可能原因：使用的模板metal.tpl中使用了反射贴图
测试5和6，,8，9，没有通过断言，且在reload之后，dml的颜色变得和chr一样
当CreateSpe（）传入的spe文件不存在时，程序能正常运行，没有给出相应的错误提示
*/



/*测试逻辑
测试目的：测试spe索引的eff，eff索引的mat ，mat索引的贴图 被修改后的reload
Lod0加载一个spe
渲染5S
引用计数快照S1
通过代码修改贴图
调用reloadAll
渲染5S，查看变化
Clearmatlib，引用计数快照S2，并比较两次引用计数
*/

class ReloadMatOfEffSpeTest_Tex1:public ReloadSPeTest
{
public:
	ReloadMatOfEffSpeTest_Tex1(const char* casename):ReloadSPeTest(casename){}
	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(string(dirPath+"Texture/back_Tex/bubble.tga").c_str(),string(dirPath+"Texture/bubble.tga").c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/back_Tex/changed.tga").c_str(),string(dirPath+"Texture/changed.tga").c_str(),TRUE);
		}
		else
		{
			DeleteFile(string(dirPath+"Texture/bubble.tga").c_str());
			DeleteFile(string(dirPath+"Texture/back.tga").c_str());
			CopyFile(string(dirPath+"Texture/back_Tex/bubble.tga").c_str(),string(dirPath+"Texture/bubble.tga").c_str(),TRUE);
		}
	}
	virtual void RenameTestFile()
	{
		rename(string(dirPath+"Texture/bubble.tga").c_str(),string(dirPath+"Texture/back.tga").c_str());
		rename(string(dirPath+"Texture/changed.tga").c_str(),string(dirPath+"Texture/bubble.tga").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_SPE_TEST_EFF",ReloadMatOfEffSpeTest_Tex1);
/*测试逻辑
lod1 加载一个spe，其他逻辑同上
*/
class ReloadMatOfEffSpeTest_Tex2:public ReloadMatOfEffSpeTest_Tex1
{
public:
	ReloadMatOfEffSpeTest_Tex2(const char* casename):ReloadMatOfEffSpeTest_Tex1(casename){}
	virtual int GetLod(){return 1;}
private: 
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_SPE_TEST_EFF",ReloadMatOfEffSpeTest_Tex2);

/*测试逻辑
lod2加载一个spe，其他逻辑同上
*/
class ReloadMatOfEffSpeTest_Tex3:public ReloadMatOfEffSpeTest_Tex1
{
public:
	ReloadMatOfEffSpeTest_Tex3(const char* casename):ReloadMatOfEffSpeTest_Tex1(casename){}
	virtual int GetLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_SPE_TEST_EFF",ReloadMatOfEffSpeTest_Tex3);
/*测试逻辑
测试目的：测试spe 索引的eff，eff索引的dml,dml索引的ase reload前后引用计数
lod0 加载一个spe，其他逻辑同测试1
*/
class ReloadDmlSpeTest_Mesh4:public ReloadSPeTest
{
public:
	ReloadDmlSpeTest_Mesh4(const char* casename):ReloadSPeTest(casename){}
	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(string(dirPath+"backAse/4560076.ase").c_str(),string(dirPath+"4560076.ase").c_str(),TRUE);
			CopyFile(string(dirPath+"backAse/changed_4560076.ase").c_str(),string(dirPath+"changed_4560076.ase").c_str(),TRUE);
		}
		else
		{
			DeleteFile(string(dirPath+"4560076.ase").c_str());
			DeleteFile(string(dirPath+"back_4560076.ase").c_str());
			CopyFile(string(dirPath+"backAse/4560076.ase").c_str(),string(dirPath+"4560076.ase").c_str(),TRUE);
		}
	}
	virtual void RenameTestFile()
	{
		rename(string(dirPath+"4560076.ase").c_str(),string(dirPath+"back_4560076.ase").c_str());
		rename(string(dirPath+"changed_4560076.ase").c_str(),string(dirPath+"4560076.ase").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_SPE_TEST_DML",ReloadDmlSpeTest_Mesh4);
/*测试逻辑
lod1 加载一个spe，其他逻辑同上
*/
class ReloadDmlSpeTest_Mesh5:public ReloadDmlSpeTest_Mesh4
{
public:
	ReloadDmlSpeTest_Mesh5(const char* casename):ReloadDmlSpeTest_Mesh4(casename){}
	virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_SPE_TEST_DML",ReloadDmlSpeTest_Mesh5);
/*测试逻辑
lod2 加载一个spe，其他逻辑同上
*/
class ReloadDmlSpeTest_Mesh6:public ReloadDmlSpeTest_Mesh4
{
public:
	ReloadDmlSpeTest_Mesh6(const char* casename):ReloadDmlSpeTest_Mesh4(casename){}
	virtual int GetLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPY_TEST_DML",ReloadDmlSpeTest_Mesh6);

/*测试逻辑
测试目的：测试spe索引的dml，dml索引的mat，mat索引的贴图 reload前后引用计数
lod0 加载一个spe，其他逻辑同测试1
*/
class ReloadDmlSpeTest_2DTex7:public ReloadSPeTest
{
public:
	ReloadDmlSpeTest_2DTex7(const char* casename):ReloadSPeTest(casename){}
	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(GetDdsPath(true).c_str(),GetDdsPath(false).c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/back_Tex/changed_d.dds").c_str(),string(dirPath+"Texture/changed_d.dds").c_str(),TRUE);
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
			return string(dirPath+"Texture/back_Tex/4560076001_d.dds");
		else
			return string(dirPath+"Texture/4560076001_d.dds");
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_DML",ReloadDmlSpeTest_2DTex7);


/*测试逻辑
lod1加载一个spe，其他逻辑同上
*/
class ReloadDmlSpeTest_2DTex8:public ReloadDmlSpeTest_2DTex7
{
public:
	ReloadDmlSpeTest_2DTex8(const char* casename):ReloadDmlSpeTest_2DTex7(casename){}
	virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_SPE_TEST_DML",ReloadDmlSpeTest_2DTex8);
/*测试逻辑
lod2 加载一个spe，其他逻辑同上
*/
class ReloadDmlSpeTest_2DTex9:public ReloadDmlSpeTest_2DTex7
{
public: 
	ReloadDmlSpeTest_2DTex9(const char* casename):ReloadDmlSpeTest_2DTex7(casename){}
	virtual int GetLod(){return 2;}
	virtual string GetDdsPath(bool bBack)
	{
		if (bBack)
			return string(dirPath+"Texture/back_Tex/4560076001_d_LOD2.dds");
		else
			return string(dirPath+"Texture/4560076001_d_LOD2.dds");
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_SPE_TEST_DML",ReloadDmlSpeTest_2DTex9);

/*测试逻辑
测试目的：测试spe索引的dml，dml索引的mat，mat索引的cube贴图 reload前后引用计数
lod0 加载一个spe，其他逻辑同测试1
*/
class ReloadDmlSpeTest_Cube10:public ReloadSPeTest
{
public: 
	ReloadDmlSpeTest_Cube10(const char* casename):ReloadSPeTest(casename){}

	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(string(dirPath+"Texture/back_Tex/hdr.dds").c_str(),string(dirPath+"Texture/hdr.dds").c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/back_Tex/changed_hdr.dds").c_str(),string(dirPath+"Texture/changed_hdr.dds").c_str(),TRUE);
		}
		else
		{
			DeleteFile(string(dirPath+"Texture/hdr.dds").c_str());
			DeleteFile(string(dirPath+"Texture/back_hdr.dds").c_str());
			CopyFile(string(dirPath+"Texture/back_Tex/hdr.dds").c_str(),string(dirPath+"Texture/hdr.dds").c_str(),TRUE);
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
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadDmlSpeTest_Cube10);
/*测试逻辑
lod1 加载一个actor，其他逻辑同上
*/
class ReloadDmlSpeTest_Cube11:public ReloadDmlSpeTest_Cube10
{
public:
	ReloadDmlSpeTest_Cube11(const char* casename):ReloadDmlSpeTest_Cube10(casename){}
	virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadDmlSpeTest_Cube11);
/*测试逻辑
lod2加载一个actor，其他逻辑同上
*/
class ReloadDmlSpeTest_Cube12:public ReloadDmlSpeTest_Cube10
{
public:
	ReloadDmlSpeTest_Cube12(const char* casename):ReloadDmlSpeTest_Cube10(casename){}
	virtual int GetLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadDmlSpeTest_Cube12);

/*测试逻辑
测试目的：测试spe索引的chr ，chr索引的蒙皮 reload前后引用计数情况
lod0 创建一个spe，其他逻辑同测试1
*/
class ReloadChrSPeTest_Skin13:public ReloadSPeTest
{
public:
	ReloadChrSPeTest_Skin13(const char* casename):ReloadSPeTest(casename){}
	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(string(dirPath+"backAse/4115008.ase").c_str(),string(dirPath+"4115008.ase").c_str(),TRUE);
			CopyFile(string(dirPath+"backAse/changed_skin.ase").c_str(),string(dirPath+"changed_skin.ase").c_str(),TRUE);
		}
		else
		{
			DeleteFile(string(dirPath+"4115008.ase").c_str());
			DeleteFile(string(dirPath+"back_skin.ase").c_str());
			CopyFile(string(dirPath+"backAse/4115008.ase").c_str(),string(dirPath+"4115008.ase").c_str(),TRUE);
		}
	}
	virtual void RenameTestFile()
	{
		rename(string(dirPath+"4115008.ase").c_str(),string(dirPath+"back_skin.ase").c_str());
		rename(string(dirPath+"changed_skin.ase").c_str(),string(dirPath+"4115008.ase").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_SPE_TEST_CHR",ReloadChrSPeTest_Skin13);
/*测试逻辑
lod1 创建一个spe，其他测试逻辑同上
*/
class ReloadChrSPeTest_Skin14:ReloadChrSPeTest_Skin13
{
public:
	ReloadChrSPeTest_Skin14(const char* casename):ReloadChrSPeTest_Skin13(casename){}
	virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_SPE_TEST_CHR",ReloadChrSPeTest_Skin14);
/*测试逻辑
lod2创建一个spe。其他逻辑同上
*/
class ReloadChrSPeTest_Skin15:ReloadChrSPeTest_Skin13
{
public:
	ReloadChrSPeTest_Skin15(const char* casename):ReloadChrSPeTest_Skin13(casename){}
	virtual int GetLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_SPE_TEST_CHR",ReloadChrSPeTest_Skin15);

/*测试逻辑
测试目的：测试spe索引的chr ，chr索引的action reload前后引用计数情况
lod0 创建一个spe，其他逻辑同测试1
*/
class ReloadChrSPeTest_Animation16:public ReloadSPeTest
{
public:
	ReloadChrSPeTest_Animation16(const char* casename):ReloadSPeTest(casename){}
	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(string(dirPath+"backAse/4115008_Anm.ase").c_str(),string(dirPath+"4115008_Anm.ase").c_str(),TRUE);
			CopyFile(string(dirPath+"backAse/changed_anm.ASE").c_str(),string(dirPath+"changed_anm.ase").c_str(),TRUE);
		}
		else
		{
			DeleteFile(string(dirPath+"4115008_Anm.ase").c_str());
			DeleteFile(string(dirPath+"back_Anm.ase").c_str());
			CopyFile(string(dirPath+"backAse/4115008_Anm.ase").c_str(),string(dirPath+"4115008_Anm.ase").c_str(),TRUE);
		}
	}
	virtual void RenameTestFile()
	{
		rename(string(dirPath+"4115008_Anm.ase").c_str(),string(dirPath+"back_Anm.ase").c_str());
		rename(string(dirPath+"changed_anm.ase").c_str(),string(dirPath+"4115008_Anm.ase").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_SPE_TEST_CHR",ReloadChrSPeTest_Animation16);
/*测试逻辑
lod1 创建一个spe，其他测试逻辑同上
*/
class ReloadChrSPeTest_Animation17:ReloadChrSPeTest_Animation16
{
public:
	ReloadChrSPeTest_Animation17(const char* casename):ReloadChrSPeTest_Animation16(casename){}
	virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_SPE_TEST_CHR",ReloadChrSPeTest_Animation17);
/*测试逻辑
lod2创建一个spe。其他逻辑同上
*/
class ReloadChrSPeTest_Animation18:ReloadChrSPeTest_Animation16
{
public:
	ReloadChrSPeTest_Animation18(const char* casename):ReloadChrSPeTest_Animation16(casename){}
	virtual int GetLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_SPE_TEST_CHR",ReloadChrSPeTest_Animation18);



/*测试逻辑
测试目的：测试spe索引的dml，dml索引的mat，mat索引的贴图 reload前后引用计数
lod0 加载一个spe，其他逻辑同测试1
*/
class ReloadMatOfDmlSpeTest_2DTex19:public ReloadSPeTest
{
public:
	ReloadMatOfDmlSpeTest_2DTex19(const char* casename):ReloadSPeTest(casename){}
	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(GetDdsPath(true).c_str(),GetDdsPath(false).c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/back_Tex/changed_d.dds").c_str(),string(dirPath+"Texture/changed_d.dds").c_str(),TRUE);
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
			return string(dirPath+"Texture/back_Tex/4115008001_d.dds");
		else
			return string(dirPath+"Texture/4115008001_d.dds");
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_DML",ReloadMatOfDmlSpeTest_2DTex19);


/*测试逻辑
lod1加载一个spe，其他逻辑同上
*/
class ReloadMatOfDmlSpeTest_2DTex20:public ReloadMatOfDmlSpeTest_2DTex19
{
public:
	ReloadMatOfDmlSpeTest_2DTex20(const char* casename):ReloadMatOfDmlSpeTest_2DTex19(casename){}
	virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_SPE_TEST_DML",ReloadMatOfDmlSpeTest_2DTex20);
/*测试逻辑
lod2 加载一个spe，其他逻辑同上
*/
class ReloadMatOfDmlSpeTest_2DTex21:public ReloadMatOfDmlSpeTest_2DTex19
{
public: 
	ReloadMatOfDmlSpeTest_2DTex21(const char* casename):ReloadMatOfDmlSpeTest_2DTex19(casename){}
	virtual int GetLod(){return 2;}
	virtual string GetDdsPath(bool bBack)
	{
		if (bBack)
			return string(dirPath+"Texture/back_Tex/4115008001_d_LOD2.dds");
		else
			return string(dirPath+"Texture/4115008001_d_LOD2.dds");
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_SPE_TEST_DML",ReloadMatOfDmlSpeTest_2DTex21);

/*测试逻辑
测试目的：测试spe索引的dml，dml索引的mat，mat索引的cube贴图 reload前后引用计数
lod0 加载一个spe，其他逻辑同测试1
*/
class ReloadMatOfDmlSpeTest_Cube22:public ReloadSPeTest
{
public: 
	ReloadMatOfDmlSpeTest_Cube22(const char* casename):ReloadSPeTest(casename){}

	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(string(dirPath+"Texture/back_Tex/hdr.dds").c_str(),string(dirPath+"Texture/hdr.dds").c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/back_Tex/changed_hdr.dds").c_str(),string(dirPath+"Texture/changed_hdr.dds").c_str(),TRUE);
		}
		else
		{
			DeleteFile(string(dirPath+"Texture/hdr.dds").c_str());
			DeleteFile(string(dirPath+"Texture/back_hdr.dds").c_str());
			CopyFile(string(dirPath+"Texture/back_Tex/hdr.dds").c_str(),string(dirPath+"Texture/hdr.dds").c_str(),TRUE);
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
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadMatOfDmlSpeTest_Cube22);
/*测试逻辑
lod1 加载一个actor，其他逻辑同上
*/
class ReloadMatOfDmlSpeTest_Cube23:public ReloadMatOfDmlSpeTest_Cube22
{
public:
	ReloadMatOfDmlSpeTest_Cube23(const char* casename):ReloadMatOfDmlSpeTest_Cube22(casename){}
	virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadMatOfDmlSpeTest_Cube23);
/*测试逻辑
lod2加载一个actor，其他逻辑同上
*/
class ReloadMatOfDmlSpeTest_Cube24:public ReloadMatOfDmlSpeTest_Cube22
{
public:
	ReloadMatOfDmlSpeTest_Cube24(const char* casename):ReloadMatOfDmlSpeTest_Cube22(casename){}
	virtual int GetLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadMatOfDmlSpeTest_Cube24);

//-------------------------------------------------------------
/*测试逻辑
测试目的：测试spe索引的mat，mat索引的贴图 reload前后引用计数情况
lod0 加载一个spe，其他逻辑同测试1
*/
class ReloadMatSpeTest25:public ReloadSPeTest
{
public:
	ReloadMatSpeTest25(const char* casename):ReloadSPeTest(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadSpe/Spe_Mat/456123.spe");
		matLod = GetLod();
	}
	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(string(dirPath+"Texture/backTex/GF_514.dds").c_str(),string(dirPath+"Texture/GF_514.dds").c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/backTex/changed.dds").c_str(),string(dirPath+"Texture/changed.dds").c_str(),TRUE);
		}
		else
		{
			DeleteFile(string(dirPath+"Texture/GF_514.dds").c_str());
			DeleteFile(string(dirPath+"Texture/back.dds").c_str());
			CopyFile(string(dirPath+"Texture/backTex/bubble.tga").c_str(),string(dirPath+"Texture/GF_514.dds").c_str(),TRUE);
		}
	}
	virtual void RenameTestFile()
	{
		rename(string(dirPath+"Texture/GF_514.dds").c_str(),string(dirPath+"Texture/back.tga").c_str());
		rename(string(dirPath+"Texture/changed.dds").c_str(),string(dirPath+"Texture/GF_514.dds").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELAOD_SPE_TEST_MAT",ReloadMatSpeTest25);

/*测试逻辑
lod1 加载一个spe，其他逻辑同测试1
*/
class ReloadMatSpeTest26:public ReloadMatSpeTest25
{
public:
	ReloadMatSpeTest26(const char* casename):ReloadMatSpeTest25(casename){}
	virtual int GetLod(){return 1;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_BPT_TEST_MAT",ReloadMatSpeTest26);

/*测试逻辑
lod2 加载一个spe，其他逻辑同测试1
*/
class ReloadMatSpeTest27:public ReloadMatSpeTest25
{
public:
	ReloadMatSpeTest27(const char* casename):ReloadMatSpeTest25(casename){}
	virtual int GetLod(){return 2;}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_SPE_TEST_MAT",ReloadMatSpeTest27);