
#include "stdafx.h"
#include "ReadDDeFile.h"
#include "SocketSession.h"
#include "DceMission.h"
#include "unip.h"
#include "UnipSession.h"
// #include "..\\Include\\d4all.hpp"
#include "IpQueryLib.h"
#include "zwsqldb.h"
#include "DCE.h"

UnipSession::UnipSession(DATEHEADER& packh, int direction, BOOL isCDMA)
{
	m_spyaddr = "";		// 截获位置在ProcessPacket()中赋值
	m_begtime = m_endtime = packh.m_tm;
	m_vlanid = packh.m_vlanid;
	m_limittype = 0;
	if ((packh.m_type==TCPPacket) || (packh.m_type==UnipTCP))
		m_pro = 'T';
	else
		m_pro = 'U';
	if (direction & 3)
		m_iscase = 1;
	else
		m_iscase = 0;
	if (direction & 4)
		m_isindex = 1;
	else
		m_isindex = 0;
	m_direction = (direction>4)? 2 : direction&3;
	if (m_direction == 2)
	{
		memcpy(m_sip, packh.m_dip, 4);
		memcpy(m_dip, packh.m_sip, 4);
		m_sport = packh.m_dport;
		m_dport = packh.m_sport;
		if (!isCDMA)
			FormatMac(packh.m_mac, m_smac, m_dmac, packh);
		m_packetnum1 = 0;
		m_bytenum1 = 0;
		m_packetnum2 = 1;
		m_bytenum2 = packh.m_datalen;
	}
	else
	{
		memcpy(m_sip, packh.m_sip, 4);
		memcpy(m_dip, packh.m_dip, 4);
		m_sport = packh.m_sport;
		m_dport = packh.m_dport;
		if (!isCDMA)
			FormatMac(packh.m_mac, m_smac, m_dmac, packh);
		m_packetnum1 = 1;
		m_bytenum1 = packh.m_datalen;
		m_packetnum2 = 0;
		m_bytenum2 = 0;
 	}
}

