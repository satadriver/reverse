//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	OurGame
//	�汾:		1.0
//	��������:	2007-7-12
//	����:		��ǿ
//	��Ҫ����:	��������Э��Ļ�ԭ
//	ģ������:	
//	��Ҫ����:	
//
//**************************************************************** 

#include "stdafx.h"
#include "imsg.h"
#include "ourgame.h"

// �ṹ����
struct OURGAMEMSG
{
	CString strID;
	CString strNikID;
	CString	strMsg;
	CString strCaseMsg;
	CString strPass;
	int intMsgtype;
};

OurGameSession::OurGameSession(DATEHEADER& packh, int aT)
:SocketSession(packh, aT)
{
	m_ProcessTime = packh.m_tm;
	m_closetm = packh.m_tm + 180;
	m_bSortList = TRUE;
	m_processseq1 = 0;
	m_processseq2 = 0;
	m_MsgBuf = NULL;
	m_MsgBufLen = 0;

	m_loginack=0;			// ��¼�������ACK(��¼״̬����ACK>��¼�������ACK+1204)����Ϊ�ǵ�¼�ɹ�
	m_stateack=0;			// ��¼״̬����ACK
	m_state=0;

	if (packh.m_dport != 2002 && packh.m_dport != 443)
	{
		// �����Ự����
//		ASSERT(packh.m_sport == 2002);
		CString tstr = m_sip;
		m_sip = m_dip;
		m_dip = tstr;
		
		tstr = m_dmac;
		m_dmac = m_smac;
		m_smac = tstr;

		m_AuthInfo.SwapAuthInfo();

		memcpy(m_csip, packh.m_dip, 4);
		memcpy(m_cdip, packh.m_sip, 4);
	}
	g_GameSion++;
}

OurGameSession::~OurGameSession()
{

	g_GameSion--;
}

int OurGameSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	
	if (packh.m_type == ProxyTCP)
	{
		m_ProxyType = 1;
	}

	// �����û���¼ (����ǵ�¼���ͷ���TRUE)
	if (ProcessLocin(packh, pPacket))
		return 0;	

	// �����������Ϣ
	if ((memcmp(pPacket, "\x8\x3", 2) == 0) && (packh.m_datalen >= 68))
	{
		if ((pPacket[12] == 0) || (memcmp(pPacket+68, "��ȥ", 4) == 0))
		{
			return 0;
		}
	}
	else if (!memcmp(pPacket, "\x5\x3", 2) == 0)
	{
		return 0;
	}

	DTcpPacket ourgamepack(packh, (CHAR *)pPacket);
	m_bSortList = FALSE;
	m_PacketList.push_back(ourgamepack);
	m_lastpacktm = packh.m_tm;
	ourgamepack.buf = NULL;
	if (abs(packh.m_tm - m_ProcessTime) >= 40)
	{
		// ���������еİ�
		ProcessOurGameList(packh.m_tm);
	}
	return 0;
}

BOOL OurGameSession::CloseSession(time_t nowtm)
{
	if (m_state & 4)
	{
		// ����ǵ�¼��Ϣ���Ѿ�����������ļ��ľ�����session
		return TRUE;
	}

	ProcessOurGameList(nowtm);
	if (m_MsgBufLen > 0)
	{
		WriteRealFile(g_pMsgBuffer, 0);
	}
	if (!m_realfname.IsEmpty())
	{
		// ƥ��IDC����Դ
		CString str;
		if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), str))
		{
			if (m_dataSrc == DATASRC_OTHER)
			{
				m_dataSrc = DATASRC_IDC;
				m_strISP = str;
			}
			// дIDCServer
			CString ttstr;
			ttstr.Format("%s\1%s\1imsg_lianzhong\1��Ϸ\1%s\1������Ϸ\1%s\1\1",
				m_dip.Left(15), m_strISP, m_dip.Right(5), TYPESTATE_DECIDED);
			v_IdcServerStat[IMSG_PROTOCOL].AddServer(ttstr);
		}
		else if ((m_dataSrc == DATASRC_OTHER)
			|| (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), str)))
		{
			m_dataSrc = DATASRC_IDC;
			m_strISP = str;
		}

		// ƥ�䰸����Ԥ����������Ϣ
		DWORD dwstate = 0;
		SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), dwstate);
		SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);
		m_OpenCaseFlg = v_opencaselzgame;
		m_OpenWarnFlg = v_openwarnlzgame;
		m_OpenIndxFlg = v_openflaglzgame;

		// ƥ������
		CheckLanguage(g_pMsgBuffer, MSGBUFFERLEN);

		// ����ļ�
		CString text;
		if (!m_userid.IsEmpty())
		{
			text.Format("user= %s\r\n", m_userid);
		}
		m_indexfname = m_realfname + ".indexfile.txt";
		WriteIndexFile("imsg_lianzhong", "imsg_lianzhong", text, g_pImsgMissObj);
		m_CaseArr.RemoveAll();
		m_WarnArr.RemoveAll();
		m_begfiletm = m_ProcessTime;
	}

	if ((nowtm - m_lastpacktm) > 500)
	{
		ASSERT(m_PacketList.size() == 0);
		return TRUE;		// �����Ự
	}
	else
	{
		m_closetm += 180;
		return FALSE;		// �����Ự
	}
}

