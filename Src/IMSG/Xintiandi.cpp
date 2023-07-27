// Xintiandi.cpp: implementation of the CXintiandi class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "imsg.h"
#include "Xintiandi.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXintiandi::CXintiandi(DATEHEADER& packh, int aT)
: SocketSession(packh, aT)
{
	m_closetm = packh.m_tm + 120;
}


CXintiandi::~CXintiandi()
{

}
const unsigned char g_pxtdtable[] = {
	0x00, 0x4D, 0xC5, 0x65, 0xCE, 0xBE, 0xF9, 0x5D, 0xC8, 0x33, 0xF3, 0x5D, 0xED, 0x47, 0x5E, 0xEF, 
	0x8A, 0x44, 0x6C, 0x46, 0x69, 0xE1, 0x89, 0xD9, 0x10, 0x33, 0x7A, 0xC1, 0x30, 0xC2, 0xC3, 0xC6, 
	0xAF, 0xAC, 0xA9, 0x46, 0x54, 0x3D, 0x3E, 0x68, 0xBA, 0x72, 0x3D, 0xA8, 0x42, 0x81, 0xD0, 0xC0, 
	0xBB, 0xF9, 0xE8, 0xC1, 0x29, 0x71, 0x29, 0x2D, 0xF0, 0x10, 0x1D, 0xE4, 0xD0, 0xE4, 0x3D, 0x14, 
	0xA9, 0xDD, 0xCF, 0xB9, 0x08, 0xC0, 0x85, 0x48, 0x08, 0xBF, 0x67, 0x06, 0x56, 0xB5, 0xFE, 0x91, 
	0x5E, 0x4C, 0xEE, 0x83, 0xFB, 0x79, 0x63, 0x53, 0x1E, 0x1B, 0x06, 0xB7, 0x55, 0xA5, 0x58, 0x3B, 
	0x25, 0x23, 0xB5, 0xE5, 0x8E, 0x0E, 0x54, 0xEF, 0x4D, 0xA1, 0xFD, 0x11, 0x95, 0xC1, 0x65, 0xDA, 
	0x58, 0x98, 0x46, 0x0C, 0x6E, 0x81, 0xC2, 0x03, 0x1C, 0xDA, 0x97, 0x95, 0xD0, 0xDE, 0x2F, 0xF4
};

int CXintiandi::DecodeLogin(char buf[0x50] )
{
	int i;
	unsigned char tt;
	int swapstep = 1;
	for(i = 2; i+swapstep <= 0x4f; i+=(swapstep+1))
	{
		tt = buf[i];
		buf[i] = buf[i+swapstep];
		buf[i+swapstep] = tt;
	}
	swapstep = buf[0];
	if(swapstep <= 0)
		return 0;
	for(i = 2; i+swapstep <= 0x4f; i+=(swapstep+1))
	{
		tt = buf[i];
		buf[i] = buf[i+swapstep];
		buf[i+swapstep] = tt;
	}
	swapstep = 1;
	for(i = 2; i+swapstep <= 0x4f; i+=(swapstep+1))
	{
		tt = buf[i];
		if(i+swapstep )
			buf[i] = buf[i+swapstep];
		buf[i+swapstep] = tt;
	}
	swapstep = buf[1];
	if(swapstep <= 0)
		return 0;
	for(i = 2; i+swapstep <= 0x4f; i+=(swapstep+1))
	{
		tt = buf[i];
		buf[i] = buf[i+swapstep];
		buf[i+swapstep] = tt;
	}
	
	int deal = 2;
	int xorstep = buf[0]&0x7f;
	for(i = xorstep; i < 0x80 && deal <0x4f ; i++)
	{
		buf[deal++] ^= g_pxtdtable[i];
	}
	for(i = 1; deal <= 0x4f ; i++)
	{
		buf[deal++] ^= g_pxtdtable[i];	
	}

	if(memcmp(buf+2, "\x1\x4", 2) != 0)
		return 0;

	return 1;
}

int CXintiandi::InsertPacket( DATEHEADER& packh, const char *pPacket )
{
	try
	{
		if (!IsxtdPacket(packh, pPacket))
		{
			// 不是边锋的包
			return 0;
		}
		
		if ((packh.m_type == ProxyTCP) || (packh.m_type == ProxyUDP))
		{
			m_ProxyType = 1;
		}
		
		DWORD xtdid = 0;
		char xtdpass[256];
		memset(xtdpass, 0, 256);
		char buf[0x50];
		if(packh.m_datalen == 0x50)
		{
			memcpy(buf, pPacket, 0x50);
			if(DecodeLogin(buf))
			{
				xtdid = *(DWORD*)(buf+0x28);
				int len = buf[0x2c];
				if(len < 0x10)
					memcpy(xtdpass, buf+0x2d, len);
			}
		}

		if (xtdid > 10000)
		{
			m_userid.Format("%d", xtdid);
			
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
				WriteHisData("imsg_xtd", "imsg_xtd", &hisRec, g_pImsgMissObj,	IMSG_PROTOCOL);//, HISTORY_DATA);
			}
			
			// 写对照文件
			if ((m_CaseArr.GetSize() > 0) || (m_WarnArr.GetSize() > 0)
				|| (!m_rubbishflag && m_OpenIndxFlg))
			{
				SYSTEMTIME now;
				GetLocalTime(&now);
				CString tmppath = g_pImsgMissObj->GetTmpPath();
				ASSERT(!tmppath.IsEmpty());
				m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_%u_imsg_xtd.indexfile.txt",
					tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
					now.wMinute, now.wSecond, m_sip, m_dip, rand());
				CString text;
				text.Format("user= %s\r\npass= %s\r\nstat= 上线\r\n", m_userid, xtdpass);
				WriteIndexFile("imsg_xtd", "imsg_xtd", text, g_pImsgMissObj);
				m_CaseArr.RemoveAll();
				m_WarnArr.RemoveAll();
			}
		} // end if(isPass)
	}catch(...){

	}
	return 0;
}

