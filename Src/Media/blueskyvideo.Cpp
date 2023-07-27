#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "BLUESKYVideo.h"

BLUESKYSession::BLUESKYSession(DATEHEADER& packh, BYTE* pPacket, int aT)
: MediaSession(packh, aT)
{
	m_MediaPro = _T("media_����");		// Э������(�������ļ��е�Э���ֶ�ֵ)
	m_MediaDirection = _T("==>>");		// ����Ƶ����(�������ļ��еķ����ֶ�ֵ)
	m_AvType = isBLUESKYVideo(packh, pPacket);						// ʵ���ļ�����(1:��Ƶ / 2:��Ƶ / 3:����Ƶ)
	m_IDCSvrType = _T("blkmedia");		// IDC����������(Ӣ�ı�ʾ) Ϊ�ջ�NULL��ʾ����Ҫ���IDCServer
	m_IDCSvrSubType = _T("��������Ƶ");	// IDC����������(���ı�ʾ) Ϊ�ջ�NULL��ʾ����Ҫ���IDCServer

	m_processtm = packh.m_tm;
	m_begfiletm = packh.m_tm;
	m_Packettype = packh.m_type;
}

// BOOL BLUESKYSession::CloseSession(time_t nowtm)
// {
// 	// ����
// 	m_packetlist.sort();
// 	m_processtm = nowtm-20;
// 	// д�ļ�
// 	LstToBLUESKYFile();
// 	if(!m_realfname.IsEmpty() && GetFileLength(m_realfname)>100000)
// 	{
// 		//д�����ļ�
// //		if (!sipBLUESKY.IsEmpty() && CaseID.IsEmpty())
// //		{
// //			CaseInfo cinfo;
// //			if (SearchCaseByKey(sipBLUESKY, sipBLUESKY.GetLength(), cinfo) ||
// //				SearchCaseByKey(dipBLUESKY, dipBLUESKY.GetLength(), cinfo) )
// //			{
// //				AllWatchFlg = cinfo.AllWacthFlg;
// //				CaseID = cinfo.CaseID;
// //				LimitAddr = cinfo.LimitAddr;
// //				LimitType = cinfo.LimitType;
// //			}
// //		}
// /*		if (WriteIndexFile(this, lastpacktm))
// 		{
// 			if (!CaseID.IsEmpty() || (!rubbishflag && v_openflagBLMedia))
// 			{
// 				CStringArray attfile;
// 				CopyFileToTmpdir(CaseID, fname, indexfilename, attfile,
// 					pthisMissObj->m_nowstm, pthisMissObj->m_ddefiletype, pthisMissObj->m_ServerPower,
// 					(v_openflagBLMedia)? pthisMissObj->m_ServerPower+1 : NULL,
// 					ruleidarr,fenleiarr,levelarr,cardarr,namearr,peopletypearr,
// 					peoplearr,keytypearr,IsOnlyAlarmArr,dummytypeArr);
// 				
// 			}
// 		}
// 		DeleteFile(fname);
// 		fname.Empty();
// 		DeleteFile(indexfilename);
// 		indexfilename.Empty();*/
// 		m_OpenIndxFlg = v_openflagBLMedia;
// 		m_OpenCaseFlg = v_opencaseBLMedia;
// 		m_OpenWarnFlg = v_openwarnBLMedia;
// 
// 		CString text;	// ��Ҫƥ�䰸����Ԥ���Ĵ�
// // 			text.Format("____%s\r\n____%s\r\n____%s", 
// // 				m_USER, m_PASS, filesfield);
// 		// ƥ�䰸����Ԥ��
// 		DWORD d_state = 1;
// 		SearchCaseOrWarn(this, (char*)(LPCTSTR)text, text.GetLength(), d_state);
// 
// 		// ����IDC����
// //		WriteIdcStat(this, "blkmedia", "��������Ƶ");
// 
// 		// д���ʷ
// 
// 		HistoryRecord hisRec;
// 		CString tmpstrhis;
// 		tmpstrhis.Format("%s",(m_AVtype==1)?"��Ƶ":"��Ƶ");
// 		hisRec.SetRecord(IDX_STATE,		tmpstrhis);
// 
// 		WriteHisData("media", "media_����", &hisRec, g_MediaMissObj, MEDIA_PROTOCOL, HISTORY_DATA);
// 
// 
// 		// д�����ļ������
// 		char starttm[40], endtm[40];
// 		GetTimeString(m_filetm,  "%Y-%m-%d %H:%M:%S",starttm, 40);
// 		GetTimeString(m_lastpacktm,  "%Y-%m-%d %H:%M:%S",endtm, 40);
// 
// 		text.Format("����= <<==>>\r\nbegtm= %s\r\nendtm= %s\r\nfiletype= %s\r\n",starttm,endtm,(m_AVtype==1)?"��Ƶ":"��Ƶ");
// 		
// 		m_indexfname.Format("%s.indexfile.txt",m_realfname);
// 
// 		SYSTEMTIME now;
// 		GetLocalTime(&now);
// 		MediaTmpPath = g_MediaMissObj->GetTmpPath();
// 		WriteIndexFile("media", "media_����", text, g_MediaMissObj);
// 
// 		m_realfname.Empty();
// 		m_indexfname.Empty();
// 	}
// 	return TRUE;
// }
// 
int BLUESKYSession::InsertPacket(DATEHEADER & packh, const char *pPacket)
{
	MEDIAPACKET blpack(packh, (CHAR *)pPacket);
	
//	if(packh.type==UDPPacket)
//	{
//		qqpack.serial=packh.ipid;
//	}
//		int ret=isBLUESKYVideo(packh,(BYTE *)pPacket);
//			
//	 if( ret==2)
//		{
//			//qqpack.serial=ntohs(*(WORD*)(pPacket+2));
//			qqpack.vtype=ret;
//		}
//		else
//			return 0;
//	}
//	else{
//		qqpack.vtype=1;
//	}

	blpack.m_vtype = m_AvType;
	if (blpack.m_buf == NULL)
	{
		return 0;
	}
	if (memcmp(blpack.m_sip, m_cdip, 4) == 0)
	{
		blpack.m_vtype |= 0x40;
	}
	m_packetlist.push_back(blpack);
	blpack.m_buf = NULL;
	m_lastpacktm = packh.m_tm;

	if (abs(packh.m_tm - m_processtm) > 60)
	{
		// д�ļ�
		m_processtm = packh.m_tm - 20;
		ListToFile();
	}
	return 0;
}

