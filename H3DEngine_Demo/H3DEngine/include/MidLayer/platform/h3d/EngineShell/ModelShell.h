#ifndef MODELSHELL
#define MODELSHELL
namespace H3DI
{
	class CAnimationChannel : public IAnimationChannel
	{
	public:

		H3DVec3 v;
		CAnimationChannel()
		{
			v.Zero();
		}

		virtual void					Release(){delete this;}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

		/************************************************************************/
		/* 0 : succeed
		1 : action is not in actionlib
		2 : action is in actionlib, but can't load ase file
		3 : Invalid Params
		*/
		/************************************************************************/
		/**
		���õ�һ��̬֡
		*/
		virtual int SetStaticFrame(const char* name) {return 0;}
		/**
		���þ�̬֡
		*/
		virtual int SetStaticFrame(unsigned int id, const char* name, float weight) {return 0;}
		/**
		���þ�̬֡����
		*/
		virtual void SetStaticSeq(const char** names, const float* intervals, unsigned int count, bool loop) {}
		/**
		���ö���֡����
		*/
		virtual void SetActionSeq(const char** names, unsigned int count, bool loop) {}
		/**
		���õ�һ����֡
		*/
		virtual int SetAction(const char* names, bool loop) {return 0;}
		virtual unsigned int SetActionAsyn(const char* names, bool loop){return 0;}

		virtual int SetNextAction(const char* names, bool loop) {return 0;}
		/**
		�õ���ǰ����
		*/
		virtual IAction* GetCurrAction(void) {return 0;}
		/**
		Clear all stuff
		*/
		virtual void Clear() {}
		/**
		�������ͨ����Ȩ��, ���м���ͨ����Ȩ�ػᱻ��һ��
		*/
		virtual void SetWeight(float) {}
		/**
		�õ�Ȩ��
		*/
		virtual float GetWeight(void)const {return 0;.5f;}
		/**
		���ö�������ʱ��
		*/
		virtual void SetTransitionTimeFromCurrent(float) {}

		virtual void SetTransitionTimeToNext(float) {}
		/**
		�õ���������ʱ��
		*/
		virtual float GetTransitionTimeFromCurrent(void)const {return 0.5;}

		virtual float GetTransitionTimeToNext(void)const {return 0.5;}
		/**
		���õ�ǰ����ʱ��,ֻ�ھ�֡����,��һ����֡,����֡����ģʽ��������
		*/
		virtual void SetCurrActionTime(float) {}
		/**
		�õ���ǰ����ʱ��
		*/
		virtual float GetCurrActionTime(void)const {return 0.5;}
		/**
		�õ���ǰ��������
		*/
		virtual float GetCurrActionLength(void)const {return 0.5;}
		/**
		ֻ�����ڹ��ɶ���
		*/
		virtual bool IsTransitingToNext(void)const {return true;}

		virtual bool IsTransitingFromCurrent(void)const {return true;}
		/**
		�Ƿ�ѭ��
		*/
		virtual bool IsLoop(void)const {return false;}
		/**
		�Ƿ��ռ
		*/
		virtual bool IsDominating(void)const {return false;}
		/**
		�õ�channel ID
		*/
		virtual unsigned int GetChannelID(void)const {return 0;}
		/**
		���ö�������ģʽ, Ĭ��ΪTRANSITION_SYNC
		*/
		virtual void SetTransitionMode(int mode) {}
		/**
		�õ���������ģʽ
		*/
		virtual int GetTransitionMode(void)const {return 0;}

		virtual void Draw(void)const {}

		//�õ���ǰ���������֣�����ֵ��0��ʾû�е�ǰ������1����ʾ�ɹ���-1��ʾ�����buffer����̫С
		virtual int GetCurrentActionName(char* buffer,unsigned int bufferlen){strcpy(buffer, "testname"); return 1;}

		//�õ���һ�����������֣�����ֵ��0��ʾû��next������1����ʾ�ɹ���-1��ʾ�����buffer����̫С
		virtual int GetNextActionName(char* buffer,unsigned int bufferlen){strcpy(buffer, "testname");return 1;}

		//�õ���һ�������Ƿ�ѭ��������-1��ʾû����һ������ ����0��ʾ��ѭ�� ����1��ʾѭ��
		virtual int GetIsNextActionLoop(){ return -1;}
		//�ڵ�ǰ������������ѭ������
		virtual	void SetActionLoop(bool) {}
		//�ڵ�ǰ��һ����������ѭ������
		virtual void SetNextActionLoop(bool){}
		//��������ʱ�Ĳ�ֵ��ʽ
		virtual void SetTrainsitionMethod(EActionTransitionMethod) {}

		virtual void SetRootPos(const H3DVec3 &){}
		virtual const H3DVec3& GetRootPos(void) const { return v;}

		virtual ~CAnimationChannel(){}
	};

	class CAnimation : public IAnimation
	{
	public:

		H3DVec3 m_dummy_vec;
		H3DQuat m_dummy_quat;


		virtual void					Release(){delete this;}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

		virtual	void					clear(){}

		virtual const H3DVec3&	get_pos(void)const {return m_dummy_vec;}
		virtual const H3DQuat&	get_rot(void)const {return m_dummy_quat;}

		virtual void					update(float interval){}
		virtual void					set_passtime(float *time_passed){}
		virtual int						getrotnum()const{return 0;}
		virtual int						getposnum()const{return 0;}


		virtual void					getposkey(int i, H3DVec3& pos){}
		virtual void					getrotkey(int i, H3DQuat& rot){}

		virtual float					get_animtime()const{return 0;}
		virtual float					get_curtime()const{return 0;}
		virtual int						get_curposfrm()const{return 0;}
		virtual int						get_currotfrm()const{return 0;}
		virtual void					reset(){}
		virtual void					setloop(int lp){}
	};

	class CAction : public IAction
	{
		CAnimation m_ani;
		CAnimationChannel m_ani_channel;
	public:
		virtual void					Release(){delete this;}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

		virtual		~CAction(){};
		virtual const char*				GetName(){return "";};
		virtual IAnimation*				GetAnimation(unsigned int nBone){return &m_ani;};
		virtual float					GetAnimationTime(){return 10;};
		virtual void					SetAnimationRunningTime(float fAnimTime){};
	};

	class CBone: public IBone
	{
	public:
		virtual void					Release(){delete this;}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

		virtual const char*				GetName(){return "root";}
		virtual	int						GetID()const{return 12;}
		virtual IBone*					GetParent(){return 0;}
		virtual unsigned int			GetNumChildren(){return 0;}
		virtual IBone*					GetChild(unsigned int n){return this;}
	};

	class CSkeleton: public ISkeleton
	{
		CBone m_bone;
	public:
		virtual void					Release(){delete this;}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

		virtual IBone*					GetRootBone(){return &m_bone;}
		virtual unsigned int			GetNumBone(){return 1;}
		virtual IBone*					GetBone(unsigned int n){return &m_bone;}
		virtual IBone*					GetBone(const char* sBoneName){return &m_bone;}
	};

	class CModel: public IModel
	{ 
		int m_flag;
	public:
		CModel():m_flag(0)
		{
		}
		virtual ~CModel(){};
		virtual void Release()
		{
			EnginePool::m_model_pool->releaseObj(this);
		}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

		H3DMat4 m_d_m;
		H3DVec3 m_d_v;

		virtual MODEL_TYPE				Type(){ return MODEL_STATIC;};
		virtual unsigned int			GetID(){return 0;};
		virtual unsigned int			PrepareDraw(){return 0;};
		virtual unsigned int			Draw(){return 0;};
		virtual void*					GetAllInside(int n){return 0;};
		//�˶�����
		virtual IAnimation*				GetAnimTrack(){return 0;};
		virtual void					SetAnimTrack(IAnimation* pAnim){}
		virtual float					GetCurAnimTime(){return 0;}
		virtual void					RunAnimation(bool bRun,float fStartTime){}
		virtual bool					IsAnimationRun(){return true;}

		virtual void					Update(){}
		virtual void					Update(float fIntervalTime){}

		virtual void                    GetAABB(float*){}
		virtual void					SetAABB(const float* boxWorld){}
		virtual float					Picking(const float* Dir,const float* Point,float* vecIntPoint=0){return 0;}

		virtual void                    GetLocalAABB(float*){}
		virtual const H3DMat4& GetLocationMatrix()const{return m_d_m;}
		virtual const H3DMat4& GetOrgMatrix()const{return m_d_m;}
		virtual void					SetLocationMatrix(const H3DMat4& mat){};//��������״̬�����Z�᷽����ת�Ƕ��Զ���0
		virtual void					SetOrgMatrix(const H3DMat4& mat){};//��������ԭʼ״̬�����
		virtual void					SetPosition(const H3DVec3&){};
		virtual void		            GetPosition(H3DVec3&)const{};

		virtual void					SetZRotation(float degree){};
		virtual float					GetZRotation()const {return 0;};
		virtual void					SetScale(const H3DVec3& scale){};
		virtual void	                GetScale(H3DVec3&)const{};
		virtual void					SetDirection(const H3DVec3& dir){};
		virtual void                    GetDirection(H3DVec3&)const{};

		virtual void					SetColor(const float* col, bool bUseColor){};
		float m_c[4];
		virtual const float*			GetColor()const{return m_c;};

		//��ι�ϵ���ƣ�������IMoveObject
		virtual unsigned int			GetNumChild()const{return 0;}
		virtual IMoveObject*			GetChild(unsigned int n){return 0;}
		virtual void					AddChild(IMoveObject* pMoveObj){}
		virtual IMoveObject*			GetParent(){return 0;}
		virtual void					SetParent(IMoveObject* pMoveObj){}

		virtual void					EnableSort(){}
		virtual void					DisableSort(){}
		virtual bool					IsSortEnabled(){return false;}
		virtual void					SetLOD(size_t nLOD){}
		virtual size_t					GetLOD()const{return 0;}
		virtual size_t					GetLODNum(){return 0;}
		virtual float					GetLodDist(size_t nLod)const{return 0;}

		virtual void					EnableChildrenUpdate(bool bEnable){}

		virtual IShader*				GetShader(){return 0;}
		virtual void					SetShader(IShader* pIShader){}
		virtual void					SetEffectShader(IShader* pShader){}
		///������������,����1��ʾ�ɹ���0��ʾʧ�ܡ�
		virtual int						ReloadAll(){return 1;}

		/*������
		0����ͬ��ReloadAll
		1��Reload Geometry
		2��Reload ���ʣ�������ͼ��
		3��Reload ��ͼ

		����ֵ-1Ϊʧ�ܣ�����0������ʾreload�˼�����ͼ
		*/
		virtual int						ReloadFromDisk(int reloadContent){return 0;}
		///��ñ�����ʹ�õ��Դ�
		virtual unsigned int GetVideoMemUse(){return 0;}

		///�����������
		virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;}


		//��AABB�����Ƿ��ĳ�������ཻ��point��dir����x��y��z��������
		//����ֵΪ1��ʾ�ཻ
		virtual int RayHitTestWithAABB(const H3DVec3& point,const H3DVec3& dir){return 0;}

		//������ɫϵ��
		//f����ɫϵ�������飬arraysize����ɫϵ�����鳤��
		//����ֵ=1��ʾ�ɹ�������ֵС��0��ʾ���ɹ������ɹ���ԭ���ǲ�����û�ж�����ɫϵ��
		virtual int SetMaterialColorFactor(float* f,int arraysize){return 0;}

		/// ���ù���ɫ��ɫϵ��,Ҫ��������float[4]��Ӱ�췽���� (Mat_d*C0.rgb)+C1.rgb��fC0��Alpha���ڱ��Ͷȼ��㣬fC1��Alpha�������ȼ���
		/// meshPart��ʾģ�ͽ��ܱ�ɫ�������ID������һ��ģ�Ϳ�����MeshID����
		///     ����ǽ�ɫ����ʾ��װ��BodypartID��Ҽ�ID(BODYPART_NUM+�Ҽ�ID)����ֵΪ-1ʱ������ɫ��ģ�����еĲ��֡�
		/// bUpdateChildrenΪtrueʱ������ɫ��ģ�͵��ӽڵ㣨����Ҽ��ϵ���Ч��
		virtual int SetDiffuseColorFactor(float* fC0,int arraysizeC0, float* fC1, int arraysizeC1, int meshPart=-1, bool bUpdateChildren=true){return 0;}

		//Ϊģ��������Ⱦ������������EEntityFlag�İ�λ����Ľ��

		///���ò���lod,nLod=0��Ϊ���ϸ�ڣ�nLod=2Ϊ���ϸ��
		virtual void SetMaterialLod(int nLod){}
		virtual int GetMaterialLod(){return 0;}


//IMODEL
		virtual unsigned int			GetFlags(){return m_flag;}
		virtual void					SetFlags(unsigned int nFlags){m_flag = nFlags;}

		virtual unsigned int			GetNumSubModel()const{return 0;}
		virtual void					SetTransparence(float trans){}
		virtual float					GetTransparence()const{return 0;}

		//���п���

		virtual void					SetGeometry(IGeometry* pGeom){}
		virtual IGeometry*				GetGeometry(){return 0;}
		virtual void					RunGeomAnimation(bool bRun,float fStartTime){}

		//CLONE
		virtual IModel*					Clone(){ return 0;}

		//LOD
		virtual void					EnableRender(){}
		virtual void					DisableRender(){}
		virtual bool					IsNeedRender(){return true;}
		virtual void					SetLoop(bool bLoop){}
		virtual bool					GetLoop()const{return false;}

		virtual void                    SetFakeShadowSize(float) {}
		virtual float                    GetFakeShadowSize(void)const{return 0;}

		//by sssa2000 090210
		virtual bool					IsCullByOctree(){return false;}
		virtual bool					IsCullByOccluder(){return false;}


		//���ӷ�Χ
		virtual	float GetVisibleDist() {return 100;}
		virtual	void SetVisibleDist(float dist){}

		//���һ��ģ�ͺŵĶ�����������ݣ�
		//pVertBuf��pIndexBuff�������������ָ�룬���ڴ�����ֵ
		//pVertNum,pIndexNum����������ĸ�������ǰpVertBuf��pIndexBuff�������ָ���ǣ����ض���������ĸ�����һ������´��붥��������ĸ�����
		//����ֵ��������Ķ�����������������ڵ���ʵ�ʵ�ģ�����������ʱ������,���򷵻ؼ�
		virtual bool GetVertexAndIndex( float *pVertBuf,unsigned int *pIndexBuff,int *pVertNum,int *pIndexNum ) { return false;}

		///�����������ģ�͵Ĳ���,����1��ʾ�ɹ���0��ʾʧ�ܡ�
		virtual int ReloadMaterialInstance(){return 0;}
		///��������fx
		virtual int ReloadFx(){return 0;}

		///����ģ�͵�ģ�����ӡ�0.0-1.0�ĸ�������Ϊ0ʱ��ʾģ����ȫ������Ϊ1�Ǳ�ʾ��ȫ�����˶�ģ����0-1֮�������ʾ�����˶�ģ���ĳ̶ȡ�
		virtual void SetMotionBlurFactor(float fFactor) {}
		///�����ڵ���ѯ��״̬
		virtual void	SetUseOcclusionQuery(bool bVal){}
		///��ȡ�Ƿ�ʹ���ڵ���ѯ��״̬
		virtual bool	UseOcclusionQuery() const {{return false;}}
		///��ò�ѯ�����0��ʾ����ȫ�ڵ���
		virtual unsigned int GetOcclusionQueryResult() {return 0;}


		//////////////////////////////////////////////////////////////////////////
		//�������

		//����������2����ɫc0,c1�����յ���ɫ��c0*������ɫ+c1
		//����ֵ1��ʾ�ɹ���0��ʾʧ�ܡ�ʧ�ܵ�ԭ��������Ĳ��ʲ�֧�ָò���
		virtual int HightLight(float r0,float b0,float c0,float r1,float b1,float c1){return 0;}