void FormatOurGameMsg(OURGAMEMSG *msg, const char* pPacket, int len)
{
	if ((pPacket[12] != 0) &&	// ����ϵͳ��Ϣ
		(memcmp(pPacket+68, "��ȥ", 4) != 0))	// �����л���Ϸ��
	{
		msg->strID = pPacket + 12;
		msg->strNikID = pPacket + 32;
		int tlen = len - 68;
		memcpy(msg->strMsg.GetBufferSetLength(tlen), pPacket + 68, tlen);
		msg->strMsg.ReleaseBuffer(tlen);
		msg->strMsg.Replace(0x0C,' ');
		msg->intMsgtype = 1;
	}
}

void FormatOurGameUserPass(OURGAMEMSG *msg, char* pPacket, int len)
{
	int nidpos;
	if(pPacket[12]=='R'|| pPacket[12]=='r' || pPacket[12]=='T' || pPacket[12]=='t')
	{
		nidpos = 664;
	}
	else if(memcmp(&pPacket[12],"G30003",6)==0)
	{
		nidpos = 680;
	}
	else if(memcmp(&pPacket[12],"G20016",6)==0 || \
		memcmp(&pPacket[12],"G10004",6)==0 || \
		memcmp(&pPacket[12],"G30002",6)==0 || \
		memcmp(&pPacket[12],"G10007",6)==0 || \
		memcmp(&pPacket[12],"G20008",6)==0)
	{
		nidpos = 676;
	}
	else if(memcmp(&pPacket[12],"G10002",6)==0)
	{
		// G10002 IDƫ�ƽذ��������������, ������������
		if(len == 744)
			nidpos = 672;
		else if(len == 824)
			nidpos = 684;
		else
		{
			// δ֪���
			return ;
		}
	}
	else
	{
		// 14 00 00 00 ID(20byte) �ǳ�
		nidpos = 672;
		DWORD tt = *(DWORD*)(pPacket+nidpos);
		if(tt == 0x14)
			nidpos += 4;
		else
		{
			// δ֪���
			return ;
		}
	}
	
	if (len > nidpos)
	{
		memcpy(msg->strID.GetBufferSetLength(len - nidpos),
			pPacket+nidpos, len - nidpos);
		msg->strID.ReleaseBuffer();
		nidpos += 20;
		if (len > nidpos)
		{
			memcpy(msg->strNikID.GetBufferSetLength(len - nidpos),
				pPacket+nidpos, len - nidpos);
			msg->strNikID.ReleaseBuffer();
		}
		msg->intMsgtype=2;
	}
	return;
}

int OurGameSession::ProcessOurGameList(time_t now)
{
	if (!m_bSortList)
	{
		m_PacketList.sort();
		m_bSortList = TRUE;
	}
	g_Msglen = 0;

 	DTcpPacketListIt item, end;
	item = m_PacketList.begin();
	end = m_PacketList.end();
	while(item != end)
	{
		DTcpPacket* pPacket = &(*item);
		if ((now - pPacket->tm) < 20)		// ����20�������
		{
			break;
		}
		int direction = 0;
		if (memcmp(pPacket->sip, m_csip, 4) == 0)
		{
			if ((m_processseq1 == 0)
				|| (0x80000000 > (pPacket->Ssequence - m_processseq1)))
			{
				m_processseq1 = pPacket->Ssequence;
				direction = 1;
			}
		}
		else
		{
			if ((m_processseq2 == 0)
				|| (0x80000000 > (pPacket->Ssequence - m_processseq2)))
			{
				m_processseq2 = pPacket->Ssequence;
				direction = 2;
			}
		}

		if (direction != 0)
		{
			ProcessOurGamePacket(*pPacket, direction);
		}
		DTcpPacketListIt tmpitem = item;
		item++;
		m_PacketList.erase(tmpitem);
		if (g_Msglen > (MSGBUFFERLEN - 4096))
		{
			break;
		}
	}
	
	if (g_Msglen > 0)
	{
		// дʵ���ļ�
		memset(g_pMsgBuffer + g_Msglen, 0, 4);
		if (g_Msglen + m_MsgBufLen >= MSGBUFLEN)
		{
			// д�ļ�
			WriteRealFile(g_pMsgBuffer, g_Msglen);
		}
		else
		{
			if (m_MsgBuf == NULL)
			{
				m_MsgBuf = new char[MSGBUFLEN];
			}
			memcpy(m_MsgBuf + m_MsgBufLen, g_pMsgBuffer, g_Msglen+1);
			m_MsgBufLen += g_Msglen;
		}
		g_Msglen = 0;
	}
	return 0;
}

