// RbStringArray.h: interface for the RbStringArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RBSTRINGARRAY_H__C0F359D2_63DF_46D4_AA72_45B4CD344051__INCLUDED_)
#define AFX_RBSTRINGARRAY_H__C0F359D2_63DF_46D4_AA72_45B4CD344051__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*! \class CRbStringArray : public CStringArray
  \brief	��������,�̳���CStringArray,�ṩ��������

  ��

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
  \brief �洢IP������Ϣ

  ��

*/
typedef struct _nIPAREATXT_
{
	char**	ptxtbuf;		///< ������ָ������
	int		txtbufcount;	///< �������ܸ���
	int		bufpos;			///< ��ǰʹ�õĻ�����
	int		txtpos;			///< �������ڵ��µ��ı�λ��
	
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
