// Fetion.h: interface for the Fetion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FETION_H__3C2F3FDE_2EE1_4D83_97E2_0B67AE7D5F6E__INCLUDED_)
#define AFX_FETION_H__3C2F3FDE_2EE1_4D83_97E2_0B67AE7D5F6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\\ReadDDeFile.h"
#include "..\\SocketSession.h"
#include "..\\DceMission.h"

class Fetion : public SocketSession
{
public:
	Fetion(DATEHEADER& packh, int aT);
	virtual ~Fetion();
public:
	void WriteRealFile(LPCTSTR pData, int datalen);
	BOOL CreateFilename();

};

class FetionSession : public SocketSession
{
public:
	FetionSession(DATEHEADER& packh, int aT);
	virtual ~FetionSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
public:
	int			m_direction;		// 会话方向标志
	BOOL		m_bIdc;				// 是否匹配过数据源
	CString     m_roomid;			// 房间号
	char*		m_pMsgBuf;			// 聊天内容缓存
	int			m_MsgBuflen;		
	CString		m_fxcontentid;		// 接收者为fetion.com.cn时的实际接收者
private:

// 	// 处理VP的聊天
// 	int FormatVpIM(DATEHEADER& packh, const char* pPacket);
// 
// 	// 处理用户登录的包
// 	int UserLogin(DATEHEADER& packh, const char* pPacket);
// 
// 	// 获得聊天信息
// 	CString GetImsg(const char* pPacket);
	BOOL FormatFriendList(LPCTSTR cchString, int lenString, CString &strGbk);

	// 写实体文件
	void WriteRealFile(char* pData, int datalen);
};

class MebileFetionSession : public SocketSession
{
public:
	MebileFetionSession(DATEHEADER& packh, int aT);
	virtual ~MebileFetionSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
public:
	int			m_direction;		// 会话方向标志
	BOOL		m_bIdc;				// 是否匹配过数据源
	CString     m_roomid;			// 房间号
	char*		m_pMsgBuf;			// 聊天内容缓存
	int			m_MsgBuflen;		
	CString		m_fxcontentid;		// 接收者为fetion.com.cn时的实际接收者
private:
	// 写实体文件
	void WriteRealFile(char* pData, int datalen);
};

BOOL isFetion(DATEHEADER& packh, const char* pPacket);
BOOL isMobileFethion(DATEHEADER& packh, const char* pPacket);
// BOOL MobileFethion_process(DATEHEADER& packh,const char *pPacket);

#endif // !defined(AFX_FETION_H__3C2F3FDE_2EE1_4D83_97E2_0B67AE7D5F6E__INCLUDED_)
