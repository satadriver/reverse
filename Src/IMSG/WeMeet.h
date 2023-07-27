
#ifndef WEMEET_H_H_H
#define WEMEET_H_H_H

#include "..//SocketSession.h"

class WeMeet : public SocketSession
{
public:
	WeMeet(DATEHEADER& packh, int aT);
	virtual ~WeMeet();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
public:

	static int IsWeMeet(DATEHEADER& packh, const char* pPacket)
	{
		if (packh.m_dport == 9090)
		{
			if (packh.m_datalen > 150)
			{
				if (memcmp(pPacket + 6,"\x08\x81\x02\x12",4) == 0)
				{
					return 1;
				}
			}
		
		}
		return 0;
	}
private:

	int IsConv(DATEHEADER& packh, const char* pPacket);
	int IsRoom(DATEHEADER& packh, const char* pPacket);
	int SendMsg();
	int SendRoomMsg();
};

#endif