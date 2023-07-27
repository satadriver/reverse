// RealipMap.cpp: implementation of the RealipMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dcepub.h"

typedef struct REALIPINFO
{
	CString		caseid;
	CString		limitaddr;
	int			limittype;
	CString		Reallimitaddr;
	DWORD		AlarmState;
}REALIPINFO;

class RealipMap : public CMap<CString, LPCTSTR, REALIPINFO*, REALIPINFO*&>
{
public:
	RealipMap();
	~RealipMap() {RemoveAll();}
};

inline void AFXAPI DestructElements(REALIPINFO** ppElements, int nCount)
{
	delete *ppElements;
}

RealipMap::RealipMap()
{
	m_pHashTable = 0;
	m_nHashTableSize = 1997;
	m_nCount = 0;
	m_pFreeList = 0;
	m_pBlocks = 0;
	m_nBlockSize = 10;
}

static RealipMap m_RipMap;
BOOL IsCaseExist(REALIPINFO *pRipInfo);

int  SearchRealIP(LPCTSTR sip, LPCTSTR dip, CaseInfoArray& caseinfoarr)
{
	ASSERT(sip != NULL);
	ASSERT(sip[0] != 0);
	ASSERT(sip[15] == 0);
	ASSERT(dip != NULL);
	ASSERT(dip[0] != 0);
	ASSERT(dip[15] == 0);
	ASSERT(v_programtype == SYSTEM_SLEUTH);
	//	return 0;
	int ret = 0;
	REALIPINFO *pRipInfo = NULL;
	m_RipMap.Lookup(sip, pRipInfo);
	if (pRipInfo!=NULL)
	{
		ret = 1;
	}
	else
	{
		m_RipMap.Lookup(dip, pRipInfo);
		if (pRipInfo!=NULL)
			ret = 2;
	}

	if (pRipInfo)
	{
		if(!IsCaseExist(pRipInfo))
		{
			return 0;
		}
		CaseInfo caseinfo;
 		caseinfo.CaseID = pRipInfo->caseid;
 		caseinfo.LimitType = pRipInfo->limittype;
 		caseinfo.LimitAddr = pRipInfo->limitaddr;
		caseinfo.AlarmState = pRipInfo->AlarmState;
		caseinfoarr.Add(caseinfo);	
	}
	
	return 0;
}
BOOL IsCaseExist(REALIPINFO *pRipInfo)
{
	if (pRipInfo->limittype<100)
		return FALSE;

	CaseInfo caseinfo;
	switch (pRipInfo->limittype-100)
	{
	case 18:		// ¹Ø¼ü×Ö
		{
			CString tmpcase;
			if (v_CaseLimitPara.m_ridreallimit[v_CaseLimitPara.m_readflg].Lookup(pRipInfo->Reallimitaddr, tmpcase))
			{
				if (pRipInfo->caseid.CompareNoCase(tmpcase) == 0)
				{
					return TRUE;
				}
			}
		}
		break;
	case 14:		// URL
		{
			CString tmpcase;
			if (v_CaseLimitPara.m_ridreallimit[v_CaseLimitPara.m_readflg].Lookup(pRipInfo->Reallimitaddr, tmpcase))
			{
				if (pRipInfo->caseid.CompareNoCase(tmpcase) == 0)
				{
					return TRUE;
				}
			}
		}
		break;
	default:
		break;
	}
	return FALSE;
}

BOOL AddRealIP(LPCTSTR rip, CString caseid, CString limit, int ltype)
{
	CString tmplimit;
	tmplimit.Format("%s",limit);
	REALIPINFO *pRipInfo = NULL;
	if (ltype < 200)
		return TRUE;

	char tmpip[20];
	memset(tmpip, 0, sizeof(tmpip));
	memcpy(tmpip, rip, 15);

	if(m_RipMap.Lookup(tmpip, pRipInfo))
	{
		if (pRipInfo->Reallimitaddr.CompareNoCase(tmplimit) ==0 )
		{
			return TRUE;
		}	
	}
	static long bacc = 0;
	char newlimit[500];
	limit += " (À©Õ¹)";
//	WriteLog("realip", "Add %s(%s)", rip, limit);
	int len = CMailCoder::base64_encode((char*)(LPCTSTR)limit,
		limit.GetLength(), newlimit);
	if ((len <= 0) || (len >= 500))
	{
		WriteLog("dce", "AddRealIPToMap(%s) Error!!");
		return FALSE;
	}
//	memset(newlimit,0,500);
//	sprintf(newlimit,"%s",limit);

	newlimit[len]=0;
	for (int i = 0; i < 100; i++)
	{
		long tmpacc = InterlockedExchange(&bacc, 0xACC);
		switch(tmpacc)
		{
		case 0:
			i = 1000;
			break;
		case 0xACC:
			::Sleep(10);
			break;
		default:
			WriteLog("dce", "acc=%d", tmpacc);
			i = 1000;
			break;
		}
	}
	if (i < 500)
	{
		WriteLog("dce", "AddRealIPToMap Error!! 0xacc-->acc");
		InterlockedExchange(&bacc, 0xACC);
	}
	if(m_RipMap.Lookup(tmpip, pRipInfo))
	{
		pRipInfo->caseid=caseid;
		pRipInfo->limitaddr = newlimit;
		pRipInfo->Reallimitaddr = tmplimit;
		pRipInfo->limittype = ltype - 100;
	}
	else
	{
		pRipInfo = new REALIPINFO;
		pRipInfo->caseid=caseid;
		pRipInfo->limitaddr = newlimit;
		pRipInfo->Reallimitaddr = tmplimit;
		pRipInfo->limittype = ltype - 100;
		pRipInfo->AlarmState = 0;
		m_RipMap.SetAt(tmpip, pRipInfo);
	}
	InterlockedExchange(&bacc, 0);
	return TRUE;
}

