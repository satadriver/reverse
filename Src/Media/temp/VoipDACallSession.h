#ifndef VOIP_DACALL_SESSION_H_H_H
#define VOIP_DACALL_SESSION_H_H_H


class VoipDACallSession : public MediaSession
{
public:
	VoipDACallSession(DATEHEADER& packh, int aT);
	virtual ~VoipDACallSession();
	
	virtual int InsertPacket(DATEHEADER & PktInfo,const char * Packet);
	virtual int ListToFile();
	static BOOL IsVoipDACall(DATEHEADER& dataheader,const char *pPacket);
private:
	MediaPacketList m_packetlist;
	int m_Packettype;
	unsigned int m_decode;
	unsigned int m_flag;
	unsigned short m_packet_idx;
	unsigned int m_packet_time;
	unsigned int m_packet_unique;
};

#endif