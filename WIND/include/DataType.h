/************************************************************************/
/* Time:	2014-11-05		6:36
/* Author:	W.S.R
/* Purpose:	Atomic data type.
/************************************************************************/
#pragma once

namespace WIND
{
	typedef bool				Bool;
	typedef char				Int8;
	typedef unsigned char		UInt8;
	typedef short				Int16;
	typedef unsigned short		UInt16;
	typedef int					Int32;
	typedef unsigned int		UInt32;
	typedef long long			Int64;
	typedef unsigned long long	UInt64;
	typedef float				Float32;
	typedef double				Double64;

	typedef UInt64				HANDLETYPE;

	const HANDLETYPE HANDLE_INVALID = 0;
}