#include "pch.h"
#include <hinatacore/image.h>

class HDRLoaderResult
{
public:
	int width, height;
	float *cols;
};

typedef unsigned char RGBE[4];
#define R			0
#define G			1
#define B			2
#define E			3

#define  MINELEN	8				// minimum scanline length for encoding
#define  MAXELEN	0x7fff			// maximum scanline length for encoding

namespace
{

	static void workOnRGBE(RGBE *scan, int len, float *cols);
	static bool decrunch(RGBE *scanline, int len, FILE *file);
	static bool oldDecrunch(RGBE *scanline, int len, FILE *file);

	bool loadHDR(const char *fileName, HDRLoaderResult &res)
	{
		int i;
		char str[200];
		FILE *file;

		file = fopen(fileName, "rb");
		if (!file)
			return false;

		fread(str, 10, 1, file);
		if (memcmp(str, "#?RADIANCE", 10)) {
			fclose(file);
			return false;
		}

		fseek(file, 1, SEEK_CUR);

		char cmd[200];
		i = 0;
		char c = 0, oldc;
		while(true) {
			oldc = c;
			c = fgetc(file);
			if (c == 0xa && oldc == 0xa)
				break;
			cmd[i++] = c;
		}

		char reso[200];
		i = 0;
		while(true) {
			c = fgetc(file);
			reso[i++] = c;
			if (c == 0xa)
				break;
		}

		int w, h;
		if (!sscanf(reso, "-Y %ld +X %ld", &h, &w)) {
			fclose(file);
			return false;
		}

		res.width = w;
		res.height = h;

		float *cols = new float[w * h * 3];
		res.cols = cols;

		RGBE *scanline = new RGBE[w];
		if (!scanline) {
			fclose(file);
			return false;
		}

		// convert image 
		for (int y = h - 1; y >= 0; y--) {
			if (decrunch(scanline, w, file) == false)
				break;
			workOnRGBE(scanline, w, cols);
			cols += w * 3;
		}

		delete [] scanline;
		fclose(file);

		return true;
	}

	float convertComponent(int expo, int val)
	{
		float v = val / 256.0f;
		float d = (float) pow(2, expo);
		return v * d;
	}

	void workOnRGBE(RGBE *scan, int len, float *cols)
	{
		while (len-- > 0) {
			int expo = scan[0][E] - 128;
			cols[0] = convertComponent(expo, scan[0][R]);
			cols[1] = convertComponent(expo, scan[0][G]);
			cols[2] = convertComponent(expo, scan[0][B]);
			cols += 3;
			scan++;
		}
	}

	bool decrunch(RGBE *scanline, int len, FILE *file)
	{
		int  i, j;

		if (len < MINELEN || len > MAXELEN)
			return oldDecrunch(scanline, len, file);

		i = fgetc(file);
		if (i != 2) {
			fseek(file, -1, SEEK_CUR);
			return oldDecrunch(scanline, len, file);
		}

		scanline[0][G] = fgetc(file);
		scanline[0][B] = fgetc(file);
		i = fgetc(file);

		if (scanline[0][G] != 2 || scanline[0][B] & 128) {
			scanline[0][R] = 2;
			scanline[0][E] = i;
			return oldDecrunch(scanline + 1, len - 1, file);
		}

		// read each component
		for (i = 0; i < 4; i++) {
			for (j = 0; j < len; ) {
				unsigned char code = fgetc(file);
				if (code > 128) { // run
					code &= 127;
					unsigned char val = fgetc(file);
					while (code--)
						scanline[j++][i] = val;
				}
				else  {	// non-run
					while(code--)
						scanline[j++][i] = fgetc(file);
				}
			}
		}

		return feof(file) ? false : true;
	}

	bool oldDecrunch(RGBE *scanline, int len, FILE *file)
	{
		int i;
		int rshift = 0;

		while (len > 0) {
			scanline[0][R] = fgetc(file);
			scanline[0][G] = fgetc(file);
			scanline[0][B] = fgetc(file);
			scanline[0][E] = fgetc(file);
			if (feof(file))
				return false;

			if (scanline[0][R] == 1 &&
				scanline[0][G] == 1 &&
				scanline[0][B] == 1) {
					for (i = scanline[0][E] << rshift; i > 0; i--) {
						memcpy(&scanline[0][0], &scanline[-1][0], 4);
						scanline++;
						len--;
					}
					rshift += 8;
			}
			else {
				scanline++;
				len--;
				rshift = 0;
			}
		}
		return true;
	}

}

