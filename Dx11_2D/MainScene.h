#pragma once
#include "GameNode.h"

class MainScene : public GameNode
{
public:
	MainScene();
	~MainScene();

private:


	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Release() override;

};