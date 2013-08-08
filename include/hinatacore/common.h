#ifndef __HINATA_CORE_COMMON_H__
#define __HINATA_CORE_COMMON_H__

// C++ compiler is required
#ifndef __cplusplus
	#error "C++ compiler is required"
#endif

// Debug mode?
#ifndef NDEBUG
	#define HINATA_DEBUG_MODE
#endif

// ----------------------------------------------------------------------

// Platform
#ifdef _WIN32
	#define HINATA_PLATFORM_WINDOWS
#elif defined(__linux)
	#define HINATA_PLATFORM_LINUX
#else
	#error "Unsupportted platform"
#endif

#ifdef HINATA_PLATFORM_WINDOWS
	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
#endif

// Compiler and architecture
#ifdef _MSC_VER
	// Microsoft Visual C++
	#define HINATA_COMPILER_MSVC
	#ifdef _M_IX86
		#define HINATA_ARCH_X86
	#elif defined(_M_X64)
		#define HINATA_ARCH_X64
	#elif defined(_M_ARM)
		#define HINATA_ARCH_ARM
	#else
		#error "Unsupportted architecture"
	#endif
#elif (defined(__GNUC__) || defined(__MINGW32__))
	// GNU GCC
	#define HINATA_COMPILER_GCC
	#ifdef __i386__
		#define HINATA_ARCH_X86
	#elif defined(__x86_64__)
		#define HINATA_ARCH_X64
	#elif defined(__arm__)
		#define HINATA_ARCH_ARM
	#else
		#error "Unsupportted architecture"
	#endif
#else
	#error "Unsupportted compiler"
#endif

// ----------------------------------------------------------------------

// Library import or export
#ifdef HINATA_COMPILER_MSVC
	#ifdef HINATA_EXPORTS
		#define HINATA_PUBLIC_API __declspec(dllexport)
	#else
		#define HINATA_PUBLIC_API __declspec(dllimport)
	#endif
	#define HINATA_HIDDEN_API
#elif defined(HINATA_COMPILER_GCC)
	#define HINATA_PUBLIC_API __attribute__ ((visibility("default")))
	#define HINATA_HIDDEN_API __attribute__ ((visibility("hidden")))
#else
	#define HINATA_PUBLIC_API
	#define HINATA_HIDDEN_API
#endif

// Plugin export
#ifdef HINATA_COMPILER_MSVC
	#define HINATA_PLUGIN_API __declspec(dllexport)
#elif defined(HINATA_COMPILER_GCC)
	#define HINATA_PLUGIN_API __attribute__ ((visibility("default")))
#else
	#define HINATA_PLUGIN_API 
#endif

// ----------------------------------------------------------------------

/*!
	\def HINATA_FORCE_NO_SIMD
	Specifies not to use SIMD.
*/

// SSE support
#ifndef HINATA_FORCE_NO_SIMD
	#define HINATA_USE_SIMD
	#ifdef HINATA_COMPILER_MSVC
		#if defined(HINATA_ARCH_X86) || defined(HINATA_ARCH_X64)
			#define HINATA_USE_SSE
			#include <immintrin.h>
		#elif defined(HINATA_ARCH_ARM)
			#define HINATA_USE_NEON
			#include <arm_neon.h>
		#endif
	#elif defined(HINATA_COMPILER_GCC)
		#if defined(HINATA_ARCH_X86) || defined(HINATA_ARCH_X64)
			#define HINATA_USE_SSE
			#ifdef __AVX2__
				#include <immintrin.h>
			#elif defined(__SSE4__)
				#include <smmintrin.h>
			#elif defined(__SSE3__)
				#include <pmmintrin.h>
			#else
				#include <emmintrin.h>
			#endif
		#elif defined(HINATA_ARCH_ARM)
			#define HINATA_USE_NEON
			#include <arm_neon.h>
		#endif
	#endif
#endif

// Force inline
#ifdef HINATA_COMPILER_MSVC
	#define HINATA_FORCE_INLINE __forceinline
#elif defined(HINATA_COMPILER_GCC)
	#define HINATA_FORCE_INLINE inline __attribute__((always_inline))
#endif

// Alignment
#ifdef HINATA_COMPILER_MSVC
	#define HINATA_ALIGN(x) __declspec(align(x))
#elif defined(HINATA_COMPILER_GCC)
	#define HINATA_ALIGN(x) __attribute__((aligned(x)))
#endif
#define HINATA_ALIGN_16 HINATA_ALIGN(16)
#define HINATA_ALIGN_32 HINATA_ALIGN(32)

// ----------------------------------------------------------------------

// Namespace
#define HINATA_NAMESPACE_BEGIN namespace hinata {
#define HINATA_NAMESPACE_END }

#define HINATA_TEST_NAMESPACE_BEGIN namespace hinatatest {
#define HINATA_TEST_NAMESPACE_END }

// ----------------------------------------------------------------------

#define HINATA_SAFE_DELETE(val) if ((val) != NULL ) { delete (val); (val) = NULL; }
#define HINATA_SAFE_DELETE_ARRAY(val) if ((val) != NULL ) { delete[] (val); (val) = NULL; }

#endif // __HINATA_CORE_COMMON_H__