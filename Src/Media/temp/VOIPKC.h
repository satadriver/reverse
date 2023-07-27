//80C8--->dial telephone number,81C8--->telephone get through,81C9--->telephone hang up
//1.KC网络电话的RTCP控制协议完全由主机端发出而不是移动电话的被叫端发出，这是他的一个重要特征
//而SKY的RTCP包是由主叫和被叫轮流发出的
//2.KC和SKY的RTCP包第一个没有SOURCE，只有一个SENDER和SDES
//3.KC和SKY的RTCP包中间通话中的全部都是SENDER,SOURCE和SDES组成的
//4.KC和SKY的RTCP包最后一个包由RECEIVER,SOURCE和GOODBYE组成
//5.在拨号动作后主机发送的不是提示音而是首先是一个RTCP包
//6.SENDER包，RECEIVER包和SDES包的头部格式相同，开头都是V,P,CC,PT,LENTH,SSRC

#ifndef VOIP_KC_H_H_H
#define VOIP_KC_H_H_H


class VoipKCCallSession : public MediaSession
{
public:
	VoipKCCallSession(DATEHEADER& packh, int aT);
	virtual ~VoipKCCallSession();
	
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual int ListToFile();
	static BOOL IsVOIPKC(DATEHEADER& packh, const char *pPacket);

private:
	MediaPacketList m_packetlist;
//	typedef list<MEDIAPACKET, allocator<MEDIAPACKET> > MediaPacketList;
	int m_Packettype;
	unsigned int m_decode;
	unsigned int m_flag;
	unsigned short m_packet_idx;
	unsigned int m_packet_time;
	unsigned int m_packet_unique;
};
#endif