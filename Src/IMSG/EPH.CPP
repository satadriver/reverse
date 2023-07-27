//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	EPH
//	�汾:		1.0
//	��������:	2007-7-11
//	����:		��ǿ
//	ģ������:	E��ͨ����Э��Ļ�ԭ
//
//**************************************************************** 

#include "stdafx.h"
#include "imsg.h"
#include "Eph.h"

/************************* E��ͨ **************************/
EphSession::EphSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 60;
	m_direction = 0;
	m_state = 0;
	g_ImsgSion++;
}

EphSession::~EphSession()
{
	g_ImsgSion--; 
}

// �������ݰ����Ự
int EphSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	try
	{
		if ((packh.m_type == ProxyTCP) || (packh.m_type == ProxyUDP))
		{
			m_ProxyType = 1;
			packh.m_type -= 6;
		}

		// �����E��ͨ�İ��ͱ���
		if(isEPHPacket(packh, pPacket))
		{
			ProcessEphPacket(packh, (const BYTE*)pPacket);
		}
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "EphSession::InsertPacket() Error!!");
		return -3;
	}
	return 0;
}

BOOL EphSession::CloseSession(time_t nowtm)
{
	if ((1 == (m_state & 3))
		|| (2 == (m_state & 3)))
	{
		// ���ߵİ�û�л������ (������ֻ��һ�����������)
		m_begfiletm = m_lastpacktm;		// ʱ��ʹ�ô����Ự��ʱ��
		OutIndexFile("����");
	}
	return TRUE;
}

