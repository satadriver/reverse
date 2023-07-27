#include "StdAfx.h"
#include "BaseStruct.h"
#include "PubClass.h"
#include "SocketSession.h"
#include "ReadDDeFile.h"
#include "DceMission.h"
// #include "..\\Include\\d4all.hPp"
#include "IpQueryLib.h"
#include "zwsqldb.h"
#include "DCE.h"
#include "ZwString.h"
#include "HttpLib.h"

extern CCrc32	g_crc32Calc;

#include "PhoneNumberLocation.h"
#include "NatAuth.h"
SocketSession::SocketSession(DATEHEADER& packh, int aT)
{
	m_VPNaccount = GetAccountFromVpn(packh);
	// 初始化IP
	memcpy(m_csip, packh.m_sip,4);
	memcpy(m_cdip, packh.m_dip,4);
	this->m_sip.Format("%03d.%03d.%03d.%03d_%05d",\
		packh.m_sip[0],packh.m_sip[1],packh.m_sip[2],packh.m_sip[3],packh.m_sport);
	this->m_dip.Format("%03d.%03d.%03d.%03d_%05d",\
		packh.m_dip[0],packh.m_dip[1],packh.m_dip[2],packh.m_dip[3],packh.m_dport);

	m_sTcpSessionKey = (this->m_sip + this->m_dip).GetBuffer(0);
	
	//GetTCPSessionKey_std(packh);

	// 处理代理标识
 	if ((packh.m_type == ProxyTCP) || (packh.m_type == ProxyUDP))
		m_ProxyType = 1;
 	else
 		m_ProxyType = 0;

	// 处理数据源
 	BOOL isCDMA = FALSE;
 	m_dataSrc = DATASRC_OTHER;
	if ((aT == CDMATCP) || (aT == CDMAUDP))
	{
		// CDMA数据类型
		m_dataSrc = DATASRC_CDMA;
		m_AuthInfo.GetAuthInfo(packh, TRUE);
		// iups
		if (g_b3g)
		{
			IUPSAAA();
		}
		NATAUTH();
	}
	else
	{
		FormatMac(packh.m_mac, m_smac, m_dmac, packh);
		m_AuthInfo.GetAuthInfo(packh, FALSE);
		// iups
		if (g_b3g)
		{
			IUPSAAA();
		}
		NATAUTH();

		if ((aT == GPRSTCP) || (aT == GPRSUDP))
		{
			// GPRS数据类型
			m_dataSrc = DATASRC_GPRS;
		}
		else
		{
			// 其他数据类型

			// 在没分清客户端和服务器端时不处理IDC
		}
	}

	// 其他变量的初始化
//	m_vLanID = packh.vlanid;
 	m_Language = _T("");
	m_spyaddr = "";
	m_rubbishflag = FALSE;
	m_lastpacktm= packh.m_tm;
	m_begfiletm = packh.m_tm;
	m_closetm = packh.m_tm + 60;
	m_endfiletm = 0;
	m_bRecord = FALSE;
	m_OpenCaseFlg = TRUE;
	m_OpenIndxFlg = TRUE;
	m_OpenWarnFlg = TRUE;
	m_rubbishflag = FALSE;
	m_dwType=0;
	
// 	if(ipidarr.GetSize()<100)
// 		ipidarr.Add(packh.ipid);

}

SocketSession::SocketSession(int aT,DATEHEADER& packh)
{
	m_VPNaccount = GetAccountFromVpn(packh);
	// 初始化IP
	memcpy(m_csip, packh.m_sip,4);
	memcpy(m_cdip, packh.m_dip,4);
	this->m_dip.Format("%03d.%03d.%03d.%03d_%05d",\
		packh.m_sip[0],packh.m_sip[1],packh.m_sip[2],packh.m_sip[3],packh.m_sport);
	this->m_sip.Format("%03d.%03d.%03d.%03d_%05d",\
		packh.m_dip[0],packh.m_dip[1],packh.m_dip[2],packh.m_dip[3],packh.m_dport);

	m_sTcpSessionKey = (this->m_sip + this->m_dip).GetBuffer(0);
	
		//GetTCPSessionKey_std(packh);

	// 处理代理标识
 	if ((packh.m_type == ProxyTCP) || (packh.m_type == ProxyUDP))
		m_ProxyType = 1;
 	else
 		m_ProxyType = 0;

	// 处理数据源
 	BOOL isCDMA = FALSE;
 	m_dataSrc = DATASRC_OTHER;
	if ((aT == CDMATCP) || (aT == CDMAUDP))
	{
		// CDMA数据类型
		m_dataSrc = DATASRC_CDMA;
		m_AuthInfo.GetAuthInfo(packh, TRUE);
	}
	else
	{
		FormatMac(packh.m_mac, m_smac, m_dmac, packh);
		m_AuthInfo.GetAuthInfo(packh, FALSE);

		if ((aT == GPRSTCP) || (aT == GPRSUDP))
		{
			// GPRS数据类型
			m_dataSrc = DATASRC_GPRS;
		}
		else
		{
			// 其他数据类型

			// 在没分清客户端和服务器端时不处理IDC
		}
	}

	// 其他变量的初始化
//	m_vLanID = packh.vlanid;
 	m_Language = _T("");
	m_spyaddr = "";
	m_rubbishflag = FALSE;
	m_lastpacktm= packh.m_tm;
	m_begfiletm = packh.m_tm;
	m_closetm = packh.m_tm + 60;
	m_endfiletm = 0;
	m_bRecord = FALSE;
	m_OpenCaseFlg = TRUE;
	m_OpenIndxFlg = TRUE;
	m_OpenWarnFlg = TRUE;
	m_rubbishflag = FALSE;
	m_dwType=0;
// 	if(ipidarr.GetSize()<100)
// 		ipidarr.Add(packh.ipid);

}
SocketSession::SocketSession()
{
	// 初始化IP
	m_ProxyType = 0;

	// 处理数据源
 	m_dataSrc = DATASRC_OTHER;
 	m_Language = _T("");
	m_spyaddr = "";
	m_rubbishflag = FALSE;
	m_lastpacktm= 0;
	m_begfiletm = 0;
	m_closetm = 0;
	m_endfiletm = 0;
	m_bRecord = FALSE;
	m_OpenCaseFlg = TRUE;
	m_OpenIndxFlg = TRUE;
	m_OpenWarnFlg = TRUE;
	m_rubbishflag = FALSE;
	m_dwType=0;
}

SocketSession::~SocketSession()
{
	if (!m_indexfname.IsEmpty())
	{
		DeleteFile(m_indexfname);
		m_indexfname.Empty();
	}

	if (!m_realfname.IsEmpty())
	{
		DeleteFile(m_realfname);
		m_realfname.Empty();
	}

	for (int i=0; i<m_attfiles.GetSize(); i++)
	{
		CString& s = m_attfiles.ElementAt(i);	
		// 以引用的方式操作数组中的字符串
		// 这样相当于直接操作数组中的字符串
		// 注意对其中任何一个变量的改变都会影响到另外一个变量
		if (s.GetLength()>2)
		{
			if (s.GetAt(s.GetLength()-2)=='*')
				s.ReleaseBuffer(s.GetLength()-2);
			DeleteFile(s);
		}
	}
	m_attfiles.RemoveAll();
}

