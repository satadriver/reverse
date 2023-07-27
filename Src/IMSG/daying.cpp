// QQTranFile.cpp: implementation of the QQTranFile class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "daying.h"
#include "imsg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



 

DayingSession::DayingSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 120;
	memset(m_firstsip,0,4);
	m_iswrite=FALSE;
}

DayingSession::~DayingSession()
{
	 
}

int DayingSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{
	if (packh.m_type ==TCPPacket)
	{
		//ssl  服务转
		if(packh.m_datalen==48
			&& memcmp(pPacket,"\x00\x01\x00\x01\x00\x00\x00\x30",8)==0
			&& memcmp(pPacket+0x1a,"\x00\x00",2)==0)
		{
			char buf[0x10];
			memcpy(buf,pPacket+0xc,0x10);
			

			 m_userid.Format("%s",buf);

 
				
			WriteDayingIndexFile();	 
			 

		}

	}
	

	return 0;
}
 
void DayingSession::WriteDayingIndexFile()
{
	CString str;
	// 写对照文件


	// 输出活动历史
		if (v_statImsgOther)
		{
			if(!m_userid.IsEmpty() )

			{
				HistoryRecord hisRec;
 				hisRec.SetRecord(IDX_ID,		m_userid);
	
				hisRec.SetRecord(IDX_STATE, "上线");
				WriteHisData("gambling_dayingjia", "gambling_dayingjia", &hisRec, g_pImsgMissObj,	IMSG_PROTOCOL);//, HISTORY_DATA);
			}
		}


 
	// 匹配案件、预警和屏蔽信息
	DWORD state = 0;

	CString text;
	text.Format("%s\r\n", m_userid);
// 	m_OpenCaseFlg = v_opencaseVp;
// 	m_OpenWarnFlg = v_openwarnVp;
	m_OpenIndxFlg = v_openflagdayingjia;

	SearchCaseOrWarn(this, text, text.GetLength(), state,"gambling_dayingjia");
	SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);


	m_indexfname = CreateFilename(m_sip, m_dip, "gambling_dayingjia.indexfile.txt");
	
	
	str.Format("user=%s\r\nstat= 上线\r\n", m_userid);
	WriteIndexFile("gambling_dayingjia", "gambling_dayingjia", str, g_pImsgMissObj);
	m_CaseArr.RemoveAll();
	m_WarnArr.RemoveAll();
	m_begfiletm = 0;
	m_closetm += 10;
}