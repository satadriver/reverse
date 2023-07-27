#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "SQQVideo.h"
#include "winsock.h"

BOOL WriteIndexFile(SQQSession* s, time_t fendtm);

SQQSession::SQQSession(DATEHEADER& packh, int aT)
: MediaSession(packh, aT)
{
	m_processtm = packh.m_tm;
	m_begfiletm = packh.m_tm;
	m_Packettype = packh.m_type;
	if(packh.m_type == TCPPacket)	//TCP	//音视频  //sip
	{
		m_AvType = 1;
	}
	else if(packh.m_type == UDPPacket)	//UDP   //音视频  //sip
	{
		m_AvType = isSQQVideo(packh, (BYTE*)g_Packet);
	}

	m_MediaPro = _T("media_SOQ");		// 协议类型(即对照文件中的协议字段值)
	m_MediaDirection = _T("<<==>>");		// 音视频方向(即对照文件中的方向字段值)
	m_IDCSvrType = _T("soqqmedia");			// IDC服务器类型(英文表示) 为空或NULL表示不需要输出IDCServer
	m_IDCSvrSubType = _T("SOQ音视频");	// IDC服务器类型(中文表示) 为空或NULL表示不需要输出IDCServer

}

// BOOL SQQSession::CloseSession(time_t nowtm)
// {
// 	// 排序
// 	m_packetlist.sort();
// 	m_processtm = nowtm-20;
// 	// 写文件
// 	LstToSQQFile();
// 	if(!m_realfname.IsEmpty() && GetFileLength(m_realfname)>100000)
// 	{
// 		//写对照文件
// //		if (!sipSQQ.IsEmpty() && CaseID.IsEmpty())
// //		{
// //			CaseInfo cinfo;
// //			if (SearchCaseByKey(sipSQQ, sipSQQ.GetLength(), cinfo) ||
// //				SearchCaseByKey(dipSQQ, dipSQQ.GetLength(), cinfo) )
// //			{
// //				AllWatchFlg = cinfo.AllWacthFlg;
// //				CaseID = cinfo.CaseID;
// //				LimitAddr = cinfo.LimitAddr;
// //				LimitType = cinfo.LimitType;
// //			}
// //		}
// /*		if (WriteIndexFile(this, lastpacktm))
// 		{
// 			if (!CaseID.IsEmpty() || (!rubbishflag && v_openflagSoQMedia))
// 			{
// 				CStringArray attfile;
// 				CopyFileToTmpdir(CaseID, fname, indexfilename, attfile,
// 					pthisMissObj->m_nowstm, pthisMissObj->m_ddefiletype, pthisMissObj->m_ServerPower,
// 					(v_openflagSoQMedia)? pthisMissObj->m_ServerPower+1 : NULL,
// 					ruleidarr,fenleiarr,levelarr,cardarr,namearr,peopletypearr,
// 					peoplearr,keytypearr,IsOnlyAlarmArr,dummytypeArr);
// 				
// 			}
// 		}
// 		DeleteFile(fname);
// 		fname.Empty();
// 		DeleteFile(indexfilename);
// 		indexfilename.Empty();*/
// 
// 		m_OpenIndxFlg = v_openflagSoQMedia;
// 		m_OpenCaseFlg = v_opencaseSoQMedia;
// 		m_OpenWarnFlg = v_openwarnSoQMedia;
// 
// 		CString text;	// 需要匹配案件和预警的串
// // 			text.Format("____%s\r\n____%s\r\n____%s", 
// // 				m_USER, m_PASS, filesfield);
// 		// 匹配案件和预警
// 		DWORD d_state = 1;
// 		SearchCaseOrWarn(this, (char*)(LPCTSTR)text, text.GetLength(), d_state);
// 
// 		// 处理IDC数据
// //		WriteIdcStat(this, "soqqmedia", "SOQ音视频");
// 
// 		// 写活动历史
// 
// 		HistoryRecord hisRec;
// 		CString tmpstrhis;
// 		tmpstrhis.Format("%s",(m_AVtype==1)?"视频":"音频");
// 		hisRec.SetRecord(IDX_STATE,		tmpstrhis);
// 
// 		WriteHisData("media", "media_SOQ", &hisRec, g_MediaMissObj, MEDIA_PROTOCOL, HISTORY_DATA);
// 
// 		// 写对照文件并输出
// 		char starttm[40], endtm[40];
// 		GetTimeString(m_filetm,  "%Y-%m-%d %H:%M:%S",starttm, 40);
// 		GetTimeString(nowtm,  "%Y-%m-%d %H:%M:%S",endtm, 40);
// 
// 		text.Format("方向= <<==>>\r\nbegtm= %s\r\nendtm= %s\r\nfiletype= %s\r\n",starttm,endtm,(m_AVtype==1)?"视频":"音频");
// 
// 		MediaTmpPath = g_MediaMissObj->GetTmpPath();
// 		m_indexfname.Format("%s.indexfile.txt",m_realfname);
// 
// 		WriteIndexFile("media", "media_SOQ", text, g_MediaMissObj);
// 		m_realfname.Empty();
// 		m_indexfname.Empty();
// 	}
// 	return TRUE;
// }

