
#ifndef FREEGATE_FINDER_H_H_H
#define FREEGATE_FINDER_H_H_H

#include "..\\src\\SocketSession.h"


class FreeGateFinder : public SocketSession
{
public:
	FreeGateFinder(DATEHEADER& packh, int aT);
	virtual ~FreeGateFinder();
	static int IsFreeGateFinder(DATEHEADER& packh, const char* pPacket);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	
protected:
	void WriteFGIndexFile();
protected:

};
#endif