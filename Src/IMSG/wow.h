//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	wow
//	版本:		1.0
//	创建日期:	2009-4-14
//	作者:		荣强
//	主要功能:	魔兽世界游戏的登录还原
//	模块描述:	
//	主要函数:	IsWoWPacket
//**************************************************************** 

#ifndef	INCLUDE_WOW_H_20090414
#define INCLUDE_WOW_H_20090414

#if _MSC_VER > 1000
#pragma once
#endif

//*************************************************
//	类名称:			WowSession
//	类继承:			SocketSession
//	描述:			魔兽世界游戏的处理类
//	主要函数:		InsertPacket
//	使用事项:		
//**************************************************
class WowSession : public SocketSession
{
public:
	WowSession(DATEHEADER& packh, int aT);
	virtual ~WowSession();

	//*************************************************
	//	函数名称:		BfSession::InsertPacket
	//	描述:			处理一个会话的一个包
	//	输入参数:		packh 是包头结构
	//					pPacket 是包数据
	//	输出参数:		
	//	返回值:			
	//	其它:			
	//**************************************************
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);

protected:
	void WriteWowIndexFile();

	// 从登录的包中获取登录ID (成功返回TRUE, 失败返回FALSE)
	BOOL GetLoginUserid(const char* pPacket, int len);

protected:
	DWORD			m_state;			// 1 表示获得了登录请求包
										// 2 表示获得了登录状态包
										// 4 表示已经输出了对照文件
};


//*************************************************
//	函数名称:		IsWoWPacket
//	描述:			检查一个包是否是魔兽世界的包
//	输入参数:		packh 是包头结构
//					pPacket 是包数据
//	输出参数:		
//	返回值:			如果是魔兽世界的包就返回TRUE, 否则返回FALSE
//	其它:			
//*************************************************
inline int IsWoWPacket(DATEHEADER& packh, const char* pPacket)
{
	if ((packh.m_type ==TCPPacket) || (packh.m_type==ProxyTCP))
	{
		WORD* tp = (WORD*)(pPacket+2);
		if ((packh.m_datalen > 34) && (*tp == packh.m_datalen-4)
			&& (pPacket[33]+34 == packh.m_datalen)
			&& (memcmp(pPacket, "\x0\x8", 2) == 0)
			&& (memcmp(pPacket+4, "WoW\0", 4) == 0))
		{
			return 1;	// 是登录的包
		}
// 		if((packh.m_datalen==4) && (memcmp(pPacket, "\1\4\3\0", 4) == 0))
// 		{
// 			return TRUE;	// 登录失败时返回的包
// 		}
		if((packh.m_datalen==5) && (memcmp(pPacket, "\x10\0\0\0\0", 5) == 0))
		{
			return 2;	// 登录成功时的包
		}
	}
	return 0;
}

// 标准端口3724
// 登录的包 Client --> Server(中括号内是userid UTF-8编码)
// 0030                     00 08 2a 00 57 6f 57 00 03 00        ..*.WoW...
// 0040   05 4e 24 36 38 78 00 6e 69 57 00 4e 43 68 7a e0  .N$68x.niW.NChz.
// 0050   01 00 00 c0 a8 00 02 0c[e6 88 91 e7 9a 84 e8 b4  ................
// 0060   a6 e5 8f b7]                                      ....

// 0030                     00 08 26 00 57 6f 57 00 03 00        ..&.WoW...
// 0040   05 4e 24 36 38 78 00 6e 69 57 00 4e 43 68 7a e0  .N$68x.niW.NChz.
// 0050   01 00 00 c0 a8 00 02 08[53 49 4e 41 54 54 4d 50] ........SINATTMP

// 字节0-1固定为"00 08" 可作为特征
// 字节2-3为包数据的长度
// 字节4-7固定为"WoW\0" 可作为特征
// 字节33 userid的长度
// 字节33- userid(即中括号内的部分)

// 如果登录失败就返回"\1\4\3\0" 4Byte Client <-- Server
// 0030                     01 04 03 00 00 00 00 00               ....

// 如果登录成功会有一个包("\x10\0\0\0\0") 5Byte Client --> Server
// 0030                     10 00 00 00 00                        .....

#endif