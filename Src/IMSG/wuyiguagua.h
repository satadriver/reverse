// wuyiguagua.h: interface for the wuyiguagua class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WUYIGUAGUA_H__EC8ECD3E_BE92_4D0B_B009_401173A8C436__INCLUDED_)
#define AFX_WUYIGUAGUA_H__EC8ECD3E_BE92_4D0B_B009_401173A8C436__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//*************************************************
//	类名称:			VPIMSession
//	类继承:       	SocketSession
//	描述:			VP聊天的还原类
//	主要函数:		
//	使用事项:		
//**************************************************
class WuyiguaguaSession : public SocketSession
{
public:
	WuyiguaguaSession(DATEHEADER& packh, int aT);
	virtual ~WuyiguaguaSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
public:
	int			m_direction;		// 会话方向标志
	BOOL		m_bIdc;				// 是否匹配过数据源
	CString     m_roomid;			// 房间号
	char*		m_pMsgBuf;			// 聊天内容缓存
	int			m_MsgBuflen;		
private:

	// 处理VP的聊天
	int FormatVpIM(DATEHEADER& packh, const char* pPacket);

	// 处理用户登录的包
	int UserLogin(DATEHEADER& packh, const char* pPacket);

	// 获得聊天信息
	CString GetImsg(const char* pPacket);

	// 写实体文件
	void WriteRealFile(char* pData, int datalen);
};

//*************************************************
//	函数名称:		IsWyggPacket
//	描述:			检查一个包是否是IsWyggPacket的包
//	输入参数:		packh 是包头结构
//					pPacket 是包数据
//	输出参数:		
//	返回值:			如果是VP的包就返回TRUE, 否则返回FALSE
//	其它:			
//**************************************************
inline BOOL IsWyggPacket(DATEHEADER& packh, const char* pPacket)
{
	if (packh.m_datalen > 28)
	{
		if (memcmp(pPacket,"\x90\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",12)==0 &&
			memcmp(pPacket+15,"\x00\x04",0)==0)
		{
			return TRUE;//上线
		}
		else if (memcmp(pPacket,"\x00\x00\x00\x00\x00\x00\x0a",7)==0&&
			memcmp(pPacket+15,"\x00\x0b",0)==0)
		{
			return TRUE;//上线
		}
		if (stricmp(pPacket+12, "ClientText") == 0)		// 聊天信息
			return TRUE;
		if (stricmp(pPacket+12, "SystemMessage") == 0)	// 系统消息 (用于区分客户端)
			return TRUE;
		if (stricmp(pPacket+12, "VerifyUser") == 0)		// 登陆 
			return TRUE;
	}
	return FALSE;
}

#endif // !defined(AFX_WUYIGUAGUA_H__EC8ECD3E_BE92_4D0B_B009_401173A8C436__INCLUDED_)