int SocketSession::FormatAttFname(char* buf, int buflen,int &atttype)
{
	// 
	int len = 0;
	CString tmps;
	for(int i=0; i < m_attfiles.GetSize() && len+MAX_PATH < buflen; i++)
	{
		LPCTSTR pFiletype = "";
		CString& attfname = m_attfiles.ElementAt(i);
		if (attfname.GetAt(attfname.GetLength() - 2) == '*')
		{
//返回值6是rar加密改名 5 8是pgp加密文件 7是rar改名 9是pgp加密改名
			
			switch(attfname.GetAt(attfname.GetLength() - 1))
			{
			case '1':
				pFiletype = "非压缩";
				break;
			case '2':
				pFiletype = "无实体";
				break;
			case '3':
				pFiletype = "已解压";
				break;
			case '4':
				pFiletype = "未解压";
				break;
			case '5':
				pFiletype = "RAR加密";
				atttype=1;
				break;
			case '6':
				pFiletype = "RAR加密改名";
				atttype=4;
				break;
			case '7':
				pFiletype = "RAR改名";
				atttype=3;
				break;
			case '8':
				pFiletype = "PGP加密";
				atttype=2;
				break;
			case '9':
				pFiletype = "PGP加密改名";
				atttype=5;
				break;

			default:
				break;
			}
			attfname.ReleaseBuffer(attfname.GetLength()-2);
		}
		
		CString fname = GetShortName(attfname);
		int pos = attfname.Find('|');
		if (pos > 0)
		{
			attfname = attfname.Left(pos);
		}
		CString strTemp=GetExtName(attfname);
		strTemp.MakeLower();
		if ((v_bLanguage) && (m_Language[0]==0) && (strTemp=="txt")
			&& (stricmp((LPCTSTR)attfname + attfname.GetLength() + 1 - sizeof("@AdditionalTxtFile.txt"),
				"@AdditionalTxtFile.txt") != 0))
		{
			// 检查语种
			int flen=GetFileLength(attfname);
			if (flen > 0)
			{
				char* ptext;
				if (flen > (buflen - 20))
				{
					ptext = tmps.GetBufferSetLength(flen + 10);
				}
				else
				{
					ptext = buf + len + 4;
				}
				memset(ptext + flen, 0, 8);
				
				FILE * fs=0;
				fs=fopen(attfname, "rb");
				if(fs > 0)
				{
					UINT readsize=0;
					readsize = fread(ptext, 1, flen, fs);
					fclose(fs);
					m_Language = CheckSPL(ptext, readsize);
				}
			}
		}
		int tmplen = sprintf(buf + len, "attfile%d= %s\r\nfiletype%d= %s\r\n",
			i+1, fname, i+1, pFiletype);
		buflen -= tmplen;
		len += tmplen;
	}
	if(i != m_attfiles.GetSize())
	{
		WriteLog("error", "附件丢失, 还原出:%d, 写至对照:%d", m_attfiles.GetSize(), i);
	}
	return len;
}

