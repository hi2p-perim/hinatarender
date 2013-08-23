#include "pch.h"
#include <hinatacore/texture.h>
#include <hinatacore/image.h>

HINATA_NAMESPACE_BEGIN

ConstantTexture::ConstantTexture( const Vec3d& color )
	: color(color)
{

}

Vec3d ConstantTexture::Evaluate( const Vec2d& uv )
{
	return color;
}

// ------------------------------------------------------------------------------------------

BitmapTexture::BitmapTexture( const std::string& path )
{
	image = std::make_shared<Image>(path);
}

Vec3d BitmapTexture::Evaluate( const Vec2d& uv )
{
	return image->Evaluate(uv);
}

HINATA_NAMESPACE_END

