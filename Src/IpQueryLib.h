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
typedef struct _IPUNIT_	// IP段单元
{
	DWORD	begip;		// 起始IP
	DWORD	endip;		// 终止IP
	long	rectm;		// 记录的时间戳
	LPCTSTR ptext;		// 区域文本
}IPUNIT, *LPIPUNIT;

typedef struct _IPUNITARRAY_	// IP段数组单元
{
	LPIPUNIT	piparray;	// IP段数组
	int			unitcount;	// IP单元个数
	int			maxipnum;	// 当前本数组的最大容量
	long		newtime;	// IP段的最新时间
	_IPUNITARRAY_ *pnext;	// 下一个IP段数组指针
	_IPUNITARRAY_ *pPrev;	// 上一个IP段数组指针
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

typedef struct _IPAREATXT_		// IP位置文本缓冲区
{
	char**	ptxtbuf;		// 缓冲区指针数组
	int		txtbufcount;	// 缓冲区总个数
	int		bufpos;			// 当前使用的缓冲区
	int		txtpos;			// 缓冲区内的新的文本位置

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

	// 全球IP库
	LPIPARRAY	*m_ppAllIpMap_64k[2];
	LPIPARRAY	*m_ppAllIpMap_256[2];
	IPARRAY		*m_pAllIplastMap[2];
	IPAREATXT	m_pAllIptxtbuf[2];		// 位置文本
	char jiangebuf[1000];//增加一个缓冲区编译之后预防加壳出错
	// 本地IP库
	LPIPARRAY	*m_ppIpMap_64k[2];
	LPIPARRAY	*m_ppIpMap_256[2];
	IPARRAY		*m_pIplastMap[2];
	IPAREATXT	m_pLocalIptxtbuf[2];	// 位置文本

	// 运营商IP库
	LPIPARRAY	*m_ppyysIpMap_64k[2];
	LPIPARRAY	*m_ppyysIpMap_256[2];
	IPARRAY		*m_pyysIplastMap[2];
	IPAREATXT	m_pyysIptxtbuf[2];	// 位置文本

	long		m_AllIpLibReadFlag;
	long		m_LocIpLibReadFlag;
	long		m_yysLibReadFlag;

public:
//	int			m_ipcount;
	int			m_error;
};

extern IpQueryLib	k_IpQueryTable;	// IP区域表

#endif // !defined(AFX_IPQUERYLIB_H__B988A5D1_BCB7_486F_87C3_58D4F2995AB8__INCLUDED_)
