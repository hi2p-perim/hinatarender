#ifndef __HINATA_CORE_MATH_H__
#define __HINATA_CORE_MATH_H__

#include "common.h"
#include <cmath>

// Core types
#include "vector.h"
#include "matrix.h"
#include "quat.h"

// Utilities
#include "mathutils.h"

HINATA_NAMESPACE_BEGIN

// Constants
const double Pi			= 3.14159265358979323846;
const double InvPi		= 0.31830988618379067154;
const double InvTwoPi	= 0.15915494309189533577;
const double Inf		= DBL_MAX;
const double Eps		= 1e-7;
const double EpsLarge	= 1e-3;

HINATA_NAMESPACE_END

#endif // __HINATA_CORE_MATH_H__