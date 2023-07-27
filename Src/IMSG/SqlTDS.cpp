// SqlTDS.cpp: implementation of the CSqlTDS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "imsg.h"
#include "SqlTDS.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSqlTDSSession::CSqlTDSSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_balreadyout = FALSE;
	m_closetm = packh.m_tm + 120;
}

CSqlTDSSession::~CSqlTDSSession()
{
}

int CSqlTDSSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	BakProData(PRO_SQL, packh, pPacket);

	if(m_balreadyout)
	{
		// 已经输出过了
		return 0;
	}
	if (!IsSqlTDSPacket(packh, pPacket))
	{
		// 不是边锋的包
		return 0;
	}
	
	if ((packh.m_type == ProxyTCP) || (packh.m_type == ProxyUDP))
	{
		m_ProxyType = 1;
	}
	
	BOOL bSqlQuery = FALSE;
	BOOL bSqlLogin = FALSE;
	BOOL bSql0x12 = FALSE;
	
	sSqlHdr8 *psqlhdr = (sSqlHdr8*)(pPacket);
	psqlhdr->m_datalen = ntohs(psqlhdr->m_datalen);
	psqlhdr->m_channel = ntohs(psqlhdr->m_channel);
	if(psqlhdr->m_chrCmd == 0x10)
	{
		//ASSERT(psqlhdr->m_datalen == packh.m_datalen);
		if(psqlhdr->m_datalen != packh.m_datalen)
			return 0;
		
		sSqlLoginHdr *ploginhdr = (sSqlLoginHdr*)(pPacket+8);
		ploginhdr->m_sqlver = ntohl(ploginhdr->m_sqlver);
		// 0x00000070 for TDS7, 0x01000071 for TDS8, 0x02000972 for mssql 2005
		ASSERT((ploginhdr->m_sqlver&0x70) == 0x70);
		if( !(ploginhdr->m_sqlver== 0x70 
			|| ploginhdr->m_sqlver== 0x01000071
			|| ploginhdr->m_sqlver== 0x02000972))
		{
			return 0;
		}
		
		sSqlLoginOff *ploginoff = (sSqlLoginOff*)(pPacket+0x24+0x8);
		unsigned char *poffdata = NULL;
		for (int i = 0; i < 9 ; i++)
		{
			// i=0 clientname, i=1 username, i=2 password, i=3 app name, i=4 servername
			// i=5 unknow1, i=6 library name, i=7 locale, i=8 database name
			poffdata = (unsigned char*)pPacket+ploginoff->m_offandlen[i].off+8;
			switch(i)
			{
			case 2:
				{
					CString strPass;
					GetSqlPass(poffdata, ploginoff->m_offandlen[i].len, strPass);
					//printf("tunr%d:%s\n", i, strData);
					m_password = strPass;
				}
				break;
			default:
				{
					CString strData;
					GetSqlValue((unsigned short*)poffdata, ploginoff->m_offandlen[i].len, strData);
					switch(i)
					{
					case 1:
						m_userid = strData;
						break;
					case 3:
						m_appname = strData;
						break;
					case 4:
						m_servername = strData;
						break;
					case 6:
						m_libname = strData;
						break;
					case 8:
						m_databasename = strData;
						break;
					}
				}
				break;
			}
		}
		bSqlLogin = (FALSE==m_userid.IsEmpty())?TRUE:FALSE;
	}
	else if(psqlhdr->m_chrCmd == 0x12)
	{
		// ssl hdr 
		// 16 handshake ?? ??(TLS ver) ?? ?? Len
		// ?? 类型(01 client hello, 02 server hello, 10 client key exchange) ?? ?? ?? len
		int chkLen = 0;
		unsigned char *pp = (unsigned char*)pPacket;
		int freedata = packh.m_datalen-11;
		pp += 11;
		while(freedata > 0)
		{
			chkLen = pp[0]*256+pp[1];
			freedata -= (chkLen+2);
			if(freedata > 0)
				pp += (chkLen+5);
		}

		pp = (unsigned char*)pPacket;
		if(freedata == 0)	// 符合ssl包特征
		{
			BOOL bSwap = FALSE;
			if(pp[8] == 0x16 && pp[13] == 0x10)
			{
				// C-->S
				bSql0x12 = TRUE;
				bSqlLogin = TRUE;
				if(memcmp(packh.m_sip, m_cdip, 4) == 0)
				{
					// 交换会话方向
					bSwap = TRUE;
				}
			}
			else if(pp[8]== 0x16 && pp[13]==0x2)
			{
				// S-->C
				bSqlLogin = TRUE;
				bSql0x12 = TRUE;
				if(memcmp(packh.m_sip, m_csip, 4) == 0)
				{
					// 交换会话方向
					bSwap = TRUE;
				}
			}
			if(bSwap)
			{
				CString tstr = m_sip;
				m_sip = m_dip;
				m_dip = tstr;

				DWORD ip;
				memcpy(&ip, m_csip, 4);
				memcpy(m_csip, m_cdip, 4);
				memcpy(m_cdip, &ip, 4);

				tstr = m_dmac;
				m_dmac = m_smac;
				m_smac = tstr;

				m_AuthInfo.SwapAuthInfo();
			}
		}
	}
	else if(psqlhdr->m_chrCmd == 0x1)		// Query
	{
		// bSqlQuery = TRUE;
	}
	
	if(bSqlLogin && FALSE == m_balreadyout) //|| bSqlQuery
	{
		m_begfiletm = packh.m_tm;
		m_OpenCaseFlg = v_opencasebf;
		m_OpenWarnFlg = v_openwarnbf;
		m_OpenIndxFlg = v_openflagbf;
		
		// 匹配案件、预警和屏蔽信息
		DWORD state = 0;
		SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), state);
		SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);
