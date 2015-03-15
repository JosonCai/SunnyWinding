#include "CommonFunction.h"

void ReplaceString (std::string &strBase, const std::string &strSrc, const std::string &strDst)
{
	std::string::size_type	pos = 0;
	std::string::size_type	srcLen = strSrc.size ();
	std::string::size_type	dstLen = strDst.size ();
	pos = strBase.find(strSrc, pos);
	while ((pos != std::string::npos)) {
		strBase.replace(pos, srcLen, strDst);
		pos = strBase.find (strSrc, (pos + dstLen));
	}
}