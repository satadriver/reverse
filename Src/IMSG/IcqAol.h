//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	IcqAol
//	版本:		1.0
//	创建日期:	2007-7-12
//	作者:		荣强
//	主要功能:	ICQ/AOL协议还原
//**************************************************************** 

#ifndef INCLUDE_AFX_YAHOO_H_20070712
#define INCLUDE_AFX_YAHOO_H_20070712

#if _MSC_VER > 1000
#pragma once
#endif

//**********************************************************
//	结构定义
//**********************************************************
// struct ICQHD
// {
// 	char			flag[4];
// 	int				Version;
// //	unsigned char	Pklen[2];
// 	WORD			Pklen;
// //	unsigned char	Service[2];
// 	WORD			Service;
// //	unsigned char	Stat[4];
// 	DWORD			Stat;
// 	char			Sessionid[4];
// };
// 
// struct ICQ
// {
// public:
// //	YMSGHD	hd;
// 	unsigned short Pklen;
// 	unsigned short Service;
// 	unsigned short Stat;
// 	LPCTSTR Code[101];
// 	LPCTSTR strContent[101];
// 	CHAR stait[10];
// 	ICQ()
// 	{
// 		memset(this, 0, sizeof(YMSG));
// 	}
// };
// 
// typedef struct _IMSG_STRUCT_
// {
// 	int			Direction;		//消息的方向，1为发，2为收,3为未知
// 	CString		Sender;			//发送者，如果为空则从socket上找
// 	LPCTSTR		SenderIP;		//发送者IP
// 	CString		Acceptant;		//接受着
// 	LPCTSTR		AcceptantIP;	//接受着IP
// 	CString		Msg;			//消息内容
// 	//	CString		MimeVersion;
// 	//	CString		ConnectType;
// 	//	CString		CharSet;
// 	//	CString		MsgSeq;
// 	//	CString		MsgType;
// 	//	CString		MsgLen;
// }ImsgMsg,*LPImsgMsg;

//**********************************************************
//	变量定义
//**********************************************************


//**********************************************************
//	函数定义
//**********************************************************
// BOOL ProcessOneBlock(MsnSession *&s);
// 
// BOOL FormatYmsg(YMSG* Ymsg, char* packetdata, int len);
// 
// BOOL GetYahooMsg(YMSG *pYmsg, ImsgMsg* pImsg, LPCTSTR sip, LPCTSTR dip);
// 
// LPCTSTR GetStrByKey(LPCTSTR strkey,YMSG *ymsg, int keylen);
// 
// CString GetAllStrByKey(LPCTSTR strkey,YMSG *ymsg, int keylen);
// 
// void WriteYmsgTmpFile(ImsgSession* s);

//*************************************************
//	类名称:			IcqAolSession
//	类继承:       	SocketSession
//	描述:			ICQ/AOL协议会话类
//	主要函数:		
//	使用事项:		
//**************************************************
class IcqAolSession : public SocketSession
{
public:
	IcqAolSession(DATEHEADER& packh, int aT);
	virtual ~IcqAolSession();

	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);

	virtual BOOL CloseSession(time_t nowtm);

	time_t			m_ProcessTime;
	DWORD			m_processseq1;
	DWORD			m_processseq2;
	BOOL			m_bSortList;
	DTcpPacketList	m_PacketList;
	CString			m_pass;
	
	char*			m_MsgBuf;
	int				m_MsgBufLen;
public:

	// 处理链表中的包
	void ProcessIcqList(time_t now);
	
	// 写实体文件
	void WriteRealFile(char* pData, int datalen);

	// 处理一个单包
	int ProcessIcqPacket(time_t optm, char* pdata, int datalen,
		int direction, char* pmsgbuf, int msgbuflen);

};
#endif	//INCLUDE_AFX_YAHOO_H_20070712