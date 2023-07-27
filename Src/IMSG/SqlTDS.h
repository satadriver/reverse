// SqlTDS.h: interface for the CSqlTDS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQLTDS_H__1B80F685_470E_47B5_B5BD_045A34ACBE9D__INCLUDED_)
#define AFX_SQLTDS_H__1B80F685_470E_47B5_B5BD_045A34ACBE9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
typedef struct _sql_header_8_
{
	char m_chrCmd;
	char m_isLast;
	unsigned short m_datalen;
	unsigned short m_channel;
	char m_packetnum;
	char m_window;
}sSqlHdr8;

typedef struct _sql2000_login_header_
{
	DWORD m_totalPacket;
	DWORD m_sqlver;
	DWORD m_packetsize;
	DWORD m_clientver;
	DWORD m_cleintpid;
	DWORD m_connectid;
	unsigned char m_flag1;
	unsigned char m_flag2;
	unsigned char m_sqltypeflag;
	unsigned char m_reserved;
	DWORD m_timezone;
	DWORD m_collation;
}sSqlLoginHdr;

typedef struct _sql2000_lenandoff_
{
	unsigned short off;
	unsigned short len;
}sLenandOff;

typedef struct _sql2000_login_offandlen_
{
	sLenandOff m_offandlen[9];
// 	sLenandOff m_clientname;
// 	sLenandOff m_username;
// 	sLenandOff m_password;
// 	sLenandOff m_appname;
// 	sLenandOff m_servername;
// 	sLenandOff m_unknow;
// 	sLenandOff m_librayrname;
// 	sLenandOff m_locale;
// 	sLenandOff m_databasename;
}sSqlLoginOff;

BOOL	GetSqlPass(const unsigned char* enPass, int lenPass, CString &strPass);
BOOL	GetSqlValue(const unsigned short*  enPass, int lenPass, CString &strPass);

class CSqlTDSSession  : public SocketSession
{
public:
	CSqlTDSSession(DATEHEADER& packh, int aT);
	virtual ~CSqlTDSSession();

	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);

public:
	CString		m_password;
	CString		m_servername;
	CString		m_appname;
	CString		m_libname;
	CString		m_databasename;
	DWORD		m_balreadyout;		// 已经输出过, 同一会话仅输出一次

public:
	// 写实体文件
	//void WriteRealFile(char* pData, int datalen);
};


inline BOOL IsSqlTDSPacket(DATEHEADER& packh, const char* pPacket)
{
//tcp[20:2]==0x1001 and (tcp[35]==0x70 or tcp[35]==0x71 or tcp[35]==0x72)
	if ((packh.m_datalen>50) && ((packh.m_type ==TCPPacket) || (packh.m_type==ProxyTCP)))
	{
		if ((memcmp(pPacket, "\x10\x01", 2) == 0)
			&& (pPacket[15]==0x70 || pPacket[15]==0x71 || pPacket[15]==0x72)
			)
		{
			return TRUE;
		}
		else if((memcmp(pPacket, "\x12\x01", 2) == 0) && pPacket[8]==0x16)
		{
			return TRUE;
		}
	}
	return FALSE;
}

#endif // !defined(AFX_SQLTDS_H__1B80F685_470E_47B5_B5BD_045A34ACBE9D__INCLUDED_)
