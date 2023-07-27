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
#include "terminal.h"

// Search	Searchrdpdr("rdpdr");
// Search	Searchrdpsnd("rdpsnd");
// Search	Searchcliprdr("cliprdr");

BOOL IsTerminalPacket(DATEHEADER& packh, const char* pPacket)
{
	// RFB xxx.xxx\n	xxxΪ��λ����
	if (packh.m_datalen > 0x20
		&& (memcmp(pPacket, "\x03\x00", 2)==0 )
		/*&& (pPacket[5]==0xe0 )*/
		&& memcmp(pPacket+6,"\x00\x00\x00\x00",4)==0) //���Ӱ�
	{
		 
		if(memcmp(pPacket+11,"Cookie: ",8)==0)
		{
			return TRUE;
		}		
	
	}
	else if(packh.m_datalen == 8
		&& (memcmp(pPacket, "\x03\x00", 2)==0 )
		/*&& (pPacket[5]==0xe0 )*/
		&& memcmp(pPacket+4,"\x02\xf0\x80\x28",4)==0)
	{
			return TRUE;
	}
	return FALSE;
}

TerminalSession::TerminalSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 60;
	m_lastpacktm = packh.m_tm;
	m_sippackcount = 0;
	m_dippackcount = 0;
	m_direction = 0;
	m_bOutput = FALSE;
	g_TerminalSion++;
	
}

TerminalSession::~TerminalSession()
{
	g_TerminalSion--;
}

int TerminalSession::InsertPacket(DATEHEADER& packh, const char* pPacket)
{
	try
	{

		if ( packh.m_datalen>0x20 && memcmp(pPacket, "\x03\x00", 2)==0 &&
		  memcmp(pPacket+11,"Cookie: ",8)==0) //���Ӱ�
		{
			 
				m_userid=CString(pPacket+0x1c,packh.m_datalen-0x1c-2);
		 		
		}


		m_sippackcount = m_dippackcount = 0;
//		m_OpenCaseFlg = v_opencasevnc;
		m_OpenIndxFlg = v_openflagterminal;
//		m_OpenWarnFlg = v_openwarnvnc;

		// ƥ�䰸����Ԥ��
		DWORD d_state = 0;
		SearchCaseOrWarn(this, NULL, 0, d_state);

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
			str.Format("%s\1%s\1rc_Terminal\1Terminal����\1%s\1Զ�̿���\1%s\1\1", 
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
		m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%d_%u_rc_terminal.indexfile.txt",
			tmpdir, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, m_sip, m_dip, ++g_VncFileNum, rand());


		CString str;
		//Ӧ��дuser��Ϊ��Ӧ��ѯ�������user userid��д��
		str.Format("user=%s\r\nuserid=%s\r\nstat= ��¼\r\n",m_userid,m_userid);
		WriteIndexFile("rc_terminal", "rc_terminal", str, g_pFtpMissObj);
	}
	catch(...)
	{
		WriteLog(FTPPRO, "InsertTerminalPacket() Error!!");
	}
	return 1;
}

BOOL TerminalSession::CloseSession(time_t nowtm)
{
	m_closetm += 60;
	return abs(nowtm-m_lastpacktm>=180);
}
