//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	VP
//	�汾:		1.0
//	��������:	2007-7-12
//	����:		��ǿ
//	��Ҫ����:	VP����Ļ�ԭ
//	ģ������:	
//	��Ҫ����:	
//
//**************************************************************** 

#include "stdafx.h"
#include "imsg.h"
#include "VP.h"

VPIMSession::VPIMSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_direction = 0;
	m_closetm = packh.m_tm + 180;
	m_bIdc = FALSE;
	m_MsgBuflen = 0;
	m_pMsgBuf = NULL;
	m_begfiletm = 0;
	
	g_ImsgSion++;
}

VPIMSession::~VPIMSession()
{
	m_MsgBuflen = 0;
	if (m_pMsgBuf != NULL)
	{
		delete[] m_pMsgBuf;
	}

	g_ImsgSion--;
}

BOOL VPIMSession::CloseSession(time_t nowtm)
{
	try
	{
		if (m_begfiletm == 0)
		{
			// �Դ��ϴδ��ļ�����һֱû�а���������ٸûỰ
			return TRUE;
		}
		
		// ����ļ�
		if (m_MsgBuflen > 0)
		{
			// дʵ���ļ�
			WriteRealFile(g_pMsgBuffer, 0);
		}

		// ���ﲻ����IDCServer �� ���ʷ
		// ��������Դ
		if(m_dataSrc == DATASRC_OTHER)
		{
			if ((v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), m_strISP))
				|| (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), m_strISP)))
			{
				m_dataSrc = DATASRC_IDC;
			}
		}

		// ƥ�䰸����Ԥ��
		m_OpenCaseFlg = v_opencaseVp;
		m_OpenWarnFlg = v_openwarnVp;
		m_OpenIndxFlg = v_openflagVp;
		DWORD state = 0;
		SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), state);
		SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);
	
		// ƥ������
		CheckLanguage(g_pMsgBuffer, MSGBUFFERLEN);

		// д�����ļ�
		if (m_realfname.IsEmpty())
		{
			m_indexfname = CreateFilename(m_sip, m_dip, "imsg_vp.txt.indexfile.txt");
		}
		else
		{
			m_indexfname = m_realfname + ".indexfile.txt";
		}
		CString str;
		str.Format("user= %s\r\n", m_userid);
		WriteIndexFile("imsg_vp", "imsg_vp", str, g_pImsgMissObj);
		m_CaseArr.RemoveAll();
		m_WarnArr.RemoveAll();
		m_begfiletm = 0;
		m_closetm += 180;
		return FALSE;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "VPIMSession::CloseSession() Error!!");
		return TRUE;
	}
}

int VPIMSession::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	try
	{
		if (m_begfiletm == 0)
		{
			m_begfiletm = packh.m_tm;
		}
		WORD plen = *((WORD*)pPacket);
		if ((plen <= 12) || (plen > packh.m_datalen))
		{
			return 0;
		}
		const char *tp = pPacket;
		g_Msglen = 0;
		do
		{
			if (stricmp(tp+12, "ClientText") == 0)
			{
				g_Msglen += FormatVpIM(packh, pPacket);		// ������Ϣ
			}
			else if (stricmp(tp+12, "SystemMessage") == 0)	// ϵͳ��Ϣ (�������ֿͻ���)
			{
				// Server --> Client
				if (m_direction == 0)	// ��û��ȷ������
				{
					if (memcmp(packh.m_sip, m_csip, 4)==0)
					{
						m_direction = 2;
					}
					else
					{	
						m_direction = 1;
					}
				}
			}
			else if (stricmp(tp+12, "VerifyUser") == 0)
			{
				g_Msglen += UserLogin(packh, pPacket);	// ��¼
			}

			if (m_direction == 2)
			{
				// �����Ự����
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
				m_direction = 1;
			}

			WORD len = *((WORD*)(pPacket+plen));
			if (len<=12)
			{
				break;
			}
			tp = pPacket+plen;
			plen += len;
		}while(plen <= packh.m_datalen);

		if (g_Msglen > 0)
		{
			// дʵ���ļ�
			memset(g_pMsgBuffer + g_Msglen, 0, 4);
			if (g_Msglen + m_MsgBuflen >= MSGBUFLEN)
			{
				// д�ļ�
				WriteRealFile(g_pMsgBuffer, g_Msglen);
			}
			else
			{
				if (m_pMsgBuf == NULL)
				{
					m_pMsgBuf = new char[MSGBUFLEN];
				}
				memcpy(m_pMsgBuf + m_MsgBuflen, g_pMsgBuffer, g_Msglen+1);
				m_MsgBuflen += g_Msglen;
			}
			g_Msglen = 0;
		}
 	}
	catch(...)
	{
		WriteLog(IMSGPRO, "VpSession::InsertPacket() Error!!");
		return -3;
	}
	return 0;
}

