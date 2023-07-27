//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	Vnc
//	�汾:		1.0
//	��������:	2007-7-28
//	����:		��ǿ
//	��Ҫ����:	VNCЭ�黹ԭ
//	ģ������:
//	��Ҫ����:	
//**************************************************************** 
#include "stdafx.h"
#include "ftp.h"
#include "radmin.h"

BOOL IsRadminPacket(DATEHEADER& packh, const char* pPacket)
{
	// RFB xxx.xxx\n	xxxΪ��λ����
	if (packh.m_datalen==10 &&
		memcmp(pPacket,"\x01\x00\x00\x00\x01\x00\x00\x00\x08\x08",10) == 0)
	{
		return TRUE;
	}
	return FALSE;
}

RadminSession::RadminSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 60;
	m_lastpacktm = packh.m_tm;
	m_sippackcount = 0;
	m_dippackcount = 0;
	m_direction = 0;
	m_bOutput = FALSE;
	g_RadminSion++;
}

RadminSession::~RadminSession()
{
	g_RadminSion--;
}

int RadminSession::InsertPacket(DATEHEADER& packh, const char* pPacket)
{
	try
	{
		if (!(packh.m_datalen==10 &&
		memcmp(pPacket,"\x01\x00\x00\x00\x01\x00\x00\x00\x08\x08",10) == 0))
		{
			return FALSE;
		}

		m_sippackcount = m_dippackcount = 0;
//		m_OpenCaseFlg = v_opencasevnc;
		m_OpenIndxFlg = v_openflagremote;
//		m_OpenWarnFlg = v_openwarnvnc;

		// ƥ�䰸����Ԥ��
		DWORD d_state = 0;
		SearchCaseOrWarn(this, NULL, 0, d_state,"rc_remote");

		// ����IDC
		CString isp;
		if(v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), isp))
		{
			if (m_dataSrc == DATASRC_OTHER)
			{
				m_dataSrc = DATASRC_IDC;
				m_strISP = isp;
			}
			// ���IDCServer
			// SIP\1SUBPRO\11PRO\1PORT\1ClassType\1DOMAIN 
			CString str;
			str.Format("%s\1%s\1rc_remote\1Radmin����\1%s\1Զ�̿���\1%s\1\1", 
				m_dip.Left(15), m_strISP, m_dip.Right(5), TYPESTATE_DECIDED);
			v_IdcServerStat[FTP_PROTOCOL].AddServer(str);
		}
		else if ((m_dataSrc == DATASRC_OTHER)
			&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), isp)))
		{
			m_dataSrc = DATASRC_IDC;
			m_strISP = isp;
		}

		// д���ʷ(��Э�鲻������ʷ)

		// д�����ļ�
		CString tmpdir = g_pFtpMissObj->GetTmpPath();
		SYSTEMTIME now;
		GetLocalTime(&now);
		m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%d_%u_rc_remote.indexfile.txt",
			tmpdir, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, m_sip, m_dip, ++g_VncFileNum, rand());
		WriteIndexFile("rc_remote", "rc_remote", "stat= ��¼\r\n", g_pFtpMissObj);
	}
	catch(...)
	{
		WriteLog(FTPPRO, "InsertRadminPacket() Error!!");
	}
	return 1;
}

BOOL RadminSession::CloseSession(time_t nowtm)
{
	m_closetm += 60;
	return abs(nowtm-m_lastpacktm>=180);
}

