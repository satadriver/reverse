// IdcIpLib.cpp: implementation of the IdcIpLib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IdcIpLib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IdcIpLib	v_IdcIpLib;

struct MAPNODE
{
	MAPNODE*	next;
	DWORD		ip;
	int			textidx;
};

struct ARRAYNODE
{
	DWORD	begip;
	DWORD	endip;
	int		textidx;
};

class NodeBlock
{
public:
	NodeBlock();
	~NodeBlock();
	struct MAPNODE* GetNewNode();
	void ReleaseNode(MAPNODE* pNode)
	{
		pNode->next = m_freeNodePtr;
		m_freeNodePtr = pNode;
	}
	void mallocNode(int blocksize);

	MAPNODE*	m_freeNodePtr;
	MAPNODE*	m_NodeBlockPtr;
	int		m_freeNodeCount;
	int		m_blocksize;
	
};

NodeBlock::NodeBlock()
{
	m_freeNodePtr = NULL;
	m_NodeBlockPtr = NULL;
	m_freeNodeCount = 0;
	m_blocksize = 1024;
}

NodeBlock::~NodeBlock()
{
	MAPNODE* tp = m_NodeBlockPtr;
	while(tp)
	{
		m_NodeBlockPtr = m_NodeBlockPtr->next;
		delete[] tp;
		tp = m_NodeBlockPtr;
	}
}

MAPNODE* NodeBlock::GetNewNode()
{
	if (m_freeNodePtr==NULL)
	{
		MAPNODE* tp = new MAPNODE[m_blocksize];
		tp->next = m_NodeBlockPtr;
		m_NodeBlockPtr = tp;
		for (int i = 1; i<m_blocksize; i++)
		{
			tp[i].next = m_freeNodePtr;
			m_freeNodePtr = tp+i;
			m_freeNodeCount++;
		}
	}
	MAPNODE* tp = m_freeNodePtr;
	m_freeNodePtr = tp->next;
	m_freeNodeCount--;
	tp->next = NULL;
	return tp;
}


static NodeBlock	nodeblock;

IdcIpLib::IdcIpLib()
{
	m_readflg = 0;
	m_blockcount[0] = m_blockcount[1] = 0;
	m_pNodePtr[0] = m_pNodePtr[1] = NULL;
	m_pIpArr[0] = m_pIpArr[1] = 0;
	m_ipcount[0] = m_ipcount[1] = 0;
	m_hashsize[0] = m_hashsize[1] = 201119;
	m_ispnamemap.InitHashTable(1101);
}

IdcIpLib::~IdcIpLib()
{
	if (m_pNodePtr[0]!=NULL)
		delete[] m_pNodePtr[0];
	if (m_pNodePtr[1]!=NULL)
		delete[] m_pNodePtr[1];

	if (m_pIpArr[0]!=NULL)
		delete[] m_pIpArr[0];
	if (m_pIpArr[1]!=NULL)
		delete[] m_pIpArr[1];
}

void IdcIpLib::AddIdcIp(DWORD begip, DWORD endip, CString strisp)
{
	strisp.TrimRight();
	ASSERT (endip>=begip);
	long writeflg = (m_readflg==0)? 1 : 0;
	if ((m_blockcount[writeflg]&127) == 0)
	{
		DWORD *tp = (DWORD*)new ARRAYNODE[m_blockcount[writeflg]+128];
		memcpy(tp, m_pIpArr[writeflg], m_blockcount[writeflg]*sizeof(ARRAYNODE));
		delete[] m_pIpArr[writeflg];
		m_pIpArr[writeflg] = tp;
	}
	ARRAYNODE *ttp = ((ARRAYNODE*)m_pIpArr[writeflg])+m_blockcount[writeflg];
	m_blockcount[writeflg]++;
	ttp->begip = begip;
	ttp->endip = endip;
	void* val = 0;
	if (!m_ispnamemap.Lookup(strisp, val))
	{
		val = (void*)m_textArr[writeflg].Add(strisp);
		m_ispnamemap.SetAt(strisp, val);
	}
	ttp->textidx = (int)val;
	if (m_ipcount[writeflg]<1000000)
		m_ipcount[writeflg] += min(endip-begip+1, 1000001);
}

// 正序
int compdword(const void* v1, const void* v2)
{
	DWORD* p1 = (DWORD*)v1;
	DWORD* p2 = (DWORD*)v2;
	if (*p1>*p2)
		return 1;
	else if (*p1<*p2)
		return -1;
	else //(*p1==*p2)
		return 0;
}

void IdcIpLib::ClearAllIp()
{
	long writeflg = (m_readflg==0)? 1 : 0;
	m_ispnamemap.RemoveAll();
	m_textArr[writeflg].SetSize(0, 8);
	if (m_pIpArr[writeflg]!=NULL)
	{
		delete[] m_pIpArr[writeflg];
		m_pIpArr[writeflg] = NULL;
		m_blockcount[writeflg] = 0;
	}
	if (m_pNodePtr[writeflg]!=NULL)
	{
		for (int i = 0; i<m_hashsize[writeflg]; i++)
		{
			if (m_pNodePtr[writeflg][i]!=0)
			{
				MAPNODE *tp = (MAPNODE*)m_pNodePtr[writeflg][i];
				while (tp)
				{
					MAPNODE *ttp = tp->next;
					nodeblock.ReleaseNode(tp);
					tp = ttp;
				}
			}
		}
		memset(m_pNodePtr[writeflg], 0, 4*m_hashsize[writeflg]);
	}
	m_ipcount[writeflg] = 0;
}

