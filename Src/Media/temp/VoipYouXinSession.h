
#ifndef VOIP_YOUXIN_SESSION_H_H_H
#define VOIP_YOUXIN_SESSION_H_H_H


class VoipYouXinSession : public MediaSession
{
public:
	VoipYouXinSession(DATEHEADER& packh, int aT);
	virtual ~VoipYouXinSession();
	
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual int ListToFile();
	static BOOL IsVoipYouXin(DATEHEADER& dataheader,const char *pPacket);
private:
	MediaPacketList m_packetlist;
	int			m_Packettype;
	unsigned int m_decode; // Ω‚¬Î±Í÷æ
	unsigned int m_flag; // 
	unsigned short m_packet_idx;
	unsigned int m_packet_time;
	unsigned int m_packet_unique;
};
#endif