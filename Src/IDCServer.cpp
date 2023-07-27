// IDCServer.cpp: implementation of the IDCServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IDCServer.h"

IDCServerStat	v_IdcServerStat[TOTALPRONUM];

void IDCServerMap::SetAt(CString key, DWORD count/*, time_t nowtime*/)
{
#ifdef		_DEBUG
	if (key.GetAt(3) == '_')
		return;
#endif
	UINT nHash;
	CAssoc* pAssoc = GetAssocAt(key, nHash);
	if (pAssoc == NULL)
	{
		if (m_pHashTable == NULL)
			InitHashTable(m_nHashTableSize);
		
		pAssoc = NewAssoc();
		if (pAssoc!=NULL)
		{
			pAssoc->value = count;
//			pAssoc->value.begtime = pAssoc->value.endtime = nowtime;
			pAssoc->nHashValue = nHash;
			pAssoc->key = key;
			pAssoc->pNext = m_pHashTable[nHash];
			m_pHashTable[nHash] = pAssoc;
#ifdef		_DEBUG
//			printf("ADD Server :%s(%d)\n", key, count);
#endif
		}
	}
	else
	{
		pAssoc->value+=count;
//		pAssoc->value.endtime = nowtime;
	}
	return;
}


void IDCServerMap::WriteServerTable(DWORD userpara, SERIALFUN pFunction, DWORD mark)
{
	if (m_pHashTable == NULL)
		return;
	if (m_nCount<=0)
	{
		m_nCount=0;
		return;
	}
	
	CAssoc** ppAssocPrev;
	for (int i=0; i<m_nHashTableSize; i++)
	{
		ppAssocPrev = m_pHashTable+i;
		for (CAssoc* pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = *ppAssocPrev)
		{
			pFunction(pAssoc->key, /*pAssoc->value.begtime, pAssoc->value.endtime,*/ pAssoc->value, userpara, mark);
			*ppAssocPrev = pAssoc->pNext;  // remove from list
			pAssoc->key.Empty();
			pAssoc->pNext = m_pFreeList;
			m_pFreeList = pAssoc;
			m_nCount--;
			ASSERT(m_nCount >= 0);
		}
	}
	ASSERT(m_nCount==0);
	return;  // not found
}

