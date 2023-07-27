
#ifndef VOIP_YLTCALL_SESSION_H_H_H
#define VOIP_YLTCALL_SESSION_H_H_H


class VoipYLTCallSession : public MediaSession
{
public:
	VoipYLTCallSession(DATEHEADER& packh, int aT);
	virtual ~VoipYLTCallSession();
	
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual int ListToFile();
	static BOOL IsVoipYLT(DATEHEADER& dataheader,const char *pPacket);
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