int SocketSession::WriteIndexFile(LPCTSTR pro,
								  LPCTSTR subpro,
								  CString txt,
								  DceMission* pMiss)
{
int error=0;
	try
	{
		ASSERT(pMiss != NULL);
		ASSERT(!m_indexfname.IsEmpty());
		// 生成FileKey(整合和输出用到)
		CString filekey;
		// 如果有实体文件存在filekey就等于实体文件的扩展名
		CString indexfname = GetShortName(m_indexfname);
		m_indexfname.Empty();
		LPCTSTR tp = m_realfname;
		int proid=0;
		if ((m_realfname.GetLength()>4) && (g_DseServer.m_count > 1)
			&& (stricmp(tp+m_realfname.GetLength()-4, ".eml") == 0))
		{
			// 是邮件就获取邮件的CRC32做filekey
			filekey = g_crc32Calc.GetEmlCrc(m_realfname);
		}
error=1;
		proid=GetProBufId(subpro);

		// 匹配IP所属地区
		CString sipaddr;
		CString dipaddr;
		if (memcmp(m_csip, "\xFF\xFF\xFF\xFF", 4) != 0)
		{
			sipaddr = k_IpQueryTable.QueryIpAddr(Swap4Byte(*((DWORD*)m_csip)));
		}
		if (memcmp(m_cdip, "\xFF\xFF\xFF\xFF", 4) != 0)
		{
			dipaddr = k_IpQueryTable.QueryIpAddr(Swap4Byte(*((DWORD*)m_cdip)));
		}

		char buffer[128*1024];		// 公共部分文本缓冲区
		int len = 0;				// 公共部分文本的长度
		
		// 写文件名部分
		if(!m_realfname.IsEmpty())
		{
			len = WriteField(buffer, "filename",		GetShortName(m_realfname));
		}
		int atttype=0;
		CString strattfile;
		if (m_attfiles.GetSize() > 0)
		{
			// 写附件文件名且匹配附件的语种
			len += FormatAttFname(buffer + len, sizeof(buffer) - len - 8,atttype);
		}
		CString strfname = CString(buffer, len);
		len = 0;
error=2;

		// 组织对照文件的公共部分
		memset(buffer, 0, sizeof(buffer));
		CString sip = m_sip.Left(15);
		sip.Replace('.', '_');
		CString dip = m_dip.Left(15);
		dip.Replace('.', '_');
		len += WriteField(buffer+len, "pro",	subpro);
		len += WriteField(buffer+len, "spyaddr",		v_ddelist.spyaddr);
		char spytm[40];
		memset(spytm, 0, sizeof(spytm));
		if(m_endfiletm == 0)
		{
			// 只有一个时间
			if (m_begfiletm <= 0x386CD300)		// 2000-01-01 00:00:00
			{
				WriteLog("dce", "协议%s 的时间<0x%08X>异常了", subpro, m_begfiletm);

				m_begfiletm = time(0);
			}
 			memset(spytm, 0, sizeof(spytm));
			GetTimeString(m_begfiletm, "%Y-%m-%d %H:%M:%S", spytm, 36);
			len += WriteField(buffer+len, "tm",			spytm);
		}
		else
		{
			// 有两个时间
			if (m_endfiletm <= 0x386CD300)		// 2000-01-01 00:00:00
			{
				WriteLog("dce", "协议%s 的时间<0x%08X>异常了", subpro, m_endfiletm);
				m_endfiletm = time(0);
			}
			if (m_begfiletm <= 0x386CD300)		// 2000-01-01 00:00:00
			{
				WriteLog("dce", "协议%s 的时间<0x%08X>异常了", subpro, m_begfiletm);
				m_begfiletm = m_endfiletm-60;
			}
			memset(spytm, 0, sizeof(spytm));
			GetTimeString(m_begfiletm, "%Y-%m-%d %H:%M:%S", spytm, 36);
			len += WriteField(buffer+len, "begtm",		spytm);
			len += WriteField(buffer+len, "tm",		spytm);
			memset(spytm, 0, sizeof(spytm));
			GetTimeString(m_endfiletm, "%Y-%m-%d %H:%M:%S", spytm, 36);
			len += WriteField(buffer+len, "endtm",		spytm);
		}
		len += WriteField(buffer+len, "sip",			sip);
		if (m_sip.GetLength() == 21)
		{
			len += WriteField(buffer+len, "sport",			m_sip.Right(5));
		}
		len += WriteField(buffer+len, "dip",			dip);
		if (m_dip.GetLength() == 21)
		{
			len += WriteField(buffer+len, "dport",			m_dip.Right(5));
		}
error=3;

		len += WriteField(buffer+len, "sipaddr",		sipaddr);
		len += WriteField(buffer+len, "dipaddr",		dipaddr);

		//源IP详细地址信息
		CString sipcountry,sipprovince,sipcity,sipcounty,siptelecom;
		CString dipcountry,dipprovince,dipcity,dipcounty,diptelecom;
		
		if(sipaddr!="")
		{
			GetClassAreaInfo(sipaddr,sipcountry,sipprovince,sipcity,sipcounty,siptelecom);
			len += WriteField(buffer+len, "sipcountry",		sipcountry);
			len += WriteField(buffer+len, "sipprovince",	sipprovince);
			len += WriteField(buffer+len, "sipcity",		sipcity);
			len += WriteField(buffer+len, "sipcounty",		sipcounty);
			len += WriteField(buffer+len, "siptelecom",		siptelecom);
		}
		
		if(dipaddr!="")
		{
			GetClassAreaInfo(dipaddr,dipcountry,dipprovince,dipcity,dipcounty,diptelecom);
			len += WriteField(buffer+len, "dipcountry",		dipcountry);
			len += WriteField(buffer+len, "dipprovince",	dipprovince);
			len += WriteField(buffer+len, "dipcity",		dipcity);
			len += WriteField(buffer+len, "dipcounty",		dipcounty);
			len += WriteField(buffer+len, "diptelecom",		diptelecom);
		}
		//////////////////////////////////////////////////////////////////////////
		//F2认证信息 wangzaixue
		if (0)
		{
			CString AAAname,AAAphone;
			CString tmpsip,tmpdip;
			tmpsip = sip;
			tmpdip = dip;
			
			tmpsip.Replace("_",".");
			tmpdip.Replace("_",".");
			
			CString cmd,strRst;
			cmd.Format("funname=GetAuthAllInfo\r\nIP=%s",tmpsip);
			strRst = CommGetData("10.10.1.201",30051,"\\AS\\",cmd,cmd.GetLength());
			
			CStringArray StrArray;
			
			if(strRst.Find("失败")==-1 && strRst!="")
			{
				StrSplit(StrArray,strRst,'|');
				
				if(StrArray.GetSize()>8)
				{
					//拆分后的项内容
					//Nasip\1ClientIp\1strPhone\1strUserName\1strUserCellLac\1strUserCellCi\1strUserImei\1SessionId
					//0--------1---------2----------3-------------4---------------5--------------6-----------7-----
					
					if(tmpsip.Find(StrArray[8])!=-1 || tmpdip.Find(StrArray[8])!=-1)
					{
						AAAname = StrArray[3];
						AAAphone = StrArray[2];
					}
				}				
			}else{
				cmd.Format("funname=GetAuthAllInfo\r\nIP=%s",tmpdip);
				strRst = CommGetData("10.10.1.201",30051,"\\AS\\",cmd,cmd.GetLength());
				if(strRst.Find("失败")==-1 && strRst!="")
				{
					StrSplit(StrArray,strRst,'|');
					if(StrArray.GetSize()>8)
					{	
						//拆分后的项内容
						//Nasip\1ClientIp\1strPhone\1strUserName\1strUserCellLac\1strUserCellCi\1strUserImei\1SessionId
						//0--------1---------2----------3-------------4---------------5--------------6-----------7-----
						
						if(tmpsip.Find(StrArray[8])!=-1 || tmpsip.Find(StrArray[8])!=-1)
						{
							AAAname = StrArray[3];
							AAAphone = StrArray[2];
						}
					}
				}
			}
		}
		
		//////////////////////////////////////////////////////////////////////////
		if(v_programtype == SYSTEM_FYY||
			v_programtype == SYSTEM_DEPARTMENT||
			v_programtype == SYSTEM_HOTEL)
		{
			/// 附加FyyTz信息
			int ifyydirect = 0;
			CString skey, dkey;
			skey.Format("%s-%s", m_sip.Left(15), m_smac);
			dkey.Format("%s-%s", m_dip.Left(15), m_dmac);
			sTzUnit *pUnit = v_fyyTzUnit.CheckTzUnit(skey, dkey, ifyydirect);
			if(pUnit)
			{
				if(pUnit->m_cardno.size()>0)
				{
					len += WriteField(buffer+len, "account",	pUnit->m_cardno.c_str());
					len += WriteField(buffer+len, "accounttype",pUnit->m_cardtype.c_str());
					if(atttype!=0)
						strattfile.Format("%d\1%s\1%s\1%d",time(0),"4",pUnit->m_cardno.c_str(),atttype);
				}
				else 
				{
					len += WriteField(buffer+len, "account",pUnit->m_umac.c_str()	);
					len += WriteField(buffer+len, "accounttype","MAC地址");
					if(atttype!=0)
						strattfile.Format("%d\1%s\1%s\1%d",time(0),"3",pUnit->m_cardno.c_str(),atttype);
				}
				len += WriteField(buffer+len, "cardtype",	pUnit->m_cardtype.c_str());
				len += WriteField(buffer+len, "cardno",		pUnit->m_cardno.c_str());
				len += WriteField(buffer+len, "username",	pUnit->m_username.c_str());
				len += WriteField(buffer+len, "terminalno",	pUnit->m_roomid.c_str());
				len += WriteField(buffer+len, "country",	pUnit->m_country.c_str());
		// 		len += WriteField(buffer+len, "account",	pUnit->m_account.c_str());
			}
			else
			{
				len += WriteField(buffer+len, "account",		m_AuthInfo.m_UserAccount1);
				if(FALSE == m_AuthInfo.m_UserAccount1.IsEmpty())
					len += WriteField(buffer+len, "accounttype",	"上网帐号");
				len += WriteField(buffer+len, "phone",			m_AuthInfo.m_Phone1);
				len += WriteField(buffer+len, "2account",		m_AuthInfo.m_UserAccount2);
				if(FALSE == m_AuthInfo.m_UserAccount2.IsEmpty())
					len += WriteField(buffer+len, "2accounttype",	"上网帐号");

				len += WriteField(buffer+len, "2phone",			m_AuthInfo.m_Phone2);
				if(atttype!=0)
				{
					strattfile.Format("%d\1%s\1%s\1%d",time(0),"1",m_AuthInfo.m_UserAccount1,atttype);
				}
			}
		}
		else
		{
			/*iups*/
			if (g_b3g)
			{
				if(m_AuthInfo.m_UserAccount1 !="" || m_AuthInfo.m_Phone1!="")
				{
					
					len += WriteField(buffer+len, "authorization",m_NetwordStandard);
					
					
					len += WriteField(buffer+len, "account",		m_AuthInfo.m_UserAccount1);
					//if(FALSE == m_AuthInfo.m_UserAccount1.IsEmpty())
					len += WriteField(buffer+len, "accounttype",	"上网帐号");
					len += WriteField(buffer+len, "phone",			m_AuthInfo.m_Phone1);
					
					if (m_AuthInfo.m_Phone1 != "")
					{
						string phone_number(m_AuthInfo.m_Phone1.GetBuffer(0));
						PhoneNumberLocation *p = PhoneNumberLocation::Instance();
						string province,city,sp;
						p->GetLocationInfo(phone_number,province,city,sp);
						len += WriteField(buffer+len, "sipcountry",		"中国");
						len += WriteField(buffer+len, "sipprovince",	province.c_str());
						len += WriteField(buffer+len, "sipcity",		city.c_str());
						len += WriteField(buffer+len, "siptelecom",		sp.c_str());
					}
				}
				
				if(m_AuthInfo.m_userCelllac1 != "" || m_AuthInfo.m_userCellCi1 != "")
				{
					len += WriteField(buffer+len, "celllac",		m_AuthInfo.m_userCelllac1);
					len += WriteField(buffer+len, "cellci",			m_AuthInfo.m_userCellCi1);
				}
				
				if (!m_AuthInfo.m_userCelllac1.IsEmpty() && !m_AuthInfo.m_userCellCi1.IsEmpty())
				{
					BasicInfoStruct tmpbasicinfo;
					CString      tmpkey;
					
					tmpkey.Format("%s%s",m_AuthInfo.m_userCelllac1,m_AuthInfo.m_userCellCi1);
					if(m_basicinfomap.Lookup(tmpkey,tmpbasicinfo))
					{
						len += sprintf(buffer+len, "sitename= %s\r\n",	tmpbasicinfo.name);
						len += sprintf(buffer+len, "siteaddress= %s\r\n",tmpbasicinfo.address);
						len += sprintf(buffer+len, "longitude= %s\r\n",	tmpbasicinfo.longitude);
						len += sprintf(buffer+len, "latitude= %s\r\n",	tmpbasicinfo.latitude);
					}
				}
			}
			
			// iups
 			/*-非IUPS ->正常  非注释即正常*/
			/**/
			if (!g_b3g)
			{
				len += WriteField(buffer+len, "account",		m_AuthInfo.m_UserAccount1);
				if(FALSE == m_AuthInfo.m_UserAccount1.IsEmpty())
					len += WriteField(buffer+len, "accounttype",	"上网帐号");
				len += WriteField(buffer+len, "phone",			m_AuthInfo.m_Phone1);
				
				if(FALSE == m_AuthInfo.m_vlanid.IsEmpty())
					len += WriteField(buffer+len, "vlanid",	m_AuthInfo.m_vlanid);
				
				if (!m_AuthInfo.m_userImei1.IsEmpty())
				{
					len += WriteField(buffer+len, "imei",			m_AuthInfo.m_userImei1);
					len += WriteField(buffer+len, "celllac",		m_AuthInfo.m_userCelllac1);
					len += WriteField(buffer+len, "cellci",			m_AuthInfo.m_userCellCi1);
				}
				
				if (!m_AuthInfo.m_userCellCi1.IsEmpty() && !m_AuthInfo.m_userCelllac1.IsEmpty())
				{
					BasicInfoStruct tmpbasicinfo;
					CString      tmpkey;
					
					tmpkey.Format("%s%s",m_AuthInfo.m_userCelllac1,m_AuthInfo.m_userCellCi1);
					if(m_basicinfomap.Lookup(tmpkey,tmpbasicinfo))
					{
						len += sprintf(buffer+len, "sitename= %s\r\n",	tmpbasicinfo.name);
						len += sprintf(buffer+len, "siteaddress= %s\r\n",	tmpbasicinfo.address);
						len += sprintf(buffer+len, "longitude= %s\r\n",	tmpbasicinfo.longitude);
						len += sprintf(buffer+len, "latitude= %s\r\n",	tmpbasicinfo.latitude);
					}
				}
				
				
				
				len += WriteField(buffer+len, "2account",		m_AuthInfo.m_UserAccount2);
				if(FALSE == m_AuthInfo.m_UserAccount2.IsEmpty())
					len += WriteField(buffer+len, "2accounttype",	"上网帐号");
				
				len += WriteField(buffer+len, "2phone",			m_AuthInfo.m_Phone2);
				
				if (!m_AuthInfo.m_userImei2.IsEmpty())
				{
					len += WriteField(buffer+len, "2imei",			m_AuthInfo.m_userImei2);
					len += WriteField(buffer+len, "2celllac",		m_AuthInfo.m_userCelllac2);
					len += WriteField(buffer+len, "2cellci",		m_AuthInfo.m_userCellCi2);
				}
				
				if (!m_AuthInfo.m_userCellCi2.IsEmpty() && !m_AuthInfo.m_userCelllac2.IsEmpty())
				{
					BasicInfoStruct tmpbasicinfo;
					CString      tmpkey;
					
					tmpkey.Format("%s%s",m_AuthInfo.m_userCelllac2,m_AuthInfo.m_userCellCi2);
					if(m_basicinfomap.Lookup(tmpkey,tmpbasicinfo))
					{
						len += sprintf(buffer+len, "2sitename= %s\r\n",	tmpbasicinfo.name);
						len += sprintf(buffer+len, "2siteaddress= %s\r\n",	tmpbasicinfo.address);
						len += sprintf(buffer+len, "2longitude= %s\r\n",	tmpbasicinfo.longitude);
						len += sprintf(buffer+len, "2latitude= %s\r\n",	tmpbasicinfo.latitude);
					}
				}
			}
//正常END

			if(atttype!=0)
			{
				strattfile.Format("%d\1%s\1%s\1%d",time(0),"1",m_AuthInfo.m_UserAccount1,atttype);
			}

		}
error=4;

		//写附件加密或改名的bcp
	// 	if (atttype!=0&&
	// 		(!strattfile.IsEmpty()))
	// 	{
	// 		printf("attfile =%s",strattfile);
	// 		WriteArrToData(SMTP_PROTOCOL, ATTFILE_DATA, strattfile);
	// 	}

		len += WriteField(buffer+len, "smac",			m_smac);
		len += WriteField(buffer+len, "dmac",			m_dmac);
		len += WriteField(buffer+len, "datasrc",		GetDataSrc(m_dataSrc));
		len += WriteField(buffer+len, "ISP",			m_strISP);
		if(m_ProxyType == 1)
		{
			len += WriteField(buffer+len, "代理5",		"TRUE");	
		}
		
		len += WriteField(buffer+len, "Language",		m_Language);	// 写语种
		m_Language = _T("");

error=5;

		// 输出文件
		// 首先处理案件
		int id = 1;
		CMapStringToString Tmp_CaseCutRepeate;
		CMapStringToString Tmp_CaseIdRid;
		CString WriteToIndexCase;
		for (int c = 0; c < m_CaseArr.GetSize(); c++)
		{
			CaseInfo& caseinfo = m_CaseArr.ElementAt(c);

			CString tmp_look,tmp_id;
			tmp_look.Format("%s%s%d%s",caseinfo.CaseID,caseinfo.ObjectID,caseinfo.LimitType%100,caseinfo.LimitAddr);
			if (Tmp_CaseCutRepeate.Lookup(tmp_look,tmp_id))//一个案件的一条线索只能中一次。去除重复数据
			{
				continue;
			}
			else
			{
				Tmp_CaseCutRepeate.SetAt(tmp_look,"");
			}

			if (!Tmp_CaseIdRid.Lookup(caseinfo.CaseID,tmp_id))
			{
				Tmp_CaseIdRid.SetAt(caseinfo.CaseID,"");
				WriteToIndexCase.IsEmpty() ? (WriteToIndexCase =  caseinfo.CaseID) : (WriteToIndexCase += "," + caseinfo.CaseID) ;
			}
			//查看此案件的不截获协议
			if (!caseinfo.nocappro.IsEmpty())
			{
				if (caseinfo.nocappro.Find(subpro)>=0)
				{
					continue;
				}
			}
			// 		// 生成告警
	//  	CString CaseAlarm;
	// 		CaseAlarm = GetCaseAlarmStr(caseinfo, subpro);
			
			// 组织案件部分
			int tlen = len;
			tlen += WriteField(buffer+tlen, "caseid",		caseinfo.CaseID);
			tlen += WriteField(buffer+tlen, "objid",		caseinfo.ObjectID);
			tlen += WriteField(buffer+tlen, "limittype",	caseinfo.LimitType % 100);
			tlen += WriteField(buffer+tlen, "limitaddr",	caseinfo.LimitAddr);
			tlen += WriteField(buffer+tlen, "limitid",	caseinfo.LimitId);
			if (caseinfo.LimitType > 100 && caseinfo.LimitType < 200)
			{
				tlen += WriteField(buffer+tlen,	"扩展",		"TRUE");
			}
			int indexflen = tlen + txt.GetLength() + strfname.GetLength() + 2;
			char* tp = pMiss->m_pSubPro->m_IndexFileData.GetBufferSetLength(indexflen);
			memcpy(tp, buffer, tlen);
			tp += tlen;
			memcpy(tp, txt, txt.GetLength());
			tp += txt.GetLength();
			CString tmpstr;
			if(id == 0)
			{
				memcpy(tp, strfname, strfname.GetLength());
				memcpy(tp+strfname.GetLength(), "\r\n", 2);
			}
			else
			{
				tmpstr.Format("%s",strfname);
				CString replasestr;
				replasestr.Format("_%d_",id);
				tmpstr.Replace("_1_",replasestr);
				memcpy(tp, tmpstr, tmpstr.GetLength());
				memcpy(tp+tmpstr.GetLength(), "\r\n", 2);

			}
			pMiss->m_pSubPro->m_IndexFileData.ReleaseBuffer(indexflen);

			// 输出该组文件
	// 		if(caseinfo.AlarmState == 2)     //界面告警
	// 		{
	// 			alarmtype = "case_webalarm";
	// 		}
	// 		else if(caseinfo.AlarmState == 4) //手机告警
	// 		{
	// 			alarmtype = "case_handalarm";
	// 		}
	// 		else if(caseinfo.AlarmState == 6) //既界面告警也手机告警
	// 		{
	// 			alarmtype = "case_allalarm";
	// 		}

			pMiss->m_pSubPro->OutPutFileToStream(pro, caseinfo.AlarmState + 1, caseinfo.CaseID, indexfname, m_realfname,
				m_attfiles, filekey, id);
			id++;
		}
error=6;

	//	m_CaseArr.RemoveAll();		// 清除案件信息(从外部清除)
		CString WriteToIndexIndex;

		// 其次处理预警
		id = 1;
		CMapStringToString Tmp_WarnCutRepeate;
		if (/*m_CaseArr.GetSize() > 0 && */v_programtype == SYSTEM_SLEUTH)
		{

		}
		else
		{
			if (m_WarnArr.GetSize()>0)
			{
				for (int w = 0; w < m_WarnArr.GetSize(); w++)
				{
					warnkeyinfo& warninfo = m_WarnArr.ElementAt(w);
					

					CString tmp_look,tmp_id;
					tmp_look.Format("%s",warninfo.m_id);
					//一条数据对同一个线索应该只写一个文件，在字典对关键字时或者多次对时会对一个线索重复对上多次。
					//此处写文件时要把重复的数据去重。
					if (Tmp_WarnCutRepeate.Lookup(tmp_look,tmp_id))
					{
						continue;
					}
					else
					{
						Tmp_WarnCutRepeate.SetAt(tmp_look,"");
					}

					// 生成告警
					CString WarnAlarm;
					//WarnAlarm = GetWarnAlarmStr(warninfo, subpro);	
					// 组织案件部分
					int tlen = len;
					if (warninfo.m_isccic)
					{
						tlen += WriteField(buffer+tlen, "ccicid",		warninfo.m_id);

						WarnAlarm.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s",spytm,warninfo.m_ccicbuf,
						v_ddelist.spyaddr,m_sip.Left(15),subpro,"",m_userid);
						//ccic告警数据
						WriteArrToData(proid, CCIC_ALARM, WarnAlarm);
					}
					else
					{
						CString idclass;
						idclass.Format("%s\1%s\1%s",warninfo.m_id,warninfo.m_firstclass,warninfo.m_secclass);
						//写重点人员在线表
						WriteArrToData(proid, KEYOBJ_ONLINE, idclass);

						tlen += WriteField(buffer+tlen, "keyobjid",		warninfo.m_id);
						tlen += WriteField(buffer+tlen, "firstclass",		warninfo.m_firstclass);
						tlen += WriteField(buffer+tlen, "secondclass",	warninfo.m_secclass);
						if (warninfo.m_keyquanzhi>0)
						{
							WarnAlarm.Format("%s\1%s\1%s\1%d",warninfo.m_id,warninfo.m_secclass,"7",warninfo.m_keyquanzhi);
							//重点人员平分数据
							WriteArrToData(proid, KEYOBJ_ALARM, WarnAlarm);
						}
						if (!strattfile.IsEmpty())
						{
							CString tmpattfile;
							tmpattfile.Format("%s\1%s",strattfile,warninfo.m_id);
							//重点人员附件加密或改名数据
							WriteArrToData(proid, ATTFILE_DATA, tmpattfile);
						}
						WriteToIndexIndex.Format("%skeyobjid= %s\r\n",(CString)WriteToIndexIndex,tmp_look);
					}

					int indexflen = tlen + txt.GetLength() + strfname.GetLength() + 2;
					char* tp = pMiss->m_pSubPro->m_IndexFileData.GetBufferSetLength(indexflen);
					memcpy(tp, buffer, tlen);
					tp += tlen;
					memcpy(tp, txt, txt.GetLength());
					tp += txt.GetLength();
					memcpy(tp, strfname, strfname.GetLength());
					memcpy(tp+strfname.GetLength(), "\r\n", 2);
					pMiss->m_pSubPro->m_IndexFileData.ReleaseBuffer(indexflen);

					// 输出该组文件
					pMiss->m_pSubPro->OutPutFileToStream(pro, 2, warninfo.m_firstclass, indexfname, m_realfname,
						m_attfiles, filekey, id);
					id++;
				}
				//非重点人员的行为类数据也入一份
				if (atttype!=0&&
					(!strattfile.IsEmpty()))
				{
					strattfile.Format("%s\1",(CString)strattfile);
					printf("attfile =%s",strattfile);
					WriteArrToData(proid, ATTFILE_DATA, strattfile);
				}

			}
			else
			{
					//写附件加密或改名的bcp
				if (atttype!=0&&
					(!strattfile.IsEmpty()))
				{
					strattfile.Format("%s\1",(CString)strattfile);
					printf("attfile =%s",strattfile);
					WriteArrToData(proid, ATTFILE_DATA, strattfile);
				}
			}

			m_WarnArr.RemoveAll();		// 清除预警信息(从外部清除)


		}

error=7;
			// 最后输出回溯数据
			// 2009-8-4 zzh
			// 以srcIP+host去重(外层控制), 写httpurl
	// 		if (strcmp(pro, "httpurl") != 0)	// httpurl 不写回溯 
			{
				if (m_OpenIndxFlg && 
					!m_rubbishflag && 
					v_workmode==3)		// 回溯开关
				{
					// 生成对照文件
					int indexflen;
					if (!WriteToIndexCase.IsEmpty())
					{
						indexflen = len + txt.GetLength() + strfname.GetLength() + 2 + WriteToIndexCase.GetLength() + 22 + 2;
					}
					else
					{
						indexflen = len + txt.GetLength() + strfname.GetLength() + 2;
					}
					char* tp = pMiss->m_pSubPro->m_IndexFileData.GetBufferSetLength(indexflen);
					memcpy(tp, buffer, len);
					tp += len;
					memcpy(tp, txt, txt.GetLength());
					tp += txt.GetLength();
					if (!WriteToIndexCase.IsEmpty())
					{
						memcpy(tp,"IsCase= TRUE\r\ncaseid= ",22);
						tp+=22;
						memcpy(tp,WriteToIndexCase,WriteToIndexCase.GetLength());
						tp+=WriteToIndexCase.GetLength();
						memcpy(tp, "\r\n", 2);
						tp+=2;
					}
					if (!WriteToIndexIndex.IsEmpty())
					{
						memcpy(tp,WriteToIndexIndex,WriteToIndexIndex.GetLength());
						tp+=WriteToIndexIndex.GetLength();
					}

					memcpy(tp, strfname, strfname.GetLength());
					memcpy(tp+strfname.GetLength(), "\r\n", 2);
					pMiss->m_pSubPro->m_IndexFileData.ReleaseBuffer(indexflen);
					pMiss->m_pSubPro->OutPutFileToStream(pro, 0, "", indexfname, m_realfname,
						m_attfiles, filekey);
					pMiss->m_pSubPro->m_IndexFileData.Empty();
				}
			}
	}
	catch (...)
	{
		WriteLog("dce", "WriteHttpUrlFile Error!! error=%d",error);
	
	}

	// 删除临时文件