int OurGameSession::ProcessOurGamePacket(DTcpPacket& pack, int direction)
{
	m_ProcessTime = pack.tm;
	if (m_begfiletm == 0)
	{
		m_begfiletm = pack.tm;
	}
	int blen = g_Msglen;
	try
	{
		OURGAMEMSG OurGameMsg;
		OurGameMsg.intMsgtype = 0;
		if ((memcmp(pack.buf, "\x8\x3", 2) == 0) && (pack.Slen >= 68))
		{
			FormatOurGameMsg(&OurGameMsg, pack.buf, pack.Slen);
		}
		else if((memcmp(pack.buf, "\x5\x3", 2) == 0) && (pack.Slen >= 684))
		{
			FormatOurGameUserPass(&OurGameMsg, pack.buf, pack.Slen);
		}
		else
		{
			return 0;
		}

		if (OurGameMsg.intMsgtype == 0)
		{
			return 0;
		}
		
		if (OurGameMsg.intMsgtype == 2)
		{
			if (m_userid.Find("�ǳ�:")<0)
			{
				if (!OurGameMsg.strNikID.IsEmpty())
				{
					m_userid.Format("%s[�ǳ�:%s]", OurGameMsg.strID, OurGameMsg.strNikID);
				}
				else
				{
					m_userid = OurGameMsg.strID;
				}
			}
		}
		else
		{
			g_Msglen += GetTimeString(pack.tm, "������Ϸ\t%Y-%m-%d %H:%M:%S\t",
				g_pMsgBuffer + g_Msglen, 80);
			if ((direction == 1) && (m_userid.IsEmpty()))
			{
				if (!OurGameMsg.strNikID.IsEmpty())
				{
					m_userid.Format("%s[�ǳ�:%s]", OurGameMsg.strID,
						OurGameMsg.strNikID);
				}
				else
				{
					m_userid = OurGameMsg.strID;
				}
			}
			if (direction == 1)
			{
				g_Msglen+=sprintf(g_pMsgBuffer + g_Msglen,
					"%s �� %s\r\n%s˵��: %s\r\n\r\n",
					m_sip, m_dip, m_userid, OurGameMsg.strMsg);
			}
			else
			{
				g_Msglen+=sprintf(g_pMsgBuffer + g_Msglen,
					"%s �� %s\r\n%s[%s]˵��: %s\r\n\r\n",
					m_sip, m_dip, OurGameMsg.strNikID, OurGameMsg.strID,
					OurGameMsg.strMsg);
			}
		}
	}
	catch(...)
	{
		g_Msglen = blen;
	}
	return 0;
}

void OurGameSession::WriteRealFile(char* pData, int datalen)
{
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateFilename(m_sip, m_dip, "imsg_lianzhong.txt");
		ASSERT(!m_realfname.IsEmpty());
	}
	FILE* fp = fopen(m_realfname, "ab");
	if (fp == NULL)
	{
		return;
	}
	if (m_MsgBufLen > 0)
	{
		fwrite(m_MsgBuf, m_MsgBufLen, 1, fp);
		m_MsgBufLen = 0;
		delete[] m_MsgBuf;
		m_MsgBuf = NULL;
	}
	fwrite(pData, datalen, 1, fp);
	fclose(fp);
}

