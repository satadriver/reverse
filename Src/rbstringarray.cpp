// RbStringArray.cpp: implementation of the RbStringArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RbStringArray.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


#define CUTOFF 8
#define		TXTBUFSIZE		4096
#define		BUFBLOCK		8
const char*	npEmpty = _T("");

LPCTSTR _nIPAREATXT_::AppendText(LPCTSTR ptxt, int len)
{
	if (len <= 0)
		return npEmpty;
	if (txtpos == 0)
	{
		// 增加一个新的缓冲区
		if (bufpos == txtbufcount)
		{
			// 开辟新的缓冲区
			if (0 == (txtbufcount & (BUFBLOCK-1)))
			{
				char **tp = new char*[txtbufcount+BUFBLOCK];
				memcpy(tp, ptxtbuf, txtbufcount*4);
				delete[] ptxtbuf;
				ptxtbuf = tp;
			}
			ptxtbuf[txtbufcount++] = new char[TXTBUFSIZE];
		}
	}
	
	int slen = (len+4) & ~3;
	if (slen > TXTBUFSIZE)
		slen = TXTBUFSIZE;
	
	if (slen+txtpos > TXTBUFSIZE)
	{
		txtpos=0;
		bufpos++;
		return AppendText(ptxt, len);
	}
	char* tpbuf = ptxtbuf[bufpos]+txtpos;
	memset(tpbuf+slen-4, 0, 4);
	memcpy(tpbuf, ptxt, len);
	txtpos += slen;
	return tpbuf;
}

/*! \struct SUBSORTPARA
  \brief	字符串的字排序串
*/
struct SUBSORTPARA
{
	int			arraysize;		///< 数组的大小
	LPCTSTR		*psubstr;		///< 子串的指针
	nIPAREATXT	substrtext;		///< 子串的缓冲区

	SUBSORTPARA(int maxnum)
	{
		ASSERT(maxnum >0);
		arraysize = maxnum;
		psubstr = new LPCTSTR[maxnum];
		memset(psubstr, 0, 4*maxnum);
	}
	~SUBSORTPARA()
	{
		delete[] psubstr;
	}

	void InitSortPara(CRbStringArray& strarray, DWORD *precordid, int subindex, char ctab);
	LPCTSTR AddSubText(LPCTSTR str, int subindex, char ctab);
};

void __cdecl rb_shortsort(DWORD *lo, DWORD *hi, void *para,
					int (__cdecl *comp)(const void *, const void *, void* para));

inline void __cdecl rb_swap(DWORD *p, DWORD *q/*, unsigned int width = 4*/);

void __cdecl rb_qsort (void *base, unsigned num, void *para,
					int (__cdecl *comp)(const void *, const void *, void* para));

//**********************************************************
//	比较字符串且不处理para参数(升序排序)
//**********************************************************
int compforascnopara(const void *str1, const void *str2, void *para)
{
	return ((CString*)str1)->Compare((LPCTSTR)*((CString*)str2));
}

//**********************************************************
//	比较字符串且不处理para参数(降序排序)
//**********************************************************
int compfordescnopara(const void *str1, const void *str2, void *para)
{
	return ((CString*)str2)->Compare((LPCTSTR)*((CString*)str1));
}

//**********************************************************
//	比较字符串的子串(升序排序)
//**********************************************************
int compsubstrforasc(const void *str1, const void *str2, void *para)
{
	ASSERT(para != NULL);
	SUBSORTPARA *pPara = (SUBSORTPARA*)para;
	ASSERT (pPara->arraysize > *((DWORD*)str1));
	ASSERT (pPara->arraysize > *((DWORD*)str2));
	return	strcmp( pPara->psubstr[*((DWORD*)str1)],
					pPara->psubstr[*((DWORD*)str2)]);
}

//**********************************************************
//	比较字符串的子串(降序排序)
//**********************************************************
int compsubstrfordesc(const void *str1, const void *str2, void *para)
{
	ASSERT(para != NULL);
	SUBSORTPARA *pPara = (SUBSORTPARA*)para;
	ASSERT (pPara->arraysize > *((DWORD*)str1));
	ASSERT (pPara->arraysize > *((DWORD*)str2));
	return	strcmp( pPara->psubstr[*((DWORD*)str2)],
					pPara->psubstr[*((DWORD*)str1)]);
}

CRbStringArray::CRbStringArray()
{

}

CRbStringArray::~CRbStringArray()
{
}

//**********************************************************
//	对字符串数组中的字符串排序
//**********************************************************
void CRbStringArray::Sort(BOOL isAsc/* = TRUE */)
{
	if (m_nSize > 1)
	{
		rb_qsort(this->m_pData, this->m_nSize, NULL,
			(isAsc)? compforascnopara : compfordescnopara);
	}
}


