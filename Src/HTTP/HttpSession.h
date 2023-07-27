// HttpSession.h: interface for the HttpSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPSESSION_H__EA2AE2DE_7A23_4736_909F_552874D9A3F6__INCLUDED_)
#define AFX_HTTPSESSION_H__EA2AE2DE_7A23_4736_909F_552874D9A3F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define			HTTP_GET			1
#define			HTTP_POST			2
#define			HTTP_200OK			3

#define			TE_NOTHING			0		// �����
#define			TE_WUJIE			1		// �޽�
#define			TE_FREEGATE			2		// ����֮��
#define			TE_DYNAPASS			3		// ����ͨ
#define			TE_FIREPHONENIX		4		// ����
#define			TE_NEWFREEGATE		5		// ������֮��
#define			TE_FREEGATE686p		6		// ������֮��
#define			TE_FREEGATE689b3	7		// 6.89��ʽ����԰�3

#define			TE_FREEGATE707		8		//7.07
#define			TE_FREEGATE708		9		//7.08
#define			TE_FREEGATE740		10		//7.40
#define			TE_FREEGATE742		11		//7.42

#define			TE_OPENVPN			20		//openvpn 
#define			TE_OPENSSH			21		//openssh 

#define			NOREQ				0		// ��ǰû��HTTP�����ڴ���
#define			REQPROCING			0xA		// ��ǰ��HTTP�����ڴ���
#define			REQOVER				0xF		// ��ǰ�ոմ�������һ��HTTP����(�Ҵ���ɹ�)

class HttpCommInfo
{
public:
	unsigned int	m_Ssequence		;
	unsigned int	m_Sack			;
	unsigned int	m_fileseq		;
	UINT			m_nseq			;	//ָ��һ������seq
	CString			m_URL			;	//URL
	CString			m_URI			;	//URI
	
	BYTE			m_filetype		;
	BYTE			m_ifchunked		;
	BYTE			m_ifcompressed	;	//add by zjx 2003.5.10
	BYTE			m_Action		;	//GET,POST,HTTP
	bool			m_HaveHeader	;	
	bool			m_bproxy		;
	bool			m_issave		;
	bool			m_biphost		;	// hostΪip
	WORD			m_Browser		;	// ���������ʽ������	0�������������ʽ, 1����ͨ, 2����֮��, 3�޽����
									
	WORD			m_BrowserVer	;	// ����������ߵİ汾
	CString			m_strBoundary	;
	DWORD			m_FreenetKey1	;	// ��ԭ����ͨ������֮�ŵ����ݵ���Կ
	DWORD			m_FreenetKey2	;	// ��ԭ����ͨ������֮�ŵ����ݵ���Կ
	int				m_Content_length;
	CString			m_Content_Type	;
	CString			m_Host			;
	CString			m_Referer		;
	CString			m_Cookie		;	//cookie�������
	CString         m_Cookie_Clone	;	//���ݳ���
	CString			m_Cookie2		;
	CString			m_SetCookie2	;
	CString			m_useragent		;
 	CString			m_emlfield		;	//˫������ �ʼ����յ��ֶ�  or  msn Id
	CString			m_IDCInfo		;
	CString			m_Language		;
	CString			m_Charset		;
	CString         m_Accept		;	//for wap 
	BOOL			m_bRbmm			;
	BOOL            m_iswap			;	//�Ƿ�wap
	CString			m_WeiBoName		;	//����host �ж�  ����΢�� �Ѻ�΢�� ��Ѷ΢�� ����΢�� ����΢�� 
	CString			m_SearchType	;	//��������������
	CString			m_SearchKeyType	;	//������������
	CString			m_SearchKey		;	//�����Ĺؼ��� 

	CString			m_from			;
	CString			m_to			;
	CString			m_subject		;
	CString			m_cc			;
	CString			m_bcc			;
	CString			m_pro			;	//webmail
	CString			m_subpro		;	//163 sina msn��

	weibostruct		m_weiboinfo		;	//΢����Ϣ
	CString			m_emailAttName	;
	CString			m_emailAttSize	;
	CString			m_emailAttOffset;
	CString			m_emailAttLegnth;

	HttpCommInfo()
	{
		Clear();
	}

	~HttpCommInfo()
	{

	}

