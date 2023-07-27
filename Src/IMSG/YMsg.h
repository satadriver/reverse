//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	YMsg
//	�汾:		1.0
//	��������:	2007-7-13
//	����:		��ǿ
//	��Ҫ����:	�Ż�����Э�黹ԭ
//**************************************************************** 
#ifndef		INCLUDE_AFX_YMSG_H_20070713_INCLUDED
#define		INCLUDE_AFX_YMSG_H_20070713_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif

//**********************************************************
//	��������
//**********************************************************
#define		YAHOO_SERVICE_LOGON  			1                                           
#define		YAHOO_SERVICE_LOGOFF 			2                                           
#define		YAHOO_SERVICE_ISAWAY 			3                                           
#define		YAHOO_SERVICE_ISBACK 			4                                           
#define		YAHOO_SERVICE_IDLE   			5	/* 5 (placemarker) */               
#define		YAHOO_SERVICE_MESSAGE 			6                                           
#define		YAHOO_SERVICE_IDACT  			7                                           
#define		YAHOO_SERVICE_IDDEACT  			8                                           
#define		YAHOO_SERVICE_MAILSTAT			9                                           
#define		YAHOO_SERVICE_USERSTAT			10/* 0xa */                                 
#define		YAHOO_SERVICE_NEWMAIL			11                                          
#define		YAHOO_SERVICE_CHATINVITE		12                                          
#define		YAHOO_SERVICE_CALENDAR			13                                          
#define		YAHOO_SERVICE_NEWPERSONALMAIL	14                                          
#define		YAHOO_SERVICE_NEWCONTACT       	15                                          
#define		YAHOO_SERVICE_ADDIDENT 			16/* 0x10 */                                
#define		YAHOO_SERVICE_ADDIGNORE         17                                          
#define		YAHOO_SERVICE_PING              18                                          
#define		YAHOO_SERVICE_GOTGROUPRENAME 	19/* < 1 36(old) 37(new) */                 
#define		YAHOO_SERVICE_SYSMESSAGE  		0x14                                        
#define		YAHOO_SERVICE_PASSTHROUGH2  	0x16                                        
#define		YAHOO_SERVICE_CONFINVITE  		0x18                                        
#define		YAHOO_SERVICE_CONFLOGON    		0x19                                        
#define		YAHOO_SERVICE_CONFDECLINE       0x1A                                        
#define		YAHOO_SERVICE_CONFLOGOFF        0x1B                                        
#define		YAHOO_SERVICE_CONFADDINVITE     0x1C                                        
#define		YAHOO_SERVICE_CONFMSG           0x1D                                        
#define		YAHOO_SERVICE_CHATLOGON         0x1E                                        
#define		YAHOO_SERVICE_CHATLOGOFF        0x1F                                        
#define		YAHOO_SERVICE_CHATMSG  			0x20                                        
#define		YAHOO_SERVICE_GAMELOGON  		0x28                                        
#define		YAHOO_SERVICE_GAMELOGOFF 		0x29                                        
#define		YAHOO_SERVICE_GAMEMSG  			0x2a                                        
#define		YAHOO_SERVICE_FILETRANSFER  	0x46                                        
#define		YAHOO_SERVICE_VOICECHAT  		0x4A                                        
#define		YAHOO_SERVICE_NOTIFY            0x4B                                        
#define		YAHOO_SERVICE_VERIFY            0x4C                                        
#define		YAHOO_SERVICE_P2PFILEXFER       0x4D                                        
#define		YAHOO_SERVICE_PEERTOPEER  		0x4F        /* Checks if P2P possible */    
#define		YAHOO_SERVICE_AUTHRESP  		0x54                                        
#define		YAHOO_SERVICE_LIST              0x55                                        
#define		YAHOO_SERVICE_AUTH  			0x57                                        
#define		YAHOO_SERVICE_ADDBUDDY  		0x83                                        
#define		YAHOO_SERVICE_REMBUDDY			0x84                                        
#define		YAHOO_SERVICE_IGNORECONTACT		0x85 /* > 1 7 13 < 1 66 13 0*/              
#define		YAHOO_SERVICE_REJECTCONTACT		0x86                                        
#define		YAHOO_SERVICE_GROUPRENAME  		0x89 /* > 1 65(new) 66(0) 67(old) */        
#define		YAHOO_SERVICE_CHATONLINE  		0x96 /* > 109(id) 1 6(abcde) < 01*/         
#define		YAHOO_SERVICE_CHATGOTO			0x97                                        
#define		YAHOO_SERVICE_CHATJOIN 			0x98 /* > 1 104-room 129-1600326591 62-2 */ 
#define		YAHOO_SERVICE_CHATLEAVE			0x99                                        
#define		YAHOO_SERVICE_CHATEXIT  		0x9b                                        
#define		YAHOO_SERVICE_CHATLOGOUT  		0xa0                                        
#define		YAHOO_SERVICE_CHATPING			0xa1                                        
#define		YAHOO_SERVICE_COMMENT  			0xa8                                        
	
