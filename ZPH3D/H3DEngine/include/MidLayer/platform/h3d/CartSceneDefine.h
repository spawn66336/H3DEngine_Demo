#ifndef  _CART_SCENE_DEFINE_H_
#define _CART_SCENE_DEFINE_H_
#include <vector>

using namespace std;

namespace H3D
{
	/**
	a cart has 8 slots for suspenders, when id is -1, it means there's no suspender
	body color is 12 pre-defined spectrums
	*/
	struct GFX_CREATE_CART_INFO
	{
		int suspenderID[8]; 
		int bodyResID;
		int bodyColor;
		char licence[32];
		int stage;   //���𣬴�1��ʼ
		//float dim[3]; //dim of the physics kart
	};

	/**
	a player has 4 slots for suspenders, when id is -1, it means there's no suspender
	body color is 12 pre-defined spectrums
	*/
	struct GFX_CREATE_PLAYER_INFO
	{
		int suspenderID[4];
		int bodyResID;
		int  bodyColor;
	};

	struct GFX_CREATE_SCENE_INFO
	{
		int ResID;
	};

	struct GFX_CREATE_EFFECT_INFO
	{
		int ResID;
	};

	struct GFX_CREATE_ENTITY_INFO
	{
		int ResID;
	};

	struct GFX_CREATE_PET_INFO
	{
		int ResID;
	};

	enum CART_EFFECT
	{
		CART_EFT_ENGINE_SMOKE,
		CART_EFT_ENGINE_FLAME,
		CART_EFT_TRACK_SMOKE,
		CART_EFT_ACCELERATE
	};

	enum PLAYER_ACTION
	{
		PLAYER_ACT_NORMAL = 0,                                       //Ĭ��
		PLAYER_ACT_HEAD_LOW,                                  //ײǽ,���
		PLAYER_ACT_HEAD_LEFT,                                  //����ײǽ
		PLAYER_ACT_HEAD_RIGHT,                               //����ײǽ
		PLAYER_ACT_TURN_LEFT,                                 //��ת��,����˦β
		PLAYER_ACT_TURN_RIGHT,                               //��ת��,����˦β
		PLAYER_ACT_ACCELERATE,                              //����
		PLAYER_ACT_BACK_LEFT,                                  //�������
		PLAYER_ACT_BACK_RIGHT,                               //���Һ���,ֱ�ߺ���
		PLAYER_ACT_LOOK_BACK_LEFT,                     //�������˳���
		PLAYER_ACT_LOOK_BACK_RIGHT,                   //�������˳���
		PLAYER_ACT_RIGHT_HAND_UP,                        //ʹ�õ��߳ɹ�
		PLAYER_ACT_CRY_GAME,                                   //ʹ�õ���ʧ��, ����ʧ��
		PLAYER_ACT_BACK_UP,                                      //ˮ����
		PLAYER_ACT_CLICKED,                                       //����ʤ��������ɱ���,�콱,�Կ�����
		PLAYER_ACT_MOUSEOFF,                                 //δ����
		PLAYER_ACT_MOUSEON,                                   //����
		PLAYER_ACT_ADMIRE,                                        //��Ľ
		PLAYER_ACT_CRY_ROOM,                                 //ʹ��
		PLAYER_ACT_ANGRY,                                          //��ŭ
		PLAYER_ACT_FAINT,                                            //�ε�
		PLAYER_ACT_SHY,                                               //����
		PLAYER_ACT_SLEEP,                                           //�˯
		PLAYER_ACT_HAPPY,                                          //����
		PLAYER_ACT_SPEAK,                                          //˵��
		PLAYER_ACT_BLINK,                                            //գ��, switch texture
		PLAYER_ACT_GRIEVE,                                        //����, switch texture
		PLAYER_ACT_STAND,                                         //վ�����̵���
		PLAYER_ACT_S_CLICKED,                                         //վ�����̵���
		PLAYER_ACT_UNKNOWN
	};

	//==============================================================

	enum PET_ACTION
	{
		PET_ACT_NORMAL = 0,
		PET_ACT_CRY,
		PET_ACT_WIN,
		PET_ACT_LOSE,
		PET_ACT_TURN_LEFT,
		PET_ACT_TURN_RIGHT,
		PET_ACT_MOUSEON,
		PET_ACT_JUMP,
		PET_ACT_HAPPY,
		PET_ACT_UNKNOWN
	};

#define CART_PARTS_COUNT 19
	enum CART_PARTS
	{
		CART_BODY = 0,
		CART_WHEEL_FL,
		CART_WHEEL_FR,
		CART_WHEEL_RL,
		CART_WHEEL_RR,
		CART_LICENCE,
		CART_LIGHT_FRONT,
		CART_LIGHT_LEFT,
		CART_LIGHT_RIGHT,
		CART_LIGHT_STOP,
		CART_BOARD,
		CART_SW,
		CART_EXHAUST1,
		CART_EXHAUST2,
		CART_EXHAUST3,
		CART_EXHAUST4,
		CART_EXHAUST5,
		CART_SEAT,
		CART_UNKNOWN
	};

#define MAX_SUSPENDER_COUNT 8



	struct  FindTriangleRequest
	{
		float pos[3];
		float dir[3];
		float triangleVert[3][3];
		float intersectPoint[3];
		int ID;
	};

	typedef vector<FindTriangleRequest*> FindTriangleRequestBuffer;

}
#endif