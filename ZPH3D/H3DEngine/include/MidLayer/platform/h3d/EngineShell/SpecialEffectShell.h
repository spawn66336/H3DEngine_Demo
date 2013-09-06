#ifndef SPECIALEFFECTSHELL
#define SPECIALEFFECTSHELL
#include "../EffectCore_dx.h"
#include "instance_pool.h"

namespace H3DI
{
	//////////////////////////////////////////////////////////////////////////
	///��Ҫ������setActor(),unSetActor(),update(),render()
	class CSpecialEffect : public ISpecialEffect{
	public:

		virtual H3DI::IMoveObject* GetModelInTreeNode(const char* inTreeNodeName) { return 0;}

		//OBJECT
		virtual void Release()
		{
			EnginePool::m_effect_pool->releaseObj(this);
		};
		virtual bool					IsValid()const{return true;};

		virtual void                    SetUserPtr(void*){};
		virtual void*                  GetUserPtr(void)const {return 0;};
		//MOVE OBJECT
		virtual MODEL_TYPE				Type(){return OBJECT_SPECIALEFFECT;};
		virtual unsigned int			GetID(){static int moveObjNumber=0; return moveObjNumber++;};
		virtual unsigned int			PrepareDraw(){return 0;};
		virtual unsigned int			Draw(){return 0;};//����֮��͸��OBJECT��������Լ���
		virtual void*					GetAllInside(int n){return 0;};
		//�˶�����
		virtual IAnimation*				GetAnimTrack(){return 0;};
		virtual void					SetAnimTrack(IAnimation* pAnim){};
		virtual float					GetCurAnimTime(){return 0;};
		virtual void					RunAnimation(bool bRun,float fStartTime){};
		virtual bool					IsAnimationRun(){return 0;};

		virtual void					Update(){};
		virtual void					Update(float fIntervalTime){};

		virtual void                    GetAABB(float*){};
		virtual void					SetAABB(const float* boxWorld){};
		virtual float					Picking(const float* Dir,const float* Point,float* vecIntPoint=0){return 0;};

		virtual void                    GetLocalAABB(float*){};
		virtual const H3DMat4& GetLocationMatrix()const{static H3DMat4 localMat4; localMat4.Identity();return localMat4;};
		virtual const H3DMat4& GetOrgMatrix()const{ static H3DMat4 orgMat4; orgMat4.Identity();return orgMat4;};
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
		virtual const float*			GetColor()const{static float ca[4]={0};return ca;};

		//��ι�ϵ���ƣ�������IMoveObject
		virtual unsigned int			GetNumChild()const{return 0;};
		virtual IMoveObject*			GetChild(unsigned int n){return 0;};
		virtual void					AddChild(IMoveObject* pMoveObj){};
		virtual IMoveObject*			GetParent(){return 0;};
		virtual void					SetParent(IMoveObject* pMoveObj){};

		virtual void					EnableSort(){};
		virtual void					DisableSort(){};
		virtual bool					IsSortEnabled(){return 0;};
		virtual void					SetLOD(size_t nLOD){};
		virtual size_t					GetLOD()const{return 0;};
		virtual size_t					GetLODNum(){return 0;};
		virtual float					GetLodDist(size_t nLod)const{return 0;};

		virtual void					EnableChildrenUpdate(bool bEnable){};

		virtual IShader*				GetShader(){return 0;};
		virtual void					SetShader(IShader* pIShader){};
		virtual void					SetEffectShader(IShader* pShader){};
		///������������,����1��ʾ�ɹ���0��ʾʧ�ܡ�
		virtual int						ReloadAll()	{return 1;};

		/*������
		0����ͬ��ReloadAll
		1��Reload Geometry
		2��Reload ���ʣ�������ͼ��
		3��Reload ��ͼ

		����ֵ-1Ϊʧ�ܣ�����0������ʾreload�˼�����ͼ
		*/
		//virtual int						ReloadFromDisk(int reloadContent){return 0;};
		///��ñ�����ʹ�õ��Դ�
		//virtual unsigned int GetVideoMemUse(){return 0;};

		///�����������
		//virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;};

		///���ڴ�����һ������ʵ����ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		//virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){};

		//��AABB�����Ƿ��ĳ�������ཻ��point��dir����x��y��z��������
		//����ֵΪ1��ʾ�ཻ
		virtual int RayHitTestWithAABB(const H3DVec3& point,const H3DVec3& dir){return 0;};

		//������ɫϵ��
		//f����ɫϵ�������飬arraysize����ɫϵ�����鳤��
		//����ֵ=1��ʾ�ɹ�������ֵС��0��ʾ���ɹ������ɹ���ԭ���ǲ�����û�ж�����ɫϵ��
		virtual int SetMaterialColorFactor(float* f,int arraysize){return 1;};

