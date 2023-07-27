/***************************************************************
Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
	文件名: 	eMule
	版本:		1.0
	创建日期:	2007-7-11 
	作者:		荣强
	主要功能:	eMule协议处理文件
***************************************************************/ 

#include "stdafx.h"
#include "ftp.h"
#include "eMule.h"

CeMuleMap	g_EMuleMap;

//*************************************************
//	函数名称:		ProcessEMulePacket
//	描述:			检查并处理一个eMule的包
//	输入参数:		packh 是数据包头结构数据
//					PacketFrom 是包类型(即原始的packh.type)
//					pPacket 是包的数据
//					spyaddr 是截获点
//	返回值:			如果是eMule的包就返回TRUE否则返回FALSE
//	其它:			
//*************************************************/
BOOL ProcessEMulePacket(DATEHEADER& packh, int PacketFrom, UCHAR* pPacket, LPCTSTR spyaddr)
{
	//tcp 0xe3 4bytes 42 16byteshashid count 6*count
	//tcp 0xc5 4bytes 93 16byteshashid ...
	//udp 0xe3 0x9b hashid count 8*count

	eMuleSession	eMuleSsion;
	CString emulefilename;
	char utf_8filename[1024];
	BOOL isCDMA = FALSE;
	BOOL bClient = FALSE;
	if ((PacketFrom == CDMAUDP) || (PacketFrom == CDMATCP))
	{
		isCDMA = TRUE;
	}

	if ((packh.m_type == TCPPacket) || (packh.m_type == ProxyTCP))
	{
		UCHAR * pemuleprot=pPacket;

		if((pPacket[0] == 0xe3) && (pPacket[5] == 0x42))
		{	
			while((pemuleprot[0]==0xe3) && (pemuleprot[5]==0x42))
			{
				long emulelen=0;
				char count=pemuleprot[22];
				emulelen=*(long *)(pemuleprot+1);
				if(emulelen<255 &&  emulelen==23+6*count)
				{
					pemuleprot=pemuleprot+emulelen+5;
					if(pemuleprot-pPacket==packh.m_datalen)
					{
						break;
					}
					else if(pemuleprot-pPacket>packh.m_datalen)
					{
						continue;
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					return FALSE;
				}
			}
			bClient = FALSE;
			eMuleSsion.InitClass(packh, PacketFrom, bClient, emulefilename);
		}
		else if((pPacket[0] == 0xc5) && (pPacket[5] == 0x93))
		{
			BOOL filename=FALSE;
			while(pemuleprot[0] == 0xc5)
			{
				long emulelen=0;
				emulelen=*(long *)(pemuleprot+1);
				if(emulelen<255 )
				{
					if(pemuleprot[22]==0x59 && !filename)
					{

						int filenamelen=(int)((BYTE)pemuleprot[23]);
						if(filenamelen>240 && USHORT(pemuleprot+filenamelen-pPacket) > packh.m_datalen)
						{
							return FALSE;
						}
						memset(utf_8filename, 0, sizeof(utf_8filename));
						memcpy(utf_8filename, pemuleprot+25, filenamelen);
						emulefilename = NetBytestoAnsi(utf_8filename, CP_ACP);
						filename = TRUE;
					}
					pemuleprot=pemuleprot+emulelen+5;
					if(pemuleprot-pPacket==packh.m_datalen)
					{
						break;
					}
					else if(pemuleprot-pPacket<packh.m_datalen)
					{
						continue;
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					return FALSE;
				}
			}
			if(!emulefilename.IsEmpty())
			{
				bClient = TRUE;
				eMuleSsion.InitClass(packh, PacketFrom, bClient, emulefilename);
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}

	}
	else if(packh.m_type==UDPPacket || packh.m_type==ProxyUDP)
	{
		UCHAR * pemuleprot=pPacket;
		
		if(pPacket[0]==0xe3 && pPacket[1]==0x9b)
		{	
			char count=pemuleprot[18];
			if(packh.m_datalen != (19+6*count))
			{
				return FALSE;
			}
			bClient = FALSE;
			eMuleSsion.InitClass(packh, PacketFrom, bClient, emulefilename);
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	if (eMuleSsion.m_bInit)
	{
		eMuleSsion.m_spyaddr = spyaddr;
		// 匹配案件、预警和屏蔽信息
		DWORD d_state = 0;
		SearchCaseOrWarn(&eMuleSsion, NULL, 0, d_state);

		// 如果是案件就删除哈希单元
		if (eMuleSsion.m_CaseArr.GetSize()>0)
		{
			g_EMuleMap.RemoveKey(eMuleSsion.m_eMuleKey);
		}

		// 该协议暂时不输出活动历史

		// 输出文件
		CString tmpdir = g_pFtpMissObj->GetTmpPath();
		SYSTEMTIME now;
		GetLocalTime(&now);
		LPCTSTR subpro;
		CString text;
		if (bClient)
		{
			subpro = _T("p2p_emule");
			eMuleSsion.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_1_%d_%u_%s.indexfile.txt",
				tmpdir, now.wYear, now.wMonth, now.wDay, now.wHour,
				now.wMinute, now.wSecond, eMuleSsion.m_sip,
				++g_EmuleFileNum, rand(), subpro);
			text.Format("downfile= %s\r\n",	eMuleSsion.m_downfname);
		}
		else
		{
			subpro = _T("p2p_eDonkey");
			eMuleSsion.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_1_%d_%u_%s.indexfile.txt",
				tmpdir, now.wYear, now.wMonth, now.wDay, now.wHour,
				now.wMinute, now.wSecond, eMuleSsion.m_dip,
				++g_EmuleFileNum, rand(), subpro);
		}
		eMuleSsion.WriteIndexFile("p2p_emule", subpro, text, g_pFtpMissObj);
	}
	return TRUE;
}

//*************************************************
//	函数名称:		CeMuleMap::SetAt
//	描述:			把新的单元插入到哈希表中
//	输入参数:		key 哈希键
//					nowtm 当前包的时间
//	输出参数:		
//	返回值:			如果Key在哈希表中存在就返回FALSE
//					否则就返回TRUE
//	其它:			改变原哈希单元的时间
//*************************************************/
// BOOL CeMuleMap::SetAt(CString key, time_t nowtm)
// {
// 	UINT nHash;
// 	CAssoc* pAssoc = GetAssocAt(key, nHash);
// 	if (pAssoc == NULL)
// 	{
// 		// 没有找到
// 		if (m_pHashTable == NULL)
// 			InitHashTable();
// 		
// 		pAssoc = NewAssoc();
// 		if (pAssoc!=NULL)
// 		{
// 			pAssoc->value.createtime = pAssoc->value.lasttime = nowtm;
// 			pAssoc->nHashValue = nHash;
// 			pAssoc->key = key;
// 			pAssoc->pNext = m_pHashTable[nHash];
// 			m_pHashTable[nHash] = pAssoc;
// 		}
// 		return FALSE;
// 	}
// 	else
// 	{
// 		// 找到了
// 		pAssoc->value.lasttime = nowtm;	// 改变时间戳
// 		return TRUE;
// 	}
// }

//*************************************************
//	函数名称:		eMuleSession::InitClass
//	描述:			初始化eMuleSession对象
//	输入参数:		packh 包的头信息结构
//					packetfrom 是包类型(即原始的packh.type)
//					isClient 是否是客户端的包
//					downfile 客户端下载的文件(isClient==TRUE时才有效)
//	输出参数:		
//	返回值:			该数据需要输出文件时返回TRUE 否则返回FALSE
//	其它:			
//*************************************************/
BOOL eMuleSession::InitClass(DATEHEADER& packh, int packetfrom,
						   BOOL isClient, CString downfile)
{
	m_begfiletm = packh.m_tm;
	m_endfiletm = 0;
	m_bInit = FALSE;
	m_OpenWarnFlg = v_openwarnemule;
	m_OpenCaseFlg = v_opencaseemule;
	m_OpenIndxFlg = v_openflagemule;
	m_rubbishflag = FALSE;
	m_Language = "";
	m_ProxyType = 0;

	// 处理数据源
	BOOL isCDMA = FALSE;
	BOOL isOther = FALSE;
	switch(packetfrom)
	{
	case ProxyTCP:
	case ProxyUDP:
	case TCPPacket: 
	case UDPPacket:
		m_dataSrc = DATASRC_OTHER;
		isOther = TRUE;
		break;
	case CDMATCP:
	case CDMAUDP:
		m_dataSrc = DATASRC_CDMA;
		isCDMA = TRUE;
		break;
	case GPRSTCP:
	case GPRSUDP:
		m_dataSrc = DATASRC_GPRS;
		break;
	default:
		return FALSE;
	}

	// 获得IP和MAC和eMuleKey
	if (!isCDMA)
	{
		FormatMac(packh.m_mac, m_smac, m_dmac, packh);
	}
	else
	{
		packh.m_type = CDMATCP;
	}
	DWORD uip;
	if (isClient)
	{
		// 这个包的目的IP是客户端IP
		m_dip.Format("%03u.%03u.%03u.%03u",
			packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3]);
		memcpy(m_cdip, packh.m_dip, 4);
//		memset(m_cdip, 0xFF, 4);
		m_sip.Format("%03u.%03u.%03u.%03u",
			packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3]);
		memcpy(m_csip, packh.m_sip, 4);

		ASSERT(!downfile.IsEmpty());
		m_downfname = downfile;
		m_eMuleKey = m_dip + "_" + downfile;
		memcpy(&uip, packh.m_dip, 4);
//		memcpy(packh.m_sip, packh.m_dip, 4);
//		memset(packh.m_dip, 0xFF, 4);
//		m_dmac.Empty();
//		m_dip.Empty();

		// 匹配IDC数据源
		if (v_IdcIpLib.QueryIdcIP(Swap4Byte(uip), m_strISP)||
			(v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), m_strISP)))
		{
			if (isOther)
			{
				m_dataSrc = DATASRC_IDC;
			}
			else
			{
				m_strISP.Empty();
			}
		}
	}
	else
	{
		// 这个包的源IP是种子服务器IP
		m_sip.Format("%03u.%03u.%03u.%03u",
			packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3]);
		memcpy(m_csip, packh.m_sip, 4);