		///�õ��²��ʣ������������id����0��ʼ
		virtual H3DI::INewShader* GetNewShaderByIndex(unsigned int i){return 0;}

		///�õ��²��ʣ����������������
		virtual H3DI::INewShader* GetNewShaderByName(const char* name){return 0;}

		///�����²��� ��Ӱ�쵽����ģ�͵�����mesh
		virtual int SetNewShader(H3DI::INewShader* p){return 0;}

		///ͨ��һ������ʵ�����ļ����Ͳ���ʵ�������� ��һ���������ø�����ģ�ͣ���Ӱ�쵽����ģ�͵�����mesh
		///����ֵ�Ǵ��ļ��еõ���INewShader,0��ʾʧ��
		virtual H3DI::INewShader* SetNewShader(const char* filename,const char* matballName){return 0;}


		///�����²��� ��һ���������ø�ĳ��mesh������0��ʾʧ�ܣ�������û�и�mesh
		virtual int SetMeshNewShaderByName(const char* meshName,H3DI::INewShader* p){ return 0;}

		///ͨ��һ������ʵ�����ļ����Ͳ���ʵ�������� ��һ���������ø�ĳ��mesh
		///����ֵ�Ǵ��ļ��еõ���INewShader,0��ʾʧ��
		virtual H3DI::INewShader* SetMeshNewShaderByName(const char* meshName,const char* filename,const char* matballName){return 0;}

		///�����²��� ��һ���������ø�ĳ��mesh������0��ʾʧ�ܣ�������û�и�mesh
		virtual int SetMeshNewShaderByIndex(int meshIdx,H3DI::INewShader* p){return 0;}

		///ͨ��һ������ʵ�����ļ����Ͳ���ʵ�������� ��һ���������ø�ĳ��mesh
		///����ֵ�Ǵ��ļ��еõ���INewShader,0��ʾʧ��
		virtual H3DI::INewShader* SetMeshNewShaderByIndex(int meshIdx,const char* filename,const char* matballName){ return 0;}

		///���ڴ�����һ������ģ���ģ�ͣ�ģ�͵����������ܵ�Ӱ�졣����1��ʾ�ɹ�
		virtual int SetMaterialTemplateFromMem(void* pXmlDoc){ return 0;}

