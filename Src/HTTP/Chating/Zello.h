
#ifndef HTTP_ZELLO_H_H_H
#define HTTP_ZELLO_H_H_H

#include "../HttpPro.h"

class Zello
{
public:
	static int IsZelloPacket(DATEHEADER & packh, const char * pPacket);
	static int ProcessZelloPacket(DATEHEADER & packh, const char * pPacket,HttpProtocol * pHttpProtocol);
};

#endif