//		memset(m_csip, 0xFF, 4);
		m_dip.Format("%03u.%03u.%03u.%03u",
			packh.m_dip[0], packh.m_dip[1], packh.m_dip[2], packh.m_dip[3]);
		memcpy(m_cdip, packh.m_dip, 4);

		m_eMuleKey = m_sip;
		memcpy(&uip, packh.m_sip, 4);
//		memcpy(packh.m_dip, packh.m_sip, 4);
//		memset(packh.m_sip, 0xFF, 4);
//		m_smac.Empty();
//		m_sip.Empty();

		// 匹配IDC数据源
		if (v_IdcIpLib.QueryIdcIP(Swap4Byte(uip), m_strISP)||
			(v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), m_strISP)))
		{
			if (v_statIdc)
			{
				CString str;
				str.Format("%s\1%s\1edonkey\1P2P服务\1%05d\1电骡\1%s\1\1", 
					m_dip, m_strISP, packh.m_sport, TYPESTATE_DECIDED);
				v_IdcServerStat[P2P_PROTOCOL].AddServer(str);
			}
			if (isOther)
			{
				m_dataSrc = DATASRC_IDC;
			}
			else
			{
				m_strISP.Empty();
			}
		}

	}

	// 从哈希表中查找
	time_t val;
	if (g_EMuleMap.Lookup(m_eMuleKey, val))
	{
		// 找到了就返回FALSE
		return FALSE;
	}
	else
	{
		g_EMuleMap.SetAt(m_eMuleKey, packh.m_tm);
		m_bInit = TRUE;
		// 获取认证信息
		m_AuthInfo.GetAuthInfo(packh, isCDMA);
		m_AuthInfo.SwapAuthInfo();
	}
	return m_bInit;
}

