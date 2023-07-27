//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	OurGame
//	版本:		1.0
//	创建日期:	2007-7-12
//	作者:		荣强
//	主要功能:	联众聊天协议的还原
//	模块描述:	
//	主要函数:	
//
//**************************************************************** 

#ifndef		INCLUDE_AFX_OURGAME_H_20070712
#define		INCLUDE_AFX_OURGAME_H_20070712

#if _MSC_VER > 1000
#pragma once
#endif

//*************************************************
//	函数名称:		IsHfPacket
//	描述:			检查一个包是否是联众游戏的包
//	输入参数:		packh 是包头结构
//					pPacket 是包数据
//	输出参数:		
//	返回值:			如果是联众游戏的包就返回TRUE, 否则返回FALSE
//	其它:			
//**************************************************
inline BOOL IsOurgamePacket(DATEHEADER& packh, const char* pPacket)
{
	// 判断是否是联众游戏的登录包
	if ((packh.m_datalen > 4)
		&& ((memcmp(pPacket, "\x63\x06\0\0", 4) == 0)
			|| (memcmp(pPacket, "\x7d\0\0\0", 4) == 0)
			|| (memcmp(pPacket, "\x65\0\0\0", 4) == 0)))
	{
		// 客户端到服务器登录的包
		return TRUE;
	}
	
	// 判断是否是联众游戏聊天的包
	if ((packh.m_sport==2002 || packh.m_dport==2002) &&
		((packh.m_datalen>=68 && memcmp(pPacket, "\x8\x3", 2)==0) ||
		(packh.m_datalen>=684 && memcmp(pPacket, "\x5\x3", 2)==0)))
	{
		return TRUE;
	}

	return FALSE;
}



//*************************************************
//	类名称:			OurGameSession
//	类继承:       	SocketSession
//	描述:			联众聊天协议的会话类
//	主要函数:		
//	使用事项:		
//**************************************************
class OurGameSession : public SocketSession
{
public:
	OurGameSession(DATEHEADER& packh, int aT);
	
	~OurGameSession();

	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);

	virtual BOOL CloseSession(time_t nowtm);

public:
	time_t			m_ProcessTime;
	DWORD			m_processseq1;
	DWORD			m_processseq2;
	BOOL			m_bSortList;
	DTcpPacketList	m_PacketList;
	
	char*			m_MsgBuf;
	int				m_MsgBufLen;

	DWORD			m_loginack;			// 登录请求包的ACK(登录状态包的ACK>登录请求包的ACK+1204)就认为是登录成功
	DWORD			m_stateack;			// 登录状态包的ACK
	DWORD			m_state;			// 1 表示获得了登录请求包
										// 2 表示获得了登录状态包
										// 4 表示已经输出了对照文件
protected:

	// 处理链表中的包
	int ProcessOurGameList(time_t now);

	// 处理一个单包
	int ProcessOurGamePacket(DTcpPacket& pack, int direction);

	// 写实体文件
	void WriteRealFile(char* pData, int datalen);

	// 处理用户登录 (如果是登录包就返回TRUE)
	BOOL ProcessLocin(DATEHEADER& packh, const char *pPacket);

	// 提取用户的登录ID
	BOOL GetUserid(const char* pPacket, int len);

	// 输出登录信息文件
	void WriteLoginFile();

	// 校验包结构
	BOOL CheckPacket(const char* pPacket, int len);

};

#endif