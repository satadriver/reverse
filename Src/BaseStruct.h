//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	BaseStruct
//	�汾:		1.0
//	��������:	2008-2-23
//	����:		��ǿ
//	��Ҫ����:	���������ݽṹ����
//	ģ������:
//	��Ҫ����:	
//**************************************************************** 
#ifndef			_BASESTRUCT_H_
#define			_BASESTRUCT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Ⱥ��DDE���б�
typedef struct STRUCT_DDELIST	
{
public:
	char	ip[20];
	int		threadnum;
	STRUCT_DDELIST *pNext;
	void	*pDDeBuf;
	CString spyaddr;
	STRUCT_DDELIST()
		{memset(this, 0, sizeof(STRUCT_DDELIST)-sizeof(CString));}
	~STRUCT_DDELIST()
		{if (pNext!=NULL) delete pNext;}
}DDELIST;

// Ⱥ��DCE���б�
typedef struct STRUCT_DCELIST
{
public:
	DWORD	uip;
	int		group;
	int		machine;
	BOOL	useramdisk;
	STRUCT_DCELIST *pNext;
	STRUCT_DCELIST()
	{
		memset(this, 0, sizeof(STRUCT_DCELIST));
		useramdisk = TRUE;
	}
	~STRUCT_DCELIST()
	{if (pNext!=0) delete pNext;}
}DCELIST;

#pragma pack(push)
#pragma pack(1)

// DDE������ݰ�����Ϣ�ṹ(�����ݽṹ���dde��ϸĶ�)
typedef struct  OLDDATEHEADER
{
	unsigned char		m_type;			// ������
	time_t				m_tm;			// ʱ��
	time_t				m_tm1;
	unsigned char   	m_mac[12];
	unsigned char		m_sip[4];		// ԴIP
	unsigned char		m_dip[4];		// Ŀ��IP
	WORD				m_sport;		// Դ�˿�
	WORD				m_ipid;
	WORD				m_dport;		// Ŀ�Ķ˿�
	WORD				m_vlanid;		// VLanID
	unsigned int		m_sequence;		// 
	unsigned int		m_ack;
	unsigned char		m_control;
	unsigned short		m_datalen;		// ������
	WORD				m_vlanid2;		// VLanID

}OLDDATEHEADER;

// DCE�ڲ��İ�ͷ�ṹ
#define MaxTelNo            60		// ��֤�����е绰�ֶεĳ���
#define MaxUserName         40		// ��֤�������ʺ��ֶεĳ���
#define MaxSessionId        40		// ��֤��������֤SessionID�ֶεĳ���

typedef struct DATEHEADER
{
	int					m_type;							// ������(��m_type2��ͬ)
	time_t				m_siptm;						// sip������ʱ��
	char				m_sipUserName[MaxUserName+4];	// sip���ʺ�
	char				m_sipPhone[MaxTelNo+4];			// sip�ĵ绰
	char				m_sipcelllac[MaxUserName+4];	// sip��celllac
	char				m_sipcellci[MaxUserName+4];		// sip��cellci
	char				m_sipimei[MaxUserName+4];		// sip��imei
	time_t				m_diptm;						// dip������ʱ��
	char				m_dipUserName[MaxUserName+4];	// dip���ʺ�
	char				m_dipPhone[MaxTelNo+4];			// dip�ĵ绰
	char				m_dipcelllac[MaxUserName+4];	// dip��celllac
	char				m_dipcellci[MaxUserName+4];		// dip��cellci
	char				m_dipimei[MaxUserName+4];		// dip��imei
	unsigned char		m_type2;						// ������
	time_t				m_tm;							// ʱ��
	time_t				m_tm1;
	unsigned char		m_mac[12];
	unsigned char		m_sip[4];						// ԴIP
	unsigned char		m_dip[4];						// Ŀ��IP
	WORD				m_sport;						// Դ�˿�
	WORD				m_ipid;
	WORD				m_dport;						// Ŀ�Ķ˿�
	WORD				m_vlanid;						// VLanID
	unsigned int		m_sequence;
	unsigned int		m_ack;
	unsigned char		m_control;
	unsigned short		m_datalen;						// ������
	WORD				m_vlanid2;						// VLanID
}DATEHEADER;
#pragma pack(pop)

// ��֤���ݵ��½ṹ(�����ݽṹ���dde��ϸĶ�)
typedef struct NEWAAAPACKET
{
	BYTE		m_packettype;							// ������(1��ʾΪ��֤����)
	BYTE		m_bWriteFile;							// �Ƿ�д�ļ�(1��ʾд�ļ�)
	WORD		m_RecType;								// 1==Begin 2==End
	time_t		m_tm;
	BYTE		m_Nasip[4];
	BYTE		m_ClientIp[4];
	int			m_ldelay;
	char		m_strPhone[MaxTelNo];
	char		m_strUserName[MaxUserName];
	char		m_strUserCellLac[MaxUserName];
	char		m_strUserCellCi[MaxUserName];
	char        m_strUserImei[MaxUserName];
	char		m_SessionId[MaxSessionId];
}NEWAAAPACKET,*LPNEWAAAPACKET;

// ���ݰ���ṹ
typedef struct DATABUFFER
{
public:
	DATABUFFER(){memset(this, 0, sizeof(DATABUFFER));}
	WORD		bufsize;
	WORD		packetnum;
	char*		pBuffer;
	LPCTSTR		posaddr;
	DATABUFFER* pNext;
}DATABUFFER;

// DCE������ļ��ṹ����
typedef struct DceFileStream
{
	char Filename[MAX_PATH];		// ʵ�ʵ��ļ���
	char Pro[80];					// �ļ�����Э��(��ԭЭ��Ŀ¼��ͬ)
	char CaseIDorWarnID[80];		// ����ID��Ԥ������ID�����ݺ���֤����Ϊ��
	UINT Type;						// 0��ʾ�������� 1��ʾԤ������ 2��ʾ��������
									// 3 ��ʾ��֤����
	UINT BeginPOS;					// �ļ��Ŀ�ʼλ��
	UINT FileLen;					// �ļ��ĳ���
}DceFileStream;


#endif
