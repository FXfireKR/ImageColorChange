#include "stdafx.h"
#include "pngInjector.h"

extern "C" {
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
}

HRESULT pngInjector::LoadImageW(const wstring & _wFileName, ImageInfo & pixels)
{
	//	Convert wstring to string
	USES_CONVERSION;
	string fileName = W2A(_wFileName.c_str());

	int n = 0, width = 0, height = 0;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &n, 4);

	if (data != nullptr) {
		pixels.origin = vector<BYTE>(data, data + width * height * 4);
		stbi_image_free(data);

		pixels.pixels.reserve(height);

		for (size_t i = 0; i < height; ++i) {

			vector<BColor> widPixel;
			widPixel.resize(width, BColor());

			for (size_t j = 0; j < width; j += 4) {
				size_t index = 4 * (i * width + j);
				widPixel[j] = BColor(pixels.origin[index + 0], pixels.origin[index + 1],
					pixels.origin[index + 2], pixels.origin[index + 3]);
			}

			pixels.pixels.push_back(widPixel);
		}

		return S_OK;
	}
	return E_FAIL;
}

HRESULT pngInjector::LoadImageA(const string & _aFileName, ImageInfo & pixels)
{
	int n = 0, width = 0, height = 0;
	vector<BYTE> image;
	unsigned char* data = stbi_load(_aFileName.c_str(), &width, &height, &n, 4);

	if (data != nullptr) {
		image = vector<BYTE>(data, data + width * height * 4);
		stbi_image_free(data);

		pixels.pixels.reserve(height);

		for (size_t i = 0; i < height; ++i) {

			vector<BColor> widPixel;
			widPixel.resize(width, BColor());

			for (size_t j = 0; j < width; j += 4) {
				size_t index = 4 * (i * width + j);
				widPixel[j] = BColor(pixels.origin[index + 0], pixels.origin[index + 1],
					pixels.origin[index + 2], pixels.origin[index + 3]);
			}

			pixels.pixels.push_back(widPixel);
		}

		return S_OK;
	}
	return E_FAIL;
}

HRESULT pngInjector::WriteImageW(const wstring & _wFileName, const ImageInfo & pixels)
{
	if (pixels.pixels.size() < 1)
		return E_FAIL;
	else {
		int width = static_cast<int>(pixels.pixels.begin()->size());
		int height = static_cast<int>(pixels.pixels.size());
		BYTE* exportPixels = new BYTE[pixels.origin.size()];

		int index = 0;
		for (int j = 0; j < height; ++j) {

			for (int i = 0; i < width; ++i) {

				exportPixels[index + 0] = pixels.origin[index + 0];
				exportPixels[index + 1] = pixels.origin[index + 1];
				exportPixels[index + 2] = pixels.origin[index + 2];
				exportPixels[index + 3] = pixels.origin[index + 3];

				index += 4;
			}
		}

		// if CHANNEL_NUM is 4, you can use alpha channel in png
		USES_CONVERSION;
		string fileName = W2A(_wFileName.c_str());
		stbi_write_png(fileName.c_str(), width, height, 
			ARGB_VALID, exportPixels, width * ARGB_VALID);

		return S_OK;
	}
}

HRESULT pngInjector::WriteImageA(const string & _aFileName, const ImageInfo & pixels)
{
	if (pixels.pixels.size() < 1)
		return E_FAIL;
	else {
		int width = static_cast<int>(pixels.pixels.begin()->size());
		int height = static_cast<int>(pixels.pixels.size());
		BYTE* exportPixels = new BYTE[pixels.origin.size()];

		int index = 0;
		for (int j = 0; j < height; ++j) {

			for (int i = 0; i < width; ++i) {

				exportPixels[index + 0] = pixels.origin[index + 0];
				exportPixels[index + 1] = pixels.origin[index + 1];
				exportPixels[index + 2] = pixels.origin[index + 2];
				exportPixels[index + 3] = pixels.origin[index + 3];

				index += 4;
			}
		}

		// if CHANNEL_NUM is 4, you can use alpha channel in png
		stbi_write_png(_aFileName.c_str(), width, height,
			ARGB_VALID, exportPixels, width * ARGB_VALID);

		return S_OK;
	}
}

void pngInjector::ChangePixelColor(UINT x, UINT y, BColor changedColor, ImageInfo & pixels)
{
	UINT index = ARGB_VALID * (y * pixels.pixels.begin()->size() + x);

	//	Base Origin Pixel Color
	pixels.origin[index + 0] = changedColor.r;
	pixels.origin[index + 1] = changedColor.g;
	pixels.origin[index + 2] = changedColor.b;
	pixels.origin[index + 3] = changedColor.a;

	//	Change Pixel Position Color
	pixels.pixels[y][x] = changedColor;
}

void pngInjector::ChangePixelColor(UINT x, UINT y, D3DXCOLOR cColor, ImageInfo & pixels)
{
	UINT index = ARGB_VALID * (y * pixels.pixels.begin()->size() + x);

	//	Base Origin Pixel Color
	pixels.origin[index + 0] = static_cast<BYTE>(cColor.r * 255.0f);
	pixels.origin[index + 1] = static_cast<BYTE>(cColor.g * 255.0f);
	pixels.origin[index + 2] = static_cast<BYTE>(cColor.b * 255.0f);
	pixels.origin[index + 3] = static_cast<BYTE>(cColor.a * 255.0f);

	//	Change Pixel Position Color
	pixels.pixels[y][x].r = static_cast<BYTE>(cColor.r * 255.0f);
	pixels.pixels[y][x].g = static_cast<BYTE>(cColor.g * 255.0f);
	pixels.pixels[y][x].b = static_cast<BYTE>(cColor.b * 255.0f);
	pixels.pixels[y][x].a = static_cast<BYTE>(cColor.a * 255.0f);
}
