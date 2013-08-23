#ifndef __HINATA_CORE_TEXTURE_H__
#define __HINATA_CORE_TEXTURE_H__

#include "common.h"
#include "math.h"
#include <memory>

HINATA_NAMESPACE_BEGIN

class Image;

class Texture
{
public:

	Texture() {}
	virtual ~Texture() {}

private:

	Texture(const Texture&);
	Texture(Texture&&);
	void operator=(const Texture&);
	void operator=(Texture&&);

public:

	virtual Vec3d Evaluate(const Vec2d& uv) = 0;

};

// ------------------------------------------------------------------------------------------

class ConstantTexture : public Texture
{
public:

	ConstantTexture(const Vec3d& color);
	Vec3d Evaluate(const Vec2d& uv);

private:

	Vec3d color;

};

// ------------------------------------------------------------------------------------------

class BitmapTexture : public Texture
{
public:

	BitmapTexture(const std::string& path);
	Vec3d Evaluate(const Vec2d& uv);

private:

	std::shared_ptr<Image> image;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_TEXTURE_H__