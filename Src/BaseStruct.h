//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	BaseStruct
//	版本:		1.0
//	创建日期:	2008-2-23
//	作者:		荣强
//	主要功能:	基本的数据结构定义
//	模块描述:
//	主要函数:	
//**************************************************************** 
#ifndef			_BASESTRUCT_H_
#define			_BASESTRUCT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 群内DDE的列表
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

// 群内DCE的列表
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

// DDE输出数据包的信息结构(该数据结构需和dde配合改动)
typedef struct  OLDDATEHEADER
{
	unsigned char		m_type;			// 包类型
	time_t				m_tm;			// 时间
	time_t				m_tm1;
	unsigned char   	m_mac[12];
	unsigned char		m_sip[4];		// 源IP
	unsigned char		m_dip[4];		// 目的IP
	WORD				m_sport;		// 源端口
	WORD				m_ipid;
	WORD				m_dport;		// 目的端口
	WORD				m_vlanid;		// VLanID
	unsigned int		m_sequence;		// 
	unsigned int		m_ack;
	unsigned char		m_control;
	unsigned short		m_datalen;		// 包长度
	WORD				m_vlanid2;		// VLanID

}OLDDATEHEADER;

// DCE内部的包头结构
#define MaxTelNo            60		// 认证数据中电话字段的长度
#define MaxUserName         40		// 认证数据中帐号字段的长度
#define MaxSessionId        40		// 认证数据中认证SessionID字段的长度

typedef struct DATEHEADER
{
	int					m_type;							// 包类型(和m_type2相同)
	time_t				m_siptm;						// sip的上线时间
	char				m_sipUserName[MaxUserName+4];	// sip的帐号
	char				m_sipPhone[MaxTelNo+4];			// sip的电话
	char				m_sipcelllac[MaxUserName+4];	// sip的celllac
	char				m_sipcellci[MaxUserName+4];		// sip的cellci
	char				m_sipimei[MaxUserName+4];		// sip的imei
	time_t				m_diptm;						// dip的上线时间
	char				m_dipUserName[MaxUserName+4];	// dip的帐号
	char				m_dipPhone[MaxTelNo+4];			// dip的电话
	char				m_dipcelllac[MaxUserName+4];	// dip的celllac
	char				m_dipcellci[MaxUserName+4];		// dip的cellci
	char				m_dipimei[MaxUserName+4];		// dip的imei
	unsigned char		m_type2;						// 包类型
	time_t				m_tm;							// 时间
	time_t				m_tm1;
	unsigned char		m_mac[12];
	unsigned char		m_sip[4];						// 源IP
	unsigned char		m_dip[4];						// 目的IP
	WORD				m_sport;						// 源端口
	WORD				m_ipid;
	WORD				m_dport;						// 目的端口
	WORD				m_vlanid;						// VLanID
	unsigned int		m_sequence;
	unsigned int		m_ack;
	unsigned char		m_control;
	unsigned short		m_datalen;						// 包长度
	WORD				m_vlanid2;						// VLanID
}DATEHEADER;
#pragma pack(pop)

// 认证数据的新结构(该数据结构需和dde配合改动)
typedef struct NEWAAAPACKET
{
	BYTE		m_packettype;							// 包类型(1表示为认证数据)
	BYTE		m_bWriteFile;							// 是否写文件(1表示写文件)
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

// 数据包块结构
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

// DCE输出流文件结构定义
typedef struct DceFileStream
{
	char Filename[MAX_PATH];		// 实际的文件名
	char Pro[80];					// 文件所属协议(和原协议目录相同)
	char CaseIDorWarnID[80];		// 案件ID或预警规则ID，回溯和认证数据为空
	UINT Type;						// 0表示回溯数据 1表示预警数据 2表示案件数据
									// 3 表示认证数据
	UINT BeginPOS;					// 文件的开始位置
	UINT FileLen;					// 文件的长度
}DceFileStream;


#endif
