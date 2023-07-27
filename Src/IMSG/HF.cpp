//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	HF
//	�汾:		1.0
//	��������:	2007-7-11
//	����:		��ǿ
//	��Ҫ����:	�Ʒ���Ϸ�Ĵ���
//	ģ������:
//	��Ҫ����:	IsHfPacket
//				FormatHfPacket
//**************************************************************** 
#include "stdafx.h"
#include "Imsg.h"
#include "hf.h"


HfSession::HfSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_acctype = 0xF;
	m_infonum = 0;
	m_closetm = packh.m_tm + 120;

	g_GameSion++;
}

HfSession::~HfSession()
{
	g_GameSion--;
}

int HfSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	try
	{
		if ((m_infonum == -1) || (m_acctype == -1))
		{
			return 0;	// �Ѿ��������
		}

		ASSERT(ProxyTCP == (TCPPacket+6));
		ASSERT(ProxyUDP == (UDPPacket+6));

		if ((packh.m_type==ProxyTCP) || (packh.m_type == ProxyUDP))
		{
			m_ProxyType = 1;
			packh.m_type -= 6;
		}

		FormatHfPacket(packh, pPacket);

		if ((m_infonum == 0xF0) && (m_acctype != 0xF))
		{
			m_OpenIndxFlg = v_openflaghf;
			m_OpenCaseFlg = v_opencasehf;
			m_OpenWarnFlg = v_openwarnhf;
			CString txt;
			txt.Format("%s________%s", m_userid,m_password);
			// ƥ�䰸����Ԥ����������Ϣ
			DWORD state = 0;
			SearchCaseOrWarn(this, txt, txt.GetLength(), state,"imsg_hf");
			SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);

			// ����IDC����Դ
			CString tmpisp;
			if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), tmpisp))
			{
				if (m_dataSrc == DATASRC_OTHER)
				{
					m_dataSrc = DATASRC_IDC;
					m_strISP = tmpisp;
				}
				if (v_statIdc)
				{
					CString ttstr;
					ttstr.Format("%s\1%s\1hf\1��Ϸ\1%s\1�Ʒ�\1%s\1\1", 
						m_dip.Left(15), m_strISP, m_dip.Right(5), TYPESTATE_DECIDED);
					v_IdcServerStat[IMSG_PROTOCOL].AddServer(ttstr);
				}
			}
			else if ((m_dataSrc == DATASRC_OTHER)
				&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), m_strISP)))
			{
				m_dataSrc = DATASRC_IDC;
			}

			// ������ʷ
			if (v_statGame)
			{
				HistoryRecord hisRec;
 				hisRec.SetRecord(IDX_FORWTYPE,	"1");
 				hisRec.SetRecord(IDX_ID,		m_userid);
				hisRec.SetRecord(IDX_USER,		m_userid);

				hisRec.SetRecord(IDX_STATE, "����");
				WriteHisData("imsg", "imsg_hf", &hisRec, g_pImsgMissObj,IMSG_PROTOCOL);//, HISTORY_DATA);
			}

			// ����ļ�
			LPCTSTR stracctype;
			switch(m_acctype)
			{
			case 0:
				stracctype = _T("acctype= ��սƽ̨��\r\n");
				break;
			case 1:
				stracctype = _T("acctype= �ƺ�\r\n");
				break;
			case 2:
				stracctype = _T("acctype= ʢ��ͨ��֤\r\n");
				break;
			default:
				stracctype = _T("");
				break;
			}
			txt.Format("user= %s\r\npass= %s\r\n%sstat= ����\r\n",
				m_userid, m_password, stracctype);
			m_indexfname = CreateFilename(m_sip, m_dip, "imsg_hf.indexfile.txt");
			WriteIndexFile("imsg_hf", "imsg_hf", txt, g_pImsgMissObj);
			m_CaseArr.RemoveAll();
			m_WarnArr.RemoveAll();
			m_infonum = -1;
			m_acctype = -1;
		}
		return 0;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "HfSession::InsertPacket() Error!!");
		return -3;
	}
}

