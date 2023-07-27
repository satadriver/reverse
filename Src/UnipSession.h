
#if !defined(AFX_UNIPSESSION_H__03EFD704_89CC_4209_B0E8_98E399D8369B__INCLUDED_)
#define AFX_UNIPSESSION_H__03EFD704_89CC_4209_B0E8_98E399D8369B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>

//extern HANDLE g_UnipHeap;

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
		return ((memcmp(&s1, ((char*)&s2)+8, 8)==0) &&
		(memcmp(((char*)&s1)+8, &s2, 8)==0));
}

// class UnipFileName
// {
// public:
// 	UnipFileName()
// 	{
// 		m_num = 0;
// 		m_type = 0;
// 	}
// 
// // 	UnipFileName(LPCTSTR filename)
// // 	{
// // 		m_num = 0;
// // 		m_type = 0;
// // 	}
// 	
// 	~UnipFileName()
// 	{
// 		if (m_file.m_hFile != CFile::hFileNull)
// 			m_file.Close();
// 	}
// 	void DumpFile();
// 	BOOL OpenTmpFileName(LPCTSTR fileKey, LPCTSTR tmpdir);
// 	int WriteUnipFile(const char* pdata, int len);
// 	int			m_num;
// 	UINT		m_type;
// 	CFile		m_file;
// 	CString		m_filename;
// 	CString		m_CaseID;
// };

// void WINAPI DestructElements(UnipFileName** pvalue, int count);
// 
// class UnipFilenameMap : public CMap<CString, LPCTSTR, UnipFileName*, UnipFileName*&>
// {
// public:
// 	UnipFilenameMap(int nBlockSize)
// 	{
// 		ASSERT(nBlockSize > 0);
// 		m_pHashTable = NULL;
// 		m_nHashTableSize = 1997;  // default size
// 		m_nCount = 0;
// 		m_pFreeList = NULL;
// 		m_pBlocks = NULL;
// 		m_nBlockSize = nBlockSize;
// 	}
// 	~UnipFilenameMap()
// 	{
// //		RemoveAll();
// 		delete[] m_pHashTable;
// 	}
// 
// 	//	HANDLE m_memheap;
// 	void InitHashTable(UINT nHashSize, BOOL bAllocNow)
// 	{
// 		if (m_nBlockSize<256)
// 			m_nBlockSize=256;
// 		m_nHashTableSize = nHashSize;
// 		ASSERT(m_pHashTable==NULL);
// 		ASSERT(g_UnipHeap!=NULL);
// 		m_pHashTable = (CAssoc**)HeapAlloc(g_UnipHeap, 0, sizeof(CAssoc*)*nHashSize);
// 		memset(m_pHashTable, 0, sizeof(CAssoc*) * nHashSize);
// 	}
// 
// 	void RemoveAll()
// 	{
// 		if (m_pHashTable != NULL)
// 		{
// 			// destroy elements (values and keys)
// 			for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
// 			{
// 				CAssoc* pAssoc = m_pHashTable[nHash];
// 				m_pHashTable[nHash] = NULL;
// 				for (; pAssoc != NULL;  )
// 				{
// 					CAssoc *tmpAssoc = pAssoc->pNext;
// 					// remove it
// 					DestructElements(&pAssoc->value, 1);
// 					DestructElements(&pAssoc->key, 1);
// 					pAssoc->pNext = m_pFreeList;
// 					m_pFreeList = pAssoc;
// 					pAssoc = tmpAssoc;
// 					m_nCount--;
// 				}
// 			}
// 		}
// 		ASSERT(m_nCount == 0);
// 		// free hash table
// //		m_pFreeList = NULL;
// //		m_pBlocks->FreeDataChain();
// //		m_pBlocks = NULL;
// 	}
// 
// 	BOOL RemoveKey(LPCTSTR key)
// 	{
// 		ASSERT_VALID(this);
// 		
// 		if (m_pHashTable == NULL)
// 			return FALSE;  // nothing in the table
// 		
// 		CAssoc** ppAssocPrev;
// 		ppAssocPrev = &m_pHashTable[HashKey(key) % m_nHashTableSize];
// 		
// 		CAssoc* pAssoc;
// 		for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)
// 		{
// 			if (CompareElements(&pAssoc->key, &key))
// 			{
// 				// remove it
// 				*ppAssocPrev = pAssoc->pNext;  // remove from list
// 				//				FreeAssoc(pAssoc);
// 				DestructElements(&pAssoc->value, 1);
// 				DestructElements(&pAssoc->key, 1);
// 				pAssoc->pNext = m_pFreeList;
// 				m_pFreeList = pAssoc;
// 				m_nCount--;
// 				ASSERT(m_nCount >= 0);  // make sure we don't underflow
// 				
// 				return TRUE;
// 			}
// 			ppAssocPrev = &pAssoc->pNext;
// 		}
// 		return FALSE;  // not found
// 	}
// 
// };