BOOL DealL2TPVpn(DATEHEADER& packh, const char* pPacket, int PacketFrom)
{
#define L2TP_BIT_TYPE	0x8000
#define L2TP_BIT_LEN	0x4000
#define L2TP_BIT_SEQ	0x0800
#define L2TP_BIT_OFF	0x0200
	int l2tphdrlen = 6;		// PacketType(1)+TunnelId(2)+SessionId(2)
	WORD wPacketType = pPacket[8]*256+pPacket[9];
	if( wPacketType & L2TP_BIT_LEN)
		l2tphdrlen += 2;
	if( wPacketType & L2TP_BIT_SEQ)
		l2tphdrlen += 4;
	if( wPacketType & L2TP_BIT_OFF)
		l2tphdrlen += 2;

	unsigned short dwTmp;
	BOOL bCheck = TRUE;

	dwTmp = *(unsigned short*)(pPacket+l2tphdrlen+8);
	if(dwTmp != 0x23c2)
		bCheck = FALSE;
	if(bCheck)
	{
		if(pPacket[l2tphdrlen+2+8] != 2)
			bCheck = FALSE;
	}
	if(FALSE == bCheck)
		return FALSE;
//              06 a5 06 a5 00 52 12 c7<40 02 00 4a ec 65  .......R..@..J.e
// 0030   6a c2>c2 23 02 78 00 40 31 bb aa ba 1d c7 34 fd  j..#.x.@1.....4.
// 0040   e3 fc 17 2b f9 12 e6 c7 9f 00 00 00 00 00 00 00  ...+............
// 0050   00 af 0e e7 8a 3e c0 90 c2 1e 1f 70 0c 83 69 72  .....>.....p..ir
// 0060   4a 74 fd 0d 38 e9 c5 d1 84 00 64 6b 73 74 61 72  Jt..8.....dkstar
// 0070   39 35 30 31                                      9501			

	CString txt;
	u_char *tsp = (u_char*)pPacket;
	tsp += 8+2+l2tphdrlen;
	int offusername = 5+tsp[4];
	int lenusername = tsp[2]*256+tsp[3]-offusername;
	if(offusername == 0x36 && lenusername < 16 )
	{
		txt.Format("user=%s", CString((char*)(tsp+offusername), lenusername));
	}
#ifdef _DEBUG
	printf("l2tp vpn user=(%s)\r\n", txt);
#endif

	RadminSession vpnSession(packh, PacketFrom);
	vpnSession.m_sippackcount = 0;
	vpnSession.m_dippackcount = 0;
	vpnSession.m_OpenCaseFlg = v_opencasevnc;
	vpnSession.m_OpenIndxFlg = v_openflagvnc;
	vpnSession.m_OpenWarnFlg = v_openwarnvnc;

	// ƥ�䰸����Ԥ��
	DWORD d_state = 0;
	if(txt.IsEmpty())
		SearchCaseOrWarn(&vpnSession, NULL, 0, d_state);
	else
		SearchCaseOrWarn(&vpnSession, txt, 0, d_state);

	// ����IDC
	CString isp;
	if(v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)vpnSession.m_cdip), isp))
	{
		if (vpnSession.m_dataSrc == DATASRC_OTHER)
		{
			vpnSession.m_dataSrc = DATASRC_IDC;
			vpnSession.m_strISP = isp;
		}
		// ���IDCServer
		// SIP\1SUBPRO\11PRO\1PORT\1ClassType\1DOMAIN 
		CString str;
		str.Format("%s\1%s\1vpn\1VPN����\1%s\1Զ�̿���\1%s\1\1", 
			vpnSession.m_dip.Left(15), vpnSession.m_strISP, vpnSession.m_dip.Right(5), TYPESTATE_DECIDED);
		v_IdcServerStat[FTP_PROTOCOL].AddServer(str);
	}
	else if ((vpnSession.m_dataSrc == DATASRC_OTHER)
		&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)vpnSession.m_csip), isp)))
	{
		vpnSession.m_dataSrc = DATASRC_IDC;
		vpnSession.m_strISP = isp;
	}
	// д���ʷ(��Э�鲻������ʷ)

	// д�����ļ�
	CString tmpdir = g_pFtpMissObj->GetTmpPath();
	SYSTEMTIME now;
	GetLocalTime(&now);
	vpnSession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%d_%u_vpn.indexfile.txt",
		tmpdir, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, vpnSession.m_sip, vpnSession.m_dip, ++g_VncFileNum, rand());
	if(txt.IsEmpty())
		vpnSession.WriteIndexFile("vpn", "vpn", "type= pptp\r\nstat= VPN����\r\n", g_pFtpMissObj);
	else
	{
		txt.Format("type= pptp\r\nstat= VPN����\r\n%s\r\n", CString(txt));
		vpnSession.WriteIndexFile("vpn", "vpn", txt, g_pFtpMissObj);
	}

	return FALSE;
}
BOOL DealPPTPVpn(DATEHEADER& packh, const char* pp, int PacketFrom)
{
	unsigned short dwTmp;
	BOOL bCheck = TRUE;

	const unsigned char *pPacket = (const unsigned char*)pp;
	dwTmp = *(unsigned short*)(pPacket+2);
	if(dwTmp != 0x0b88)
		bCheck = FALSE;
#define PP2P_BIT_ACK	0x80
#define PP2P_BIT_SEQ	0x1000
#define PP2P_BIT_KEY	0x2000
#define PP2P_BIT_OFFSET	0x4000
#define PP2P_BIT_CHKSUM	0x8000
	int pp2phdr = 4;		// PacketType(1)+TunnelId(2)+SessionId(2)
	WORD wPacketType = pPacket[0]*256+pPacket[1];
	if( wPacketType & PP2P_BIT_SEQ)
		pp2phdr += 4;
	if( wPacketType & PP2P_BIT_ACK)
		pp2phdr += 4;
	if( wPacketType & PP2P_BIT_KEY)
		pp2phdr += 4;
	if( (wPacketType & PP2P_BIT_CHKSUM) || (wPacketType & PP2P_BIT_OFFSET))
		pp2phdr += 4;
	
	if(bCheck)
	{
		dwTmp = *(unsigned short*)(pPacket+pp2phdr);
		if(dwTmp != 0x23c2)
			bCheck = FALSE;
	}
	if(bCheck)
	{
		if(pPacket[pp2phdr+2] != 2)
			bCheck = FALSE;
	}
	int offname = 0;
	int lenname = 0;
	if(bCheck)
	{
		WORD psize = pPacket[pp2phdr+4]*256+pPacket[pp2phdr+5];
		if(packh.m_datalen != psize+pp2phdr+2)
			bCheck = FALSE;
		else
		{
			offname = pp2phdr+7+pPacket[pp2phdr+6];
			lenname = packh.m_datalen-offname;
		}
	}
	if(FALSE == bCheck)
		return FALSE;

	CString txt;
	if(lenname < 32 && lenname > 2)
		txt.Format("user=%s", CString((const char*)pPacket+offname, lenname));
#ifdef _DEBUG
	printf("pp2t vpn user=(%s)\r\n", txt);
#endif

	RadminSession vpnSession(packh, PacketFrom);
	vpnSession.m_sippackcount = 0;
	vpnSession.m_dippackcount = 0;
	vpnSession.m_OpenCaseFlg = v_opencasevnc;
	vpnSession.m_OpenIndxFlg = v_openflagvnc;
	vpnSession.m_OpenWarnFlg = v_openwarnvnc;

	// ƥ�䰸����Ԥ��
	DWORD d_state = 0;
	if(txt.IsEmpty())
		SearchCaseOrWarn(&vpnSession, NULL, 0, d_state);
	else
		SearchCaseOrWarn(&vpnSession, txt, 0, d_state);

	// ����IDC
	CString isp;
	if(v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)vpnSession.m_cdip), isp))
	{
		if (vpnSession.m_dataSrc == DATASRC_OTHER)
		{
			vpnSession.m_dataSrc = DATASRC_IDC;
			vpnSession.m_strISP = isp;
		}
		// ���IDCServer
		// SIP\1SUBPRO\11PRO\1PORT\1ClassType\1DOMAIN 
		CString str;
		str.Format("%s\1%s\1vpn\1VPN����\1%s\1Զ�̿���\1%s\1\1", 
			vpnSession.m_dip.Left(15), vpnSession.m_strISP, vpnSession.m_dip.Right(5), TYPESTATE_DECIDED);
		v_IdcServerStat[FTP_PROTOCOL].AddServer(str);
	}
	else if ((vpnSession.m_dataSrc == DATASRC_OTHER)
		&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)vpnSession.m_csip), isp)))
	{
		vpnSession.m_dataSrc = DATASRC_IDC;
		vpnSession.m_strISP = isp;
	}
	// д���ʷ(��Э�鲻������ʷ)

	// д�����ļ�
	CString tmpdir = g_pFtpMissObj->GetTmpPath();
	SYSTEMTIME now;
	GetLocalTime(&now);
	vpnSession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%d_%u_vpn.indexfile.txt",
		tmpdir, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, vpnSession.m_sip, vpnSession.m_dip, ++g_VncFileNum, rand());
	if(txt.IsEmpty())
		vpnSession.WriteIndexFile("vpn", "vpn", "type= pptp\r\nstat= VPN����\r\n", g_pFtpMissObj);
	else
	{
		txt.Format("type= pptp\r\nstat= VPN����\r\n%s\r\n", CString(txt));
		vpnSession.WriteIndexFile("vpn", "vpn", txt, g_pFtpMissObj);
	}
	return TRUE;
}


