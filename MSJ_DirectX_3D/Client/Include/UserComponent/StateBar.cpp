#include "StateBar.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Device.h"
#include "Scene/Layer.h"
#include "GameObject.h"
#include "Component/Transform.h"
#include "Component/Material.h"

StateBar::StateBar() :
	m_fRatio(1.0f),
	m_StateBar(nullptr),
	m_StateText(nullptr)
{
	m_State.m_iCur = 0;
	m_State.m_iMax = 0;
	m_State.m_iMin = 0;
}

StateBar::StateBar(const StateBar & _StateBar) : UserComponent(_StateBar)
{
	*this = _StateBar;
	m_iReferenceCount = 1;

	m_fRatio = 1.0f;
	m_State.m_iCur = _StateBar.m_State.m_iCur;
	m_State.m_iMax = _StateBar.m_State.m_iMax;
	m_State.m_iMin = _StateBar.m_State.m_iMin;

	//m_StateBar = nullptr;
	m_StateBar = _StateBar.m_StateBar;
	m_StateText = nullptr;
}


StateBar::~StateBar()
{
	SAFE_RELEASE(m_StateBar);
	SAFE_RELEASE(m_StateText);
}

UIBar * StateBar::GetUIBar()
{
	return m_StateBar;
}

UIText * StateBar::GetStateText()
{
	return m_StateText;
}

void StateBar::SetMin(int _iMin)
{
	m_State.m_iMin = _iMin;
}

void StateBar::SetMax(int _iMax)
{
	m_State.m_iMax = _iMax;
	m_State.m_iCur = _iMax;
}

void StateBar::SetUIBar(UIBar * _Bar)
{
	m_StateBar = _Bar;
}

void StateBar::SetUIText(UIText * _Text)
{
	m_StateText = _Text;
}

void StateBar::SetColor(Vector4 _vColor)
{
	m_StateBar->SetColor(_vColor);
}

void StateBar::SetChangeBarShader(std::string & _strKey)
{
	m_StateBar->SetShader(_strKey);
}

void StateBar::SetStateBarTexture(const std::string & _strName, const TCHAR * _pFileName, const std::string & _strPathName)
{
	m_StateBar->SetTexture(_strName, _pFileName, _strPathName);
}

void StateBar::Start()
{
}

bool StateBar::Init()
{
	return true;
}

int StateBar::Input(float _fTime)
{
	return 0;
}

int StateBar::Update(float _fTime)
{
	return 0;
}

int StateBar::LateUpdate(float _fTime)
{
	ShowStateText();
	return 0;
}

int StateBar::Collision(float _fTime)
{
	return 0;
}

int StateBar::PrevRender(float _fTime)
{
	return 0;
}

int StateBar::Render(float _fTime)
{
	return 0;
}

StateBar * StateBar::Clone() const
{
	return nullptr;
}

void StateBar::UpdateRatio(float _fPercent)
{
	float fPercent = _fPercent;
	if (fPercent < 0.0f)
	{
		fPercent = 0.0f;
	}
	else if (fPercent > 1.0f)
	{
		fPercent = 1.0f;
	}

	if (nullptr != m_StateBar)
	{
		m_StateBar->SetRatio(fPercent);
	}
}

void StateBar::UpdateRatio()
{
	// ���� ���°����� ������ ����Ѵ�.
	float fPercent = (float)(m_State .m_iCur) / (float)(m_State.m_iMax);
	UpdateRatio(fPercent);
}

void StateBar::ShowStateText()
{
	if (nullptr == m_StateText)
		return;

	// ���⿡ �ؽ�Ʈ üũ�Ȱ� ǥ�����ֱ�
	std::string CurValue = std::to_string(m_State.m_iCur);
	std::string MaxValue = std::to_string(m_State.m_iMax);

	std::string CurState = CurValue + " / " + MaxValue;

	std::wstring Statews = StringToWstring(CurState);
	m_StateText->SetText(Statews.c_str());

}

void StateBar::Heal(int _Value)
{
	m_State.m_iCur += _Value;

	if (m_State.m_iMax < m_State.m_iCur)
	{
		m_State.m_iCur = m_State.m_iMax;
	}

	UpdateRatio();
}

void StateBar::Damage(int _Value)
{
	m_State.m_iCur -= _Value;

	if (m_State.m_iMin > m_State.m_iCur)
	{
		m_State.m_iCur = m_State.m_iMin;
	}

	UpdateRatio();
}
