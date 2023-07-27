//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	YMsg
//	�汾:		1.0
//	��������:	2007-7-13
//	����:		��ǿ
//	��Ҫ����:	�Ż�����Э�黹ԭ
//	ģ������:	
//	��Ҫ����:	YmsgSession::ProcessYmsgPacket
//
//**************************************************************** 

#include "stdafx.h"
#include "imsg.h"
#include "Ymsg.h"

static Search s_fe("��");	// "\0xC0\0x80"	// ���ݱ߽��ַ�

BOOL FormatYmsg(YMSG *pYmsg, char* packetdata, int blocklen);
BOOL GetYahooMsg(YMSG *pYmsg, ImsgMsg* pImsg, LPCTSTR tsip, LPCTSTR tdip);
LPCTSTR GetStrByKey(LPCTSTR strkey, YMSG *ymsg, int keylen);
CString GetAllStrByKey(LPCTSTR strkey, YMSG* ymsg, int keylen, LPCTSTR fgchar);

YmsgSession::YmsgSession(DATEHEADER& packh, int aT):SocketSession(packh, aT)
{
	m_direction = 0;
	m_ProcessTime = packh.m_tm;
	m_bSortList = TRUE;
	m_processseq1 = 0;
	m_processseq2 = 0;
	m_closetm = packh.m_tm + 180;
	m_MsgBuf = NULL;
	m_MsgBufLen = 0;

	InterlockedIncrement((long*)&g_YmsgSion);
}

YmsgSession::YmsgSession()
{
	m_direction = 0;
	m_ProcessTime = 0;
	m_bSortList = TRUE;
	m_processseq1 = 0;
	m_processseq2 = 0;
	m_MsgBuf = NULL;
	m_MsgBufLen = 0;

	InterlockedIncrement((long*)&g_YmsgSion);
}

YmsgSession::~YmsgSession()
{

	InterlockedDecrement((long*)&g_YmsgSion);
}

int YmsgSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	try
	{
		if (packh.m_type==ProxyTCP)
		{
			m_ProxyType = 1;
		}

		DTcpPacket ourgamepack(packh, (CHAR *)pPacket);
		m_bSortList = FALSE;
		m_PacketList.push_back(ourgamepack);
		ourgamepack.buf = NULL;
		m_lastpacktm = packh.m_tm;
		if (abs(packh.m_tm - m_ProcessTime) >= 40)
		{
			// ���������еİ�
			ProcessYmsgList(packh.m_tm);
		}
		return 0;
	}
	catch (...)
	{
		WriteLog(IMSGPRO, "YmsgSession::InsertPacket() Error!!");
		return -3;
	}
}

BOOL YmsgSession::CloseSession(time_t nowtm)
{
	try
	{
		ProcessYmsgList(nowtm);
		if (m_MsgBufLen > 0)
		{
			WriteRealFile(g_pMsgBuffer, 0);
		}
		if (!m_realfname.IsEmpty())
		{
			// ƥ�䰸����Ԥ����������Ϣ
			DWORD dwstate = 0;
			// userid��ʵ���ļ�
			SearchCaseOrWarn(this, "", 0, dwstate);
			m_OpenCaseFlg = v_opencaseyahoo;
			m_OpenWarnFlg = v_openwarnyahoo;
			m_OpenIndxFlg = v_openflagyahoo;

			// ƥ������
			CheckLanguage(g_pMsgBuffer, MSGBUFFERLEN);

			// ����ļ�
			CString text;
			text.Format("user= %s\r\n", m_userid);
			m_indexfname = m_realfname + ".indexfile.txt";
			WriteIndexFile("imsg_ymsg", "imsg_ymsg", text, g_pImsgMissObj);
			m_CaseArr.RemoveAll();
			m_WarnArr.RemoveAll();
			ASSERT(m_ProcessTime != 0);
			m_begfiletm = m_ProcessTime;
		}

		if ((nowtm - m_lastpacktm) > 360)
		{
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
		WriteLog(IMSGPRO, "YmsgSession::CloseSession() Error!!");
		return TRUE;
	}
}

// дʵ���ļ�
void YmsgSession::WriteRealFile(char* pData, int datalen)
{
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateFilename(m_sip, m_dip, "imsg_ymsg.txt");
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
	if (datalen > 0)
	{
		fwrite(pData, datalen, 1, fp);
	}
	fclose(fp);
}

void YmsgSession::ProcessYmsgList(time_t now)
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
	IMSGPARA msgpara;
	msgpara.m_msgbuf = g_pMsgBuffer;
	msgpara.m_msgbufsize = MSGBUFFERLEN;
	msgpara.m_msglen = 0;
	msgpara.m_bIdcSvr = FALSE;
	while(item != end)
	{
		DTcpPacket* pPacket = &(*item);
		m_ProcessTime = pPacket->tm;
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
			msgpara.m_direction = direction;
			msgpara.m_packetbuf = pPacket->buf;
			msgpara.m_packetbufsize = pPacket->Slen;
			msgpara.m_packetlen = pPacket->Slen;
			msgpara.m_packtm = pPacket->tm;
			if (ProcessYmsgPacket(&msgpara))
			{
				msgpara.m_msgbuf += msgpara.m_msglen;
				msgpara.m_msgbufsize -= msgpara.m_msglen;
				msgpara.m_msglen = 0;
			}
		}
		DTcpPacketListIt tmpitem = item;
		item++;
		m_PacketList.erase(tmpitem);
		if (msgpara.m_msgbufsize < 4096)
		{
			break;
		}
	}
	
	g_Msglen = MSGBUFFERLEN - msgpara.m_msgbufsize;
	if (msgpara.m_webidarr.GetSize()>0)
	{
		WriteArrToData(IMSG_PROTOCOL, WEBID_DATA, CString(), &msgpara.m_webidarr);
	}
