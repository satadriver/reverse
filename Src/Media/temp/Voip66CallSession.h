#ifndef VOIP_66CALL_SESSION_H_H_H
#define VOIP_66CALL_SESSION_H_H_H

class Voip66CallSession : public MediaSession
{
public:
	Voip66CallSession(DATEHEADER& packh, int aT);
	virtual ~Voip66CallSession();
	
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual int ListToFile();
	static BOOL IsVoip66Call(DATEHEADER& packh, const char *pPacket);
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