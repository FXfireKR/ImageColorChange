#pragma once
#include "singleton.h"

class ImageManager : public singleton<ImageManager>
{
private:
	mutable map<string, ID3D11ShaderResourceView*> mImage;

public:
	ImageManager();
	~ImageManager();

	void InsertImage_FromeFile(string _key, const char* _path)const;
	void release()const;

public:
	inline ID3D11ShaderResourceView* getResourceView(__in string _key) const 
	{ 
		return mImage.count(_key) ? mImage[_key] : nullptr; 
	}
};