//	DeleteFile(m_indexfname);
	m_indexfname.Empty();

	if ((!m_realfname.IsEmpty()) && (_access(m_realfname, 0) == 0))
	{
		DeleteFile(m_realfname);
	}
	m_realfname.Empty();

	for (int i=0; i<m_attfiles.GetSize(); i++)
	{
		CString& attfilename = m_attfiles.ElementAt(i);
		if ((!attfilename.IsEmpty()) && (_access(attfilename, 0) == 0))
		{
			DeleteFile(attfilename);
		}
	}
	m_attfiles.RemoveAll();
	return 0;
}

// 检查实体文件的语种
void SocketSession::CheckLanguage(char* buffer/* = NULL*/, int buflen/* = 0*/)
{
	m_Language = _T("");
	if (v_bLanguage)
	{
		// 检查语种
		CString tmps;
		int flen=GetFileLength(m_realfname);
		if (flen > 0)
		{
			char* ptext;
			if ((flen + 10) > buflen)
			{
				ptext = tmps.GetBufferSetLength(flen+10);
				memset(ptext + flen, 0, 8);
			}
			else
			{
				ptext = buffer;
				memset(ptext + flen, 0, 8);
			}
			
			FILE * fs=0;
			fs = fopen(m_realfname, "rb");
			if(fs > 0)
			{
				UINT readsize = 0;
				readsize = fread(ptext, 1, flen, fs);
				fclose(fs);
				m_Language = CheckSPL(ptext, readsize);
			}
		}
	}
}