BOOL Dealopenvpn(const unsigned char* pPacket, DATEHEADER& packh)
{
	
	RadminSession vpnSession(packh, 0);
	vpnSession.m_sippackcount = 0;
	vpnSession.m_dippackcount = 0;
	vpnSession.m_OpenCaseFlg = v_opencasevnc;
	vpnSession.m_OpenIndxFlg = v_openflagvnc;
	vpnSession.m_OpenWarnFlg = v_openwarnvnc;

	// ƥ�䰸����Ԥ��
	DWORD d_state = 0;
	SearchCaseOrWarn(&vpnSession, NULL, 0, d_state);

	// ����IDC
	CString isp;
	if(v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)vpnSession.m_cdip), isp))
	{
		if (vpnSession.m_dataSrc == DATASRC_OTHER)
		{
			vpnSession.m_dataSrc = DATASRC_IDC;
			vpnSession.m_strISP = isp;
		}
		// ���IDCServer
		// SIP\1SUBPRO\11PRO\1PORT\1ClassType\1DOMAIN 
		CString str;
		str.Format("%s\1%s\1vpn\1VPN����\1%s\1Զ�̿���\1%s\1\1", 
			vpnSession.m_dip.Left(15), vpnSession.m_strISP, vpnSession.m_dip.Right(5), TYPESTATE_DECIDED);
		v_IdcServerStat[FTP_PROTOCOL].AddServer(str);
	}
	else if ((vpnSession.m_dataSrc == DATASRC_OTHER)
		&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)vpnSession.m_csip), isp)))
	{
		vpnSession.m_dataSrc = DATASRC_IDC;
		vpnSession.m_strISP = isp;
	}
	// д���ʷ(��Э�鲻������ʷ)
	
	// д�����ļ�
	CString tmpdir = g_pFtpMissObj->GetTmpPath();
	SYSTEMTIME now;
	GetLocalTime(&now);
	vpnSession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%d_%u_vpn.indexfile.txt",
		tmpdir, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, vpnSession.m_sip, vpnSession.m_dip, ++g_VncFileNum, rand());
	vpnSession.WriteIndexFile("vpn", "vpn", "type= openvpn\r\nstat= VPN����\r\n", g_pFtpMissObj);
	return TRUE;
}


