
#include "stdafx.h"
#include "dcepub.h"
#include <sys\\stat.h>
#include "zwsqldb.h"

// 构造函数
AuthInfo::AuthInfo()
{
	m_AuthType = 0;
}

//*************************************************
//	函数名称:		GetAuthInfo
//	描述:			获取认证信息
//	输入参数:		packh是数据包的包头结构
//					bCDMA表示该数据是否是CDMA(缺省是FALSE)
//	返回值:			返回获取的认证的个数(0 - 2)
//	其它:			
//*************************************************/
int  AuthInfo::GetAuthInfo(DATEHEADER& packh, BOOL bCDMA)
{
	int account = 0;
// 	if (bCDMA)
// 	{
// 		// CDMA的帐号在MAC中获取
// 		// CA标识, 1字节长度(只能为6或8) 
// 		// 1字节方向(0未知,1源IP,2目标IP) 
// 		// 若len=6 00+6位IMSI 若len=8 8字节IMSI
// 		// 结构 CA len direct ??+IMSI 
// 		if(packh.m_mac[3] == 2)
// 		{
// 			m_UserAccount2 = GetCdmaAccount(packh.m_mac);
// 		}
// 		else
// 		{
// 			m_UserAccount1 = GetCdmaAccount(packh.m_mac);
// 		}
// 		m_AuthType = 2;
// 		return 1;	// 匹配到1个帐号
// 	}
//	if(v_programtype != SYSTEM_FYY)//20100604 lp 此处所有都匹配，fyy时写对照时首先匹配客户表，否则用此处匹配的认证。虚拟身份只匹配认证表，此处匹配的信息不使用
	{
		/// fyy 不匹配认证信息, 由client表在SocketSession.cpp中输出文件时匹配
		if (packh.m_siptm > 0)
		{
			m_UserAccount1 = packh.m_sipUserName;
			m_Phone1 = packh.m_sipPhone;
			m_userImei1 = packh.m_sipimei;
			m_userCelllac1 = packh.m_sipcelllac;
			m_userCellCi1 = packh.m_sipcellci;

			account++;
			m_AuthType = 1;
		}

		if (packh.m_diptm > 0)
		{
			m_UserAccount2 = packh.m_dipUserName;
			m_Phone2 = packh.m_dipPhone;
			m_userImei2 = packh.m_dipimei;
			m_userCelllac2 = packh.m_dipcelllac;
			m_userCellCi2 = packh.m_dipcellci;
			account++;
			m_AuthType = 1;
		}

		//if (account == 0 )
		//{
		//	if(v_programtype != SYSTEM_ZONE)
		//	{
		// 最后就用VLan的帐号(VLanID)
		if (packh.m_vlanid != 0)
		{
			if(packh.m_vlanid2!=0)
			{
				m_vlanid.Format("%s_VLanID_%d_%d", v_ddelist.spyaddr,packh.m_vlanid,packh.m_vlanid2);
			}
			else
			{
				m_vlanid.Format("%s_VLanID_%d", v_ddelist.spyaddr,packh.m_vlanid);
			}
			
			//account++;
			//m_AuthType = 3;
		}
		//	}
		//}
	}

	return account;
}

//*************************************************
//	函数名称:		AuthInfo::SwapAuthInfo
//	描述:			交换认证信息
//	其它:			只有当m_AuthType==1和2时才交换
//*************************************************/
void AuthInfo::SwapAuthInfo()
{
	if ((m_AuthType != 0) && (m_AuthType != 3))
	{
		CString tmpstr;
		tmpstr = m_UserAccount1;
		m_UserAccount1 = m_UserAccount2;
		m_UserAccount2 = tmpstr;

		tmpstr = m_Phone1;
		m_Phone1 = m_Phone2;
		m_Phone2 = tmpstr;

	}
}



//#ifdef		DUMPHTTPPACKET

DumpPacket::DumpPacket()
{
	m_buf = NULL;
	m_datalen = 0;
	m_bCanDump = FALSE;
	m_FileSerial = 1;
//	InitDumpBuffer();
}