/******************************************
pro 协议
subpro 子协议
hisrec 从外边定义的存放活动历史的临时类，并已初始化完私有的数据
num/*第几个数组，为确保向数组中写时单线程，每个插件分配一个num
type 数据的类型 1为网络活动历史
写完活动历史之后立即写虚拟身份
*******************************************/
int SocketSession::WriteHisData(LPCTSTR pro,
								  LPCTSTR subpro,
								  HistoryRecord *hisRec,
								  DceMission* pMiss,
								  int num,
								  int type)
{
	int error=0;
	try
	{
		char spytm[40];
		GetTimeString(m_begfiletm, "%Y%m%d%H%M%S", spytm, 40);

			// 匹配IP所属地区
		CString sipaddr = k_IpQueryTable.QueryIpAddr(Swap4Byte(*((DWORD*)m_csip)));
		CString dipaddr = k_IpQueryTable.QueryIpAddr(Swap4Byte(*((DWORD*)m_cdip)));
error=1;
		hisRec->SetRecord(IDX_BEGTM,	spytm);
		hisRec->SetRecord(IDX_ENDTM,	spytm);
		hisRec->SetRecord(IDX_IP1,		m_sip.Left(15));
		hisRec->SetRecord(IDX_IP2,		m_dip.Left(15));
		hisRec->SetRecord(IDX_MAC1,		m_smac);
		hisRec->SetRecord(IDX_MAC2,		m_dmac);
		hisRec->SetRecord(IDX_PORT1,	m_sip.Right(5));
		hisRec->SetRecord(IDX_PORT2,	m_dip.Right(5));
		hisRec->SetRecord(IDX_ADDR1,	sipaddr);
error=2;
		hisRec->SetRecord(IDX_ADDR2,	dipaddr);

//		if(v_programtype != SYSTEM_FYY)
		{
			hisRec->SetRecord(IDX_ACCOUNT1,	m_AuthInfo.m_UserAccount1);
			if(FALSE == m_AuthInfo.m_UserAccount1.IsEmpty())
				hisRec->SetRecord(IDX_AccountType1,"上网帐号");
			hisRec->SetRecord(IDX_PHONE1,	m_AuthInfo.m_Phone1);
			hisRec->SetRecord(IDX_ACCOUNT2,	m_AuthInfo.m_UserAccount2);
			if(FALSE == m_AuthInfo.m_UserAccount2.IsEmpty())
				hisRec->SetRecord(IDX_AccountType2,"上网帐号");
			hisRec->SetRecord(IDX_PHONE2,	m_AuthInfo.m_Phone2);
		}

		hisRec->SetRecord(IDX_PRO,		pro);
		hisRec->SetRecord(IDX_SUBPRO,	subpro);
		hisRec->SetRecord(IDX_SPYADDR,	v_ddelist.spyaddr);
	 
		if(m_ProxyType==1)
			hisRec->SetRecord(IDX_IFAGENT, "2");
error=3;
		hisRec->SetRecord(IDX_DATASRC,	GetDataSrc(m_dataSrc));
		CString tempstr;
		if(hisRec->GetWebidRecord(tempstr))
		{
error=4;
			WriteArrToData(num, type, tempstr);

		}
				
// 		CString tempstr;
// 		if(hisRec->GetHisRecord(tempstr))
// 		{
// error=4;
// 			WriteArrToData(num, type, tempstr);
// 
// 		}
error=5;
		//如果写的是活动历史，再继续写虚拟身份
// 		if (type == HISTORY_DATA)
// 		{
// 			if (!m_userid.IsEmpty())
// 			{
// 				if(hisRec->GetWebfigRecord(tempstr))
// 				{
// 	error=6;
// 					WriteArrToData(num, WEBFIG_DATA, tempstr);
// 				}			
// 			}
// 			else if((!m_AuthInfo.m_UserAccount1.IsEmpty())||
// 				(!m_AuthInfo.m_UserAccount2.IsEmpty()))
// 			{
// 				if (subpro=="dynapassdown"||
// 					subpro=="freegatedown"||
// 					subpro=="dynapassup"||
// 					subpro=="freegateup"||
// 					subpro=="freegateurl"||
// 					subpro=="ultrabrowse")
// 				{
// 					if(hisRec->GetWebfigRecord(tempstr))
// 					{
// 		error=16;
// 						WriteArrToData(num, WEBFIG_DATA, tempstr);
// 					}
// 				}
// 			}
// error=7;
// 
// 		}
	//	WriteArrToData(MEDIA_PROTOCOL, HISTORY_DATA, tempstr);
	}
	catch (...)
	{
		WriteLog("dce", "WriteHisData() Error!!error=%d",error);
	}
	return 0;
}