//*************************************************
//	��������:		HfSession::FormatHfPacket
//	����:			�����Ʒ������ݰ�
//	�������:		packh �ǰ�ͷ�ṹ
//					pPacket �ǰ�����
//	�������:		
//	����ֵ:			
//	����:			
//**************************************************
void HfSession::FormatHfPacket(DATEHEADER& packh, const char* pPacket)
{
	int direction = 0;
	if(memcmp(pPacket, "\x28\x0\x0\x0", 4) == 0)
	{
		if(memcmp(pPacket+8, "\x03\x0\x0\x0", 4) == 0)
		{
			// Server --> Client
			m_infonum |= 0x10;
		}
		else if(memcmp(pPacket+8, "\x04\x0\x0\x0", 4) == 0)
		{
			// Client --> Server
			m_infonum |= 0x20;
		}
		else	// ��½ʧ��
		{
			m_infonum = 0;
			m_userid.Empty();
			m_password.Empty();
			m_acctype = 0xF;
		}
	}
	else if(memcmp(pPacket, "\xb0\x01\x0\x0", 4) == 0)
	{
		// �û���½�� Client --> Server
		if(packh.m_datalen < 128)
			return ;

		if(memcmp(packh.m_sip, m_csip, 4) == 0)
		{
			direction = 1;
		}
		else
		{
			direction = 2;
		}
		m_infonum |= 0x40;
		m_acctype = *(DWORD*)(pPacket+20);
		if(m_acctype == 1)
		{
			m_userid.Format("%u", *(DWORD*)(pPacket+40));
		}
		else
		{
			m_userid = CString(pPacket+40);
		}
		m_password = CString(pPacket+72);
	}
	else if((memcmp(pPacket+1, "\x04\x0\x0", 3) == 0)
	 || (memcmp(pPacket+1, "\x04\x0\x0", 3) == 0) 
	 || (memcmp(pPacket+1, "\x05\x0\x0", 3) == 0) )
	{
		// ��½��Ӧ�� Server --> Client
		if(memcmp(pPacket+8, "\0\0\0\0", 4) == 0)
		{
			m_infonum |= 0x80;
			if(memcmp(packh.m_dip, m_csip, 4) == 0)
			{
				direction = 1;
			}
			else
			{
				direction = 2;
			}
		}
	}
	if (direction == 2)
	{
		// �����Ự����
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

/// �Ʒ���սƽ̨ /////////////////////////////////////////////////////////
//
//��½��Ϊ1201�˿�, ��Ϊ�Ĳ�.
//
//��ʼ
//
//1, �ͻ��˷�   28 00 00 00 ��ͷ�����ְ�.
//2, ��������Ӧ 28 00 00 00 ��ͷ�İ�, ��ͻ��˰�����ֻ��һ�ֽڲ�ͬ.
//3, �ͻ��˷� �û���/���� ��½��. ��b0 01 00 00��ͷ, 
//   ƫ��20λ������ֽڱ����ʺ�����02Ϊʢ��ͨ��֤, 01Ϊ�ƺ�, 00Ϊ��սƽ̨��.
//   ƫ��40�ֽں�Ϊ��½�û���, ƫ��72�ֽں�Ϊ����, ����16λ�Ĵ�.
//   ֮��Ϊƽ̨�汾����Ϣ.
//4, ���������سɹ������. �ɹ���68 04 00 00��ͷ(V4.5.8�� 98 04 00 00��ͷ), ��ƫ��8λ��4�ֽں�Ϊ00 00 00 00. 
//   ʧ����28 00 00 00��ͷ, ƫ��8λ���4�ֽڱ�ʾʧ��ԭ��, ��֪, 12 00 00 00, Ϊ�������, 07 00 00 00Ϊ������æ.
//
//����.
// �������ɹ���, xx 04 00 ��ʼ, xx ����Ϊ�汾, �汾�仯���λ�仯.
//////////////////////////////////////////////////////////////////////////
