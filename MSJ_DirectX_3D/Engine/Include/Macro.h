#pragma once
//export�϶��� import�϶�
#ifdef ENGINE_EXPORT
#define ENGINE_DLL __declspec(dllexport)
#else
#define ENGINE_DLL __declspec(dllimport)
#endif // ENGINE_EXPORT

#define ENGINE_BEGIN namespace Engine {
#define ENGINE_END }
#define ENGINE_USING using namespace Engine;

#define SAFE_DELETE(p)					if(p)	{ delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p)			if(p)	{ delete[] p; p = nullptr; }
#define SAFE_RELEASE(p)					if(p)	{ p->Release(); p = nullptr; }
#define SAFE_RELEASE_PROTOTYPE(p)		if(p)	{ p->Release();}


 //�̱���
 //DECLARE_SINGLETON -> �̱��� �����ϱ�
#define	DECLARE_SINGLETON(Type)	\
private:\
	static Type* m_pInst;\
public:\
	static Type* GetInst()\
	{\
		if(!m_pInst)\
			m_pInst = new Type;\
		return m_pInst;\
	}\
	static void DestroyInst()\
	{\
		SAFE_DELETE(m_pInst);\
	}\
private:\
	Type();\
	~Type();

#define DEFINITION_SINGLETON(Type) Type* Type::m_pInst = nullptr;

#define GET_SINGLETON(Type)			Type::GetInst()
#define DESTROY_SINGLETON(Type)		Type::DestroyInst()

#define _DEVICE	GET_SINGLETON(Device)->GetDevice()
#define _CONTEXT GET_SINGLETON(Device)->GetContext()
#define _RESOLUTION GET_SINGLETON(Device)->GetResolution()

// ������ �ν��Ͻ�, �ڵ�
#define WINDOWINSTANCE	GET_SINGLETON(Core)->GetWindowInstance()
#define WINDOWHANDLE	GET_SINGLETON(Core)->GetWindowHandle()
#define	RENDERTARGET2D	GET_SINGLETON(Device)->Get2DRenderTarget()

// Split�Լ��� Path�� �߶��ش�. -> Ȯ����, ������, ����̺� ��
// �����ڵ�� ��Ƽ����Ʈ ������ '��'
// ���� Ȯ���ڿ��� ��ҹ��ڿ� ���� �ٸ� ���ڿ��� �νĵȴ�.
// �׷��� ��� �빮�ڷ� �ٲ۵ڿ� ���ڿ��� �˻��ϵ��� �Ѵ�.
#ifdef UNICODE
#define	SplitPath	_wsplitpath_s
#else
#define	SplitPath	_splitpath_s
#endif // UNICODE