// 生成网络活动历史的串
// pMiss是各自协议的DceMission对象指针
// pro 协议 如：media
// subpro 子协议 如：media_asf
// hisrec 从外边定义的存放活动历史的临时类，并已初始化完私有的数据
// optm 是时间如果optm==0 则使用m_begfiletm
CString SocketSession::GetWebidStr(LPCTSTR pro,
					LPCTSTR subpro,
					HistoryRecord *hisRec,
					DceMission* pMiss,
					time_t optm)
{
	char spytm[40];
	if (optm == 0)
	{
		GetTimeString(m_begfiletm, "%Y%m%d%H%M%S", spytm, 40);
	}
	else
	{
		GetTimeString(optm, "%Y%m%d%H%M%S", spytm, 40);
	}

	// 匹配IP所属地区
	CString sipaddr = k_IpQueryTable.QueryIpAddr(Swap4Byte(*((DWORD*)m_csip)));
	CString dipaddr = k_IpQueryTable.QueryIpAddr(Swap4Byte(*((DWORD*)m_cdip)));

	hisRec->SetRecord(IDX_BEGTM,	spytm);
	hisRec->SetRecord(IDX_ENDTM,	spytm);
	hisRec->SetRecord(IDX_IP1,		m_sip.Left(15));
	hisRec->SetRecord(IDX_IP2,		m_dip.Left(15));
	hisRec->SetRecord(IDX_MAC1,		m_smac);
	hisRec->SetRecord(IDX_MAC2,		m_dmac);
	hisRec->SetRecord(IDX_PORT1,	m_sip.Right(5));
	hisRec->SetRecord(IDX_PORT2,	m_dip.Right(5));
	hisRec->SetRecord(IDX_ADDR1,	sipaddr);
	hisRec->SetRecord(IDX_ADDR2,	dipaddr);
//	if(v_programtype != SYSTEM_FYY)
	{
		hisRec->SetRecord(IDX_ACCOUNT1,	m_AuthInfo.m_UserAccount1);
		if(FALSE == m_AuthInfo.m_UserAccount1.IsEmpty())
			hisRec->SetRecord(IDX_AccountType1,"上网帐号");
		hisRec->SetRecord(IDX_PHONE1,	m_AuthInfo.m_Phone1);
		hisRec->SetRecord(IDX_ACCOUNT2,	m_AuthInfo.m_UserAccount2);
		if(FALSE == m_AuthInfo.m_UserAccount2.IsEmpty())
			hisRec->SetRecord(IDX_AccountType2,"上网帐号");
		hisRec->SetRecord(IDX_PHONE2,	m_AuthInfo.m_Phone2);
	}

	hisRec->SetRecord(IDX_PRO,		pro);
	hisRec->SetRecord(IDX_SUBPRO,	subpro);
	hisRec->SetRecord(IDX_SPYADDR,	v_ddelist.spyaddr);

	hisRec->SetRecord(IDX_DATASRC,	GetDataSrc(m_dataSrc));
			
	CString tempstr;
	if(!hisRec->GetWebidRecord(tempstr))
	{
		tempstr.Empty();
	}
	return tempstr;
}