void IdcIpLib::Optimize()
{
	long writeflg = (m_readflg==0)? 1 : 0;
	m_ispnamemap.RemoveAll();
	if (m_ipcount[writeflg]<=1000000)
	{
		if (m_ipcount[writeflg]<=0)
		{
			m_ipcount[writeflg]=0;
			InterlockedExchange(&m_readflg, writeflg);
			return;
		}
		UINT tmpsize = 201119;
		if (m_ipcount[writeflg] > 100000)
			tmpsize = 500111;

		if (abs(m_hashsize[writeflg]-tmpsize)>1000)
		{
			delete[] m_pNodePtr[writeflg];
			m_pNodePtr[writeflg] = NULL;
			m_hashsize[writeflg] = tmpsize;
		}
		ARRAYNODE *ttp = (ARRAYNODE*)m_pIpArr[writeflg];
		if (m_pNodePtr[writeflg]==NULL)
			m_pNodePtr[writeflg] = new DWORD[m_hashsize[writeflg]];
		memset(m_pNodePtr[writeflg], 0, m_hashsize[writeflg]*4);

		m_ipcount[writeflg]=0;
		for (int i=0; i<m_blockcount[writeflg]; i++,ttp++)
		{
			for (DWORD ip=ttp->begip; ip<=ttp->endip; ip++)
			{
				UINT hash = ip%m_hashsize[writeflg];
				MAPNODE* pNode = (MAPNODE*)(m_pNodePtr[writeflg][hash]);
				while (pNode!=NULL)
				{
					if (pNode->ip == ip)
					{
						break;
					}
					else
					{
						pNode = pNode->next;
					}
				}
				if (pNode == NULL)
				{
					pNode = nodeblock.GetNewNode();
					pNode->next = (MAPNODE*)m_pNodePtr[writeflg][hash];
					m_pNodePtr[writeflg][hash] = (DWORD)pNode;
					pNode->ip = ip;
					pNode->textidx = ttp->textidx;
					m_ipcount[writeflg]++;
				}
			}
		}
		delete[] m_pIpArr[writeflg];
		m_pIpArr[writeflg] = NULL;
		m_blockcount[writeflg]=0;
	}
	else
	{
		// 排序(正序)
		qsort(m_pIpArr[writeflg], m_blockcount[writeflg], 12, compdword);
		// 合并ip段
		ARRAYNODE *tp1 = (ARRAYNODE*)m_pIpArr[writeflg];
		int c = 1;
		for (int i=1; i<m_blockcount[writeflg]; i++)
		{
			ARRAYNODE *tp2 = ((ARRAYNODE*)m_pIpArr[writeflg])+i;
			ASSERT(tp1->begip <= tp2->begip);
			if (tp1->textidx == tp2->textidx)
			{
				if (tp1->endip+1 >= tp2->begip)
				{
					if (tp1->endip < tp2->endip)
						tp1->endip = tp2->endip;
					continue;
				}
			}
			c++;
			tp1++;
		}
		m_blockcount[writeflg] = c;
	}
	InterlockedExchange(&m_readflg, writeflg);
}

BOOL IdcIpLib::QueryIdcIP(DWORD ip, CString& strisp)
{
	strisp.Empty();
	long readflg = m_readflg;
	if (m_ipcount[readflg]<=0)
		return FALSE;
	if (m_ipcount[readflg]<=1000000)
	{
		// 哈希查找
		UINT hash = ip%m_hashsize[readflg];
		MAPNODE* pNode = (MAPNODE*)(m_pNodePtr[readflg][hash]);
		if (pNode!=NULL)
		{
			while (pNode!=NULL)
			{
				if (pNode->ip==ip)
				{
					strisp = m_textArr[readflg].GetAt(pNode->textidx);
					return TRUE;
				}
				pNode = pNode->next;
			}
		}
	}
	else
	{
		ASSERT(m_blockcount[readflg]>0);
		// 折半法查找IP段(IP数组为begip的正序列)
		int beg = 0, end = m_blockcount[readflg]-1, pos;
		ARRAYNODE *tp;
		while (beg <= end)
		{
			pos = (beg + end) / 2;
			tp = ((ARRAYNODE*)m_pIpArr[readflg]) + pos;
			if (ip > tp->begip)
			{
				if (ip <= tp->endip)
				{
					strisp = m_textArr[readflg].GetAt(tp->textidx);
					return TRUE;
				}
				beg = pos + 1;
			}
			else if (ip < tp->begip)
			{
				end = pos - 1;
			}
			else
			{
				strisp = m_textArr[readflg].GetAt(tp->textidx);
				return TRUE;
			}
		}
		if (end < pos)
		{
			ASSERT(tp->begip < ip);
			if (tp->endip >= ip)
			{
				strisp = m_textArr[readflg].GetAt(tp->textidx);
				return TRUE;
			}
			else
				return FALSE;
		}
		if (beg > pos)
		{
			if (beg >= m_blockcount[readflg])	// 搜索指针定位到结尾且末尾的记录不不合法
				return FALSE;
			tp++;	// n.begip > n+1.begip
			ASSERT(tp->begip < ip);
			if (tp->endip >= ip)
			{
				strisp = m_textArr[readflg].GetAt(tp->textidx);
				return TRUE;
			}
			else
				return FALSE;
		}
	}
	return FALSE;
}
