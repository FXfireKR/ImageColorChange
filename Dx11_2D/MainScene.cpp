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
	convertList.reserve(10);
	ReadBaseColorSetting();
}

void MainScene::release()
{
}

void MainScene::update()
{
	if (ImGui::Button("Insert Lists", ImVec2(120, 20))) {
		string path = "./Export/*.png";
		struct _finddata_t fd;
		intptr_t handle;

		if ((handle = _findfirst(path.c_str(), &fd)) == -1L)
			cout << "No file in directory!" << endl;
		do
		{
			string curFolder = "./Export/"; curFolder += fd.name;
			if (curFolder.find("_filter") == string::npos)
				fileList.push_back(curFolder);
		} while (_findnext(handle, &fd) == 0);

		_findclose(handle);
	}

	if (ImGui::Button("Convert Images", ImVec2(120, 20))) {
		for (size_t i = 0; i < fileList.size(); ++i) {
			USES_CONVERSION;
			ConvertImage(A2W(fileList[i].c_str()));
		}

	}
		
}

void MainScene::render()
{
}

void MainScene::ReadBaseColorSetting()
{
	FILE* fp = fopen(BASE_COLOR_FILES, "rb");
	if (fp != NULL) {
		while (!feof(fp)) {
			char buffer[256];
			fgets(buffer, 256, fp);

			string sBuffer = buffer;

			if (sBuffer.size() < 2)continue;
			if (sBuffer[0] == '/' && sBuffer[1] == '/') continue;	//	// 는 주석처리한다
			
			// Get Color Red, Green, Blue
			string id; 
			BColor newColor;

			id.append(sBuffer, 0, sBuffer.find_first_of(' '));	
			newColor.r = atoi(id.c_str());
			sBuffer.erase(0, sBuffer.find_first_of(' ') + 1);
			id.clear();

			id.append(sBuffer, 0, sBuffer.find_first_of(' '));
			newColor.g = atoi(id.c_str());
			sBuffer.erase(0, sBuffer.find_first_of(' ') + 1);
			id.clear();

			id.append(sBuffer, 0, sBuffer.find_first_of(' '));
			newColor.b = atoi(id.c_str());
			id.clear();

			newColor.a = 255;

			convertList.push_back(newColor);
		}
	}
	else
		MessageBoxA(g_hWnd, "Not readable file or Not placed file", "ColorSetting.inl", MB_OK);
	fclose(fp);
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

	pngInjector::LoadImageW(origPath, originImage);
	pngInjector::LoadImageW(origPath, exportImage);
	pngInjector::LoadImageW(filterPath, filterImage);

	width = originImage.pixels.begin()->size();
	height = originImage.pixels.size();

	for (size_t i = 0; i < convertList.size(); ++i) {
		ChangeFilterWithOrigin(convertList[i]);
		WRITEIMAGE_TO(ImageNamingRule(origPath, static_cast<int>(i + 1)), exportImage);
	}
}

void MainScene::ConvertImageA(string origPath)
{
	//	2개의 파일을 동시 샘플링해서 pixel 값을 바꾼다.
	string filterPath;
	filterPath.append(origPath, 0, origPath.find_last_of('.'));
	filterPath += "_filter.png";

	originImage.origin.clear();
	originImage.pixels.clear();

	filterImage.origin.clear();
	filterImage.pixels.clear();

	exportImage.origin.clear();
	exportImage.pixels.clear();

	pngInjector::LoadImageA(origPath, originImage);
	pngInjector::LoadImageA(origPath, exportImage);
	pngInjector::LoadImageA(filterPath, filterImage);

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
	newFileName.append(wFileName, 0, wFileName.find_last_of('/') + 1);
	newFileName += L"Mutant";

	string numberString = (offset < 9) ? ConvertFormat("_0%d", offset) : ConvertFormat("_%d", offset);
	USES_CONVERSION;
	wstring temp = A2W(numberString.c_str());

	newFileName.append(temp);
	newFileName.append(wFileName, wFileName.find_first_of('_'), wFileName.size());

	return newFileName;
}

string MainScene::ImageNamingRuleA(const string aFileName, int offset)
{
	string newFileName;
	newFileName.append(aFileName, 0, aFileName.find_first_of('_'));

	string numberString = (offset < 9) ? ConvertFormat("_0%d", offset) : ConvertFormat("_%d", offset);

	newFileName.append(numberString);
	newFileName.append(aFileName, aFileName.find_first_of('_'), aFileName.size());

	return newFileName;
}