DumpPacket::~DumpPacket()
{
	if (m_file.m_hFile != CFile::hFileNull)
	{
		if ((m_buf!=NULL) && (m_datalen>0))
		{
			m_file.Write(m_buf, m_datalen);
			m_datalen = 0;
		}
		CString tmpfile = m_file.GetFilePath();
		m_file.Close();
		CString outfile = tmpfile + ".bak";
		DeleteFile(outfile);
		rename(tmpfile, outfile);
	}
	delete[] m_buf;
	m_buf = NULL;
	m_datalen = 0;
	m_bCanDump = FALSE;
	m_FileSerial = 1;
}

void DumpPacket::InitDumpBuffer()
{
//	if (0 == (_access("x:\\xxx\\flg\\dumphttp.flg", 0)))
	{
//		if (m_bCanDump)
		{
			m_bCanDump = TRUE;
			if (m_buf == NULL)
			{
				m_buf = new char[DUMPBUFFERSIZE];
				if (m_buf == NULL)
					return;
//				memset(m_buf, '?', DUMPBUFFERSIZE);
			}
			struct _stat fsta;
			CString str;
			CreateDir(v_datapath+"\\httpbak");
			time_t lastfiletm = time(0);
			DWORD serial = 0;
			for (m_FileSerial=0; m_FileSerial<DUMPFilECOUNT; m_FileSerial++)
			{
				str.Format("%s\\httpbak\\httpbak%u", v_datapath, m_FileSerial+1);
				if (!_stat(str+".bak", &fsta))
				{
					if (lastfiletm < fsta.st_mtime)
						serial = m_FileSerial;
				}
				else if (!_stat(str, &fsta))
				{
					if (lastfiletm < fsta.st_mtime)
						serial = m_FileSerial;
				}
				else
					break;
			}
			if (m_FileSerial>=DUMPFilECOUNT)
				m_FileSerial = serial;
		}
	}
// 	else
// 	{
// 		if (m_bCanDump == TRUE)
// 		{
// 			m_bCanDump = FALSE;
// 			SaveBuffer();
// 			m_file.Close();
// 			m_datalen = 0;
// 			delete[] m_buf;
// 			m_buf = NULL;
// 		}
// 	}
}

void DumpPacket::DumpOnePacket(DATEHEADER& packh, const char* pPacket)
{
	if ((m_bCanDump) && (packh.m_datalen>0))
	{
		if (m_buf==NULL)
		{
			m_buf = new char[DUMPBUFFERSIZE];
			if (m_buf == NULL)
				return;
//			memset(m_buf, '?', DUMPBUFFERSIZE);
			m_datalen = 0;
		}
		if (m_file.m_hFile == CFile::hFileNull)
		{
			CString filename;
			CreateDir(v_datapath+"\\httpbak");
			filename.Format("%s\\httpbak\\httpbak%u", v_datapath, (m_FileSerial%DUMPFilECOUNT)+1);
			m_file.Open(filename, CFile::modeWrite|CFile::modeCreate/*|CFile::modeNoTruncate*/);
			if (m_file.m_hFile == CFile::hFileNull)
				return;
			m_FileSerial++;
		}
		if ((packh.m_datalen + m_datalen + OLDHEADERLEN) >= DUMPBUFFERSIZE)
		{
			m_file.Write(m_buf, min(m_datalen, DUMPBUFFERSIZE));
			memset(m_buf, '?', DUMPBUFFERSIZE);
			m_datalen = 0;
			if (m_file.GetLength() >= DUMPFILESIZE)
			{
				CString tmpfile = m_file.GetFilePath();
				m_file.Close();
				CString outfile = tmpfile + ".bak";
				DeleteFile(outfile);
				rename(tmpfile, outfile);
			}
		}
		memcpy(m_buf+m_datalen, &packh.m_type2, OLDHEADERLEN);
		m_datalen += OLDHEADERLEN;
		memcpy(m_buf+m_datalen, pPacket, packh.m_datalen);
		m_datalen += packh.m_datalen;
	}
}

void DumpPacket::SaveBuffer()
{
	if (m_file.m_hFile != CFile::hFileNull)
	{
		if ((m_buf!=NULL) && (m_datalen>0))
		{
			m_file.Write(m_buf, min(m_datalen, DUMPBUFFERSIZE));
//			memset(m_buf, '?', DUMPBUFFERSIZE);
			m_datalen = 0;
			m_file.Flush();
		}
	}
}

DumpPacket	v_HttpDumpHttpPacket;

//#endif		DUMPHTTPPACKET

