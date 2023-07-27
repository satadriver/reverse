// IpQueryLib.h: interface for the IpQueryLib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPQUERYLIB_H__B988A5D1_BCB7_486F_87C3_58D4F2995AB8__INCLUDED_)
#define AFX_IPQUERYLIB_H__B988A5D1_BCB7_486F_87C3_58D4F2995AB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// class Code4;
// class Data4;
// class Index4;
// 
// class CTableAccess  
// {
// public:
// 	CTableAccess();
// 	virtual ~CTableAccess();
// 	
// public:
// 	BOOL SetCode4(Code4 *Incode, Data4 *Indata);
// 	BOOL SetIndex4(Index4 *Inindex);
// 	
// private:
// 	Code4	*code;
// 	Data4	*data;
// 	Index4	*index;
// };
// 
int strtrimcpy(char*pdtr, char *pstr);
// BOOL OpenDbfTable (Code4& code, Data4& data, LPCTSTR dbfname);
// BOOL OpenIndex(Data4& data, Index4& index, TAG4INFO* ptaginfo, LPCTSTR cdxname);
// BOOL CloseTable(Code4 *code, Data4 *data, Index4 *index, CTableAccess *acTable);
typedef struct _IPUNIT_	// IP�ε�Ԫ
{
	DWORD	begip;		// ��ʼIP
	DWORD	endip;		// ��ֹIP
	long	rectm;		// ��¼��ʱ���
	LPCTSTR ptext;		// �����ı�
}IPUNIT, *LPIPUNIT;

typedef struct _IPUNITARRAY_	// IP�����鵥Ԫ
{
	LPIPUNIT	piparray;	// IP������
	int			unitcount;	// IP��Ԫ����
	int			maxipnum;	// ��ǰ��������������
	long		newtime;	// IP�ε�����ʱ��
	_IPUNITARRAY_ *pnext;	// ��һ��IP������ָ��
	_IPUNITARRAY_ *pPrev;	// ��һ��IP������ָ��
	_IPUNITARRAY_ () {memset(this, 0, sizeof(_IPUNITARRAY_));}
	~_IPUNITARRAY_() 
	{
		if (pnext != 0)
			delete pnext;

		/*if (piparray!=0) */
		delete[] piparray;
	}
	void Reset()
	{
		if (pnext != 0)
			pnext->Reset();
		unitcount = 0;
	}
	BOOL QueryIP(DWORD uip, IPUNIT& ipunit);

}IPARRAY, *LPIPARRAY;

typedef struct _IPAREATXT_		// IPλ���ı�������
{
	char**	ptxtbuf;		// ������ָ������
	int		txtbufcount;	// �������ܸ���
	int		bufpos;			// ��ǰʹ�õĻ�����
	int		txtpos;			// �������ڵ��µ��ı�λ��

	_IPAREATXT_()	{memset(this, 0, sizeof(_IPAREATXT_));}

	~_IPAREATXT_()
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
}IPAREATXT, *LPIPAREATXT;

class IpQueryLib  
{
public:
	IpQueryLib();
	~IpQueryLib();

public:
	CString QueryIpAddr(LPCTSTR strip);
	CString QueryIpAddr(DWORD uip);
	BOOL GetLocalWebSite(DWORD uip);
	BOOL GetLocalWebSite(LPCTSTR strip);

	BOOL LoadAllIPFromDbf(LPCTSTR dbfname);
	BOOL LoadLocalIPFromDbf(LPCTSTR dbfname);
	BOOL LoadyysIPFromDbf(LPCTSTR dbfname);

protected:
	static	void InsertAllIP(LPIPARRAY piparray, LPIPUNIT pipunit, int& error);
	static	void InsertLocIP(LPIPARRAY piparray, LPIPUNIT pipunit, int& error);
//	static	void InsertyysIP(LPIPARRAY piparray, LPIPUNIT pipunit, int& error);
	static  int  CheckIPUnit(LPIPARRAY piparray, LPIPUNIT pipunit);
protected:
	void InsertAllIPAddr(LPCTSTR begip, LPCTSTR endip, LPCTSTR txt, long rectm, long bufid);
	void InsertLocIPAddr(LPCTSTR begip, LPCTSTR endip, LPCTSTR txt, long iptm, long bufid);	
	void InsertyysIPAddr(LPCTSTR begip, LPCTSTR endip, LPCTSTR txt, long iptm, long bufid);

	void ClearAllIpLib(long bufid);
	void ClearLocIpLib(long bufid);
	void ClearyysIpLib(long bufid);

	// ȫ��IP��
	LPIPARRAY	*m_ppAllIpMap_64k[2];
	LPIPARRAY	*m_ppAllIpMap_256[2];
	IPARRAY		*m_pAllIplastMap[2];
	IPAREATXT	m_pAllIptxtbuf[2];		// λ���ı�
	char jiangebuf[1000];//����һ������������֮��Ԥ���ӿǳ���
	// ����IP��
	LPIPARRAY	*m_ppIpMap_64k[2];
	LPIPARRAY	*m_ppIpMap_256[2];
	IPARRAY		*m_pIplastMap[2];
	IPAREATXT	m_pLocalIptxtbuf[2];	// λ���ı�

	// ��Ӫ��IP��
	LPIPARRAY	*m_ppyysIpMap_64k[2];
	LPIPARRAY	*m_ppyysIpMap_256[2];
	IPARRAY		*m_pyysIplastMap[2];
	IPAREATXT	m_pyysIptxtbuf[2];	// λ���ı�

	long		m_AllIpLibReadFlag;
	long		m_LocIpLibReadFlag;
	long		m_yysLibReadFlag;

public:
//	int			m_ipcount;
	int			m_error;
};

extern IpQueryLib	k_IpQueryTable;	// IP�����

#endif // !defined(AFX_IPQUERYLIB_H__B988A5D1_BCB7_486F_87C3_58D4F2995AB8__INCLUDED_)