//*************************************************
//	��������:		EphSession::WriteEphmsgFile
//	����:			��ԭһ��������ö���Ļ״̬(������)
//	�������:		packh �ǰ�ͷ
//					pData ��һ����������
//	�������:		
//	����ֵ:			�л�ԭ�������ݷ���TRUE���򷵻�FALSE
//	����:			
//**************************************************
BOOL EphSession::ProcessEphPacket(DATEHEADER& packh, const BYTE* pData)
{
	LPCTSTR statetxt = "";
	DWORD tmpState = 0;
	DWORD intuserid = 0;		// UserID
	int direction = 0;
	intuserid = Swap4Byte(*(DWORD*)(pData + 0x6));
	if (intuserid == 0)
	{
		return FALSE;
	}

	//�е�½��
	if((memcmp(pData, "\x72\x20\0\0",4) == 0)
		&& (pData[0xA] == 0)
		&& (*(USHORT*)(pData+0xe) == 0x0400)
		&& (pData[0x14] == 0))
	{
		if (*(USHORT*)(pData + 0x12) == 0x0002)
		{
			// Client --> Server
			direction = 1;
			m_userid.Format("%u", intuserid);
			tmpState = 1;	// ֻ�����ID
			statetxt = "����";
			const BYTE* subip = (pData + 0x3d);
			m_subip.Format("%03d.%03d.%03d.%03d",subip[0],subip[1],subip[2],subip[3]);

		}
		else if ((Swap2Byte(*(WORD*)(pData+0x10)) - Swap2Byte(*(WORD*)(pData+0x12))) == 1)
		{
			// Server --> Client
			direction = 2;
			// ����ǳ�
			CHAR nick[100];
			memset(nick, 0, sizeof(nick));
			if ((pData[0x21] < 40) && (pData[0x22+pData[0x21]] == 0))
			{
				strncpy(nick, (char*)pData+0x22, pData[0x21]);
				if (nick[0] != 0)
				{
					m_Nick.Format("%s", nick);	// �ǳ�
				}

				m_userid.Format("%u", intuserid);
				statetxt = "����";
				tmpState = 2;
			}
		}
	} 
	else if((memcmp(pData, "\x72\x20\0\0",4) == 0)//����
		&& (pData[0xA] == 0)
		&& (*(USHORT*)(pData+0xe) == 0x0500)
		&& (pData[0x14] == 0)) 
	{
		// Client --> Server
		direction = 1;
		m_userid.Format("%u", intuserid);
		tmpState |= 0x10;	// ���߰�
		statetxt = "����";
	}

	if ((m_direction == 0) && (direction != 0))
	{
		if (memcmp(packh.m_sip, m_csip, 4) == 0)
		{
			m_direction = direction;
		}
		else
		{
			m_direction = (direction == 1)? 2 : 1;
		}
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

	if ((m_state & tmpState) != tmpState)
	{
		m_state |= tmpState;
		if ((tmpState == 0x10)				// ����
			|| (3 == (m_state & 3)))		// ����
		{
			// ��������ļ�
			m_begfiletm = packh.m_tm;		// ʱ��ʹ�õ�ǰ����ʱ��
			OutIndexFile(statetxt);
		}
	}
	return TRUE;
}

//*************************************************
//	��������:		OutIndexFile
//	����:			���һ��DoShow�Ķ�λ��Ϣ
//	�������:		state ����Ļ״̬
//	�������:		
//	����ֵ:			
//	����:			
//*************************************************
void EphSession::OutIndexFile(LPCTSTR state)
{
	ASSERT(!m_userid.IsEmpty());
	ASSERT(state != NULL);
	ASSERT(state[0] != 0);
	DWORD dwstate = 0;

	// ƥ������
	CString text;
	text.Format("%s_______%s", m_userid,m_Nick);
	m_OpenCaseFlg = v_opencaseEht;
	m_OpenWarnFlg = v_openwarnEht;
	m_OpenIndxFlg = v_openflagEht;
	SearchCaseOrWarn(this, text, text.GetLength(), dwstate);
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
			ttstr.Format("%s\1%s\1doshow\1��ʱ��Ϣ\1%s\1����\1%s\1\1", 
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
	if (v_statImsgOther)
	{
		HistoryRecord hisRec;
		hisRec.SetRecord(IDX_FORWTYPE,	"1");
		hisRec.SetRecord(IDX_ID,		m_userid);
		 hisRec.SetRecord(IDX_USER,		m_userid);

		hisRec.SetRecord(IDX_STATE, state);
		WriteHisData("imsg", "imsg_doshow", &hisRec, g_pImsgMissObj,IMSG_PROTOCOL);//, HISTORY_DATA);
	}
	

	// ����ļ�
	if (!m_Nick.IsEmpty())
	{
		text.Format("user= %s[�ǳ�:%s]\r\nstat= %s\r\n", m_userid, m_Nick, state);
	}
	else
	{
		text.Format("user= %s\r\nstat= %s\r\n", m_userid, state);
	}
	if (m_subip)
	{
		text.Format("%ssubip= %s\r\n", (CString)text, m_subip);
	}
	m_indexfname = CreateFilename(m_sip, m_dip, "imsg_doshow.indexfile.txt");
	WriteIndexFile("imsg_doshow", "imsg_doshow", text, g_pImsgMissObj);
	return;
}



//////////////////////new doshow ���컹ԭ////////////////////////////////////////////////////
ImsgEphSession::ImsgEphSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
#ifdef		_DEBUG
	m_closetm = packh.m_tm + 60;
#else
	m_closetm = packh.m_tm + 180;
#endif
	m_ProcessTime = packh.m_tm;
	m_PackCount = 0;
	m_MsgBuflen = 0;
	m_pMsgBuf = NULL;
	m_direction = 0;


	g_ImsgSion++;
}

ImsgEphSession::~ImsgEphSession()
{
	m_MsgBuflen = 0;
	if (m_pMsgBuf != NULL)
	{
		delete[] m_pMsgBuf;
	}


	g_ImsgSion--; 
}

// �������ݰ����Ự
int ImsgEphSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if ((packh.m_type == ProxyTCP) || (packh.m_type == ProxyUDP))
	{
		m_ProxyType = 1;
		packh.m_type -= 6;
	}

	// �����E��ͨ�İ��ͱ���
	if(isEPHImsgPacket(packh, pPacket))
	{
		// �����ʱ���������ֻ��TCPPacket �� UDPPacket��
		DTcpPacket ephpack(packh, (CHAR *)pPacket);
		m_PackCount++;
		m_bSortList = FALSE;
		m_PacketList.push_back(ephpack);
		ephpack.buf = NULL;


	}
	else
	{
		return 0;
	}


 	if ((packh.m_tm - m_ProcessTime) >= 40)
	{
		return ProcessEPHList(packh.m_tm);
	}

	return 0;
}