#define HISRECORD_COUNTFIELD	IDX_MARK+1
HistoryRecord::HistoryRecord()
{
	m_arrField.RemoveAll();
	m_arrField.SetSize(HISRECORD_COUNTFIELD);
	
	for(int idx = IDX_BEGTM; idx <= IDX_MARK; idx++)
		m_arrField.SetAt(idx, "");	// strtm
	m_arrField[IDX_COUNT	] = "1";
}

void HistoryRecord::SetRecord(int idx, const char *szValue)
{
	m_arrField.SetAt(idx, szValue);
}

BOOL HistoryRecord::GetHisRecord(CString &tempstr)
{
	int idirect = 0;
	CString skey, dkey;
	skey.Format("%s-%s", m_arrField[IDX_IP1], m_arrField[IDX_MAC1]);
	dkey.Format("%s-%s", m_arrField[IDX_IP2], m_arrField[IDX_MAC2]);
	sTzUnit *pUnit = v_fyyTzUnit.CheckTzUnit(skey, dkey, idirect);
	if(pUnit)
	{
// 		if(idirect == 1 && m_arrField[IDX_ACCOUNT1	].IsEmpty())
// 			m_arrField[IDX_ACCOUNT1	] = pUnit->m_account.c_str();
// 		else if(idirect == 2 && m_arrField[IDX_ACCOUNT2	].IsEmpty())
// 			m_arrField[IDX_ACCOUNT2	] = pUnit->m_account.c_str();
		if(idirect == 1 && m_arrField[IDX_ACCOUNT1	].IsEmpty())
		{
			if (!pUnit->m_cardno.empty())
			{
				m_arrField[IDX_ACCOUNT1	] = pUnit->m_cardno.c_str();
				m_arrField[IDX_AccountType1	]=pUnit->m_cardtype.c_str();
			}
			else if (!pUnit->m_umac.empty())
			{
				m_arrField[IDX_ACCOUNT1	].Format("%s",pUnit->m_umac.c_str());
				m_arrField[IDX_AccountType1	]="MAC地址";
			}
		//	v_ddelist.spyaddr
		//	m_arrField[IDX_ACCOUNT1	] = pUnit->m_account.c_str();
		}
		else if(idirect == 2 && m_arrField[IDX_ACCOUNT2	].IsEmpty())
		{
			if (!pUnit->m_cardno.empty())
			{
				m_arrField[IDX_ACCOUNT2	] = pUnit->m_cardno.c_str();
				m_arrField[IDX_AccountType2	]=pUnit->m_cardtype.c_str();
			}
			else if (!pUnit->m_umac.empty())
			{
				m_arrField[IDX_ACCOUNT2	].Format("%s",pUnit->m_umac.c_str());
				m_arrField[IDX_AccountType2	]="MAC地址";
			}
		//	m_arrField[IDX_ACCOUNT2	] = pUnit->m_account.c_str();
		}
	}

	tempstr.Empty();
	tempstr.Format(
		"%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1"
		"%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1"
		"%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1"
		"%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s",
		m_arrField[IDX_SPYADDR	],
		m_arrField[IDX_BEGTM	],
		m_arrField[IDX_ENDTM	],
		m_arrField[IDX_FORWTYPE	],
		m_arrField[IDX_IP1		],
		m_arrField[IDX_IP2		],
		m_arrField[IDX_PORT1	],
		m_arrField[IDX_PORT2	],
		m_arrField[IDX_ADDR1	],
		m_arrField[IDX_ADDR2	],
		m_arrField[IDX_ACCOUNT1	],
		m_arrField[IDX_PHONE1	],
		m_arrField[IDX_ACCOUNT2],
		m_arrField[IDX_PHONE2	],
		m_arrField[IDX_ID		],
		m_arrField[IDX_CONTACT	],
		m_arrField[IDX_PRO		],
		m_arrField[IDX_SUBPRO	],
		m_arrField[IDX_PFORMAT	],	
		m_arrField[IDX_UFORMAT	],
		m_arrField[IDX_UTYPE	],
		m_arrField[IDX_USER		],	
		m_arrField[IDX_PASS		],	
		m_arrField[IDX_URL		],
		m_arrField[IDX_DOMAIN	],
		m_arrField[IDX_STATE	],	
//		m_arrField[IDX_DATASRC	],	
		m_arrField[IDX_IFAGENT	],	
		m_arrField[IDX_CRCID	],	
		m_arrField[IDX_COUNT	],	
		m_arrField[IDX_MARK		],
		m_arrField[IDX_MAC1		],
		m_arrField[IDX_DATASRC	],
		m_arrField[IDX_AccountType1],
		m_arrField[IDX_AccountType2]
		);
	
	return TRUE;
}
// 	{"BEGTIME",  r4str, 20, 0}, ///< 开始时间
// 	{"ENDTIME",  r4str, 20, 0}, ///< 结束时间
// 	{"CRCID",  r4str, 40, 0}, ///< CRCID 
// 	{"ACCOUNT",  r4str, 30, 0}, ///< 帐号
// 	{"UFORMAT",  r4str, 50, 0}, ///< 对应user字段的拆隔串
// 	{"ID",   r4str, 50, 0}, ///< ID 
// 	{"Pro",  r4str, 20, 0}, ///< ID类型 
// 	{"PFORMAT",  r4str, 50, 0}, ///< 对应密码字段的拆隔符 
// 	{"PASS",     r4str, 30, 0}, ///< 密码
// 	{"IP",     r4str, 100, 0}, ///< 登录服务器
// 	{"COUNT",  r4str, 10, 0}, ///< 登陆次数
// 	{"DOMAIN",     r4str, 50,  0}, ///< 域名
// 	{"PHONE",  r4str, 30, 0}, ///< 电话2
// 	{ 0,0,0,0 }

