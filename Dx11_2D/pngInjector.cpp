#include "stdafx.h"
#include "pngInjector.h"

extern "C" {
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
}

HRESULT pngInjector::LoadImageW(const wstring & _wFileName, vector<vector<BColor>>& pixels)
{
	
	//	Convert wstring to string
	USES_CONVERSION;
	string fileName = W2A(_wFileName.c_str());

	int n = 0, width = 0, height = 0;
	vector<BYTE> image;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &n, 4);

	if (data != nullptr) {
		image = vector<BYTE>(data, data + width * height * 4);
		stbi_image_free(data);

		pixels.reserve(height);

		for (size_t i = 0; i < height; ++i) {

			vector<BColor> widPixel;
			widPixel.resize(width, BColor());

			for (size_t j = 0; j < width; j += 4) {
				size_t index = 4 * (i * width + j);
				widPixel[j] = BColor(image[index + 0], image[index + 1],
					image[index + 2], image[index + 3]);
			}

			pixels.push_back(widPixel);
		}

		return S_OK;
	}
	return E_FAIL;
}

HRESULT pngInjector::LoadImageA(const string & _aFileName, vector<vector<BColor>>& pixels)
{
	int n = 0, width = 0, height = 0;
	vector<BYTE> image;
	unsigned char* data = stbi_load(_aFileName.c_str(), &width, &height, &n, 4);

	if (data != nullptr) {
		image = vector<BYTE>(data, data + width * height * 4);
		stbi_image_free(data);

		pixels.reserve(height);

		for (size_t i = 0; i < height; ++i) {

			vector<BColor> widPixel;
			widPixel.resize(width, BColor());

			for (size_t j = 0; j < width; j += 4) {
				size_t index = ARGB_VALID * (i * width + j);
				widPixel[j] = BColor(image[index + 0], image[index + 1],
					image[index + 2], image[index + 3]);
			}

			pixels.push_back(widPixel);
		}

		return S_OK;
	}
	return E_FAIL;
}

HRESULT pngInjector::WriteImageW(const wstring & _wFileName, const vector<vector<BColor>>& pixels)
{
	if (pixels.size() < 1)
		return E_FAIL;
	else {
		int width = static_cast<int>(pixels.begin()->size());
		int height = static_cast<int>(pixels.size());
		BYTE* exportPixels = new BYTE[width * height * ARGB_VALID];

		int index = 0;
		for (int j = 0; j < height; ++j) {

			for (int i = 0; i < width; ++i) {

				float r = static_cast<float>(pixels[j][i].r);
				float g = static_cast<float>(pixels[j][i].g);
				float b = static_cast<float>(pixels[j][i].b);
				float a = static_cast<float>(pixels[j][i].a);

				exportPixels[index + 0] = pixels[j][i].r;
				exportPixels[index + 1] = pixels[j][i].g;
				exportPixels[index + 2] = pixels[j][i].b;
				exportPixels[index + 3] = pixels[j][i].a;

				index += 4;
			}
		}

		// if CHANNEL_NUM is 4, you can use alpha channel in png
		USES_CONVERSION;
		string fileName = W2A(_wFileName.c_str());
		stbi_write_png(fileName.c_str(), width, height, ARGB_VALID, exportPixels, width * ARGB_VALID);

		return S_OK;
	}
}

HRESULT pngInjector::WriteImageA(const string & _aFileName, const vector<vector<BColor>>& pixels)
{
	return E_NOTIMPL;
}
