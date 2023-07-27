#ifndef VOIP_DYTCALL_SESSION_H_H_H
#define VOIP_DYTCALL_SESSION_H_H_H


class VoipDYTCallSession : public MediaSession
{
public:
	VoipDYTCallSession(DATEHEADER& packh, int aT);
	virtual ~VoipDYTCallSession();
	
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual int ListToFile();
	static BOOL IsVoipDYT(DATEHEADER& packh,const char *pPacket);
	int __stdcall DecodeDYT(unsigned char * EncodeBuf , unsigned char * MaskBuf , int EncodeBufSize);
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