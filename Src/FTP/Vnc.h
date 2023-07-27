#if !defined(AFX_VNC_H__B913CD31_96F9_45F7_8DFD_04E16CD23239__INCLUDED_)
#define AFX_VNC_H__B913CD31_96F9_45F7_8DFD_04E16CD23239__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 100

BOOL IsVNCPacket(DATEHEADER& DataHeader, const char* pPacket);

typedef	struct VNCPACKET
{
	DWORD		m_seq;
	DWORD		m_ack;
}VNCPACKET;

class VncSession : public SocketSession
{
public:
	VncSession(DATEHEADER& packh, int aT);
	virtual ~VncSession();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);

	// 关闭VNC会话
	// 如果返回TRUE表示该会话需要删除
	// 返回FALSE表示该会话需要保留
	virtual BOOL CloseSession(time_t nowtm);
public:
	VNCPACKET	m_sippack[8];
	VNCPACKET	m_dippack[8];
	int			m_sippackcount;
	int			m_dippackcount;
	int			m_direction;
	BOOL		m_bOutput;			// 表示是否已经输出文件

};


#endif // !defined(AFX_VNC_H__B913CD31_96F9_45F7_8DFD_04E16CD23239__INCLUDED_)
