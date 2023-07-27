
#ifndef VOIP_ALCALL_SESSION_H_H_H
#define VOIP_ALCALL_SESSION_H_H_H

class VoipALCallSession : public MediaSession
{
public:
	VoipALCallSession(DATEHEADER& packh, int aT);
	virtual ~VoipALCallSession();
	
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual int ListToFile();
	static BOOL IsVoipALICall(DATEHEADER& packh, const char *pPacket);
	int __stdcall DecodeALICALL(unsigned int * Packet,int PacketSize);
private:

	MediaPacketList m_packetlist;
	int m_Packettype;
	unsigned int m_decode;
	unsigned int m_flag;

	unsigned short m_packet_idx;
	unsigned int m_packet_time;
	unsigned int m_packet_unique;

	
	unsigned int m_callerip ;
	unsigned short m_callerport ;
	unsigned int m_calleeip ;
	unsigned short m_calleeport ;
};


#endif