// 	if (msgpara.m_hisstrarr.GetSize()>0)
// 	{
// 		WriteArrToData(IMSG_PROTOCOL, HISTORY_DATA, CString(), &msgpara.m_hisstrarr);
// 	}
// 	if (msgpara.m_webfigrarr.GetSize()>0)
// 	{
// 		WriteArrToData(IMSG_PROTOCOL, WEBFIG_DATA, CString(), &msgpara.m_webfigrarr);
// 	}

	if (msgpara.m_bIdcSvr)
	{
		// дIDCServer
		CString str;
		if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), str))
		{
			CString idcstr;
			idcstr.Format("%s\1%s\1yahoo\1��ʱ��Ϣ\1%s\1�Ż�ͨ\1%s\1\1", 
				m_dip.Left(15), str, m_dip.Right(5), TYPESTATE_DECIDED);
			v_IdcServerStat[IMSG_PROTOCOL].AddServer(idcstr);
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
	return;
}

//*************************************************
//	��������:		YmsgSession::ProcessYmsgPacket
//	����:			����һ���Ż��ĵ����ݰ�
//	����:			pPara ����(���庬��μ��ṹ����)
//
//	����ֵ:			�����ݱ���ԭʱ����TRUE
//
//	����:			�˺���Ϊ���̵߳��ã�����ʹ��new
//					msgbuflen >= datalen + 200
//**************************************************
// BOOL YmsgSession::ProcessYmsgPacket(time_t optm, char* pdata, int datalen,
// 		int direction, char* pmsgbuf, int& msgbuflen, CStringArray& hisarr, BOOL& bWIdcSvr)
BOOL YmsgSession::ProcessYmsgPacket(IMSGPARA* pPara)
{
	ASSERT(pPara != NULL);
	ASSERT(pPara->m_msgbufsize > pPara->m_packetlen + 200);
	ASSERT(pPara->m_msglen == 0);
// 	int tmplen = pPara->m_msglen;
	int pos = pPara->m_msglen;
	// ��������
	try
	{
		while(pPara->m_packetlen > 20)
		{
			YMSG Ymsg;
			Ymsg.Stat = 0;
			YMSGHD *pymsghd = (YMSGHD*)(pPara->m_packetbuf);
			Ymsg.Pklen = Swap2Byte(pymsghd->Pklen);
			unsigned int blocklen = Ymsg.Pklen + 20;
			if (blocklen > pPara->m_packetlen)
			{
				return TRUE;
			}
			if (FormatYmsg(&Ymsg, pPara->m_packetbuf, blocklen))
			{
				ImsgMsg YmsgMsg;
				LPCTSTR serverip, clientip;
				
				if (!GetYahooMsg(&Ymsg, &YmsgMsg, clientip, serverip))
				{
					return 0;
				}
				// ȷ���ͻ���
				if (m_direction == 0)
				{
					m_direction = (YmsgMsg.Direction == pPara->m_direction)? 1 : 2;
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

					// ƥ��IDC����Դ
					if (m_dataSrc == DATASRC_OTHER)
					{
						if ((v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), m_strISP))
							|| (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), m_strISP)))
						{
							m_dataSrc = DATASRC_IDC;
						}
					}
				}
				if(Ymsg.Service == 0xdc)
				{
					WriteTransFileIndexFile(&YmsgMsg);
					return FALSE;
				}
				// ��ʽ����Ϣ
				CString strMsg;
				if (!YmsgMsg.Msg.IsEmpty())
				{
					pos += GetTimeString(pPara->m_packtm, "YMSG    %Y-%m-%d %H:%M:%S    ",
						pPara->m_msgbuf + pos, 80);
				}
				CString tsender;
				if (pPara->m_direction == 1)
				{
					if (m_userid.GetLength() < YmsgMsg.Sender.GetLength())
					{
						m_userid = YmsgMsg.Sender;
					}
					if (!YmsgMsg.Msg.IsEmpty())
					{
						pos += sprintf(pPara->m_msgbuf + pos,
							"%s �� %s    %s �� %s :\r\n%s\r\n\r\n",
							m_sip, m_dip, m_userid, YmsgMsg.Acceptant, YmsgMsg.Msg);
					}
					tsender = YmsgMsg.Acceptant;
				}
				else
				{
					if (m_userid.GetLength() < YmsgMsg.Acceptant.GetLength())
					{
						m_userid = YmsgMsg.Acceptant;
					}
					if (!YmsgMsg.Msg.IsEmpty())
					{
						pos += sprintf(pPara->m_msgbuf + pos,
							"%s �� %s    %s �� %s :\r\n%s\r\n\r\n",
							m_sip, m_dip, m_userid, YmsgMsg.Sender, YmsgMsg.Msg);
					}
					tsender = YmsgMsg.Sender;
				}
				
				if (v_statOther && (Ymsg.stait == 1 || Ymsg.stait == 2))
				{
					if (Ymsg.stait == 1)
					{
						// дIDCServer
						pPara->m_bIdcSvr = TRUE;
					}
					
					// д���ʷ
					CString hisstr,webfigstr;
					HistoryRecord hisRec;
					hisRec.SetRecord(IDX_ID,		m_userid);
					hisRec.SetRecord(IDX_USER,		m_userid);

					hisRec.SetRecord(IDX_FORWTYPE,	"1");
					if(Ymsg.stait == 1)
					{
						hisRec.SetRecord(IDX_STATE,	"����");
						hisstr = GetWebidStr("imsg", "imsg_ymsg", &hisRec, g_pImsgMissObj,
							pPara->m_packtm);
// 						hisstr = GetHisStr("imsg", "imsg_ymsg", &hisRec, g_pImsgMissObj,
// 							pPara->m_packtm);
// 						webfigstr = GetWebfigStr("imsg", "imsg_msn", &hisRec,
// 							g_pImsgMissObj, pPara->m_packtm);

					}
					else if (m_Linkman.Find(tsender + "\r\n") < 0)
					{
						m_Linkman += tsender + "\r\n";
						hisRec.SetRecord(IDX_CONTACT, tsender);
						hisRec.SetRecord(IDX_STATE,	"ͨѶ");
						hisstr = GetWebidStr("imsg", "imsg_ymsg", &hisRec, g_pImsgMissObj,
							pPara->m_packtm);
// 						hisstr = GetHisStr("imsg", "imsg_ymsg", &hisRec, g_pImsgMissObj,
// 							pPara->m_packtm);
// 						webfigstr = GetWebfigStr("imsg", "imsg_msn", &hisRec,
// 							g_pImsgMissObj, pPara->m_packtm);

					}
					if (!hisstr.IsEmpty())
					{
						pPara->m_webidarr.Add(hisstr);
					}
// 					if (!hisstr.IsEmpty())
// 					{
// 						pPara->m_hisstrarr.Add(hisstr);
// 					}
// 					if (!webfigstr.IsEmpty())
// 					{
// 						pPara->m_webfigrarr.Add(webfigstr);
// 					}
				}
			} // end if (FormatYmsg(&Ymsg, pPara->m_packetbuf, blocklen))

			pPara->m_packetbuf += blocklen;
			pPara->m_packetlen -= blocklen;
		} // end while(datalen>20)
	}
	catch(...)
	{
		WriteLog(IMSGPRO, "ProcessYmsgpacket() Error!!");
		pPara->m_msglen	= pos;
		pos = 0;
		return FALSE;
	}
	pPara->m_msglen	= pos;
	return TRUE;
}