// 创建UNIP会话并匹配数据包是否是案件或是预设端口
// 如果是案件或是预设端口就返回一个新的会话的指针否则返回NULL
UnipSession* UnipSession::CreateUnipSession(DATEHEADER& packh, BOOL isCDMA)
{
	char sip[32];
	char dip[32];
	memset(sip+12, 0, 4);
	sprintf(sip, "%03u.%03u.%03u.%03u", packh.m_sip[0],
		packh.m_sip[1], packh.m_sip[2], packh.m_sip[3]);
	memset(dip+12, 0, 4);
	sprintf(dip, "%03u.%03u.%03u.%03u", packh.m_dip[0],
		packh.m_dip[1], packh.m_dip[2], packh.m_dip[3]);

	// 首先匹配案件
	AuthInfo aaa;
	int direction = 0;
	CaseInfoArray caseinfoarr;

	// 获取并匹配认证
	aaa.GetAuthInfo(packh, isCDMA); // by lyb

	// 匹配认证
	if(SearchCaseByAAA(aaa.m_Phone1, aaa.m_UserAccount1, caseinfoarr, NULL) )
		direction |= 1;
	if(SearchCaseByAAA(aaa.m_Phone2, aaa.m_UserAccount2, caseinfoarr, NULL))
		direction |= 2;


	// 匹配MAC
	CString smac, dmac;
	if (!isCDMA)
	{
		FormatMac(packh.m_mac, smac, dmac, packh);
		if (v_opencaseunip)
		{
			int d = SearchCaseByMAC(smac, dmac, caseinfoarr);
			if (d != 0)
				direction = d;
		}
	}

	// 匹配IP
	if (v_opencaseunip)
	{
		int d = SearchCaseByIP(sip, dip, caseinfoarr);
		if (d != 0)
			direction = d;
	}
	CStringArray tmpwarnidarr;
	if (v_programtype == SYSTEM_SLEUTH)
	{
		if (caseinfoarr.GetSize()==0)
		{
			if (v_openflagunip)
			{
				DWORD SipPower = 0;
				DWORD DipPower = 0;
				g_ipMap.Lookup(*((DWORD*)packh.m_sip), SipPower);
				g_ipMap.Lookup(*((DWORD*)packh.m_dip), DipPower);
				if ((SipPower < DipPower) && (aaa.m_UserAccount1.IsEmpty()))
					direction = 8+4;
				else
					direction = 4;
				SipPower++;
				DipPower++;
				g_ipMap.SetAt(*((DWORD*)packh.m_sip), SipPower);
				g_ipMap.SetAt(*((DWORD*)packh.m_dip), DipPower);
			}
			else
			{
				return NULL;
			}
		}
		else if (direction == 0)
		{
			if (aaa.m_UserAccount2.IsEmpty())
			{
				direction = 1;
			}
			else
			{
				direction = 2;
			}
		}
	}
	else
	{
//		// 匹配预警数据(目前该协议的不处理预警)

		WarnlimitMap& warnlimitMap = v_warnlimitPara.m_warnlimitMap[v_CaseLimitPara.m_readflg]; //重点人员帐号
		Lpwarnkeyinfo lpwarnkeyinfo = NULL;
		//找出所有符合账号，ip，mac的所有重点人员来，放到m_WarnlimitMap中，匹配上关键字之后需要与这个map匹配
		//判断是否中上重点人
		//先处理帐号
		if (warnlimitMap.Lookup(sip, lpwarnkeyinfo))
		{
			while (lpwarnkeyinfo != NULL)
			{
				tmpwarnidarr.Add(lpwarnkeyinfo->m_id);
				lpwarnkeyinfo = lpwarnkeyinfo->m_next;
			}
			direction |= 8;
		}
		if (warnlimitMap.Lookup(dip, lpwarnkeyinfo))
		{
			while (lpwarnkeyinfo != NULL)
			{
				tmpwarnidarr.Add(lpwarnkeyinfo->m_id);
				lpwarnkeyinfo = lpwarnkeyinfo->m_next;
			}
			direction |= 8;
		}
		if (warnlimitMap.Lookup(aaa.m_UserAccount1,lpwarnkeyinfo))
		{
			while (lpwarnkeyinfo != NULL)
			{
				tmpwarnidarr.Add(lpwarnkeyinfo->m_id);
				lpwarnkeyinfo = lpwarnkeyinfo->m_next;
			}
			direction |= 8;
		}


		// 匹配回溯
		if (v_openflagunip && v_IndexLimitPara.CheckRule(sip, dip, packh.m_sport, packh.m_dport))
		{
			// 匹配屏蔽信息
			if (SearchBurrishByIP(sip, dip)
				|| SearchBruuishByAAA(aaa.m_Phone1, aaa.m_UserAccount1)
				|| SearchBruuishByAAA(aaa.m_Phone2, aaa.m_UserAccount2))
			{
				return NULL;	// 
			}

			direction |= 4;
		}
	}

	if (direction)
	{
		UnipSession* s = new UnipSession(packh, direction, isCDMA);
		if (s->m_direction==2)
		{
			s->m_authinfo.m_UserAccount1 = aaa.m_UserAccount2;
			s->m_authinfo.m_Phone1 = aaa.m_Phone2;
			s->m_authinfo.m_UserAccount2 = aaa.m_UserAccount1;
			s->m_authinfo.m_Phone2 = aaa.m_Phone1;
		}
		else
		{
			s->m_authinfo = aaa;
		}

		if (caseinfoarr.GetSize()>0)
		{
			s->m_CaseInfo.Append(caseinfoarr);
		}
		if (tmpwarnidarr.GetSize()>0)
		{
			s->m_WarnInfo.Append(tmpwarnidarr);
		}
		return s;
	}
	else
	{
		return NULL;
	}
}

void UnipSession::AddPacket(DATEHEADER& packh)
{
	if (memcmp(m_sip, packh.m_sip, 4) == 0)
	{
		m_packetnum1++;
		m_bytenum1+=packh.m_datalen;
	}
	else
	{
		m_packetnum2++;
		m_bytenum2+=packh.m_datalen;
	}
	m_endtime = packh.m_tm;
}