		///���ڴ�����һ������ģ���ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ��������ֵ�����
		virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){ return 0;}

		///���ڴ�����һ������ģ���ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		virtual int SetMeshMaterialTemplateFromMemByIndex(int meshIdx,void* pXmlDoc){ return 0;}

		///���ڴ�����һ������ʵ����ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		virtual int SetMeshMaterialInstanceFromMemByName(int meshIdx,void* pXmlDoc){ return 0;}

		///���ڴ�����һ������ʵ����ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		virtual int SetMeshMaterialInstanceFromMemByName(const char* meshName,void* pXmlDoc){ return 0;}
		//////////////////////////////////////////////////////////////////////////
		//���½ӿڲ�Ҫʹ��

		///���������Ƿ���з���Ч��
		///����b:�Ƿ�������
		///����plane����һ��4��Ԫ�ص����飬�������ƽ��ķ���
		virtual void SetIsReflectionObj(bool b,float* plane){ }

		///��ȡ�����Ƿ���з���Ч��
		///����:�Ƿ�������
		///����plane����һ��4��Ԫ�ص����飬�������ƽ��ķ���
		virtual bool GetIsReflectionObj(float* plane){ return false;}

		///���������Ƿ���뷴����Ⱦ
		virtual void SetCanBeReflection(bool b){}

		///��ȡ�����Ƿ���뷴����Ⱦ
		virtual bool GetCanBeReflection(){ return false;}

		///���÷���Զƽ��
		///������Զƽ�����
		virtual void SetReflectionFarClipPlane(float dis){}

		///���������Ƿ�����������
		virtual void SetIsRefracionObj(bool b){ }
		virtual bool GetIsRefracionObj(){ return false;}

		///����������
		virtual void SetRefracionIndex(float f){ }
		virtual float GetRefracionIndex(){ return 0;}

		//�༭�����ɹ�âʱ���õĽӿ�
		virtual void  Editor_GenerateMetalStar() { }
		//////////////////////////////////////////////////////////////////////////
		//��ȡ��â�ļ�����
		//	���أ���â����ֵ��
		//	������psize������Ͳ��������ع�â���ݴ�С��
		virtual void * Editor_GetMetalStarData (unsigned int* psize) { return 0;}

		//���ܣ�����һ��������ģ���ϵİ�Χ������ײ����Ϣ
		//����
		//rayOrigin������������������ɵ����飬��ʾһ�����ߵ���ʼ��
		//rayDir������������������ɵ����飬��ʾһ�����ߵķ���
		//ModelBVTestInfo ��һ�������׵�ַ�����ڽ���ķ��ء�������ײ���İ�Χ�а��Ⱥ�˳��ID����ײ�����귵�ظ��ͻ��ˡ�����Ĵ�С����С��һ��ģ�͵İ�Χ�и�������Χ�и������������ṩһ�����еõ�
		//nResultBufferCount�������Ԫ�ظ�������ֹ�����������Խ��
		//����ֵ�����صĽ���ĸ���,���Ϊ��˵��û����ײ
		virtual int  RayTestWithBoundingVolumes(const float* rayOrigin, const float * rayDir, ModelBoundingVolumeTestInfo *pOut, int nResultBufferCount) const { return 0;}
		//��ȡ��Χ��ĸ���
		virtual int	 GetBoundingVolumesCount() { return 0;}

		//���ܣ�������
		//	//����
		//	RayOrg��������������
		//	RayDir���������߷���
		//	fOutDistance�����淵�صĽ���������ֵ����ײ�㵽�������ľ��롣
		//����ֵ��������ģ���Ƿ���ײ
		virtual bool  RayTestWithTriangles(const H3DVec3 & RayOrg, const H3DVec3 & RayDir,float & fOutDistance) {return false;}

		//!������ؽӿ�
		//�������ߵõ��ҽ����ݼ�
		//�ڶ���������ʾ�Ƿ���Ҫת�����߿ռ�
		virtual int	  QueryAttachPoint(IAttachModelInfo& rayResInfo,bool bTransToLocal=true) { return 0;}

		//!�ҽӹ���.
		virtual int	  AttachModel(const IAttachModelInfo& rayInfo) {return 0;}

		//!ȡ���ҽ�
		virtual int	  DetachModel(const IAttachModelInfo& rayInfo) {return 0;}

		//!Ԥ����fx
		virtual int PreGenFx(H3DI::IFxInfo* p){ return 0;}

		///!����LightMap��ͼ�ļ�����Info��,MeshIDΪ��ģ�͵���������
		virtual int SetLightMapInfo(unsigned int MeshID, const ILightMapInfo& info){return 0;}

		/**
		lightmapSizeBuffer: mesh��Ӧ��lightmap size����geometry��mesh������һ��
		uvTexturePathName: ����Ԥ��uv��ֵĽ�����紫��"d:\\lightmapUVTest%d.bmp",��
		�����d:\\lightmapUVTest0.bmp, d:\\lightmapUVTest1.bmp���ļ���
		����ʱ��NULL
		bReadOnlyModified: ���ڷ����Ƿ��޸���lmm�ļ���ֻ�����ԣ�����ΪNULL
		����ֵ���ɹ�����true��ʧ�ܷ���false�����ʧ�ܣ������ø����lightmapsize�������������ֶ�չuv
		*/
		virtual bool GenerateLightmapUV(const int* lightmapSizeBuffer, const char* uvTexturePathName, int* bReadOnlyModified) {return 0;}

		///���ź���
		virtual void HelpFunction(int i,void* param0,void* param1=0,void* param2=0,void* param3=0,void* param4=0){ }


	};

	class CMoveObject : public IMoveObject//IUserObject
	{
	public:
		H3DMat4 m_d_m;
		H3DVec3 m_d_v;


		virtual MODEL_TYPE				Type(){ return MODEL_STATIC;};
		virtual unsigned int			GetID(){return 0;};
		virtual unsigned int			PrepareDraw(){return 0;};
		virtual unsigned int			Draw(){return 0;};
		virtual void*					GetAllInside(int n){return 0;};
		//�˶�����
		virtual IAnimation*				GetAnimTrack(){return 0;};
		virtual void					SetAnimTrack(IAnimation* pAnim){}
		virtual float					GetCurAnimTime(){return 0;}
		virtual void					RunAnimation(bool bRun,float fStartTime){}
		virtual bool					IsAnimationRun(){return true;}

		virtual void					Update(){}
		virtual void					Update(float fIntervalTime){}

		virtual void                    GetAABB(float*){}
		virtual void					SetAABB(const float* boxWorld){}
		virtual float					Picking(const float* Dir,const float* Point,float* vecIntPoint=0){return 0;}

		virtual void                    GetLocalAABB(float*){}
		virtual const H3DMat4& GetLocationMatrix()const{return m_d_m;}
		virtual const H3DMat4& GetOrgMatrix()const{return m_d_m;}
		virtual void					SetLocationMatrix(const H3DMat4& mat){};//��������״̬�����Z�᷽����ת�Ƕ��Զ���0
		virtual void					SetOrgMatrix(const H3DMat4& mat){};//��������ԭʼ״̬�����
		virtual void					SetPosition(const H3DVec3&){};
		virtual void		            GetPosition(H3DVec3&)const{};

		virtual void					SetZRotation(float degree){};
		virtual float					GetZRotation()const {return 0;};
		virtual void					SetScale(const H3DVec3& scale){};
		virtual void	                GetScale(H3DVec3&)const{};
		virtual void					SetDirection(const H3DVec3& dir){};
		virtual void                    GetDirection(H3DVec3&)const{};

		virtual void					SetColor(const float* col, bool bUseColor){};
		float m_c[4];
		virtual const float*			GetColor()const{return m_c;};

		//��ι�ϵ���ƣ�������IMoveObject
		virtual unsigned int			GetNumChild()const{return 0;}
		virtual IMoveObject*			GetChild(unsigned int n){return 0;}
		virtual void					AddChild(IMoveObject* pMoveObj){}
		virtual IMoveObject*			GetParent(){return 0;}
		virtual void					SetParent(IMoveObject* pMoveObj){}

		virtual void					EnableSort(){}
		virtual void					DisableSort(){}
		virtual bool					IsSortEnabled(){return false;}
		virtual void					SetLOD(size_t nLOD){}
		virtual size_t					GetLOD()const{return 0;}
		virtual size_t					GetLODNum(){return 0;}
		virtual float					GetLodDist(size_t nLod)const{return 0;}

		virtual void					EnableChildrenUpdate(bool bEnable){}

		virtual IShader*				GetShader(){return 0;}
		virtual void					SetShader(IShader* pIShader){}
		virtual void					SetEffectShader(IShader* pShader){}
		///������������,����1��ʾ�ɹ���0��ʾʧ�ܡ�
		virtual int						ReloadAll(){return 1;}

		/*������
		0����ͬ��ReloadAll
		1��Reload Geometry
		2��Reload ���ʣ�������ͼ��
		3��Reload ��ͼ

		����ֵ-1Ϊʧ�ܣ�����0������ʾreload�˼�����ͼ
		*/
		virtual int						ReloadFromDisk(int reloadContent){return 0;}
		///��ñ�����ʹ�õ��Դ�
		virtual unsigned int GetVideoMemUse(){return 0;}

		///�����������
		virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;}

		///���ڴ�����һ������ʵ����ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){return 0;}

		//��AABB�����Ƿ��ĳ�������ཻ��point��dir����x��y��z��������
		//����ֵΪ1��ʾ�ཻ
		virtual int RayHitTestWithAABB(const H3DVec3& point,const H3DVec3& dir){return 0;}

		//������ɫϵ��
		//f����ɫϵ�������飬arraysize����ɫϵ�����鳤��
		//����ֵ=1��ʾ�ɹ�������ֵС��0��ʾ���ɹ������ɹ���ԭ���ǲ�����û�ж�����ɫϵ��
		virtual int SetMaterialColorFactor(float* f,int arraysize){return 0;}

		/// ���ù���ɫ��ɫϵ��,Ҫ��������float[4]��Ӱ�췽���� (Mat_d*C0.rgb)+C1.rgb��fC0��Alpha���ڱ��Ͷȼ��㣬fC1��Alpha�������ȼ���
		/// meshPart��ʾģ�ͽ��ܱ�ɫ�������ID������һ��ģ�Ϳ�����MeshID����
		///     ����ǽ�ɫ����ʾ��װ��BodypartID��Ҽ�ID(BODYPART_NUM+�Ҽ�ID)����ֵΪ-1ʱ������ɫ��ģ�����еĲ��֡�
		/// bUpdateChildrenΪtrueʱ������ɫ��ģ�͵��ӽڵ㣨����Ҽ��ϵ���Ч��
		virtual int SetDiffuseColorFactor(float* fC0,int arraysizeC0, float* fC1, int arraysizeC1, int meshPart=-1, bool bUpdateChildren=true){return 0;}

		//Ϊģ��������Ⱦ������������EEntityFlag�İ�λ����Ľ��
		virtual unsigned int			GetFlags(){return 0;}
		virtual void					SetFlags(unsigned int nFlags){;}

		///���ò���lod,nLod=0��Ϊ���ϸ�ڣ�nLod=2Ϊ���ϸ��
		virtual void SetMaterialLod(int nLod){}
		virtual int GetMaterialLod(){return 0;}
	};

	class CSkeletonModel : public ISkeletonModel
	{
	public:

		CSkeletonModel():m_flag(0)
		{
		}

		H3DMat4 m_d_m;
		H3DVec3 m_d_v;

		virtual MODEL_TYPE				Type(){ return MODEL_STATIC;};
		virtual unsigned int			GetID(){return 0;};
		virtual unsigned int			PrepareDraw(){return 0;};
		virtual unsigned int			Draw(){return 0;};
		virtual void*					GetAllInside(int n){return 0;};
		//�˶�����
		virtual IAnimation*				GetAnimTrack(){return 0;};
		virtual void					SetAnimTrack(IAnimation* pAnim){}
		virtual float					GetCurAnimTime(){return 0;}
		virtual void					RunAnimation(bool bRun,float fStartTime){}
		virtual bool					IsAnimationRun(){return true;}

		virtual void					Update(){}
		virtual void					Update(float fIntervalTime){}

		virtual void                    GetAABB(float*){}
		virtual void					SetAABB(const float* boxWorld){}
		virtual float					Picking(const float* Dir,const float* Point,float* vecIntPoint=0){return 0;}

		virtual void                    GetLocalAABB(float*){}
		virtual const H3DMat4& GetLocationMatrix()const{return m_d_m;}
		virtual const H3DMat4& GetOrgMatrix()const{return m_d_m;}
		virtual void					SetLocationMatrix(const H3DMat4& mat){};//��������״̬�����Z�᷽����ת�Ƕ��Զ���0
		virtual void					SetOrgMatrix(const H3DMat4& mat){};//��������ԭʼ״̬�����
		virtual void					SetPosition(const H3DVec3&){};
		virtual void		            GetPosition(H3DVec3&)const{};

		virtual void					SetZRotation(float degree){};
		virtual float					GetZRotation()const {return 0;};
		virtual void					SetScale(const H3DVec3& scale){};
		virtual void	                GetScale(H3DVec3&)const{};
		virtual void					SetDirection(const H3DVec3& dir){};
		virtual void                    GetDirection(H3DVec3&)const{};

		virtual void					SetColor(const float* col, bool bUseColor){};
		float m_c[4];
		virtual const float*			GetColor()const{return m_c;};

		//��ι�ϵ���ƣ�������IMoveObject
		virtual unsigned int			GetNumChild()const{return 0;}
		virtual IMoveObject*			GetChild(unsigned int n){return 0;}
		virtual void					AddChild(IMoveObject* pMoveObj){}
		virtual IMoveObject*			GetParent(){return 0;}
		virtual void					SetParent(IMoveObject* pMoveObj){}

		virtual void					EnableSort(){}
		virtual void					DisableSort(){}
		virtual bool					IsSortEnabled(){return false;}
		virtual void					SetLOD(size_t nLOD){}
		virtual size_t					GetLOD()const{return 0;}
		virtual size_t					GetLODNum(){return 0;}
		virtual float					GetLodDist(size_t nLod)const{return 0;}

		virtual void					EnableChildrenUpdate(bool bEnable){}

		virtual IShader*				GetShader(){return 0;}
		virtual void					SetShader(IShader* pIShader){}
		virtual void					SetEffectShader(IShader* pShader){}
		///������������,����1��ʾ�ɹ���0��ʾʧ�ܡ�
		virtual int						ReloadAll(){return 1;}

		/*������
		0����ͬ��ReloadAll
		1��Reload Geometry
		2��Reload ���ʣ�������ͼ��
		3��Reload ��ͼ

		����ֵ-1Ϊʧ�ܣ�����0������ʾreload�˼�����ͼ
		*/
		virtual int						ReloadFromDisk(int reloadContent){return 0;}
		///��ñ�����ʹ�õ��Դ�
		virtual unsigned int GetVideoMemUse(){return 0;}

		///�����������
		virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;}


		//��AABB�����Ƿ��ĳ�������ཻ��point��dir����x��y��z��������
		//����ֵΪ1��ʾ�ཻ
		virtual int RayHitTestWithAABB(const H3DVec3& point,const H3DVec3& dir){return 0;}

		//������ɫϵ��
		//f����ɫϵ�������飬arraysize����ɫϵ�����鳤��
		//����ֵ=1��ʾ�ɹ�������ֵС��0��ʾ���ɹ������ɹ���ԭ���ǲ�����û�ж�����ɫϵ��
		virtual int SetMaterialColorFactor(float* f,int arraysize){return 0;}

		/// ���ù���ɫ��ɫϵ��,Ҫ��������float[4]��Ӱ�췽���� (Mat_d*C0.rgb)+C1.rgb��fC0��Alpha���ڱ��Ͷȼ��㣬fC1��Alpha�������ȼ���
		/// meshPart��ʾģ�ͽ��ܱ�ɫ�������ID������һ��ģ�Ϳ�����MeshID����
		///     ����ǽ�ɫ����ʾ��װ��BodypartID��Ҽ�ID(BODYPART_NUM+�Ҽ�ID)����ֵΪ-1ʱ������ɫ��ģ�����еĲ��֡�
		/// bUpdateChildrenΪtrueʱ������ɫ��ģ�͵��ӽڵ㣨����Ҽ��ϵ���Ч��
		virtual int SetDiffuseColorFactor(float* fC0,int arraysizeC0, float* fC1, int arraysizeC1, int meshPart=-1, bool bUpdateChildren=true){return 0;}

		//Ϊģ��������Ⱦ������������EEntityFlag�İ�λ����Ľ��

		///���ò���lod,nLod=0��Ϊ���ϸ�ڣ�nLod=2Ϊ���ϸ��
		virtual void SetMaterialLod(int nLod){}
		virtual int GetMaterialLod(){return 0;}


		int m_flag;
		virtual unsigned int			GetFlags(){return m_flag;}
		virtual void					SetFlags(unsigned int nFlags){m_flag = nFlags;}

		virtual unsigned int			GetNumSubModel()const{return 0;}
		virtual void					SetTransparence(float trans){}
		virtual float					GetTransparence()const{return 0;}

		//���п���

		virtual void					SetGeometry(IGeometry* pGeom){}
		virtual IGeometry*				GetGeometry(){return 0;}
		virtual void					RunGeomAnimation(bool bRun,float fStartTime){}

		//CLONE
		virtual IModel*					Clone(){ return 0;}

		//LOD
		virtual void					EnableRender(){}
		virtual void					DisableRender(){}
		virtual bool					IsNeedRender(){return true;}
		virtual void					SetLoop(bool bLoop){}
		virtual bool					GetLoop()const{return false;}

		virtual void                    SetFakeShadowSize(float) {}
		virtual float                    GetFakeShadowSize(void)const{return 0;}

		//by sssa2000 090210
		virtual bool					IsCullByOctree(){return false;}
		virtual bool					IsCullByOccluder(){return false;}


		//���ӷ�Χ
		virtual	float GetVisibleDist() {return 100;}
		virtual	void SetVisibleDist(float dist){}

		//���һ��ģ�ͺŵĶ�����������ݣ�
		//pVertBuf��pIndexBuff�������������ָ�룬���ڴ�����ֵ
		//pVertNum,pIndexNum����������ĸ�������ǰpVertBuf��pIndexBuff�������ָ���ǣ����ض���������ĸ�����һ������´��붥��������ĸ�����
		//����ֵ��������Ķ�����������������ڵ���ʵ�ʵ�ģ�����������ʱ������,���򷵻ؼ�
		virtual bool GetVertexAndIndex( float *pVertBuf,unsigned int *pIndexBuff,int *pVertNum,int *pIndexNum ) { return false;}

		///�����������ģ�͵Ĳ���,����1��ʾ�ɹ���0��ʾʧ�ܡ�
		virtual int ReloadMaterialInstance(){return 0;}
		///��������fx
		virtual int ReloadFx(){return 0;}

		///����ģ�͵�ģ�����ӡ�0.0-1.0�ĸ�������Ϊ0ʱ��ʾģ����ȫ������Ϊ1�Ǳ�ʾ��ȫ�����˶�ģ����0-1֮�������ʾ�����˶�ģ���ĳ̶ȡ�
		virtual void SetMotionBlurFactor(float fFactor) {}
		///�����ڵ���ѯ��״̬
		virtual void	SetUseOcclusionQuery(bool bVal){}
		///��ȡ�Ƿ�ʹ���ڵ���ѯ��״̬
		virtual bool	UseOcclusionQuery() const {{return false;}}
		///��ò�ѯ�����0��ʾ����ȫ�ڵ���
		virtual unsigned int GetOcclusionQueryResult() {return 0;}


		//////////////////////////////////////////////////////////////////////////
		//�������

		//����������2����ɫc0,c1�����յ���ɫ��c0*������ɫ+c1
		//����ֵ1��ʾ�ɹ���0��ʾʧ�ܡ�ʧ�ܵ�ԭ��������Ĳ��ʲ�֧�ָò���
		virtual int HightLight(float r0,float b0,float c0,float r1,float b1,float c1){return 0;}

		///�õ��²��ʣ������������id����0��ʼ
		virtual H3DI::INewShader* GetNewShaderByIndex(unsigned int i){return 0;}

		///�õ��²��ʣ����������������
		virtual H3DI::INewShader* GetNewShaderByName(const char* name){return 0;}

		///�����²��� ��Ӱ�쵽����ģ�͵�����mesh
		virtual int SetNewShader(H3DI::INewShader* p){return 0;}

		///ͨ��һ������ʵ�����ļ����Ͳ���ʵ�������� ��һ���������ø�����ģ�ͣ���Ӱ�쵽����ģ�͵�����mesh
		///����ֵ�Ǵ��ļ��еõ���INewShader,0��ʾʧ��
		virtual H3DI::INewShader* SetNewShader(const char* filename,const char* matballName){return 0;}


		///�����²��� ��һ���������ø�ĳ��mesh������0��ʾʧ�ܣ�������û�и�mesh
		virtual int SetMeshNewShaderByName(const char* meshName,H3DI::INewShader* p){ return 0;}

		///ͨ��һ������ʵ�����ļ����Ͳ���ʵ�������� ��һ���������ø�ĳ��mesh
		///����ֵ�Ǵ��ļ��еõ���INewShader,0��ʾʧ��
		virtual H3DI::INewShader* SetMeshNewShaderByName(const char* meshName,const char* filename,const char* matballName){return 0;}

		///�����²��� ��һ���������ø�ĳ��mesh������0��ʾʧ�ܣ�������û�и�mesh
		virtual int SetMeshNewShaderByIndex(int meshIdx,H3DI::INewShader* p){return 0;}

		///ͨ��һ������ʵ�����ļ����Ͳ���ʵ�������� ��һ���������ø�ĳ��mesh
		///����ֵ�Ǵ��ļ��еõ���INewShader,0��ʾʧ��
		virtual H3DI::INewShader* SetMeshNewShaderByIndex(int meshIdx,const char* filename,const char* matballName){ return 0;}

		///���ڴ�����һ������ģ���ģ�ͣ�ģ�͵����������ܵ�Ӱ�졣����1��ʾ�ɹ�
		virtual int SetMaterialTemplateFromMem(void* pXmlDoc){ return 0;}

		///���ڴ�����һ������ģ���ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ��������ֵ�����
		virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){ return 0;}

		///���ڴ�����һ������ģ���ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		virtual int SetMeshMaterialTemplateFromMemByIndex(int meshIdx,void* pXmlDoc){ return 0;}

		///���ڴ�����һ������ʵ����ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		virtual int SetMeshMaterialInstanceFromMemByName(int meshIdx,void* pXmlDoc){ return 0;}

		///���ڴ�����һ������ʵ����ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		virtual int SetMeshMaterialInstanceFromMemByName(const char* meshName,void* pXmlDoc){ return 0;}
		//////////////////////////////////////////////////////////////////////////
		//���½ӿڲ�Ҫʹ��

		///���������Ƿ���з���Ч��
		///����b:�Ƿ�������
		///����plane����һ��4��Ԫ�ص����飬�������ƽ��ķ���
		virtual void SetIsReflectionObj(bool b,float* plane){ }

		///��ȡ�����Ƿ���з���Ч��
		///����:�Ƿ�������
		///����plane����һ��4��Ԫ�ص����飬�������ƽ��ķ���
		virtual bool GetIsReflectionObj(float* plane){ return false;}

		///���������Ƿ���뷴����Ⱦ
		virtual void SetCanBeReflection(bool b){}

		///��ȡ�����Ƿ���뷴����Ⱦ
		virtual bool GetCanBeReflection(){ return false;}

		///���÷���Զƽ��
		///������Զƽ�����
		virtual void SetReflectionFarClipPlane(float dis){}

		///���������Ƿ�����������
		virtual void SetIsRefracionObj(bool b){ }
		virtual bool GetIsRefracionObj(){ return false;}

		///����������
		virtual void SetRefracionIndex(float f){ }
		virtual float GetRefracionIndex(){ return 0;}

		//�༭�����ɹ�âʱ���õĽӿ�
		virtual void  Editor_GenerateMetalStar() { }
		//////////////////////////////////////////////////////////////////////////
		//��ȡ��â�ļ�����
		//	���أ���â����ֵ��
		//	������psize������Ͳ��������ع�â���ݴ�С��
		virtual void * Editor_GetMetalStarData (unsigned int* psize) { return 0;}

		//���ܣ�����һ��������ģ���ϵİ�Χ������ײ����Ϣ
		//����
		//rayOrigin������������������ɵ����飬��ʾһ�����ߵ���ʼ��
		//rayDir������������������ɵ����飬��ʾһ�����ߵķ���
		//ModelBVTestInfo ��һ�������׵�ַ�����ڽ���ķ��ء�������ײ���İ�Χ�а��Ⱥ�˳��ID����ײ�����귵�ظ��ͻ��ˡ�����Ĵ�С����С��һ��ģ�͵İ�Χ�и�������Χ�и������������ṩһ�����еõ�
		//nResultBufferCount�������Ԫ�ظ�������ֹ�����������Խ��
		//����ֵ�����صĽ���ĸ���,���Ϊ��˵��û����ײ
		virtual int  RayTestWithBoundingVolumes(const float* rayOrigin, const float * rayDir, ModelBoundingVolumeTestInfo *pOut, int nResultBufferCount) const { return 0;}
		//��ȡ��Χ��ĸ���
		virtual int	 GetBoundingVolumesCount() { return 0;}

		//���ܣ�������
		//	//����
		//	RayOrg��������������
		//	RayDir���������߷���
		//	fOutDistance�����淵�صĽ���������ֵ����ײ�㵽�������ľ��롣
		//����ֵ��������ģ���Ƿ���ײ
		virtual bool  RayTestWithTriangles(const H3DVec3 & RayOrg, const H3DVec3 & RayDir,float & fOutDistance) {return false;}

		//!������ؽӿ�
		//�������ߵõ��ҽ����ݼ�
		//�ڶ���������ʾ�Ƿ���Ҫת�����߿ռ�
		virtual int	  QueryAttachPoint(IAttachModelInfo& rayResInfo,bool bTransToLocal=true) { return 0;}

		//!�ҽӹ���.
		virtual int	  AttachModel(const IAttachModelInfo& rayInfo) {return 0;}

		//!ȡ���ҽ�
		virtual int	  DetachModel(const IAttachModelInfo& rayInfo) {return 0;}

		//!Ԥ����fx
		virtual int PreGenFx(H3DI::IFxInfo* p){ return 0;}

		///!����LightMap��ͼ�ļ�����Info��,MeshIDΪ��ģ�͵���������
		virtual int SetLightMapInfo(unsigned int MeshID, const ILightMapInfo& info){return 0;}

		/**
		lightmapSizeBuffer: mesh��Ӧ��lightmap size����geometry��mesh������һ��
		uvTexturePathName: ����Ԥ��uv��ֵĽ�����紫��"d:\\lightmapUVTest%d.bmp",��
		�����d:\\lightmapUVTest0.bmp, d:\\lightmapUVTest1.bmp���ļ���
		����ʱ��NULL
		bReadOnlyModified: ���ڷ����Ƿ��޸���lmm�ļ���ֻ�����ԣ�����ΪNULL
		����ֵ���ɹ�����true��ʧ�ܷ���false�����ʧ�ܣ������ø����lightmapsize�������������ֶ�չuv
		*/
		virtual bool GenerateLightmapUV(const int* lightmapSizeBuffer, const char* uvTexturePathName, int* bReadOnlyModified) {return 0;}

		///���ź���
		virtual void HelpFunction(int i,void* param0,void* param1=0,void* param2=0,void* param3=0,void* param4=0){ }

		H3DVec3 m_dummy_vec3;
		H3DMat4 m_dummy_mat4;
		CAction m_action;
		CBone m_bone;
		H3DQuat m_dummy_quat;


		virtual void					Release()
		{
			EnginePool::m_skeleton_pool->releaseObj(this);
		}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

		virtual const H3DMat4&	 GetCurMatrix(unsigned int nBone)const{ return m_dummy_mat4;}
		//������Ⱦ���󡣺��߼��������������ͬ
		virtual const H3DMat4&	 GetRendingMatrix(unsigned int nBone)const{ return m_dummy_mat4;}
		virtual const H3DMat4&	 GetCurRalativeMatrix(unsigned int nBone)const{ return m_dummy_mat4;}
		virtual const H3DVec3&  GetCurPos(unsigned int nBone)const{ return m_dummy_vec3;}
		virtual const H3DQuat&	 GetCurRot(unsigned int nBone)const{ return m_dummy_quat;}

		virtual int					SetAction(const char* sName){return 0;}
		virtual int					SetMixAction(const char* sName, float fWeight, int nIdx, int nParam){return 0;}
		virtual void					GetMixAction(int nIdx, float& fWeight, int& nActionIdx){}
		virtual IAction*				GetCurAction(){ return 0;}
		virtual IAnimation*			GetCurBoneAnimation(unsigned int nBone){ return 0;}
		virtual float					GetCurActionTime(){return 0;}
		virtual void					SetCurActionTime(float fActionTime){}
		virtual void					SetActionTransitionTime(float fTime){}
		virtual float					GetActionTransitionTime()const{return 0;}

		virtual void					SetHeadDir(const H3DVec3& dir){}
		virtual void					SetWaistDir(const H3DVec3& dir){}

		virtual const H3DVec3& GetHeadDir()const{ return m_dummy_vec3;}
		virtual const H3DVec3& GetWaistDir()const{ return m_dummy_vec3;}

		CSkeleton m_ske;


		//================Used in action lib model====================
		virtual ISkeleton*					GetSkeleton(){return &m_ske;} // ���û��д��
		virtual const char*					GetActionName(unsigned int n){return "testname";} // ���û��д��
		virtual IAction*						GetAction(unsigned int n){return &m_action;}
		virtual IAction*						GetAction(const char* sActionName){return &m_action;}
		virtual unsigned int					GetNumAction()const{return 0;}
		virtual int								SetStaticMixAction(unsigned int id/*0, 1*/, const char* actName) {return 0;}
		virtual void							SetStaticActionMixWeight(float weight /*0-1*/) {}
		/*
		actNames = const char*[count]; interval = float[count-1]
		*/
		virtual void SetStaticActionSequence(const char** actNames, float* intervals, unsigned int count, bool loop) {}
		/*
		�õ�ĳһ��ͨ���Ķ���, id�Ϸ�ֵΪ[0,3]
		*/
		CAnimationChannel m_ac;
		virtual IAnimationChannel* GetAnmChannel(unsigned int id) {return &m_ac;} // ���û��д�꣬��Ҫ������д
		/*
		�������ͨ��������
		*/
		virtual void          ClearAllAnmChannels(void) {}
		//by zhengchengjun 2011-11-25
		/*
		������Щ����ͨ��Ϊ��ռͨ��, buff=NULL�������ͨ����Ϊ�Ƕ�ռͨ��
		*/
		virtual void   SetDominatingChannel(int* buff, unsigned int buffLen) {}

		virtual unsigned int   GetDominatingChannel(int* buff, unsigned int buffLen)const {return 0;}

		virtual void  SetChannelModifier(IAnimationChannelModifier* p) {}

		virtual IAnimationChannelModifier*  GetChannelModifier(void)const { return 0;}

		virtual void ClearChannelModifier(void) { }

		virtual void LoadBodyBounds(const char* fileName) { }


		virtual unsigned int GetActionNames(const char** buff, unsigned int buffLen) { return 0;}
		virtual unsigned int GetActionNamesCopy(char* buff, unsigned int buffLen, unsigned int nameLen) { return 0;}

		//by sssa2000 090203 Ϊ�༭����ӽӿ�
		//���ƹ���
		virtual void DrawSkeleton(){}
		virtual void CopyTo(ISkeletonModel *pModel)const {}
		//������������Ķ�����ʹ��ǰ����Ķ��������������������ĳ������,iBoneId�ǰ󶨵Ĺ���ID
		virtual bool	RelationAnimation(ISkeletonModel *pHorseModel,const char *animNameSelf,const char *animHorse,int iBoneId = 0) {return false;}

		//�ڸ��¹���ģ�͵�ʱ��,�Ƿ�Ը�����ͷ���м���, Ĭ��ֵΪtrue
		//�����ѡ��Ϊfalse, ��ISkeletonModel��copy������Ҳ����Ҫ������ͷ����.
		virtual void	SetUpdateBones(bool bUpdateBones) {}

		//�����������,̫������,����������Բ��ϸ��µ�Ӱ��
		virtual void	SetMountMatrix(const H3DMat4& mountMatrix){}

		//������������Ա������,����ָ��������ʵ������
		virtual int		GetPhxEntityNames(const char** pNames,int num,int phxType){return 0;}
		//��������ʵ�������,���ǲ��ϲ���ʱ,������ǲ���������
		virtual void   SetPhxEntityPara(const char* pName,void* phxPara,int phxType) {}
		//��ȡ����ʵ�������,���ǲ��ϲ���ʱ,������ǲ���������
		virtual void	GetPhxEntityPara(const char* pName,void* phxPara,int phxType) {}
		//��ȡ�������,�������������,Ŀǰֻ֧�ֲ���
		virtual void   GetPhxParaByName(const char* paraName,void* phxPara,int phxType) {}
		//!�Ӳ�������ɾ����һ������,Ŀǰֻ֧�ֲ���
		virtual void   DelPhxParaByName(const char* paraName,int phxType) {}
		//�½�һ������ʵ��
		virtual void   CreatePhxEntity(const char* pName,void* phxPara,int phxType) {}
		//ɾ��һ������ʵ��
		virtual void   DelPhxEntity(const char* pName) {}
		//������������ģ��,������EPhxDataTypeָ��
		virtual void   SavePhxDataTemplate(const char* pName,int phxDataType){}
		//�����������ݵ�bp3��,��ǰ����
		virtual void   SavePhxDataToBp3(const char* pFileName) {}
		//����.����clearCur�����Ƿ���Ҫ�������ǰ�Ѿ���ȡ�Ĳ���
		virtual void   LoadPhxDataTemplate(const char* pName,int phxDataType,bool clearCur) {}
		//��ȡ��ǰ���в��ϲ�����
		virtual int   GetAllPhxDataParas(const char** pNames,int bufferLen,int phxDataType) {return 0;}
		//�ں�������
		virtual void   BlendPhxResult() {}
		//��������
		virtual void   RenderPhxInfo() {}

		//���һ�������ڹ����ϵ���Ԫ�����ڶ�������֮�󣬽���ת���ӵ�������
		virtual void	AddQuatToBone(int index,const H3DQuat& quat) {}
		//������е���Ԫ��
		virtual void	ClearAdditionalQuat() {}
		//��ȡ���Ϲ�â�ļ�����
		//���أ���â����ֵ��
		//������psize������Ͳ��������ع�â���ݴ�С��
		virtual void *  Editor_GetClothMetalStarData (unsigned int* psize) {return 0;}
		//���Ź���,by lzp 2011-4-14
		virtual void    SetScaleInfo(const ScaleSkeletonInfo& sInfo){}
		virtual void	 ClearScaleInfo() {}

		//sssa2000 08.03.24:
		//��̬���ø����������
		virtual void                SetThisActorPhy(bool b){}
		//��ø����������״̬��ע�⣬������һ���� ����Ҫ��ʾ������һ��SetThisActorPhy������ÿ�ε�GetThisActorPhy������Ч
		virtual bool				GetThisActorPhy(){return false;}

		// ��ĳ��mesh�������򲹳�ֵ
		virtual void SetSortingOffset(const char* meshName, float offset) {}

		// ʹĳ��mesh������ʾ
		// bHightLight true : ���ø߹�Ч����false : �ָ�
		virtual void HightLightSubEntity(const char* subEntityName, bool bHightLight) {}

		//
		virtual void	UpdateCpuSkin(float t) {}

		//������Ƥ��Ԥ������̬
		virtual void  UpdateSkinPose(int bpID,bool bUpdate) {}
	};

	class CActor : public IActor
	{
	public:

		H3DMat4 m_d_m;
		H3DVec3 m_d_v;

		void EnableHeelOffset(bool) {}

		virtual unsigned int			GetNumSubModel()const{return 0;}
		virtual void					SetTransparence(float trans){}
		virtual float					GetTransparence()const{return 0;}

		//���п���

		virtual void					SetGeometry(IGeometry* pGeom){}
		virtual IGeometry*				GetGeometry(){return 0;}
		virtual void					RunGeomAnimation(bool bRun,float fStartTime){}

		//CLONE
		virtual IModel*					Clone(){ return 0;}

		//LOD
		virtual void					EnableRender(){}
		virtual void					DisableRender(){}
		virtual bool					IsNeedRender(){return true;}
		virtual void					SetLoop(bool bLoop){}
		virtual bool					GetLoop()const{return false;}

		virtual void                    SetFakeShadowSize(float) {}
		virtual float                    GetFakeShadowSize(void)const{return 0;}

		//by sssa2000 090210
		virtual bool					IsCullByOctree(){return false;}
		virtual bool					IsCullByOccluder(){return false;}


		//���ӷ�Χ
		virtual	float GetVisibleDist() {return 100;}
		virtual	void SetVisibleDist(float dist){}

		//���һ��ģ�ͺŵĶ�����������ݣ�
		//pVertBuf��pIndexBuff�������������ָ�룬���ڴ�����ֵ
		//pVertNum,pIndexNum����������ĸ�������ǰpVertBuf��pIndexBuff�������ָ���ǣ����ض���������ĸ�����һ������´��붥��������ĸ�����
		//����ֵ��������Ķ�����������������ڵ���ʵ�ʵ�ģ�����������ʱ������,���򷵻ؼ�
		virtual bool GetVertexAndIndex( float *pVertBuf,unsigned int *pIndexBuff,int *pVertNum,int *pIndexNum ) { return false;}

		///�����������ģ�͵Ĳ���,����1��ʾ�ɹ���0��ʾʧ�ܡ�
		virtual int ReloadMaterialInstance(){return 0;}
		///��������fx
		virtual int ReloadFx(){return 0;}

		///����ģ�͵�ģ�����ӡ�0.0-1.0�ĸ�������Ϊ0ʱ��ʾģ����ȫ������Ϊ1�Ǳ�ʾ��ȫ�����˶�ģ����0-1֮�������ʾ�����˶�ģ���ĳ̶ȡ�
		virtual void SetMotionBlurFactor(float fFactor) {}
		///�����ڵ���ѯ��״̬
		virtual void	SetUseOcclusionQuery(bool bVal){}
		///��ȡ�Ƿ�ʹ���ڵ���ѯ��״̬
		virtual bool	UseOcclusionQuery() const {{return false;}}
		///��ò�ѯ�����0��ʾ����ȫ�ڵ���
		virtual unsigned int GetOcclusionQueryResult() {return 0;}


		//////////////////////////////////////////////////////////////////////////
		//�������

		//����������2����ɫc0,c1�����յ���ɫ��c0*������ɫ+c1
		//����ֵ1��ʾ�ɹ���0��ʾʧ�ܡ�ʧ�ܵ�ԭ��������Ĳ��ʲ�֧�ָò���
		virtual int HightLight(float r0,float b0,float c0,float r1,float b1,float c1){return 0;}

		///�õ��²��ʣ������������id����0��ʼ
		virtual H3DI::INewShader* GetNewShaderByIndex(unsigned int i){return 0;}

		///�õ��²��ʣ����������������
		virtual H3DI::INewShader* GetNewShaderByName(const char* name){return 0;}

		///�����²��� ��Ӱ�쵽����ģ�͵�����mesh
		virtual int SetNewShader(H3DI::INewShader* p){return 0;}

		///ͨ��һ������ʵ�����ļ����Ͳ���ʵ�������� ��һ���������ø�����ģ�ͣ���Ӱ�쵽����ģ�͵�����mesh
		///����ֵ�Ǵ��ļ��еõ���INewShader,0��ʾʧ��
		virtual H3DI::INewShader* SetNewShader(const char* filename,const char* matballName){return 0;}


		///�����²��� ��һ���������ø�ĳ��mesh������0��ʾʧ�ܣ�������û�и�mesh
		virtual int SetMeshNewShaderByName(const char* meshName,H3DI::INewShader* p){ return 0;}

		///ͨ��һ������ʵ�����ļ����Ͳ���ʵ�������� ��һ���������ø�ĳ��mesh
		///����ֵ�Ǵ��ļ��еõ���INewShader,0��ʾʧ��
		virtual H3DI::INewShader* SetMeshNewShaderByName(const char* meshName,const char* filename,const char* matballName){return 0;}

		///�����²��� ��һ���������ø�ĳ��mesh������0��ʾʧ�ܣ�������û�и�mesh
		virtual int SetMeshNewShaderByIndex(int meshIdx,H3DI::INewShader* p){return 0;}

		///ͨ��һ������ʵ�����ļ����Ͳ���ʵ�������� ��һ���������ø�ĳ��mesh
		///����ֵ�Ǵ��ļ��еõ���INewShader,0��ʾʧ��
		virtual H3DI::INewShader* SetMeshNewShaderByIndex(int meshIdx,const char* filename,const char* matballName){ return 0;}

		///���ڴ�����һ������ģ���ģ�ͣ�ģ�͵����������ܵ�Ӱ�졣����1��ʾ�ɹ�
		virtual int SetMaterialTemplateFromMem(void* pXmlDoc){ return 0;}


		///���ڴ�����һ������ģ���ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		virtual int SetMeshMaterialTemplateFromMemByIndex(int meshIdx,void* pXmlDoc){ return 0;}

		///���ڴ�����һ������ʵ����ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		virtual int SetMeshMaterialInstanceFromMemByName(int meshIdx,void* pXmlDoc){ return 0;}

		///���ڴ�����һ������ʵ����ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		virtual int SetMeshMaterialInstanceFromMemByName(const char* meshName,void* pXmlDoc){ return 0;}
		//////////////////////////////////////////////////////////////////////////
		//���½ӿڲ�Ҫʹ��

		///���������Ƿ���з���Ч��
		///����b:�Ƿ�������
		///����plane����һ��4��Ԫ�ص����飬�������ƽ��ķ���
		virtual void SetIsReflectionObj(bool b,float* plane){ }

		///��ȡ�����Ƿ���з���Ч��
		///����:�Ƿ�������
		///����plane����һ��4��Ԫ�ص����飬�������ƽ��ķ���
		virtual bool GetIsReflectionObj(float* plane){ return false;}

		///���������Ƿ���뷴����Ⱦ
		virtual void SetCanBeReflection(bool b){}

		///��ȡ�����Ƿ���뷴����Ⱦ
		virtual bool GetCanBeReflection(){ return false;}

		///���÷���Զƽ��
		///������Զƽ�����
		virtual void SetReflectionFarClipPlane(float dis){}

		///���������Ƿ�����������
		virtual void SetIsRefracionObj(bool b){ }
		virtual bool GetIsRefracionObj(){ return false;}

		///����������
		virtual void SetRefracionIndex(float f){ }
		virtual float GetRefracionIndex(){ return 0;}

		//�༭�����ɹ�âʱ���õĽӿ�
		virtual void  Editor_GenerateMetalStar() { }
		//////////////////////////////////////////////////////////////////////////
		//��ȡ��â�ļ�����
		//	���أ���â����ֵ��
		//	������psize������Ͳ��������ع�â���ݴ�С��
		virtual void * Editor_GetMetalStarData (unsigned int* psize) { return 0;}

		//���ܣ�����һ��������ģ���ϵİ�Χ������ײ����Ϣ
		//����
		//rayOrigin������������������ɵ����飬��ʾһ�����ߵ���ʼ��
		//rayDir������������������ɵ����飬��ʾһ�����ߵķ���
		//ModelBVTestInfo ��һ�������׵�ַ�����ڽ���ķ��ء�������ײ���İ�Χ�а��Ⱥ�˳��ID����ײ�����귵�ظ��ͻ��ˡ�����Ĵ�С����С��һ��ģ�͵İ�Χ�и�������Χ�и������������ṩһ�����еõ�
		//nResultBufferCount�������Ԫ�ظ�������ֹ�����������Խ��
		//����ֵ�����صĽ���ĸ���,���Ϊ��˵��û����ײ
		virtual int  RayTestWithBoundingVolumes(const float* rayOrigin, const float * rayDir, ModelBoundingVolumeTestInfo *pOut, int nResultBufferCount) const { return 0;}
		//��ȡ��Χ��ĸ���
		virtual int	 GetBoundingVolumesCount() { return 0;}

		//���ܣ�������
		//	//����
		//	RayOrg��������������
		//	RayDir���������߷���
		//	fOutDistance�����淵�صĽ���������ֵ����ײ�㵽�������ľ��롣
		//����ֵ��������ģ���Ƿ���ײ
		virtual bool  RayTestWithTriangles(const H3DVec3 & RayOrg, const H3DVec3 & RayDir,float & fOutDistance) {return false;}

		//!������ؽӿ�
		//�������ߵõ��ҽ����ݼ�
		//�ڶ���������ʾ�Ƿ���Ҫת�����߿ռ�
		virtual int	  QueryAttachPoint(IAttachModelInfo& rayResInfo,bool bTransToLocal=true) { return 0;}

		//!�ҽӹ���.
		virtual int	  AttachModel(const IAttachModelInfo& rayInfo) {return 0;}

		//!ȡ���ҽ�
		virtual int	  DetachModel(const IAttachModelInfo& rayInfo) {return 0;}

		//!Ԥ����fx
		virtual int PreGenFx(H3DI::IFxInfo* p){ return 0;}

		///!����LightMap��ͼ�ļ�����Info��,MeshIDΪ��ģ�͵���������
		virtual int SetLightMapInfo(unsigned int MeshID, const ILightMapInfo& info){return 0;}

		/**
		lightmapSizeBuffer: mesh��Ӧ��lightmap size����geometry��mesh������һ��
		uvTexturePathName: ����Ԥ��uv��ֵĽ�����紫��"d:\\lightmapUVTest%d.bmp",��
		�����d:\\lightmapUVTest0.bmp, d:\\lightmapUVTest1.bmp���ļ���
		����ʱ��NULL
		bReadOnlyModified: ���ڷ����Ƿ��޸���lmm�ļ���ֻ�����ԣ�����ΪNULL
		����ֵ���ɹ�����true��ʧ�ܷ���false�����ʧ�ܣ������ø����lightmapsize�������������ֶ�չuv
		*/
		virtual bool GenerateLightmapUV(const int* lightmapSizeBuffer, const char* uvTexturePathName, int* bReadOnlyModified) {return 0;}

		///���ź���
		virtual void HelpFunction(int i,void* param0,void* param1=0,void* param2=0,void* param3=0,void* param4=0){ }

		virtual MODEL_TYPE				Type(){ return MODEL_STATIC;};
		virtual unsigned int			GetID(){return 0;};
		virtual unsigned int			PrepareDraw(){return 0;};
		virtual unsigned int			Draw(){return 0;};
		virtual void*					GetAllInside(int n){return 0;};
		//�˶�����
		virtual IAnimation*				GetAnimTrack(){return 0;};
		virtual void					SetAnimTrack(IAnimation* pAnim){}
		virtual float					GetCurAnimTime(){return 0;}
		virtual void					RunAnimation(bool bRun,float fStartTime){}
		virtual bool					IsAnimationRun(){return true;}

		virtual void					Update(){}
		virtual void					Update(float fIntervalTime){}

		virtual void                    GetAABB(float*){}
		virtual void					SetAABB(const float* boxWorld){}
		virtual float					Picking(const float* Dir,const float* Point,float* vecIntPoint=0){return 0;}

		virtual void                    GetLocalAABB(float*){}
		virtual const H3DMat4& GetLocationMatrix()const{return m_d_m;}
		virtual const H3DMat4& GetOrgMatrix()const{return m_d_m;}
		virtual void					SetLocationMatrix(const H3DMat4& mat){};//��������״̬�����Z�᷽����ת�Ƕ��Զ���0
		virtual void					SetOrgMatrix(const H3DMat4& mat){};//��������ԭʼ״̬�����
		virtual void					SetPosition(const H3DVec3&){};
		virtual void		            GetPosition(H3DVec3&)const{};

		virtual void					SetZRotation(float degree){};
		virtual float					GetZRotation()const {return 0;};
		virtual void					SetScale(const H3DVec3& scale){};
		virtual void	                GetScale(H3DVec3&)const{};
		virtual void					SetDirection(const H3DVec3& dir){};
		virtual void                    GetDirection(H3DVec3&)const{};

		virtual void					SetColor(const float* col, bool bUseColor){};
		float m_c[4];
		virtual const float*			GetColor()const{return m_c;};

		//��ι�ϵ���ƣ�������IMoveObject
		virtual unsigned int			GetNumChild()const{return 0;}
		virtual IMoveObject*			GetChild(unsigned int n){return 0;}
		virtual void					AddChild(IMoveObject* pMoveObj){}
		virtual IMoveObject*			GetParent(){return 0;}
		virtual void					SetParent(IMoveObject* pMoveObj){}

		virtual void					EnableSort(){}
		virtual void					DisableSort(){}
		virtual bool					IsSortEnabled(){return false;}
		virtual void					SetLOD(size_t nLOD){}
		virtual size_t					GetLOD()const{return 0;}
		virtual size_t					GetLODNum(){return 0;}
		virtual float					GetLodDist(size_t nLod)const{return 0;}

		virtual void					EnableChildrenUpdate(bool bEnable){}

		virtual IShader*				GetShader(){return 0;}
		virtual void					SetShader(IShader* pIShader){}
		virtual void					SetEffectShader(IShader* pShader){}
		///������������,����1��ʾ�ɹ���0��ʾʧ�ܡ�
		virtual int						ReloadAll(){return 1;}

		/*������
		0����ͬ��ReloadAll
		1��Reload Geometry
		2��Reload ���ʣ�������ͼ��
		3��Reload ��ͼ

		����ֵ-1Ϊʧ�ܣ�����0������ʾreload�˼�����ͼ
		*/
		virtual int						ReloadFromDisk(int reloadContent){return 0;}
		///��ñ�����ʹ�õ��Դ�
		virtual unsigned int GetVideoMemUse(){return 0;}

		///�����������
		virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;}

		///���ڴ�����һ������ʵ����ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){return 0;}

		//��AABB�����Ƿ��ĳ�������ཻ��point��dir����x��y��z��������
		//����ֵΪ1��ʾ�ཻ
		virtual int RayHitTestWithAABB(const H3DVec3& point,const H3DVec3& dir){return 0;}

		//������ɫϵ��
		//f����ɫϵ�������飬arraysize����ɫϵ�����鳤��
		//����ֵ=1��ʾ�ɹ�������ֵС��0��ʾ���ɹ������ɹ���ԭ���ǲ�����û�ж�����ɫϵ��
		virtual int SetMaterialColorFactor(float* f,int arraysize){return 0;}

		/// ���ù���ɫ��ɫϵ��,Ҫ��������float[4]��Ӱ�췽���� (Mat_d*C0.rgb)+C1.rgb��fC0��Alpha���ڱ��Ͷȼ��㣬fC1��Alpha�������ȼ���
		/// meshPart��ʾģ�ͽ��ܱ�ɫ�������ID������һ��ģ�Ϳ�����MeshID����
		///     ����ǽ�ɫ����ʾ��װ��BodypartID��Ҽ�ID(BODYPART_NUM+�Ҽ�ID)����ֵΪ-1ʱ������ɫ��ģ�����еĲ��֡�
		/// bUpdateChildrenΪtrueʱ������ɫ��ģ�͵��ӽڵ㣨����Ҽ��ϵ���Ч��
		virtual int SetDiffuseColorFactor(float* fC0,int arraysizeC0, float* fC1, int arraysizeC1, int meshPart=-1, bool bUpdateChildren=true){return 0;}

		//Ϊģ��������Ⱦ������������EEntityFlag�İ�λ����Ľ��
		virtual unsigned int			GetFlags(){return 0;}
		virtual void					SetFlags(unsigned int nFlags){;}

		///���ò���lod,nLod=0��Ϊ���ϸ�ڣ�nLod=2Ϊ���ϸ��
		virtual void SetMaterialLod(int nLod){}
		virtual int GetMaterialLod(){return 0;}

		H3DVec3 m_dummy_vec3;
		H3DMat4 m_dummy_mat4;
		CAction m_action;
		CBone m_bone;
		H3DQuat m_dummy_quat;


		virtual const H3DMat4&	 GetCurMatrix(unsigned int nBone)const{ return m_dummy_mat4;}
		//������Ⱦ���󡣺��߼��������������ͬ
		virtual const H3DMat4&	 GetRendingMatrix(unsigned int nBone)const{ return m_dummy_mat4;}
		virtual const H3DMat4&	 GetCurRalativeMatrix(unsigned int nBone)const{ return m_dummy_mat4;}
		virtual const H3DVec3&  GetCurPos(unsigned int nBone)const{ return m_dummy_vec3;}
		virtual const H3DQuat&	 GetCurRot(unsigned int nBone)const{ return m_dummy_quat;}

		virtual int					SetAction(const char* sName){return 0;}
		virtual int					SetMixAction(const char* sName, float fWeight, int nIdx, int nParam){return 0;}
		virtual void					GetMixAction(int nIdx, float& fWeight, int& nActionIdx){}
		virtual IAction*				GetCurAction(){ return 0;}
		virtual IAnimation*			GetCurBoneAnimation(unsigned int nBone){ return 0;}
		virtual float					GetCurActionTime(){return 0;}
		virtual void					SetCurActionTime(float fActionTime){}
		virtual void					SetActionTransitionTime(float fTime){}
		virtual float					GetActionTransitionTime()const{return 0;}

		virtual void					SetHeadDir(const H3DVec3& dir){}
		virtual void					SetWaistDir(const H3DVec3& dir){}

		virtual const H3DVec3& GetHeadDir()const{ return m_dummy_vec3;}
		virtual const H3DVec3& GetWaistDir()const{ return m_dummy_vec3;}

		CSkeleton m_ske;


		//================Used in action lib model====================
		virtual ISkeleton*					GetSkeleton(){return &m_ske;} // ���û��д��
		virtual const char*					GetActionName(unsigned int n){return "testname";} // ���û��д��
		virtual IAction*						GetAction(unsigned int n){return &m_action;}
		virtual IAction*						GetAction(const char* sActionName){return &m_action;}
		virtual unsigned int					GetNumAction()const{return 0;}
		virtual int								SetStaticMixAction(unsigned int id/*0, 1*/, const char* actName) {return 0;}
		virtual void							SetStaticActionMixWeight(float weight /*0-1*/) {}
		/*
		actNames = const char*[count]; interval = float[count-1]
		*/
		virtual void SetStaticActionSequence(const char** actNames, float* intervals, unsigned int count, bool loop) {}
		/*
		�õ�ĳһ��ͨ���Ķ���, id�Ϸ�ֵΪ[0,3]
		*/
		CAnimationChannel m_ac;
		virtual IAnimationChannel* GetAnmChannel(unsigned int id) {return &m_ac;} // ���û��д�꣬��Ҫ������д
		/*
		�������ͨ��������
		*/
		virtual void          ClearAllAnmChannels(void) {}
		//by zhengchengjun 2011-11-25
		/*
		������Щ����ͨ��Ϊ��ռͨ��, buff=NULL�������ͨ����Ϊ�Ƕ�ռͨ��
		*/
		virtual void   SetDominatingChannel(int* buff, unsigned int buffLen) {}

		virtual unsigned int   GetDominatingChannel(int* buff, unsigned int buffLen)const {return 0;}

		virtual void  SetChannelModifier(IAnimationChannelModifier* p) {}

		virtual IAnimationChannelModifier*  GetChannelModifier(void)const { return 0;}

		virtual void ClearChannelModifier(void) { }

		virtual void LoadBodyBounds(const char* fileName) { }


		virtual unsigned int GetActionNames(const char** buff, unsigned int buffLen) { return 0;}
		virtual unsigned int GetActionNamesCopy(char* buff, unsigned int buffLen, unsigned int nameLen) { return 0;}

		//by sssa2000 090203 Ϊ�༭����ӽӿ�
		//���ƹ���
		virtual void DrawSkeleton(){}
		virtual void CopyTo(ISkeletonModel *pModel)const {}
		//������������Ķ�����ʹ��ǰ����Ķ��������������������ĳ������,iBoneId�ǰ󶨵Ĺ���ID
		virtual bool	RelationAnimation(ISkeletonModel *pHorseModel,const char *animNameSelf,const char *animHorse,int iBoneId = 0) {return false;}

		//�ڸ��¹���ģ�͵�ʱ��,�Ƿ�Ը�����ͷ���м���, Ĭ��ֵΪtrue
		//�����ѡ��Ϊfalse, ��ISkeletonModel��copy������Ҳ����Ҫ������ͷ����.
		virtual void	SetUpdateBones(bool bUpdateBones) {}

		//�����������,̫������,����������Բ��ϸ��µ�Ӱ��
		virtual void	SetMountMatrix(const H3DMat4& mountMatrix){}

		//������������Ա������,����ָ��������ʵ������
		virtual int		GetPhxEntityNames(const char** pNames,int num,int phxType){return 0;}
		//��������ʵ�������,���ǲ��ϲ���ʱ,������ǲ���������
		virtual void   SetPhxEntityPara(const char* pName,void* phxPara,int phxType) {}
		//��ȡ����ʵ�������,���ǲ��ϲ���ʱ,������ǲ���������
		virtual void	GetPhxEntityPara(const char* pName,void* phxPara,int phxType) {}
		//��ȡ�������,�������������,Ŀǰֻ֧�ֲ���
		virtual void   GetPhxParaByName(const char* paraName,void* phxPara,int phxType) {}
		//!�Ӳ�������ɾ����һ������,Ŀǰֻ֧�ֲ���
		virtual void   DelPhxParaByName(const char* paraName,int phxType) {}
		//�½�һ������ʵ��
		virtual void   CreatePhxEntity(const char* pName,void* phxPara,int phxType) {}
		//ɾ��һ������ʵ��
		virtual void   DelPhxEntity(const char* pName) {}
		//������������ģ��,������EPhxDataTypeָ��
		virtual void   SavePhxDataTemplate(const char* pName,int phxDataType){}
		//�����������ݵ�bp3��,��ǰ����
		virtual void   SavePhxDataToBp3(const char* pFileName) {}
		//����.����clearCur�����Ƿ���Ҫ�������ǰ�Ѿ���ȡ�Ĳ���
		virtual void   LoadPhxDataTemplate(const char* pName,int phxDataType,bool clearCur) {}
		//��ȡ��ǰ���в��ϲ�����
		virtual int   GetAllPhxDataParas(const char** pNames,int bufferLen,int phxDataType) {return 0;}
		//�ں�������
		virtual void   BlendPhxResult() {}
		//��������
		virtual void   RenderPhxInfo() {}

		//���һ�������ڹ����ϵ���Ԫ�����ڶ�������֮�󣬽���ת���ӵ�������
		virtual void	AddQuatToBone(int index,const H3DQuat& quat) {}
		//������е���Ԫ��
		virtual void	ClearAdditionalQuat() {}
		//��ȡ���Ϲ�â�ļ�����
		//���أ���â����ֵ��
		//������psize������Ͳ��������ع�â���ݴ�С��
		virtual void *  Editor_GetClothMetalStarData (unsigned int* psize) {return 0;}
		//���Ź���,by lzp 2011-4-14
		virtual void    SetScaleInfo(const ScaleSkeletonInfo& sInfo){}
		virtual void	 ClearScaleInfo() {}

		//sssa2000 08.03.24:
		//��̬���ø����������
		virtual void                SetThisActorPhy(bool b){}
		//��ø����������״̬��ע�⣬������һ���� ����Ҫ��ʾ������һ��SetThisActorPhy������ÿ�ε�GetThisActorPhy������Ч
		virtual bool				GetThisActorPhy(){return false;}

		// ��ĳ��mesh�������򲹳�ֵ
		virtual void SetSortingOffset(const char* meshName, float offset) {}

		// ʹĳ��mesh������ʾ
		// bHightLight true : ���ø߹�Ч����false : �ָ�
		virtual void HightLightSubEntity(const char* subEntityName, bool bHightLight) {}

		//
		virtual void	UpdateCpuSkin(float t) {}

		//������Ƥ��Ԥ������̬
		virtual void  UpdateSkinPose(int bpID,bool bUpdate) {}

		virtual void					Release()
		{
			EnginePool::m_actor_pool->releaseObj(this);
		}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

		virtual bool				SetBodyPart(const char* sBodyName){return true;}
		virtual unsigned int		SetBodyPartAsyn(const char* sBodyName){return 0;}
		virtual void				SetSkinColor(const float* color){}
		virtual unsigned int         GetCloth(ICloth** buff, unsigned int buffCount) { return 0;}
		//add by 2007-12
		//����ACTORλ�ã�����Z��Զ������Ϊ0
		virtual void				SetActorLocationMatrix(const H3DMat4& mat){};
		//��BODYPART�������ƴ��BODYPART֮��Ķ���ͷ���
		virtual void				PostProcessCreating(){};
		virtual unsigned int		PostProcessCreatingAsyn(){return 0;};
		//�ı�IACTOR���ݣ��������һ��actor
		virtual int					ChangeToOtherActor(IActor* actor){return 0;};
		//�����outʵ���Ͼ���&this ,��������clone�Ѿ����
		virtual unsigned int		ChangeToOtherActorAsyn(IActor* actor,IActor** out){return 0;};

		//��̬����͸������
		virtual	void				SetThisActorClothSort(bool b){};
		virtual bool				GetThisActorClothSort(){return false;};

		///by sssa2000 2008.04.25
		// ���ͷ�������ˣ���ô���øú�������
		virtual void				ResetActorHair(){};

		//���Ժ���
		virtual void				DebugFunction(int i){};
		//add by puzzy, 2008-10-10
		//H3DSet geometry lod and texture lod
		virtual void				setGeometryLod(float lod){};
		virtual unsigned int		setFragmentLodAsyn(float n){return 0;};
		virtual void				setFragmentLod(float ){};
		virtual float				getGeometryLod(){return 0;};
		virtual float				getFragmentLod(){return 0;};

		virtual int GetSkeletonModels(IModel** pSkeletonModelVector,int vectorSize) {return 0;}
		virtual int GetAdornmentSkeletonModels(H3DI::IModel** pSkeletonModelVector, int vectorSize) {return 0;}

		//���������Ƿ�ʹ�ð�Χ��. ����ֵ1��ʾ�ɹ���С��0���ɹ�
		virtual int SetUseCustomBoundBox (bool b){return 0;}

		//ͨ��bodpyart�����ֻ�ȡ��bodypart ��ǰ������������renderable�����в��ʡ���������䵽pResArray
		//�������Ҫ���صĲ����������ڵ����߷��������ĳ��ȣ�����ֵΪ-1
		//����ֵ������ʵ�����Ĳ��ʸ���
		//����bpName��bodypart�����֣�����104001003������ǹҼ� Ҳ����
		//����pResArray:�ɵ����߷����INewShader�����飬
		//����len:����ĳ���
		virtual int GetAllShaderByBodyPartName(const char* bpName,INewShader** pResArray,int arraylen){ return -1;}

		//����bodypart���ֻ�ȡINewShader
		// ���ùҼ�
		virtual bool				SetAdornment(EActorAdornmentPosition adornmentposition, const char* adornmentname) {return true;}
	};

	class CAvatarSkeletonModel:public IAvatarSkeletonModel
	{
	public:

		H3DMat4 m_d_m;
		H3DVec3 m_d_v;
//MOVE OBJECT
		virtual MODEL_TYPE				Type(){ return MODEL_STATIC;};
		virtual unsigned int			GetID(){return 0;};
		virtual unsigned int			PrepareDraw(){return 0;};
		virtual unsigned int			Draw(){return 0;};
		virtual void*					GetAllInside(int n){return 0;};
		//�˶�����
		virtual IAnimation*				GetAnimTrack(){return 0;};
		virtual void					SetAnimTrack(IAnimation* pAnim){}
		virtual float					GetCurAnimTime(){return 0;}
		virtual void					RunAnimation(bool bRun,float fStartTime){}
		virtual bool					IsAnimationRun(){return true;}

		virtual void					Update(){}
		virtual void					Update(float fIntervalTime){}

		virtual void                    GetAABB(float*){}
		virtual void					SetAABB(const float* boxWorld){}
		virtual float					Picking(const float* Dir,const float* Point,float* vecIntPoint=0){return 0;}

		virtual void                    GetLocalAABB(float*){}
		virtual const H3DMat4& GetLocationMatrix()const{return m_d_m;}
		virtual const H3DMat4& GetOrgMatrix()const{return m_d_m;}
		virtual void					SetLocationMatrix(const H3DMat4& mat){};//��������״̬�����Z�᷽����ת�Ƕ��Զ���0
		virtual void					SetOrgMatrix(const H3DMat4& mat){};//��������ԭʼ״̬�����
		virtual void					SetPosition(const H3DVec3&){};
		virtual void		            GetPosition(H3DVec3&)const{};

		virtual void					SetZRotation(float degree){};
		virtual float					GetZRotation()const {return 0;};
		virtual void					SetScale(const H3DVec3& scale){};
		virtual void	                GetScale(H3DVec3&)const{};
		virtual void					SetDirection(const H3DVec3& dir){};
		virtual void                    GetDirection(H3DVec3&)const{};

		virtual void					SetColor(const float* col, bool bUseColor){};
		float m_c[4];
		virtual const float*			GetColor()const{return m_c;};

		//��ι�ϵ���ƣ�������IMoveObject
		virtual unsigned int			GetNumChild()const{return 0;}
		virtual IMoveObject*			GetChild(unsigned int n){return 0;}
		virtual void					AddChild(IMoveObject* pMoveObj){}
		virtual IMoveObject*			GetParent(){return 0;}
		virtual void					SetParent(IMoveObject* pMoveObj){}

		virtual void					EnableSort(){}
		virtual void					DisableSort(){}
		virtual bool					IsSortEnabled(){return false;}
		virtual void					SetLOD(size_t nLOD){}
		virtual size_t					GetLOD()const{return 0;}
		virtual size_t					GetLODNum(){return 0;}
		virtual float					GetLodDist(size_t nLod)const{return 0;}

		virtual void					EnableChildrenUpdate(bool bEnable){}

		virtual IShader*				GetShader(){return 0;}
		virtual void					SetShader(IShader* pIShader){}
		virtual void					SetEffectShader(IShader* pShader){}
		///������������,����1��ʾ�ɹ���0��ʾʧ�ܡ�
		virtual int						ReloadAll(){return 1;}

		/*������
		0����ͬ��ReloadAll
		1��Reload Geometry
		2��Reload ���ʣ�������ͼ��
		3��Reload ��ͼ

		����ֵ-1Ϊʧ�ܣ�����0������ʾreload�˼�����ͼ
		*/
		virtual int						ReloadFromDisk(int reloadContent){return 0;}
		///��ñ�����ʹ�õ��Դ�
		virtual unsigned int GetVideoMemUse(){return 0;}

		///�����������
		virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;}


		//��AABB�����Ƿ��ĳ�������ཻ��point��dir����x��y��z��������
		//����ֵΪ1��ʾ�ཻ
		virtual int RayHitTestWithAABB(const H3DVec3& point,const H3DVec3& dir){return 0;}

		//������ɫϵ��
		//f����ɫϵ�������飬arraysize����ɫϵ�����鳤��
		//����ֵ=1��ʾ�ɹ�������ֵС��0��ʾ���ɹ������ɹ���ԭ���ǲ�����û�ж�����ɫϵ��
		virtual int SetMaterialColorFactor(float* f,int arraysize){return 0;}

		/// ���ù���ɫ��ɫϵ��,Ҫ��������float[4]��Ӱ�췽���� (Mat_d*C0.rgb)+C1.rgb��fC0��Alpha���ڱ��Ͷȼ��㣬fC1��Alpha�������ȼ���
		/// meshPart��ʾģ�ͽ��ܱ�ɫ�������ID������һ��ģ�Ϳ�����MeshID����
		///     ����ǽ�ɫ����ʾ��װ��BodypartID��Ҽ�ID(BODYPART_NUM+�Ҽ�ID)����ֵΪ-1ʱ������ɫ��ģ�����еĲ��֡�
		/// bUpdateChildrenΪtrueʱ������ɫ��ģ�͵��ӽڵ㣨����Ҽ��ϵ���Ч��
		virtual int SetDiffuseColorFactor(float* fC0,int arraysizeC0, float* fC1, int arraysizeC1, int meshPart=-1, bool bUpdateChildren=true){return 0;}

		//Ϊģ��������Ⱦ������������EEntityFlag�İ�λ����Ľ��

		///���ò���lod,nLod=0��Ϊ���ϸ�ڣ�nLod=2Ϊ���ϸ��
		virtual void SetMaterialLod(int nLod){}
		virtual int GetMaterialLod(){return 0;}