//**********************************************************
//	对字符串数组中的字符串排序(按照子串)
//**********************************************************
void CRbStringArray::SortSubstring(DWORD* pRecord, int subindex, char ctab, BOOL isAsc/* = TRUE*/)
{
	if (m_nSize > 1)
	{
		SUBSORTPARA	SortPara(m_nSize);
		SortPara.InitSortPara(*this, pRecord, subindex, ctab);
		rb_qsort(pRecord, m_nSize, &SortPara,
			(isAsc)? compsubstrforasc : compsubstrfordesc);
	}
}


inline void __cdecl rb_swap (DWORD *a, DWORD *b)
{
	DWORD tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

void __cdecl rb_shortsort (DWORD *lo, DWORD *hi, void *para,
				int (__cdecl *comp)(const void *, const void *, void* para))
{
	// 可以排序的单元的宽度必须为4Byte
    DWORD *p, *max;
    while (hi > lo)
	{
        max = lo;
        for (p = lo+1; p <= hi; p++)
		{
            if (comp(p, max, para) > 0)
                max = p;
        }
        rb_swap(max, hi);
        hi--;
    }
}


void __cdecl rb_qsort (void *base, unsigned num, void *para,
				int (__cdecl *comp)(const void *, const void *, void* para))
{
    char *lo, *hi;
    char *mid;
    char *loguy, *higuy;
    unsigned size;
    char *lostk[30], *histk[30];
    int stkptr;
	#define		UNITWIDTH	4		 // 可以排序的单元的宽度必须为4Byte
	
    if (num < 2)
        return;
    stkptr = 0;
    lo = (char*)base;
    hi = (char *)base + UNITWIDTH * (num-1);

recurse:

    size = (hi - lo) / UNITWIDTH + 1;
    if (size <= CUTOFF)
	{
         rb_shortsort((DWORD*)lo, (DWORD*)hi, para, comp);
    }
    else
	{
        mid = lo + (size / 2) * UNITWIDTH;
        rb_swap((DWORD*)mid, (DWORD*)lo);

        loguy = lo;
        higuy = hi + UNITWIDTH;

        for (;;)
		{
            do
			{
                loguy += UNITWIDTH;
            } while (loguy <= hi && comp(loguy, lo, para) <= 0);

            do
			{
                higuy -= UNITWIDTH;
            } while (higuy > lo && comp(higuy, lo, para) >= 0);


            if (higuy < loguy)
                break;
            rb_swap((DWORD*)loguy, (DWORD*)higuy);
        }

        rb_swap((DWORD*)lo, (DWORD*)higuy); 

        if ( higuy - 1 - lo >= hi - loguy )
		{
            if (lo + UNITWIDTH < higuy)
			{
                lostk[stkptr] = lo;
                histk[stkptr] = higuy - UNITWIDTH;
                ++stkptr;
            } 

            if (loguy < hi)
			{
                lo = loguy;
                goto recurse;
            }
        }
        else
		{
            if (loguy < hi)
			{
                lostk[stkptr] = loguy;
                histk[stkptr] = hi;
                ++stkptr; 
            }

            if (lo + UNITWIDTH < higuy)
			{
                hi = higuy - UNITWIDTH;
                goto recurse;
            }
        }
    }

    --stkptr;
    if (stkptr >= 0)
	{
        lo = lostk[stkptr];
        hi = histk[stkptr];
        goto recurse;
    }
    else
        return;
}

void SUBSORTPARA::InitSortPara(CRbStringArray& strarray, DWORD *precordid, int subindex, char ctab)
{
	ASSERT(precordid != NULL);
	ASSERT(ctab>0);
	ASSERT(ctab<0x20);
	arraysize = min(strarray.GetSize(), arraysize);
	for (int i = 0; i < arraysize; i++)
	{
		precordid[i] = i;
		psubstr[i] = AddSubText(strarray[i], subindex, ctab);
	}
}

LPCTSTR SUBSORTPARA::AddSubText(LPCTSTR str, int subindex, char ctab)
{
	ASSERT(str != NULL);
	ASSERT(subindex >= 0);
	// 拆解子串
	int idx = subindex;
	char *psubstr = (char*)str, *tp = NULL;
	while (idx-- > 0)
	{
		tp = strchr(psubstr, ctab);
		if (tp == NULL)
			return npEmpty;	// 没有找到相应的子串
		else
			psubstr = tp+1;
	}
	tp = strchr(psubstr, ctab);
	
	// 增加子串到缓冲区
	return substrtext.AppendText(psubstr,
		(tp == NULL)? lstrlen(psubstr) : tp-psubstr);
}