void UnipSession::WriteUnipFile(LPCTSTR tmpdir, UnipProtocol* pProObject)
{
	static long fid = 0;
	if (fid >= 100000)
	{
		fid = 0;
	}
	// 写通用信息
 	char str[4000];
//	memset(str, 0, sizeof(str));
 	CString fileKey;
 	int len = 0;
	CString strsport,strdport,strsip,strdip;
	strsport.Format("%d",m_sport);
	strdport.Format("%d",m_dport);
	strsip.Format("%03u.%03u.%03u.%03u",m_sip[0], m_sip[1], m_sip[2], m_sip[3]);
	strdip.Format("%03u.%03u.%03u.%03u",m_dip[0], m_dip[1], m_dip[2], m_dip[3]);
	
// | accounttype | int(3)       | YES  |     | NULL    |                |
// | account     | varchar(50)  | YES  |     | NULL    |                |
// | areatype    | int(1)       | YES  |     | 0       |                |
// | num         | int(11)      | NO   |     | 1       |                |
// | ip         | int(11)      | YES  |     | NULL    |                |
// | tm          | int(11)      | NO   | MUL | NULL    |    20100615    |

	CString strindexlimit;
	CString straccount1,straccount2,straccounttype1,straccounttype2;


	len = sprintf(str, "pro= unip\r\nspyaddr= %s\r\nprotocol= %s\r\n",
		v_ddelist.spyaddr, (m_pro=='U'?"UDP":"TCP"));
	len += sprintf(str+len, "tm= %s\r\nbegtm= %s\r\nendtm= %s\r\n",
		GetTimeString(m_begtime, "%Y-%m-%d %H:%M:%S"),
		GetTimeString(m_begtime, "%Y-%m-%d %H:%M:%S"),
		GetTimeString(m_endtime, "%Y-%m-%d %H:%M:%S"));
	len += sprintf(str+len, "sip= %03u_%03u_%03u_%03u\r\nsport= %05d\r\n", 
		m_sip[0], m_sip[1], m_sip[2], m_sip[3], m_sport);
	len += sprintf(str+len, "dip= %03u_%03u_%03u_%03u\r\ndport= %05d\r\n", 
		m_dip[0], m_dip[1], m_dip[2], m_dip[3], m_dport);

	CString sipaddr = k_IpQueryTable.QueryIpAddr(Swap4Byte(*((DWORD*)m_sip)));
	CString dipaddr = k_IpQueryTable.QueryIpAddr(Swap4Byte(*((DWORD*)m_dip)));
	if (!sipaddr.IsEmpty())
		len += sprintf(str+len, "sipaddr= %s\r\n", sipaddr);
	if (!dipaddr.IsEmpty())
		len += sprintf(str+len, "dipaddr= %s\r\n", dipaddr);

	len += sprintf(str+len, "datasrc= %s\r\n",	GetDataSrc(m_DataSrc));
	if (!m_ISP.IsEmpty())
		len += sprintf(str+len, "ISP= %s\r\n", m_ISP);
	if (m_DataSrc != DATASRC_CDMA)
	{
		len += sprintf(str+len, "smac= %s\r\ndmac= %s\r\n", m_smac, m_dmac);
	}

	if (!m_authinfo.m_UserAccount1.IsEmpty())
	{
		len += sprintf(str+len, "account= %s\r\n",	m_authinfo.m_UserAccount1);
		straccount1=m_authinfo.m_UserAccount1;
		straccounttype1.Format("1");
	}

	if (!m_authinfo.m_Phone1.IsEmpty())
	{
		len += sprintf(str+len, "phone= %s\r\n",	m_authinfo.m_Phone1);
	}

	if (!m_authinfo.m_userImei1.IsEmpty())
	{
		len += sprintf(str+len, "imei= %s\r\n",	   m_authinfo.m_userImei1);
	}

	if (!m_authinfo.m_userCelllac1.IsEmpty())
	{
		len += sprintf(str+len, "celllac= %s\r\n",	m_authinfo.m_userCelllac1);
	}
	
	if (!m_authinfo.m_userCellCi1.IsEmpty())
	{
		len += sprintf(str+len, "cellci= %s\r\n",	m_authinfo.m_userCellCi1);
	}

	if (!m_authinfo.m_userCellCi1.IsEmpty() && !m_authinfo.m_userCelllac1.IsEmpty())
	{
		BasicInfoStruct tmpbasicinfo;
		CString      tmpkey;

		tmpkey.Format("%s%s",m_authinfo.m_userCelllac1,m_authinfo.m_userCellCi1);
		if(m_basicinfomap.Lookup(tmpkey,tmpbasicinfo))
		{
			len += sprintf(str+len, "sitename= %s\r\n",	tmpbasicinfo.name);
			len += sprintf(str+len, "siteaddress= %s\r\n",	tmpbasicinfo.address);
			len += sprintf(str+len, "longitude= %s\r\n",	tmpbasicinfo.longitude);
			len += sprintf(str+len, "latitude= %s\r\n",	tmpbasicinfo.latitude);
		}
	}

	if (!m_authinfo.m_UserAccount2.IsEmpty())
	{
		len += sprintf(str+len, "2account= %s\r\n",	m_authinfo.m_UserAccount2);
		straccount2=m_authinfo.m_UserAccount2;
		straccounttype2.Format("1");
	}

	if (!m_authinfo.m_Phone2.IsEmpty())
	{
		len += sprintf(str+len, "2phone= %s\r\n",	m_authinfo.m_Phone2);
	}

	if (!m_authinfo.m_userImei2.IsEmpty())
	{
		len += sprintf(str+len, "2imei= %s\r\n",	   m_authinfo.m_userImei2);
	}

	if (!m_authinfo.m_userCelllac2.IsEmpty())
	{
		len += sprintf(str+len, "2celllac= %s\r\n",	m_authinfo.m_userCelllac2);
	}
	
	if (!m_authinfo.m_userCellCi2.IsEmpty())
	{
		len += sprintf(str+len, "2cellci= %s\r\n",	m_authinfo.m_userCellCi2);
	}

	if (!m_authinfo.m_userCellCi2.IsEmpty() && !m_authinfo.m_userCelllac2.IsEmpty())
	{
		BasicInfoStruct tmpbasicinfo;
		CString      tmpkey;

		tmpkey.Format("%s%s",m_authinfo.m_userCelllac2,m_authinfo.m_userCellCi2);
		if(m_basicinfomap.Lookup(tmpkey,tmpbasicinfo))
		{
			len += sprintf(str+len, "2sitename= %s\r\n",	tmpbasicinfo.name);
			len += sprintf(str+len, "2siteaddress= %s\r\n",	tmpbasicinfo.address);
			len += sprintf(str+len, "2longitude= %s\r\n",	tmpbasicinfo.longitude);
			len += sprintf(str+len, "2latitude= %s\r\n",	tmpbasicinfo.latitude);
		}
	}

	
	len += sprintf(str+len, "packetnum1= %u\r\npacketnum2= %u\r\npacketbyte1= %u\r\npacketbyte2= %u\r\n",
		m_packetnum1, m_packetnum2, m_bytenum1, m_bytenum2);

	if(v_programtype == SYSTEM_FYY||
		v_programtype == SYSTEM_DEPARTMENT||
		v_programtype == SYSTEM_HOTEL)
	{
		/// 附加FyyTz信息
		int ifyydirect = 0;
		CString skey, dkey;
		skey.Format("%03u.%03u.%03u.%03u-%s", m_sip[0], m_sip[1], m_sip[2], m_sip[3], m_smac);
		dkey.Format("%03u.%03u.%03u.%03u-%s", m_dip[0], m_dip[1], m_dip[2], m_dip[3], m_dmac);
		sTzUnit *pUnit = v_fyyTzUnit.CheckTzUnit(skey, dkey, ifyydirect);
		if(pUnit)
		{
			char buffer[1024];
			int lena = 0;
			if(pUnit->m_cardno.size()>0)
			{
				lena += WriteField(buffer+lena, "account",	pUnit->m_cardno.c_str());
				lena += WriteField(buffer+lena, "accounttype",pUnit->m_cardtype.c_str());
				straccount1.Format("%s",pUnit->m_cardno.c_str());
				straccounttype1.Format("4");
			}
			else 
			{
				lena += WriteField(buffer+lena, "account",pUnit->m_umac.c_str()	);
				lena += WriteField(buffer+lena, "accounttype","MAC地址");
				straccount1.Format("%s",pUnit->m_cardno.c_str());
				straccounttype1.Format("3");
			}
			lena += WriteField(buffer+lena, "cardtype",	pUnit->m_cardtype.c_str());
			lena += WriteField(buffer+lena, "cardno",		pUnit->m_cardno.c_str());
			lena += WriteField(buffer+lena, "username",	pUnit->m_username.c_str());
			lena += WriteField(buffer+lena, "terminalno",	pUnit->m_roomid.c_str());
			lena += WriteField(buffer+lena, "country",	pUnit->m_country.c_str());
	// 		len += WriteField(buffer+len, "account",	pUnit->m_account.c_str());
			if(lena > 0)
				len += sprintf(str+len, "%s", buffer);
		}
	}

	long	bufid = v_IdexlimitID;
	sindexlimit tindexlimit;
	if (v_Indexlimtmap[bufid].Lookup(strsip,tindexlimit))
	{
		if (tindexlimit.typeinfo==8)
		{
			strindexlimit.Format("%d\1%s\1%s\1%s\1%s\1%s\1",time(0),tindexlimit.no,straccount2,straccounttype2,GetDataSrcDword(m_DataSrc),strdip);
		}
		else if (tindexlimit.typeinfo==52)
		{
			if (tindexlimit.m_port==strsport)
			{
				strindexlimit.Format("%d\1%s\1%s\1%s\1%s\1%s\1",time(0),tindexlimit.no,straccounttype2,straccount2,GetDataSrcDword(m_DataSrc),strdip);
			}
		}
	}
	else if (v_Indexlimtmap[bufid].Lookup(strdip,tindexlimit))
	{
		if (tindexlimit.typeinfo==8)
		{
			strindexlimit.Format("%d\1%s\1%s\1%s\1%s\1%s\1",time(0),tindexlimit.no,straccounttype1,straccount1,GetDataSrcDword(m_DataSrc),strsip);
		}
		else if (tindexlimit.typeinfo==52)
		{
			if (tindexlimit.m_port==strsport)
			{
				strindexlimit.Format("%d\1%s\1%s\1%s\1%s\1%s\1",time(0),tindexlimit.no,straccounttype1,straccount1,GetDataSrcDword(m_DataSrc),strsip);
			}
		}
	}
	else if (v_Indexlimtmap[bufid].Lookup(strsport,tindexlimit))
	{
		if (tindexlimit.typeinfo==49)
		{
			strindexlimit.Format("%d\1%s\1%s\1%s\1%s\1%s\1",time(0),tindexlimit.no,straccounttype2,straccount2,GetDataSrcDword(m_DataSrc),strdip);
		}
	}
	else if (v_Indexlimtmap[bufid].Lookup(strdport,tindexlimit))
	{
		if (tindexlimit.typeinfo==49)
		{
			strindexlimit.Format("%d\1%s\1%s\1%s\1%s\1%s\1",time(0),tindexlimit.no,straccounttype1,straccount1,GetDataSrcDword(m_DataSrc),strsip);
		}
	}
	if (!strindexlimit.IsEmpty())
	{
		if (m_WarnInfo.GetSize()>0)
		{
			for (int mm=0;mm<m_WarnInfo.GetSize();mm++)
			{
				CString tmpindexlimit;
				tmpindexlimit.Format("%s%s",strindexlimit,m_WarnInfo.GetAt(mm));
				WriteArrToData(UNIP_PROTOCOL, UNIP_DATA, tmpindexlimit);
			}
			WriteArrToData(UNIP_PROTOCOL, UNIP_DATA, strindexlimit);
		}
		else
		{
			WriteArrToData(UNIP_PROTOCOL, UNIP_DATA, strindexlimit);
		}
	}

	// 生成对照文件名
	SYSTEMTIME nowtm;
	GetLocalTime(&nowtm);
	char fname[MAX_PATH];

	// 写案件数据
	for (int i=0; i<m_CaseInfo.GetSize(); i++)
	{
		CaseInfo& caseinfo = m_CaseInfo.ElementAt(i);
		if (v_programtype == SYSTEM_SLEUTH)
		{
			fileKey.Format("%03u.%03u.%03u.%03u_%s_unip",
				m_sip[0], m_sip[1], m_sip[2], m_sip[3], caseinfo.CaseID);
		}
		else
		{
			fileKey = caseinfo.CaseID + "_unip";
		}
		int tlen = len;
		tlen += sprintf(str+tlen, "caseid= %s\r\nobjid= %s\r\n",
			caseinfo.CaseID, caseinfo.ObjectID);
		tlen += sprintf(str+tlen, "limitaddr= %s\r\nlimittype= %d\r\n",
			caseinfo.LimitAddr,
			(caseinfo.LimitType>100)? caseinfo.LimitType-100 : caseinfo.LimitType);

		if ((caseinfo.LimitType>100) && (v_programtype == SYSTEM_SLEUTH))
		{
			memcpy(str+len, "扩展=TRUE\r\n", 11);
			tlen += 11;
		}
		
		// 输出文件
		sprintf(fname, "%04d%02d%02d%02d%02d%02d_unip_%u_%u.indexfile.txt",
			nowtm.wYear, nowtm.wMonth, nowtm.wDay, nowtm.wHour, nowtm.wMinute, nowtm.wSecond,
			nowtm.wMilliseconds+rand(), ++fid);
		LPIpDataStruct pval = NULL;
		pval = g_pUnipMissObj->m_pSubPro->AllocFileToDse(1, 0);
		ASSERT(pval != NULL);
		DceFileStream StreamInfo;
		memset(&StreamInfo, 0, sizeof(StreamInfo));
		strcpy(StreamInfo.CaseIDorWarnID, caseinfo.CaseID);
		memcpy(StreamInfo.Pro, "unip", 4);
		StreamInfo.Type = 1;		// 案件数据
		BOOL bSucc = g_pUnipMissObj->m_pSubPro->MergeMemDataToStream(pval, &StreamInfo,
			str, tlen, fname);
		
		// 保持流文件在线
		if ((!bSucc) || (ftell(pval->m_prealfile) >= 2*1024*1024))
		{
			// 提交流文件
			int re = fclose(pval->m_prealfile);
			if (re != 0)
			{
				WriteLog("dce", "关闭文件<%s>失败! ErrCode = %d", pval->m_prealfile, re);
			}
			fclose(pval->m_prealfile);
			pval->m_prealfile = NULL;
			re = fclose(pval->m_pindexfile);
			if (re != 0)
			{
				WriteLog("dce", "关闭文件<%s>失败! ErrCode = %d", pval->m_indexfname, re);
			}
			pval->m_pindexfile = NULL;
			CString idxfname = pval->m_indexfname;
			CString ralfname = pval->m_realfname;
			idxfname.Replace("tmp20", "20");
			ralfname.Replace("tmp20", "20");
//			rename(pval->m_realfname, ralfname);
			re = rename(pval->m_realfname, ralfname);
			if (re != 0)
			{
				::Sleep(200);
				re = rename(pval->m_realfname, ralfname);
				if (re != 0)
				{
					pProObject->m_TmpFileArr.Add(pval->m_realfname);
				}
			}
//			rename(pval->m_indexfname, idxfname);
			re = rename(pval->m_indexfname, idxfname);
			if (re != 0)
			{
				::Sleep(200);
				re = rename(pval->m_indexfname, idxfname);
				if (re != 0)
				{
					pProObject->m_TmpFileArr.Add(pval->m_indexfname);
				}
			}
// 			DeleteFile(pval->m_realfname);
// 			DeleteFile(pval->m_indexfname);
			pval->m_realfname.Empty();
			pval->m_indexfname.Empty();
			pval->m_filenum=0;
			pval->m_realflen=0;
		}
	}

	// 写预警数据(目前该协议的不处理预警)

	// 写回溯数据
	if (m_isindex)
	{
		// 输出文件
		sprintf(fname, "%04d%02d%02d%02d%02d%02d_unip_%u_%u.indexfile.txt",
			nowtm.wYear, nowtm.wMonth, nowtm.wDay, nowtm.wHour, nowtm.wMinute, nowtm.wSecond,
			nowtm.wMilliseconds+rand(), ++fid);
		LPIpDataStruct pval = NULL;
		pval = g_pUnipMissObj->m_pSubPro->AllocFileToDse(0, 0);
		ASSERT(pval != NULL);
		DceFileStream StreamInfo;
		memset(&StreamInfo, 0, sizeof(StreamInfo));
		memcpy(StreamInfo.Pro, "unip", 4);
		StreamInfo.Type = 0;		// 回溯数据

		BOOL bSucc = g_pUnipMissObj->m_pSubPro->MergeMemDataToStream(pval, &StreamInfo,
			str, len, fname);
		
		// 保持流文件在线
		if ((!bSucc) || (ftell(pval->m_prealfile) >= 2*1024*1024))
//		if (ftell(pval->m_prealfile) >= 2*1024*1024)
		{
			// 提交流文件
			int re = fclose(pval->m_prealfile);
			if (re != 0)
			{
				WriteLog("dce", "关闭文件<%s>失败! ErrCode = %d", pval->m_prealfile, re);
			}
			fclose(pval->m_prealfile);
			pval->m_prealfile = NULL;
			re = fclose(pval->m_pindexfile);
			if (re != 0)
			{
				WriteLog("dce", "关闭文件<%s>失败! ErrCode = %d", pval->m_indexfname, re);
			}
			pval->m_pindexfile = NULL;
			CString idxfname = pval->m_indexfname;
			CString ralfname = pval->m_realfname;
			idxfname.Replace("tmp20", "20");
			ralfname.Replace("tmp20", "20");
//			rename(pval->m_realfname, ralfname);
			re = rename(pval->m_realfname, ralfname);
			if (re != 0)
			{
				::Sleep(200);
				re = rename(pval->m_realfname, ralfname);
				if (re != 0)
				{
					pProObject->m_TmpFileArr.Add(pval->m_realfname);
				}
			}
//			rename(pval->m_indexfname, idxfname);
			re = rename(pval->m_indexfname, idxfname);
			if (re != 0)
			{
				::Sleep(200);
				re = rename(pval->m_indexfname, idxfname);
				if (re != 0)
				{
					pProObject->m_TmpFileArr.Add(pval->m_indexfname);
				}
			}
// 			DeleteFile(pval->m_realfname);
// 			DeleteFile(pval->m_indexfname);
			pval->m_realfname.Empty();
			pval->m_indexfname.Empty();
			pval->m_filenum=0;
			pval->m_realflen=0;
		}
	}
}