BOOL Dealopenssh(const unsigned char* pPacket, DATEHEADER& packh)
{
	
	RadminSession vpnSession(packh, 0);
	vpnSession.m_sippackcount = 0;
	vpnSession.m_dippackcount = 0;
	vpnSession.m_OpenCaseFlg = v_opencasevnc;
	vpnSession.m_OpenIndxFlg = v_openflagvnc;
	vpnSession.m_OpenWarnFlg = v_openwarnvnc;
	
	// ƥ�䰸����Ԥ��
	DWORD d_state = 0;
	SearchCaseOrWarn(&vpnSession, NULL, 0, d_state);
	
	// ����IDC
	CString isp;
	if(v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)vpnSession.m_cdip), isp))
	{
		if (vpnSession.m_dataSrc == DATASRC_OTHER)
		{
			vpnSession.m_dataSrc = DATASRC_IDC;
			vpnSession.m_strISP = isp;
		}
		// ���IDCServer
		// SIP\1SUBPRO\11PRO\1PORT\1ClassType\1DOMAIN 
		CString str;
		str.Format("%s\1%s\1ssh\1ssh����\1%s\1Զ�̿���\1%s\1\1", 
			vpnSession.m_dip.Left(15), vpnSession.m_strISP, vpnSession.m_dip.Right(5), TYPESTATE_DECIDED);
		v_IdcServerStat[FTP_PROTOCOL].AddServer(str);
	}
	else if ((vpnSession.m_dataSrc == DATASRC_OTHER)
		&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)vpnSession.m_csip), isp)))
	{
		vpnSession.m_dataSrc = DATASRC_IDC;
		vpnSession.m_strISP = isp;
	}
	// д���ʷ(��Э�鲻������ʷ)
	
	// д�����ļ�
	// 2013-11-26 wangzaixue ��Ϊ����Э��
	CString tmpdir = g_pFtpMissObj->GetTmpPath();
	SYSTEMTIME now;
	GetLocalTime(&now);
	vpnSession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%d_%u.ssh.indexfile.txt",
		tmpdir, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, vpnSession.m_sip, vpnSession.m_dip, ++g_VncFileNum, rand());
	vpnSession.WriteIndexFile("rc_ssh", "rc_ssh", "type= openssh\r\nstat= ssh����\r\n", g_pFtpMissObj);
	
	return TRUE;
}




