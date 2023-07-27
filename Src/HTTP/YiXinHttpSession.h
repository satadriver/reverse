#ifndef YIXINHTTPSISSION_H
#define YIXINHTTPSISSION_H

#if _MSC_VER > 1000
#pragma once
#endif

#define YIXIN_PC_FIRSTRECV		1
#define YIXIN_PC_FIRSTSEND		2

#define YIXIN_KEY_LEN			260
#define YIXIN_MAX_PACKET_SIZE	4096
#define YIXIN_BUDDYLIST_SIZE	4096

#define YIXIN_MSG				1
#define YIXIN_BUDDYLIST			2
#define YIXIN_LOGIN				3

class YiXinHttpSession:public SocketSession
{
public:
	YiXinHttpSession(DATEHEADER& stPackHeader, const char * lpcstrPacket);
	virtual ~YiXinHttpSession();
	char * YixinFindMemory(char Flag[], int FlagLen, char Address[], int Offset);
	int BuildDecryptKey(DATEHEADER& stPackHeader, const char * lpcstrPacket);
	int static sub_67FEF0(int a1, unsigned int a2, int a3, int a4);
	int static IsYiXinStartPacket(DATEHEADER& stPackHeader, const char * lpcstrPacket);
	int ProcessYiXinPacket(DATEHEADER& stPackHeader, const char * lpcstrPacket,HttpProtocol * pHttpPro);
	void WriteYiXinIndexFile(DATEHEADER & stPackHeader,HttpProtocol * pHttpPro);
	
public:
	CString m_cstrUserPhone;
	CString m_cstrUserNick;
	CString m_cstrNetEasyID;
	CString m_cstrMessage;
	
	CString m_cstrSendID;
	CString m_cstrRecvID;
	unsigned int m_uiMode;
	unsigned int m_uiState;


	unsigned int m_srcip;
	unsigned int m_dstip;
	unsigned int m_sport;
	unsigned int m_dport;

	unsigned int m_strYiXinDecryptKey[YIXIN_KEY_LEN];
	char m_strDecryptBuffer[YIXIN_MAX_PACKET_SIZE];
	char m_strEncryptBuffer[YIXIN_MAX_PACKET_SIZE];
	char m_cmpbuf[8];
	
};


#endif
