#include "StringHash.h"

// BKDR哈希函数
WIND::HANDLETYPE BKDRHash (const char *str)
{
	unsigned int seed = 131; // 31 131 1313 13131 131313 etc...
	unsigned int hash = 0;
	
	while (*str)
	{
		hash = hash * seed + (*str++);
	}
	
	return (hash & 0x7FFFFFFF);
}

// AP哈希函数
WIND::HANDLETYPE APHash (const char *str)
{
	unsigned int hash = 0;
	for (int i = 0; *str; ++i) {
		if ( (i & 1) == 0) {
			hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
		}
		else {
			hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
		}
	}
	
	return (hash & 0x7FFFFFFF);
}