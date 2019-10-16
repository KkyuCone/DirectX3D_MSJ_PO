#include "TitleScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Device.h"
#include "Scene/Layer.h"
#include "GameObject.h"
#include "Component/Renderer.h"
#include "Component/TPCamera.h"
#include "Component/Transform.h"
#include "Component/Material.h"
#include "Component/Particle.h"
#include "Component/AnimationFrame.h"
#include "Component/UIButton.h"
#include "Component/UIBar.h"
#include "Component/LandScape.h"
#include "Component/UIText.h"
#include "Component/AudioSound.h"
#include "../UserComponent/Player.h"
#include "../UserComponent/Monster.h"
#include "../UserComponent/Bullet.h"
#include "CollisionManager.h"
#include "Render/RenderManager.h"

#include "MainScene.h"
#include "MapToolScene.h"


TitleScene::TitleScene()
{
}


TitleScene::~TitleScene()
{
}

bool TitleScene::Init()
{
	Layer* pDefaultLayer = m_pScene->FindLayer("Default");
	Layer*	pUILayer = m_pScene->FindLayer("UI");

	// 타이틀 배경 이미지
	GameObject* pTitleBGObj = GameObject::CreateObject("TitleBG", pUILayer);
	UIBar*		pTitleBGImage = pTitleBGObj->AddComponent<UIBar>("TitleBG");
	pTitleBGImage->SetTexture("TitleBG", TEXT("TitleBG.png"), PATH_UI_TITLESCENE);
	pTitleBGImage->SetShader(SHADER_UI_ONLY_IMAGE);
	Transform* pTitleBGTr = pTitleBGObj->GetTransform();
	pTitleBGTr->SetWorldPosition(0.0f, 0.0f, 1.8f);
	pTitleBGTr->SetWorldScale(1980.0f, 1080.0f, 1.0f);
	SAFE_RELEASE(pTitleBGTr);


	// 타이틀 버튼
	GameObject*	pStartButtonObj = GameObject::CreateObject("StartButton", pUILayer);
	Transform*	pTr = pStartButtonObj->GetTransform();

	UIButton*	pStartButton = pStartButtonObj->AddComponent<UIButton>("StartButton");
	pStartButton->SetStateTexture(BS_NORMAL, "StartButton_NORMAL", TEXT("Title_Start_NORMAL.png"), PATH_UI_TITLESCENE);
	pStartButton->SetStateTexture(BS_MOUSEON, "StartButton_MOUSEON", TEXT("Title_Start_MOUSEON.png"), PATH_UI_TITLESCENE);
	pStartButton->SetStateTexture(BS_CLICK, "StartButton_CLICK", TEXT("Title_Start_CLICK.png"), PATH_UI_TITLESCENE);
	//pStartButton->SetStateTexture(BS_NORMAL, "StartButton", TEXT("GageBar_I3.tga"), PATH_UI_GAGEBAR);
	pStartButton->SetPushNoneOnlyMouseON(true);
	pStartButton->SetCallBackFunc(BS_CLICK, this, &TitleScene::SceneChangeMainScene);

	SAFE_RELEASE(pStartButton);
	SAFE_RELEASE(pStartButtonObj);

	pTr->SetWorldPosition(_RESOLUTION.iWidth / 2.f - 600.0f, _RESOLUTION.iHeight / 2.f - 400.0f, 1.f);
	pTr->SetWorldScale(Vector3(356.0f, 140.f, 1.0f));
	SAFE_RELEASE(pTr);

	// 툴씬
	GameObject*	pMapToolObj = GameObject::CreateObject("MapToolButton", pUILayer);

	UIButton*	pMapToolButton = pMapToolObj->AddComponent<UIButton>("MapToolButton");
	pMapToolButton->SetStateTexture(BS_NORMAL, "MapToolButton_NORMAL", TEXT("Title_MapTool_NORMAL.png"), PATH_UI_TITLESCENE);
	pMapToolButton->SetStateTexture(BS_MOUSEON, "MapToolButton_MOUSEON", TEXT("Title_MapTool_MOUSEON.png"), PATH_UI_TITLESCENE);
	pMapToolButton->SetStateTexture(BS_CLICK, "MapToolButton_CLICK", TEXT("Title_MapTool_CLICK.png"), PATH_UI_TITLESCENE);
	pMapToolButton->SetPushNoneOnlyMouseON(true);
	pMapToolButton->SetCallBackFunc(BS_CLICK, this, &TitleScene::SceneChangeMapToolScene);

	Transform*	pMapToolTr = pMapToolObj->GetTransform();
	pMapToolTr->SetWorldPosition(_RESOLUTION.iWidth / 2.f - 150.0f, _RESOLUTION.iHeight / 2.f - 400.0f, 1.f);
	pMapToolTr->SetWorldScale(356.0f, 140.f, 1.0f);
	SAFE_RELEASE(pMapToolTr);

	SAFE_RELEASE(pMapToolButton);
	SAFE_RELEASE(pMapToolObj);

	// 나가기 버튼
	GameObject* pExitObj = GameObject::CreateObject("ExitButton", pUILayer);
	UIButton*	pExitButton = pExitObj->AddComponent<UIButton>("ExitButton");
	pExitButton->SetStateTexture(BS_NORMAL, "ExitButton_NORMAL", TEXT("Title_Exit_NORMAL.png"), PATH_UI_TITLESCENE);
	pExitButton->SetStateTexture(BS_MOUSEON, "ExitButton_MOUSEON", TEXT("Title_Exit_MOUSEON.png"), PATH_UI_TITLESCENE);
	pExitButton->SetStateTexture(BS_CLICK, "ExitButton_CLICK", TEXT("Title_Exit_CLICK.png"), PATH_UI_TITLESCENE);
	pExitButton->SetPushNoneOnlyMouseON(true);
	pExitButton->SetCallBackFunc(BS_CLICK, this, &TitleScene::ExitScene);

	Transform*	pExitTr = pExitObj->GetTransform();
	pExitTr->SetWorldPosition(_RESOLUTION.iWidth / 2.f + 300.0f, _RESOLUTION.iHeight / 2.f - 400.0f, 1.f);
	pExitTr->SetWorldScale(356.0f, 140.f, 1.0f);
	SAFE_RELEASE(pExitTr);

	SAFE_RELEASE(pExitButton);
	SAFE_RELEASE(pExitObj);

	// 텍스트
	GameObject* pTextObject = GameObject::CreateObject("Text", pUILayer);
	Transform* pTextTransform = pTextObject->GetTransform();
	pTextTransform->SetWorldPosition(1500.0f, 350.0f, 1.0f);

	SAFE_RELEASE(pTextTransform);

	UIText* pText = pTextObject->AddComponent<UIText>("Text");

	pText->SetText(TEXT("무보수 용사님은 짠내나.."));
	pText->Shadow(true);
	pText->SetShadowColor(1.0f, 0.0f, 1.0f, 1.0f);
	pText->SetShadowOffset(Vector3(2.f, -2.f, 0.f));
	pText->SetShadowOpacity(1.f);
	pText->SetFont("Gungseo");

	pText->SetColor(1.f, 1.f, 0.f, 1.f);
	pText->SetOpacity(0.5f);
	pText->AlphaBlend(true);
	pText->SetRenderArea(0.f, 0.f, 300.f, 50.f);

	SAFE_RELEASE(pText);
	SAFE_RELEASE(pTextObject);

	// 배경음
	GameObject* pBGMObject = GameObject::CreateObject("BGM", pDefaultLayer);
	AudioSound* pBGM = pBGMObject->AddComponent<AudioSound>("BGM");
	pBGM->SetSound("TitleBGM", m_pScene, true, "Piano_Only_version.wav", PATH_SOUND_BGM);
	pBGM->SetVolume(0.2f);
	pBGM->Play();

	SAFE_RELEASE(pBGM);
	SAFE_RELEASE(pBGMObject);

	return true;
}