int VPIMSession::UserLogin(DATEHEADER& packh, const char* pPacket)
{
	LPCTSTR user = "";
	if (stricmp(pPacket + 12+ 64, "UserID") != 0)
	{
		return 0;
	}
	
	DWORD ulen = *((DWORD*)(pPacket + 12 + 64 + 24));
	user = pPacket + 12 + 64 + 28;
	if (strlen(user) + 1 != ulen)
	{
		return 0;
	}

	int len = 0;
	m_userid = user;
	LPCTSTR tmproomid = "";
	const char* p = pPacket + 12 + 64 + 28 + ulen;
	ulen = *((DWORD*)(p + 24));
	if (stricmp(p + ulen + 28, "RoomID") == 0)
	{
		p += ulen + 28;
		ulen = *((DWORD*)(p + 24));
		tmproomid = p + 28;
		if (strlen(tmproomid) + 1 == ulen)
		{
			m_roomid = tmproomid;
		}
	}

	if (m_direction == 0)
	{
		if(memcmp(packh.m_sip, m_cdip, 4) == 0)
		{
			// �����Ự����
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
		m_direction = 1;
	}

	len += GetTimeString(packh.m_tm, "VP\t%Y-%m-%d %H:%M:%S\t",
		g_pMsgBuffer + g_Msglen + len, 80);
	len += sprintf(g_pMsgBuffer + g_Msglen + len,
		"%s �� %s    %s �� ������:\r\n��¼��������[����id:%s]\r\n\r\n",
		m_sip, m_dip, user, m_roomid);

	m_userid = user;

	// ��������Դ��дIDCServer
	CString str;
	if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), str))
	{
		if (m_dataSrc == DATASRC_OTHER)
		{
			m_dataSrc = DATASRC_IDC;
			m_strISP = str;
		}
		if (v_statIdc)
		{
			CString ttstr;
			ttstr.Format("%s\1%s\1vp\1��ʱ��Ϣ\1%s\1VP\1%s\1\1",
				m_dip.Left(15), m_strISP, m_dip.Right(5), TYPESTATE_DECIDED);
			v_IdcServerStat[IMSG_PROTOCOL].AddServer(ttstr);
		}
	}
	
	// д���ʷ
 	if (v_statImsgOther)
	{
		HistoryRecord hisRec;
 		hisRec.SetRecord(IDX_FORWTYPE,	"1");
		hisRec.SetRecord(IDX_ID,		m_userid);
		hisRec.SetRecord(IDX_USER,		m_userid);

		hisRec.SetRecord(IDX_STATE,		"��¼");
		WriteHisData("imsg", "imsg_vp", &hisRec, g_pImsgMissObj,IMSG_PROTOCOL);//, HISTORY_DATA);
	}
	return len;
}

int VPIMSession::FormatVpIM(DATEHEADER& packh, const char* pPacket)
{
	int len = 0;
	LPCTSTR fromuser="";
	LPCTSTR touser="";
	if (stricmp(pPacket + 32, "FromUser") != 0)
	{
		return 0;
	}

	DWORD ulen = *((DWORD*)(pPacket + 48 + 8));
	fromuser = pPacket + 48 + 12;
	if ((strlen(fromuser) + 1 != ulen)
		|| (stricmp(pPacket + 48 + 12 + ulen, "ToUser") != 0))
	{
		return 0;
	}

	touser = pPacket + 48 + 12 + ulen + 24 + 4;
	ulen = *((DWORD*)(pPacket + 48 + 12 + ulen + 24));
	if (strlen(touser)+1 != ulen)
	{
		return 0;
	}

	if ((ulen==0x11)
		&& (stricmp(touser, "_system_command_") == 0))
	{
		// Client --> Server
		if (m_direction == 0)
		{
			if(memcmp(packh.m_sip, m_cdip, 4) == 0)
			{
				// �����Ự����
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
			m_direction = 1;
		}
	}
	else if (m_direction == 1)	// �Ѿ���ȷ������
	{
		CString msg = GetImsg(touser+ulen);
		if (memcmp(packh.m_sip, m_csip, 4)==0)
		{
			// Client --> Server
			if (m_userid.IsEmpty())
			{
				m_userid = fromuser;
			}
			if (!msg.IsEmpty() && !m_userid.IsEmpty())
			{
				len += GetTimeString(packh.m_tm, "VP\t%Y-%m-%d %H:%M:%S\t",
					g_pMsgBuffer + g_Msglen + len, 80);
				len += sprintf(g_pMsgBuffer + g_Msglen + len,
					"%s �� %s    %s �� %s:\r\n%s\r\n\r\n",
					m_sip, m_dip, fromuser, touser, msg);
			}
		}
		else
		{
			// Server --> Client
			if (!msg.IsEmpty() && !m_userid.IsEmpty()
				&& (fromuser != m_userid))
			{
				len += GetTimeString(packh.m_tm, "VP\t%Y-%m-%d %H:%M:%S\t",
					g_pMsgBuffer + g_Msglen + len, 80);
				len += sprintf(g_pMsgBuffer + g_Msglen + len,
					"%s �� %s    %s �� %s:\r\n%s\r\n\r\n",
					m_sip, m_dip, touser, fromuser, msg);
			}
		}
	}
	else
	{
		ASSERT(m_direction == 0);	// δ��ȷ����
	}
	return len;
}

// дʵ���ļ�
void VPIMSession::WriteRealFile(char* pData, int datalen)
{
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateFilename(m_sip, m_dip, "imsg_vp.txt");
		ASSERT(!m_realfname.IsEmpty());
	}
	FILE* fp = fopen(m_realfname, "ab");
	if (fp == NULL)
	{
		return;
	}
	if (m_MsgBuflen > 0)
	{
		fwrite(m_pMsgBuf, m_MsgBuflen, 1, fp);
		m_MsgBuflen = 0;
		delete[] m_pMsgBuf;
		m_pMsgBuf = NULL;
	}
	if (datalen > 0)
	{
		fwrite(pData, datalen, 1, fp);
	}
	fclose(fp);
}

CString	VPIMSession::GetImsg(const char* pPacket)
{
	CString msg;
	if (stricmp(pPacket, "text")==0)
	{
		DWORD txtlen = *((DWORD*)(pPacket+24));
		LPCTSTR ptxt = pPacket+28;
		if (strlen(ptxt)+1 == txtlen)
		{
			if (ptxt[0]=='[')
			{
				ptxt = strchr(ptxt, ']');
				if (ptxt!=NULL)
					ptxt++;
				else
					ptxt = pPacket+28;
			}
			msg = ptxt;
		}
	}
	return msg;
}
