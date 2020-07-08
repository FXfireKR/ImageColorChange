#pragma once
#include "stb_image.h"
#include "stb_image_write.h"

#define ARGB_VALID	4
#define ARGB_MAX	255.0F

#ifdef UNICODE
#define PNG_LOADIMAGE pngInjector::LoadImageW
#define WRITEIMAGE_TO pngInjector::WriteImageW
#else
#define PNG_LOADIMAGE pngInjector::LoadImageA
#define WRITEIMAGE_TO pngInjector::WriteImageA
#endif

typedef struct stColorBytes {
	BYTE a, r, g, b;
	stColorBytes() : r(0), g(0), b(0), a(0) {}
	stColorBytes(BYTE _r, BYTE _g, BYTE _b, BYTE _a)
		: r(_r), g(_g), b(_b), a(_a) {}
}BColor;

typedef struct stImageInfo {
	vector<vector<BColor>> pixels;
	vector<BYTE> origin;
}ImageInfo;

enum eImageType {
	IMAGE_PNG = 0,
	IMAGE_JPG,
	IMAGE_BMP
};

class pngInjector
{
private:
	pngInjector() {}
	pngInjector(const pngInjector& other) = delete;
	pngInjector operator =(const pngInjector& other) = delete;
	~pngInjector() {}

public:
	static HRESULT LoadImageW(const wstring& _wFileName, ImageInfo& pixels);
	static HRESULT LoadImageA(const string& _aFileName, ImageInfo& pixels);
	static HRESULT WriteImageW(const wstring& _wFileName, const ImageInfo& pixels);
	static HRESULT WriteImageA(const string& _aFileName, const ImageInfo& pixels);
	
	static void ChangePixelColor(UINT x, UINT y, BColor changedColor, ImageInfo& pixels);
	static void ChangePixelColor(UINT x, UINT y, D3DXCOLOR cColor, ImageInfo& pixels);
	
};