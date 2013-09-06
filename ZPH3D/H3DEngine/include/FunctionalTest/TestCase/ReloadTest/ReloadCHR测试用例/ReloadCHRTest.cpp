#include "../../FunctionalTest/TestCase/ReloadTest/ReloadTestBase.h"
/*备注
蒙皮 和动作ase 的reload之后 动作不能正常播放
布料测试 在lod0下 不能通过断言，但是lod1 和lod2下可以
mask贴图测试不能通过断言，原因是G通道中使用metal模板，该模板索引了一个cube贴图hdr.dds
*/

//#define  RELOAD_CHR_TEST_TWO _CHR_TWO
class ReloadCHRTestBase:public ReloadTestBase
{
public:
	ReloadCHRTestBase(const char* casename):ReloadTestBase(casename){}
	H3DI::ISkeletonModel*  m_pChr;
#ifdef RELOAD_CHR_TEST_TWO
	H3DI::ISkeletonModel* m_pChr1;
#endif
public:
	virtual void CreatModel()
	{
		m_pChr = m_pScene->CreateChr(filePath.c_str(),matLod);
#ifdef RELOAD_CHR_TEST_TWO
		m_pChr1=m_pScene->CreateChr(filePath.c_str(),matLod);
		m_pChr1->SetPosition(H3DVec3(1.5f,0.f,0.f));
		m_pChr->SetPosition(H3DVec3(-1.5f,0.f,0.f));
#endif
	}
	virtual void ReloadModel()
	{
		m_pChr->ReloadAll();
	}
};


/*测试逻辑
测试目的：测试chr索引的ase文件被修改后的reload
Lod0加载一个chr
渲染5S
引用计数快照S1
通过代码修改ASE内容，（）
调用reloadAll
渲染5S，查看变化
Clearmatlib，引用计数快照S2，并比较两次引用计数
*/