//MODEL
		int m_flag;
		virtual unsigned int			GetFlags(){return m_flag;}
		virtual void					SetFlags(unsigned int nFlags){m_flag = nFlags;}

		virtual unsigned int			GetNumSubModel()const{return 0;}
		virtual void					SetTransparence(float trans){}
		virtual float					GetTransparence()const{return 0;}

		//���п���

		virtual void					SetGeometry(IGeometry* pGeom){}
		virtual IGeometry*				GetGeometry(){return 0;}
		virtual void					RunGeomAnimation(bool bRun,float fStartTime){}

		//CLONE
		virtual IModel*					Clone(){ return 0;}

		//LOD
		virtual void					EnableRender(){}
		virtual void					DisableRender(){}
		virtual bool					IsNeedRender(){return true;}
		virtual void					SetLoop(bool bLoop){}
		virtual bool					GetLoop()const{return false;}

		virtual void                    SetFakeShadowSize(float) {}
		virtual float                    GetFakeShadowSize(void)const{return 0;}

		//by sssa2000 090210
		virtual bool					IsCullByOctree(){return false;}
		virtual bool					IsCullByOccluder(){return false;}


		//���ӷ�Χ
		virtual	float GetVisibleDist() {return 100;}
		virtual	void SetVisibleDist(float dist){}

		//���һ��ģ�ͺŵĶ�����������ݣ�
		//pVertBuf��pIndexBuff�������������ָ�룬���ڴ�����ֵ
		//pVertNum,pIndexNum����������ĸ�������ǰpVertBuf��pIndexBuff�������ָ���ǣ����ض���������ĸ�����һ������´��붥��������ĸ�����
		//����ֵ��������Ķ�����������������ڵ���ʵ�ʵ�ģ�����������ʱ������,���򷵻ؼ�
		virtual bool GetVertexAndIndex( float *pVertBuf,unsigned int *pIndexBuff,int *pVertNum,int *pIndexNum ) { return false;}

		///�����������ģ�͵Ĳ���,����1��ʾ�ɹ���0��ʾʧ�ܡ�
		virtual int ReloadMaterialInstance(){return 0;}
		///��������fx
		virtual int ReloadFx(){return 0;}

		///����ģ�͵�ģ�����ӡ�0.0-1.0�ĸ�������Ϊ0ʱ��ʾģ����ȫ������Ϊ1�Ǳ�ʾ��ȫ�����˶�ģ����0-1֮�������ʾ�����˶�ģ���ĳ̶ȡ�
		virtual void SetMotionBlurFactor(float fFactor) {}
		///�����ڵ���ѯ��״̬
		virtual void	SetUseOcclusionQuery(bool bVal){}
		///��ȡ�Ƿ�ʹ���ڵ���ѯ��״̬
		virtual bool	UseOcclusionQuery() const {{return false;}}
		///��ò�ѯ�����0��ʾ����ȫ�ڵ���
		virtual unsigned int GetOcclusionQueryResult() {return 0;}


		//////////////////////////////////////////////////////////////////////////
		//�������

		//����������2����ɫc0,c1�����յ���ɫ��c0*������ɫ+c1
		//����ֵ1��ʾ�ɹ���0��ʾʧ�ܡ�ʧ�ܵ�ԭ��������Ĳ��ʲ�֧�ָò���
		virtual int HightLight(float r0,float b0,float c0,float r1,float b1,float c1){return 0;}

		///�õ��²��ʣ������������id����0��ʼ
		virtual H3DI::INewShader* GetNewShaderByIndex(unsigned int i){return 0;}

		///�õ��²��ʣ����������������
		virtual H3DI::INewShader* GetNewShaderByName(const char* name){return 0;}

		///�����²��� ��Ӱ�쵽����ģ�͵�����mesh
		virtual int SetNewShader(H3DI::INewShader* p){return 0;}

		///ͨ��һ������ʵ�����ļ����Ͳ���ʵ�������� ��һ���������ø�����ģ�ͣ���Ӱ�쵽����ģ�͵�����mesh
		///����ֵ�Ǵ��ļ��еõ���INewShader,0��ʾʧ��
		virtual H3DI::INewShader* SetNewShader(const char* filename,const char* matballName){return 0;}


		///�����²��� ��һ���������ø�ĳ��mesh������0��ʾʧ�ܣ�������û�и�mesh
		virtual int SetMeshNewShaderByName(const char* meshName,H3DI::INewShader* p){ return 0;}

		///ͨ��һ������ʵ�����ļ����Ͳ���ʵ�������� ��һ���������ø�ĳ��mesh
		///����ֵ�Ǵ��ļ��еõ���INewShader,0��ʾʧ��
		virtual H3DI::INewShader* SetMeshNewShaderByName(const char* meshName,const char* filename,const char* matballName){return 0;}

		///�����²��� ��һ���������ø�ĳ��mesh������0��ʾʧ�ܣ�������û�и�mesh
		virtual int SetMeshNewShaderByIndex(int meshIdx,H3DI::INewShader* p){return 0;}

		///ͨ��һ������ʵ�����ļ����Ͳ���ʵ�������� ��һ���������ø�ĳ��mesh
		///����ֵ�Ǵ��ļ��еõ���INewShader,0��ʾʧ��
		virtual H3DI::INewShader* SetMeshNewShaderByIndex(int meshIdx,const char* filename,const char* matballName){ return 0;}

		///���ڴ�����һ������ģ���ģ�ͣ�ģ�͵����������ܵ�Ӱ�졣����1��ʾ�ɹ�
		virtual int SetMaterialTemplateFromMem(void* pXmlDoc){ return 0;}

		///���ڴ�����һ������ģ���ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ��������ֵ�����
		virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){ return 0;}

		///���ڴ�����һ������ģ���ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		virtual int SetMeshMaterialTemplateFromMemByIndex(int meshIdx,void* pXmlDoc){ return 0;}

		///���ڴ�����һ������ʵ����ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		virtual int SetMeshMaterialInstanceFromMemByName(int meshIdx,void* pXmlDoc){ return 0;}

		///���ڴ�����һ������ʵ����ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		virtual int SetMeshMaterialInstanceFromMemByName(const char* meshName,void* pXmlDoc){ return 0;}
		//////////////////////////////////////////////////////////////////////////
		//���½ӿڲ�Ҫʹ��

		///���������Ƿ���з���Ч��
		///����b:�Ƿ�������
		///����plane����һ��4��Ԫ�ص����飬�������ƽ��ķ���
		virtual void SetIsReflectionObj(bool b,float* plane){ }

		///��ȡ�����Ƿ���з���Ч��
		///����:�Ƿ�������
		///����plane����һ��4��Ԫ�ص����飬�������ƽ��ķ���
		virtual bool GetIsReflectionObj(float* plane){ return false;}

		///���������Ƿ���뷴����Ⱦ
		virtual void SetCanBeReflection(bool b){}

		///��ȡ�����Ƿ���뷴����Ⱦ
		virtual bool GetCanBeReflection(){ return false;}

		///���÷���Զƽ��
		///������Զƽ�����
		virtual void SetReflectionFarClipPlane(float dis){}

		///���������Ƿ�����������
		virtual void SetIsRefracionObj(bool b){ }
		virtual bool GetIsRefracionObj(){ return false;}

		///����������
		virtual void SetRefracionIndex(float f){ }
		virtual float GetRefracionIndex(){ return 0;}

		//�༭�����ɹ�âʱ���õĽӿ�
		virtual void  Editor_GenerateMetalStar() { }
		//////////////////////////////////////////////////////////////////////////
		//��ȡ��â�ļ�����
		//	���أ���â����ֵ��
		//	������psize������Ͳ��������ع�â���ݴ�С��
		virtual void * Editor_GetMetalStarData (unsigned int* psize) { return 0;}

		//���ܣ�����һ��������ģ���ϵİ�Χ������ײ����Ϣ
		//����
		//rayOrigin������������������ɵ����飬��ʾһ�����ߵ���ʼ��
		//rayDir������������������ɵ����飬��ʾһ�����ߵķ���
		//ModelBVTestInfo ��һ�������׵�ַ�����ڽ���ķ��ء�������ײ���İ�Χ�а��Ⱥ�˳��ID����ײ�����귵�ظ��ͻ��ˡ�����Ĵ�С����С��һ��ģ�͵İ�Χ�и�������Χ�и������������ṩһ�����еõ�
		//nResultBufferCount�������Ԫ�ظ�������ֹ�����������Խ��
		//����ֵ�����صĽ���ĸ���,���Ϊ��˵��û����ײ
		virtual int  RayTestWithBoundingVolumes(const float* rayOrigin, const float * rayDir, ModelBoundingVolumeTestInfo *pOut, int nResultBufferCount) const { return 0;}
		//��ȡ��Χ��ĸ���
		virtual int	 GetBoundingVolumesCount() { return 0;}

		//���ܣ�������
		//	//����
		//	RayOrg��������������
		//	RayDir���������߷���
		//	fOutDistance�����淵�صĽ���������ֵ����ײ�㵽�������ľ��롣
		//����ֵ��������ģ���Ƿ���ײ
		virtual bool  RayTestWithTriangles(const H3DVec3 & RayOrg, const H3DVec3 & RayDir,float & fOutDistance) {return false;}

		//!������ؽӿ�
		//�������ߵõ��ҽ����ݼ�
		//�ڶ���������ʾ�Ƿ���Ҫת�����߿ռ�
		virtual int	  QueryAttachPoint(IAttachModelInfo& rayResInfo,bool bTransToLocal=true) { return 0;}

		//!�ҽӹ���.
		virtual int	  AttachModel(const IAttachModelInfo& rayInfo) {return 0;}

		//!ȡ���ҽ�
		virtual int	  DetachModel(const IAttachModelInfo& rayInfo) {return 0;}

		//!Ԥ����fx
		virtual int PreGenFx(H3DI::IFxInfo* p){ return 0;}

		///!����LightMap��ͼ�ļ�����Info��,MeshIDΪ��ģ�͵���������
		virtual int SetLightMapInfo(unsigned int MeshID, const ILightMapInfo& info){return 0;}

		/**
		lightmapSizeBuffer: mesh��Ӧ��lightmap size����geometry��mesh������һ��
		uvTexturePathName: ����Ԥ��uv��ֵĽ�����紫��"d:\\lightmapUVTest%d.bmp",��
		�����d:\\lightmapUVTest0.bmp, d:\\lightmapUVTest1.bmp���ļ���
		����ʱ��NULL
		bReadOnlyModified: ���ڷ����Ƿ��޸���lmm�ļ���ֻ�����ԣ�����ΪNULL
		����ֵ���ɹ�����true��ʧ�ܷ���false�����ʧ�ܣ������ø����lightmapsize�������������ֶ�չuv
		*/
		virtual bool GenerateLightmapUV(const int* lightmapSizeBuffer, const char* uvTexturePathName, int* bReadOnlyModified) {return 0;}

		///���ź���
		virtual void HelpFunction(int i,void* param0,void* param1=0,void* param2=0,void* param3=0,void* param4=0){ }

		H3DVec3 m_dummy_vec3;
		H3DMat4 m_dummy_mat4;
		CAction m_action;
		CBone m_bone;
		H3DQuat m_dummy_quat;
