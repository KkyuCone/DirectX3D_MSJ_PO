#pragma once
#include "../Reference.h"

ENGINE_BEGIN

// 叔霜旋生稽 神崎詮闘(暁澗 梓端)亜 壕帖鞠澗員精 Layer虞壱 左檎 吉陥. しさし
// 傾戚嬢澗 嬢恐 授辞稽 穣汽戚闘拝 依昔走亜 掻推馬陥.
// 食奄辞 授辞澗 m_iOrder稽 歳嫌馬悟 神硯託授生稽 穣汽戚闘敗 しさし
class ENGINE_DLL Layer : public Reference
{
private:
	friend class Scene;

private:
	Layer();
	~Layer();

private:
	class Scene*	m_pScene;
	int				m_iOrder;			// 傾戚嬢 授辞研 舛馬奄 是廃 痕呪 (神硯託授生稽 拝暗)

	// 唖唖税 傾戚嬢級精 切重級戚 巷充 神崎詮闘亜 赤澗走 硝壱赤製.
	// vector研 紫遂馬走 省壱 list研 紫遂馬澗 戚政澗 左搭 諮脊 肢薦亜 朔腰馬惟 (授辞拭 淫域蒸戚)析嬢劾 井酔 list研 紫遂敗 しさし
	// 食奄識 惟績神崎詮闘級精 情薦, 刊亜 肢薦 吃走 乞牽奄 凶庚拭 list稽 紫遂
	std::list<class GameObject*>	m_GameObjectList;

	std::list<class GameObject*>	m_StartList;

	// + STL梓端稽 閤聖井酔 (古鯵痕呪蟹..閤澗井酔) 凧繕焼還 匂昔斗稽 隔嬢醤廃陥.
	// -> 戚政澗 幻鉦 析鋼痕呪坦軍 隔惟鞠檎 益凶原陥 葵 差紫亜 焼観 五乞軒研 拝雁馬奄 凶庚戚陥. (五乞軒 涯搾)

public:
	int GetOrder()	const;
	void SetOrder(int _Order);
	void Start();

	void SetScene(Scene* _pScene);
	Scene* GetScene();

public:
	bool Init();
	int Input(float _fTime);
	int Update(float _fTime);
	int LateUpdate(float _fTime);
	int Collision(float _fTime);
	int Render(float _fTime);

	void AddGameObject(class GameObject* _GameObject);
	class GameObject* FindGameObject(std::string _Tag);
	
	void DeleteGameObjectToTag(class GameObject* _Object);					// Delete澗 焼森 傾戚嬢拭辞 糎仙研 走酔澗依績
	void DeleteGameObject(class GameObject* _Object);
	void DeleteGameObject(std::string _Tag);

	void DeleteAllGameObject();

public:
	
};

ENGINE_END