// 处理IDC数据源
// 		CString tmpisp;
// 		if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), tmpisp))
// 		{
// 			if (m_dataSrc == DATASRC_OTHER)
// 			{
// 				m_dataSrc = DATASRC_IDC;
// 				m_strISP = tmpisp;
// 			}
// 			if (v_statIdc)
// 			{
// 				CString ttstr;
// 				ttstr.Format("%s\1%s\1bf\1游戏%s\1边锋\1%s\1\1",
// 					m_dip.Left(15), m_strISP, m_dip.Right(5), TYPESTATE_DECIDED);
// 				v_IdcServerStat[IMSG_PROTOCOL].AddServer(ttstr);
// 			}
// 		}
// 		else if ((m_dataSrc == DATASRC_OTHER)
// 			&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), tmpisp)))
// 		{
// 			m_dataSrc = DATASRC_IDC;
// 			m_strISP = tmpisp;
// 		}

// 		// 输出活动历史
//  		if (v_statGame)
// 		{
// 			HistoryRecord hisRec;
//  			hisRec.SetRecord(IDX_FORWTYPE,	"1");
//  			hisRec.SetRecord(IDX_ID,		m_userid);
// 			hisRec.SetRecord(IDX_USER,		m_userid);
// 			hisRec.SetRecord(IDX_PASS,		m_password);
// 
// 			hisRec.SetRecord(IDX_STATE, "上线");
// 			WriteHisData("imsg_sql", "imsg_sql", &hisRec, g_pImsgMissObj,	IMSG_PROTOCOL);//, HISTORY_DATA);
// 		}
		
		// 写对照文件
		if ((m_CaseArr.GetSize() > 0) || (m_WarnArr.GetSize() > 0)
			|| (!m_rubbishflag && m_OpenIndxFlg))
		{
			SYSTEMTIME now;
			GetLocalTime(&now);
			//LPCTSTR subpro;
			CString tmppath = g_pImsgMissObj->GetTmpPath();
			ASSERT(!tmppath.IsEmpty());
			m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_1_%u_sqlserver.indexfile.txt",
				tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
				now.wMinute, now.wSecond, m_sip, m_dip, rand());
			CString text;
			if(bSql0x12)
			{
				text.Format("ver=sql2005\r\nstat= 上线\r\n");
			}
			else
			{
				text.Format("ver=sql2000\r\nuser= %s\r\nstat= 上线\r\n", m_userid);
				if(FALSE == m_password.IsEmpty())
					text.Format("%snpass= %s\r\n", CString(text), m_password);
				if(FALSE == m_libname.IsEmpty())
					text.Format("%slibname= %s\r\n", CString(text), m_libname);
				if(FALSE == m_appname.IsEmpty())
					text.Format("%sappname= %s\r\n", CString(text), m_appname);
				if(FALSE == m_servername.IsEmpty())
					text.Format("%sservername= %s\r\n", CString(text), m_servername);
				if(FALSE == m_databasename.IsEmpty())
					text.Format("%sdatabasename= %s\r\n", CString(text), m_databasename);
				
			}
			m_balreadyout = TRUE;
			WriteIndexFile("sqlserver", "sqlserver", text, g_pImsgMissObj);
			m_CaseArr.RemoveAll();
			m_WarnArr.RemoveAll();
		}
	}
	
	return 0;
}