// �����û���¼ (����ǵ�¼���ͷ���TRUE)
BOOL OurGameSession::ProcessLocin(DATEHEADER& packh, const char *pPacket)
{
	// �����¼����Ϣ
// 	DWORD			m_loginack;			// ��¼�������ACK(��¼״̬����ACK>��¼�������ACK+1204)����Ϊ�ǵ�¼�ɹ�
//	DWORD			m_stateack;			// ��¼״̬����ACK
// 	DWORD			m_state;			// 1 ��ʾ����˵�¼�����
// 										// 2 ��ʾ����˵�¼״̬��
//										// 4 ��ʾ�Ѿ�����˶����ļ�
	if (packh.m_datalen <= 4)
		return FALSE;

	if ((memcmp(pPacket, "\x63\x06\0\0", 4) == 0)		// ��¼�����1����(�ð����ܰ�����¼�����2)
		|| (memcmp(pPacket, "\x7d\0\0\0", 4) == 0))		// ��¼�����2����(�ð��п��Ի��userid)
	{
		if(CheckPacket(pPacket, packh.m_datalen))
		{
			if ((0 == (m_state & 1)) && (!m_userid.IsEmpty()))
			{
				m_loginack = packh.m_ack;
				m_state |= 1;
				if (m_state == 3)
				{
					WriteLoginFile();
				}
			}
			return TRUE;
		}
	}
	else if (memcmp(pPacket, "\x65\0\0\0", 4) == 0)		// ��¼״̬��
	{
		// �ͻ��˵���������¼�İ�
		if (CheckPacket(pPacket, packh.m_datalen))
		{
			if (0 == (m_stateack & 2))
			{
				m_stateack = packh.m_ack;
				m_state |= 2;
				if (m_state == 3)
				{
					WriteLoginFile();
				}
			}
			return TRUE;
		}
	}
	return FALSE;
}

// У����ṹ
BOOL OurGameSession::CheckPacket(const char* pPacket, int len)
{
	int pos = 0;
	while(len > 8)
	{
		DWORD head[2];
		memcpy(head, pPacket+pos, 8);
		DWORD slen;
		if (head[0] & 0x80000000)
			slen = head[1]+4+8;
		else
			slen = head[1]+8;

		if (slen > len)
			break;
		if (head[0] == 0x7D)
		{
			// ��¼������
			if (GetUserid(pPacket+pos, slen))
				return TRUE;
		}
		len -= slen;
		pos += slen;
	}
	return (len==0);
}

// ��ȡ�û��ĵ�¼ID
BOOL OurGameSession::GetUserid(const char* pPacket, int len)
{
	ASSERT(len > 8);
	ASSERT(memcmp(pPacket, "\x7d\0\0\0", 4) == 0);
	char id[32];
	memset(id, 0, sizeof(id));
	if ((len > 40) && (pPacket[40] == 0x20))
	{
		memcpy(id, pPacket+8+12, 20);
		int l = strlen(id);
		if (l > 0)
		{
			for (int i=0; i<l; i++)
			{
				if (id[i] < 0x20)
					return FALSE;
			}
			m_userid = id;
			return TRUE;
		}
	}
	return FALSE;	
}

// �����¼��Ϣ�ļ�
void OurGameSession::WriteLoginFile()
{
	if (m_state == 3)
	{
		// �����Ҫ�İ��������һ�û������������ļ�
		m_state |= 4;
		int ackwindage = m_stateack - m_loginack;
		if (ackwindage <= 0)
		{
			WriteLog(IMSGPRO, "Ourgame ACK����� loginack=%u stateack=%u Error!!", m_loginack, m_stateack);
			return;
		}
		if (ackwindage > 256)
		{
			// ��¼�ɹ� д�����ļ�
			m_OpenCaseFlg = v_opencaselzgame;
			m_OpenWarnFlg = v_openwarnlzgame;
			m_OpenIndxFlg = v_openflaglzgame;

			// ƥ�䰸����Ԥ����������Ϣ
			DWORD state = 0;
			SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), state);
			SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);

			// ������ʷ

			// д�����ļ�
			if ((m_CaseArr.GetSize() > 0) || (m_WarnArr.GetSize() > 0)
				|| (!m_rubbishflag && m_OpenIndxFlg))
			{
				SYSTEMTIME now;
				GetLocalTime(&now);
// 				LPCTSTR subpro;
				CString tmppath = g_pImsgMissObj->GetTmpPath();
				ASSERT(!tmppath.IsEmpty());
				m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_1_%u_imsg_lianzhong.indexfile.txt",
					tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
					now.wMinute, now.wSecond, m_sip, rand());
				CString text;
				text.Format("user= %s\r\nstat= ����\r\n", m_userid);
				WriteIndexFile("imsg_lianzhong", "imsg_lianzhong", text, g_pImsgMissObj);
				m_CaseArr.RemoveAll();
				m_WarnArr.RemoveAll();
			}
		}
	}
}
