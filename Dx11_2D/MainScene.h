#pragma once
#include "Scene.h"

#include <iostream>


class MainScene : public Scene
{
public:
	MainScene();
	~MainScene();

private:
	virtual void init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

};