// 生成虚拟身份的串
// pMiss是各自协议的DceMission对象指针
// pro 协议 如：media
// subpro 子协议 如：media_asf
// hisrec 从外边定义的存放虚拟身份的临时类，并已初始化完私有的数据
// optm 是时间如果optm==0 则使用m_begfiletm
CString SocketSession::GetWebfigStr(LPCTSTR pro,
					LPCTSTR subpro,
					HistoryRecord *hisRec,
					DceMission* pMiss,
					time_t optm)
{
// 	char spytm[40];
// 	if (optm == 0)
// 	{
// 		GetTimeString(m_begfiletm, "%Y-%m-%d %H:%M:%S", spytm, 40);
// 	}
// 	else
// 	{
// 		GetTimeString(optm, "%Y-%m-%d %H:%M:%S", spytm, 40);
// 	}
// 
// 	// 匹配IP所属地区
// 	CString sipaddr = k_IpQueryTable.QueryIpAddr(Swap4Byte(*((DWORD*)m_csip)));
// 	CString dipaddr = k_IpQueryTable.QueryIpAddr(Swap4Byte(*((DWORD*)m_cdip)));

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

// 	hisRec->SetRecord(IDX_BEGTM,	spytm);
// 	hisRec->SetRecord(IDX_ENDTM,	spytm);
// 	hisRec->SetRecord(IDX_CRCID,	m_sip);
// 	hisRec->SetRecord(IDX_IP2,		m_dip);
// 	hisRec->SetRecord(IDX_PORT1,	m_sip.Right(5));
// 	hisRec->SetRecord(IDX_PORT2,	m_dip.Right(5));
// 	hisRec->SetRecord(IDX_ADDR1,	sipaddr);
// 	hisRec->SetRecord(IDX_ADDR2,	dipaddr);
// 	hisRec->SetRecord(IDX_ACCOUNT1,	m_AuthInfo.m_UserAccount1);
// 	hisRec->SetRecord(IDX_PHONE1,	m_AuthInfo.m_Phone1);
// 	hisRec->SetRecord(IDX_ACCOUNT2,	m_AuthInfo.m_UserAccount2);
// 	hisRec->SetRecord(IDX_PHONE2,	m_AuthInfo.m_Phone2);
// 	hisRec->SetRecord(IDX_PRO,		pro);
// 	hisRec->SetRecord(IDX_SUBPRO,	subpro);

//	hisRec->SetRecord(IDX_DATASRC,	GetDataSrc(m_dataSrc));
	if (m_userid.IsEmpty())/*||
	(!m_AuthInfo.m_UserAccount2.IsEmpty()))*/
	{
		return "";
	}		
	CString tempstr;
	if(!hisRec->GetWebfigRecord(tempstr))
	{
		tempstr.Empty();
	}
	return tempstr;
}
// #define		HTTP_PROTOCOL		0
// #define		POP3_PROTOCOL		1
// #define		MEDIA_PROTOCOL		2
// #define		IMSG_PROTOCOL		3
// #define		SMTP_PROTOCOL		4
// #define		FTP_PROTOCOL		5
// #define		TELNET_PROTOCOL		5
// #define		P2P_PROTOCOL		5
// #define		UNIP_PROTOCOL		6
// #define		AAA_PROTOCOL		7

int SocketSession::GetProBufId(LPCTSTR pro)
{
	if (strstr(pro,"http")!=NULL)
	{
		return HTTP_PROTOCOL;
	}
	if (strstr(pro,"imsg")!=NULL)
	{
		return IMSG_PROTOCOL;
	}
	if (strstr(pro,"media")!=NULL)
	{
		return MEDIA_PROTOCOL;
	}
	if (strstr(pro,"pop3")!=NULL)
	{
		return POP3_PROTOCOL;
	}
	if (strstr(pro,"smtp")!=NULL)
	{
		return SMTP_PROTOCOL;
	}
	if (strstr(pro,"ftp")!=NULL)
	{
		return FTP_PROTOCOL;
	}
	if (strstr(pro,"telnet")!=NULL)
	{
		return TELNET_PROTOCOL;
	}
	if (strstr(pro,"unip")!=NULL)
	{
		return UNIP_PROTOCOL;
	}
	return OTHER_PROTOCOL;
}