		/// ���ù���ɫ��ɫϵ��,Ҫ��������float[4]��Ӱ�췽���� (Mat_d*C0.rgb)+C1.rgb��fC0��Alpha���ڱ��Ͷȼ��㣬fC1��Alpha�������ȼ���
		/// meshPart��ʾģ�ͽ��ܱ�ɫ�������ID������һ��ģ�Ϳ�����MeshID����
		///     ����ǽ�ɫ����ʾ��װ��BodypartID��Ҽ�ID(BODYPART_NUM+�Ҽ�ID)����ֵΪ-1ʱ������ɫ��ģ�����еĲ��֡�
		/// bUpdateChildrenΪtrueʱ������ɫ��ģ�͵��ӽڵ㣨����Ҽ��ϵ���Ч��
		virtual int SetDiffuseColorFactor(float* fC0,int arraysizeC0, float* fC1, int arraysizeC1, int meshPart=-1, bool bUpdateChildren=true){return 0;};

		//Ϊģ��������Ⱦ������������EEntityFlag�İ�λ����Ľ��
		virtual unsigned int			GetFlags(){return 0;};
		virtual void					SetFlags(unsigned int nFlags){};

		///���ò���lod,nLod{}��Ϊ���ϸ�ڣ�nLod=2Ϊ���ϸ��
		virtual void SetMaterialLod(int nLod){};
		virtual int GetMaterialLod(){return 0;};

		//special effect
		/**
		*���ļ�����һ����Ч
		*/
		virtual bool  load(const char* filename) {return true;};

		/// ��õ�ǰ��Ч�Ķ�Ӧ�ļ�����
		virtual const char*  getName() {return "";};
		/**
		*�趨��Ч�ķ����ߡ����ж�������ߣ����ϼ���
		*/
		virtual void  setActor(EffectActor eActor[], int actorCount, int type =0) {};

		virtual void  setObject(H3DI::ISkeletonModel* actor) {};

		///
		///�����Ч�ķ����ߣ��������Խ�����Чֱ����������������
		virtual void  unsetActor() {};

		/// 
		/// ʹ����Ч��������һ��Ч
		virtual void  attachTo(ISpecialEffect* pEffect, bool isLink = false, int index = 1) {};

		/// 
		/// ȡ����ǰ��Ч�����ӡ��統ǰ��Чδ�������κ���Ч��ִ�пղ�����
		virtual void  detach() {};

		/// 
		/// �õ���Ч��״̬
		virtual int  getState() {return 0;};

		/// 
		/// �õ���Ч�ĵ�ǰʱ��
		virtual float  getCurrentTime() {return 0;};

		/// 
		///������Ч��״̬
		virtual void  setState(int state) {};

		///
		/// �õ���Ч����Ծ���(������actor����Ч���þ���Ϊ�������)
		//virtual const float*  getLocationMatrix() {};

		/// 
		///������Ч����Ծ���(������actor����Ч���þ���Ϊ�������
		//virtual void  setLocationMatrix(const float* mat) {};

		/// 
		/// �õ�����Ч�İ�Χ��
		//virtual const float*  getAABB() {};

		/// 
		/// ����������Ч��͸����
		virtual void  setTransparence(float trans) {};

		///
		/// ������Ч�Ƿ�Ͷ����Ӱ
		virtual void setCastShadow(bool bCastShadow) {};

		///
		///��������Ч����Ч������
		virtual void  release() {};

		///
		///�ж���Ч�Ƿ���actor
		virtual bool  hasActor() {return 0;};

		///
		///�õ���Ч���ܳ��ȣ�����TIME_MAX��ʾѭ����Ч
		virtual float  getAnimationLength() {return 0;};

		///
		///������Ч��Ĭ�ϸ���ʱ��Ϊ���ʱ��
		virtual void  update(float interval, bool isinterval = true) {};

		///
		///��Ⱦ��Ч
		virtual void  render() {};
		virtual void  NewRender(int pipeType) {};
		/// 
		/// ������Ч״̬
		virtual void  reset() {};

		/// ��õ�ǰ��Ч�и��ִ�IMoveObject�̳е��ӽڵ�
		virtual void  getMoveObjectArray(H3DI::IMoveObject** pLightArray, 
			H3DI::IMoveObject** pSoundArray, int* pLightSize, int* pSoundSize) {};

		// ��ʼ����ǰ��Ч
		virtual bool  Initialize() {return true;};

		// ����3���������ϵ��
		virtual void _stdcall setScaleInfo(float x, float y, float z) {};



