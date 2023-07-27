//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	QQGame
//	�汾:		1.0
//	��������:	2008-06-03
//	����:		ccj
//	��Ҫ����:	QQ��Ϸ�Ļ�ԭ
//	ģ������:	
//	��Ҫ����:	
//
//**************************************************************** 

#include "stdafx.h"
#include "imsg.h"
#include "QQGame.h"


QQGameSession::QQGameSession(DATEHEADER& packh, int aT)
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

QQGameSession::~QQGameSession()
{
	m_MsgBuflen = 0;
	if (m_pMsgBuf != NULL)
	{
		delete[] m_pMsgBuf;
	}

	g_ImsgSion--;
}

BOOL isQQGame(DATEHEADER& packh, const char* pPacket)
{
	if (packh.m_dport==443)
	{
		if (packh.m_datalen < 13)
		{
			return FALSE;
		}

		if (memcmp(pPacket+1,"\x40\x02\x0b\x00\x00\x82\x00\x03",8)==0||
			memcmp(pPacket+1,"\x40\x02\x0b\x00\x00\x82\x00\x01",8)==0)
		{
			return TRUE;
		}

		if (memcmp(pPacket,"\x4d\x20\x66\x65\x74\x69\x6f\x6e\x2e\x63\x6f\x6d\x2e",13)==0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL QQGameSession::CloseSession(time_t nowtm)
{
	try
	{
		if (m_begfiletm == 0 )
		{
			// �Դ��ϴδ��ļ�����һֱû�а���������ٸûỰ
			return TRUE;
		}
		
// 		// ����ļ�
// 		if (m_MsgBuflen > 0)
// 		{
// 			// дʵ���ļ�
// 			WriteRealFile(g_pMsgBuffer, 0);
// 		}
// 		if (m_realfname.IsEmpty())
// 		{
// 			// û��ʵ���ļ����ǲ��Ϸ�����
// 			return TRUE;
// 		}

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
			ttstr.Format("%s\1%s\1��Ϸ\1imsg_game_qq\1%s\1QQ��Ϸ\1%s\1\1",
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
		m_OpenCaseFlg = v_opencaseqqgame;
		m_OpenWarnFlg = v_openwarnqqgame;
		m_OpenIndxFlg = v_openflagqqgame;
		DWORD state = 0;
		SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), state,"imsg_game_qq");
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

			WriteHisData("imsg_game_qq", "imsg_game_qq", &hisRec, g_pImsgMissObj,IMSG_PROTOCOL);//, HISTORY_DATA);
		}


		// д�����ļ�
		if (m_realfname.IsEmpty())
		{
			m_indexfname = CreateFilename(m_sip, m_dip, "imsg_game_qq.txt.indexfile.txt");
		}
		else
		{
			m_indexfname = m_realfname + ".indexfile.txt";
		}

		str.Format("user= %s\r\nstat=��¼\r\n", m_userid);
		WriteIndexFile("imsg_game_qq", "imsg_game_qq", str, g_pImsgMissObj);
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

int QQGameSession::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	try
	{
		if (packh.m_dport==443)
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

		const char *tp = pPacket;
		char tmpchar[10];

		memset(tmpchar, 0, 10);
		memcpy(tmpchar,pPacket+9,4);

		DWORD dwQQ = Swap4Byte(*((DWORD*)(tmpchar)));

		if(dwQQ>10000)	///< ����QQ����Ϸ���
		{
			m_begfiletm = packh.m_tm;
			m_userid.Format("%u",dwQQ);
		}
		else
			return FALSE;	
 	}
	catch(...)
	{
		WriteLog(IMSGPRO, "QQGameSession::InsertPacket() Error!!");
		return -3;
	}
	return 0;
}
