#ifndef __HINATA_CORE_ENVIRONMENT_LIGHT_H__
#define __HINATA_CORE_ENVIRONMENT_LIGHT_H__

#include "common.h"
#include "math.h"
#include <memory>

HINATA_NAMESPACE_BEGIN

/*!
	Environment light.
	We note that current implementation separates AreaLight and EnvironmentLight,
	and the class does not have the sampling related functions.
	So for now, it can be applicable only the PT based renderers.
	TODO : Integrate environment light and area light into general Light class.
*/
class EnvironmentLight
{
public:

	EnvironmentLight() {}
	virtual ~EnvironmentLight() {}

private:

	EnvironmentLight(const EnvironmentLight&);
	EnvironmentLight(EnvironmentLight&&);
	void operator=(const EnvironmentLight&);
	void operator=(EnvironmentLight&&);

public:

	virtual Vec3d Evaluate(const Vec3d& d) = 0;

};

// ------------------------------------------------------------------------------------------

class ConstantEnvironmentLight : public EnvironmentLight
{
public:

	ConstantEnvironmentLight(const Vec3d& Le);

public:

	Vec3d Evaluate(const Vec3d& d) { return Le; }

private:

	Vec3d Le;

};

// ------------------------------------------------------------------------------------------

class Image;

class BitmapEnvironmentLight : public EnvironmentLight
{
public:

	BitmapEnvironmentLight(const std::string& path, double offset, double scale);

public:

	Vec3d Evaluate(const Vec3d& d);

private:

	std::shared_ptr<Image> image;
	double offset;
	double scale;

};

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_ENVIRONMENT_LIGHT_H__