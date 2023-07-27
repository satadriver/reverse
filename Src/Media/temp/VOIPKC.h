//80C8--->dial telephone number,81C8--->telephone get through,81C9--->telephone hang up
//1.KC����绰��RTCP����Э����ȫ�������˷����������ƶ��绰�ı��ж˷�������������һ����Ҫ����
//��SKY��RTCP���������кͱ�������������
//2.KC��SKY��RTCP����һ��û��SOURCE��ֻ��һ��SENDER��SDES
//3.KC��SKY��RTCP���м�ͨ���е�ȫ������SENDER,SOURCE��SDES��ɵ�
//4.KC��SKY��RTCP�����һ������RECEIVER,SOURCE��GOODBYE���
//5.�ڲ��Ŷ������������͵Ĳ�����ʾ������������һ��RTCP��
//6.SENDER����RECEIVER����SDES����ͷ����ʽ��ͬ����ͷ����V,P,CC,PT,LENTH,SSRC

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