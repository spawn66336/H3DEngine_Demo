#ifndef __H_MEDIA_PROPERTY_TYPE_H__
#define __H_MEDIA_PROPERTY_TYPE_H__

enum MEDIA_PROPERTY_TYPE
{
	MEDIA_PROPERTY_TYPE_INVALID = -1,
	MEDIA_PROPERTY_TYPE_VISIBLE,
	MEDIA_PROPERTY_TYPE_LOCATION,
	MEDIA_PROPERTY_TYPE_POSITION,
	MEDIA_PROPERTY_TYPE_PATH_POSITION,
	MEDIA_PROPERTY_TYPE_ROTATION,
	MEDIA_PROPERTY_TYPE_SCALE,
	MEDIA_PROPERTY_TYPE_RESOURCE,
	MEDIA_PROPERTY_TYPE_CAMERA_FRUSTUM,
	MEDIA_PROPERTY_TYPE_CAMERA_FOV,
	MEDIA_PROPERTY_TYPE_CAMERA_RATIO,
	MEDIA_PROPERTY_TYPE_CAMERA_NEAR_PLANE,
	MEDIA_PROPERTY_TYPE_CAMERA_FAR_PLANE,
	MEDIA_PROPERTY_TYPE_CAMERA_FOCAL_PLANE,
	MEDIA_PROPERTY_TYPE_CAMERA_DOF_RANGE,
	MEDIA_PROPERTY_TYPE_CAMERA_CONFUSION,
	MEDIA_PROPERTY_TYPE_CAMERA_LOOKAT,
	MEDIA_PROPERTY_TYPE_CAMERA_UP,
	MEDIA_PROPERTY_TYPE_CAMERA_FOV_AUTO_CHANGE,
	MEDIA_PROPERTY_TYPE_CAMERA_ADJUST_TYPE,
	MEDIA_PROPERTY_TYPE_CAMERA_TARGET_TYPE,
	MEDIA_PROPERTY_TYPE_CAMERA_TARGETS,
	MEDIA_PROPERTY_TYPE_CAMERA_POST_PROCESS,
	MEDIA_PROPERTY_TYPE_CAMERA_POSITION,
	MEDIA_PROPERTY_TYPE_ENABLE_MODEL_SHADOW,
	MEDIA_PROPERTY_TYPE_HIGH_LIGHT_MODEL,
	MEDIA_PROPERTY_TYPE_SHADER_VAR,
	MEDIA_PROPERTY_TYPE_SHADER_WAVEFORM,
	MEDIA_PROPERTY_TYPE_LIGHT_ENABLE,
	MEDIA_PROPERTY_TYPE_LIGHT_COLOR,
	MEDIA_PROPERTY_TYPE_LIGHT_INTENSITY,
	MEDIA_PROPERTY_TYPE_ENABLE_LIGHT_SHADOW,
	MEDIA_PROPERTY_TYPE_LIGHT_SHADOW_COLOR,
	MEDIA_PROPERTY_TYPE_LIGHT_NEAR_RANGE,
	MEDIA_PROPERTY_TYPE_LIGHT_FAR_RANGE,
	MEDIA_PROPERTY_TYPE_LIGHT_AFFECT_PARAM,
	MEDIA_PROPERTY_TYPE_LIGHT_GLARE_WIDTH,
	MEDIA_PROPERTY_TYPE_LIGHT_GLARE_HEIGHT,
	MEDIA_PROPERTY_TYPE_LIGHT_GLARE_COLOR,
	MEDIA_PROPERTY_TYPE_LIGHT_GLARE_TEXTURE_NAME,
	MEDIA_PROPERTY_TYPE_LIGHT_GLARE_SHADER_LIB,
	MEDIA_PROPERTY_TYPE_LIGHT_GLARE_SHADER_NAME,
	MEDIA_PROPERTY_TYPE_LIGHT_GLARE_DISTANCE,
	MEDIA_PROPERTY_TYPE_LIGHT_GLARE_ROTATION_VELOCITY,
	MEDIA_PROPERTY_TYPE_LIGHT_GLARE_VISIBLE,
	MEDIA_PROPERTY_TYPE_LIGHT_GLARE_ROTATE_WITH_CAMERA,
	MEDIA_PROPERTY_TYPE_LIGHT_GLARE_VIEW_CAMERA,
	MEDIA_PROPERTY_TYPE_DRAW_LIGHT_BEAM_LINE,
	MEDIA_PROPERTY_TYPE_SET_LIGHT_PRIORITY,
	MEDIA_PROPERTY_TYPE_LIGHT_SHAFT_ENABLE,
	MEDIA_PROPERTY_TYPE_LIGHT_SHAFT_COLOR,
	MEDIA_PROPERTY_TYPE_LIGHT_SHAFT_INTENSITY,
	MEDIA_PROPERTY_TYPE_LIGHT_SHAFT_THRESHOLD,
	MEDIA_PROPERTY_TYPE_LIGHT_SHAFT_DEPTH,
	MEDIA_PROPERTY_TYPE_LIGHT_INNER_ANGLE,
	MEDIA_PROPERTY_TYPE_LIGHT_OUTTER_ANGLE,
	MEDIA_PROPERTY_TYPE_LIGHT_ENABLE_PROJ_TEXTURE,
	MEDIA_PROPERTY_TYPE_LIGHT_PROJ_TEXTURE,
	MEDIA_PROPERTY_TYPE_LIGHT_BEAM_VISIBLE,
	MEDIA_PROPERTY_TYPE_LIGHT_BEAM_BRIGHTNESS,
	MEDIA_PROPERTY_TYPE_LIGHT_BEAM_NEAR_RANGE,
	MEDIA_PROPERTY_TYPE_LIGHT_BEAM_FAR_RANGE,
	MEDIA_PROPERTY_TYPE_LIGHT_BEAM_INNER_ANGLE,
	MEDIA_PROPERTY_TYPE_LIGHT_BEAM_OUTTER_ANGLE,
	MEDIA_PROPERTY_TYPE_LIGHT_BEAM_TYPE,
	MEDIA_PROPERTY_TYPE_FAKE_LIGHT_SPOT_ENABLE,
	MEDIA_PROPERTY_TYPE_EFFECT_TRANSPARENCE,
	MEDIA_PROPERTY_TYPE_PLAY,
	MEDIA_PROPERTY_TYPE_SYSTEM_AMBIENT_COLOR,
	MEDIA_PROPERTY_TYPE_SYSTEM_AMBIENT_ACTOR_COLOR,
	MEDIA_PROPERTY_TYPE_COLOR_FACTOR_A,
	MEDIA_PROPERTY_TYPE_COLOR_FACTOR_B,
	MEDIA_PROPERTY_TYPE_COLOR_FACTOR_A_INTENSITY,
	MEDIA_PROPERTY_TYPE_COLOR_FACTOR_B_INTENSITY,
	MEDIA_PROPERTY_TYPE_LIGHT_BEAM_INNER_RADIUS,
	MEDIA_PROPERTY_TYPE_LIGHT_POST_BEAM_BLUR_COEFF,
	MEDIA_PROPERTY_TYPE_IS_MALE,
	MEDIA_PROPERTY_TYPE_BODY_PART,
	MEDIA_PROPERTY_TYPE_ACTION,
	MEDIA_PROPERTY_TYPE_ACTION_LOOP,
	MEDIA_PROPERTY_TYPE_ACTION_LIST,
	MEDIA_PROPERTY_TYPE_ZROTATION,
	MEDIA_PROPERTY_TYPE_RECEIVE_FAKE_LIGHT_SPOT,
	MEDIA_PROPERTY_TYPE_LIGHT_SPECULAR_INTENSITY,
	MEDIA_PROPERTY_TYPE_SOUND_PLAY,
	MEDIA_PROPERTY_TYPE_SOUND_RESOURCE,
	MEDIA_PROPERTY_TYPE_LIGHTMAP_SHADOW_INDEX,
	MEDIA_PROPERTY_TYPE_LIGHTMAP_RADIUS,
	MEDIA_PROPERTY_TYPE_FOLLOW_LIGHT_TYPE,
	MEDIA_PROPERTY_TYPE_FOLLOW_LIGHT_TARGET_INFO,
	MEDIA_PROPERTY_TYPE_TRANSITION_TIME_FROM_CURRENT,
	MEDIA_PROPERTY_TYPE_TRANSITION_TIME_TO_NEXT,


	MEDIA_PROPERTY_TYPE_COUNT,
};


#endif