// MaiQuan.cpp: implementation of the CMaiQuan class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "imsg.h"
#include "MaiQuan.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMaiQuan::CMaiQuan(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 120;
}

CMaiQuan::~CMaiQuan()
{

}

int CMaiQuan::InsertPacket( DATEHEADER& packh, const char *pPacket )
{
	try
	{
		if (!IsMQPacket(packh, pPacket))
		{
			// 不是边锋的包
			return 0;
		}
		
		if ((packh.m_type == ProxyTCP) || (packh.m_type == ProxyUDP))
		{
			m_ProxyType = 1;
		}

		char passmd5[40];
		memset(passmd5, 0, 40);
		for (int ii = 22; ii < 0x20 ; ii++)
		{
			if((pPacket[ii] >= '0' && pPacket[ii] <='9') 
				|| (pPacket[ii] >= 'a' && pPacket[ii] <='f') )
			{
				passmd5[ii-22] = pPacket[ii];
			}
			else
			{
				passmd5[0] = 0;
				break;
			}
		}

		DWORD mqid = Swap4Byte(*(DWORD*)(pPacket+6));
		if (passmd5[0] != 0)
		{
			m_userid.Format("%d", mqid);
			
			m_begfiletm = packh.m_tm;
			m_OpenCaseFlg = v_opencasebf;
			m_OpenWarnFlg = v_openwarnbf;
			m_OpenIndxFlg = v_openflagbf;
			
			// 匹配案件、预警和屏蔽信息
			DWORD state = 0;
			SearchCaseOrWarn(this, m_userid, m_userid.GetLength(), state);
			SearchCaseEmailIm(this, m_userid, m_userid.GetLength(), 0);
			// 处理IDC数据源
			CString tmpisp;
			if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), tmpisp))
			{
				if (m_dataSrc == DATASRC_OTHER)
				{
					m_dataSrc = DATASRC_IDC;
					m_strISP = tmpisp;
				}
// 				if (v_statIdc)
// 				{
// 					CString ttstr;
// 					ttstr.Format("%s\1%s\1bf\1游戏%s\1边锋\1%s\1\1",
// 						m_dip.Left(15), m_strISP, m_dip.Right(5), TYPESTATE_DECIDED);
// 					v_IdcServerStat[IMSG_PROTOCOL].AddServer(ttstr);
// 				}
			}
			else if ((m_dataSrc == DATASRC_OTHER)
				&& (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), tmpisp)))
			{
				m_dataSrc = DATASRC_IDC;
				m_strISP = tmpisp;
			}
			
			// 输出活动历史
			if (v_statGame)
			{
				HistoryRecord hisRec;
				hisRec.SetRecord(IDX_FORWTYPE,	"1");
				hisRec.SetRecord(IDX_ID,		m_userid);
				hisRec.SetRecord(IDX_USER,		m_userid);
				
				hisRec.SetRecord(IDX_STATE, "上线");
				WriteHisData("imsg_mq", "imsg_mq", &hisRec, g_pImsgMissObj,	IMSG_PROTOCOL);//, HISTORY_DATA);
			}
			
			// 写对照文件
			if ((m_CaseArr.GetSize() > 0) || (m_WarnArr.GetSize() > 0)
				|| (!m_rubbishflag && m_OpenIndxFlg))
			{
				SYSTEMTIME now;
				GetLocalTime(&now);
				CString tmppath = g_pImsgMissObj->GetTmpPath();
				ASSERT(!tmppath.IsEmpty());
				m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_%u_imsg_mq.indexfile.txt",
					tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
					now.wMinute, now.wSecond, m_sip, m_dip, rand());
				CString text;
				text.Format("user= %s\r\nmd5pass= %s\r\nstat= 上线\r\n", m_userid, passmd5);
				WriteIndexFile("imsg_mq", "imsg_mq", text, g_pImsgMissObj);
				m_CaseArr.RemoveAll();
				m_WarnArr.RemoveAll();
			}
		} // end if(isPass)
	}catch(...){

	}

	return 0;
}
