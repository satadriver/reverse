#ifndef ZELLOAUDIO_H_H_H
#define ZELLOAUDIO_H_H_H
#include "..\\src\\media\\voip.h"







#define UDP_OPUS_FRAME		1
#define UDP_AMR_FRAME		2
#define UDP_SPEEX_FRAME		3
#define TCP_OPUS_FRAME		4
#define TCP_AMR_FRAME		5
#define TCP_SPEEX_FRAME		6
#define ZELLO_MSG			7

#define ZELLO_ITEM_LENTH	64

class ZelloAudio : public MediaSession
{
public:
	ZelloAudio(DATEHEADER& packh, int aT, char * Packet, int EncodeType);
	virtual ~ZelloAudio();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual int ListToFile();
	static BOOL IsZelloAudio(DATEHEADER& packh, const char *pPacket);
	static BOOL ParseZelloPacket(DATEHEADER & packh, const char * pPacket);
	void WriteZelloIndexFile(int Action);
	
private:
	VoipPacketList m_CallerPackList;
	VoipPacketList m_CalleePackList;
	int				m_Packettype;
	
	VOIPFILEHEADER	m_VoipFileHdr;
	unsigned int	m_EncodeType;
	
	char m_FromName[ZELLO_ITEM_LENTH];
	char m_ToName[ZELLO_ITEM_LENTH];
	char m_Message[ZELLO_ITEM_LENTH * 16];
	char m_Codec[ZELLO_ITEM_LENTH];
	char m_BuddyList[ZELLO_ITEM_LENTH * 64];
	char m_PlatForm[ZELLO_ITEM_LENTH];

	unsigned short	m_CallerPackIdx;
	unsigned short	m_CallerPackUnique;
	unsigned short	m_CalleePackIdx;
	unsigned short	m_CalleePackUnique;
	unsigned int	m_CallerIP;
	unsigned int	m_CalleeIP;
	unsigned short	m_CallerPORT;
	unsigned short	m_CalleePORT;
	unsigned int	m_CallerPackCnt;
	unsigned int	m_CalleePackCnt;
};


#endif