HINATA_NAMESPACE_BEGIN

Image::Image( int width, int height )
	: width(width)
	, height(height)
	, data(width * height)
{

}

Image::Image(const std::string& path, bool verticalFlip)
{
	namespace fs = boost::filesystem;

	// Extension
	auto ext = fs::path(path).extension().string();

	if (ext == ".hdr")
	{
		HDRLoaderResult res;
		loadHDR(path.c_str(), res);
		
		width = res.width;
		height = res.height;

		data.assign(width * height, Vec3d());

		for (int y = 0; y < res.height; y++)
		{
			for (int x = 0; x < res.width; x++)
			{
				int idx = y * width + x;
				data[idx] = Vec3d(
					static_cast<double>(res.cols[idx * 3]),
					static_cast<double>(res.cols[idx * 3 + 1]),
					static_cast<double>(res.cols[idx * 3 + 2]));
			}
		}

		HINATA_SAFE_DELETE(res.cols);
	}
	else
	{
		// Change extenstion to .ppm
		auto newPath = fs::path(path).replace_extension(".ppm").string();

		// Load ppm file
		FILE* fp;
		fp = fopen(newPath.c_str(), "rb");
		if (!fp)
		{
			throw std::exception(("fopen : " + newPath).c_str());
		}

		// Header string (P*)
		char buff[16];
		if (!fgets(buff, sizeof(buff), fp))
		{
			throw std::exception("Invalid image");
		}

		if (buff[0] != 'P' || buff[1] != '6')
		{
			throw std::exception("Invalid image (not P6)");
		}

		// Skip comments
		char t = getc(fp);
		while (t == '#')
		{
			while (getc(fp) != '\n');
			t = getc(fp);
		}
		ungetc(t, fp);

		// Width, height
		if (fscanf(fp, "%d %d", &width, &height) != 2)
		{
			throw std::exception("Invalid image");
		}

		// Component depth
		int compColor;
		if (fscanf(fp, "%d", &compColor) != 1)
		{
			throw std::exception("Invalid image");
		}

		while (fgetc(fp) != '\n');

		// Allocate data
		data.assign(width * height, Vec3d());

		// Load data
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				// Get color
				char r = fgetc(fp);
				char g = fgetc(fp);
				char b = fgetc(fp);

				// Convert to float and assign
				data[y * width + x] = Vec3d(
					static_cast<double>(r) / 255.0,
					static_cast<double>(g) / 255.0,
					static_cast<double>(b) / 255.0);
			}
		}

		fclose(fp);
	}
}

Vec3d Image::Evaluate(const Vec2d& uv)
{
	// 'repeat' texture coordinates
	int x = Math::Clamp((int)(Math::Fract(uv.x) * width), 0, width - 1);
	int y = Math::Clamp((int)(Math::Fract(uv.y) * height), 0, height - 1);
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
	FILE* fp;

#ifdef HINATA_PLATFORM_WINDOWS
	fopen_s(&fp, path.c_str(), "wb");
#else
	fp = fopen(path.c_str(), "wb");
#endif

	fprintf(fp, "P6\n");
	fprintf(fp, "%d %d\n", width, height);
	fprintf(fp, "255\n");

	for (int y = width - 1; y >= 0; y--)
	{
		for (int x = 0; x < width; x++)
		{
			int idx = y * width + x;
			unsigned char color[3];
			color[0] = Math::Clamp(static_cast<int>(data[idx].r * weight * 255.0), 0, 255);
			color[1] = Math::Clamp(static_cast<int>(data[idx].g * weight * 255.0), 0, 255);
			color[2] = Math::Clamp(static_cast<int>(data[idx].b * weight * 255.0), 0, 255);
			fwrite(color, 1, 3, fp);
		}
	}

	fclose(fp);
}

HINATA_NAMESPACE_END