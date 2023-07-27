#if !defined(AFX_TERMINAL_H__B913CD31_96F9_45F7_8DFD_04E16CD23239__INCLUDED_)
#define AFX_TERMINAL_H__B913CD31_96F9_45F7_8DFD_04E16CD23239__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 100

BOOL IsTerminalPacket(DATEHEADER& DataHeader, const char* pPacket);


class TerminalSession : public SocketSession
{
public:
	TerminalSession(DATEHEADER& packh, int aT);
	virtual ~TerminalSession();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);

	// 关闭VNC会话
	// 如果返回TRUE表示该会话需要删除
	// 返回FALSE表示该会话需要保留
	virtual BOOL CloseSession(time_t nowtm);
public:
	int			m_sippackcount;
	int			m_dippackcount;
	int			m_direction;
	BOOL		m_bOutput;			// 表示是否已经输出文件

};


#endif // !defined(AFX_TERMINAL_H__B913CD31_96F9_45F7_8DFD_04E16CD23239__INCLUDED_)
