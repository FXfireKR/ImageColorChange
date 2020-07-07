#include "stdafx.h"
#include "MainScene.h"
#include "pngInjector.h"

MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}


void MainScene::init()
{
	vector<vector<BColor>> pixels;
	PNG_LOADIMAGE(L"../Export/Ant_035_S_0_template.png", pixels);
	WRITEIMAGE_TO(L"../Export/Ant_035_S_0_Red.png", pixels);
}

void MainScene::release()
{
	
}

void MainScene::update()
{
}

void MainScene::render()
{
}
