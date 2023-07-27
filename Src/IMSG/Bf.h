//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	Bf
//	版本:		1.0
//	创建日期:	2007-7-11
//	作者:		荣强
//	主要功能:	边锋游戏的处理
//	模块描述:	
//	主要函数:	IsBfPacket
//**************************************************************** 

#ifndef	INCLUDE_BF_H_20070711
#define INCLUDE_BF_H_20070711

#if _MSC_VER > 1000
#pragma once
#endif

//*************************************************
//	类名称:			BfSession
//	类继承:			SocketSession
//	描述:			边锋游戏的处理类
//	主要函数:		InsertPacket
//	使用事项:		
//**************************************************
class BfSession : public SocketSession
{
public:
	BfSession(DATEHEADER& packh, int aT);
	virtual ~BfSession();

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

public:
	void WriteBfIndexFile(time_t optm);
};


//*************************************************
//	函数名称:		IsBfPacket
//	描述:			检查一个包是否是边锋的包
//	输入参数:		packh 是包头结构
//					pPacket 是包数据
//	输出参数:		
//	返回值:			如果是边锋的包就返回TRUE, 否则返回FALSE
//	其它:			
//*************************************************
inline BOOL IsBfPacket(DATEHEADER& packh, const char* pPacket)
{
	if ((packh.m_datalen>50) && ((packh.m_type ==TCPPacket) || (packh.m_type==ProxyTCP)))
	{
		if ((memcmp(pPacket, "\x1\x0\x0\x0\x1\x0\x0\x0", 8) == 0)
			&& ((pPacket[14] > 0) && (pPacket[14] <= 24))
			&& ((pPacket[14+1 + pPacket[14]]) == 20))
		{
			return TRUE;
		}
	}
	return FALSE;
}

#endif