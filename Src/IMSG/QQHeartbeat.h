
#ifndef QQHEARTBEAT_H_H_H
#define QQHEARTBEAT_H_H_H

#include "..//SocketSession.h"

class QQHeartbeat : public SocketSession
{
public:
	QQHeartbeat(DATEHEADER& packh, int aT);
	virtual ~QQHeartbeat();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
	int ProcessData(const char*pPacket,int n);
public:
    //判断是不是QQ 心跳
	static int IsQQHeartbeat(DATEHEADER& packh, const char* pPacket)
	{
		if (packh.m_dport == 8000)
		{
			if (packh.m_datalen == 39)
			{
				if ( (memcmp(pPacket,"\x02",1) == 0) && (memcmp(pPacket +38,"\x03",1) == 0) )  
				{
					return 1;
				}
			}
		}

		
		if ( (packh.m_dport == 8080) && (packh.m_datalen > 30))
		{

			if ( (memcmp(pPacket,"\x00\x00",2) == 0) &&
				 (memcmp(pPacket + 2,"\x00\x00",2) != 0) &&
				 (memcmp(pPacket + 4,"\x00\x00",2) == 0) &&
				 (memcmp(pPacket + 7,"\x09\x01\x00\x00",4) == 0))
			{
				return 2;
			}
		}

		if ((packh.m_dport == 80) && (packh.m_datalen > 30))
		{

			if ((memcmp(pPacket,"\x00\x00",2) == 0) &&
				(memcmp(pPacket + 2,"\x00\x00",2) != 0) &&
				(memcmp(pPacket + 6,"\x00\x00",2) == 0) &&
				(memcmp(pPacket + 9,"\x09\x01\x00\x00",4) == 0)
				)
			{
				return 3;
			}
		}
		return 0;
	}
};

#endif