BOOL FormatYmsg(YMSG *pYmsg, char* packetdata, int blocklen)
{
	YMSGHD *pymsghd = (YMSGHD*)packetdata;
	pYmsg->Service = Swap2Byte(pymsghd->Service);	// ���ɷ����

	switch(pYmsg->Service)
	{
		case YAHOO_SERVICE_MESSAGE:		// ��ʱ��Ϣ
		case YAHOO_SERVICE_LIST:		// ��ú����б�
		case 0xF1:						// ��ú����б��°汾��
		case YAHOO_SERVICE_LOGON:		// �û�����
		case YAHOO_SERVICE_ADDBUDDY:	// �����û���һ����
		case YAHOO_SERVICE_REMBUDDY:	// ɾ���û���һ����
		case YAHOO_SERVICE_CHATJOIN:
		case YAHOO_SERVICE_CHATPING:
		case YAHOO_SERVICE_COMMENT:
		case 0xdc:		// �շ��ļ�
			// ������Ϣ��Ҫ����
			break;
		default:
			// �������Ϣ����Ҫ
			return FALSE;
			break;
	}
		
	int i,j = 0;
	CString strContent;
	try
	{
		pYmsg->Stat = Swap4Byte(pymsghd->Stat);	// ����״̬
		pYmsg->Pklen = Swap2Byte(pymsghd->Pklen);
		char *p = packetdata + 20;
		int len = pYmsg->Pklen;
		int tlen = 0;
		ASSERT(len<=blocklen-20);
		if (blocklen<len+20)
			return FALSE;
		while(1)
		{
			tlen += strlen(p+tlen);
			if (tlen < len-1)
			{
				p[tlen] = ' ';
			}
			else
			{
				break;
			}
		}

		char *tp;
		i = s_fe.find(p, len);
		while (i > 0)
		{
			tp = p+i+2;
			len -= i+2;
			p[i]=0;
			pYmsg->Code[j] = p;
			
			if((i = s_fe.find(tp, len))<0)
				break;
			
			p = tp+i+2;
			len -= i+2;
			tp[i] = 0;
			pYmsg->strContent[j] = tp;
			while(*tp != 0)
			{
				if ((*tp == '<') || (*tp == '>'))
				{
					*tp = ' ';
				}
				tp++;
			}

			j++;
			if((i = s_fe.find(p, len))<0)
			{
				break;
			}
			if(j==100)	// �������һ����������־
			{
				break;
			}
		}
	}
	catch(...)
	{
		WriteLog(IMSGPRO, "FormatYmsg() Error!!");
	}	
	return TRUE;
}

