#include "stdafx.h"
#include "MainScene.h"

MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

void MainScene::init()
{
	
}

void MainScene::release()
{
}

void MainScene::update()
{
	if (ImGui::Button("Convert Image", ImVec2(100, 50)))
	{
		ConvertImageW(L"../Export/Ant_46_S_1.png");
	}
}

void MainScene::render()
{
}

void MainScene::ConvertImageW(wstring origPath)
{
	//	2개의 파일을 동시 샘플링해서 pixel 값을 바꾼다.
	wstring filterPath;
	filterPath.append(origPath, 0, origPath.find_last_of('.'));
	filterPath += L"_filter.png";

	originImage.origin.clear();
	originImage.pixels.clear();

	filterImage.origin.clear();
	filterImage.pixels.clear();

	exportImage.origin.clear();
	exportImage.pixels.clear();

	PNG_LOADIMAGE(origPath, originImage);
	PNG_LOADIMAGE(origPath, exportImage);
	PNG_LOADIMAGE(filterPath, filterImage);

	width = originImage.pixels.begin()->size();
	height = originImage.pixels.size();

	ChangeFilterWithOrigin(BColor(255, 153, 0, 255));

	WRITEIMAGE_TO(ImageNamingRule(L"../Export/Ant_46_S_1.png", 1), exportImage);
	MessageBoxA(g_hWnd, "Create New Image File !", "System", MB_OK);
}

void MainScene::ChangeFilterWithOrigin(const BColor _inputColor)
{
	D3DXCOLOR color, iptColor, pixelColor;
	UINT index;
	for(size_t y = 0; y < height; ++y)
		for (size_t x = 0; x < width; ++x) {
			index = ARGB_VALID * (y * exportImage.pixels.begin()->size() + x);

			if (filterImage.origin[index + 0] == 255 && 
				filterImage.origin[index + 1] == 0 && 
				filterImage.origin[index + 2] == 255) {

				iptColor = D3DXCOLOR((float)_inputColor.r / ARGB_MAX, (float)_inputColor.g / ARGB_MAX,
					(float)_inputColor.b / ARGB_MAX, (float)_inputColor.a / ARGB_MAX);

				pixelColor = D3DXCOLOR((float)originImage.origin[index + 0] / ARGB_MAX, (float)originImage.origin[index + 1] / ARGB_MAX,
					(float)originImage.origin[index + 2] / ARGB_MAX, (float)originImage.origin[index + 3] / ARGB_MAX);

				color = D3DXCOLOR(iptColor.r * pixelColor.r, iptColor.g * pixelColor.g,
					iptColor.b * pixelColor.b, iptColor.a * pixelColor.a);

				//	Float Epsilon Complete
				{
					if (color.r < FLOAT_EPSILON)
						color.r = 0.0f;
					if (color.g < FLOAT_EPSILON)
						color.g = 0.0f;
					if (color.b < FLOAT_EPSILON)
						color.b = 0.0f;
					if (color.a < FLOAT_EPSILON)
						color.a = 0.0f;

					if (color.r > ARGB_MAX - FLOAT_EPSILON)
						color.r = ARGB_MAX;
					if (color.g > ARGB_MAX - FLOAT_EPSILON)
						color.g = ARGB_MAX;
					if (color.b > ARGB_MAX - FLOAT_EPSILON)
						color.b = ARGB_MAX;
					if (color.a > ARGB_MAX - FLOAT_EPSILON)
						color.a = ARGB_MAX;
				}

				pngInjector::ChangePixelColor(x, y, color, exportImage);
			}
		}
	
}

wstring MainScene::ImageNamingRuleW(const wstring wFileName, int offset)
{
	wstring newFileName;
	newFileName.append(wFileName, 0, wFileName.find_first_of('_'));

	string numberString = (offset < 9) ? ConvertFormat("_0%d", offset) : ConvertFormat("_%d", offset);
	USES_CONVERSION;
	wstring temp = A2W(numberString.c_str());

	newFileName.append(temp);
	newFileName.append(wFileName, wFileName.find_first_of('_'), wFileName.size());

	return newFileName;
}

string MainScene::ImageNamingRuleA(const string aFileName, int offset)
{
	return string();
}