//skeletonmodel
		virtual const H3DMat4&	 GetCurMatrix(unsigned int nBone)const{ return m_dummy_mat4;}
		//������Ⱦ���󡣺��߼��������������ͬ
		virtual const H3DMat4&	 GetRendingMatrix(unsigned int nBone)const{ return m_dummy_mat4;}
		virtual const H3DMat4&	 GetCurRalativeMatrix(unsigned int nBone)const{ return m_dummy_mat4;}
		virtual const H3DVec3&  GetCurPos(unsigned int nBone)const{ return m_dummy_vec3;}
		virtual const H3DQuat&	 GetCurRot(unsigned int nBone)const{ return m_dummy_quat;}

		virtual int					SetAction(const char* sName){return 0;}
		virtual int					SetMixAction(const char* sName, float fWeight, int nIdx, int nParam){return 0;}
		virtual void					GetMixAction(int nIdx, float& fWeight, int& nActionIdx){}
		virtual IAction*				GetCurAction(){ return 0;}
		virtual IAnimation*			GetCurBoneAnimation(unsigned int nBone){ return 0;}
		virtual float					GetCurActionTime(){return 0;}
		virtual void					SetCurActionTime(float fActionTime){}
		virtual void					SetActionTransitionTime(float fTime){}
		virtual float					GetActionTransitionTime()const{return 0;}

		virtual void					SetHeadDir(const H3DVec3& dir){}
		virtual void					SetWaistDir(const H3DVec3& dir){}

		virtual const H3DVec3& GetHeadDir()const{ return m_dummy_vec3;}
		virtual const H3DVec3& GetWaistDir()const{ return m_dummy_vec3;}

		CSkeleton m_ske;


		//================Used in action lib model====================
		virtual ISkeleton*					GetSkeleton(){return &m_ske;} // ���û��д��
		virtual const char*					GetActionName(unsigned int n){return "testname";} // ���û��д��
		virtual IAction*						GetAction(unsigned int n){return &m_action;}
		virtual IAction*						GetAction(const char* sActionName){return &m_action;}
		virtual unsigned int					GetNumAction()const{return 0;}
		virtual int								SetStaticMixAction(unsigned int id/*0, 1*/, const char* actName) {return 0;}
		virtual void							SetStaticActionMixWeight(float weight /*0-1*/) {}
		/*
		actNames = const char*[count]; interval = float[count-1]
		*/
		virtual void SetStaticActionSequence(const char** actNames, float* intervals, unsigned int count, bool loop) {}
		/*
		�õ�ĳһ��ͨ���Ķ���, id�Ϸ�ֵΪ[0,3]
		*/
		CAnimationChannel m_ac;
		virtual IAnimationChannel* GetAnmChannel(unsigned int id) {return &m_ac;} // ���û��д�꣬��Ҫ������д
		/*
		�������ͨ��������
		*/
		virtual void          ClearAllAnmChannels(void) {}
		//by zhengchengjun 2011-11-25
		/*
		������Щ����ͨ��Ϊ��ռͨ��, buff=NULL�������ͨ����Ϊ�Ƕ�ռͨ��
		*/
		virtual void   SetDominatingChannel(int* buff, unsigned int buffLen) {}

		virtual unsigned int   GetDominatingChannel(int* buff, unsigned int buffLen)const {return 0;}

		virtual void  SetChannelModifier(IAnimationChannelModifier* p) {}

		virtual IAnimationChannelModifier*  GetChannelModifier(void)const { return 0;}

		virtual void ClearChannelModifier(void) { }

		virtual void LoadBodyBounds(const char* fileName) { }


		virtual unsigned int GetActionNames(const char** buff, unsigned int buffLen) { return 0;}
		virtual unsigned int GetActionNamesCopy(char* buff, unsigned int buffLen, unsigned int nameLen) { return 0;}

		//by sssa2000 090203 Ϊ�༭����ӽӿ�
		//���ƹ���
		virtual void DrawSkeleton(){}
		virtual void CopyTo(ISkeletonModel *pModel)const {}
		//������������Ķ�����ʹ��ǰ����Ķ��������������������ĳ������,iBoneId�ǰ󶨵Ĺ���ID
		virtual bool	RelationAnimation(ISkeletonModel *pHorseModel,const char *animNameSelf,const char *animHorse,int iBoneId = 0) {return false;}

		//�ڸ��¹���ģ�͵�ʱ��,�Ƿ�Ը�����ͷ���м���, Ĭ��ֵΪtrue
		//�����ѡ��Ϊfalse, ��ISkeletonModel��copy������Ҳ����Ҫ������ͷ����.
		virtual void	SetUpdateBones(bool bUpdateBones) {}

		//�����������,̫������,����������Բ��ϸ��µ�Ӱ��
		virtual void	SetMountMatrix(const H3DMat4& mountMatrix){}

		//������������Ա������,����ָ��������ʵ������
		virtual int		GetPhxEntityNames(const char** pNames,int num,int phxType){return 0;}
		//��������ʵ�������,���ǲ��ϲ���ʱ,������ǲ���������
		virtual void   SetPhxEntityPara(const char* pName,void* phxPara,int phxType) {}
		//��ȡ����ʵ�������,���ǲ��ϲ���ʱ,������ǲ���������
		virtual void	GetPhxEntityPara(const char* pName,void* phxPara,int phxType) {}
		//��ȡ�������,�������������,Ŀǰֻ֧�ֲ���
		virtual void   GetPhxParaByName(const char* paraName,void* phxPara,int phxType) {}
		//!�Ӳ�������ɾ����һ������,Ŀǰֻ֧�ֲ���
		virtual void   DelPhxParaByName(const char* paraName,int phxType) {}
		//�½�һ������ʵ��
		virtual void   CreatePhxEntity(const char* pName,void* phxPara,int phxType) {}
		//ɾ��һ������ʵ��
		virtual void   DelPhxEntity(const char* pName) {}
		//������������ģ��,������EPhxDataTypeָ��
		virtual void   SavePhxDataTemplate(const char* pName,int phxDataType){}
		//�����������ݵ�bp3��,��ǰ����
		virtual void   SavePhxDataToBp3(const char* pFileName) {}
		//����.����clearCur�����Ƿ���Ҫ�������ǰ�Ѿ���ȡ�Ĳ���
		virtual void   LoadPhxDataTemplate(const char* pName,int phxDataType,bool clearCur) {}
		//��ȡ��ǰ���в��ϲ�����
		virtual int   GetAllPhxDataParas(const char** pNames,int bufferLen,int phxDataType) {return 0;}
		//�ں�������
		virtual void   BlendPhxResult() {}
		//��������
		virtual void   RenderPhxInfo() {}

		//���һ�������ڹ����ϵ���Ԫ�����ڶ�������֮�󣬽���ת���ӵ�������
		virtual void	AddQuatToBone(int index,const H3DQuat& quat) {}
		//������е���Ԫ��
		virtual void	ClearAdditionalQuat() {}
		//��ȡ���Ϲ�â�ļ�����
		//���أ���â����ֵ��
		//������psize������Ͳ��������ع�â���ݴ�С��
		virtual void *  Editor_GetClothMetalStarData (unsigned int* psize) {return 0;}
		//���Ź���,by lzp 2011-4-14
		virtual void    SetScaleInfo(const ScaleSkeletonInfo& sInfo){}
		virtual void	 ClearScaleInfo() {}

		//sssa2000 08.03.24:
		//��̬���ø����������
		virtual void                SetThisActorPhy(bool b){}
		//��ø����������״̬��ע�⣬������һ���� ����Ҫ��ʾ������һ��SetThisActorPhy������ÿ�ε�GetThisActorPhy������Ч
		virtual bool				GetThisActorPhy(){return false;}

		// ��ĳ��mesh�������򲹳�ֵ
		virtual void SetSortingOffset(const char* meshName, float offset) {}

		// ʹĳ��mesh������ʾ
		// bHightLight true : ���ø߹�Ч����false : �ָ�
		virtual void HightLightSubEntity(const char* subEntityName, bool bHightLight) {}

		//
		virtual void	UpdateCpuSkin(float t) {}

		//������Ƥ��Ԥ������̬
		virtual void  UpdateSkinPose(int bpID,bool bUpdate) {}



		virtual void					Release()
		{
			EnginePool::m_avaskeleton_pool->releaseObj(this);
		}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