BOOL GetYahooMsg(YMSG *pYmsg, ImsgMsg* pImsg, LPCTSTR tsip, LPCTSTR tdip)
{
	CString strInfo="";
	CString strAnsiInfo="";
	LPCTSTR strSid	= NULL;
	LPCTSTR strSid1 = NULL;
	LPCTSTR strDid	= NULL;
	int i=0;
int ierror = 0;
	try
	{
	pImsg->Direction=3;
	switch(pYmsg->Service)
	{
		//ֻ��һ���˵Ķ���
		case YAHOO_SERVICE_LOGON:			//Yahoo ID=1
ierror = 1;
			strSid = GetStrByKey("0", pYmsg, 1);
			ASSERT(strSid != NULL);
			if(*strSid != 0)
			{
				pImsg->Direction=2;
				////pImsg->SenderIP=tsip;//GetPacketSip();
				pImsg->Sender="������";
				pImsg->Acceptant=strSid;
				////pImsg->AcceptantIP=tdip;//GetPacketDip();
				pImsg->Msg.Format("%s ���ߡ�\r\n",strSid);
			}
			if (pYmsg->Stat==YAHOO_STATUS_BRB)
				strDid = GetStrByKey("7", pYmsg, 1);
			else
			{
				strSid1 = GetStrByKey("1", pYmsg, 1);
				ASSERT(strSid1 != NULL);
				
				if(*strSid1 != 0 && strcmp(strSid, strSid1) != 0)
					pImsg->Msg = pImsg->Msg + strSid1 + " ���ߡ�";
			}
			pYmsg->stait = 1;	// ����
			//sprintf(pYmsg->stait,"����");
			break;
		case YAHOO_SERVICE_NEWMAIL:
ierror = 2;
			strSid=GetStrByKey("43", pYmsg, 2);
			pImsg->Direction=2;
			pImsg->Sender="������";
			//pImsg->SenderIP=tsip;//GetPacketSip();
			pImsg->Acceptant=strSid;
			//pImsg->AcceptantIP=tdip;//GetPacketDip();
			strSid1 = GetStrByKey("42", pYmsg, 2);
ierror = 20;
			pImsg->Msg.Format("ϵͳ��Ϣ:\r\n(%s) ���յ�����(%s)��һ���ʼ�!  �ʼ�������:%s",
					strSid, GetStrByKey("42", pYmsg, 2),
					NetBytestoAnsi(GetStrByKey("18", pYmsg, 2)));
			break;
		//��ʱ��Ϣ
		case YAHOO_SERVICE_MESSAGE:
ierror = 3;
			switch(pYmsg->Stat)
			{
			case YAHOO_STATUS_AVAILABLE:
				strSid=GetStrByKey("0", pYmsg, 1);
				break;
			case YAHOO_STATUS_BRB:
				strSid=GetStrByKey("4", pYmsg, 1);
				break;
			default:
				strSid=GetStrByKey("1", pYmsg, 1);
			}
			strDid=GetStrByKey("5", pYmsg, 1);

			//pImsg->SenderIP=tsip;//GetPacketSip();
			//pImsg->AcceptantIP=tdip;//GetPacketDip();
			
			ASSERT(GetStrByKey("4", pYmsg, 1) != NULL);
			if(*(GetStrByKey("4", pYmsg, 1)) != 0)
			{
				pImsg->Direction=2;
				pImsg->Sender=strSid;
				pImsg->Acceptant=strDid;
				strSid = strDid ;
			}
			else
			{
				pImsg->Direction=1;
				pImsg->Sender=strSid;
				pImsg->Acceptant=strDid;
			}
			pImsg->Msg = NetBytestoAnsi(GetStrByKey("14", pYmsg, 2));
			pYmsg->stait = 2;		// ����
			break;
		case YAHOO_SERVICE_LIST:
ierror = 4;
			//==================================================================================
			//	��ú����б�
			//==================================================================================
//			strSid=GetStrByKey("3", pYmsg, 1);
//			
//			pImsg->Direction=2;
//			//pImsg->SenderIP=tsip;//GetPacketSip();
//			//pImsg->AcceptantIP=tdip;//GetPacketDip();
//			pImsg->Sender="������";
//			pImsg->Acceptant=strSid;
//			pImsg->Msg.Format("ϵͳ��Ϣ:(%s) ��ú����б�:\r\n%s",
//								strSid, GetStrByKey("87", pYmsg, 2));
			strSid=GetStrByKey("3", pYmsg, 1);
			if (strSid[0] != 0)
			{
				pImsg->Direction=2;
				//pImsg->SenderIP=tsip;//GetPacketSip();
				//pImsg->AcceptantIP=tdip;//GetPacketDip();
				pImsg->Sender="������";
				pImsg->Acceptant=strSid;
				LPCTSTR ulist = GetStrByKey("87", pYmsg, 2);
				if (ulist[0] != 0)
				{
					pImsg->Msg.Format("ϵͳ��Ϣ:(%s) ��ú����б�:\r\n%s",
						strSid, ulist);
					pImsg->Msg.TrimRight();
				}
			}
			else
			{
				strSid=GetStrByKey("1", pYmsg, 1);
				if (strSid[0] != 0)
				{
					pImsg->Direction=1;
					//pImsg->SenderIP=tdip;//GetPacketSip();
					//pImsg->AcceptantIP=tsip;//GetPacketDip();
					pImsg->Sender=strSid;
					pImsg->Acceptant="������";
					pImsg->Msg.Format("(%s) �����ȡ�����б�\r\n", strSid);
				}
			}
			break;
		case 0xF1:		// ��ú����б�(�°汾)
ierror = 4;
			strInfo=GetAllStrByKey("7", pYmsg, 1, "\r\n");
			if (!strInfo.IsEmpty())
			{
				pImsg->Direction=2;
				//pImsg->SenderIP=tsip;//GetPacketSip();
				//pImsg->AcceptantIP=tdip;//GetPacketDip();
				pImsg->Sender="������";
				pImsg->Msg.Format("ϵͳ��Ϣ: ��ú����б�:\r\n%s", strInfo);
				pImsg->Msg.TrimRight();
			}
			break;

		case YAHOO_SERVICE_AUTH:
ierror = 6;
			strSid=GetStrByKey("1", pYmsg, 1);

			//pImsg->SenderIP=tsip;//GetPacketSip();
			//pImsg->AcceptantIP=tdip;//GetPacketDip();

			if(pYmsg->Stat==0)
			{
				pImsg->Direction=1;
				pImsg->Sender=strSid;
				pImsg->Acceptant="������";
			}
			else if(pYmsg->Stat==1)
			{
				pImsg->Direction=2;
				pImsg->Sender="������";
				pImsg->Acceptant=strSid;
			}
			pImsg->Msg.Format("ϵͳ��Ϣ:(%s) ��ǰ��������",strSid);
//			sprintf(pYmsg->stait,"��������");
			break;
		case YAHOO_SERVICE_ADDBUDDY:
ierror = 7;
			strSid=GetStrByKey("1", pYmsg, 1);
			strDid=GetStrByKey("7", pYmsg, 1);

			if (pYmsg->Stat == 0)
			{
				pImsg->Direction=1;
				//pImsg->SenderIP=tsip;//GetPacketSip();
				//pImsg->AcceptantIP=tdip;//GetPacketDip();
				pImsg->Sender=strSid;
				pImsg->Acceptant="������";
				pImsg->Msg.Format("ϵͳ��Ϣ:(%s) ���� (%s) ��<%s>����",
					strSid, strDid, GetStrByKey("65", pYmsg, 2));
			}
			else
			{
				pImsg->Direction = 2;
				//pImsg->SenderIP=tdip;//GetPacketSip();
				//pImsg->AcceptantIP=tsip;//GetPacketDip();
				pImsg->Sender="������";
				pImsg->Acceptant=strSid;
				pImsg->Msg.Format("ϵͳ��Ϣ:(%s) ���� (%s) ��<%s>����",
					strSid, strDid, GetStrByKey("65", pYmsg, 2));
			}
			break;
		case YAHOO_SERVICE_REMBUDDY:
ierror = 8;
			strSid=GetStrByKey("1", pYmsg, 1);
			strDid=GetStrByKey("7", pYmsg, 1);

			if (pYmsg->Stat == 0)
			{
				pImsg->Direction=1;
				//pImsg->SenderIP=tsip;//GetPacketSip();
				//pImsg->AcceptantIP=tdip;//GetPacketDip();
				pImsg->Sender=strSid;
				pImsg->Acceptant="������";
				pImsg->Msg.Format("ϵͳ��Ϣ:(%s) ɾ�� (%s) ��<%s>����",
					strSid, strDid, GetStrByKey("65", pYmsg, 2));
			}
			else
			{
				pImsg->Direction = 2;
				//pImsg->SenderIP=tdip;//GetPacketSip();
				//pImsg->AcceptantIP=tsip;//GetPacketDip();
				pImsg->Sender="������";
				pImsg->Acceptant=strSid;
				pImsg->Msg.Format("ϵͳ��Ϣ:(%s) ɾ�� (%s) ��<%s>����",
					strSid, strDid, GetStrByKey("65", pYmsg, 2));
			}
			break;
		case YAHOO_SERVICE_CHATJOIN:
ierror = 9;
			strSid=GetStrByKey("1", pYmsg, 1);

			//pImsg->SenderIP=tsip;//GetPacketSip();
			//pImsg->AcceptantIP=tdip;//GetPacketDip();

			if(pYmsg->Stat==0)
			{
				pImsg->Direction=1;
				pImsg->Sender=strSid;
				pImsg->Acceptant="������";
				pImsg->Msg.Format("ϵͳ��Ϣ:(%s) ����������%s[%s]",
					strSid, NetBytestoAnsi(GetStrByKey("104", pYmsg, 3)),
					NetBytestoAnsi(GetStrByKey("105", pYmsg, 3)));
			}
			else
			{
				pImsg->Direction=2;
				pImsg->Sender="������";
				pImsg->Acceptant=strSid;

				if(*(GetStrByKey("128", pYmsg, 3)) != 0)
				{
					strAnsiInfo=NetBytestoAnsi(GetAllStrByKey("109", pYmsg, 3, ""));
					pImsg->Msg.Format("ϵͳ��Ϣ: ������%s[%s]��Ա�б�:%s",
						NetBytestoAnsi(GetStrByKey("104", pYmsg, 3)),
						NetBytestoAnsi(GetStrByKey("105", pYmsg, 3)),
						strAnsiInfo);
				}
				else
				{
					pImsg->Msg.Format("(%s) ����������%s[%s]",
						GetStrByKey("109", pYmsg, 3),
						NetBytestoAnsi(GetStrByKey("104", pYmsg, 3)),
						NetBytestoAnsi(GetStrByKey("105", pYmsg, 3)));
				}
			}

			break;
		case YAHOO_SERVICE_CHATPING:
ierror = 10;
			strSid=GetStrByKey("109", pYmsg, 3);
			break;
		case YAHOO_SERVICE_COMMENT:
ierror = 11;
			//pImsg->SenderIP=tsip;//GetPacketSip();
			//pImsg->AcceptantIP=tdip;//GetPacketDip();

			if(pYmsg->Stat==0)
			{
				strSid=GetStrByKey("1", pYmsg, 1);
				ASSERT(strSid!=NULL);
				if (strSid[0]==0)
					strSid = GetStrByKey("0", pYmsg, 1);
				pImsg->Direction=1;
				pImsg->Sender=strSid;
				pImsg->Acceptant="������";
				pImsg->Msg.Format("(%s)����������%s said:\r\n%s",
					strSid,NetBytestoAnsi(GetStrByKey("104", pYmsg, 3)),
					NetBytestoAnsi(GetStrByKey("117", pYmsg, 3)));
			}
			else if(pYmsg->Stat==1)
			{
				strSid=GetStrByKey("109", pYmsg, 3);

				pImsg->Direction=2;
				pImsg->Acceptant="�ͻ���";
				pImsg->Sender="������";
				pImsg->Msg.Format("(%s)����������%s said:\r\n%s",strSid,
					NetBytestoAnsi(GetStrByKey("104", pYmsg, 3)),
					NetBytestoAnsi(GetStrByKey("117", pYmsg, 3)));

				//==================================================================================
				//	strSid��һ���ǿͻ����û�
				//==================================================================================
				pImsg->Acceptant="";
				strSid = "";
			}
			break;
		case 0xdc:	// �շ��ļ�
			{
				BOOL bok = false;
				CString fname, from, to;
				strSid=GetStrByKey("27", pYmsg, 2);
				if(strSid[0] != 0)
				{
					int len = 0;
					for (; strSid[len] != 0 && len < 100 ; len++);
					if(FALSE == ConvertUtf8toGbk(strSid, len, fname))
						fname = strSid;
					strSid=GetStrByKey("5", pYmsg, 1);
					if(strSid[0] != 0)
					{
						to = strSid;
						strSid = GetStrByKey("1", pYmsg, 1);
						if(strSid[0] != 0)
						{
							pImsg->Direction = 1;	// �����ļ�
							from = strSid;
							bok = true;
						}
						else
						{
							strSid = GetStrByKey("4", pYmsg, 1);
							if(strSid[0] != 0)
							{
								pImsg->Direction = 2;	// S->C �����ļ�
								from = strSid;
								bok = true;
							}
						}
					}
				}
				if(bok)
				{
					pImsg->fname = fname;
					if(pImsg->Direction == 1)
					{
						//pImsg->SenderIP=tsip;//GetPacketSip();
						//pImsg->AcceptantIP=tdip;//GetPacketDip();
						pImsg->Acceptant=to;
						pImsg->Sender=from;
						pImsg->Msg.Format("%s ���͸� %s �ļ� %s\r\n", from, to, fname);
					}
					else
					{
						//pImsg->SenderIP=tdip;//GetPacketSip();
						//pImsg->AcceptantIP=tsip;//GetPacketDip();
						pImsg->Acceptant=to;
						pImsg->Sender=from;
						pImsg->Msg.Format("%s �������� %s ���ļ� %s\r\n", to, from, fname);
					}
				}
			}
			break;
		default:
			break;
	}
	}
	catch(...)
	{
		WriteLog(IMSGPRO, "GetYahooMsg() Error=%d Thread=%d", ierror, GetCurrentThreadId());
		return FALSE;
	}
	
//	return (!pImsg->Msg.IsEmpty());
	return (pImsg->Direction==1 || pImsg->Direction==2);
}

