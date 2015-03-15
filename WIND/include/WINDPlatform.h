#pragma once

namespace WIND {
#define WIND_PLATFORM_WIN32			1
#define WIND_PLATFORM_LINUX			2
//#define WIND_PLATFORM_APPLE			3
//#define WIND_PLATFORM_APPLE_IOS		4
//#define WIND_PLATFORM_ANDROID		5

// compiler option
#define WIND_COMPILER_MSVC			1
#define WIND_COMPILER_GNUC			2

// big-end or little-end
#define WIND_ENDIAN_LITTLE			1
#define WIND_ENDIAN_BIG				2

// CPU architecture
#define WIND_ARCHITECTURE_32		1
#define WIND_ARCHITECTURE_64		2

// Find compiler type and version
#if defined (_MSC_VER)
#	define WIND_COMPILER	WIND_COMPILER_MSVC
#	define WIND_COMP_VER	_MSC_VER
#else
#	pragma error "Unknow complier. Abort!!!"
#endif

#if defined (WIN32)
#define WIND_PLATFORM	WIND_PLATFORM_WIN32
#endif
}