BOOL ImsgEphSession::CloseSession(time_t nowtm)
{
	ProcessEPHList(nowtm);
	m_lastpacktm = m_ProcessTime;

// ����ļ�
	if (m_MsgBuflen > 0)
	{
		// дʵ���ļ�
		WriteRealFile(g_pMsgBuffer, 0);
	}
	if (m_realfname.IsEmpty())
	{
		// û��ʵ���ļ����ǲ��Ϸ�����
		return TRUE;
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
	m_OpenCaseFlg = v_opencaseEht;
	m_OpenWarnFlg = v_openwarnEht;
	m_OpenIndxFlg = v_openflagEht;
	DWORD state = 0;
	SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), state);
	SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);

	// ƥ������
//		CheckLanguage(g_pMsgBuffer, MSGBUFFERLEN);

	// д�����ļ�
	CString str;
	m_indexfname = m_realfname + ".indexfile.txt";
	str.Format("user= %s\r\n", m_userid);
	WriteIndexFile("imsg_doshow", "imsg_doshow", str, g_pImsgMissObj);
	m_CaseArr.RemoveAll();
	m_WarnArr.RemoveAll();
	m_begfiletm = 0;
	m_closetm += 180;
	if(abs(nowtm - m_lastpacktm) > 300)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//*************************************************
//	��������:		EphSession::WriteEphmsgFile
//	����:			��ԭһ��������дʵ���ļ�
//	�������:		Packet ��һ����
//					msgbuf �Ǵ�Ż�ԭ����Ϣ�Ļ�����
//					int �ǻ������Ĵ�С
//	�������:		
//	����ֵ:			���ػ�ԭ����Ϣ��С
//	����:			
//**************************************************
int ImsgEphSession::ProcessEphPacket(DTcpPacket* Packet, char* msgbuf, int len)
{
					// ��ʽ����Ϣ�ı�
	char strtm[40];
	memset(strtm, 0, sizeof(strtm));
	int pos = 0;
 	BYTE* pData = (BYTE*)Packet->buf;
	DWORD intuserid = 0;		// UserID
	DWORD intcontactid = 0;		// ��ϵ��ID
	int direction = 0;
	CString Msg;				// ��Ϣ
	if (Packet->packettype == UDPPacket)
	{
		intuserid = Swap4Byte(*(DWORD*)(pData + 0x6));
		if (intuserid == 0)
		{
			return 0;
		}

		if ((memcmp(pData, "\x72\0\0", 3) == 0)
			&& (pData[0xA] == 0) 
			&& ((*(USHORT*)(pData + 0x0e) == 0x8580) || (*(USHORT*)(pData+0x0e) == 0x8500))
			&& (*(USHORT*)(pData + 0x12) == 0))
		{
			char* pp = strchr((char*)pData + 0x1a, 0xFF);
			if(pp == NULL)
			{
				return 0;
			}
			*pp = 0;
			if (memcmp(pData+0xe,"\x80\x85",2)==0 
				&& *(UCHAR *)(pData+0x19)<0xff 
				&& *(UCHAR *)(pData+0x19)>0 
				)
			{
				//server-client
				//����ϵ��
				intcontactid=ntohl(*(UINT*)(pData+0x14));
				if (intcontactid==0) {
					return FALSE;
				}
				direction = 2;
				*(pData+*(UCHAR *)(pData+0x19)+0x1a)=0;
				m_userid.Format("%u", intuserid);
				m_Colloquist.Format("%u", intcontactid);
				Msg = pData+0x1A;
			}
			else if (memcmp(pData+0xe,"\x0\x85",2)==0
				&& (pData[0x19] < 0xff)
				&& (pData[0x19] > 0))
			{
				// Client --> Server
				direction = 1;
				if(m_userid.IsEmpty())
				{
					m_userid.Format("%u", intuserid);
				}
				Msg = pData + 0x1A;
			}
			else
			{
				return 0;
			}
		}
	
	} // if (Packet->packettype == UDPPacket)

	// �����Ự����
	if ((m_direction == 0) && (direction != 0))
	{
		if (memcmp(Packet->sip, m_csip, 4) == 0)
		{
			m_direction = direction;
		}
		else
		{
			m_direction = (direction == 1)? 2 : 1;
		}
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
		ip = m_processseq1;
		m_processseq1 = m_processseq2;
		m_processseq2 = ip;
	}

	// ��ʽ����Ϣ
	if (!Msg.IsEmpty())
	{
		if (m_begfiletm == 0)
		{
			m_begfiletm = Packet->tm;
		}

		int msglen = GetTimeString(Packet->tm, "DoShow    %Y-%m-%d %H:%M:%S    ",
				msgbuf, 80);

		CString tsender;
		if (direction == 1)
		{
			msglen += sprintf(msgbuf + msglen,
				"%s �� %s    %s �� %s :\r\n%s\r\n\r\n",
				m_sip, m_dip, m_userid, m_Colloquist, Msg);
		}
		else
		{
			msglen += sprintf(msgbuf + msglen,
				"%s �� %s    %s �� %s :\r\n%s\r\n\r\n",
				m_sip, m_dip, m_userid, m_Colloquist, Msg);
		}
		return msglen;
	}
	else
	{
		return 0;
	}
}