BOOL HistoryRecord::GetWebfigRecord(CString &tempstr)
{
	CString tmpaccount;
	tmpaccount.Format("%s",m_arrField[IDX_ID	]);
	CString tmppro;
	tmppro.Format("%s",m_arrField[IDX_PRO	]);
	if (tmpaccount.IsEmpty())
	{
		tmpaccount.Format("%s",m_arrField[IDX_USER	]);
		if (tmpaccount.IsEmpty())
		{
			if(tmppro.CompareNoCase("httpup")!=0 &&
				tmppro.CompareNoCase("dynapassdown")!=0 &&
				tmppro.CompareNoCase("freegatedown")!=0 &&
				tmppro.CompareNoCase("freegateup")!=0 &&
				tmppro.CompareNoCase("freegateurl")!=0 &&
				tmppro.CompareNoCase("ultrabrowse")!=0)
			{
				return FALSE;
			}
			tmpaccount.Format("%s",m_arrField[IDX_PASS	]);//HTTP拆解只有pass也要入虚拟身份表
			if (tmppro.CompareNoCase("httpup")==0 &&
				tmpaccount.IsEmpty())
			{
				return FALSE;
			}
		}
		else
			SetRecord(IDX_ID, m_arrField[IDX_USER	]);
	}
	else if ((tmppro.CompareNoCase("pop3") == 0) && (tmpaccount.Find('@') < 0))
	{
		return FALSE;	// 虚拟身份中pop3的不入userid不包含'@'的数据
	}
	//非经营下虚拟身份，活动历时落地信息修改为 有身份证号用身份证号，无身份证号用 场所编码_mac
	int idirect = 0;
	CString skey, dkey;
	skey.Format("%s-%s", m_arrField[IDX_IP1], m_arrField[IDX_MAC1]);
	dkey.Format("%s-%s", m_arrField[IDX_IP2], m_arrField[IDX_MAC2]);
	sTzUnit *pUnit = v_fyyTzUnit.CheckTzUnit(skey, dkey, idirect);
	if(pUnit)
	{
		if(idirect == 1 && m_arrField[IDX_ACCOUNT1	].IsEmpty())
		{
			if (!pUnit->m_cardno.empty())
			{
				m_arrField[IDX_ACCOUNT1	] = pUnit->m_cardno.c_str();
				m_arrField[IDX_AccountType1	] = pUnit->m_cardtype.c_str();
			}
			else if (!pUnit->m_umac.empty())
			{
				m_arrField[IDX_ACCOUNT1	].Format("%s",pUnit->m_umac.c_str());
				m_arrField[IDX_AccountType1	] ="MAC地址";
			}
		//	v_ddelist.spyaddr
		//	m_arrField[IDX_ACCOUNT1	] = pUnit->m_account.c_str();
		}
		else if(idirect == 2 && m_arrField[IDX_ACCOUNT2	].IsEmpty())
		{
			if (!pUnit->m_cardno.empty())
			{
				m_arrField[IDX_ACCOUNT2	] = pUnit->m_cardno.c_str();
				m_arrField[IDX_AccountType2	] = pUnit->m_cardtype.c_str();
			}
			else if (!pUnit->m_umac.empty())
			{
				m_arrField[IDX_ACCOUNT2	].Format("%s",pUnit->m_umac.c_str());
				m_arrField[IDX_AccountType2	] ="MAC地址";
			}
		//	m_arrField[IDX_ACCOUNT2	] = pUnit->m_account.c_str();
		}
	}

	if(m_arrField[IDX_ACCOUNT1].IsEmpty() && m_arrField[IDX_ID].IsEmpty())
		return FALSE;		/// 分配字段全为空, 此虚拟身份无效
	
	tempstr.Empty();
	tempstr.Format(
		"%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1"
		"%d\1%s\1%s\1%s\1%s\1%s\1%s\1%s\1%s",
		m_arrField[IDX_SPYADDR	],
		m_arrField[IDX_BEGTM	],
		m_arrField[IDX_ENDTM	],
		m_arrField[IDX_SUBPRO	],
		m_arrField[IDX_ACCOUNT1	],	
		m_arrField[IDX_ID		],		
		m_arrField[IDX_PFORMAT	],
		m_arrField[IDX_UFORMAT	],
		m_arrField[IDX_UTYPE	],
		m_arrField[IDX_CRCID	],
		1,
		m_arrField[IDX_IP2		],
//		m_arrField[IDX_CONTACT	]1,
		m_arrField[IDX_DOMAIN	],
		m_arrField[IDX_PHONE1	],
		m_arrField[IDX_PASS		],
		m_arrField[IDX_IP1		],
		m_arrField[IDX_MAC1		],
		m_arrField[IDX_DATASRC	],
		m_arrField[IDX_AccountType1]


// 		m_arrField[IDX_ACCOUNT2],
// 		m_arrField[IDX_PHONE2	],
// 		m_arrField[IDX_UFORMAT	],	
// 		m_arrField[IDX_PFORMAT	],	
// 		m_arrField[IDX_UTYPE	],	
// 		m_arrField[IDX_PTYPE	],
// 		m_arrField[IDX_DATASRC	],	
// 		m_arrField[IDX_CRCID	],	
// 		m_arrField[IDX_COUNT	],	
// 		m_arrField[IDX_MARK		]
		);
	
	return TRUE;
}

