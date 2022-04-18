#pragma once
#include "KManager.h"
class KSprite
{
public:
	std::wstring	   m_Name;
	float			   m_anim_time;
	std::vector<RECT>  m_anim_array;
	int				   m_AnimIndex;
public:
	void			SetAnimArray(RECT rt);
	void			RunAnim(int speed, int start, int end);
	bool			Release();
public:
	bool			Load(std::wstring filename);
};
class KSpriteManager : public KManager<KSprite, KSpriteManager>
{
	friend class Singleton<KSpriteManager>;
public:
	KSprite* Load(std::wstring filename)override;
	KSprite* Load(KSprite* sprite);
private:
	KSpriteManager() {};
public:
	~KSpriteManager() {};
};
#define g_SpriteManager KSpriteManager::Get()

