/***************************************************************
Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
	�ļ���: 	eMule
	�汾:		1.0
	��������:	2007-7-11 
	����:		��ǿ
	��Ҫ����:	eMuleЭ�鴦���ļ�
***************************************************************/ 

#include "stdafx.h"
#include "ftp.h"
#include "eMule.h"

CeMuleMap	g_EMuleMap;

//*************************************************
//	��������:		ProcessEMulePacket
//	����:			��鲢����һ��eMule�İ�
//	�������:		packh �����ݰ�ͷ�ṹ����
//					PacketFrom �ǰ�����(��ԭʼ��packh.type)
//					pPacket �ǰ�������
//					spyaddr �ǽػ��
//	����ֵ:			�����eMule�İ��ͷ���TRUE���򷵻�FALSE
//	����:			
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
		// ƥ�䰸����Ԥ����������Ϣ
		DWORD d_state = 0;
		SearchCaseOrWarn(&eMuleSsion, NULL, 0, d_state);

		// ����ǰ�����ɾ����ϣ��Ԫ
		if (eMuleSsion.m_CaseArr.GetSize()>0)
		{
			g_EMuleMap.RemoveKey(eMuleSsion.m_eMuleKey);
		}

		// ��Э����ʱ��������ʷ

		// ����ļ�
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
//	��������:		CeMuleMap::SetAt
//	����:			���µĵ�Ԫ���뵽��ϣ����
//	�������:		key ��ϣ��
//					nowtm ��ǰ����ʱ��
//	�������:		
//	����ֵ:			���Key�ڹ�ϣ���д��ھͷ���FALSE
//					����ͷ���TRUE
//	����:			�ı�ԭ��ϣ��Ԫ��ʱ��
//*************************************************/
// BOOL CeMuleMap::SetAt(CString key, time_t nowtm)
// {
// 	UINT nHash;
// 	CAssoc* pAssoc = GetAssocAt(key, nHash);
// 	if (pAssoc == NULL)
// 	{
// 		// û���ҵ�
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
// 		// �ҵ���
// 		pAssoc->value.lasttime = nowtm;	// �ı�ʱ���
// 		return TRUE;
// 	}
// }

