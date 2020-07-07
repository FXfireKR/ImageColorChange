#include "stdafx.h"
#include "ImageManager.h"

ImageManager::ImageManager()
{
}

ImageManager::~ImageManager()
{
}

void ImageManager::InsertImage_FromeFile(string _key, const char * _path) const
{
	if (!mImage.count(_key))
	{
		FILE* f = fopen(_path, "r");
		if (f != NULL)
		{
			ID3D11ShaderResourceView* srv;
			HRESULT hr;

			D3DX11CreateShaderResourceViewFromFileA(Device, _path, NULL, NULL, &srv, &hr);
			assert(SUCCEEDED(hr));

			mImage.insert(make_pair(_key, srv));
		}
	}
}

void ImageManager::release() const
{
	for (auto& it : mImage)
		SAFE_RELEASE(it.second);
	mImage.clear();	
}
