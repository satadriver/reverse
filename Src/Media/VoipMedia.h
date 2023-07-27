#ifndef VOIP_MEDIA_H
#define VOIP_MEDIA_H
#define VOIP_CALL 1

#include "../VOIP_CALL.H"

// 8080 15060 5060 5070 10000 80 5060 6060 5068

int Voip97CallPhoneNum(DATEHEADER& packh, const char *pPacket,int g_PacketFrom);


class Voip97CallMediaSession : public MediaSession
{
public:
	Voip97CallMediaSession(DATEHEADER& packh, int aT);
	virtual ~Voip97CallMediaSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual int ListToFile();
	static BOOL IsVoip97CallMedia(DATEHEADER& packh, const char* pPacket)
	{
		char  Flag97CALL[] = {0x4b,0x53,0x00,0x00,0x00,0x01};
		if( ( (packh.m_dport == 5060 ) || ( packh.m_dport == 5070 ) ) && (memcmp(pPacket,Flag97CALL,6) == 0) )
		{
			if( (*(unsigned short*)(pPacket + 8) == 0x0b00) || (*(unsigned short*)(pPacket + 8) == 0x0d00) )
			{
				return TRUE;
			}
		}
		return FALSE;
	}	
protected:
	int	m_Packettype;
};







class VoipYltMediaSession : public MediaSession
{
public:
	VoipYltMediaSession(DATEHEADER& packh, int aT);
	virtual ~VoipYltMediaSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual int ListToFile();
	static BOOL IsVoipYltMedia(DATEHEADER& packh, const char* pPacket)
	{
		if( ( (packh.m_sport == 8080 ) || ( packh.m_sport == 15060 ) ) && (packh.m_type ==UDPPacket) )
		{
			if( (memcmp(pPacket,"INVITE sip:",strlen("INVITE sip:")) == 0) || 
				(memcmp(pPacket,"SIP/2.0 ",strlen("SIP/2.0 ")) == 0) )
			{
				return VOIP_CALL;
			}
		}
		return FALSE;
	}	
protected:
	int	m_Packettype;
};









class VoipHHTMediaSession : public MediaSession
{
public:
	VoipHHTMediaSession(DATEHEADER& packh, int aT);
	virtual ~VoipHHTMediaSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual int ListToFile();
	static int IsVoipHHTMedia(DATEHEADER& packh, const char * pPacket)
	{/*
		if( (packh.m_dport == 80) && (packh.m_type ==UDPPacket) )
		{
			return VOIP_CALL;
		}*/
		return 0;
	}
protected:
	int	m_Packettype;
};





class VoipRYBMediaSession : public MediaSession
{
public:
	VoipRYBMediaSession(DATEHEADER& packh, int aT);
	virtual ~VoipRYBMediaSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual int ListToFile();
	static int IsVoipRYBMedia(DATEHEADER& packh, const char* pPacket)
	{
		if( (packh.m_type ==UDPPacket) && ((packh.m_dport == 5060) || (packh.m_sport == 5060 ) ) )
		{
			if( (memcmp(pPacket,"INVITE sip:",strlen("INVITE sip:")) == 0) || 
				(memcmp(pPacket,"SIP/2.0 ",strlen("SIP/2.0 ")) == 0) )
			{
				return VOIP_CALL;
			}
		}
		return FALSE;	
	}
protected:
	int	m_Packettype;
};



//手机版云呼号码的提取与电脑版爱丁玲相同
class VoipADLMediaSession : public MediaSession
{
public:
	VoipADLMediaSession(DATEHEADER& packh, int aT);
	virtual ~VoipADLMediaSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual int ListToFile();
	static int IsVoipADLMedia(DATEHEADER& packh, const char* pPacket)
	{
		if( ( (packh.m_sport == 6060 ) || ( packh.m_dport == 6060 ) ) && (packh.m_type ==UDPPacket) )
		{
			if( (memcmp(pPacket,"INVITE sip:",strlen("INVITE sip:")) == 0) || 
				(memcmp(pPacket,"SIP/2.0 ",strlen("SIP/2.0 ")) == 0) )
			{
				return VOIP_CALL;
			}
		}
		return 0;
	}
protected:
	int	m_Packettype;
};





//手机版云呼号码的提取与电脑版爱丁玲相同
class VoipDuoLiaoMediaSession : public MediaSession
{
public:
	VoipDuoLiaoMediaSession(DATEHEADER& packh, int aT);
	virtual ~VoipDuoLiaoMediaSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual int ListToFile();
	static int IsVoipDuoLiaoMedia(DATEHEADER& packh, const char* pPacket)
	{
		if( ( (packh.m_sport == 5068 ) || ( packh.m_dport == 5068 ) ) && (packh.m_type ==UDPPacket) )
		{
			if( (memcmp(pPacket,"INVITE sip:",strlen("INVITE sip:")) == 0) || 
				(memcmp(pPacket,"SIP/2.0 ",strlen("SIP/2.0 ")) == 0) )
			{
				return VOIP_CALL;
			}
		}
		return 0;
	}
protected:
	int	m_Packettype;
};
#endif

