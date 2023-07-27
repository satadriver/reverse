// NogSession.h: interface for the CNogSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NOGSESSION_H__A31A6318_62C2_429A_AAB1_4DAF6C64CEF4__INCLUDED_)
#define AFX_NOGSESSION_H__A31A6318_62C2_429A_AAB1_4DAF6C64CEF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CXorBuffer
{
public:
	CXorBuffer();

	BOOL HaveBegin(BOOL bCheckClient);
	BOOL AddBuf(BOOL bClient, int total, unsigned char *packet, int len);
	BOOL SetSeqAck(BOOL bClient, DWORD seq, DWORD ack);
	BOOL ResetBuf(BOOL bClient, int trimlen);
	BOOL CheckHaveOne(BOOL bClient, unsigned char **pdata);
	
public:
	DWORD c_ack;
	int c_havelen;
	unsigned char *c_oneBuf;//[0xffff];

	DWORD s_seq;
	int s_havelen;
	unsigned char *s_oneBuf;//[0xffff];
};

class COneGet200OK :public CObject
{
public:
	COneGet200OK()
	{
		bGetOK = FALSE;
		pGetData = NULL;
		haveGetlen = 0;
		mallocGetLen = 0;

		b200OK = FALSE;
		p200OKData = NULL;
		have200Len = 0;
		malloc200Len = 0;
	}
	~COneGet200OK()
	{
		if(pGetData)
			delete []pGetData;
		if(p200OKData)
			delete []p200OKData;
	}
	BOOL PutData(BOOL bClient, unsigned char *pData, int dataLen, BOOL bReset)
	{
		int tmpmalloc = 0;
		if(bClient)
		{
			if(pGetData == NULL)
			{
				mallocGetLen = dataLen;
				haveGetlen = dataLen;
				pGetData = new unsigned char[mallocGetLen+2];
				memcpy(pGetData, pData, dataLen);
			}
			else
			{
				if(bReset)
				{
					delete []pGetData;
					mallocGetLen = dataLen;
					haveGetlen = dataLen;
					pGetData = new unsigned char[mallocGetLen+2];
					memcpy(pGetData, pData, dataLen);
				}
				else
				{
					mallocGetLen += dataLen;
					unsigned char *tp = new unsigned char[mallocGetLen+2];
					memcpy(tp, pGetData, haveGetlen);
					memcpy(tp+haveGetlen, pData, dataLen);
					delete []pGetData;
					pGetData = tp;
				}
			}
		}
		else
		{
			if(p200OKData == NULL)
			{
				malloc200Len = dataLen;
				have200Len = dataLen;
				p200OKData = new unsigned char[malloc200Len+2];
				memcpy(p200OKData, pData, dataLen);
			}
			else
			{
				if(bReset)
				{
					delete []p200OKData;
					malloc200Len = dataLen;
					have200Len = dataLen;
					p200OKData = new unsigned char[malloc200Len+2];
					memcpy(p200OKData, pData, dataLen);
				}
				else
				{
					malloc200Len += dataLen;
					unsigned char *tp = new unsigned char[malloc200Len+2];
					memcpy(tp, p200OKData, have200Len);
					memcpy(tp+have200Len, pData, dataLen);
					delete []p200OKData;
					p200OKData = tp;
				}
			}
		}
		return TRUE;
	}
	BOOL Submit(BOOL bGet, BOOL b200OK)
	{
		if(bGet)
			bGetOK = TRUE;
		if(b200OK)
			b200OK = TRUE;
		return bGetOK && b200OK;
	}

	BOOL	bGetOK;
	int		haveGetlen;
	int		mallocGetLen;
	unsigned char *pGetData;

	BOOL b200OK;
	int		have200Len;
	int		malloc200Len;
	unsigned char *p200OKData;
};

class CNogSession  : public SocketSession
{
public:
	CNogSession(DATEHEADER& packh, int aT);
	
	virtual ~CNogSession();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);

public:
	void SetOriKey(int ver, unsigned char key[16], const unsigned char *pFirst);
	BOOL DecodePacket(BOOL bClient, unsigned char *pdata, DWORD ClientAck, DWORD ServerSeq);

private:
	BOOL WriteTmpData(CString szFilename, unsigned char *pData, int datalen);
	BOOL DealPacketList(BOOL bClient, BOOL bLast);
	BOOL DealOneGet(COneGet200OK *pOneGet);

private:
	int		m_ver;
	unsigned char m_Key[16];

	BOOL	m_bServerOK;		// TRUE正常, FALSE服务器回应解码中遇到错误
	BOOL	m_bClientOK;		// TRUE正常, FALSE客户端请求解码中遇到错误

	DWORD	m_PrevGET;			// 当前正处理的Get的ACK
	DWORD	m_cseq;				// 
	DWORD	m_cnseq;
	TcpPacketList	m_cpacketlist;

	DWORD	m_Prev200OK;
	DWORD	m_sseq;
	DWORD	m_snseq;
	TcpPacketList	m_spacketlist;

	CXorBuffer	m_xorBuffer;
	CMapStringToOb	m_mapGet200OK;

	unsigned char m_cxorstring[300];
	unsigned char m_sxorstring[300];

public:
	HttpProtocol*	m_dceprocotol;

};

class CFgServer :public CObject
{
public:
	CFgServer(){
		m_lastUse = time(0);
		m_bHave = FALSE;
		m_cntKey = 0;
		memset(m_Key, 0, 10*16);
	}

public:
	BOOL CheckIsOutTime(time_t now)
	{
		if(abs(now-m_lastUse) > 5*60)	//  5分钟内未确定回应
		{
			if(memcmp(m_Key, "\x0\x0\x0\x0", 4) == 0)
				return TRUE;
		}
		// 只要有确认过Key, 永远不删, 暂定
		return FALSE;
	}
	BOOL SetKey(int idx, unsigned char rKey[16])
	{
		if(idx >=16)
		{
			return FALSE;
		}
		else
		{
			memcpy(m_Key+idx*16, rKey, 16);
			m_cntKey = idx+1;
			return TRUE;
		}
	}	// 5小时未使用

	BOOL			m_bHave;			// 是否有确认的服务器返回
	time_t			m_lastUse;			// 最后一次使用该服务器时间
	int				m_cntKey;			// 已用数量
	unsigned char	m_Key[10*16];		// 最大16组Key
};

// 待确定及确定服务器列表
extern time_t			g_chkFgServer;
extern CMapStringToOb	g_MapFgServer;
void	CheckFreegateServer();		// 检查长时间未确认服务器, 清

// 自由之门判断
#define FG_675HELLO				1		// 查找之hello
#define FG_676HELLO				2	
#define FG_675COMMUNICATION		3		// 通讯之hello
#define FG_676COMMUNICATION		4	
#define FG_MAYRESPONSES			5		// 查找之服务器回应17 03 00
DWORD	isFreeGate(const char* pPacket, DATEHEADER& packh, unsigned char rKey[16],HttpProtocol* httpprotocl);

#endif // !defined(AFX_NOGSESSION_H__A31A6318_62C2_429A_AAB1_4DAF6C64CEF4__INCLUDED_)
