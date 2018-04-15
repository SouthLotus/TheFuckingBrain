#ifndef __stb_image_owner
#define __stb_image_owner
#include <stb\stb_image.h>
class STBImageOwner {
private:
	stbi_uc *imgData;
public:
	STBImageOwner() = default;
	STBImageOwner(stbi_uc *imgData);
	~STBImageOwner();
	void obtain(stbi_uc *imgData);
	void destroy();
	const stbi_uc * get() const;
};
#endif