		//////////////////////////////////////////////////////////////////////////
		////////////////   ���½ӿ���Ҫ���༭��ʹ��   ////////////////////////////
		////////////////   ����ʱ������ã��������� ////////////////////////////

		///
		///������Ч���ļ�(��Ҫ���༭��ʹ��,����ʱ������ã���������)
		virtual bool  save(const char* filename) {return 0;};

		///
		///�õ�������Ч�ڵ������(��Ҫ���༭��ʹ��,����ʱ������ã���������)
		virtual IForest*  getForest() {return 0;};

		/// �õ���ǰ��Ч�ӽڵ����Ŀ
		virtual int  getForestChildCount() {return 0;};

		///
		///�õ���ЧԪ�ع�����(��Ҫ���༭��ʹ��,����ʱ������ã���������)
		//hide this interface

		///
		///����һ����Ч��������actor��Ϣ���������Խ�����Ч���Ʋ���������actor
		virtual ISpecialEffect*  clone(bool soundSample = true) {return 0;};

		///
		///ɾ����Ч���ͷ��ڴ�ռ�(��Ҫ���༭��ʹ��,����ʱ������ã���������)
		virtual void  destroy() {};

		/// for editor only, add a shader
		virtual bool  addShader(const char* shaderName) {return 0;};

		/// 
		/// for editor only
		//virtual IAnimationTrack*  createAnimationTrack() {};

		/// ����һ���ƹ���Դ
		virtual ILightSource*  createLightSource() {return 0;};

		/** �ӱ���Ч�п�����Ӧ�Ľڵ㵽ָ������Ч
		@remark
		�����ڱ༭��Ӧ����
		@par
		pDstEffect: ָ����Ч��ָ��
		dstNodeAsParent: ָ����Ч�е�ĳ���ӽڵ������
		srcNodeName: ��ǰ��Ч��Ҫ�������ӽڵ������
		*/
		virtual const char*  copyNodeTo(ISpecialEffect* pDstEffect, const char* dstNodeAsParent, const char* srcNodeName) {return "";};

		/// ��ȡ��ǰ�򿪵�����shaderlib����Ŀ
		virtual int  getShaderCount() {return 0;};
		/// ����index��ȡshaderlib���ļ�����
		virtual const char*  getShaderURL(int index) {return "";};

		/////////////////////add in 7.28 for IModel
		/// ��ȡ��ǰ��Ч�е�ģ��ָ��
		virtual int  getIModelArray(H3DI::IModel** modelArray, int max_count) {return 0;};
		/// ������Ч�е����ӽڵ��Ƿ�����µ�����
		virtual void  ParticleCreateNewOne(bool create) {};
		/// ��̬�ı�ģ�͵�shader
		virtual void  changeWeaponShader(const char* shaderLibName) {};
		/// ǿ�����ò��������ӵĸ߶�
		virtual void  ForceParticleHeight(float height) {};
		/// ��ȡ��Ч������ģ�͵����ƣ��༭��
		virtual int  getModelName(char nameArray[NameArrayLength][NameArrayWidth]) {return 0;};
		/// ����Ϊ2�����ļ�
		virtual void  exportBinFile(const char* fileName) {};
		/// get the sound source array
		virtual int  getSoundSourceArray(H3DI::ISoundSource** soundArray, int maxCount) {return 0;};

		// for multi thread ��ѯ��Ч��Դ�Ƿ�loading���
		virtual bool  isLoadingEnd() {return true;};

		/// disable sound node
		virtual void  disableSoundSourceNode() {};

		/// ���÷����λ��
		virtual void  setDirectionPosition(const H3DVec3& dir, const H3DVec3& pos, const H3DVec3& scale) {};

		// ����ɾ��
		virtual void  clean() {};

		virtual void  cancelLoading() {};

		virtual void  setShaderLoadingLevel(int scale, int compress) {};

		virtual void  resetSkillEffect() {};

		virtual void  setEffectShader() {};
		//reload shader
		virtual void reloadShader() {};

		// ������Ч�����ӵ����ӵ������
		virtual void  setParticleLinkPoints(float* array, int count) {};

		// ������Ч�����ӵĴ�С
		virtual void  setParticleSize(float length, float width) {};

		///��ñ�����ʹ�õ��Դ�
		unsigned int GetVideoMemUse()
		{
			return 0;
		}
		///�����������
		void* GetPerfData(H3DI::MODEL_PERFDATA_TYPE t)
		{
			return 0;
		}

		///���ڴ�����һ������ʵ����ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		int SetMeshMaterialTemplateFromMemByName(const char* meshName, void* pXmlDoc)
		{
			return 1;
		}