void TitleScene::CreatePrototype()
{
}

void TitleScene::SceneChangeMainScene(float _fTime)
{
	Layer* pDefaultLayer = m_pScene->FindLayer("Default");
	GameObject* pEffectMObject = GameObject::CreateObject("BGM", pDefaultLayer);
	AudioSound* pEffectSound = pEffectMObject->AddComponent<AudioSound>("BGM");
	pEffectSound->SetSound("TitleBtClick", m_pScene, false, "Button-4.mp3", PATH_SOUND_UI);
	pEffectSound->Play();

	SAFE_RELEASE(pEffectSound);
	SAFE_RELEASE(pEffectMObject);

	Scene* pScene = GET_SINGLETON(SceneManager)->CreateScene<MainScene>();
	GET_SINGLETON(SceneManager)->SetNextScene(pScene);
	GET_SINGLETON(RenderManager)->SetPostEffectEnable(true);
	GET_SINGLETON(RenderManager)->SetShadowCompute(true);
	GET_SINGLETON(RenderManager)->SetSSAOEnable(false);
	GET_SINGLETON(RenderManager)->SetNavMeshRender(false);
}

void TitleScene::SceneChangeMapToolScene(float _fTime)
{
	Layer* pDefaultLayer = m_pScene->FindLayer("Default");
	GameObject* pEffectMObject = GameObject::CreateObject("BGM", pDefaultLayer);
	AudioSound* pEffectSound = pEffectMObject->AddComponent<AudioSound>("BGM");
	pEffectSound->SetSound("TitleBtClick", m_pScene, false, "Button-4.mp3", PATH_SOUND_UI);
	pEffectSound->Play();

	SAFE_RELEASE(pEffectSound);
	SAFE_RELEASE(pEffectMObject);

	Scene* pScene = GET_SINGLETON(SceneManager)->CreateScene<MapToolScene>();
	GET_SINGLETON(SceneManager)->SetNextScene(pScene);
	GET_SINGLETON(RenderManager)->SetPostEffectEnable(true);
	GET_SINGLETON(RenderManager)->SetShadowCompute(true);
	GET_SINGLETON(RenderManager)->SetSSAOEnable(false);
	GET_SINGLETON(RenderManager)->SetNavMeshRender(true);
}

void TitleScene::ExitScene(float _fTime)
{
	Layer* pDefaultLayer = m_pScene->FindLayer("Default");
	GameObject* pEffectMObject = GameObject::CreateObject("BGM", pDefaultLayer);
	AudioSound* pEffectSound = pEffectMObject->AddComponent<AudioSound>("BGM");
	pEffectSound->SetSound("TitleBtClick", m_pScene, false, "Button-4.mp3", PATH_SOUND_UI);
	pEffectSound->Play();

	SAFE_RELEASE(pEffectSound);
	SAFE_RELEASE(pEffectMObject);
}
