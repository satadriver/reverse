
#ifndef VOIP_YUNTONG_SESSION_H_H_H
#define VOIP_YUNTONG_SESSION_H_H_H


class VoipYunTongSession : public MediaSession
{
public:
	VoipYunTongSession(DATEHEADER& packh, int aT);
	virtual ~VoipYunTongSession();
	
	virtual int InsertPacket(DATEHEADER& packh,const char *pPacket);
	virtual int ListToFile();
	static BOOL IsVoipYunTong(DATEHEADER& packh,const char *pPacket);
private:
	MediaPacketList m_packetlist;
	int			m_Packettype;


	unsigned int m_decode;
	unsigned int m_flag;

	unsigned short m_packet_idx;
	unsigned int m_packet_time;
	unsigned int m_packet_unique;
};
#endif




