	void Clear()
	{
		m_Ssequence=0;
		m_Sack=0;
		m_nseq=0;
		m_fileseq=0;
		m_HaveHeader=0;
		m_ifchunked=0;
		m_filetype=0;
		m_ifcompressed=0;
		m_bproxy=0;
		m_biphost = 0;
		m_Content_length=0;
		m_Content_Type.Empty();
		m_Host.Empty();
		m_Referer.Empty();
		m_Cookie.Empty();
		m_Cookie2.Empty();
		m_SetCookie2.Empty();
		m_useragent.Empty();
		m_emlfield.Empty();
		m_strBoundary.Empty();
		m_Browser = TE_NOTHING;
		m_BrowserVer = 0;
		m_bRbmm = FALSE;
		m_Language.Empty();
		m_Charset.Empty();
		m_Accept.Empty();
		m_iswap=FALSE;
	}
};

class HttpSession : public SocketSession  
{
public:
	HttpSession(DATEHEADER& packh, int aT);
	
	virtual ~HttpSession();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
public:
	int				m_FLAG				;	//����״̬
	TcpPacketList	m_ReqPacketlst		;	//���������
	TcpPacketList	m_ResponsePacketlst	;	//��Ӧ������
	time_t			m_ProcessTime		;	//��ǰ�Ự�����ʱ��
	int				m_PacketType		;	//1: HTTP 7:IDCHTTP 10:ImsgProxy 15: Index IP+URL 20: IP+PORT
	HttpCommInfo	m_Requestline		;
	HttpCommInfo	m_Responseline		;
	UINT			m_CSeqOffset		;
	UINT			m_CAckOffset		;
	UINT			m_reqsortflag		;	// ����һ��������ֽ�����еİ���
	UINT			m_ressortflag		;	// ����һ��������ֽ�����еİ���
	char*			m_databuffer		;	//[HEADBUFFERSIZE]	;
//	WORD			m_databufsize		;	
//	WORD			m_datalen			;
	int				m_databufsize		;	
	int				m_datalen			;
	int				m_chunksize			;	// Chunked����Ŀ��С
	CString			m_id;
	HttpProtocol*	m_dceprocotol;

public:
	//*************************************************
	//	��������:		
	//	����:			
	//	�������:		
	//	�������:		
	//	����ֵ:			
	//	����:			
	//*************************************************
	int InsertPacketToBuffer(const char* pPacket, DWORD seq, int packetlen, HttpCommInfo* pheader);

	//*************************************************
	//	��������:		FindHttpHeader
	//	����:			
	//	�������:		
	//	�������:		
	//	����ֵ:			
	//	����:			
	//*************************************************
	int FindHttpHeader(char* pData, int datalen, HttpCommInfo* pheader);
	int FindHttpHeaderFromList(HttpCommInfo* pheader, TcpPacketList& TcpList, UINT& sortflg);
	int ListToFile(TcpPacketList& TcpList, HttpCommInfo* pHeader, char* buf, UINT& sortflg);
	int WriteDataToTmpFile(char* buf, int len, int overflg, BOOL bChunked);
	BOOL DealHttpTmpData();
	BOOL DealMultiPackGet();
	void FindNextRequest();
	int InsertHeadBuff(TcpPacketList &TcpList,HttpCommInfo* pheader,char ** buf,UINT Seq,unsigned int &PacketCount,UINT &buflen);	
	int LstToHttpDataBuffer(TcpPacketList &TcpList,HttpCommInfo* pheader,char ** buf,unsigned int &PacketCount,UINT & buflen);		
	int ChunkLstToFile(TcpPacketList &TcpList,HttpCommInfo* pheader,char* buf,char* filebuffer, unsigned int &PacketCount);	
	int LstToFile(TcpPacketList &TcpList,HttpCommInfo* pheader,char* buf,unsigned int& PacketCount, BOOL isAll);	
	int FilterFile(HttpCommInfo* phttpinfo);	
	int ParserUrl();
	void WriteMobileFetionFile(CString txt);
	void WriteMobileMomoFile(CString txt);
	void WritePcanyWhereFile(CString txt);
	void WriteSshFile(CString txt);
	void WriteKuaidiFile(CString txt);
	void WriteHttpUrlFile(BOOL bCase);
	void WriteSearchFile(BOOL bCase);	
	void WriteTeUrlIndexFile();	
	void DumpHttpFile(CString attflst);
// 	void DealCompressedFile(HttpCommInfo* phttpinfo);
	BOOL NextSeqSessionForImsgProxy(time_t ptm);
	void WriteSkypeIndex();
	void WriteTaoBaoIndex();
	BOOL SetIdcDomain();
};
inline HttpSession* CreateSession(DATEHEADER& packh, int aT,HttpProtocol* dceprocotol)
{
	HttpSession* s = new HttpSession(packh, aT);
	if (s)
	{
		/*
		char key[64] = {0};
		sprintf(key, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d_%d",
			packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3], packh.m_sport,
			packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3], packh.m_dport,packh.m_sequence);
						m_httpMissObj->m_SocketMap.SetAt(key, (SocketSession*&)s);*/
		dceprocotol->m_httpMissObj->m_SocketMap.SetAt(s->m_sip+"_"+s->m_dip, (SocketSession*&)s);
	}
	return s;
}

