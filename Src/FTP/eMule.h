// eMule.h: interface for the eMule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EMULE_H__C9DDD2DE_2E29_422F_BBF6_24B00A0AACEA__INCLUDED_)
#define AFX_EMULE_H__C9DDD2DE_2E29_422F_BBF6_24B00A0AACEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>

//extern HANDLE g_FtpHeap;

class CeMuleMap : public CMap<CString, LPCTSTR, time_t, time_t&>
{
public:
	CeMuleMap()
	{
		m_nBlockSize = 4096;
		m_nHashTableSize = 120077;
	}

// 	void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE)
// 	{
// 		ASSERT(m_pHashTable==NULL);
// //		ASSERT(g_FtpHeap!=NULL);
// 		if (m_nHashTableSize < hashSize)
// 		{
// 			m_nHashTableSize = hashSize;
// 		}
// 		m_pHashTable = (CAssoc**)HeapAlloc(g_FtpHeap, 0, sizeof(CAssoc*)*m_nHashTableSize);
// 		memset(m_pHashTable, 0, sizeof(CAssoc*) * m_nHashTableSize);
// 	}


	BOOL RemoveKey(LPCTSTR key)
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
//				DestructElements(&pAssoc->value, 1);
				DestructElements(&pAssoc->key, 1);
				pAssoc->pNext = m_pFreeList;
				m_pFreeList = pAssoc;
				m_nCount--;
				ASSERT(m_nCount >= 0);  // make sure we don't underflow
				if (m_nCount == 0)
				{
					m_pFreeList = NULL;
					m_pBlocks->FreeDataChain();
					m_pBlocks = NULL;

				}
				return TRUE;
			}

			ppAssocPrev = &pAssoc->pNext;
		}
		return FALSE;  // not found
	}

	void RemoveAll()
	{
		if (m_pHashTable != NULL)
		{
			// destroy elements (values and keys)
			for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
			{
				CAssoc* pAssoc;
				for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
				  pAssoc = pAssoc->pNext)
				{
					DestructElements(&pAssoc->value, 1);
					DestructElements(&pAssoc->key, 1);
				}
			}
		}

// 		// free hash table
// 		delete[] m_pHashTable;
// 		m_pHashTable = NULL;

		m_nCount = 0;
		m_pFreeList = NULL;
		m_pBlocks->FreeDataChain();
		m_pBlocks = NULL;

	}

};

extern CeMuleMap	g_EMuleMap;

BOOL ProcessEMulePacket(DATEHEADER& packh, int PacketFrom, UCHAR* pPacket, LPCTSTR spyaddr);

// eMuleЭ��Ĵ�����
class eMuleSession : public SocketSession
{
public:
	eMuleSession() {}
	virtual ~eMuleSession() {}
	
	// ��ʼ���� ���������ظ�(m_eMuleMap��)�ͷ���FALSE
	BOOL InitClass (DATEHEADER& packh,
					int packetfrom,			// ������
					BOOL isClient,			// �Ƿ��ǿͻ��˰�
					CString downfile		// ���ص��ļ���(ֻ��isClient==TRUEʱ��������)
				   );

// 	// ƥ�䰸����Ԥ��
// 	void SearchCaseAndWarn(DATEHEADER& packh);

	// д�����ļ������(�����ڻ���)
	// pro�����Ŀ¼��Э��
	// subpro��д�ڶ����ļ��е�Э��
	// txt��Ҫд�ڶ����ļ��еķǱ�׼�Ĵ�
	// pPro�Ǹ���Э���DceMission����ָ��
// 	virtual int	WriteIndexFile(LPCTSTR pro,
// 						LPCTSTR subpro,
// 						CString txt,
// 						DceMission* pMiss
// 						);

public:
	BOOL			m_bInit;			// �ö����Ƿ��Ѿ���ʼ��
	CString			m_downfname;		// ���ص��ļ���
	CString			m_eMuleKey;			// m_eMuleMap�е�key
};				

#endif