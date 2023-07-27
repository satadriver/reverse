//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	IcqAol
//	�汾:		1.0
//	��������:	2007-7-12
//	����:		��ǿ
//	��Ҫ����:	ICQ/AOLЭ�黹ԭ
//**************************************************************** 

#ifndef INCLUDE_AFX_YAHOO_H_20070712
#define INCLUDE_AFX_YAHOO_H_20070712

#if _MSC_VER > 1000
#pragma once
#endif

//**********************************************************
//	�ṹ����
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
// 	int			Direction;		//��Ϣ�ķ���1Ϊ����2Ϊ��,3Ϊδ֪
// 	CString		Sender;			//�����ߣ����Ϊ�����socket����
// 	LPCTSTR		SenderIP;		//������IP
// 	CString		Acceptant;		//������
// 	LPCTSTR		AcceptantIP;	//������IP
// 	CString		Msg;			//��Ϣ����
// 	//	CString		MimeVersion;
// 	//	CString		ConnectType;
// 	//	CString		CharSet;
// 	//	CString		MsgSeq;
// 	//	CString		MsgType;
// 	//	CString		MsgLen;
// }ImsgMsg,*LPImsgMsg;

//**********************************************************
//	��������
//**********************************************************


//**********************************************************
//	��������
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
//	������:			IcqAolSession
//	��̳�:       	SocketSession
//	����:			ICQ/AOLЭ��Ự��
//	��Ҫ����:		
//	ʹ������:		
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

	// ���������еİ�
	void ProcessIcqList(time_t now);
	
	// дʵ���ļ�
	void WriteRealFile(char* pData, int datalen);

	// ����һ������
	int ProcessIcqPacket(time_t optm, char* pdata, int datalen,
		int direction, char* pmsgbuf, int msgbuflen);

};
#endif	//INCLUDE_AFX_YAHOO_H_20070712