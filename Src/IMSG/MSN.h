//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	MSN
//	版本:		1.0
//	创建日期:	2007-7-16
//	作者:		荣强
//	主要功能:	MSN即时消息的处理
//	模块描述:	
//**************************************************************** 

#ifndef	INCLUDE_MSN_H_20070716
#define INCLUDE_MSN_H_20070716

#if _MSC_VER > 1000
#pragma once
#endif


typedef struct _MSN_MSG_STRUCT_
{
	int			Direction;		// 消息的方向，1为发，2为收,3为未知
	LPCTSTR		language;		// 消息文本的字符集
	CString		Sender;			// 发送者，如果为空则从socket上找
	CString		Acceptant;		// 接受着
	CString		Msg;			// 消息内容
	CString		fname;			// 收发文件操作时的文件名
	time_t		sendtm;			// 发送时间
	DWORD		operation;		// 操作 1: 上线  2: 聊天  3: 其他
}MSNMSG,*LPMSNMSG;

//*************************************************
//	类名称:			MsnSession
//	类继承:			SocketSession
//	描述:			MSN即时消息的处理类
//	主要函数:		InsertPacket
//	使用事项:		
//**************************************************
class MsnSession : public SocketSession
{
public:
	MsnSession(DATEHEADER& packh, int aT);
	MsnSession();
	virtual ~MsnSession();

	//*************************************************
	//	函数名称:		MsnSession::InsertPacket
	//	描述:			处理一个会话的一个包
	//	输入参数:		packh 是包头结构
	//					pPacket 是包数据
	//	输出参数:		
	//	返回值:			
	//	其它:			
	//**************************************************
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);

	virtual BOOL CloseSession(time_t nowtm);

public:
//	void WriteMsnRealFile(time_t optm);

	int				m_direction;
	int				m_language;
	BOOL			m_bRecord;
	char*			m_pMsgBuf;
	int				m_MsgBufLen;
// 	CString			m_userid;
	CString			m_Colloquist;
	CString			m_ownDeKey;
	CString			m_othDeKey;
	time_t			m_ProcessTime;
//	int				m_packcount;
	DWORD			m_SeqOffset;
	DWORD			m_AckOffset;
	DWORD			m_processseq1;
	DWORD			m_processseq2;
	BOOL			m_bSort1;
	BOOL			m_bSort2;
	TcpPacketList	m_packetlist1;
	TcpPacketList	m_packetlist2;

protected:
	int ProcessMsnPacket(time_t packtm);

	int ProcessMsnList(TcpPacketList& packlist,
		DWORD& processseq,
		IMSGPARA* pPara);

	void WriteRealFile(LPCTSTR pData, int datalen);

public:
	void WriteTransFileIndexFile(MSNMSG *pImsg);

};

//*************************************************
//	函数名称:		ProcessMsnData
//	描述:			处理一块MSN的数据
//	输入参数:		pMsnSion 是MSN会话指针
//					pPara 参数结构
//	输出参数:		
//	返回值:			处理的字节数
//	其它:			
//**************************************************
int	ProcessMsnData(MsnSession* pMsnSion,
				   IMSGPARA* pPara);
int ProcessMsnTransfile(const char *pbuf, int datalen, MSNMSG *msg);

