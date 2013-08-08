#ifndef __HINATA_CORE_IMAGE_H__
#define __HINATA_CORE_IMAGE_H__

#include "common.h"
#include "math.h"
#include <mutex>

HINATA_NAMESPACE_BEGIN

class Image
{
public:

	Image(int width, int height);
	Image::Image(const std::string& path, bool verticalFlip = false);
	
	int Width() { return width; }
	int Height() { return height; }

	Vec3d Evaluate(const Vec2d& uv);
	void Accumulate(const Vec4i& rect, const std::vector<Vec3d>& v);
	void Save(const std::string& path, double weight);

private:

	int width;
	int height;
	std::vector<Vec3d> data;
	std::mutex accumColorMutex;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_IMAGE_H__