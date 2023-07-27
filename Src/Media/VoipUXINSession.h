#ifndef VOIP_UXIN_SESSION_H_H_H
#define VOIP_UXIN_SESSION_H_H_H
#include "voip.h"

class VoipUXINSession : public MediaSession
{
public:
	VoipUXINSession(DATEHEADER& packh, int aT, char * pPacket);
	virtual ~VoipUXINSession();
	
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual int ListToFile();
	static BOOL IsVoipUXIN(DATEHEADER& dataheader,const char *pPacket);
private:
	VoipPacketList	m_CallerPackList;
	VoipPacketList	m_CalleePackList;
	int				m_Packettype;

	VOIPFILEHEADER	m_VoipFileHdr;

	unsigned short	m_CallerPackIdx;
	unsigned int	m_CallerPackTime;
	unsigned int	m_CallerPackUnique;
	unsigned short	m_CalleePackIdx;
	unsigned int	m_CalleePackTime;
	unsigned int	m_CalleePackUnique;
	unsigned int	m_CallerIP;
	unsigned int	m_CalleeIP;
	unsigned short	m_CallerPORT;
	unsigned short	m_CalleePORT;
	unsigned int	m_CallerPackCnt;
	unsigned int	m_CalleePackCnt;
};

#endif