#define		YAHOO_STATUS_AVAILABLE			0                           
#define		YAHOO_STATUS_BRB				1                           
#define		YAHOO_STATUS_BUSY				2                           
#define		YAHOO_STATUS_NOTATHOME			3                           
#define		YAHOO_STATUS_NOTATDESK			4                           
#define		YAHOO_STATUS_NOTINOFFICE		5                           
#define		YAHOO_STATUS_ONPHONE			6                           
#define		YAHOO_STATUS_ONVACATION			7                           
#define		YAHOO_STATUS_OUTTOLUNCH			8                           
#define		YAHOO_STATUS_STEPPEDOUT			8                           
#define		YAHOO_STATUS_INVISIBLE			12                          
#define		YAHOO_STATUS_CUSTOM  			99                          
#define		YAHOO_STATUS_IDLE 				999                        
#define		YAHOO_STATUS_OFFLINE			0x5a55aa56 /* don't ask */ 
#define		YAHOO_STATUS_TYPING				0x16                        
	
//**********************************************************
//	�ṹ����
//**********************************************************
struct YMSGHD
{
	char			flag[4];
	int				Version;
	WORD			Pklen;
	WORD			Service;
	DWORD			Stat;
	char			Sessionid[4];
};

struct YMSG
{
public:
	unsigned short Pklen;
	unsigned short Service;
	unsigned short Stat;
	DWORD		stait;
	LPCTSTR Code[101];
	LPCTSTR strContent[101];
	YMSG()
	{
		memset(this, 0, sizeof(YMSG));
	}
};

typedef struct _IMSG_STRUCT_
{
	int			Direction;		//��Ϣ�ķ���1Ϊ����2Ϊ��,3Ϊδ֪
	CString		Sender;			//�����ߣ����Ϊ�����socket����
//	LPCTSTR		SenderIP;		//������IP
	CString		Acceptant;		//������
//	LPCTSTR		AcceptantIP;	//������IP
	CString		Msg;			//��Ϣ����
	CString		fname;			//�����ļ�ʱ���ļ���
}ImsgMsg, *LPImsgMsg;


//*************************************************
//	������:			YmsgSession
//	��̳�:       	SocketSession
//	����:			�Ż�����Э��ĻỰ��
//	��Ҫ����:		
//	ʹ������:		
//**************************************************
class YmsgSession : public SocketSession
{
public:
	YmsgSession(DATEHEADER& packh, int aT);

	YmsgSession();

	virtual ~YmsgSession();

	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);

	virtual BOOL CloseSession(time_t nowtm);

	BOOL ProcessYmsgPacket(IMSGPARA* pPara);

public:
	int				m_direction;
	time_t			m_ProcessTime;
	DWORD			m_processseq1;
	DWORD			m_processseq2;
	BOOL			m_bSortList;
	DTcpPacketList	m_PacketList;
	
	char*			m_MsgBuf;
	int				m_MsgBufLen;
	CString			m_Linkman;		// ��ϵ���б� \r\nlinkman1\r\nlinkman2...
protected:

	// ���������еİ�
	void ProcessYmsgList(time_t now);
	
	// дʵ���ļ�
	void WriteRealFile(char* pData, int datalen);

	// д���ļ�����
	void WriteTransFileIndexFile(ImsgMsg *pImsg);



	// ����һ������
// 	BOOL ProcessYmsgPacket(time_t optm, char* pdata, int datalen,
// 		int direction, char* pmsgbuf, int& msgbuflen, CStringArray& hisarr, BOOL& bWIdcSvr);

};

#endif