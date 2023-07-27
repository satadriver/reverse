// RbStringArray.h: interface for the RbStringArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBSTRINGARRAY_H__C0F359D2_63DF_46D4_AA72_45B4CD344051__INCLUDED_)
#define AFX_RBSTRINGARRAY_H__C0F359D2_63DF_46D4_AA72_45B4CD344051__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*! \class CRbStringArray : public CStringArray
  \brief	排序数组,继承自CStringArray,提供了排序功能

  类

*/
class CRbStringArray : public CStringArray  
{
public:
	void SortSubstring(DWORD* pRecord, int subindex, char ctab, BOOL isAsc = TRUE);
	void Sort(BOOL isAsc = TRUE);
	CRbStringArray();
	virtual ~CRbStringArray();
	
};

/*! \struct _IPAREATXT_
  \brief 存储IP地区信息

  类

*/
typedef struct _nIPAREATXT_
{
	char**	ptxtbuf;		///< 缓冲区指针数组
	int		txtbufcount;	///< 缓冲区总个数
	int		bufpos;			///< 当前使用的缓冲区
	int		txtpos;			///< 缓冲区内的新的文本位置
	
	_nIPAREATXT_()	{memset(this, 0, sizeof(_nIPAREATXT_));} 
	
	~_nIPAREATXT_()
	{
		for (int i = txtbufcount-1; i >= 0; i--)
		{
			delete[] ptxtbuf[i];
		}
		delete[] ptxtbuf;
	}
	
	void	Reset() {txtpos = 0; bufpos = 0;}
	LPCTSTR AppendIPAreaText(LPCTSTR ptxt, int len = -1);
	LPCTSTR AppendText(LPCTSTR ptxt, int len);
}nIPAREATXT, *LPnIPAREATXT;


#endif // !defined(AFX_RBSTRINGARRAY_H__C0F359D2_63DF_46D4_AA72_45B4CD344051__INCLUDED_)