class ReloadSkinCHRTest1 : public ReloadCHRTestBase
{
public:
	ReloadSkinCHRTest1(const char* casename):ReloadCHRTestBase(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/4115008001/4115008.chr");
		matLod = 0;
	}
	//传入参数影响了执行的操作
	//true 拷贝测试文件和被修改的文件到执行目录
	//false 删除执行目录中与测试文件相关的文件 并拷贝测试文件到执行目录
	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{//copy testfile and changedfile 
			CopyFile(string(dirPath+"back/4115008.ase").c_str(),string(dirPath+"4115008.ase").c_str(),TRUE);
			CopyFile(string(dirPath+"back/changed_skin.ase").c_str(),string(dirPath+"changed_skin.ase").c_str(),TRUE);
		}
		else 
		{//del testfile changedfile backfile
			//back the testfile eg: XXX.ase
			DeleteFile(string(dirPath+"4115008.ase").c_str());
			DeleteFile(string(dirPath+"back.ase").c_str());
			CopyFile(string(dirPath+"back/4115008.ase").c_str(),string(dirPath+"4115008.ase").c_str(),TRUE);
		}
	}

	virtual void RenameTestFile()
	{
		rename(string(dirPath+"4115008.ase").c_str(),string(dirPath+"back.ase").c_str());
		rename(string(dirPath+"changed_skin.ase").c_str(),string(dirPath+"4115008.ase").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadSkinCHRTest1);

/*测试逻辑
lod1加载chr，其他逻辑同测试1
*/
class ReloadSkinCHRTest2 : public ReloadSkinCHRTest1
{
public:
	ReloadSkinCHRTest2(const char* casename):ReloadSkinCHRTest1(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/4115008001/4115008.chr");
		matLod = 1;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadSkinCHRTest2);
/*测试逻辑
lod2加载该dml，其他逻辑同测试1
*/
class ReloadSkinCHRTest3 :public ReloadSkinCHRTest1
{
	ReloadSkinCHRTest3(const char* casename):ReloadSkinCHRTest1(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/4115008001/4115008.chr");
		matLod = 2;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadSkinCHRTest3);


//****************************************************************
/*测试逻辑
测试目的：测试CHR索引的动作文件被改变后的reload
Lod0加载一个chr
渲染5S
引用计数快照S1
通过代码修改动作文件内容，（）
调用reloadAll
渲染5S，查看变化
Clearmatlib，引用计数快照S2，并比较两次引用计数
*/
class ReloadActionCHRTest4 :public ReloadCHRTestBase
{
public:
	ReloadActionCHRTest4(const char* casename):ReloadCHRTestBase(casename){}
	
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/4115008001/4115008.chr");
		matLod = 0;
	}
	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(string(dirPath+"back/4115008Anm.ase").c_str(),string(dirPath+"4115008Anm.ase").c_str(),TRUE);
			CopyFile(string(dirPath+"back/changed_Anm.ase").c_str(),string(dirPath+"changed_Anm.ase").c_str(),TRUE);
		}
		else 
		{
			DeleteFile(string(dirPath+"4115008Anm.ase").c_str());
			DeleteFile(string(dirPath+"back_anm.ase").c_str());
			CopyFile(string(dirPath+"back/4115008Anm.ase").c_str(),string(dirPath+"4115008Anm.ase").c_str(),TRUE);
		}
	}

	virtual void RenameTestFile()
	{
		rename(string(dirPath+"4115008Anm.ase").c_str(),string(dirPath+"back_anm.ase").c_str());
		rename(string(dirPath+"changed_Anm.ase").c_str(),string(dirPath+"4115008Anm.ase").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadActionCHRTest4);

/*测试逻辑
lod1加载该，其他逻辑同测试1
*/
class ReloadActionCHRTest5 : public ReloadActionCHRTest4
{
public:
	ReloadActionCHRTest5(const char* casename):ReloadActionCHRTest4(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/4115008001/4115008.chr");
		matLod = 1;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadActionCHRTest5);

/*测试逻辑
lod2加载，其他逻辑同测试1
*/
class ReloadActionCHRTest6 : public ReloadActionCHRTest4
{
public:
	ReloadActionCHRTest6(const char* casename):ReloadActionCHRTest4(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/4115008001/4115008.chr");
		matLod = 2;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadActionCHRTest6);

//****************************************************************
/*测试逻辑
测试目的：测试CHR索引的布料被改变后的reload
Lod0加载一个chr
渲染5S
引用计数快照S1
通过代码修改布料文件内容，
调用reloadAll
渲染5S，查看变化
Clearmatlib，引用计数快照S2，并比较两次引用计数
*/
class ReloadCltCHRTest7:public ReloadCHRTestBase
{
public:
	ReloadCltCHRTest7(const char* casename):ReloadCHRTestBase(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/116044001/116044_M.chr");
		matLod = 0;
	}
	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(string(dirPath+"back/CM_116044_M.ase").c_str(),string(dirPath+"CM_116044_M.ase").c_str(),TRUE);
			CopyFile(string(dirPath+"back/changed_M.ase").c_str(),string(dirPath+"changed_M.ase").c_str(),TRUE);
		}
		else 
		{
			DeleteFile(string(dirPath+"CM_116044_M.ase").c_str());
			DeleteFile(string(dirPath+"back.ase").c_str());

		}
	}

	virtual void RenameTestFile()
	{
		rename(string(dirPath+"CM_116044_M.ase").c_str(),string(dirPath+"back.ase").c_str());
		rename(string(dirPath+"changed_M.ase").c_str(),string(dirPath+"CM_116044_M.ase").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadCltCHRTest7);
/*测试逻辑
lod1加载该chr，其他逻辑同测试1
*/
class ReloadCltCHRTest8:public ReloadCltCHRTest7
{
public:
	ReloadCltCHRTest8(const char* casename):ReloadCltCHRTest7(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/116044001/116044_M.chr");
		matLod = 1;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadCltCHRTest8);
/*测试逻辑
lod2加载该chr，其他逻辑同测试1
*/
class ReloadCltCHRTest9:public ReloadCltCHRTest7
{
public:
	ReloadCltCHRTest9(const char* casename):ReloadCltCHRTest7(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/116044001/116044_M.chr");
		matLod = 2;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadCltCHRTest9);



//******************************************************
/*测试逻辑
测试目的：测试CHR索引的mat，mat索引的贴图被修改后reload
Lod0加载一个chr
渲染5S
引用计数快照S1
通过代码修改mat索引的贴图，（用一个立方体贴图替换立方体贴图）
调用reloadAll
渲染5S，查看变化
Clearmatlib，引用计数快照S2，并比较两次引用计数
*/
class ReloadMatCHRTest_CubeTex10: public ReloadCHRTestBase
{
public:
	ReloadMatCHRTest_CubeTex10(const char* casename):ReloadCHRTestBase(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/114001001/114001_M.chr");
		matLod = 0;
	}
	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(string(dirPath+"Texture/back/hdr.dds").c_str(),string(dirPath+"Texture/hdr.dds").c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/back/changed_hdr.dds").c_str(),string(dirPath+"Texture/changed_hdr.dds").c_str(),TRUE);
		}
		else
		{
			DeleteFile(string(dirPath+"Texture/hdr.dds").c_str());
			DeleteFile(string(dirPath+"Texture/back.dds").c_str());
			//还原贴图 其他用例要使用
			CopyFile(string(dirPath+"Texture/back/hdr.dds").c_str(),string(dirPath+"Texture/hdr.dds").c_str(),TRUE);
		}
	}

	virtual void RenameTestFile()
	{
		rename(string(dirPath+"Texture/hdr.dds").c_str(),string(dirPath+"Texture/back.dds").c_str());
		rename(string(dirPath+"Texture/changed_hdr.dds").c_str(),string(dirPath+"Texture/hdr.dds").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadMatCHRTest_CubeTex10);

/*测试逻辑
lod1加载chr ，其他逻辑同上
*/
class ReloadMatCHRTest_CubeTex11 : public ReloadMatCHRTest_CubeTex10
{
public:
	ReloadMatCHRTest_CubeTex11(const char* casename):ReloadMatCHRTest_CubeTex10(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/114001001/114001_M.chr");
		matLod = 1;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadMatCHRTest_CubeTex11);
/*测试逻辑
lod2 加载chr，其他逻辑同测试1
*/
class ReloadMatCHRTest_CubeTex12 : public ReloadMatCHRTest_CubeTex10
{
public:
	ReloadMatCHRTest_CubeTex12(const char* casename):ReloadMatCHRTest_CubeTex10(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/114001001/114001_M.chr");
		matLod = 2;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadMatCHRTest_CubeTex12);
//------------------------------------------
/*测试逻辑
lod0 记载chr，普通贴图之间替换
*/
class ReloadMatCHRTest_2DTex13:public ReloadCHRTestBase
{
public:
	ReloadMatCHRTest_2DTex13(const char* casename):ReloadCHRTestBase(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/114001001/114001_M.chr");
		matLod = 0;
	}
	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(string(dirPath+"Texture/back/114001001_d.dds").c_str(),string(dirPath+"Texture/114001001_d.dds").c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/back/changed_d.dds").c_str(),string(dirPath+"Texture/changed_d.dds").c_str(),TRUE);
		}
		else
		{
			DeleteFile(string(dirPath+"Texture/114001001_d.dds").c_str());
			DeleteFile(string(dirPath+"Texture/back.dds").c_str());
			//贴图 其他用例要使用
			CopyFile(string(dirPath+"Texture/back/114001001_d.dds").c_str(),string(dirPath+"Texture/114001001_d.dds").c_str(),TRUE);
		}
	}

	virtual void RenameTestFile()
	{
		rename(string(dirPath+"Texture/114001001_d.dds").c_str(),string(dirPath+"Texture/back.dds").c_str());
		rename(string(dirPath+"Texture/changed_d.dds").c_str(),string(dirPath+"Texture/114001001_d.dds").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadMatCHRTest_2DTex13);

/*测试逻辑
lod1
*/
class ReloadMatCHRTest_2DTex14 : public ReloadMatCHRTest_2DTex13
{
public:
	ReloadMatCHRTest_2DTex14(const char* casename):ReloadMatCHRTest_2DTex13(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/114001001/114001_M.chr");
		matLod = 1;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadMatCHRTest_2DTex14);
/*测试逻辑
lod2
*/
class ReloadMatCHRTest_2DTex15 : public ReloadMatCHRTest_2DTex13
{
public:
	ReloadMatCHRTest_2DTex15(const char* casename):ReloadMatCHRTest_2DTex13(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/114001001/114001_M.chr");
		matLod = 2;
	}
	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(string(dirPath+"Texture/back/114001001_d_LOD2.dds").c_str(),string(dirPath+"Texture/114001001_d_LOD2.dds").c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/back/changed_d.dds").c_str(),string(dirPath+"Texture/changed_d.dds").c_str(),TRUE);
		}
		else
		{
			DeleteFile(string(dirPath+"Texture/114001001_d_LOD2.dds").c_str());
			DeleteFile(string(dirPath+"Texture/back.dds").c_str());
	
			CopyFile(string(dirPath+"Texture/back/114001001_d_LOD2.dds").c_str(),string(dirPath+"Texture/114001001_d_LOD2.dds").c_str(),TRUE);
		}
	}

	virtual void RenameTestFile()
	{
		rename(string(dirPath+"Texture/114001001_d_LOD2.dds").c_str(),string(dirPath+"Texture/back.dds").c_str());
		rename(string(dirPath+"Texture/changed_d.dds").c_str(),string(dirPath+"Texture/114001001_d_LOD2.dds").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadMatCHRTest_2DTex15);
//-----------------------------------------
/*测试逻辑
lod0加载chr，mask'贴图之间的替换
*/
class ReloadMatCHRTest_MaskTex16:public ReloadCHRTestBase
{
public:
	ReloadMatCHRTest_MaskTex16(const char* casename):ReloadCHRTestBase(casename){}
	virtual void  GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/114001001/114001_M.chr");
		matLod = 0;
	}
	virtual void CopyTestFileOrDel(bool bcopy)
	{
		if (bcopy)
		{
			CopyFile(string(dirPath+"Texture/back/114001001_m.dds").c_str(),string(dirPath+"Texture/114001001_m.dds").c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/back/changed_m.dds").c_str(),string(dirPath+"Texture/changed_m.dds").c_str(),TRUE);
		}
		else
		{
			DeleteFile(string(dirPath+"Texture/114001001_m.dds").c_str());
			DeleteFile(string(dirPath+"Texture/back.dds").c_str());
			//还原mask'贴图 其他用例要使用
			CopyFile(string(dirPath+"Texture/back/114001001_m.dds").c_str(),string(dirPath+"Texture/114001001_m.dds").c_str(),TRUE);
		}
	}
	virtual void RenameTestFile()
	{
		rename(string(dirPath+"Texture/114001001_m.dds").c_str(),string(dirPath+"Texture/back.dds").c_str());
		rename(string(dirPath+"Texture/changed_m.dds").c_str(),string(dirPath+"Texture/114001001_m.dds").c_str());
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadMatCHRTest_MaskTex16);

/*测试逻辑
lod1
*/
class ReloadMatCHRTest_MaskTex17 : public ReloadMatCHRTest_MaskTex16
{
public:
	ReloadMatCHRTest_MaskTex17(const char* casename):ReloadMatCHRTest_MaskTex16(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/114001001/114001_M.chr");
		matLod = 1;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadMatCHRTest_MaskTex17);
/*测试逻辑
lod2
*/
class ReloadMatCHRTest_MaskTex18 : public ReloadMatCHRTest_MaskTex16
{
public:
	ReloadMatCHRTest_MaskTex18(const char* casename):ReloadMatCHRTest_MaskTex16(casename){}
	virtual void  GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/114001001/114001_M.chr");
		matLod = 2;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadMatCHRTest_MaskTex18);


/*测试逻辑
lod0加载模型chr
这里对 蒙皮 动作 和贴图同时进行修改 比较reloadall前后的引用计数 并查看显示效果 
其他逻辑同上
*/
class ReloadBlendCHRTest19:public ReloadCHRTestBase
{
public:
	ReloadBlendCHRTest19(const char* casename):ReloadCHRTestBase(casename){}
	virtual void  GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/4115008001/4115008.chr");
		matLod = 0;
	}
	virtual void CopyTestFileOrDel(bool bcopy)
	{
		GetDdsPath();
		if (bcopy)
		{
			//skin
			CopyFile(string(dirPath+"back/4115008.ase").c_str(),string(dirPath+"4115008.ase").c_str(),TRUE);
			CopyFile(string(dirPath+"back/changed_skin.ase").c_str(),string(dirPath+"changed_skin.ase").c_str(),TRUE);
			//action
			CopyFile(string(dirPath+"back/4115008Anm.ase").c_str(),string(dirPath+"4115008Anm.ase").c_str(),TRUE);
			CopyFile(string(dirPath+"back/changed_Anm.ase").c_str(),string(dirPath+"changed_Anm.ase").c_str(),TRUE);
			//2D texture
			CopyFile(ddsBackPath.c_str(),ddsPath.c_str(),TRUE);
			CopyFile(string(dirPath+"Texture/back/changed_d.dds").c_str(),string(dirPath+"Texture/changed_d.dds").c_str(),TRUE);
		}
		else
		{
			//skin
			DeleteFile(string(dirPath+"4115008.ase").c_str());
			DeleteFile(string(dirPath+"back_skin.ase").c_str());
			CopyFile(string(dirPath+"back/4115008.ase").c_str(),string(dirPath+"4115008.ase").c_str(),TRUE);
			//action 
			DeleteFile(string(dirPath+"4115008Anm.ase").c_str());
			DeleteFile(string(dirPath+"back_anm.ase").c_str());
			CopyFile(string(dirPath+"back/4115008Anm.ase").c_str(),string(dirPath+"4115008Anm.ase").c_str(),TRUE);
			//2D
			DeleteFile(ddsPath.c_str());
			DeleteFile(string(dirPath+"Texture/back.dds").c_str());
			CopyFile(ddsBackPath.c_str(),ddsPath.c_str(),TRUE);
		}
	}
	virtual void RenameTestFile()
	{
		//skin
		rename(string(dirPath+"4115008.ase").c_str(),string(dirPath+"back_skin.ase").c_str());
		rename(string(dirPath+"changed_skin.ase").c_str(),string(dirPath+"4115008.ase").c_str());
		//action
		rename(string(dirPath+"4115008Anm.ase").c_str(),string(dirPath+"back_anm.ase").c_str());
		rename(string(dirPath+"changed_Anm.ase").c_str(),string(dirPath+"4115008Anm.ase").c_str());
		//2D
		rename(ddsPath.c_str(),string(dirPath+"Texture/back.dds").c_str());
		rename(string(dirPath+"Texture/changed_d.dds").c_str(),ddsPath.c_str());
	}

	virtual void GetDdsPath()
	{
		ddsBackPath=dirPath+"Texture/back/4115008001_D.dds";
		ddsPath=dirPath+"Texture/4115008001_D.dds";
	}
public:
	string ddsBackPath;
	string ddsPath;
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadBlendCHRTest19);
/*测试逻辑
lod1加载模型chr
这里对 蒙皮 动作 和贴图同时进行修改 比较reloadall前后的引用计数 并查看显示效果 
其他逻辑同上
*/
class ReloadBlendCHRTest20 :public ReloadBlendCHRTest19
{
public:
	ReloadBlendCHRTest20(const char* casename):ReloadBlendCHRTest19(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/4115008001/4115008.chr");
		matLod = 1;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadBlendCHRTest20);

/*测试逻辑
lod2加载模型chr
这里对 蒙皮 动作 和贴图同时进行修改 比较reloadall前后的引用计数 并查看显示效果 
其他逻辑同上
*/
class ReloadBlendCHRTest21:public ReloadBlendCHRTest19
{
public:
	ReloadBlendCHRTest21(const char* casename):ReloadBlendCHRTest19(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload测试用例资源/ReloadCHR/4115008001/4115008.chr");
		matLod = 2;
	}
	virtual void GetDdsPath()
	{
		ddsBackPath=dirPath+"Texture/back/4115008001_d_LOD2.dds";
		ddsPath=dirPath+"Texture/4115008001_d_LOD2.dds";
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadBlendCHRTest21);