BOOL HistoryRecord::GetWebidRecord( CString &tempstr )
{
	CString tmpaccount;
	tmpaccount.Format("%s",m_arrField[IDX_ID	]);
	CString tmppro;
	tmppro.Format("%s",m_arrField[IDX_PRO	]);
	if (tmpaccount.IsEmpty())
	{
		tmpaccount.Format("%s",m_arrField[IDX_USER	]);
		if (tmpaccount.IsEmpty())
		{
			if( tmppro.CompareNoCase("dynapassdown")!=0 &&
				tmppro.CompareNoCase("freegatedown")!=0 &&
				tmppro.CompareNoCase("freegateup")!=0 &&
				tmppro.CompareNoCase("freegateurl")!=0 &&
				tmppro.CompareNoCase("ultrabrowse")!=0 &&
				tmppro.CompareNoCase("skype")!=0 &&
				tmppro.CollateNoCase("imsg_taobao")!=0)
			{
				return FALSE;
			}
		}
		else
		{
			SetRecord(IDX_ID, m_arrField[IDX_USER	]);
		}
	}
	else if ((tmppro.CompareNoCase("pop3") == 0) && (tmpaccount.Find('@') < 0))
	{
		return FALSE;	// 虚拟身份中pop3的不入userid不包含'@'的数据
	}
	//非经营下虚拟身份，活动历时落地信息修改为 有身份证号用身份证号，无身份证号用 场所编码_mac
	int idirect = 0;
	CString skey, dkey;
	skey.Format("%s-%s", m_arrField[IDX_IP1], m_arrField[IDX_MAC1]);
	dkey.Format("%s-%s", m_arrField[IDX_IP2], m_arrField[IDX_MAC2]);
	sTzUnit *pUnit = v_fyyTzUnit.CheckTzUnit(skey, dkey, idirect);
	if(pUnit)
	{
		if(idirect == 1 && m_arrField[IDX_ACCOUNT1	].IsEmpty())
		{
			if(FALSE == pUnit->m_umac.empty())
				SetRecord(IDX_CLIENTMAC, pUnit->m_umac.c_str());

			if (!pUnit->m_cardno.empty())
			{
				m_arrField[IDX_ACCOUNT1	] = pUnit->m_cardno.c_str();
				m_arrField[IDX_AccountType1	] = pUnit->m_cardtype.c_str();
			}
			else if (!pUnit->m_umac.empty())
			{
				m_arrField[IDX_ACCOUNT1	].Format("%s",pUnit->m_umac.c_str());
				m_arrField[IDX_AccountType1	] ="MAC地址";
			}
		}
		else if(idirect == 2 && m_arrField[IDX_ACCOUNT2	].IsEmpty())
		{
			if (!pUnit->m_cardno.empty())
			{
				m_arrField[IDX_ACCOUNT2	] = pUnit->m_cardno.c_str();
				m_arrField[IDX_AccountType2	] = pUnit->m_cardtype.c_str();
			}
			else if (!pUnit->m_umac.empty())
			{
				m_arrField[IDX_ACCOUNT2	].Format("%s",pUnit->m_umac.c_str());
				m_arrField[IDX_AccountType2	] ="MAC地址";
			}
		}
	}

	if(m_arrField[IDX_ACCOUNT1].IsEmpty() && m_arrField[IDX_ID].IsEmpty())
		return FALSE;		/// 分配字段全为空, 此虚拟身份无效


	if (m_bWiFiRouter == false)
	{
		
		//	截获点\twebfigureid\t协议\t时间\t落地信息类型\t
		//	落地信息\t电话\t密码\t异情类别\t联系人\t
		//	源IP\t目的IP\t域名\t状态\t服务名称\t
//	源Mac\t\r\n
		tempstr.Empty();
		tempstr.Format(
			"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t"
			"%s\t%s\t%s\t%s\t%s\t%s",
			m_arrField[IDX_SPYADDR	],
			m_arrField[IDX_ID		],
			m_arrField[IDX_SUBPRO	],
			m_arrField[IDX_BEGTM	],
			m_arrField[IDX_AccountType1	],
			m_arrField[IDX_ACCOUNT1	],	
			m_arrField[IDX_PHONE1	],		
			m_arrField[IDX_PASS	],
			"",
			m_arrField[IDX_CONTACT	],
			m_arrField[IDX_IP1	],
			m_arrField[IDX_IP2	],
			m_arrField[IDX_DOMAIN	],
			m_arrField[IDX_STATE	],
			m_arrField[IDX_UTYPE	],
			m_arrField[IDX_CLIENTMAC]
			);

	}

	
	if (m_bWiFiRouter == true)
	{
		//	截获点\twebfigureid\t协议\t时间\t落地信息类型\t
		//	落地信息\t电话\t密码\t异情类别\t联系人\t
		//	源IP\t目的IP\t域名\t状态\t服务名称\t
		//	源Mac\tDstMac\tWiFiRouterName\tWiFiRouterLng\tWiFiRouterLat\t\r\n

		CString wifimac(m_arrField[IDX_MAC2].GetBuffer(0));
		WiFiRouterInfo *pi = GetWiFiRouterInfo(wifimac);
		if (pi != NULL)
		{
			tempstr.Empty();
			tempstr.Format(
				"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t"
				"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
				m_arrField[IDX_SPYADDR	],
				m_arrField[IDX_ID		],
				m_arrField[IDX_SUBPRO	],
				m_arrField[IDX_BEGTM	],
				m_arrField[IDX_AccountType1	],
				m_arrField[IDX_ACCOUNT1	],	
				m_arrField[IDX_PHONE1	],		
				m_arrField[IDX_PASS	],
				"",
				m_arrField[IDX_CONTACT	],
				m_arrField[IDX_IP1	],
				m_arrField[IDX_IP2	],
				m_arrField[IDX_DOMAIN	],
				m_arrField[IDX_STATE	],
				m_arrField[IDX_UTYPE	],
				m_arrField[IDX_MAC1],
				m_arrField[IDX_MAC2],
				pi->Name,
				pi->Lng,
				pi->Lat
			);
		}
		else
		{
			tempstr.Empty();
			tempstr.Format(
				"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t"
				"%s\t%s\t%s\t%s\t%s\t%s",
				m_arrField[IDX_SPYADDR	],
				m_arrField[IDX_ID		],
				m_arrField[IDX_SUBPRO	],
				m_arrField[IDX_BEGTM	],
				m_arrField[IDX_AccountType1	],
				m_arrField[IDX_ACCOUNT1	],	
				m_arrField[IDX_PHONE1	],		
				m_arrField[IDX_PASS	],
				"",
				m_arrField[IDX_CONTACT	],
				m_arrField[IDX_IP1	],
				m_arrField[IDX_IP2	],
				m_arrField[IDX_DOMAIN	],
				m_arrField[IDX_STATE	],
				m_arrField[IDX_UTYPE	],
				m_arrField[IDX_CLIENTMAC]
			);
		}

		DWORD nowtm = GetTickCount();
		CString opath;
		opath.Format("v:\\netspy\\statdata\\webfiguremac\\127.0.0.1\\%d.bcp",nowtm);
		FILE *fp = fopen(opath,"a+");
		if (fp)
		{
			fwrite(tempstr.GetBuffer(0),1,tempstr.GetLength(),fp);
			fclose(fp);
		}
		tempstr.Empty();
			return FALSE;
		
	}

	
	return TRUE;	
}

