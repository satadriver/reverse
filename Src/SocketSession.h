// SocketSession.h: interface for the CSocketSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETSESSION_H__327B8E8F_6138_4B99_8C16_58EA64EB9929__INCLUDED_)
#define AFX_SOCKETSESSION_H__327B8E8F_6138_4B99_8C16_58EA64EB9929__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//*************************************************
//	������:			SocketSession
//	��̳�:			
//	����:			�Ự������
//	��Ҫ����:		InsertPacket
//					WriteIndexFile
//	ʹ������:		������������ʹ��
//*************************************************/
#define			MAPTYPE_OICQ		1	// m_dwTypeȡֵ
class SocketSession
{
public:
	SocketSession(DATEHEADER& packh, int aT);
	SocketSession(int aT,DATEHEADER& packh);//ip����Ҫ��������
	SocketSession();
	virtual ~SocketSession();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket){return 0;}
	virtual BOOL CloseSession(time_t nowtm){return TRUE;}

	// д�����ļ������
	// pro�����Ŀ¼��Э��
	// subpro��д�ڶ����ļ��е�Э��
	// txt��Ҫд�ڶ����ļ��еķǱ�׼�Ĵ�
	// pMiss�Ǹ���Э���DceMission����ָ��
	virtual int	WriteIndexFile(LPCTSTR pro,
						LPCTSTR subpro,
						CString txt,
						DceMission* pMiss
						);

	//д������ʷ�����
	// pMiss�Ǹ���Э���DceMission����ָ��
	// pro Э�� �磺media
	// subpro ��Э�� �磺media_asf
	// hisrec ����߶���Ĵ�Ż��ʷ����ʱ�࣬���ѳ�ʼ����˽�е�����
	// num/*�ڼ������飬Ϊȷ����������дʱ���̣߳�ÿ���������һ��num
	// type ���ݵ����� 1Ϊ������ʷ
	int WriteHisData(LPCTSTR pro,
					  LPCTSTR subpro,
					  HistoryRecord *hisRec,
					  DceMission* pMiss,
					  int num,
					  int type = WEBID_DATA);
	
	// ����������ʷ�Ĵ�
	// pMiss�Ǹ���Э���DceMission����ָ��
	// pro Э�� �磺media
	// subpro ��Э�� �磺media_asf
	// hisrec ����߶���Ĵ�Ż��ʷ����ʱ�࣬���ѳ�ʼ����˽�е�����
	// optm ��ʱ�����optm==0 ��ʹ��m_begfiletm
	CString GetWebidStr(LPCTSTR pro,
						LPCTSTR subpro,
						HistoryRecord *hisRec,
						DceMission* pMiss,
						time_t optm = 0);

	// ����������ݵĴ�
	// pMiss�Ǹ���Э���DceMission����ָ��
	// pro Э�� �磺media
	// subpro ��Э�� �磺media_asf
	// hisrec ����߶���Ĵ��������ݵ���ʱ�࣬���ѳ�ʼ����˽�е�����
	// optm ��ʱ�����optm==0 ��ʹ��m_begfiletm
	CString SocketSession::GetWebfigStr(LPCTSTR pro,
					LPCTSTR subpro,
					HistoryRecord *hisRec,
					DceMission* pMiss,
					time_t optm);

	int SearchCase(BOOL opencaseflag, time_t nowtm);
	int SearchCaseByKey(LPCTSTR text, int len, DWORD typemark);

	//��ȡ�����澯�����桢�ֻ�����ʵʱ���٣�SLEUTH�У��ļ���
	CString GetCaseAlarmStr(CaseInfo& caseinfo, LPCTSTR pro);

	//��ȡԤ���澯�ļ���
	CString	GetWarnAlarmStr(warnkeyinfo& warninfo, LPCTSTR pro);

	// ���ʵ���ļ�������
	void CheckLanguage(char* buffer = NULL, int buflen = 0);

	//���д������ݵȵ�buf�����id���ڼ���buf���Ǹ�Э��
	int GetProBufId(LPCTSTR pro);

	BOOL GetClassAreaInfo(CString ipaddr,CString& country,CString& province,CString& city,CString& county,CString& telecom) ;

public:
	
	unsigned char	m_csip[4]		;	//net��ʽip
	unsigned char	m_cdip[4]		;	//net��ʽip
	CString			m_sip			;	//134.035.006.001:00025
	CString			m_dip			;
	CString			m_smac		;
	CString			m_dmac		;
	AuthInfo		m_AuthInfo	;		// ��֤��Ϣ
	int				m_ProxyType	;		// ����ʹ�ô��������
