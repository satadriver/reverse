//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	Bf
//	�汾:		1.0
//	��������:	2007-7-11
//	����:		��ǿ
//	��Ҫ����:	����ħ������Э��
//	ģ������:	
//	֧�ֵİ汾:	3.0.5(9294)��ʽ�汾 Nov 26 2008
//	��Ҫ����:	IsWoWPacket
//				WowSession::InsertPacket
//**************************************************************** 

#include "stdafx.h"
#include "imsg.h"
#include "wow.h"


WowSession::WowSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 120;
	m_state = 0;
	g_GameSion++;
}

WowSession::~WowSession()
{
	g_GameSion--;
}

//*************************************************
//	��������:		WowSession::InsertPacket
//	����:			����һ���Ự��һ����
//	�������:		packh �ǰ�ͷ�ṹ
//					pPacket �ǰ�����
//	�������:		
//	����ֵ:			
//	����:			
//**************************************************
int WowSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	try
	{
		if ((packh.m_type == ProxyTCP) || (packh.m_type == ProxyUDP))
		{
			m_ProxyType = 1;
		}
		if (m_state & 4)
		{
			return 0;	// �Ѿ��������
		}
		int packettype = IsWoWPacket(packh, pPacket);
		if ((packettype == 1) && (0 == (m_state & 1)))
		{
			// ��¼�İ� ��ȡ�û�ID Client --> Server
			if (GetLoginUserid(pPacket, packh.m_datalen))
			{
				if (memcmp(packh.m_sip, m_cdip, 4) == 0)
				{
					// ����IP����
					CString tstr = m_sip;
					m_sip = m_dip;
					m_dip = tstr;
					tstr = m_dmac;
					m_dmac = m_smac;
					m_smac = tstr;
					DWORD ip;
					memcpy(&ip, m_csip, 4);
					memcpy(m_csip, m_cdip, 4);
					memcpy(m_cdip, &ip, 4);
					m_AuthInfo.SwapAuthInfo();
				}
				ASSERT(!m_userid.IsEmpty());
				m_state |= 1;
				// ������ݶ�ȫ�˾���������ļ�
				if (m_state == 3)
				{
					WriteWowIndexFile();
					m_state |= 4;
				}
			}
		}
		else if((packettype == 2) && (0 == (m_state & 2)))
		{
			// ��¼״̬�� ȷ����¼�ɹ�
			m_state |= 2;
			// ������ݶ�ȫ�˾���������ļ�
			if (m_state == 3)
			{
				WriteWowIndexFile();
				m_state |= 4;
			}
		}

		return 0;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "BfSession::InsertPacket() Error!!");
		return -3;	
	}
}

// �ӵ�¼�İ��л�ȡ��¼ID (�ɹ�����TRUE, ʧ�ܷ���FALSE)
BOOL WowSession::GetLoginUserid(const char* pPacket, int len)
{
// ��¼�İ� Client --> Server(����������userid UTF-8����)
// 0030                     00 08 2a 00 57 6f 57 00 03 00        ..*.WoW...
// 0040   05 4e 24 36 38 78 00 6e 69 57 00 4e 43 68 7a e0  .N$68x.niW.NChz.
// 0050   01 00 00 c0 a8 00 02 0c[e6 88 91 e7 9a 84 e8 b4  ................
// 0060   a6 e5 8f b7]                                      ....
	ASSERT(len > 34);
	if (len > 34)
	{
		char buf[260];
		memset(buf, 0, sizeof(buf));
		memcpy(buf, pPacket+34, len-34);
		int l = strlen(buf);
		if (l+34 == len)
		{
			CString tmpuserid = NetBytestoAnsi(buf, CP_ACP);
			if (!tmpuserid.IsEmpty())
			{
				tmpuserid.MakeLower();
				m_userid = tmpuserid;
				return TRUE;
			}
		}	
	}
	return FALSE;
}

void WowSession::WriteWowIndexFile()
{
	if (m_state == 3)
	{
		// �����Ҫ�İ��������һ�û������������ļ�
		m_state |= 4;

		// ��¼�ɹ� д�����ļ�
		m_OpenCaseFlg = v_opencasewow;
		m_OpenWarnFlg = v_openwarnwow;
		m_OpenIndxFlg = v_openflagwow;

		// ƥ�䰸����Ԥ����������Ϣ
		DWORD state = 0;
		SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), state,"imsg_game_wow");
		SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);

		// ������ʷ

		// д�����ļ�
		if ((m_CaseArr.GetSize() > 0) || (m_WarnArr.GetSize() > 0)
			|| (!m_rubbishflag && m_OpenIndxFlg))
		{
			SYSTEMTIME now;
			GetLocalTime(&now);
			//LPCTSTR subpro;
			CString tmppath = g_pImsgMissObj->GetTmpPath();
			ASSERT(!tmppath.IsEmpty());
			m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_1_%u_imsg_game_wow.indexfile.txt",
				tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
				now.wMinute, now.wSecond, m_sip, m_dip, rand());
			CString text;
			text.Format("user= %s\r\nstat= ����\r\n", m_userid);
			WriteIndexFile("imsg_game_wow", "imsg_game_wow", text, g_pImsgMissObj);
			m_CaseArr.RemoveAll();
			m_WarnArr.RemoveAll();
		}
	}
}