void YmsgSession::WriteTransFileIndexFile(ImsgMsg *pImsg)
{
	CString str;
	// ƥ�䰸����Ԥ����������Ϣ
	DWORD state = 0;
	
	CString text;
	text.Format("%s\r\n%s", pImsg->Acceptant, pImsg->Sender);
	
	SearchCaseOrWarn(this, text, text.GetLength(), state,"imsg_ymsg_tranfile");
	
	// ƥ�䰸����Ԥ��
	// 	m_OpenCaseFlg = v_opencaseVp;
	// 	m_OpenWarnFlg = v_openwarnVp;
	m_OpenIndxFlg = v_openflagqqtranfile;
	m_indexfname = CreateFilename(m_sip, m_dip, "imsg_ymsg_tranfile.indexfile.txt");
	
	if(pImsg->Msg.Find("����") != -1)
	{
		str.Format("user=%s\r\nfrom=%s\r\ncontact=%s\r\ntranfilename=%s\r\n",
			pImsg->Acceptant, pImsg->Sender, pImsg->Acceptant, pImsg->fname);
	}
	else
	{
		str.Format("user=%s\r\nfrom=%s\r\ncontact=%s\r\ntranfilename=%s\r\n",
			pImsg->Sender, pImsg->Sender, pImsg->Acceptant, pImsg->fname);
	}
	WriteIndexFile("imsg_ymsg_tranfile", "imsg_ymsg_tranfile", str, g_pImsgMissObj);
	m_CaseArr.RemoveAll();
	m_WarnArr.RemoveAll();
	m_begfiletm = 0;
	m_closetm += 10;
}