//*************************************************
//	��������:		eMuleSession::InitClass
//	����:			��ʼ��eMuleSession����
//	�������:		packh ����ͷ��Ϣ�ṹ
//					packetfrom �ǰ�����(��ԭʼ��packh.type)
//					isClient �Ƿ��ǿͻ��˵İ�
//					downfile �ͻ������ص��ļ�(isClient==TRUEʱ����Ч)
//	�������:		
//	����ֵ:			��������Ҫ����ļ�ʱ����TRUE ���򷵻�FALSE
//	����:			
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

	// ��������Դ
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

	// ���IP��MAC��eMuleKey
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
		// �������Ŀ��IP�ǿͻ���IP
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

		// ƥ��IDC����Դ
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
		// �������ԴIP�����ӷ�����IP
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

		// ƥ��IDC����Դ
		if (v_IdcIpLib.QueryIdcIP(Swap4Byte(uip), m_strISP)||
			(v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), m_strISP)))
		{
			if (v_statIdc)
			{
				CString str;
				str.Format("%s\1%s\1edonkey\1P2P����\1%05d\1����\1%s\1\1", 
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

	// �ӹ�ϣ���в���
	time_t val;
	if (g_EMuleMap.Lookup(m_eMuleKey, val))
	{
		// �ҵ��˾ͷ���FALSE
		return FALSE;
	}
	else
	{
		g_EMuleMap.SetAt(m_eMuleKey, packh.m_tm);
		m_bInit = TRUE;
		// ��ȡ��֤��Ϣ
		m_AuthInfo.GetAuthInfo(packh, isCDMA);
		m_AuthInfo.SwapAuthInfo();
	}
	return m_bInit;
}

//*************************************************
//	��������:		eMuleSession::WriteIndexFile
//	����:			дeMuleЭ��Ķ����ļ�
//	�������:		pro�����Ŀ¼��Э��
//					subpro��д�ڶ����ļ��е�Э��
//					txt��Ҫд�ڶ����ļ��еķǱ�׼�Ĵ�
//					pMiss�Ǹ���Э���DceMission����ָ��
//	�������:		
//	����ֵ:			
//	����:			
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
// 	// ƥ��IP��������
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
// 	// ��֯�����ļ��Ĺ�������
// 	char buffer[4096];		// ���������ı�������
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
// 	// ֻ��һ��ʱ��
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
// 		len += WriteField(buffer+len, "����5",		"TRUE");	
// 	}
// 
// 	// ����ļ�
// 	// ���ȴ�����
// 	for (int c = 0; c < m_CaseArr.GetSize(); c++)
// 	{
// 		CaseInfo& caseinfo = m_CaseArr.ElementAt(c);
// 		// ���ɸ澯
// 		CString CaseAlarm = GetCaseAlarmStr(caseinfo, subpro);
// 
// 		// ���ɶ����ļ�
// 		FILE* pf = fopen(m_indexfname, "wb");
// 		if (pf != NULL)
// 		{
// 			// д�ļ��Ĺ�������
// 			fwrite(buffer, len, 1, pf);
// 			
// 			// д��������
// 			WriteIndexfileField(pf, "caseid",		caseinfo.CaseID);
// 			WriteIndexfileField(pf, "objid",		caseinfo.ObjectID);
// 			WriteIndexfileField(pf, "limitaddr",	caseinfo.LimitType % 100);
// 			WriteIndexfileField(pf, "limittype",	caseinfo.LimitType);
// 			if (caseinfo.LimitType > 100)
// 			{
// 				WriteIndexfileField(pf,	"��չ",		"TRUE");
// 			}
// 
// 			// д���в���
// 			fwrite((LPCTSTR)txt, txt.GetLength(), 1, pf);
// 			fwrite("\r\n", 2, 1, pf);
// 			fclose(pf);
// 			pf = NULL;
// 
// 			// ��������ļ�
// 			pMiss->m_pSubPro->OutPutFileToStream(pro, 1, caseinfo.CaseID, m_indexfname, m_realfname,
// 				m_attfiles, "");
// 		}
// 		else
// 		{
// 			// д��־
// 		}
// 	}
// 	m_CaseArr.RemoveAll();		// ���������Ϣ
// 
// 	// ��δ���Ԥ��
// 	for (int w = 0; w < m_WarnArr.GetSize(); w++)
// 	{
// 		warnkeyinfo& warninfo = m_WarnArr.ElementAt(w);
// 		// ���ɸ澯
// 		CString WarnAlarm = GetWarnAlarmStr(warninfo, subpro);
// 
// 		// ���ɶ����ļ�
// 		FILE* pf = fopen(m_indexfname, "wb");
// 		if (pf != NULL)
// 		{
// 			// д�ļ��Ĺ�������
// 			fwrite(buffer, len, 1, pf);
// 			
// 			// дԤ������
// 
// 			// д���в���
// 			fwrite((LPCTSTR)txt, txt.GetLength(), 1, pf);
// 			fwrite("\r\n", 2, 1, pf);
// 			fclose(pf);
// 			pf = NULL;
// 
// 			// ��������ļ�
// 			pMiss->m_pSubPro->OutPutFile(pro, 2, warninfo.m_fenlei, m_indexfname, m_realfname,
// 				m_attfiles, "");
// 		}
// 		else
// 		{
// 			// д��־
// 		}
// 	}
// 	m_WarnArr.RemoveAll();		// ���Ԥ����Ϣ
// 
// 	// ��������������
// 	if (m_OpenIndxFlg && !m_rubbishflag)		// ���ݿ���
// 	{
// 		// ���ɶ����ļ�
// 		FILE* pf = fopen(m_indexfname, "wb");
// 		if (pf != NULL)
// 		{
// 			// д�ļ��Ĺ�������
// 			fwrite(buffer, len, 1, pf);
// 			
// 			// �Ƿ�д������Ϣ
// 
// 			// д���в���
// 			fwrite((LPCTSTR)txt, txt.GetLength(), 1, pf);
// 			fwrite("\r\n", 2, 1, pf);
// 			fclose(pf);
// 			pf = NULL;
// 
// 			// ��������ļ�
// 			pMiss->m_pSubPro->OutPutFile(pro, 0, "", m_indexfname, m_realfname,
// 				m_attfiles, "");
// 		}
// 	}
// 
// 	// ɾ����ʱ�ļ�
// 	DeleteFile(m_indexfname);
// 	m_indexfname.Empty();
// 	return 0;
// }
// 