// дʵ���ļ�
void ImsgEphSession::WriteRealFile(char* pData, int datalen)
{
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateFilename(m_sip, m_dip, "imsg_doshow.txt");
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


int ImsgEphSession::ProcessEPHList(time_t now)
{
	if(!m_bSortList)
	{
		m_PacketList.sort();
		m_bSortList = TRUE;
	}

	g_Msglen = 0;
	DTcpPacketListIt item, begin,end;
	while(m_PacketList.size()>0)
	{
		begin = m_PacketList.begin();
		item = begin;
		end = m_PacketList.end();
		item++;
#ifdef		_DEBUG
		DTcpPacket	*p1, *p2;
		p1 = &(*item);
		p2 = &(*begin);
#endif
		m_ProcessTime = begin->tm;
		if ((begin->packettype == UDPPacket) || (begin->packettype == ProxyUDP))
		{
			g_Msglen += ProcessEphPacket(&(*begin),
				g_pMsgBuffer + g_Msglen, MSGBUFFERLEN - g_Msglen);
		}
		else if(begin->packettype == TCPPacket || begin->packettype == ProxyTCP)
		{
			if(memcmp(begin->sip, m_csip,4)==0)
			{
				if ((begin->Nextsequence - m_processseq1) < 0x80000000)
				{
					g_Msglen += ProcessEphPacket(&(*begin),
						g_pMsgBuffer + g_Msglen, MSGBUFFERLEN - g_Msglen);
				}
				else
				{
					m_PacketList.erase(begin);
					m_PackCount--;
					continue;
				}
				m_processseq1 = begin->Nextsequence;
			}
			else
			{
				if((begin->Nextsequence - m_processseq2) < 0x80000000)
				{
					g_Msglen += ProcessEphPacket(&(*begin),
						g_pMsgBuffer + g_Msglen, MSGBUFFERLEN - g_Msglen);
				}
				else
				{
					m_PacketList.erase(begin);
					m_PackCount--;
					continue;
				}			
				m_processseq2 = begin->Nextsequence;					
			}
			//д�ļ�
		}
		m_PacketList.erase(begin);
		m_PackCount--;

		if (g_Msglen > (MSGBUFFERLEN - 4096))
		{
			// д�ļ�
			memset(g_pMsgBuffer + g_Msglen, 0, 4);
			WriteRealFile(g_pMsgBuffer, g_Msglen);
			g_Msglen = 0;
		}

	}

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
	return 0;
}