//	DWORD			m_vLanID	;		// VLaID
	BOOL			m_bRecord;			// �Ƿ��Ѿ���¼�����ʷ
	LPCTSTR			m_Language	;		// �ı�������
	DWORD			m_dataSrc	;		// ����Դ, CDMA/GPRS/IDC/OTHER		
	CString			m_strISP	;		// IDC֮ISP
	time_t			m_lastpacktm;		// �ûỰ���һ������ʱ��
	time_t			m_begfiletm	;		// �ػ�ʼʱ��(Ҫд�������ļ��е�ʱ��)
	
	// �ػ����ʱ��(Ҫд�������ļ��е�ʱ��)
	// ���m_endfiletm==0��ʾֻ��m_begfiletm��Ч
	time_t			m_endfiletm	;		
										
	int				m_closetm	;		// 
	LPCTSTR			m_spyaddr	;		// ���ݵĽػ�λ��

	CaseInfoArray	m_CaseArr	;		// ������Ϣ����
	WarnkeyInfoArray	m_WarnArr;
	
	CString			m_indexfname;		// �����ļ���
	CString			m_realfname;		// ʵ���ļ���
	CStringArray	m_attfiles;			// �����ļ�������

	CString			m_url;				// URL
	CString			m_userid;			// �û�ID
	CString			m_from;				// ������
	CString			m_to;				// �ռ���

	BOOL			m_OpenCaseFlg;		// ��������
	BOOL			m_OpenWarnFlg;		// Ԥ������
	BOOL			m_OpenIndxFlg;		// ���ݿ���
	BOOL			m_rubbishflag;		// �Ƿ���������Ϣ
	DWORD			m_dwType;			// �˻Ự������, ��QQ/..
	CString			m_VPNaccount;		// vpn account
protected:

	// ��ʽ���ļ���(ʵ���ļ��Ͷ����ļ�)ͬʱƥ������
	int FormatAttFname(char* buf, int buflen,int &atttype);
	CString m_NetwordStandard;//������ʽ

protected:
	
	CString GetTCPSessionKey_mfc(DATEHEADER& packh);
	string GetTCPSessionKey_std(DATEHEADER& packh);

public:	
	string SocketSession::GetTCPSessionKey_std();

private:
	string m_sTcpSessionKey;

	CString GetAccountFromVpn(DATEHEADER& packh);
	void IUPSAAA();
	void NATAUTH();
};

// ��ϣ����
class SocketMapClass : public CMap<CString, LPCTSTR, SocketSession*, SocketSession*&>
{
public:
//	HANDLE m_memheap;
	UINT m_curpos;
	SocketMapClass(int nBlockSize)
	{
		ASSERT(nBlockSize > 0);
		m_pHashTable = NULL;
		m_nHashTableSize = 201119;  // default size
		m_nCount = 0;
		m_pFreeList = NULL;
		m_pBlocks = NULL;
		m_nBlockSize = nBlockSize;
//		m_memheap = NULL;
		m_curpos = 0;				// ���ڱ���Map
	}

	~SocketMapClass()
	{
//		delete[] m_pHashTable;
	}

	BOOL ClearSocketMap(time_t nowpacktm, int maxtm)
	{
		if (m_nCount==0)
			return TRUE;
		ASSERT(m_pHashTable!=NULL);
		DWORD endtm = GetTickCount()+maxtm;
		for (; m_curpos<m_nHashTableSize;)
		{
			CAssoc** ppAssocPrev;
			ppAssocPrev = &m_pHashTable[m_curpos];
			CAssoc* pAssoc;
			BOOL isChk = FALSE;
			for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = *ppAssocPrev)
			{
				SocketSession* s = (SocketSession*)pAssoc->value;
//				CString key(pAssoc->key);
//				pAssocRet = pAssocRet->pNext;
				ASSERT(s!=NULL);
				if (nowpacktm >= s->m_closetm)
				{
					isChk = TRUE;
					if (s->CloseSession(nowpacktm))
					{
						// remove it
						*ppAssocPrev = pAssoc->pNext;  // remove from list
						//				FreeAssoc(pAssoc);
						try
						{
							DestructElements(&pAssoc->value, 1);
						}
						catch (...)
						{
							WriteLog("error", "DestructElements(%s) Error", pAssoc->key);
						}
						DestructElements(&pAssoc->key, 1);
						pAssoc->pNext = m_pFreeList;
						m_pFreeList = pAssoc;
						m_nCount--;
						ASSERT(m_nCount >= 0);  // make sure we don't underflow
						continue;
					}
				}
				ppAssocPrev = &pAssoc->pNext;
			}
			m_curpos++;
			if (isChk && GetTickCount()>endtm)
				break;
			else
				continue;
		}
		if (m_curpos >= m_nHashTableSize)
		{
			m_curpos=0;
			return TRUE;
		}
		return FALSE;
	}
// 	void InitHashTable(UINT nHashSize, BOOL bAllocNow)
// 	{
// 		if (m_nBlockSize<256)
// 			m_nBlockSize=256;
// 		m_nHashTableSize = nHashSize;
// 		ASSERT(m_pHashTable==NULL);
// 		ASSERT(m_memheap!=NULL);
// 		m_pHashTable = (CAssoc**)HeapAlloc(m_memheap, 0, sizeof(CAssoc*)*nHashSize);
// 		memset(m_pHashTable, 0, sizeof(CAssoc*) * nHashSize);
// 	}
//	void RemoveAll() {}
	BOOL RemoveKey(CString key)
	{
		ASSERT_VALID(this);
		
		if (m_pHashTable == NULL)
			return FALSE;  // nothing in the table
		
		CAssoc** ppAssocPrev;
		ppAssocPrev = &m_pHashTable[HashKey<LPCTSTR>(key) % m_nHashTableSize];
		
		CAssoc* pAssoc;
		for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)
		{
			if (CompareElements(&pAssoc->key, &key))
			{
				// remove it
				*ppAssocPrev = pAssoc->pNext;  // remove from list
//				FreeAssoc(pAssoc);
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
};

inline void AFXAPI DestructElements(SocketSession** ppElements, int nCount)
{
	delete *ppElements;
}

#endif // !defined(AFX_SOCKETSESSION_H__327B8E8F_6138_4B99_8C16_58EA64EB9929__INCLUDED_)