BOOL SocketSession::GetClassAreaInfo(CString ipaddr,CString& country,CString& province,CString& city,CString& county,CString& telecom)
{
	CString tmpstr;
	int pos,pos2;
	
	CString chinasheng="北京天津上海重庆河北山西陕西山东河南辽宁吉林黑龙江江苏浙江安徽江西福建湖北湖南四川贵州云南广东海南甘肃青海台湾内蒙新疆西藏广西宁夏香港澳门";
	
	tmpstr = ipaddr.Left(4);
	if(chinasheng.Find(tmpstr)!=-1)
	{
		//中国
		country = "中国";
		
		//查找省
		province = tmpstr;
		
		pos = ipaddr.Find("省");
		if(pos!=-1)
		{
			//查找市
			pos2 = ipaddr.Find("市");
			if(pos2!=-1)
			{
				city = ipaddr.Mid(pos+2,pos2-pos-2);
				
				//查找县
				pos = ipaddr.Find("县");
				if(pos!=-1)
				{
					county = ipaddr.Mid(pos2+2,pos-pos2-2);
				}
			}
		}else{
			//查找市
			pos2 = ipaddr.Find("市");
			if(pos2!=-1)
			{
				//查找县
				pos = ipaddr.Find("县");
				if(pos!=-1)
				{
					county = ipaddr.Mid(pos2+2,pos-pos2-2);
				}
			}
		}
		
	}else{
		//其它国家
		pos = ipaddr.Find(" ");
		country = ipaddr.Left(pos);
	}
	
	//查找运营商
	pos = ipaddr.Find(" ");
	tmpstr = ipaddr.Right(ipaddr.GetLength()-pos);
	if(tmpstr.Find("电信")!=-1)
	{
		telecom = "电信";
	}
	else if(tmpstr.Find("联通")!=-1)
	{
		telecom = "联通";
	}
	else if(tmpstr.Find("移动")!=-1)
	{
		telecom = "移动";
	}
	else if(tmpstr.Find("铁通")!=-1)
	{
		telecom = "铁通";
	}
	else if(tmpstr.Find("有线通")!=-1)
	{
		telecom = "有线通";
	}
	else if(tmpstr.Find("长城宽带")!=-1)
	{
		telecom = "长城宽带";
	}
	else
	{
		telecom = "";
	}
	
	return TRUE;
}


CString SocketSession::GetAccountFromVpn(DATEHEADER& packh)
{
	// vpn 标识
//	CString res;
	if (packh.m_mac[0] == 0xEF && packh.m_mac[6] == 0xEF && packh.m_mac[7] == 0xEF)
	{
		unsigned int vpnid = 0;
		vpnid += ((unsigned int)packh.m_mac[11]) << 24;
		vpnid += ((unsigned int)packh.m_mac[10]) << 16;
		vpnid += ((unsigned int)packh.m_mac[9]) << 8;
		vpnid += ((unsigned int)packh.m_mac[8]);
//		res.Format("VPN_%d", vpnid);
		memset(packh.m_sipUserName, 0x00, MaxUserName+4);
		sprintf(packh.m_sipUserName, "VPN_%u", vpnid);
	}
	return packh.m_sipUserName;
}

void SocketSession::IUPSAAA()
{
	CString tmpimei;
	tmpimei.Format("%s%s",m_dmac,m_smac);
	tmpimei.Replace(":","");
	
	CString tmpimei2;
	tmpimei2 = tmpimei.Left(15);
	
	//去除添加项
	CString imei;
	imei.Format("%s%s%s",tmpimei2.Left(1),tmpimei2.Mid(2,11),tmpimei2.Right(1));
	
	ULONGLONG imsiint=0;
	int p=16;
	ULONGLONG base=1;
	
	for(int i=0; i<imei.GetLength(); i++)
	{
		CString tmpnum;
		tmpnum = imei.Mid(imei.GetLength()-i-1,1);
		
		int tmpa;
		sscanf(tmpnum,"%x",&tmpa);
		imsiint+=tmpa*base;
		
		base*=p;
	}
	
	CString imsi;
	ULONGLONG x=10;
	for (;;)
	{
		CString tmponlynum;
		tmponlynum.Format("%d",imsiint % x);
		imsi=tmponlynum+imsi;
		
		imsiint=imsiint/x;
		
		if(imsiint==0)
			break;
	}
	//LAC，CI信息
	int lac = strtol(tmpimei.Mid(16,4),0,16); 
	int cid = strtol(tmpimei.Mid(20,4),0,16);
	
	if(imsi.Left(3)=="460")
	{
		CString tmp = tmpimei.Mid(15,1);
		if (tmp == "0")
		{
			m_NetwordStandard = "3G网络";
		}
		if (tmp == "1")
		{
			m_NetwordStandard = "4G网络";
		}

		m_AuthInfo.m_UserAccount1 = imsi;
			
		v_imsiphone.Lookup(m_AuthInfo.m_UserAccount1,m_AuthInfo.m_Phone1);
		if(lac!=4369)
		{
			
			m_AuthInfo.m_userCelllac1.Format("%d",lac);
			m_AuthInfo.m_userCellCi1.Format("%d",cid);
		}
		else
		{
			m_AuthInfo.m_userCelllac1 = "";
			m_AuthInfo.m_userCellCi1 = "";
		}	
	}
	if (m_AuthInfo.m_UserAccount1.GetLength() < 2)
	{
		m_AuthInfo.m_UserAccount1 = m_VPNaccount;
	}
	
}

void SocketSession::NATAUTH()
{
	
	CString tmp = m_smac;
	tmp.Replace(":","");
	CString tmp1 = tmp.Mid(0,4);
	if (tmp1 == "0000")
	{
	//	{"00016c93"}
		CString tmp2 = tmp.Mid(4);

		CString temp0 = tmp.Mid(4,2);
		CString temp1 = tmp.Mid(6,2);
		CString temp2 = tmp.Mid(8,2);
		CString temp3 = tmp.Mid(10,2);
		unsigned char temp[5] = {0};
		temp[0] = strtol(temp0,0,16);
		temp[1] = strtol(temp1,0,16);
		temp[2] = strtol(temp2,0,16);
		temp[3] = strtol(temp3,0,16);

		DWORD dw = *(DWORD *)temp;

		CString tmp3;
		ULONGLONG x=10;
		for (;;)
		{
			CString tmponlynum;
			tmponlynum.Format("%d",dw % x);
			tmp3=tmponlynum+tmp3;
			
			dw=dw/x;
			
			if(dw==0)
				break;
		}
		NatAuth *na = NatAuth::GetInstance();
		CString auth = na->GetAuth(tmp3);
		if (auth != "")
		{
			m_AuthInfo.m_UserAccount1 = auth;
		}	
	}
	if (m_AuthInfo.m_UserAccount1.GetLength() < 2)
	{
		m_AuthInfo.m_UserAccount1 = m_VPNaccount;
	}	
}

CString SocketSession::GetTCPSessionKey_mfc(DATEHEADER& packh)
{
	char key[64] = {0};
	sprintf(key, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d-%d",
		packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3], packh.m_sport,
		packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3], packh.m_dport,packh.m_ack);
	return CString(key);
}

string SocketSession::GetTCPSessionKey_std(DATEHEADER& packh)
{
	char key[64] = {0};
	sprintf(key, "%03d.%03d.%03d.%03d_%05d_%03d.%03d.%03d.%03d_%05d-%d",
		packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3], packh.m_sport,
		packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3], packh.m_dport,packh.m_ack);
	return string(key);
}

string SocketSession::GetTCPSessionKey_std()
{
	return m_sTcpSessionKey;
}