inline int IsMsnTranfilePacket(DATEHEADER& packh, const char* pPacket)
{
	if((memcmp(pPacket+0x10, "INVITE MSNMSGR", 14) == 0)
		|| (memcmp(pPacket+0x1C, "INVITE MSNMSGR", 14) == 0)
 			|| (memcmp(pPacket+0x20, "INVITE MSNMSGR", 14) == 0)
			)
	{
		return 1;
	}
	return 0;
}
inline int IsMsnPacket(DATEHEADER& packh, const char* pPacket)
{
	if (memcmp(pPacket,"VER",3)==0	||
		memcmp(pPacket,"CVR",3)==0	||
		memcmp(pPacket,"USR",3)==0	||
		memcmp(pPacket,"XFR",3)==0	||
		memcmp(pPacket,"BLP",3)==0	||
		memcmp(pPacket,"BPR",3)==0	||
		memcmp(pPacket,"GTC",3)==0	||
		memcmp(pPacket,"ILN",3)==0	||
		memcmp(pPacket,"LSG",3)==0	||
		memcmp(pPacket,"LST",3)==0	||
		memcmp(pPacket,"MSG",3)==0	||
		memcmp(pPacket,"PRP",3)==0	||
		memcmp(pPacket,"ADL",3)==0	||
		memcmp(pPacket,"ADC",3)==0	||
		memcmp(pPacket,"ADD",3)==0	||
		memcmp(pPacket,"ADG",3)==0	||
		memcmp(pPacket,"CHG",3)==0	||
		memcmp(pPacket,"GCF",3)==0	||
		memcmp(pPacket,"OUT",3)==0	||
		memcmp(pPacket,"PNG",3)==0	||
		memcmp(pPacket,"QNG",3)==0	||
		memcmp(pPacket,"QRY",3)==0	||
		memcmp(pPacket,"SBS",3)==0	||
		memcmp(pPacket,"SYN",3)==0	||
		memcmp(pPacket,"REA",3)==0	||
		memcmp(pPacket,"REG",3)==0	||
		memcmp(pPacket,"REM",3)==0	||
		memcmp(pPacket,"RMG",3)==0	||
		memcmp(pPacket,"XFR",3)==0	||
		memcmp(pPacket,"UBX",3)==0	||
		memcmp(pPacket,"CHL",3)==0	||
		memcmp(pPacket,"FLN",3)==0	||
		memcmp(pPacket,"NLN",3)==0	||
		memcmp(pPacket,"RNG",3)==0	||
		memcmp(pPacket,"ANS",3)==0	||
		memcmp(pPacket,"IRO",3)==0	||
		memcmp(pPacket,"USR",3)==0	||
		memcmp(pPacket,"CAL",3)==0	||
		memcmp(pPacket,"JOI",3)==0	||
		memcmp(pPacket,"MSG",3)==0	||
		memcmp(pPacket,"BYE",3)==0	||
		memcmp(pPacket,"OUT",3)==0	||
		memcmp(pPacket,"SDG",3)==0)
	{
		return 1;
	}
	return 0;
}

// 1．Logon/Dispatch server
// 下面这些命令在连接到Dispatch服务器或Notification服务器时被发送和接收：
// VER - Protocol version
// CVR - Sends version information
// USR - Authentication command
// XFR - Redirection to Notification server
// 2．Notification Server
// 下面这些命令在请求联机以后，通过Dispatch服务器或Notification服务器被发送和接收。
// 联系人列表/设置/初始化同步命令：
// BLP - Initial settings download
// BPR - Initial settings download
// GTC - Initial contact list/settings download
// ILN - Initial contact presence notification
// LSG - Initial contact list download - Groups
// LST - Initial contact list download - Contacts
// MSG - Initial profile download
// PRP - Initial settings download - Mobile settings and display name
// 标准的发送/接收命令
// ADL - Add users to your contact lists
// ADC - Add users to your contact lists (deprecated as of MSNP13)
// ADD - Add users to your contact lists (deprecated as of MSNP11)
// ADG - Create groups
// CHG - Change client’s online status
// GCF - Unknown
// OUT - Gracefully logout
// PNG - Client ping
// QNG - Server response to PNG
// QRY - Response to CHL by client
// SBS - Unknown
// SYN - Begin synchronization/download contact list
// REA - Change display name
// REG - Rename groups
// REM - Remove contacts
// RMG - Remove groups
// XFR - Opens new chat session on switchboard server
// UBX - Inform you with a user PSM/Media
// 异步命令：
// CHL - Client challenge (see MSNP8:Challenges)
// FLN - Principal signed off
// NLN - Principal changed presence/signed on
// RNG - Client invited to chat session
// 3．Switchboard Server
// 通过Notification服务器请求一个连接到Switchboard服务器的会话后，下面的这些命令将会被发送和接收：
// ANS - Log in to switchboard chat session using invitation
// IRO - Defines which principals are in the current chat session
// USR - Log in to switchboard chat session after requesting session from NS
// CAL - Invite a user to a chat session
// JOI - Response to CAL, when user connected successfully
// MSG - Used to send and receive messages in the chat session
// BYE - Contact has left conversation
// OUT - Gracefully leave switchboard chat session
// 命令详细内容查询网址：
// http://msnpiki.msnfanatic.com/index.php/Reference:Commands
#endif