LPCTSTR GetStrByKey(LPCTSTR strkey, YMSG *ymsg, int keylen)
{
	int i=0;
	while (ymsg->Code[i] != NULL)
	{
		if (memcmp(ymsg->Code[i], strkey, keylen+1) == 0)	// ��memcmpЧ�ʱ�strcmp��
		{
			return (ymsg->strContent[i] == NULL) ? "" : ymsg->strContent[i];
		}
		i++;
	}
	return "";
}

CString GetAllStrByKey(LPCTSTR strkey, YMSG* ymsg, int keylen, LPCTSTR fgchar)
{
	ASSERT(fgchar!=NULL);
	int lfglen = strlen(fgchar);
	int i=0;
	CString strAll;
	// ����һ���㹻��Ļ���
	char buf[2000];
	memset(buf, 0, sizeof(buf));
	int len = 0;
	while (ymsg->Code[i] != NULL)
	{
		if ((ymsg->strContent[i] != 0)
			&& (memcmp(ymsg->Code[i], strkey, keylen+1) == 0))	// ��memcmpЧ�ʱ�strcmp��
		{
			int l = strlen(ymsg->strContent[i]);
			memcpy(buf+len, ymsg->strContent[i], l);
			len = len+l;
			memcpy(buf+len, fgchar, lfglen);
			len += lfglen;
		}
		i++;
	}
	ASSERT(len < sizeof(buf));
	buf[len] = 0;
	return buf;
}