// HTTP- IDC
typedef struct SOCKETKEY
{
	DWORD	sip;
	DWORD	sport;
	DWORD	dip;
	DWORD	dport;
}SOCKETKEY;

inline UINT AFXAPI HashKey(SOCKETKEY key)
{
	return (((key.sip+key.dip)<<5)+(key.sport+key.dport));
}

inline BOOL AFXAPI operator==(const SOCKETKEY& s1, const SOCKETKEY& s2)
{
	if (memcmp(&s1, &s2, sizeof(SOCKETKEY)) == 0)
		return TRUE;
	else
		return FALSE;
}

typedef struct HttpInfo
{
//	time_t	lasttime;
	time_t	firsttime;
	CString info;
}HttpInfo;

inline void AFXAPI DestructElements(HttpInfo* ppElements, int nCount)
{
	ppElements->info.Empty();
}

class HttpSocketMap : public CMap<SOCKETKEY, SOCKETKEY, HttpInfo, HttpInfo&>
{
public:
	HttpSocketMap(int nBlockSize)
	{
		ASSERT(nBlockSize > 0);
		m_pHashTable = NULL;
		m_nHashTableSize = 101111;  // default size
		m_nCount = 0;
		m_pFreeList = NULL;
		m_pBlocks = NULL;
		m_nBlockSize = nBlockSize;
	}
	//	HANDLE m_memheap;
// 	void InitHashTable(UINT nHashSize, BOOL bAllocNow)
// 	{
// 		if (m_nBlockSize<256)
// 			m_nBlockSize=256;
// 		m_nHashTableSize = nHashSize;
// 		ASSERT(m_pHashTable==NULL);
// 		ASSERT(g_httpHeap!=NULL);
// 		m_pHashTable = (CAssoc**)HeapAlloc(g_httpHeap, 0, sizeof(CAssoc*)*nHashSize);
// 		memset(m_pHashTable, 0, sizeof(CAssoc*) * nHashSize);
// 	}
	void RemoveAll()
	{
		delete[] m_pHashTable;
	}
	
	BOOL RemoveKey(SOCKETKEY key)
	{
		ASSERT_VALID(this);
		
		if (m_pHashTable == NULL)
			return FALSE;  // nothing in the table
		
		CAssoc** ppAssocPrev;
		ppAssocPrev = &m_pHashTable[HashKey(key) % m_nHashTableSize];
		
		CAssoc* pAssoc;
		for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)
		{
			if (CompareElements(&pAssoc->key, &key))
			{
				// remove it
				*ppAssocPrev = pAssoc->pNext;  // remove from list
				// FreeAssoc(pAssoc);
				DestructElements(&pAssoc->value, 1);
				DestructElements(&pAssoc->key, 1);
				pAssoc->pNext = m_pFreeList;
				m_pFreeList = pAssoc;
				m_nCount--;
				ASSERT(m_nCount >= 0);  // make sure we don't underflow			
				return TRUE;
			}
			ppAssocPrev = &pAssoc->pNext;
		}
		return FALSE;  // not found
	}
	void ClearHttpSocketMap(time_t);
	void AddHttpRequest(SOCKETKEY& key, CString reqinfo);
	void AddHttpResponse(DATEHEADER& packh);
};

extern HttpSocketMap g_HttpMap;

#endif // !defined(AFX_HTTPSESSION_H__EA2AE2DE_7A23_4736_909F_552874D9A3F6__INCLUDED_)
