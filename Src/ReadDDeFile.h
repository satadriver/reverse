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

class DDeDataQueue		// DDE���ݶ���
{
public:
	DDeDataQueue(){memset(this, 0, sizeof(DDeDataQueue));}
	~DDeDataQueue(){}
public:
	DATABUFFER	*pfirst;	// ���ݶ����׵�Ԫָ��
	DATABUFFER	*plast;		// ���ݶ���ĩ��Ԫָ��
	long		unitcount;	// ���ݶ��е�Ԫ����
public:
//	BOOL PushDataBuf(DATABUFFER& dbuf);
	BOOL PushDataBuf(DATABUFFER* pBuffer);
	BOOL PopDataBuf(DATABUFFER& dbuf);
	BOOL PopDataBuf2(DATABUFFER& dbuf);
	BOOL IsFull()
	{
		return unitcount>100;	// ���ݶ�����(�������Ƿ���Խ������)
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

typedef struct STRUCT_DDEFILEINFO			// DDE�ļ���Ϣ�ṹ
{
public:
	STRUCT_DDEFILEINFO()
	{
		memset(this, 0, sizeof(STRUCT_DDEFILEINFO)-4-(4*SEARCHDDEFILECOUNT)-sizeof(FilAuthMap));
	}
	~STRUCT_DDEFILEINFO(){}
public:
	STRUCT_DDEFILEINFO*			m_pNext;
	DWORD						m_filetm;				// �ļ�ʱ��
	DWORD						m_InsertFlg;			// ��������־
	DDeDataQueue				m_queue;				// ���ݶ���
	FILE*						m_pf;					// �ļ�ָ��
	char*						m_pbuffer;				// ��������ַ
	int							m_datalen;				// ���ݳ���
	LPCTSTR						m_ddefiletype;			// DDE�ļ�����չ��
	UINT						m_proid;				// ��Э��ID
	time_t						m_queuestate;			// ����״̬
	DWORD						m_lastsertm;			// ��һ�������ļ���ʱ��
	char						m_lastfile[16];
	WORD						m_fileState;			// ��ǰ�ļ���ȡ��״̬
														// 0(������ȡ)/1(���û���)/2(��ȫ��ѹ)
														// ����˳��: 0��-z���ļ�����12������>1��������Ŀ¼�ļ����ڵ���30������>2�������ļ���С��8������>0
	WORD						m_filecount;			// filearr�а������ļ���
	CString						m_filearr[SEARCHDDEFILECOUNT];
	CString						m_fname;				// �ļ���
	FilAuthMap					m_FileAuthMap;			// �ļ�����֤��ϣ
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
		LPCTSTR ddeip;			// DDE��IP
		LPCTSTR spyaddr;		// �ػ�λ��
		int		ddethnum;		// DDE���߳���
		DDEFILEINFO *pDdeFile;	// ���dde��Ҫ������ļ���Ϣ����
};

int ReadDDeData(DDEFILEINFO* pDDEInfo, DATABUFFER& databuf, LPCTSTR pro);
void AddHttpSpread(DATABUFFER &oneBuf);
BOOL GetHttpSpread(DATABUFFER &oneBuf);

#endif
