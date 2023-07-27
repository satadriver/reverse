//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	MSN
//	�汾:		1.0
//	��������:	2007-7-16
//	����:		��ǿ
//	��Ҫ����:	MSN����Э�黹ԭ
//	ģ������:	
//	��Ҫ����:	
//
//**************************************************************** 
#include "stdafx.h"
#include "imsg.h"
#include "MSN.h"


MsnSession::MsnSession(DATEHEADER& packh, int aT)
:SocketSession(packh, aT)
{
	m_direction = 0;
	m_bSort1 = m_bSort2 = TRUE;
	m_ProcessTime = packh.m_tm;
	m_begfiletm = packh.m_tm;
	m_closetm = packh.m_tm + 180;
	m_processseq1 = 0;
	m_processseq2 = 0;
	m_pMsgBuf = NULL;
	m_MsgBufLen = 0;
	m_bRecord = FALSE;
	m_language = CP_ACP;	// (����ȱʡ�Ĵ���ҳ"��������")

	if(packh.m_sequence > MaxSeq)
	{
		m_SeqOffset = SeqOffset;
	}

	if (packh.m_ack > MaxSeq)
	{
		m_AckOffset = SeqOffset;
	}

	InterlockedIncrement((long*)&g_MsnSion);	// g_MsnSion++
}

MsnSession::MsnSession()
{
	m_SeqOffset = m_AckOffset = 0;
	m_direction = 0;
	m_bSort1 = m_bSort2 = TRUE;
	m_ProcessTime = 0;
	m_begfiletm = m_endfiletm = 0;
	m_pMsgBuf = NULL;
	m_MsgBufLen = 0;
	m_bRecord = FALSE;
	m_language = CP_ACP;	// (����ȱʡ�Ĵ���ҳ"��������")

	InterlockedIncrement((long*)&g_MsnSion);	// g_MsnSion++
}

MsnSession::~MsnSession()
{
	if (m_pMsgBuf != NULL)
	{
		delete[] m_pMsgBuf;
		m_pMsgBuf = NULL;
	}

	InterlockedDecrement((long*)&g_MsnSion);	// g_MsnSion--
}

BOOL MsnSession::CloseSession(time_t nowtm)
{
	try
	{
		if ((m_packetlist1.size() > 0) || (m_packetlist2.size() > 0))
		{
			ProcessMsnPacket(nowtm);
		}
		if (m_MsgBufLen > 0)
		{
			WriteRealFile(g_pMsgBuffer, 0);
		}
		m_OpenCaseFlg = v_opencasemsn;
		m_OpenWarnFlg = v_openwarnmsn;
		m_OpenIndxFlg = v_openflagmsn;

		if (!m_realfname.IsEmpty())
		{
			// ƥ�䰸����Ԥ����������Ϣ
			DWORD dwstate = 0;
			SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), dwstate);
			SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);

			// ƥ������
			CheckLanguage(g_pMsgBuffer, MSGBUFFERLEN);

			// ����ļ�
			CString text;
			text.Format("user= %s\r\nfrom= %s\r\nto= %s\r\n", m_userid,m_userid,m_to);
			m_indexfname = m_realfname + ".indexfile.txt";
			WriteIndexFile("imsg_msn", "imsg_msn", text, g_pImsgMissObj);
			ASSERT(m_ProcessTime != 0);
			m_begfiletm = m_ProcessTime;
		}

		if ((nowtm - m_lastpacktm) > 360)
		{
			ASSERT(m_packetlist1.size() == 0);
			ASSERT(m_packetlist2.size() == 0);
			return TRUE;		// �����Ự
		}
		else
		{
			m_closetm += 180;
			return FALSE;		// �����Ự
		}
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "CloseMsnSession() Error!!");
		return TRUE;
	}
}

int MsnSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	BakProData(PRO_MSN, packh, pPacket);

	int error=0;
	try
	{
		if (packh.m_type == ProxyTCP)
		{
			m_ProxyType = 1;
		}
#ifdef		_DEBUG
		int	direction;
		if (memcmp(packh.m_sip, m_csip, 4) == 0)
		{
			direction = 1;
		}
		else
		{
			direction = 2;
		}
#endif
// 		if(packh.m_datalen > 0x20 
// 			&& (memcmp(pPacket+0x10, "INVITE MSNMSGR", 14) == 0 
// 			||  memcmp(pPacket+0x20, "INVITE MSNMSGR", 14) == 0) )
// 		{
// 		// ��Ҫ��Ự����,�ſ���ȷ���˻Ự�����û�, ����ȷ�����ͷ��ǿͻ��˻��Ƿ�����
// 		// �������, ��δ�ҵ���ȷ�ķ���, �˴��ѷ��ͷ�д��ΪԴ, ���շ�ΪĿ��
// 			CString strfrom, strto, strtfile;
// 			if(ProcessMsnTransfile(pPacket+0x20, packh.m_datalen-0x20, strfrom, strto, strtfile))
// 			{
// 				int msglen = 0;
// 				char buf[1024];
// 				msglen += sprintf(buf + msglen, "MSN: ");
// 				msglen += GetTimeString(packh.m_tm, "%Y-%m-%d %H:%M:%S",
// 					buf + msglen, 40);
// 				msglen += sprintf(buf + msglen, "    %s �� %s    %s �� %s:\r\n�����ļ� %s\r\n\r\n",
// 					m_sip, m_dip, strfrom, strto, strtfile);
// 				WriteRealFile(buf, msglen);
// 			}
// 			return 0;
// 		}


error=1;
		m_lastpacktm = packh.m_tm;
		if (m_direction == 2)
		{
			// ���������ip��mac����֤ �ͷ���洢
			if (memcmp(packh.m_sip, m_csip, 4) == 0)
			{
error=2;
				TcpPacket pack(packh, pPacket, m_AckOffset, m_SeqOffset);
				m_packetlist2.push_back(pack);
				m_bSort2 = FALSE;
				pack.buf = NULL;
				ASSERT(direction == 1);
			}
			else
			{
error=3;
				TcpPacket pack(packh, pPacket, m_SeqOffset, m_AckOffset);
				m_packetlist1.push_back(pack);
				m_bSort1 = FALSE;
				pack.buf = NULL;
				ASSERT(direction == 2);
			}
		}
		else
		{
			if (memcmp(packh.m_sip, m_csip, 4) == 0)
			{
error=4;
				TcpPacket pack(packh, pPacket, m_SeqOffset, m_AckOffset);
				m_packetlist1.push_back(pack);
				m_bSort1 = FALSE;
				pack.buf = NULL;
				ASSERT(direction == 1);
			}
			else
			{
error=5;
				TcpPacket pack(packh, pPacket, m_AckOffset, m_SeqOffset);
				m_packetlist2.push_back(pack);
				m_bSort2 = FALSE;
				pack.buf = NULL;
				ASSERT(direction == 2);
			}
		}
error=5;
 		if ((abs(packh.m_tm - m_ProcessTime) >= 30))
		{
			return ProcessMsnPacket(packh.m_tm);
		}
		return 0;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "InsertMsnPacket() Error!!=%d",error);
		return -3;
	}
}

