#ifndef __stb_image_owner
#define __stb_image_owner
#include <stb\stb_image.h>
class STBImageOwner {
private:
	int width;
	int height;
	int channels;
	stbi_uc *imgData;
public:
	STBImageOwner() = default;
	STBImageOwner(stbi_uc *imgData);
	STBImageOwner(const char *filename, int desiredChannels);
	~STBImageOwner();
	void obtain(stbi_uc *imgData);
	void destroy();
	const stbi_uc * get() const;
	int getW() const;
	int getH() const;
	int getChannels() const;
};
#endif