#include "pch.h"
#include <hinatacore/image.h>
#include <FreeImage.h>

HINATA_NAMESPACE_BEGIN

Image::Image( int width, int height )
	: width(width)
	, height(height)
	, data(width * height)
{

}

Image::Image(const std::string& path, bool verticalFlip)
{
	// Try to deduce the file format by the file signature
	auto format = FreeImage_GetFileType(path.c_str(), 0);

	if (format == FIF_UNKNOWN)
	{
		// Try to deduce the file format by the extension
		format = FreeImage_GetFIFFromFilename(path.c_str());

		if (format == FIF_UNKNOWN)
		{
			// Unknown image
			throw std::exception("Unknown image format");
		}
	}

	// Check the plugin capability
	if (!FreeImage_FIFSupportsReading(format))
	{
		throw std::exception("Unsupported format");
	}

	auto* bitmap = FreeImage_Load(format, path.c_str(), 0);

	if (!bitmap)
	{
		throw std::exception(("Failed to load an image " + path).c_str());
	}

	width = FreeImage_GetWidth(bitmap);
	height = FreeImage_GetHeight(bitmap);

	auto type = FreeImage_GetImageType(bitmap);
	auto bpp = FreeImage_GetBPP(bitmap);

	if (!(type == FIT_RGBF || type == FIT_RGBAF || (type == FIT_BITMAP && (bpp == 24 || bpp == 32))))
	{
		FreeImage_Unload(bitmap);
		throw std::exception("Unsupportted format by the renderer");
	}

	if (verticalFlip)
	{
		// Flip the image
		FreeImage_FlipVertical(bitmap);
	}

	for (int y = 0; y < height; y++)
	{
		if (type == FIT_RGBF)
		{
			FIRGBF* bits = (FIRGBF*)FreeImage_GetScanLine(bitmap, y);

			for (int x = 0; x < width; x++)
			{
				data.push_back(Vec3d(bits[x].red, bits[x].green, bits[x].blue));
			}
		}
		else if (type == FIT_RGBAF)
		{
			FIRGBAF* bits = (FIRGBAF*)FreeImage_GetScanLine(bitmap, y);

			for (int x = 0; x < width; x++)
			{
				data.push_back(Vec3d(bits[x].red, bits[x].green, bits[x].blue));
			}
		}
		else if (type == FIT_BITMAP)
		{
			BYTE* bits = (BYTE*)FreeImage_GetScanLine(bitmap, y);

			for (int x = 0; x < width; x++)
			{
				data.push_back(Vec3d(
					(double)bits[FI_RGBA_RED] / 255.0,
					(double)bits[FI_RGBA_GREEN] / 255.0,
					(double)bits[FI_RGBA_BLUE] / 255.0));
				bits += bpp / 8;
			}
		}
	}

	FreeImage_Unload(bitmap);
}

Vec3d Image::Evaluate(const Vec2d& uv)
{
	// 'repeat' texture coordinates
	int x = glm::clamp((int)(glm::fract(uv.x) * width), 0, width - 1);
	int y = glm::clamp((int)(glm::fract(uv.y) * height), 0, height - 1);
	return data[width * y + x];
}

void Image::Accumulate(const Vec4i& rect, const std::vector<Vec3d>& v)
{
	std::unique_lock<std::mutex> lock(accumColorMutex);

	for (int y = 0; y < rect[3]; y++)
	{
		for (int x = 0; x < rect[2]; x++)
		{
			int xx = x + rect[0];
			int yy = y + rect[1];
			int idx = yy * width + xx;

			data[idx] += v[idx];
		}
	}
}

void Image::Save(const std::string& path, double weight)
{
	// Create bitmap
	// 128 bit RGBA float image
	// Note: EXR - FIT_RGBAF, HDR - FIT_RGBF
	FIBITMAP* bitmap = FreeImage_AllocateT(FIT_RGBF, width, height);

	// Copy data
	for (int y = 0; y < height; y++)
	{
		FIRGBF* bits = (FIRGBF*)FreeImage_GetScanLine(bitmap, y);

		for (int x = 0; x < width; x++)
		{
			int idx = y * width + x;
			bits[x].red		= (float)(data[idx].r * weight);
			bits[x].green	= (float)(data[idx].g * weight);
			bits[x].blue	= (float)(data[idx].b * weight);
		}
	}

	// Save image as HDR format
	FreeImage_Save(FIF_HDR, bitmap, path.c_str(), HDR_DEFAULT);

	// Save image as OpenEXR format
	//FreeImage_Save(FIF_EXR, bitmap, path.c_str(), EXR_DEFAULT);

	FreeImage_Unload(bitmap);
}

HINATA_NAMESPACE_END