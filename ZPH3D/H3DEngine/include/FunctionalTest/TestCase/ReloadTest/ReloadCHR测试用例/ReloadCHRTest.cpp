#include "../../FunctionalTest/TestCase/ReloadTest/ReloadTestBase.h"
/*��ע
��Ƥ �Ͷ���ase ��reload֮�� ����������������
���ϲ��� ��lod0�� ����ͨ�����ԣ�����lod1 ��lod2�¿���
mask��ͼ���Բ���ͨ�����ԣ�ԭ����Gͨ����ʹ��metalģ�壬��ģ��������һ��cube��ͼhdr.dds
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


/*�����߼�
����Ŀ�ģ�����chr������ase�ļ����޸ĺ��reload
Lod0����һ��chr
��Ⱦ5S
���ü�������S1
ͨ�������޸�ASE���ݣ�����
����reloadAll
��Ⱦ5S���鿴�仯
Clearmatlib�����ü�������S2�����Ƚ��������ü���
*/

class ReloadSkinCHRTest1 : public ReloadCHRTestBase
{
public:
	ReloadSkinCHRTest1(const char* casename):ReloadCHRTestBase(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/4115008001/4115008.chr");
		matLod = 0;
	}
	//�������Ӱ����ִ�еĲ���
	//true ���������ļ��ͱ��޸ĵ��ļ���ִ��Ŀ¼
	//false ɾ��ִ��Ŀ¼��������ļ���ص��ļ� �����������ļ���ִ��Ŀ¼
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

/*�����߼�
lod1����chr�������߼�ͬ����1
*/
class ReloadSkinCHRTest2 : public ReloadSkinCHRTest1
{
public:
	ReloadSkinCHRTest2(const char* casename):ReloadSkinCHRTest1(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/4115008001/4115008.chr");
		matLod = 1;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadSkinCHRTest2);
/*�����߼�
lod2���ظ�dml�������߼�ͬ����1
*/
class ReloadSkinCHRTest3 :public ReloadSkinCHRTest1
{
	ReloadSkinCHRTest3(const char* casename):ReloadSkinCHRTest1(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/4115008001/4115008.chr");
		matLod = 2;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadSkinCHRTest3);


//****************************************************************
/*�����߼�
����Ŀ�ģ�����CHR�����Ķ����ļ����ı���reload
Lod0����һ��chr
��Ⱦ5S
���ü�������S1
ͨ�������޸Ķ����ļ����ݣ�����
����reloadAll
��Ⱦ5S���鿴�仯
Clearmatlib�����ü�������S2�����Ƚ��������ü���
*/
class ReloadActionCHRTest4 :public ReloadCHRTestBase
{
public:
	ReloadActionCHRTest4(const char* casename):ReloadCHRTestBase(casename){}
	
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/4115008001/4115008.chr");
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

/*�����߼�
lod1���ظã������߼�ͬ����1
*/
class ReloadActionCHRTest5 : public ReloadActionCHRTest4
{
public:
	ReloadActionCHRTest5(const char* casename):ReloadActionCHRTest4(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/4115008001/4115008.chr");
		matLod = 1;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadActionCHRTest5);

/*�����߼�
lod2���أ������߼�ͬ����1
*/
class ReloadActionCHRTest6 : public ReloadActionCHRTest4
{
public:
	ReloadActionCHRTest6(const char* casename):ReloadActionCHRTest4(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/4115008001/4115008.chr");
		matLod = 2;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadActionCHRTest6);

//****************************************************************
/*�����߼�
����Ŀ�ģ�����CHR�����Ĳ��ϱ��ı���reload
Lod0����һ��chr
��Ⱦ5S
���ü�������S1
ͨ�������޸Ĳ����ļ����ݣ�
����reloadAll
��Ⱦ5S���鿴�仯
Clearmatlib�����ü�������S2�����Ƚ��������ü���
*/
class ReloadCltCHRTest7:public ReloadCHRTestBase
{
public:
	ReloadCltCHRTest7(const char* casename):ReloadCHRTestBase(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/116044001/116044_M.chr");
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
/*�����߼�
lod1���ظ�chr�������߼�ͬ����1
*/
class ReloadCltCHRTest8:public ReloadCltCHRTest7
{
public:
	ReloadCltCHRTest8(const char* casename):ReloadCltCHRTest7(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/116044001/116044_M.chr");
		matLod = 1;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadCltCHRTest8);
/*�����߼�
lod2���ظ�chr�������߼�ͬ����1
*/
class ReloadCltCHRTest9:public ReloadCltCHRTest7
{
public:
	ReloadCltCHRTest9(const char* casename):ReloadCltCHRTest7(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/116044001/116044_M.chr");
		matLod = 2;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadCltCHRTest9);



//******************************************************
/*�����߼�
����Ŀ�ģ�����CHR������mat��mat��������ͼ���޸ĺ�reload
Lod0����һ��chr
��Ⱦ5S
���ü�������S1
ͨ�������޸�mat��������ͼ������һ����������ͼ�滻��������ͼ��
����reloadAll
��Ⱦ5S���鿴�仯
Clearmatlib�����ü�������S2�����Ƚ��������ü���
*/
class ReloadMatCHRTest_CubeTex10: public ReloadCHRTestBase
{
public:
	ReloadMatCHRTest_CubeTex10(const char* casename):ReloadCHRTestBase(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/114001001/114001_M.chr");
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
			//��ԭ��ͼ ��������Ҫʹ��
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

/*�����߼�
lod1����chr �������߼�ͬ��
*/
class ReloadMatCHRTest_CubeTex11 : public ReloadMatCHRTest_CubeTex10
{
public:
	ReloadMatCHRTest_CubeTex11(const char* casename):ReloadMatCHRTest_CubeTex10(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/114001001/114001_M.chr");
		matLod = 1;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadMatCHRTest_CubeTex11);
/*�����߼�
lod2 ����chr�������߼�ͬ����1
*/
class ReloadMatCHRTest_CubeTex12 : public ReloadMatCHRTest_CubeTex10
{
public:
	ReloadMatCHRTest_CubeTex12(const char* casename):ReloadMatCHRTest_CubeTex10(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/114001001/114001_M.chr");
		matLod = 2;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadMatCHRTest_CubeTex12);
//------------------------------------------
/*�����߼�
lod0 ����chr����ͨ��ͼ֮���滻
*/
class ReloadMatCHRTest_2DTex13:public ReloadCHRTestBase
{
public:
	ReloadMatCHRTest_2DTex13(const char* casename):ReloadCHRTestBase(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/114001001/114001_M.chr");
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
			//��ͼ ��������Ҫʹ��
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

/*�����߼�
lod1
*/
class ReloadMatCHRTest_2DTex14 : public ReloadMatCHRTest_2DTex13
{
public:
	ReloadMatCHRTest_2DTex14(const char* casename):ReloadMatCHRTest_2DTex13(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/114001001/114001_M.chr");
		matLod = 1;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadMatCHRTest_2DTex14);
/*�����߼�
lod2
*/
class ReloadMatCHRTest_2DTex15 : public ReloadMatCHRTest_2DTex13
{
public:
	ReloadMatCHRTest_2DTex15(const char* casename):ReloadMatCHRTest_2DTex13(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/114001001/114001_M.chr");
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
/*�����߼�
lod0����chr��mask'��ͼ֮����滻
*/
class ReloadMatCHRTest_MaskTex16:public ReloadCHRTestBase
{
public:
	ReloadMatCHRTest_MaskTex16(const char* casename):ReloadCHRTestBase(casename){}
	virtual void  GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/114001001/114001_M.chr");
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
			//��ԭmask'��ͼ ��������Ҫʹ��
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

/*�����߼�
lod1
*/
class ReloadMatCHRTest_MaskTex17 : public ReloadMatCHRTest_MaskTex16
{
public:
	ReloadMatCHRTest_MaskTex17(const char* casename):ReloadMatCHRTest_MaskTex16(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/114001001/114001_M.chr");
		matLod = 1;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadMatCHRTest_MaskTex17);
/*�����߼�
lod2
*/
class ReloadMatCHRTest_MaskTex18 : public ReloadMatCHRTest_MaskTex16
{
public:
	ReloadMatCHRTest_MaskTex18(const char* casename):ReloadMatCHRTest_MaskTex16(casename){}
	virtual void  GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/114001001/114001_M.chr");
		matLod = 2;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadMatCHRTest_MaskTex18);


/*�����߼�
lod0����ģ��chr
����� ��Ƥ ���� ����ͼͬʱ�����޸� �Ƚ�reloadallǰ������ü��� ���鿴��ʾЧ�� 
�����߼�ͬ��
*/
class ReloadBlendCHRTest19:public ReloadCHRTestBase
{
public:
	ReloadBlendCHRTest19(const char* casename):ReloadCHRTestBase(casename){}
	virtual void  GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/4115008001/4115008.chr");
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
/*�����߼�
lod1����ģ��chr
����� ��Ƥ ���� ����ͼͬʱ�����޸� �Ƚ�reloadallǰ������ü��� ���鿴��ʾЧ�� 
�����߼�ͬ��
*/
class ReloadBlendCHRTest20 :public ReloadBlendCHRTest19
{
public:
	ReloadBlendCHRTest20(const char* casename):ReloadBlendCHRTest19(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/4115008001/4115008.chr");
		matLod = 1;
	}
private:
	static HippoTestCaseBase* const testcase_;
};
ADD_TESTCASE("RELOAD_CHR_TEST",ReloadBlendCHRTest20);

/*�����߼�
lod2����ģ��chr
����� ��Ƥ ���� ����ͼͬʱ�����޸� �Ƚ�reloadallǰ������ü��� ���鿴��ʾЧ�� 
�����߼�ͬ��
*/
class ReloadBlendCHRTest21:public ReloadBlendCHRTest19
{
public:
	ReloadBlendCHRTest21(const char* casename):ReloadBlendCHRTest19(casename){}
	virtual void GetMatLodAndPath()
	{
		filePath = Make_HippoTestResource_Path("Reload����������Դ/ReloadCHR/4115008001/4115008.chr");
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