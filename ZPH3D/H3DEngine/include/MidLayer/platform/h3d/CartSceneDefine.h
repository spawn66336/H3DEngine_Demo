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
		int stage;   //级别，从1开始
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
		PLAYER_ACT_NORMAL = 0,                                       //默认
		PLAYER_ACT_HEAD_LOW,                                  //撞墙,落地
		PLAYER_ACT_HEAD_LEFT,                                  //左面撞墙
		PLAYER_ACT_HEAD_RIGHT,                               //右面撞墙
		PLAYER_ACT_TURN_LEFT,                                 //左转弯,向左甩尾
		PLAYER_ACT_TURN_RIGHT,                               //右转弯,向右甩尾
		PLAYER_ACT_ACCELERATE,                              //加速
		PLAYER_ACT_BACK_LEFT,                                  //向左后退
		PLAYER_ACT_BACK_RIGHT,                               //向右后退,直线后退
		PLAYER_ACT_LOOK_BACK_LEFT,                     //左面有人超车
		PLAYER_ACT_LOOK_BACK_RIGHT,                   //右面有人超车
		PLAYER_ACT_RIGHT_HAND_UP,                        //使用道具成功
		PLAYER_ACT_CRY_GAME,                                   //使用道具失败, 比赛失败
		PLAYER_ACT_BACK_UP,                                      //水泡中
		PLAYER_ACT_CLICKED,                                       //比赛胜利或者完成比赛,领奖,试开车辆
		PLAYER_ACT_MOUSEOFF,                                 //未激活
		PLAYER_ACT_MOUSEON,                                   //激活
		PLAYER_ACT_ADMIRE,                                        //仰慕
		PLAYER_ACT_CRY_ROOM,                                 //痛哭
		PLAYER_ACT_ANGRY,                                          //愤怒
		PLAYER_ACT_FAINT,                                            //晕倒
		PLAYER_ACT_SHY,                                               //害羞
		PLAYER_ACT_SLEEP,                                           //瞌睡
		PLAYER_ACT_HAPPY,                                          //开心
		PLAYER_ACT_SPEAK,                                          //说话
		PLAYER_ACT_BLINK,                                            //眨眼, switch texture
		PLAYER_ACT_GRIEVE,                                        //伤心, switch texture
		PLAYER_ACT_STAND,                                         //站立，商店中
		PLAYER_ACT_S_CLICKED,                                         //站立，商店中
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