int SQQSession::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	MEDIAPACKET soqpack(packh, (CHAR *)pPacket);
	if(packh.m_type == UDPPacket)
	{
		int ret = isSQQVideo(packh,(BYTE *)pPacket);
			
		if(ret > 0)
		{
			soqpack.m_serial = Swap2Byte(*(WORD*)(pPacket + 2));
			soqpack.m_vtype = ret;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		soqpack.m_vtype = 1;
	}
	if (soqpack.m_buf == NULL)
	{
		return 0;
	}
	if (memcmp(packh.m_sip,m_cdip, 4) == 0)
	{
		soqpack.m_vtype |= 0x40;
	}
	m_packetlist.push_back(soqpack);
	soqpack.m_buf = NULL;
	m_lastpacktm = packh.m_tm;
	if (abs(packh.m_tm - m_processtm) >= 60)
	{
		// 写文件
		m_processtm = packh.m_tm - 30;
		ListToFile();
	}
	return 0;
}

int SQQSession::ListToFile()
{
	m_OpenWarnFlg = v_openwarnSoQMedia;
	m_OpenCaseFlg = v_opencaseSoQMedia;
	m_OpenIndxFlg = v_openflagSoQMedia;

	m_packetlist.sort();
	MediaPacketListIt item, begin,end;
	CFile	file;
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateMediaFilename("SoQ", m_sip, m_dip);
		if (m_realfname.IsEmpty())
		{
			return -2;
		}
	}

	if (!file.Open(m_realfname, CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate))
	{
		return -2;
	}
	file.SeekToEnd();
		
	try
	{
		OLDDATEHEADER h1, h2;
		memset(&h1, 0, sizeof(h1));
		memset(&h2, 0, sizeof(h2));
		h1.m_type = h2.m_type = this->m_Packettype;
		memcpy(h1.m_sip, this->m_csip, 4);
		memcpy(h1.m_dip, this->m_cdip, 4);
		memcpy(h2.m_dip, this->m_csip, 4);
		memcpy(h2.m_sip, this->m_cdip, 4);
		h1.m_sport = h2.m_dport = atoi(this->m_sip.Right(5));
		h1.m_dport = h2.m_sport = atoi(this->m_dip.Right(5));
		while(m_packetlist.size()>10)
		{
			begin = m_packetlist.begin();
			item = begin;
			end=m_packetlist.end();
			item++;
#ifdef		_DEBUG
			MEDIAPACKET	*p1, *p2;
			p1 = &(*item);
			p2 = &(*begin);
#endif
			if(this->m_Packettype == UDPPacket)
			{
				for (int i = 1; i < min(m_packetlist.size(), 40); i++)
				{
					if(item == end)
						break;
					if (begin->m_vtype == item->m_vtype)
					{
						if ((begin->m_serial - item->m_serial) == 0)
						{
							MediaPacketListIt itemtmp;
							itemtmp=item;
							item++;							
							m_packetlist.erase(itemtmp);
							continue;
						}
						else if((WORD)(begin->m_serial-item->m_serial)<0x8000 && abs(begin->m_tm-item->m_tm)<=2 )
						{
							MediaPacketListIt itemtmp;
							itemtmp=item;
							itemtmp++;
							swap(begin,item);
							item=itemtmp;
							continue;
						}
					}
					item++;
				}
				if (begin->m_tm>m_processtm)
					break;
			}
			m_endfiletm = begin->m_tm;
			WriteMediaFile(file, (begin->m_vtype&0x40)? h2: h1, *begin);
			m_packetlist.erase(begin);
		}
		file.Close();
	}
	catch(...)
	{
		WriteLog(MEDIAPRO, "LstToSQQFile() Error!!");
		return -3;
	}
	return 0;
}

