// IDCServer.h: interface for the IDCServer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IDCSERVER_H__F6F8C0AF_1CF4_4274_BC28_EF21278A76BE__INCLUDED_)
#define AFX_IDCSERVER_H__F6F8C0AF_1CF4_4274_BC28_EF21278A76BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dcepub.h"
#include <afxtempl.h>

inline UINT AFXAPI HashKey(CString key)
{
	return HashKey((LPCTSTR)key);
}

typedef void (*SERIALFUN)(CString key, /*time_t tmBeg, time_t tmEnd, */DWORD val, DWORD userpara, DWORD mark);
//typedef struct DomainActiveTime
//{
//	DWORD		count;
//	time_t		begtime;
//	time_t		endtime;
//}DOMAINACTIVETIME;

//class  IDCServerMap : public CMap<CString, LPCTSTR, DOMAINACTIVETIME, DOMAINACTIVETIME&>
class  IDCServerMap : public CMap<CString, LPCTSTR, DWORD, DWORD&>
{
public:
	IDCServerMap()
	{
		m_nBlockSize = 8192;
		m_nHashTableSize = 201119;
	}
	void SetAt(CString key, DWORD count);
	void WriteServerTable(DWORD userpara, SERIALFUN pFunction, DWORD mark);		
};

class  IDCServerStat
{
public:
	IDCServerStat()
	{
		m_writeflg = 0;
// 		m_dbftm = 0;
	}
public:
	void AddServer(CString strServer, DWORD count = 1)
	{
//		printf("ADD Server :%s(%d)\n", strServer, count);
		ASSERT(m_writeflg==0 || m_writeflg==1);
		m_map[m_writeflg].SetAt(strServer, count);
	}
	
	void PutInServerMap(time_t time)
	{
// 		m_dbftm = time;
		long tmp = (m_writeflg==1)? 0 : 1;
		InterlockedExchange(&m_writeflg, tmp);
	}
	
//	time_t GetServerStatTime()
//	{
//		return m_dbftm;
//	}

	int	GetServerCount()
	{
		long readflg = (m_writeflg==1)? 0 : 1;
		return m_map[readflg].GetCount();
	}
	void WriteServerTable(DWORD userpara, SERIALFUN pFunction, DWORD mark)
	{
		long readflg = (m_writeflg==1)? 0 : 1;
		m_map[readflg].WriteServerTable(userpara, pFunction, mark);
		m_map[readflg].RemoveAll();
	}
protected:
	long			m_writeflg;
// 	time_t			m_dbftm;
	IDCServerMap	m_map[2];
};

extern  IDCServerStat	v_IdcServerStat[TOTALPRONUM];
// extern  IDCServerStat	v_IdcPortStat[TOTALPRONUM];
#endif // !defined(AFX_IDCSERVER_H__F6F8C0AF_1CF4_4274_BC28_EF21278A76BE__INCLUDED_)