const char _sql_2000_pass_table[] = 
{
	'Z', 'J', 'z', 'j', -1 ,-1 ,':', '*', -1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		'[', 'K', '{', 'k', -1 ,-1 ,-1 , '+', -1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		'X', 'H', 'x', 'h', -1 ,-1 ,'8', '(', -1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		'Y', 'I', 'y', 'i', -1 ,-1 ,'9', ')', -1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		'^', 'N', '~', 'n', -1 ,-1 ,'>', '.', -1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		'_', 'O', -1 , 'o', -1 ,-1 ,'?', '/', -1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		'\\','L', '|', 'l', -1 ,-1 ,'<', ',', -1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		']', 'M', '}', 'm', -1 ,-1 ,'=', '-', -1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		'R', 'B', 'r', 'b', -1 ,-1 ,'2', '"', -1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		'S', 'C', 's', 'c', -1 ,-1 ,'3', '#', -1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		'P', '@', 'p', '`', -1 ,-1 ,'0', -1 , -1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		'Q', 'A', 'q', 'a', -1 ,-1 ,'1', '!', -1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		'V', 'F', 'v', 'f', -1 ,-1 ,'6', '&', -1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		'W', 'G', 'w', 'g', -1 ,-1 ,'7', '\'',-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		'T', 'D', 't', 'd', -1 ,-1 ,'4', '$', -1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,
		'U', 'E', 'u', 'e', -1 ,-1 ,'5', '%', -1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 
};

// enPass缓冲区指向, lenPass密码长度(实际缓冲区为该长度*2), strPass解密后密码
// Sql每位间以0xa5分隔, 每位查表获得实际值.
BOOL GetSqlPass( const unsigned char* enPass, int lenPass, CString &strPass )
{
	BOOL bCheckOK = TRUE;
	strPass = CString(' ', lenPass);
	for (int i = 0; i < lenPass*2 ; i+=2)
	{
		strPass.SetAt(i/2, _sql_2000_pass_table[ enPass[i] ] );
		ASSERT(enPass[i+1] == 0xa5);
		if(enPass[i+1] != 0xa5)
		{
			bCheckOK = FALSE;
			break;
		}
	}
	
	return bCheckOK;
}

BOOL GetSqlValue( const unsigned short*  enPass, int lenPass, CString &strPass )
{
	// Calculate unicode string length.
	//   USES_CONVERSION;  
	//   pTemp=W2A(wszSomeString);     
	char   szANSIString   [MAX_PATH];    
	memset(szANSIString, 0, MAX_PATH);
	WideCharToMultiByte   (   
		CP_ACP,   
		WC_COMPOSITECHECK, 
		enPass,  
		lenPass,  
		szANSIString,  
		sizeof(szANSIString), 
		NULL,  
		NULL   );
	strPass = szANSIString;
	
	return TRUE;
}