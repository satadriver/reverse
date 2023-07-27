// mysql.cpp: implementation of the mysql class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "mysql.h"
#include "imsg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MysqlSession::MysqlSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 120;

}

MysqlSession::~MysqlSession()
{

}

int MysqlSession::InsertPacket(DATEHEADER& packh, const char *pPacket)
{

	if(IsMySqlPacket(packh,pPacket))
	{
		 m_userid.Format("%s",pPacket+0x24);

		 if(!m_userid.IsEmpty())
		 {
			WriteMySqlIndexFile();	 
		 }
	}
	return 0;
}


void MysqlSession::WriteMySqlIndexFile()
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
			WriteHisData("mysql", "mysql", &hisRec, g_pImsgMissObj,	IMSG_PROTOCOL);//, HISTORY_DATA);
		}
	}
	
	
	
	// 匹配案件、预警和屏蔽信息
	DWORD state = 0;
	
	CString text;
	text.Format("%s\r\n", m_userid);
	// 	m_OpenCaseFlg = v_opencaseVp;
	// 	m_OpenWarnFlg = v_openwarnVp;
	//m_OpenIndxFlg = v_openflagdayingjia;
	
	SearchCaseOrWarn(this, text, text.GetLength(), state,"mysql");
	SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);
	
	
	m_indexfname = CreateFilename(m_sip, m_dip, "mysql.indexfile.txt");
	
	
	str.Format("user=%s\r\nstat= 上线\r\n", m_userid);
	WriteIndexFile("mysql", "mysql", str, g_pImsgMissObj);
	m_CaseArr.RemoveAll();
	m_WarnArr.RemoveAll();
	m_begfiletm = 0;
	m_closetm += 10;
}