// �Ż��Ĵ�����

//CString ProcessOneproxyBlock(IMSGPROXY& imsgproxy)
//{
//	// ��������
//	YMSG Ymsg;
//	if (FormatYmsg(&Ymsg, imsgproxy.pPacketData))
//	{
//		ImsgMsg YmsgMsg;
//		if (!GetYahooMsg(&Ymsg, &YmsgMsg, imsgproxy.sip, imsgproxy.dip))
//			return "";
//		
//		// ��ʽ����Ϣ
//		CString stm = CTime(imsgproxy.packettm).Format("YMSG\t%Y-%m-%d %H:%M:%S");
//		CString message;
//		ASSERT(!YmsgMsg.Msg.IsEmpty());
//		if(imsgproxy.Direction==1)
//		{
//			if (Ymsg.Service != YAHOO_SERVICE_LIST)
//			{
//				if (imsgproxy.userid.IsEmpty())
//					imsgproxy.userid = YmsgMsg.Sender;
//				message.Format("%s\t%s �� %s\t%s �� %s :\r\n%s\r\n\r\n",
//					stm, YmsgMsg.SenderIP, YmsgMsg.AcceptantIP,
//					imsgproxy.userid, YmsgMsg.Acceptant, YmsgMsg.Msg);
//			}
//		}
//		else if(imsgproxy.Direction==2)
//		{
//			if (imsgproxy.userid.IsEmpty())
//				imsgproxy.userid = YmsgMsg.Acceptant;
//			message.Format("%s\t%s �� %s\t%s �� %s :\r\n%s\r\n\r\n",
//				stm, YmsgMsg.AcceptantIP, YmsgMsg.SenderIP,
//				imsgproxy.userid, YmsgMsg.Sender, YmsgMsg.Msg);
//		}
//		return message;
//	}
//	return "";
//}

//CString Process_YahooProxy(IMSGPROXY& imsgproxy)
//{
//	CString strRes;
//	try
//	{
//		while (imsgproxy.datalen >= 20)
//		{
//			IMSG::YMSGHD *pymsghd = (IMSG::YMSGHD*)imsgproxy.pPacketData;
//			Swap2Byte(&pymsghd->Pklen);
//			unsigned int blocklen = pymsghd->Pklen + 20;
//			if (imsgproxy.datalen >= blocklen)
//			{
//				strRes += IMSG::ProcessOneproxyBlock(imsgproxy);
//				imsgproxy.datalen -= blocklen;
//				imsgproxy.pPacketData += blocklen;
//			}
//			else
//				break;
//		}
//	}
//	catch(...)
//	{
//		WriteLog(strImsg, "Process_YahooProxy() Error!!");
//		imsgproxy.userid="";
//		strRes.Empty();
//	}
//	return strRes;
