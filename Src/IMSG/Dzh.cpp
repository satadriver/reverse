// Dzh.cpp: implementation of the CDzh class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "imsg.h"
#include "Dzh.h"
#include "../CheckHard/DogBM.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDzh60Session::CDzh60Session(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 120;
	m_balreadyout = FALSE;
}

CDzh60Session::~CDzh60Session()
{

}

int CDzh60Session::InsertPacket( DATEHEADER& packh, const char *pPacket )
{
	if(IsDzh60(packh,pPacket))
	{
		// L=125 \1 D=9 \1 O= V=1 1=3 2=1 3=gwi4364089964 R4=37
		char *ib = strstr(pPacket, "\1\x33\x3d");
		char *ie = strstr(pPacket, "\1\x52\x34");
		if(ie-ib > 10)
		{
			m_userid = CString(ib+3, ie-ib-3);
			
			if(FALSE == m_balreadyout) //|| bSqlQuery
			{
				m_begfiletm = packh.m_tm;
				m_OpenCaseFlg = v_opencasebf;
				m_OpenWarnFlg = v_openwarnbf;
				m_OpenIndxFlg = v_openflagbf;
				
				// 匹配案件、预警和屏蔽信息
				DWORD state = 0;
				SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), state);
				SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);

				// 写对照文件
				if ((m_CaseArr.GetSize() > 0) || (m_WarnArr.GetSize() > 0)
					|| (!m_rubbishflag && m_OpenIndxFlg))
				{
					SYSTEMTIME now;
					GetLocalTime(&now);
					CString tmppath = g_pImsgMissObj->GetTmpPath();
					ASSERT(!tmppath.IsEmpty());
					m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_1_%u_dzh.indexfile.txt",
						tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
						now.wMinute, now.wSecond, m_sip, m_dip, rand());
					CString text;
					text.Format("user= %s\r\nstat= 上线\r\n", m_userid);
					WriteIndexFile("dzh", "dzh", text, g_pImsgMissObj);
					m_CaseArr.RemoveAll();
					m_WarnArr.RemoveAll();
				}
				m_balreadyout = TRUE;
			}
		}
	}
	return 0;
}

CThsSession::CThsSession(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 120;
	m_balreadyout = FALSE;
}

CThsSession::~CThsSession()
{

}

int CThsSession::InsertPacket( DATEHEADER& packh, const char *pPacket )
{
	if(IsThsPacket(packh,pPacket))
	{
		RbSearch srhAccount("account=");
		RbSearch srhEnd("\xd\xa");
		int ib = srhAccount.find(pPacket, packh.m_datalen);
		if(ib > 0)
		{
			int ie = srhEnd.find(pPacket+ib, packh.m_datalen-ib);
			if(ie > 10)
			{
				m_userid = CString(pPacket+ib+srhAccount.m, ie-srhAccount.m);
			
				if(FALSE == m_balreadyout) //|| bSqlQuery
				{
					m_begfiletm = packh.m_tm;
					m_OpenCaseFlg = v_opencasebf;
					m_OpenWarnFlg = v_openwarnbf;
					m_OpenIndxFlg = v_openflagbf;
					
					// 匹配案件、预警和屏蔽信息
					DWORD state = 0;
					SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), state);
					SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);

					// 写对照文件
					if ((m_CaseArr.GetSize() > 0) || (m_WarnArr.GetSize() > 0)
						|| (!m_rubbishflag && m_OpenIndxFlg))
					{
						SYSTEMTIME now;
						GetLocalTime(&now);
						CString tmppath = g_pImsgMissObj->GetTmpPath();
						ASSERT(!tmppath.IsEmpty());
						m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_1_%u_dzh.indexfile.txt",
							tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
							now.wMinute, now.wSecond, m_sip, m_dip, rand());
						CString text;
						text.Format("user= %s\r\nstat= 上线\r\n", m_userid);
						WriteIndexFile("ths", "ths", text, g_pImsgMissObj);
						m_CaseArr.RemoveAll();
						m_WarnArr.RemoveAll();
					}
					m_balreadyout = TRUE;
				}
			}
		}
	}
	return 0;
}