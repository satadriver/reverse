#if !defined(AFX_RADMIN_H__B913CD31_96F9_45F7_8DFD_04E16CD23239__INCLUDED_)
#define AFX_RADMIN_H__B913CD31_96F9_45F7_8DFD_04E16CD23239__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 100

BOOL DealL2TPVpn(DATEHEADER& packh, const char* pPacket, int PacketFrom);
BOOL DealPPTPVpn(DATEHEADER& packh, const char* pPacket, int PacketFrom);
BOOL IsRadminPacket(DATEHEADER& DataHeader, const char* pPacket);

BOOL Dealopenvpn(const unsigned char* pPacket, DATEHEADER& packh);
BOOL Dealopenssh(const unsigned char* pPacket, DATEHEADER& packh);

BOOL Deal360Mac(const unsigned char* pPacket, DATEHEADER& packh);



class RadminSession : public SocketSession
{
public:
	RadminSession(DATEHEADER& packh, int aT);
	virtual ~RadminSession();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);

	// �ر�VNC�Ự
	// �������TRUE��ʾ�ûỰ��Ҫɾ��
	// ����FALSE��ʾ�ûỰ��Ҫ����
	virtual BOOL CloseSession(time_t nowtm);
public:
	int			m_sippackcount;
	int			m_dippackcount;
	int			m_direction;
	BOOL		m_bOutput;			// ��ʾ�Ƿ��Ѿ�����ļ�

};

#endif // !defined(AFX_RADMIN_H__B913CD31_96F9_45F7_8DFD_04E16CD23239__INCLUDED_)
