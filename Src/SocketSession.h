// SocketSession.h: interface for the CSocketSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKETSESSION_H__327B8E8F_6138_4B99_8C16_58EA64EB9929__INCLUDED_)
#define AFX_SOCKETSESSION_H__327B8E8F_6138_4B99_8C16_58EA64EB9929__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//*************************************************
//	类名称:			SocketSession
//	类继承:			
//	描述:			会话对象类
//	主要函数:		InsertPacket
//					WriteIndexFile
//	使用事项:		必须重载子类使用
//*************************************************/
#define			MAPTYPE_OICQ		1	// m_dwType取值
class SocketSession
{
public:
	SocketSession(DATEHEADER& packh, int aT);
	SocketSession(int aT,DATEHEADER& packh);//ip对需要调换方向
	SocketSession();
	virtual ~SocketSession();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket){return 0;}
	virtual BOOL CloseSession(time_t nowtm){return TRUE;}

	// 写对照文件并输出
	// pro是输出目录的协议
	// subpro是写在对照文件中的协议
	// txt是要写在对照文件中的非标准的串
	// pMiss是各自协议的DceMission对象指针
	virtual int	WriteIndexFile(LPCTSTR pro,
						LPCTSTR subpro,
						CString txt,
						DceMission* pMiss
						);

	//写网络活动历史并输出
	// pMiss是各自协议的DceMission对象指针
	// pro 协议 如：media
	// subpro 子协议 如：media_asf
	// hisrec 从外边定义的存放活动历史的临时类，并已初始化完私有的数据
	// num/*第几个数组，为确保向数组中写时单线程，每个插件分配一个num
	// type 数据的类型 1为网络活动历史
	int WriteHisData(LPCTSTR pro,
					  LPCTSTR subpro,
					  HistoryRecord *hisRec,
					  DceMission* pMiss,
					  int num,
					  int type = WEBID_DATA);
	
	// 生成网络活动历史的串
	// pMiss是各自协议的DceMission对象指针
	// pro 协议 如：media
	// subpro 子协议 如：media_asf
	// hisrec 从外边定义的存放活动历史的临时类，并已初始化完私有的数据
	// optm 是时间如果optm==0 则使用m_begfiletm
	CString GetWebidStr(LPCTSTR pro,
						LPCTSTR subpro,
						HistoryRecord *hisRec,
						DceMission* pMiss,
						time_t optm = 0);

	// 生成虚拟身份的串
	// pMiss是各自协议的DceMission对象指针
	// pro 协议 如：media
	// subpro 子协议 如：media_asf
	// hisrec 从外边定义的存放虚拟身份的临时类，并已初始化完私有的数据
	// optm 是时间如果optm==0 则使用m_begfiletm
	CString SocketSession::GetWebfigStr(LPCTSTR pro,
					LPCTSTR subpro,
					HistoryRecord *hisRec,
					DceMission* pMiss,
					time_t optm);

	int SearchCase(BOOL opencaseflag, time_t nowtm);
	int SearchCaseByKey(LPCTSTR text, int len, DWORD typemark);

	//获取案件告警（界面、手机）和实时跟踪（SLEUTH中）文件串
	CString GetCaseAlarmStr(CaseInfo& caseinfo, LPCTSTR pro);

	//获取预警告警文件串
	CString	GetWarnAlarmStr(warnkeyinfo& warninfo, LPCTSTR pro);

	// 检查实体文件的语种
	void CheckLanguage(char* buffer = NULL, int buflen = 0);

	//获得写虚拟身份等的buf数组的id，第几个buf是那个协议
	int GetProBufId(LPCTSTR pro);

	BOOL GetClassAreaInfo(CString ipaddr,CString& country,CString& province,CString& city,CString& county,CString& telecom) ;

public:
	
	unsigned char	m_csip[4]		;	//net格式ip
	unsigned char	m_cdip[4]		;	//net格式ip
	CString			m_sip			;	//134.035.006.001:00025
	CString			m_dip			;
	CString			m_smac		;
	CString			m_dmac		;
	AuthInfo		m_AuthInfo	;		// 认证信息
	int				m_ProxyType	;		// 数据使用代理的类型
//	DWORD			m_vLanID	;		// VLaID
	BOOL			m_bRecord;			// 是否已经记录到活动历史
	LPCTSTR			m_Language	;		// 文本的语种
	DWORD			m_dataSrc	;		// 数据源, CDMA/GPRS/IDC/OTHER		
	CString			m_strISP	;		// IDC之ISP
	time_t			m_lastpacktm;		// 该会话最后一个包的时间
	time_t			m_begfiletm	;		// 截获开始时间(要写到对照文件中的时间)
	
	// 截获结束时间(要写到对照文件中的时间)
	// 如果m_endfiletm==0表示只有m_begfiletm有效
	time_t			m_endfiletm	;		
										
	int				m_closetm	;		// 
	LPCTSTR			m_spyaddr	;		// 数据的截获位置

	CaseInfoArray	m_CaseArr	;		// 案件信息数组
	WarnkeyInfoArray	m_WarnArr;
	
	CString			m_indexfname;		// 对照文件名
	CString			m_realfname;		// 实体文件名
	CStringArray	m_attfiles;			// 附件文件名数组

	CString			m_url;				// URL
	CString			m_userid;			// 用户ID
	CString			m_from;				// 发件人
	CString			m_to;				// 收件人

	BOOL			m_OpenCaseFlg;		// 案件开关
	BOOL			m_OpenWarnFlg;		// 预警开关
	BOOL			m_OpenIndxFlg;		// 回溯开关
	BOOL			m_rubbishflag;		// 是否是屏蔽信息
	DWORD			m_dwType;			// 此会话的类型, 如QQ/..
	CString			m_VPNaccount;		// vpn account
protected:

	// 格式化文件名(实体文件和对照文件)同时匹配语种
	int FormatAttFname(char* buf, int buflen,int &atttype);
	CString m_NetwordStandard;//网络制式

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

// 哈希表定义
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
		m_curpos = 0;				// 用于遍历Map
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
