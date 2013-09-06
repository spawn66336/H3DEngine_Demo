#ifndef LEVELSHELL
#define LEVELSHELL

#include "instance_pool.h"

namespace H3DI
{
	class  CLevel: public ILevel
	{
	public:
		//OBJECT
		virtual void					Release()
		{
			EnginePool::m_level_pool->releaseObj(this);
		};

		virtual bool					IsValid()const{return true;};

		virtual void                    SetUserPtr(void*){};
		virtual void*                  GetUserPtr(void)const {return 0;};
		//MOVE OBJECT
		virtual MODEL_TYPE				Type(){return MODEL_STATIC;};
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
		virtual int						ReloadFromDisk(int reloadContent){return 0;};
		///��ñ�����ʹ�õ��Դ�
		virtual unsigned int GetVideoMemUse(){return 0;};

		///�����������
		virtual void* GetPerfData(MODEL_PERFDATA_TYPE t){return 0;};

		///���ڴ�����һ������ʵ����ģ�͵�һ�����񣬷���1��ʾ�ɹ���0��ʾ�Ҳ���������
		virtual int SetMeshMaterialTemplateFromMemByName(const char* meshName,void* pXmlDoc){return 1;};

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

		//SCENE
		//��һ��������볡����
		virtual void					AttachModel(IMoveObject* pObj,int nLayer){};

		//��һ������ӳ���ɾ��
		virtual void					DetachModel(IMoveObject* pObj){};

		//�������ƶ�ʱ������ʹ�����ڳ������ƶ��� ֻ��CLIENT֪��MODEL�Ƿ��ƶ���
		//������û���ƶ�ʱ����Ҫ���ô˺�����
		virtual void					MoveModel(IMoveObject* pObj){};	

		//!!!!!! ISceneSearch must be Released by CLIENT !!!!!!
		//use aabb to search
		//ʹ��AABB����
		virtual ISceneSearch*			FindObjects(const float* pAABB,const int* pLayers,int numLayer){return 0;};
		//ʹ��AABB��X��Yƽ����������������Z��Χ
		virtual ISceneSearch*			FindObjects2D(const float* pAABB,const int* pLayers,int numLayer){return 0;};
		//use IRender's camera to search
		virtual ISceneSearch*			FindObjects(IRender* pRenderer,const int* pLayers,int numLayer){return 0;};
		//��׶�����������ڸ���RENDER TO IMAGE����Ч�У���SPOT LIGHTING����Ҫ
		//virtual ISceneSearch			FindObjects(tClipPlanes[5],const int* pLayers,int numLayer){}


		//����MATRIX�õ�CAMERA��Ϣ��δʵ��03-12-3)
		virtual void					SetCurrentCamera(const float* pViewMatrix,const float* pProjectMatrix){};
		//����λ�úͽǶȵõ�CAMERA��Ϣ��δʵ��03-12-3)
		virtual void					SetCurrentCamera(const float* vecEyePos,float fov,float ratio,float near, float far){};
		//����IRENDER�е�ǰ��CAMERA�����ó���CULLING CAMERA���Ƽ�ʹ��
		virtual void					SetCurrentCamera(IRender* pRenderer){};
		//LEVEL
		//!�õ�AABB
		virtual void GetWorldAABB(float* pArray6)const {  };
		//!���û�������ɫ
		virtual void SetAmbientColor(float r, float g, float b) {  };
		//!��geometry
		virtual bool Load(const char* fileName) { return true; };
		//!�����culling
		virtual void CullByCamera(ICamera*) {  };

		//by sssa2000 081203
		//��õ�����Ϣ
		virtual unsigned int GetOctreeAABBTestNum(){return 0;};
		virtual unsigned int GetSubMeshOctreeSectionTestNum(){return 0;};

		//by sssa2000 090216
		//Condition1��Condition2�ܹ����������������2�������Ľ���Χ��
		virtual void SetDebugCondition1(LEVEL_DEBUG_CONDITION ldc){};
		virtual void SetDebugCondition2(LEVEL_DEBUG_CONDITION ldc){};
		virtual void SetDebugCalc(LEVEL_DEBUG_SET_CALC ldc){};
		virtual void SetDrawOctreeLevel(int i){}; //���û���octree�İ�Χ�еĲ㼶��Ϊ-1ʱ�������в㼶
		//by sssa2000 090209
		virtual void EnableOccluder(bool b){};
		virtual bool IsOccluderEnable(){return 0;};
		virtual int GetOccluderNumber(){return 0;};
		virtual void DrawOccluder(int i){};
		virtual void DrawOccluderFrustum(int i,H3DVec3& viewPoint){};
		//���ݳ�Ӱ��ϸ���������·ָ�˲���
		virtual void RestructOctree(){};
		//��һ����������Ӻ���
		virtual void			AddPostProcess(const char* filename){};
		//��һ��ɾ������
		virtual void			DelPostProcess(const char* filename){};
		//����һ�������ĺ������
		virtual void			SetPostProcessParam(const char *name,const void *pvalue,int sizeOfValue){};
		//����һ���������ӿ�
		virtual	void			SetViewPort(const unsigned short *vp) {  };
		//�õ�һ���������ӿ�,����һ���ĸ�Ԫ�ص�����
		virtual	void			GetViewPort(unsigned short *vp) {  };
		//�����Ӿ���
		virtual void			SetViewMatrix(const H3DMat4 &mat) {  };
		//����͸�Ӿ���
		virtual void			SetPerspectiveMatrix(const H3DMat4 &mat) {  };

		//�Ӿ���
		virtual void					GetViewMatrix(H3DMat4 &mat) {  };
		//͸�Ӿ���
		virtual void					GetPerspectiveMatrix(H3DMat4 &mat) {  };
		virtual void					SetEyePosition(const H3DVec3& pos) {  };
		virtual void					GetEyePosition(H3DVec3& pos) {  };
		virtual void					LookAt(const H3DVec3& eyepos, const H3DVec3& at, const H3DVec3& up) {  };

		virtual void					RotateDirection(float yaw_offset,float pitch_offset,float roll_offset){};
		virtual void					SetCameraRotation(float yaw,float pitch,float roll){};
		virtual void					GetRotateDir(float& yaw,float& pitch,float& roll){};

		virtual void					GetEyeDirection(H3DVec3& vec) {  };
		virtual void					SetFrustum(float fov,float ratio,float fNear,float fFar,ECameraType camType = ECT_Render){};
		virtual void					GetFrustum(float& fov,float& ratio,float& fNear,float& fFar,ECameraType camType = ECT_Render){};
		virtual void					UpdateCamera(){};

		/// ���ó�����������ɫ
		virtual void					SetAmbientColor(const float *col){};
		/// ���ý�ɫ��������ɫ			
		virtual void					SetActorAmbientColor(const float *col){};
		//���ôα���ɢ����ɫ
		virtual void					SetTransmissionColor(const float* col) {  };
		/// ���ó�������LOD
		//virtual void					SetMaterialLod(int nLod){};
		/// �õ�����LOD			
		//virtual int						GetMaterialLod(){return 0;};

		virtual void					SetShadowFadeoutEnd(float fParam) {  };

		// Ӱ��lightmap��ɫ lm' = lm * a + b
		// a, b��Ϊfloat3
		virtual void				SetLightmapColorFactor(const float* a, const float* b) {  };


		virtual ~CLevel(){}
	};
}
#endif