class UnipSession
{
public:
	UnipSession(DATEHEADER& packh, int direction, BOOL isCDMA);
	~UnipSession(){}
public:
	time_t			m_begtime;			// 开始时间
	time_t			m_endtime;			// 结束时间
	BYTE			m_pro;				// 协议类型'T','U'
	BYTE			m_direction;
	BYTE			m_iscase;
	BYTE			m_isindex;
	BYTE			m_sip[4];			
	BYTE			m_dip[4];
	DWORD			m_sport;
	DWORD			m_dport;
	DWORD			m_packetnum1;
	DWORD			m_packetnum2;
	DWORD			m_bytenum1;
	DWORD			m_bytenum2;
	DWORD			m_vlanid;
	DWORD			m_DataSrc;			// 数据源
	LPCTSTR			m_spyaddr;			// 截获位置
	int				m_limittype;		// 地址类型
	CString			m_smac;
	CString			m_dmac;
	AuthInfo		m_authinfo;
	CaseInfoArray	m_CaseInfo;
	CStringArray	m_WarnInfo;
	CString			m_key;				// 
	CString			m_ISP;				// 运营商
public:
	void AddPacket(DATEHEADER& packh);
	void WriteUnipFile(LPCTSTR tmpdir, UnipProtocol* pProObject);
	static UnipSession* CreateUnipSession(DATEHEADER& packh, BOOL isCDMA);
};


inline void AFXAPI DestructElements(UnipSession** ppElements, int nCount)
{
	delete *ppElements;
}

//typedef CMap<DWORD, DWORD, DWORD, DWORD>IpMap;		// 
class IPMap : public CMap<DWORD, DWORD, DWORD, DWORD>
{
public:
	IPMap(int nBlockSize)
	{
		ASSERT(nBlockSize > 0);
		m_pHashTable = NULL;
		m_nHashTableSize = 101;  // default size
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
// 		ASSERT(g_UnipHeap!=NULL);
// 		m_pHashTable = (CAssoc**)HeapAlloc(g_UnipHeap, 0, sizeof(CAssoc*)*nHashSize);
// 		memset(m_pHashTable, 0, sizeof(CAssoc*) * nHashSize);
// 	}
	void RemoveAll();
	BOOL RemoveKey(DWORD key)
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

inline void IPMap::RemoveAll()
{
	delete[] m_pHashTable;
}


//typedef CMap<SOCKETKEY, SOCKETKEY, UnipSession*, UnipSession*&>UnipSocketMap;
class UnipSocketMap : public CMap<SOCKETKEY, SOCKETKEY, UnipSession*, UnipSession*&>
{
public:
	UnipSocketMap(int nBlockSize)
	{
		ASSERT(nBlockSize > 0);
		m_pHashTable = NULL;
		m_nHashTableSize = 1001;  // default size
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
// 		ASSERT(g_UnipHeap!=NULL);
// 		m_pHashTable = (CAssoc**)HeapAlloc(g_UnipHeap, 0, sizeof(CAssoc*)*nHashSize);
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



extern UnipSocketMap	g_unippackmap;
extern IPMap			g_ipMap;		

#endif // !defined(AFX_UNIPSESSION_H__03EFD704_89CC_4209_B0E8_98E399D8369B__INCLUDED_)
