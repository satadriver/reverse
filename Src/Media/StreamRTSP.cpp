// StreamRTSP.cpp: implementation of the CStreamRTSP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MediaSession.h"
#include "Media.h"
#include "StreamRTSP.h"

Search	SearchCharset("Accept-Charset: ");
Search	SearchLanguage("Accept-Language: ");


RtspSession::RtspSession(DATEHEADER& packh, int aT)
: MediaSession(packh, aT)
{
	SocketSession::SocketSession(packh, aT);
	if (IsRTSPPacket(packh, g_Packet)==2)
	{
		// Server --> Client
		// 交换会话方向
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
	}
	m_closetm = packh.m_tm + 60;
	m_lastpacktm = packh.m_tm;
	m_opHappen = 0;

	// 匹配数据源
	if (m_dataSrc == DATASRC_OTHER)
	{
		if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_cdip), m_strISP)
			|| (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)m_csip), m_strISP)))
		{
			m_dataSrc = DATASRC_IDC;
		}
	}
}

int RtspSession::InsertPacket(DATEHEADER& packh, const char* pPacket)
{
	try
	{
		if (memcmp(pPacket, "DESCRIBE", 8) == 0)
		{
			char firstline[1024];
			int iCrLf = SearchCRLF.find(pPacket, packh.m_datalen);
			if (iCrLf < 0)
			{
				iCrLf = Search2CRLFUnix.find(pPacket, packh.m_datalen);
			}
			if (iCrLf > 0)
			{
				memset(firstline, 0, 1024);
				memcpy(firstline, pPacket, min(iCrLf, 1000));
				int offCharset = SearchCharset.find(pPacket, packh.m_datalen);
				if(offCharset == -1)
					m_codeType = "ANSI";
				else
				{
					iCrLf = SearchCRLF.find(pPacket+offCharset, packh.m_datalen-offCharset); 
					if(iCrLf > 16)
						m_codeType = CString(pPacket+offCharset+16, iCrLf-16);
				}

				offCharset = SearchLanguage.find(pPacket, packh.m_datalen);
				if(offCharset == -1)
					m_language.Empty();
				else
				{
					iCrLf = SearchCRLF.find(pPacket+offCharset, packh.m_datalen-offCharset); 
					if(iCrLf > 17)
						m_language = CString(pPacket+offCharset+17, iCrLf-17);
				}
				
				m_realname = ConverHttp2Html(firstline+8);	// 跳过 DESCRIBE rtsp://
				if(m_codeType.Find("UTF-8") != -1)
				{
					m_realname = NetBytestoAnsi(m_realname);	// 跳过 DESCRIBE rtsp://
				}
				m_realname.TrimRight("RTSP/1.023456789");
				m_realname.TrimRight();
				m_realname.TrimLeft();
				m_opHappen |= 0x1;		// DESCRIBE
			}
		}
		else if (memcmp(pPacket, "SETUP", 5) == 0)
		{
			m_opHappen |= 0x2;		// DESCRIBE
		}
		else if (memcmp(pPacket, "RTSP/1.", 7) == 0)
		{
			if(memcmp(pPacket+9, "200 OK", 6) == 0)
				m_opHappen |= 0x4;
		}

		if(m_opHappen == 0x7 && !m_realname.IsEmpty())
		{
			m_OpenIndxFlg = v_openflagrtsp;
			m_OpenCaseFlg = v_opencasertsp;
			m_OpenWarnFlg = v_openwarnrtsp;

			// 匹配案件和预警
			DWORD dwState = 1;
			SearchCaseOrWarn(this, m_realname, m_realname.GetLength(), dwState);

			// 处理IDCServer
			CString strISP;
			if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*((DWORD*)m_cdip)), strISP))
			{
				CString idcstr;
				idcstr.Format("%03u.%03u.%03u.%03u\1%s\1%s\1"
					"音视频服务\1%s\1%s\1%s\1\1",
					m_cdip[0], m_cdip[1], m_cdip[2], m_cdip[3], strISP,
					"rtsp", m_dip.Right(5), "RTSP", TYPESTATE_DECIDED);
				v_IdcServerStat[MEDIA_PROTOCOL].AddServer(idcstr);
			}

			// 写活动历史(该协议不写活动历史)

			// 写对照文件并输出
			CString text;
			text.Format("charset= %s\r\nfile= %s\r\n", m_codeType, m_realname);

			SYSTEMTIME now;
			GetLocalTime(&now);
			CString tmpdir = g_MediaMissObj->GetTmpPath();
			
			m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_rtsp.indexfile.txt",
				tmpdir, now.wYear, now.wMonth, now.wDay,
				now.wHour, now.wMinute, now.wSecond, m_sip, m_dip, ++g_MediaFileNum);


			WriteIndexFile("rtsp", "rtsp", text, g_MediaMissObj);
			m_realfname.Empty();
			m_indexfname.Empty();
			m_opHappen = 0;
			m_realname.Empty();
		}
	}
	catch(...)
	{
		WriteLog(MEDIAPRO, "RtspSession::InsertPacket() Error!!");
	}
	return 1;
}



