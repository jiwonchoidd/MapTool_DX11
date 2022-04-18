#pragma once
#include "KCore.h"
#include "KSceneManager.h"
class Sample : public KCore
{
public:
	bool		Init()override;
	bool		Frame()override;
	bool		Render()override;
	bool		Release()override;
public:
	Sample();
	virtual ~Sample();
};