//*************************************************
//	函数名称:		eMuleSession::WriteIndexFile
//	描述:			写eMule协议的对照文件
//	输入参数:		pro是输出目录的协议
//					subpro是写在对照文件中的协议
//					txt是要写在对照文件中的非标准的串
//					pMiss是各自协议的DceMission对象指针
//	输出参数:		
//	返回值:			
//	其它:			
//*************************************************/
// int	eMuleSession::WriteIndexFile(LPCTSTR pro,
// 						LPCTSTR subpro,
// 						CString txt,
// 						DceMission* pMiss
// 						)
// {
// 	ASSERT(pMiss != NULL);
// 	ASSERT(!m_indexfname.IsEmpty());
// 	ASSERT(m_realfname.IsEmpty());
// 
// 	// 匹配IP所属地区
// 	CString sipaddr, dipaddr;
// 	if (!m_sip.IsEmpty())
// 	{
// 		sipaddr = pMiss->QueryIpAddr(Swap4Byte(*((DWORD*)m_csip)));
// 	}
// 	if (!m_dip.IsEmpty())
// 	{
// 		dipaddr = pMiss->QueryIpAddr(Swap4Byte(*((DWORD*)m_cdip)));
// 	}
// 
// 	// 组织对照文件的公共部分
// 	char buffer[4096];		// 公共部分文本缓冲区
// 	int len = 0;
// 	memset(buffer, 0, sizeof(buffer));
// 	CString sip = m_sip.Left(15);
// 	sip.Replace('.', '_');
// 	CString dip = m_dip.Left(15);
// 	dip.Replace('.', '_');
// 	len += WriteField(buffer+len, "[1]\r\npro",	subpro);
// 	len += WriteField(buffer+len, "spyaddr",		m_spyaddr);
// 	char spytm[40];
// 	memset(spytm, 0, sizeof(spytm));
// 
// 	// 只有一个时间
//  	memset(spytm, 0, sizeof(spytm));
// 	GetTimeString(m_begfiletm, "%Y-%m-%d %H:%M:%S", spytm, 36);
// 	len += WriteField(buffer+len, "tm",			spytm);
// 
// 	len += WriteField(buffer+len, "sip",			sip);
// 	len += WriteField(buffer+len, "dip",			dip);
// 	len += WriteField(buffer+len, "sipaddr",		sipaddr);
// 	len += WriteField(buffer+len, "dipaddr",		dipaddr);
// 	len += WriteField(buffer+len, "account",		m_AuthInfo.m_UserAccount1);
// 	len += WriteField(buffer+len, "phone",			m_AuthInfo.m_Phone1);
// 	len += WriteField(buffer+len, "2account",		m_AuthInfo.m_UserAccount2);
// 	len += WriteField(buffer+len, "2phone",			m_AuthInfo.m_Phone2);
// 	len += WriteField(buffer+len, "smac",			m_smac);
// 	len += WriteField(buffer+len, "dmac",			m_dmac);
// 	len += WriteField(buffer+len, "datasrc",		GetDataSrc(m_dataSrc));
// 	len += WriteField(buffer+len, "ISP",			m_strISP);
// 	len += WriteField(buffer+len, "downfile",		m_downfname);
// 	if(m_ProxyType == 1)
// 	{
// 		len += WriteField(buffer+len, "代理5",		"TRUE");	
// 	}
// 
// 	// 输出文件
// 	// 首先处理案件
// 	for (int c = 0; c < m_CaseArr.GetSize(); c++)
// 	{
// 		CaseInfo& caseinfo = m_CaseArr.ElementAt(c);
// 		// 生成告警
// 		CString CaseAlarm = GetCaseAlarmStr(caseinfo, subpro);
// 
// 		// 生成对照文件
// 		FILE* pf = fopen(m_indexfname, "wb");
// 		if (pf != NULL)
// 		{
// 			// 写文件的公共部分
// 			fwrite(buffer, len, 1, pf);
// 			
// 			// 写案件部分
// 			WriteIndexfileField(pf, "caseid",		caseinfo.CaseID);
// 			WriteIndexfileField(pf, "objid",		caseinfo.ObjectID);
// 			WriteIndexfileField(pf, "limitaddr",	caseinfo.LimitType % 100);
// 			WriteIndexfileField(pf, "limittype",	caseinfo.LimitType);
// 			if (caseinfo.LimitType > 100)
// 			{
// 				WriteIndexfileField(pf,	"扩展",		"TRUE");
// 			}
// 
// 			// 写特有部分
// 			fwrite((LPCTSTR)txt, txt.GetLength(), 1, pf);
// 			fwrite("\r\n", 2, 1, pf);
// 			fclose(pf);
// 			pf = NULL;
// 
// 			// 输出该组文件
// 			pMiss->m_pSubPro->OutPutFileToStream(pro, 1, caseinfo.CaseID, m_indexfname, m_realfname,
// 				m_attfiles, "");
// 		}
// 		else
// 		{
// 			// 写日志
// 		}
// 	}
// 	m_CaseArr.RemoveAll();		// 清除案件信息
// 
// 	// 其次处理预警
// 	for (int w = 0; w < m_WarnArr.GetSize(); w++)
// 	{
// 		warnkeyinfo& warninfo = m_WarnArr.ElementAt(w);
// 		// 生成告警
// 		CString WarnAlarm = GetWarnAlarmStr(warninfo, subpro);
// 
// 		// 生成对照文件
// 		FILE* pf = fopen(m_indexfname, "wb");
// 		if (pf != NULL)
// 		{
// 			// 写文件的公共部分
// 			fwrite(buffer, len, 1, pf);
// 			
// 			// 写预警部分
// 
// 			// 写特有部分
// 			fwrite((LPCTSTR)txt, txt.GetLength(), 1, pf);
// 			fwrite("\r\n", 2, 1, pf);
// 			fclose(pf);
// 			pf = NULL;
// 
// 			// 输出该组文件
// 			pMiss->m_pSubPro->OutPutFile(pro, 2, warninfo.m_fenlei, m_indexfname, m_realfname,
// 				m_attfiles, "");
// 		}
// 		else
// 		{
// 			// 写日志
// 		}
// 	}
// 	m_WarnArr.RemoveAll();		// 清除预警信息
// 
// 	// 最后输出回溯数据
// 	if (m_OpenIndxFlg && !m_rubbishflag)		// 回溯开关
// 	{
// 		// 生成对照文件
// 		FILE* pf = fopen(m_indexfname, "wb");
// 		if (pf != NULL)
// 		{
// 			// 写文件的公共部分
// 			fwrite(buffer, len, 1, pf);
// 			
// 			// 是否写案件信息
// 
// 			// 写特有部分
// 			fwrite((LPCTSTR)txt, txt.GetLength(), 1, pf);
// 			fwrite("\r\n", 2, 1, pf);
// 			fclose(pf);
// 			pf = NULL;
// 
// 			// 输出该组文件
// 			pMiss->m_pSubPro->OutPutFile(pro, 0, "", m_indexfname, m_realfname,
// 				m_attfiles, "");
// 		}
// 	}
// 
// 	// 删除临时文件
// 	DeleteFile(m_indexfname);
// 	m_indexfname.Empty();
// 	return 0;
// }
// 