/*!\fn �����ϣ��ֵ��
*/
typedef struct _MACUnit_
{
	time_t tmout;		///< �ϴ������ʱ��
}MACUnit;

///< ��ssl sip_SessionIdΪKey,���ظ���Ϊֵ
static std::map<CString, MACUnit> g_mapMac;


BOOL Deal360Mac(const unsigned char* pPacket, DATEHEADER& packh)
{
	if(memcmp(pPacket+8 ,"360P",4)!=0)
		return FALSE;

	CString macstr=CString((char*)pPacket+0x14,8);

	char fgsession[100];
	memset(fgsession, 0, 100);
	int ioff = sprintf(fgsession, "%03u.%03u.%03u.%03u_%s",
		packh.m_sip[0], packh.m_sip[1], packh.m_sip[2], packh.m_sip[3],macstr );

	std::map<CString, MACUnit>::iterator It;
	It = g_mapMac.find(fgsession);
	if(It == g_mapMac.end()) //�״�
	{
		MACUnit oneFg;
		oneFg.tmout = time(0);
		g_mapMac[fgsession] = oneFg;

		RadminSession vpnSession(packh, 0);
		vpnSession.m_sippackcount = 0;
		vpnSession.m_dippackcount = 0;
		vpnSession.m_OpenCaseFlg = v_opencasevnc;
		vpnSession.m_OpenIndxFlg = v_openflagvnc;
		vpnSession.m_OpenWarnFlg = v_openwarnvnc;
		
		// ƥ�䰸����Ԥ��
		DWORD d_state = 0;
		SearchCaseOrWarn(&vpnSession, NULL, 0, d_state);
		
		// 	// ����IDC
		// 	CString isp;
		// 	if(v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)vpnSession.m_cdip), isp))
		// 	{
		// 		if (vpnSession.m_dataSrc == DATASRC_OTHER)
		// 		{
		// 			vpnSession.m_dataSrc = DATASRC_IDC;
		// 			vpnSession.m_strISP = isp;
		// 		}
		// 		// ���IDCServer
		// 		// SIP\1SUBPRO\11PRO\1PORT\1ClassType\1DOMAIN 
		// 		CString str;
		// 		str.Format("%s\1%s\1vpn\1VPN����\1%s\1Զ�̿���\1%s\1\1", 
		// 			vpnSession.m_dip.Left(15), vpnSession.m_strISP, vpnSession.m_dip.Right(5), TYPESTATE_DECIDED);
		// 		v_IdcServerStat[FTP_PROTOCOL].AddServer(str);
		// 	}
		// 	else if ((vpnSession.m_dataSrc == DATASRC_OTHER)
		// 		&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)vpnSession.m_csip), isp)))
		// 	{
		// 		vpnSession.m_dataSrc = DATASRC_IDC;
		// 		vpnSession.m_strISP = isp;
		// 	}
		// д���ʷ(��Э�鲻������ʷ)
		
		// д�����ļ�
		CString tmpdir = g_pFtpMissObj->GetTmpPath();
		SYSTEMTIME now;
		GetLocalTime(&now);
		vpnSession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%d_%u_mac.indexfile.txt",
			tmpdir, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, vpnSession.m_sip, vpnSession.m_dip, ++g_VncFileNum, rand());
		
		CString txt;
		txt.Format("360mac=%s",macstr);
		vpnSession.WriteIndexFile("360mac", "360mac", txt ,g_pFtpMissObj);

	}
	else
	{
		It->second.tmout = time(0);
	}
	
	/// �������1Сʱ�Բ����������ĻỰ
	CStringArray arrErase;
	time_t tmnow = time(0);
	for (It = g_mapMac.begin(); It != g_mapMac.end(); It++)
	{
		if(abs(tmnow-It->second.tmout) > 30)
		{
			// 30���������������, ��
			arrErase.Add(It->first);
		}
	}
	for (int iie = 0; iie < arrErase.GetSize() ; iie++)
	{
		g_mapMac.erase(arrErase[iie]);
	}


	
	return TRUE;
}