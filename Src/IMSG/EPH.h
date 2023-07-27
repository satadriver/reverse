//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	EPH
//	版本:		1.0
//	创建日期:	2007-7-11
//	作者:		荣强
//	主要功能:	E话通聊天的处理类
//	主要函数:	isEPHPacket
//**************************************************************** 

#ifndef		INCLUDE_EPH_H_20070711
#define		INCLUDE_EPH_H_20070711

#if _MSC_VER > 1000
#pragma once
#endif

class EphSession : public SocketSession
{
public:
	EphSession(DATEHEADER& packh, int aT);
	virtual ~EphSession();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
public:
	int				m_direction;
	DWORD			m_state;		// 状态1(已经得到了ID); 2(已经得到了昵称); 0x10(下线)
	CString			m_subip;		// 子网ip
	CString			m_Nick;			// 昵称
protected:

	//*************************************************
	//	函数名称:		EphSession::WriteEphmsgFile
	//	描述:			还原一个单包获得对象的活动状态(上下线)
	//	输入参数:		packh 是包头
	//					pData 是一个包的数据
	//	输出参数:		
	//	返回值:			有还原出的数据返回TRUE否则返回FALSE
	//	其它:			
	//**************************************************
	BOOL ProcessEphPacket(DATEHEADER& packh, const BYTE* pData);

	//*************************************************
	//	函数名称:		OutIndexFile
	//	描述:			输出一个DoShow的定位信息
	//	输入参数:		state 对象的活动状态
	//	输出参数:		
	//	返回值:			
	//	其它:			
	//*************************************************
	void OutIndexFile(LPCTSTR state);

};

//*************************************************
//	函数名称:		isEPHPacket
//	描述:			判断一个包是不是E话通的包
//	输入参数:		packh 是包头结构
//					pPacket 是包数据
//	输出参数:		
//	返回值:			如果是E话通的包就返回TRUE, 否则返回FALSE
//	其它:			
//**************************************************
inline BOOL isEPHPacket(DATEHEADER& packh, const char* pPacket)
{
	if(packh.m_datalen<0x10)
		return FALSE;
	if ((packh.m_type == UDPPacket) || (packh.m_type == ProxyUDP))
	{
//判登陆包
		if ((pPacket[0x0A] == 0) && (pPacket[0x14] == 0)
			&& (*(USHORT*)(pPacket + 0x0E) == 0x400)
			&& (*(USHORT*)(pPacket + 2) == 0))
		{
			return (memcmp(pPacket, "\x72\x20", 2) == 0);

		}
		else if ((pPacket[0x0A] == 0) && (pPacket[0x13] == 0)
			&& (*(USHORT*)(pPacket + 0x0E) == 0x500)
			&& (*(USHORT*)(pPacket + 2) == 0))
		{
			return (memcmp(pPacket, "\x72\x20", 2) == 0);
		}
	}

	return FALSE;
}


//////////////////////////////////////////////////////////////////////////


class ImsgEphSession : public SocketSession
{
public:
	ImsgEphSession(DATEHEADER& packh, int aT);
	virtual ~ImsgEphSession();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
 
public:
	int				m_direction;
	time_t			m_ProcessTime;
	int				m_PackCount;
	DWORD			m_processseq1;
	DWORD			m_processseq2;
	BOOL			m_bSortList;
	DTcpPacketList	m_PacketList;


	char*		m_pMsgBuf;			// 聊天内容缓存
	int			m_MsgBuflen;
	CString     m_Colloquist; //联系人

protected:

	//*************************************************
	//	函数名称:		EphSession::ProcessEPHList
	//	描述:			还原包链表中保存的包
	//	输入参数:		当前包的时间
	//	输出参数:		
	//	返回值:			
	//	其它:			
	//**************************************************
	int ProcessEPHList(time_t now);

	//*************************************************
	//	函数名称:		EphSession::WriteEphmsgFile
	//	描述:			还原一个单包并写实体文件
	//	输入参数:		Packet 是一个包
	//					msgbuf 是存放还原的信息的缓冲区
	//					int 是缓冲区的大小
	//	输出参数:		
	//	返回值:			返回还原的信息大小
	//	其它:			
	//**************************************************
	int ProcessEphPacket(DTcpPacket* Packet, char* msgbuf, int len);

	// 写实体文件
	void WriteRealFile(char* pData, int datalen);

};

//*************************************************
//	函数名称:		isEPHImsgPacket
//	描述:			判断一个包是不是E话通的包
//	输入参数:		packh 是包头结构
//					pPacket 是包数据
//	输出参数:		
//	返回值:			如果是E话通的包就返回TRUE, 否则返回FALSE
//	其它:			
//**************************************************
inline BOOL isEPHImsgPacket(DATEHEADER& packh, const char* pPacket)
{
	if(packh.m_datalen<0x10)
		return FALSE;
	if ((packh.m_type == UDPPacket) || (packh.m_type == ProxyUDP))
	{
//判登陆包
		if ((pPacket[0x0A] == 0) && (pPacket[0x13] == 0)
			&& memcmp(pPacket+0xe,"\x0\x85",2)==0)
			
		{
			return (memcmp(pPacket, "\x72\x00\x00", 3) == 0);

		}
		else if  ((pPacket[0x0A] == 0) && (pPacket[0x13] == 0)
			&& memcmp(pPacket+0xe,"\x80\x85",2)==0)
		{
			return (memcmp(pPacket, "\x72\x00\x00", 3) == 0);
		}
// 		else if  ((pPacket[0x0A] == 0) && (pPacket[0x13] == 0)
// 			&& memcmp(pPacket+0xe,"\x0\x85",2)==0)
// 		{
// 			return (memcmp(pPacket, "\x72\x00", 2) == 0);
// 		}
//判MSG
	}

	return FALSE;
}


#endif		//INCLUDE_EPH_H_20070711