BOOL HistoryRecord::GetAccount( CString &tempstr )
{
	tempstr.Empty();
	if (m_arrField[IDX_AccountType1]=="上网帐号")
	{
		tempstr.Format("%s\1%s",m_arrField[IDX_ACCOUNT1],"1");	
	}
	else if (m_arrField[IDX_AccountType1]=="MAC地址")
	{
		tempstr.Format("%s\1%s",m_arrField[IDX_ACCOUNT1],"3");	
	}
	else 
	{
		tempstr.Format("%s\1%s",m_arrField[IDX_ACCOUNT1],"4");	
	}
//	tempstr.Format("%s\1%s",m_arrField[IDX_ACCOUNT1],m_arrField[IDX_AccountType1]);	
	return TRUE;
}

map<CString,WiFiRouterInfo *> HistoryRecord::s_mapWiFiRouter = map<CString, WiFiRouterInfo *>();
bool HistoryRecord::m_bWiFiRouter = false;
void HistoryRecord::LoadWiFiRouter()
{

	const char *pathfolder1 = "v:\\netspy\\statdata\\webfiguremac\\";
	

	if(_access(pathfolder1,0)!=0)
	{
		mkdir(pathfolder1);
		const char *pathfolder2 = "v:\\netspy\\statdata\\webfiguremac\\127.0.0.1\\";
		if(_access(pathfolder2,0)!=0)
		{
			mkdir(pathfolder2);
		}
	}


	if (_access("e:\\netspy\\flg\\webfiguremac.flg",0) == 0)
	{
		m_bWiFiRouter = true;
	}
	else
	{
		m_bWiFiRouter = false;
	}
	const char *pathtxt = "e:\\netspy\\para\\wifirouter.txt";
	FILE *fp = ::fopen(pathtxt,"rb");
	if (fp)
	{
		while ( !feof(fp))
		{
			char tmp[512] = {0};
			::fgets(tmp,512,fp);
			CString strRaw(tmp);
			CStringArray dstArr;
			Splitstr(strRaw,dstArr,',');
			if (dstArr.GetSize() == 4)
			{
				CString mac = dstArr.GetAt(0);
				WiFiRouterInfo *p = new WiFiRouterInfo();
				p->Name = dstArr.GetAt(1);
				p->Lng = dstArr.GetAt(2);
				p->Lat = dstArr.GetAt(3);
				s_mapWiFiRouter.insert(make_pair(mac,p));
			}
		}
	}



}

WiFiRouterInfo *HistoryRecord::GetWiFiRouterInfo(const CString &WiFiRouterMac)
{
	map<CString, WiFiRouterInfo *>::iterator iter = s_mapWiFiRouter.find(WiFiRouterMac);
	if (iter !=  s_mapWiFiRouter.end())
	{
		return iter->second;
	}
	return NULL;
}