int MsnSession::ProcessMsnPacket(time_t packtm)
{
#ifdef ADD_RUNSTATE_INFO
	v_imsgrunstate.AddState(10);
#endif
	int packcount = 0;
	g_Msglen = 0;
	if (!m_bSort1)
	{
		m_packetlist1.sort();
		m_bSort1 = TRUE;
	}
	if (!m_bSort2)
	{
		m_packetlist2.sort();
		m_bSort2 = TRUE;
	}

	TcpPacketListIt nowitem, item1, item2, tmpitem;
	int curpack = 0;
	BOOL pack1=FALSE, pack2=FALSE;
	TcpPacket	*p1, *p2;
	IMSGPARA msnpara;
	msnpara.m_bIdcSvr = FALSE;
	msnpara.m_msgbuf = g_pMsgBuffer;
	msnpara.m_msgbufsize = 128*1024;
	msnpara.m_msglen = 0;
	msnpara.m_packetbuf = g_pMsgBuffer + 160*1024;
	msnpara.m_packetbufsize = MSGBUFFERLEN - 160*1024;
	msnpara.m_packetlen = 0;
	do
	{
		item1 = m_packetlist1.begin();
		item2 = m_packetlist2.begin();
		if ((m_processseq1 == 0) && (m_packetlist1.size() > 0))
		{
			m_processseq1 = item1->Ssequence;
		}
		if ((m_processseq2 == 0) && (m_packetlist2.size() > 0))
		{
			m_processseq2 = item2->Ssequence;
		}
		while (!pack1 && m_packetlist1.size()>0)
		{
			p1 = &(*item1);
			if (p1->Nextsequence <= m_processseq1)
			{
				tmpitem = item1;
				item1++;
				m_packetlist1.erase(tmpitem);
			}
			else
			{
				pack1 = TRUE;
				break;
			}
		}
		while (!pack2 && m_packetlist2.size()>0)
		{
			p2 = &(*item2);
			if (p2->Nextsequence <= m_processseq2)
			{
				tmpitem = item2;
				item2++;
				m_packetlist2.erase(tmpitem);
			}
			else
			{
				pack2 = TRUE;
				break;
			}
		}
		if (pack1 && pack2)
		{
			if ((p1->Nextsequence == p2->Sack)
				|| ((p1->Nextsequence - p2->Sack) >= 0x80000000))
			// if (item1->Nextsequence <= item2->Sack)
			{
				// ��������1
				curpack = 1;
#ifdef ADD_RUNSTATE_INFO
				v_imsgrunstate.AddState(11);
#endif
			}
			else
			{
				// ��������1
				curpack = 2;
#ifdef ADD_RUNSTATE_INFO
				v_imsgrunstate.AddState(12);
#endif
			}
		}
		else if (pack1)
		{
			// ֻ������1������
#ifdef ADD_RUNSTATE_INFO
			v_imsgrunstate.AddState(13);
#endif
			curpack = 1;
		}
		else if (pack2)
		{
			// ֻ������2������
#ifdef ADD_RUNSTATE_INFO
			v_imsgrunstate.AddState(14);
#endif
			curpack = 2;
		}
		else
		{
			// ��Ӧ�ñ�ִ��
			break;
		}
		if (abs(packtm - m_ProcessTime) <= 20)
		{
			break;
		}
		ASSERT((curpack == 1) || (curpack == 2));
		if (curpack == 1)
		{
#ifdef ADD_RUNSTATE_INFO
			v_imsgrunstate.AddState(15);
#endif
			msnpara.m_direction = 1;
			ProcessMsnList(m_packetlist1, m_processseq1, &msnpara);
// 			m_packcount = m_packetlist1.size() + m_packetlist2.size();
			pack1 = FALSE;
		}
		else
		{
#ifdef ADD_RUNSTATE_INFO
			v_imsgrunstate.AddState(16);
#endif
			msnpara.m_direction = 2;
			ProcessMsnList(m_packetlist2, m_processseq2, &msnpara);
// 			m_packcount = m_packetlist1.size() + m_packetlist2.size();
			pack2=FALSE;
		}
		packcount = m_packetlist1.size() + m_packetlist2.size();
	}while((packcount > 0) && (msnpara.m_msglen < 80*1024));
	
	// д���ʷ
// 	if (msnpara.m_hisstrarr.GetSize()>0)
// 	{
// 		WriteArrToData(IMSG_PROTOCOL, HISTORY_DATA, "", &msnpara.m_hisstrarr);
// 	}
// 	// д�������
// 	if (msnpara.m_webfigrarr.GetSize()>0)
// 	{
// 		WriteArrToData(IMSG_PROTOCOL, WEBFIG_DATA, "", &msnpara.m_webfigrarr);
// 	}
	if (msnpara.m_webidarr.GetSize()>0)
	{
		WriteArrToData(IMSG_PROTOCOL, WEBID_DATA, "", &msnpara.m_webidarr);
	}

	// дIDCServer
	if (msnpara.m_bIdcSvr)
	{
		// дIDCServer
		CString str;
		if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), str))
		{
			CString idcstr;
			idcstr.Format("%s\1%s\1yahoo\1��ʱ��Ϣ\1%s\1MSN\1%s\1\1", 
				m_dip.Left(15), str, m_dip.Right(5), TYPESTATE_DECIDED);
			v_IdcServerStat[IMSG_PROTOCOL].AddServer(idcstr);
		}
	}

	// д�ļ�
	if (msnpara.m_msglen > 0)
	{
		if(m_MsgBufLen + msnpara.m_msglen >= MSGBUFLEN)
		{
			WriteRealFile(msnpara.m_msgbuf, msnpara.m_msglen);
		}
		else
		{
			if (m_pMsgBuf == NULL)
			{
				m_pMsgBuf = new char[MSGBUFLEN];
			}
			if (MSGBUFLEN < m_MsgBufLen+msnpara.m_msglen+1)
			{
				WriteLog("dce","msn len error");
				return 0;
			}
			memcpy(m_pMsgBuf + m_MsgBufLen, msnpara.m_msgbuf, msnpara.m_msglen+1);
			m_MsgBufLen += msnpara.m_msglen;
		}
	}

	return 0;
}