//avatarskeletonmodel
		//!����bodypart��������bpt�ļ���·��
		virtual bool SetBodyPart(const char* bpt_path){return true;};
		virtual unsigned int SetBodyPartAsyn(const char* bpt_path){return 0;};

		//!���ùҼ�������0��λ�õ�����������1��spe�ļ���·��
		virtual bool SetAdornment(EPetAdornmentPosition adornmentposition, const char* adornmentname){return true;};
		virtual unsigned int SetAdornmentAsyn(int pos_idx,const char* spe_path){return 0;};

		//!lod���
		virtual void				setGeometryLod(float lod){}
		virtual unsigned int		setFragmentLodAsyn(float n){return 0;};
		virtual void				setFragmentLod(float ){}

		virtual float				getGeometryLod(){return 0;};
		virtual float				getFragmentLod(){return 0;};

		virtual int GetSkeletonModels(IModel** pSkeletonModelVector,int vectorSize) { return 0;};
		virtual int GetAdornmentSkeletonModels(H3DI::IModel** pSkeletonModelVector, int vectorSize) { return 0;};

		//���������Ƿ�ʹ�ð�Χ��. ����ֵ1��ʾ�ɹ���С��0���ɹ�
		virtual int SetUseCustomBoundBox (bool b){return 0;}; 

		//���Ժ���
		virtual void				DebugFunction(int i){ };
	};



	class CPrePassLight : public IPrePassLight
	{
	public:
		CPrePassLight():m_light_type(LIGHT_DIR)
		{
		}

		LIGHT_TYPE m_light_type;

		H3DMat4 m_d_m;
		H3DVec3 m_d_v;


		virtual MODEL_TYPE				Type(){ return MODEL_STATIC;};
		virtual unsigned int			GetID(){return 0;};
		virtual unsigned int			PrepareDraw(){return 0;};
		virtual unsigned int			Draw(){return 0;};
		virtual void*					GetAllInside(int n){return 0;};
		//�˶�����
		virtual IAnimation*				GetAnimTrack(){return 0;};
		virtual void					SetAnimTrack(IAnimation* pAnim){}
		virtual float					GetCurAnimTime(){return 0;}
		virtual void					RunAnimation(bool bRun,float fStartTime){}
		virtual bool					IsAnimationRun(){return true;}

		virtual void					Update(){}
		virtual void					Update(float fIntervalTime){}

		virtual void                    GetAABB(float*){}
		virtual void					SetAABB(const float* boxWorld){}
		virtual float					Picking(const float* Dir,const float* Point,float* vecIntPoint=0){return 0;}

		virtual void                    GetLocalAABB(float*){}
		virtual const H3DMat4& GetLocationMatrix()const{return m_d_m;}
		virtual const H3DMat4& GetOrgMatrix()const{return m_d_m;}
		virtual void					SetLocationMatrix(const H3DMat4& mat){};//��������״̬�����Z�᷽����ת�Ƕ��Զ���0
		virtual void					SetOrgMatrix(const H3DMat4& mat){};//��������ԭʼ״̬�����
		virtual void					SetPosition(const H3DVec3&){};
		virtual void		            GetPosition(H3DVec3&)const{};

		virtual void					SetZRotation(float degree){};
		virtual float					GetZRotation()const {return 0;};
		virtual void					SetScale(const H3DVec3& scale){};
		virtual void	                GetScale(H3DVec3&)const{};
		virtual void					SetDirection(const H3DVec3& dir){};
		virtual void                    GetDirection(H3DVec3&)const{};

		virtual void					SetColor(const float* col, bool bUseColor){};
		float m_c[4];
		virtual const float*			GetColor()const{return m_c;};

		//��ι�ϵ���ƣ�������IMoveObject
		virtual unsigned int			GetNumChild()const{return 0;}
		virtual IMoveObject*			GetChild(unsigned int n){return 0;}
		virtual void					AddChild(IMoveObject* pMoveObj){}
		virtual IMoveObject*			GetParent(){return 0;}
		virtual void					SetParent(IMoveObject* pMoveObj){}

		virtual void					EnableSort(){}
		virtual void					DisableSort(){}
		virtual bool					IsSortEnabled(){return false;}
		virtual void					SetLOD(size_t nLOD){}
		virtual size_t					GetLOD()const{return 0;}
		virtual size_t					GetLODNum(){return 0;}
		virtual float					GetLodDist(size_t nLod)const{return 0;}

		virtual void					EnableChildrenUpdate(bool bEnable){}

		virtual IShader*				GetShader(){return 0;}
		virtual void					SetShader(IShader* pIShader){}
		virtual void					SetEffectShader(IShader* pShader){}
		///������������,����1��ʾ�ɹ���0��ʾʧ�ܡ�
		virtual int						ReloadAll(){return 1;}

		/*������
		0����ͬ��ReloadAll
		1��Reload Geometry
		2��Reload ���ʣ�������ͼ��
		3��Reload ��ͼ

		����ֵ-1Ϊʧ�ܣ�����0������ʾreload�˼�����ͼ
		*/
		virtual int						ReloadFromDisk(int reloadContent){return 0;}
		///��ñ�����ʹ�õ��Դ�
		virtual unsigned int GetVideoMemUse(){return 0;}

		///�����������
		virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;}

		///���ڴ�����һ������ʵ����ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){return 0;}

		//��AABB�����Ƿ��ĳ�������ཻ��point��dir����x��y��z��������
		//����ֵΪ1��ʾ�ཻ
		virtual int RayHitTestWithAABB(const H3DVec3& point,const H3DVec3& dir){return 0;}

		//������ɫϵ��
		//f����ɫϵ�������飬arraysize����ɫϵ�����鳤��
		//����ֵ=1��ʾ�ɹ�������ֵС��0��ʾ���ɹ������ɹ���ԭ���ǲ�����û�ж�����ɫϵ��
		virtual int SetMaterialColorFactor(float* f,int arraysize){return 0;}

		/// ���ù���ɫ��ɫϵ��,Ҫ��������float[4]��Ӱ�췽���� (Mat_d*C0.rgb)+C1.rgb��fC0��Alpha���ڱ��Ͷȼ��㣬fC1��Alpha�������ȼ���
		/// meshPart��ʾģ�ͽ��ܱ�ɫ�������ID������һ��ģ�Ϳ�����MeshID����
		///     ����ǽ�ɫ����ʾ��װ��BodypartID��Ҽ�ID(BODYPART_NUM+�Ҽ�ID)����ֵΪ-1ʱ������ɫ��ģ�����еĲ��֡�
		/// bUpdateChildrenΪtrueʱ������ɫ��ģ�͵��ӽڵ㣨����Ҽ��ϵ���Ч��
		virtual int SetDiffuseColorFactor(float* fC0,int arraysizeC0, float* fC1, int arraysizeC1, int meshPart=-1, bool bUpdateChildren=true){return 0;}

		//Ϊģ��������Ⱦ������������EEntityFlag�İ�λ����Ľ��
		virtual unsigned int			GetFlags(){return 0;}
		virtual void					SetFlags(unsigned int nFlags){;}

		///���ò���lod,nLod=0��Ϊ���ϸ�ڣ�nLod=2Ϊ���ϸ��
		virtual void SetMaterialLod(int nLod){}
		virtual int GetMaterialLod(){return 0;}

		virtual void					Release()
		{
			EnginePool::m_light_pool->releaseObj(this);
		}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}


		virtual void SetLightAffectParam(LightAffectParam t){};
		virtual LightAffectParam GetLightAffectParam(){return LightAffectParam();};

		virtual LIGHT_TYPE				GetLightType()const{return m_light_type;};

		virtual void                    GetColor(float* vec4) const {};
		virtual void					SetColor(const float* col) {};

		//	��ǿ,������ǿ��Ϊ�������ֱ�Ӱ�����ɫ�͸߹⣬�������͹⡢�ӳ���Ⱦֻʹ�õ�һ����ǿ��
		//	��ȡ��ǿindex=1ʱȡ�߹��ǿSpecIntensity,index=0ʱȥԭʼ��ǿintensity
		virtual void					SetIntensity(float intensity, float SpecIntensity=-1.0f) {};
		virtual float					GetIntensity(int index=0) const {return 1;};


		// �۹�Ƶ�����Ƕȣ���ǿ���ڽǿ�ʼ˥����������޹��ա���λΪ�Ƕ�
		virtual void					GetAngle(float* inner, float* outter) const{};
		virtual void					SetAngle(float inner, float outter) {};

		// ����������Ƿ������Ӱ�Լ���Ӱ����ɫ
		virtual bool					GetShadowEnable() const {return true;};
		virtual void					SetShadowEnable(bool bEnable) {};

		virtual void					GetShadowColor(float* vec4) const{};
		virtual void					SetShadowColor(const float* col) {};

		// ˥����Χ����near��ʼ˥������far����
		virtual void					GetRange(float* nearRange, float* farRange) const {};;
		virtual void					SetRange(float nearRange, float farRange) {};;

		// ���ȼ�
		virtual int						GetPriority() const {return 1;};
		virtual void					SetPriority(int nPriority) {};

		// �Ƿ�ʹ��ͶӰ��ͼ
		virtual bool					GetProjTextureEnable() const {return false;};
		virtual void					SetProjTextureEnable(bool bEnable) {};
		// ͶӰ��ͼ���ļ���
		virtual void					SetProjTextureName(const char* strFileName){};

		// �Ƿ�ʹ�ù����ͼ
		virtual bool					GetFakeLightSpot() const {return false;};
		virtual void					EnableFakeLightSpot(bool bEnable) {};
		// �����ͼ���ļ���
		virtual void					SetFakeLightSpotTexture(const char* strFileName) {};

		//����һ������
		virtual void CreateLightBeam(EH3DBEAMTYPE beamType = E_H3D_BEAM_VOL) {};
		//���ù�������
		virtual void SetLightBeamType(EH3DBEAMTYPE beamType) {};
		//�ͷŹ���.���ͷŵ���ǰ�Ѿ����������й�������
		virtual void ReleaseLightBeam() {};
		//����һ�����.����һ���ƹ���ж�����,�������ﷵ�ع�ߵ�����
		virtual int CreateGlare(const tGlareProperty& gPro) {return 0;};
		//�ͷŹ��
		virtual void ReleaseGlare(int glareIndex) {};
		//���ù������ؾ���
		virtual void SetBeamLocalMat(const H3DMat4& mat) {};
		//���ù�ߵ�����
		virtual void SetGlareProperty(int glareIndex,const tGlareProperty& gPro) {};
		//��ȡ��ߵ�����
		virtual void GetGlareProperty(int glareIndex,tGlareProperty& gPro) {};
		//���ù����Ŀɼ���
		virtual void SetBeamVisable(bool b) {};
		//����/ͣ�ù�Դ
		virtual void SetLightEnable(bool b) {}
		//
		virtual bool GetLightEnable() {return true;}
		//���ù���������
		virtual void SetBeamBrightness(float t) {}
		//���ù������ھ���0.1~1��
		virtual void SetBeamInnerRadius(float t) {}
		//���ú��������ģ����(0~1)
		virtual void SetBeamBlurCoeff(float f) {}

		// ����˥����Χ����near��ʼ˥������far����
		virtual void GetBeamRange(float* nearRange, float* farRange) const {}
		virtual void SetBeamRange(float nearRange, float farRange) {}
		// ��������Ƕȣ���λΪ�Ƕ�
		virtual void GetBeamAngle(float* inner, float* outter) const {}
		virtual void SetBeamAngle(float inner, float outter) {}
		//===============================LightShaft===================================
		//add by liren 2010-11-2 
		//!������ر������
		virtual void SetLightShaftEnable(bool bEnable) {}
		//!�����������ɫ
		virtual void SetLightShaftColor(const float *pcolor){}
		//!������������� 
		virtual void SetLightShaftDepth(float fDepth) {}
		//!������Ƿ���
		virtual bool GetLightShaftEnable() const {return false;}
		//!��ȡ�������ɫ
		virtual void GetLightShaftColor(float *pColor) const {}
		//!��ȡ��������� 
		virtual float GetLightShaftDepth() const {return 1;}

		//!�����������ֵ
		virtual void SetLightShaftThreshold(float fThreshold) {}
		//!�����������ֵ
		virtual float GetLightShaftThreshold()const {return 1;}
		//!���������ǿ��
		virtual float GetLightShaftIntensity() const {return 1;}
		//!��ȡ�������ɫ
		virtual void SetLightShaftIntensity(float fIntensity) {}
		//===============================!LightShaft===================================

		//!���õƹ��GUID
		virtual void SetGUID(const char* szGUID) {}
	};

	class CCamera : public ICamera
	{
	public:
		H3DVec3 m_c_v;

		H3DMat4 m_d_m;
		H3DVec3 m_d_v;


		virtual MODEL_TYPE				Type(){ return MODEL_STATIC;};
		virtual unsigned int			GetID(){return 0;};
		virtual unsigned int			PrepareDraw(){return 0;};
		virtual unsigned int			Draw(){return 0;};
		virtual void*					GetAllInside(int n){return 0;};
		//�˶�����
		virtual IAnimation*				GetAnimTrack(){return 0;};
		virtual void					SetAnimTrack(IAnimation* pAnim){}
		virtual float					GetCurAnimTime(){return 0;}
		virtual void					RunAnimation(bool bRun,float fStartTime){}
		virtual bool					IsAnimationRun(){return true;}

		virtual void					Update(){}
		virtual void					Update(float fIntervalTime){}

		virtual void                    GetAABB(float*){}
		virtual void					SetAABB(const float* boxWorld){}
		virtual float					Picking(const float* Dir,const float* Point,float* vecIntPoint=0){return 0;}

		virtual void                    GetLocalAABB(float*){}
		virtual const H3DMat4& GetLocationMatrix()const{return m_d_m;}
		virtual const H3DMat4& GetOrgMatrix()const{return m_d_m;}
		virtual void					SetLocationMatrix(const H3DMat4& mat){};//��������״̬�����Z�᷽����ת�Ƕ��Զ���0
		virtual void					SetOrgMatrix(const H3DMat4& mat){};//��������ԭʼ״̬�����
		virtual void					SetPosition(const H3DVec3&){};
		virtual void		            GetPosition(H3DVec3&)const{};

		virtual void					SetZRotation(float degree){};
		virtual float					GetZRotation()const {return 0;};
		virtual void					SetScale(const H3DVec3& scale){};
		virtual void	                GetScale(H3DVec3&)const{};
		virtual void					SetDirection(const H3DVec3& dir){};
		virtual void                    GetDirection(H3DVec3&)const{};

		virtual void					SetColor(const float* col, bool bUseColor){};
		float m_c[4];
		virtual const float*			GetColor()const{return m_c;};

		//��ι�ϵ���ƣ�������IMoveObject
		virtual unsigned int			GetNumChild()const{return 0;}
		virtual IMoveObject*			GetChild(unsigned int n){return 0;}
		virtual void					AddChild(IMoveObject* pMoveObj){}
		virtual IMoveObject*			GetParent(){return 0;}
		virtual void					SetParent(IMoveObject* pMoveObj){}

		virtual void					EnableSort(){}
		virtual void					DisableSort(){}
		virtual bool					IsSortEnabled(){return false;}
		virtual void					SetLOD(size_t nLOD){}
		virtual size_t					GetLOD()const{return 0;}
		virtual size_t					GetLODNum(){return 0;}
		virtual float					GetLodDist(size_t nLod)const{return 0;}

		virtual void					EnableChildrenUpdate(bool bEnable){}

		virtual IShader*				GetShader(){return 0;}
		virtual void					SetShader(IShader* pIShader){}
		virtual void					SetEffectShader(IShader* pShader){}
		///������������,����1��ʾ�ɹ���0��ʾʧ�ܡ�
		virtual int						ReloadAll(){return 1;}

		/*������
		0����ͬ��ReloadAll
		1��Reload Geometry
		2��Reload ���ʣ�������ͼ��
		3��Reload ��ͼ

		����ֵ-1Ϊʧ�ܣ�����0������ʾreload�˼�����ͼ
		*/
		virtual int						ReloadFromDisk(int reloadContent){return 0;}
		///��ñ�����ʹ�õ��Դ�
		virtual unsigned int GetVideoMemUse(){return 0;}

		///�����������
		virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;}

		///���ڴ�����һ������ʵ����ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){return 0;}

		//��AABB�����Ƿ��ĳ�������ཻ��point��dir����x��y��z��������
		//����ֵΪ1��ʾ�ཻ
		virtual int RayHitTestWithAABB(const H3DVec3& point,const H3DVec3& dir){return 0;}

		//������ɫϵ��
		//f����ɫϵ�������飬arraysize����ɫϵ�����鳤��
		//����ֵ=1��ʾ�ɹ�������ֵС��0��ʾ���ɹ������ɹ���ԭ���ǲ�����û�ж�����ɫϵ��
		virtual int SetMaterialColorFactor(float* f,int arraysize){return 0;}

		/// ���ù���ɫ��ɫϵ��,Ҫ��������float[4]��Ӱ�췽���� (Mat_d*C0.rgb)+C1.rgb��fC0��Alpha���ڱ��Ͷȼ��㣬fC1��Alpha�������ȼ���
		/// meshPart��ʾģ�ͽ��ܱ�ɫ�������ID������һ��ģ�Ϳ�����MeshID����
		///     ����ǽ�ɫ����ʾ��װ��BodypartID��Ҽ�ID(BODYPART_NUM+�Ҽ�ID)����ֵΪ-1ʱ������ɫ��ģ�����еĲ��֡�
		/// bUpdateChildrenΪtrueʱ������ɫ��ģ�͵��ӽڵ㣨����Ҽ��ϵ���Ч��
		virtual int SetDiffuseColorFactor(float* fC0,int arraysizeC0, float* fC1, int arraysizeC1, int meshPart=-1, bool bUpdateChildren=true){return 0;}

		//Ϊģ��������Ⱦ������������EEntityFlag�İ�λ����Ľ��
		virtual unsigned int			GetFlags(){return 0;}
		virtual void					SetFlags(unsigned int nFlags){;}

		///���ò���lod,nLod=0��Ϊ���ϸ�ڣ�nLod=2Ϊ���ϸ��
		virtual void SetMaterialLod(int nLod){}
		virtual int GetMaterialLod(){return 0;}

		virtual void					Release()
		{
			EnginePool::m_camera_pool->releaseObj(this);
		}
		virtual bool					IsValid()const{return true;}

		virtual void                    SetUserPtr(void*){}
		virtual void*                  GetUserPtr(void)const {return 0;}

		//!�õ����ϵķ���
		virtual const H3DVec3& GetUp(void) {return m_c_v;};
		//!�õ����ҵķ���
		virtual const H3DVec3& GetRight(void) {return m_c_v;};
		//!�ӵ�ǰλ����position��
		virtual void LookAt(const H3DVec3& position) {}
		//!��eyePos��atPos��
		virtual void LookAt(const H3DVec3& eyePos, const H3DVec3& atPos, const H3DVec3& upVec) {}
		//!����ǰ����ת,���Ƕ�
		virtual void Roll(float degree) {}
		//!�����ҷ���ת,���Ƕ�
		virtual void Yaw(float degree) {}
		//!�����Ϸ���ת,���Ƕ�
		virtual void Pitch(float degree) {}
		//!����axisתdegree�Ƕ�
		virtual void Rotate(const H3DVec3& axis, float degree) {}
		//!�����ƶ�step����
		virtual void MoveUp(float step) {}
		//!�����ƶ�step����
		virtual void MoveRight(float step) {}
		//!��ǰ�ƶ�step����
		virtual void MoveFront(float step) {};
		//!�õ�view martix
		virtual const H3DMat4& GetViewMatrix(void)const {return m_d_m;};
		//!�õ�ͶӰmatrix
		virtual const H3DMat4& GetProjectionMatrix(void)const {return m_d_m;};

		virtual ~CCamera(){}


	};


	// CPrePassLight t;
	//CActor tt;
	//CCamera ttt;

}
#endif