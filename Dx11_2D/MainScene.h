#pragma once
#include "Scene.h"
#include "pngInjector.h"

#ifdef UNICODE
#define ImageNamingRule ImageNamingRuleW
#else
#define ImageNamingRule ImageNamingRuleA
#endif

class MainScene : public Scene
{
public:
	MainScene();
	~MainScene();

	virtual void init() override;
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

private:
	void ConvertImageW(wstring origPath);

	void ChangeFilterWithOrigin(const BColor _inputColor);

	wstring ImageNamingRuleW(const wstring wFileName, int offset);
	string ImageNamingRuleA(const string aFileName, int offset);


private:
	ImageInfo originImage;
	ImageInfo filterImage;
	ImageInfo exportImage;

	size_t width, height;

	

};