#include "STBImageOwner.hpp"
#include <stb\stb_image.h>
#include <iostream>

STBImageOwner::STBImageOwner(stbi_uc *imgData) :
imgData(imgData)
{
}

STBImageOwner::~STBImageOwner() {
	destroy();
}

void STBImageOwner::obtain(stbi_uc *imgData) {
	this->imgData = imgData;
}

void STBImageOwner::destroy()
{
	if (imgData != NULL) {
		std::cout << "STBImageOwner destroyed." << std::endl;
		stbi_image_free(imgData);
		imgData = NULL;
	}
}

const stbi_uc * STBImageOwner::get() const
{
	return imgData;
}
