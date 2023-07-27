//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	QQGame
//	�汾:		1.0
//	��������:	2008-06-03
//	����:		ccj
//	��Ҫ����:	������Ϸ�Ļ�ԭ
//	ģ������:	
//	��Ҫ����:	
//
//**************************************************************** 

#include "stdafx.h"
#include "imsg.h"
#include "PerfectGame.h"


PerfectGameSession::PerfectGameSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_direction = 0;
	m_closetm = packh.m_tm + 30;
	m_bIdc = FALSE;
	m_MsgBuflen = 0;
	m_pMsgBuf = NULL;
	m_begfiletm = 0;
	
	g_ImsgSion++;
}

PerfectGameSession::~PerfectGameSession()
{
	m_MsgBuflen = 0;
	if (m_pMsgBuf != NULL)
	{
		delete[] m_pMsgBuf;
	}

	g_ImsgSion--;
}

BOOL isPerfectGame(DATEHEADER& packh, const char* pPacket)
{

//	return FALSE;

	if (packh.m_dport==29000)
	{
		if (memcmp(pPacket,"\x03",1)==0 || memcmp(pPacket,"\x02",1) ==0)
		{
			return TRUE;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

BOOL PerfectGameSession::CloseSession(time_t nowtm)
{
	try
	{
		if (m_begfiletm == 0 )
		{
			// �Դ��ϴδ��ļ�����һֱû�а���������ٸûỰ
			return TRUE;
		}

		// ���ﲻ����IDCServer �� ���ʷ
		// ��������Դ

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
			ttstr.Format("%s\1%s\1��Ϸ\1imsg_game_perfect\1%s\1������Ϸ\1%s\1\1",
				m_dip.Left(15), m_strISP, m_dip.Right(5), TYPESTATE_DECIDED);
			v_IdcServerStat[IMSG_PROTOCOL].AddServer(ttstr);
		}
		else if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), str))
		{
			if (m_dataSrc == DATASRC_OTHER)
			{
				m_dataSrc = DATASRC_IDC;
				m_strISP = str;
			}
		}

		// ƥ�䰸����Ԥ��
//		m_OpenCaseFlg = v_opencaseqqgame;
//		m_OpenWarnFlg = v_openwarnqqgame;
		m_OpenIndxFlg = v_openflaggameperfect;
		DWORD state = 0;
		SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), state,"imsg_game_perfect");
		SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);
	
		// ƥ������
//		CheckLanguage(g_pMsgBuffer, MSGBUFFERLEN);

		// ������ʷ
		if (v_statImsgOther)
		{
			HistoryRecord hisRec;
			hisRec.SetRecord(IDX_FORWTYPE,	"1");
			hisRec.SetRecord(IDX_ID,		m_userid);
			hisRec.SetRecord(IDX_USER,		m_userid);

			WriteHisData("imsg_game_perfect", "imsg_game_perfect", &hisRec, g_pImsgMissObj,	IMSG_PROTOCOL);//, HISTORY_DATA);
		}


		// д�����ļ�
		if (m_realfname.IsEmpty())
		{
			m_indexfname = CreateFilename(m_sip, m_dip, "imsg_game_perfect.txt.indexfile.txt");
		}
		else
		{
			m_indexfname = m_realfname + ".indexfile.txt";
		}

		str.Format("user= %s\r\nstat=��¼\r\n", m_userid);
		WriteIndexFile("imsg_game_perfect", "imsg_game_perfect", str, g_pImsgMissObj);
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

int PerfectGameSession::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	int error = 0;
	try
	{
		if (packh.m_dport==29000)
		{
			if (packh.m_datalen < 13)
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
error =1;
		char tmpuserid[50];

		memset(tmpuserid,0,50);

		unsigned int len = pPacket[2];

//		ASSERT(len>=0);
		if (len < 50 && 
			packh.m_datalen-3 > len && 
			len>0)
		{
error =2;			
			memcpy(tmpuserid,pPacket+3,len);

			m_userid.Format("%s",tmpuserid);

			/// >���û�ID���жϣ�������6-16֮�䣬����������
			if (m_userid.GetLength()>=6 &&
				m_userid.GetLength()<=16 &&
				CheckIfExcludeChinese(tmpuserid,len))
			{
				m_begfiletm = packh.m_tm;
			}
			
error =3;
		}
 	}
	catch(...)
	{
		WriteLog(IMSGPRO, "PerfectGameSession::InsertPacket() Error!!");
		return -3;
	}
	return 0;
}