// дʵ���ļ�
void MsnSession::WriteRealFile(LPCTSTR pData, int datalen)
{
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateFilename(m_sip, m_dip, "imsg_msn.txt");
		ASSERT(!m_realfname.IsEmpty());
	}
	FILE* fp = fopen(m_realfname, "ab");
	if (fp == NULL)
	{
		return;
	}
	if (m_MsgBufLen > 0)
	{
		fwrite(m_pMsgBuf, m_MsgBufLen, 1, fp);
		m_MsgBufLen = 0;
	}
	if (datalen > 0)
	{
		fwrite(pData, datalen, 1, fp);
	}
	fclose(fp);
}


int MsnSession::ProcessMsnList(TcpPacketList& packlist, DWORD& processseq, IMSGPARA* pPara)
{
#ifdef ADD_RUNSTATE_INFO
	v_imsgrunstate.AddState(20);
#endif

	TcpPacketListIt nextitem, item, end;
	item = packlist.begin();
	end = packlist.end();

	// ���Ȳ���item����������
#ifdef	_DEBUG
	memset(pPara->m_packetbuf, 0, pPara->m_packetbufsize);
#endif

	int l;
	if (item->Ssequence > processseq)
	{
		l = item->Slen;
	}
	else
	{
		l = item->Nextsequence - processseq;
	}

	if (pPara->m_packetbufsize > l)
	{
		if (item->Slen - l >= 0)
		{
			memcpy(pPara->m_packetbuf, item->buf + item->Slen - l, l);
			pPara->m_packetlen = l;
			pPara->m_packtm = item->tm;
			processseq = item->Nextsequence;
			nextitem = item;
			nextitem++;
			if (m_begfiletm == 0)
			{
				m_begfiletm = item->tm;
			}
			m_ProcessTime = item->tm;
		}
// 		else
// 		{
// 			WriteLog("error","feifa23");
// 
// 		}
	}
	else
	{
		WriteLog("dce","procelist error len");
	}
	if (item == end)
	{
		packlist.erase(item);
		return 0;
	}

	packlist.erase(item);

	// �����һ����
	CStringArray temstr;
	int pos = ProcessMsnData(this, pPara);

	if (pos < 0)
	{
		// ���������
		return -3;
	}

	if (pos == pPara->m_packetlen)
	{
		// һ��������������
		return 0;
	}

	// �����ݰ���Ҫ����һ�������ϴ���
	// ���û�к����İ��Ͷ���
	while (nextitem != end)
	{
		item = nextitem;
		nextitem++;
		TcpPacket	*pp = &(*item);
		if (pp->Nextsequence <= processseq)
		{
			// �ظ��İ�
			continue;
		}
		if (pp->Ssequence > processseq)
		{
			// ��������
			return -1;
		}
		
		if (pos > 0 && 
			(pPara->m_packetlen >= pos) && 
			(pPara->m_packetbufsize > pPara->m_packetlen))
		{
			pPara->m_packetlen -= pos;
			memmove(pPara->m_packetbuf, pPara->m_packetbuf + pos,
				pPara->m_packetlen);
		}
		if (pPara->m_packetbufsize > pPara->m_packetlen + pp->Nextsequence - processseq)
		{
			memcpy(pPara->m_packetbuf + pPara->m_packetlen,
				pp->buf + processseq - pp->Ssequence,
				pp->Nextsequence - processseq);
			pPara->m_packetlen += pp->Nextsequence - processseq;
			processseq = pp->Nextsequence;
			pos = ProcessMsnData(this, pPara);
			if (pos < 0) 
			{
				packlist.erase(item);
				return -2;
			}
			if (pos != pPara->m_packetlen)
			{
				// ��ʣ������ݷŵ�������(���ı�pp�е�ֵ)
				int dlen = pPara->m_packetlen - pos;
				pp->Ssequence = processseq - dlen;
				processseq = pp->Ssequence;	// ���������кŻع�
				if (dlen > pp->Slen)
				{
					delete[] pp->buf;
					pp->buf = new char[dlen];
				}
				memcpy(pp->buf, pPara->m_packetbuf + pos, dlen);
				pp->Slen = dlen;
			}
			else
			{
				packlist.erase(item);
			}
		}
		else
		{
			packlist.erase(item);
			WriteLog("error","weifa1");
		}
		return 0;
	}
	return 0;
}
