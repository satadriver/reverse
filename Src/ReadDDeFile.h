// ReadDDeFile.h: interface for the ReadDDeFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_READDDEFILE_H__CEE9230B_A3B3_4DE1_9CF1_D3AC9FEF599B__INCLUDED_)
#define AFX_READDDEFILE_H__CEE9230B_A3B3_4DE1_9CF1_D3AC9FEF599B__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FILEBUFFERLEN		512*1024
#define MAXBLOCKSIZE		20*1024

class DDeDataQueue		// DDE数据队列
{
public:
	DDeDataQueue(){memset(this, 0, sizeof(DDeDataQueue));}
	~DDeDataQueue(){}
public:
	DATABUFFER	*pfirst;	// 数据队列首单元指针
	DATABUFFER	*plast;		// 数据队列末单元指针
	long		unitcount;	// 数据队列单元总数
public:
//	BOOL PushDataBuf(DATABUFFER& dbuf);
	BOOL PushDataBuf(DATABUFFER* pBuffer);
	BOOL PopDataBuf(DATABUFFER& dbuf);
	BOOL PopDataBuf2(DATABUFFER& dbuf);
	BOOL IsFull()
	{
		return unitcount>100;	// 数据队列满(新数据是否可以进入队列)
	}
};

#define		SEARCHDDEFILECOUNT		12

typedef	struct AuthHead 
{
	time_t		m_starttm;
	char		m_useraccount[MaxUserName+4];
    char		m_phone[MaxTelNo+4];
	char        m_usercelllac[MaxUserName+4];
	char        m_usercellci[MaxUserName+4];
	char        m_userimei[MaxUserName+4];
}AuthHead;

class FilAuthMap :public CMap<DWORD, DWORD, AuthHead, AuthHead&>
{
public:
	FilAuthMap()
	{
		m_pHashTable = NULL;
		m_nCount = 0;
		m_pFreeList = NULL;
		m_pBlocks = NULL;
		InitHashTable(4001, TRUE);
		m_nBlockSize = 256;
	}

	~FilAuthMap()
	{
		RemoveAll();
	}

	void SetAtEx(DWORD key, NEWAAAPACKET* pval);

	BOOL LookupEx(DWORD key, AuthHead*& pval);

	void ClearMap();

};

typedef struct STRUCT_DDEFILEINFO			// DDE文件信息结构
{
public:
	STRUCT_DDEFILEINFO()
	{
		memset(this, 0, sizeof(STRUCT_DDEFILEINFO)-4-(4*SEARCHDDEFILECOUNT)-sizeof(FilAuthMap));
	}
	~STRUCT_DDEFILEINFO(){}
public:
	STRUCT_DDEFILEINFO*			m_pNext;
	DWORD						m_filetm;				// 文件时间
	DWORD						m_InsertFlg;			// 允许插入标志
	DDeDataQueue				m_queue;				// 数据队列
	FILE*						m_pf;					// 文件指针
	char*						m_pbuffer;				// 缓冲区地址
	int							m_datalen;				// 数据长度
	LPCTSTR						m_ddefiletype;			// DDE文件的扩展名
	UINT						m_proid;				// 该协议ID
	time_t						m_queuestate;			// 队列状态
	DWORD						m_lastsertm;			// 上一次搜索文件的时间
	char						m_lastfile[16];
	WORD						m_fileState;			// 当前文件读取的状态
														// 0(正常读取)/1(启用缓存)/2(完全积压)
														// 跳变顺序: 0－-z盘文件大于12个－－>1－－缓存目录文件大于等于30个－－>2－－总文件数小于8个－－>0
	WORD						m_filecount;			// filearr中包含的文件数
	CString						m_filearr[SEARCHDDEFILECOUNT];
	CString						m_fname;				// 文件名
	FilAuthMap					m_FileAuthMap;			// 文件中认证哈希
public:
	int		Open(CString& bakfile);
	void	Delete();
	int		Read(char *pfilebuf);
	void	BakData(char* pfilebuf, int len)
	{
		ASSERT(len < 2048);
		memcpy(m_pbuffer, pfilebuf, len);
		m_datalen = len;
	}
	int		PushQueue(LPCTSTR spyaddr, char *pfilebuf);
}DDEFILEINFO;

class DDeInfo
{
	public:
		DDeInfo(){}
		~DDeInfo(){}
	public:
		LPCTSTR ddeip;			// DDE的IP
		LPCTSTR spyaddr;		// 截获位置
		int		ddethnum;		// DDE的线程数
		DDEFILEINFO *pDdeFile;	// 这个dde需要处理的文件信息链表
};

int ReadDDeData(DDEFILEINFO* pDDEInfo, DATABUFFER& databuf, LPCTSTR pro);
void AddHttpSpread(DATABUFFER &oneBuf);
BOOL GetHttpSpread(DATABUFFER &oneBuf);

#endif