int BLUESKYSession::ListToFile()
{
	m_OpenIndxFlg = v_openflagBLMedia;
	m_OpenCaseFlg = v_opencaseBLMedia;
	m_OpenWarnFlg = v_openwarnBLMedia;

	m_packetlist.sort();
	MediaPacketListIt item, begin,end;
	CFile	file;
	if (m_realfname.IsEmpty())
	{
		m_realfname = CreateMediaFilename("bsv", m_sip, m_dip);
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
		begin = m_packetlist.begin();
		end = m_packetlist.end();
		while(begin != end)
		{
			item = begin;
			begin++;
#ifdef		_DEBUG
			MEDIAPACKET	*p1, *p2;
			p1 = &(*item);
			p2 = &(*begin);
#endif
// 			int count = min(m_packetlist.size(), 40);
// 			for (int i = 1; i < count; i++)
// 			{
// 				if(item==end)
// 					break;
// 				if (begin->m_vtype == item->m_vtype)
// 				{
// 
// 					if(begin->m_serial ==0 && item->m_serial==0)
// 					{
// 						break;
// 					}
// 					else
// 					{
// 						if ((begin->m_serial-item->m_serial)==0 )
// 						{
// 							MediaPacketListIt itemtmp;
// 							itemtmp=item;
// 							item++;						
// 							m_packetlist.erase(itemtmp);
// 							continue;
// 							
// 						}
// 						else if((WORD)(begin->m_serial-item->m_serial)<0x8000 && abs(begin->m_tm-item->m_tm)<=2 )
// 						{
// 							MediaPacketListIt itemtmp;
// 							itemtmp=item;
// 							itemtmp++;
// 							swap(begin,item);
// 							item=itemtmp;
// 							continue;
// 						}
// 					}
// 
// 				}
// 				++item;
// 			}
			if (item->m_tm > m_processtm)
			{
				break;
			}
			m_endfiletm = item->m_tm;
			WriteMediaFile(file, (item->m_vtype & 0x40)? h2: h1, *item);
			m_packetlist.erase(item);
		}
		file.Close();
	}
	catch(...)
	{
		WriteLog(MEDIAPRO, "LstToBLUESKYFile() Error!!");
		return -3;
	}
	return 0;
}