// BOOL UnipFileName::OpenTmpFileName(LPCTSTR fileKey, LPCTSTR tmpdir)
// {
// 	ASSERT(fileKey != NULL);
// 	ASSERT(fileKey[0] != NULL);
// 	SYSTEMTIME nowtm;
// 	GetLocalTime(&nowtm);
// 	m_filename.Format("%s%04d%02d%02d%02d%02d%02d_%s_%u_%u.indexfile.txt", tmpdir,
// 			nowtm.wYear, nowtm.wMonth, nowtm.wDay, nowtm.wHour, nowtm.wMinute, nowtm.wSecond,
// 			fileKey, nowtm.wMilliseconds+rand(), g_dataheader.tm1+rand());
// 	return m_file.Open(m_filename, CFile::modeCreate | CFile::modeReadWrite);
// }
// 
// void UnipFileName::DumpFile()
// {
// 	if (m_file.m_hFile != CFile::hFileNull)
// 	{
// 		m_file.Close();
// 		CString str;
// 		CStringArray attfiles;
// 		ASSERT(!m_filename.IsEmpty());
// 		g_pUnipMissObj->m_pSubPro->OutPutFile("unip", m_type, m_CaseID,
// 			m_filename, str, attfiles, "", "");
// 		DeleteFile(m_filename);
// 	}
// 	m_filename.Empty();
// }
// 
// int UnipFileName::WriteUnipFile(const char* pdata, int len)
// {
// 	ASSERT(m_file.m_hFile != CFile::hFileNull);
// 	ASSERT(!m_filename.IsEmpty());
// 	m_num++;
// 	char buf[20];
// 	int l = sprintf(buf, "[%d]\r\n", m_num);
// 	m_file.Write(buf, l);
// 	m_file.Write(pdata, len);
// 	return m_num;
// }
// 
// void WINAPI DestructElements(UnipFileName** pvalue, int count)
// {
// 	if (*pvalue != NULL)
// 	{
// 		(*pvalue)->DumpFile();
// 		delete *pvalue;
// 	}
// }