		/*������
		0����ͬ��ReloadAll
		1��Reload Geometry
		2��Reload ���ʣ�������ͼ��
		3��Reload ��ͼ

		����ֵ-1Ϊʧ�ܣ�����0������ʾreload�˼�����ͼ
		*/
		int ReloadFromDisk(int reloadContent)
		{
			return 0;
		}

		/// ���ô���ЧΪ��ɫ�����Ч
		virtual void	SetIsActorEffect(bool b) {};

		//!Ԥ����fx
		virtual int PreGenFx(H3DI::IFxInfo* p){return 0;};

		//////////////////////////////////////////////////////////////////////////
		// ���úͻ�ȡ��Ч����ʱ�����
		// ����ЧUpdate��ʱ���������ʱ��t����Ϊt*ratio
		//! Ŀǰ����Ч�༭���༭ʱ���������ı�����������
		//! ͬʱ����Ч�༭���༭״̬�´˲������������ã�ֻ�ڿͻ���״̬�²�������
		virtual void SetSpeLengthRatio(float fRatio) {};
		virtual float GetSpeLengthRatio() {return 0;};
		//////////////////////////////////////////////////////////////////////////

	};
class SpecialEffectManager : public ISpecialEffectManager{
public:

	/// Ԥ�ȶ���xml�ļ�
	virtual void  getSpecialEffectXMLRead(const char* name) { };
	//////////////////////////////////////////////////////////////////////////
	/// �������ƻ��һ����Ч
	virtual ISpecialEffect*  getSpecialEffect(const char* name, bool binary = false) { return dynamic_cast<CSpecialEffect*>(EnginePool::m_effect_pool->createObj(name)); };
	virtual ISpecialEffect*  getSpecialEffect(H3DI::sCreateOp& op,const char* name, bool binary = false) { return dynamic_cast<CSpecialEffect*>(EnginePool::m_effect_pool->createObj(name)); };
	/// for multi thread loading
	virtual ISpecialEffect*  getSpecialEffectThread(const char* name, bool binary = false, int scale =0, int compress =0) {  return dynamic_cast<CSpecialEffect*>(EnginePool::m_effect_pool->createObj(name));};

	//////////////////////////////////////////////////////////////////////////
	/// �õ����ӵ�Ч���Ĺ�����
	virtual IParticleSystemManager*  getParticleManager() { return 0;};

	//////////////////////////////////////////////////////////////////////////
	/// get ISoundManager
	virtual ISoundManager*  getSoundManager() {return 0; };

	//////////////////////////////////////////////////////////////////////////
	/// ɾ��������Ч�������ã�
	virtual void  deleteAllSpecialEffects() { };

	/// ���õ���
	virtual void         setTerrain(H3DI::ITerrain*) { };

	/// ����������Դ������
	virtual void 		setSoundManager(ISoundManager* sound) { };
	/// �����ӽڵ��Ӧ�ĸ�����Դ���༭�����ã�
	virtual IEModel*  createEModel(int type, int createinfo, const char* url, const char* extrainfo =0) { return 0; };
	/// ����ʱ��Track
	virtual IAnimationTrack*  createNewAnimationTrack() {return 0; };
	/// ���������ƵĹ�����
	virtual H3DI::I_CameraManager*  getCameraManager() { return 0;};

	virtual H3DI::ILight*  getLightFirst() {return 0; };

	virtual H3DI::ILight*  getLightNext() {return 0; };

	virtual void  beginRenderParticle() { };

	virtual void  endRenderParticle() { };

	/// ������Ч�ڵ������Ƿ�������  true����������,false����������
	virtual void  setSoundEffectState(bool state) { };

	virtual bool  getSoundEffectState() { return false;};

	virtual void  beginOpenShaderLib(int scale =0, int compress =0) { };

	virtual bool  isOpenShaderLibOK() { return false;};

	virtual void  cancelLoadingShaderLib() { };
	//������Чû����ֲ���,������Ҫ�����ϰ汾�ı���ͼ���
	virtual void  setEditModel(bool b){};

	// �����༭��ʹ�õĶ�ȡ�ӿڣ��༭������Ҫ����
	virtual ISpecialEffect* getEffectForEditor(const char* name, bool binary = false) {return 0; };

	// ���༭����ȡ��Чʹ�õĲ�����Ϣ
	// ������Ч�ļ�����һ��ָ����count��Ԫ�ص������ָ��
	// ����ʵ�ʵ�Ԫ�ظ�������0��ʼ
	// �ռ��ɵ����߷���
	virtual int Editor_GetMatFromSPEFile(const char* filename, Editor_SPEMatInfo* matInfo, unsigned int count) { return 0;};


	//virtual ~SpecialEffectManager(){}	
};
}
#endif