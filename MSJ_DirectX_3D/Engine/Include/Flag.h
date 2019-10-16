#pragma once
#include "Macro.h"

ENGINE_BEGIN

// �� ü������ 
enum SCENE_CHANGE
{
	SC_NONE,
	SC_NEXT				// ������
};

// ���̴� Ÿ�� ( ���ؽ� / �ȼ� )
enum SHADER_TYPE
{
	ST_VERTEX = 0,
	ST_PIXEL,
	ST_GEOMETRY,
	ST_COMPUTE,
	ST_END
};

// ������Ʈ Ÿ��
enum COMPONENT_TYPE
{
	CT_TRANSFORM = 0,
	CT_RENDERER,
	CT_USERCOMPONENT,				// ����������Ʈ
	CT_CAMERA,						// �Ϲ� ī�޶�
	CT_OPCAMERA,					// 1��Ī ī�޶�
	CT_TPCAMERA,					// 3��Ī ī�޶�
	CT_MATERIAL,
	CT_COLLIDER,
	CT_LIGHT,
	CT_UI,
	CT_LANDSCAPE,					// ǳ�� (�����뵵�� ����)
	CT_ANIMATION,
	CT_ANIMATIONFRAME,				// ������ �ִϸ��̼� -> 2D�ִϸ��̼�
	CT_PARTICLE,
	CT_NAVIGATION,					// �׺���̼�
	CT_SOUND,
	CT_TEXT
};

// ��
enum AXIS
{
	AXIS_X = 0,
	AXIS_Y,
	AXIS_Z,
	AXIS_END
};

// 
enum CONSTANT_SHADER
{
	CS_VERTEX = 0x1,
	CS_PIXEL = 0x2,
	CS_GEOMETRY = 0x4,
	CS_COMPUTE = 0x8,
};

// Ű����
enum KEY_STATE
{
	KS_PRESS = 0,
	KS_PUSH,
	KS_RELEASE,
	KS_END
};

// Ű ���� (�������� ��Ʈ�� �Ǵ�)
enum KEY_STATE_FALG
{
	KSF_PRESS = 0x1,
	KSF_PUSH = 0x2,
	KSF_RELEASE = 0x4
};

// ����Ű(���Ű : ��Ʈ�� ����Ʈ ��Ʈ )
enum FUNCTION_KEY
{
	FK_CTRL = 0,
	FK_ALT,
	FK_SHIFT,
	FK_END
};

enum FUNCTION_KEY_ENABLE
{
	FKE_CTRL = 0x1,
	FKE_ALT = 0x2,
	FKE_SHIFT = 0x4
};


// LOOKAT ��,  ALL�� �������/ �� ����� �����ؼ� ����°���
enum LOOKAT_AXIS
{
	LOOK_X = 0,
	LOOK_Y,
	LOOK_Z,
	LOOK_ALL,
	LOOK_END
};

// ī�޶�Ÿ��
enum CAMERA_TYPE
{
	CAM_PERSPECTIVE = 0,			// ��������
	CAM_ORTHOGONAL,					// ��������
	CAM_ORTHOGONALMAINCAM,			// ���������ε�, �Ϲݸ��Ͱ��� HP�ٿ�
	CAM_END
};

// ���� ���� Ÿ�� ( �����Ͷ�����, ����, ���� )
enum RENDER_STATE
{
	RS_RASTERIZER,
	RS_DEPTH,
	RS_BLEND,
	RS_END
};

// ����ƮŸ��
enum LIGHT_TYPE
{
	LT_DIR = 0,					// Directional Light (������ ����)
	LT_POINT,				// Point Light
	LT_SPOT,				// Spot Light
	LT_END
};

// ���� �׷� ( ���� �׷캰�� ���������ٰ��� ������ )
enum RENDER_GROUP
{
	RG_LANDSCAPE = 0,
	RG_STATIC,
	RG_DEFAULT,
	RG_ALPHA,
	RG_HUD,					// Head up display
	RG_UI,
	RG_END
};

// ������ ��� ( ������, ���۵�), �⺻���� ���۵�
enum RENDER_MODE
{
	RM_FORWARD = 0,
	RM_DEFERRED
};

// �ִϸ��̼� �ɼ� ( loop or �ѹ��� ���� ������ ..)
enum ANIMATION_OPTION
{
	AO_LOOP,
	AO_ONCE_DESTROY
};

// �ݶ��̴� Ÿ�� (��, ĸ��, OBB �� )
enum COLLIDER_TYPE
{
	COL_SPHERE,
	COL_CAPSULE,
	COL_OBB,
	COL_POINT,
	COL_RAY,
	COL_AABB,
	COL_LAND,
};

// �ݶ��̴� ä�� ���� -> �ݶ��̴��� ���¶�� ���� �ɵ�
enum COLLISION_CHANNEL_STATE
{
	CCS_IGNORE,			// ����
	CCS_OVERLAP,		// ��������
	CCS_BLOCK			// �ε�������
};

// �浹ü �ݹ��Լ��� ȣ���ϱ� ���� �浹���� ǥ�� -> ó���浹, �浹��, �浹��
// �浹���·� ���� �ɵ�
enum COLLISION_CALLBACK_STATE
{
	CCBS_ENTER,					// �浹 ����
	CCBS_STAY,					// �浹 ��
	CCBS_EXIT,					// �浹�� ������ ��
	CCBS_END
};

// �ִϸ��̼� ������ Ÿ��
enum ANIMATION_FRAME_TYPE
{
	AFT_ATLAS,				// ��Ʋ��
	AFT_FRAME,				// �����Ӻ���.. (���� �밡��ó�� ������)
	AFT_END
};


// ���콺 ��ư ��
enum MOUSE_BUTTON
{
	MB_NONE = -1,
	MB_LBUTTON,			// ���� ��ư
	MB_RBUTTON,			// ������ ��ư
	MB_MBUTTON,			// �����ư
	MB_END
};

ENGINE_END