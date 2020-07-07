#pragma once
#include "SingletonBase.h"
#include "Timer.h"

#define g_pTimeManager TimeManager::GetInstance()

class TimeManager : public SingletonBase<TimeManager>
{
private:
	Timer*	m_pTimer;

public:
	TimeManager();
	~TimeManager();

	void Init();
	void Update(float lock = 0.0f);
	void Render();

	float GetDeltaTime() const { return m_pTimer->GetDeltaTime(); }
	float GetWorldTime() const { return m_pTimer->GetWorldTime(); }
};

