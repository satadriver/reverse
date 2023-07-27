
/***************************  Flash视频处理  ******************************/

#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "FlashVideo.h"

#define MEDIABUFFERSIZE		300000
#define CHUNKBUFFERSIZE		1000000

//深圳 下行回滚
char	FILEBUFFER[FILEBUFFERSIZE];

UCHAR chunkbuf[CHUNKBUFFERSIZE];
UCHAR filebuffer[CHUNKBUFFERSIZE];

//深圳 下行回滚
BOOL ParserFlashCommand(UCHAR *p,BOOL bhead);
BOOL ParserFlashCommand(UCHAR *p,flashstatus & flashdatastatus,UCHAR suffix);
int SearchFlashHeader(UCHAR * p,int len);

int WriteFlashStream(FILE * fs,UCHAR * buf, UINT Length,UINT medialength,UCHAR mediatype,UINT timestramp,MediaCommInfo *ppheader);
BOOL WriteFlashIndexFIle(FlashSession* s, time_t endtm);




inline void doubletonet8byte(UCHAR * buf,double ff)
{
	UCHAR * temp=(UCHAR*)(&ff);
	for (int i=0;i<8;i++)
		buf[i]=temp[7-i];
	
}

inline void net2bytetoshort(UCHAR* p, USHORT& ilong)
{
	ilong=0;
	memcpy((UCHAR *)&ilong+1,p,1);
	memcpy((UCHAR *)&ilong,p+1,1);
}

inline void net3bytetolong(UCHAR* p, UINT& ilong)
{
	ilong=0;
	//memcpy((UCHAR *)&ilong+1,p,3);
	//ilong=Swap4Byte((ULONG)ilong);
	memcpy((UCHAR *)&ilong+2,p,1);
	memcpy((UCHAR *)&ilong+1,p+1,1);
	memcpy((UCHAR *)&ilong+0,p+2,1);
}

CHAR FLVHEADER[]=//DOUBLE durationtime offset 0x35
"\x46\x4C\x56\x01\x0D\x00\x00\x00\x09\x00\x00\x00\x00\x12\x00\x01"
"\xA3\x00\x00\x00\x00\x00\x00\x00\x02\x00\x0A\x6F\x6E\x4D\x65\x74"
"\x61\x44\x61\x74\x61\x08\x00\x00\x00\x11\x00\x08\x64\x75\x72\x61"
"\x74\x69\x6F\x6E\x00\x40\x61\x4B\xB6\x45\xA1\xCA\xC1\x00\x0D\x6C"
"\x61\x73\x74\x74\x69\x6D\x65\x73\x74\x61\x6D\x70\x00\x40\x00\x00"
"\x00\x00\x00\x00\x00\x00\x15\x6C\x61\x73\x74\x6B\x65\x79\x66\x72"
"\x61\x6D\x65\x74\x69\x6D\x65\x73\x74\x61\x6D\x70\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x0C\x76\x69\x64\x65\x6F\x63\x6F\x64\x65"
"\x63\x69\x64\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0C\x63\x61"
"\x6E\x53\x65\x65\x6B\x54\x6F\x45\x6E\x64\x01\x00\x00\x05\x77\x69"
"\x64\x74\x68\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x06\x68\x65"
"\x69\x67\x68\x74\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0D\x76"
"\x69\x64\x65\x6F\x64\x61\x74\x61\x72\x61\x74\x65\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x0D\x61\x75\x64\x69\x6F\x64\x61\x74\x61"
"\x72\x61\x74\x65\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x09\x66"
"\x72\x61\x6D\x65\x72\x61\x74\x65\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x0C\x63\x72\x65\x61\x74\x69\x6F\x6E\x64\x61\x74\x65\x02"
"\x00\x07\x75\x6E\x6B\x6E\x6F\x77\x6E\x00\x0F\x6D\x65\x74\x61\x64"
"\x61\x74\x61\x63\x72\x65\x61\x74\x6F\x72\x02\x00\x24\x52\x42\x59"
"\x57\x20\x20\x20\x47\x72\x6F\x75\x70\x20\x46\x4C\x56\x20\x4D\x65"
"\x74\x61\x44\x61\x74\x61\x20\x49\x6E\x6A\x65\x63\x74\x6F\x72\x20"
"\x32\x00\x0C\x6D\x65\x74\x61\x64\x61\x74\x61\x64\x61\x74\x65\x0B"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x08\x66\x69\x6C\x65"
"\x73\x69\x7A\x65\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x09\x76"
"\x69\x64\x65\x6F\x73\x69\x7A\x65\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x09\x61\x75\x64\x69\x6F\x73\x69\x7A\x65\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x08\x64\x61\x74\x61\x73\x69\x7A\x65\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x09\x00\x00\x01\xAE";




int FlashSession::WriteFlashStream(FILE ** fs,UCHAR * buf,INT Length,UCHAR mediatype,  MediaCommInfo *pheader)
{
	int re=0;

	int bufpos=0;


	//写文件...

	if(*fs==0)
	{	
		if(m_realfname.IsEmpty())
		{
			m_realfname = CreateMediaFilename("flv", m_sip, m_dip);
			if (m_realfname.IsEmpty())
			{
				return -2;
			}
			//创建新文件，要置头标志为0，以写入头文件
			pheader->m_havehead=0;
		}

		CreateDir(MediaTmpPath);
		*fs=fopen(m_realfname,"ab");
		if(*fs==0)
		{
			WriteLog(MEDIAPRO, "Open File %s Error!!", m_realfname);
			return -1;
		}
	}



	try
	{
		while(bufpos<Length)
		{
			ULONG tagbodylen=0;
			ULONG tagtimestramp=0;   
			tagbodylen=*(ULONG *)(buf+bufpos);
			tagtimestramp=*(ULONG *)(buf+bufpos+4);
			bufpos+=8;
			int filelen=ftell((*fs));
//			ASSERT(filelen<=0x2d067-0x200);
			//printf("%s time stramp %8x tagbodylen=%05d %05d\n",(mediatype==8)?"音频":"视频",tagtimestramp,tagbodylen,Length);
			if( *fs>0 ){
				if(pheader->m_havehead==0)
				{
					fwrite(FLVHEADER,1,0x1bf,*fs);
					pheader->m_havehead=1;
				}
				if(tagbodylen)
				{
						fwrite(&mediatype,1,1,*fs);
						//写当前长度  3
						fwrite(((char *)&tagbodylen)+2,1,1,*fs);
						fwrite(((char *)&tagbodylen)+1,1,1,*fs);
						fwrite(((char *)&tagbodylen),1,1,*fs);
						
						fwrite(((char *)&tagtimestramp)+2,1,1,*fs);
						fwrite(((char *)&tagtimestramp)+1,1,1,*fs);
						fwrite(((char *)&tagtimestramp)+0,1,1,*fs);

						fwrite("\x0\x0\x0\x0",1,4,*fs);
						fwrite(buf+bufpos,1,tagbodylen,*fs);

						UINT tmplength=tagbodylen+11;
						fwrite(((char *)&tmplength)+3,1,1,*fs);
						fwrite(((char *)&tmplength)+2,1,1,*fs);
						fwrite(((char *)&tmplength)+1,1,1,*fs);
						fwrite(((char *)&tmplength),1,1,*fs);	

						pheader->m_havehead=1;
				}
			}
			bufpos+=tagbodylen;
		}
	}
	catch(...)
	{
//		WriteLog(strMedia, "WriteFlashStream() Error!!");
	}
	return re;
}


FlashSession::FlashSession(DATEHEADER& packh, int aT)
: MediaSession(packh, aT)
{
	m_closetm = packh.m_tm + 60;
	m_begfiletm = packh.m_tm;

	if(packh.m_sequence > MaxSeq)
	{
		m_CSeqOffset = SeqOffset;
	}
	else
	{
		m_CSeqOffset = 0;
	}
	if(packh.m_ack > MaxSeq)
	{
		m_CAckOffset = SeqOffset;
	}
	else
	{
		m_CAckOffset = 0;
	}
	m_bfromserver = 3;

	m_MediaPro = _T("media_FLASH");		// 协议类型(即对照文件中的协议字段值)
	m_MediaDirection = _T("==>>");		// 音视频方向(即对照文件中的方向字段值)
	m_AvType = 3;						// 实体文件类型(1:视频 / 2:音频 / 3:音视频)
	m_IDCSvrType = _T("flash");		// IDC服务器类型(英文表示) 为空或NULL表示不需要输出IDCServer
	m_IDCSvrSubType = _T("FLV音视频");	// IDC服务器类型(中文表示) 为空或NULL表示不需要输出IDCServer

}

// FlashSession::CloseSession(time_t nowtm)
// {
// 	
// 	if(m_PacketCount>20)
// 	{
// 		if (this->m_sortflag=='L')
// 			m_Packetlst.sort();
// 		if(this->m_havesep)
// 		{
// 			this->lstToFlvFile(chunkbuf, filebuffer, TRUE);
// 		}
// 		else
// 		{
// 			this->lstToFlvFileOld(chunkbuf, filebuffer, m_Responseline.m_Ssequence);
// 		}
// 	}
// 	if(m_realfname!="" && GetFileLength(m_realfname)>100000)
// 	{
// 		//写对照文件
// /*		WriteFlashIndexFIle(this, filetm);
// 		//copy 对照文件和视频文件
// 		if (!CaseID.IsEmpty() || (!rubbishflag && v_openflagFlashMedia))
// 		{
// 			CStringArray attfiles;
// 			CopyFileToTmpdir(CaseID, Responseline.filename, Responseline.indexfilename, attfiles,
// 				pthisMissObj->m_nowstm, pthisMissObj->m_ddefiletype, pthisMissObj->m_ServerPower,
// 				(v_openflagFlashMedia)? pthisMissObj->m_ServerPower+1 : NULL,
// 				ruleidarr,fenleiarr,levelarr,cardarr,namearr,peopletypearr,
// 				peoplearr,keytypearr,IsOnlyAlarmArr,dummytypeArr);
// 			
// 		}*/
// 		m_OpenIndxFlg = v_openflagFlashMedia;
// 		m_OpenCaseFlg = v_opencaseFlashMedia;
// 		m_OpenWarnFlg = v_openwarnFlashMedia;
// 
// 		CString text;	// 需要匹配案件和预警的串
// // 			text.Format("____%s\r\n____%s\r\n____%s", 
// // 				m_USER, m_PASS, filesfield);
// 		// 匹配案件和预警
// 		DWORD d_state = 1;
// 		SearchCaseOrWarn(this, (char*)(LPCTSTR)text, text.GetLength(), d_state);
// 
// 		// 处理IDC数据
// //		WriteIdcStat(this, "flash", "FLV音视频");
// 
// 		// 写活动历史
// 
// 		HistoryRecord hisRec;
// 		hisRec.SetRecord(IDX_STATE,		"视频聊天");
// 
// 		WriteHisData("media", "media_FLASH", &hisRec, g_MediaMissObj, MEDIA_PROTOCOL, HISTORY_DATA);
// 
// 		// 写对照文件并输出
// 		char starttm[40], endtm[40];
// 		GetTimeString(m_filetm,  "%Y-%m-%d %H:%M:%S",starttm, 40);
// 		GetTimeString(nowtm,  "%Y-%m-%d %H:%M:%S",endtm, 40);
// 
// 		text.Format("方向= ==>>\r\nbegtm= %s\r\nendtm= %s\r\nfiletype= 音视频",starttm,endtm);
// 
// 		m_indexfname.Format("%s.indexfile.txt",m_realfname);
// 
// 		WriteIndexFile("media", "media_FLASH", text, g_MediaMissObj);
// 		m_realfname.Empty();
// 				m_indexfname.Empty();
// 	}
// 	return TRUE;
// }
// 
FlashSession::InsertPacket(DATEHEADER& packh,const char *pPacket)
{
	if(packh.m_type != TCPPacket)
	{
		return 0;
	}
	TcpPacket	Tcpp(packh, pPacket, m_CSeqOffset, m_CAckOffset) ;
	m_Packetlst.push_back(Tcpp);
	Tcpp.buf=0;
	m_filetm = packh.m_tm;	
	m_lastpacktm=packh.m_tm;
	if(m_Packetlst.size() > 400)
	{
		return ListToFile();
	}
	return 0;
}

int FlashSession::ListToFile()
{
	m_OpenWarnFlg = v_openwarnFlashMedia;
	m_OpenCaseFlg = v_opencaseFlashMedia;
	m_OpenIndxFlg = v_openflagFlashMedia;

	m_Packetlst.sort();
//VMPBEGIN
	if((m_Responseline.m_fileseq==0) && (m_Packetlst.size()>0))
	{
		TcpPacketListIt begin = m_Packetlst.begin();
		m_Responseline.m_Stm = begin->tm;
		m_Responseline.m_fileseq = begin->Ssequence;
		m_Responseline.m_Ssequence = begin->Ssequence;
		m_Responseline.m_nseq = begin->Ssequence;
	}
	if(m_havesep)
	{
		return lstToFlvFile(chunkbuf, filebuffer);
	}
	else 
	{
		return lstToFlvFileOld(chunkbuf, filebuffer, m_Responseline.m_Ssequence);
	}
//VMPEND
}

int FlashSession::lstToFlvFile(UCHAR * buf, UCHAR * savefilebuf, BOOL bclose)
{
	int re=0;
	int i=0;
	//////////////////////////////////////////////////////////////////////////
	BOOL blostpacket=FALSE;
	UINT filebufferlen=0;

	int Reservedlen=m_Responseline.m_Reservedlen;

	if(m_Responseline.m_audiolen>0)
	{
		memcpy(buf,m_Responseline.m_audiobuf,m_Responseline.m_audiolen);
	}
	m_audiolen=m_Responseline.m_audiolen;
	m_audiobuf=buf;
	if(m_Responseline.m_videolen>0)
	{
		memcpy(buf+0x30000,m_Responseline.m_videobuf,m_Responseline.m_videolen);
	}
	m_videolen=m_Responseline.m_videolen;
	m_videobuf=buf+0x30000;

	if(m_Responseline.m_taudiolen>0)
	{
		memcpy(buf+0x60000,m_Responseline.m_taudiobuf,m_Responseline.m_taudiolen);
	}
	m_taudiolen=m_Responseline.m_taudiolen;
	m_taudiobuf=buf+0x60000;

	if(m_Responseline.m_tvideolen>0)
	{
		memcpy(buf+0x90000,m_Responseline.m_tvideobuf,m_Responseline.m_tvideolen);
	}
	m_tvideolen=m_Responseline.m_tvideolen;
	m_tvideobuf=buf+0x90000;

	if(m_Responseline.m_Reservedlen>0)
	{
		memcpy(savefilebuf,m_Responseline.m_reserverbuf,m_Responseline.m_Reservedlen);
	}
	filebufferlen=m_Responseline.m_Reservedlen;


	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

	int error=0;
	//////////////////////////////////////////////////////////////////////////


	TcpPacketListIt start,last,itl,tmpitl;
	start=m_Packetlst.begin();
	last=m_Packetlst.end();
	static int l = 0;
	l++;
	if (l == 21)
		l = 21;
	try
	{

	
		while((m_Packetlst.size()>0 && bclose) || m_Packetlst.size()>30)
		{
error=0;
			itl=m_Packetlst.begin();
			tmpitl=itl;
			tmpitl++;
			last=m_Packetlst.end();
			m_endfiletm=itl->tm;
//			currenttm1=itl->tm1;
error=1;
			if(itl->Nextsequence < m_Responseline.m_fileseq)
			{
				m_Packetlst.pop_front();
				continue;
			}
			if(itl->Ssequence > m_Responseline.m_nseq)
			{

				m_Responseline.m_fileseq=itl->Ssequence;
				m_Responseline.m_nseq=itl->Nextsequence;

#ifdef		_DEBUG
//				printf("Loast Packet %u\n",Responseline.nseq);
#endif
				m_Packetlst.pop_front();
				blostpacket=TRUE;
				break;
			}//endif
error=2;
			if(itl->Ssequence < m_Responseline.m_fileseq)
			{
				memcpy(savefilebuf+m_Responseline.m_Reservedlen,	itl->buf+m_Responseline.m_fileseq-itl->Ssequence,
					itl->Slen-m_Responseline.m_fileseq+itl->Ssequence);
			}
			else
			{
				memcpy(savefilebuf+m_Responseline.m_Reservedlen-m_Responseline.m_fileseq+itl->Ssequence, itl->buf, itl->Slen);
			}

			if(m_Responseline.m_nseq< itl->Nextsequence)
				m_Responseline.m_nseq=itl->Nextsequence;
			m_Packetlst.pop_front();
		}

		//
		filebufferlen+=(m_Responseline.m_nseq-m_Responseline.m_fileseq);
		if(m_Responseline.m_havehead || filebufferlen >=0x800)
		{
			int relen=	ParserFlashFormat(savefilebuf,filebufferlen);
			//
			if(blostpacket)
				return -3;
			if(relen==-3 )
				return -3;
			else if(relen==8)
				return 8;
			m_Responseline.m_fileseq=m_Responseline.m_nseq;
				

		}
		else
		{
			return -3;
		}





	}
	catch (...)
	{
		WriteLog(MEDIAPRO, "LstToFlvFile() Error=%d!!", error);

		return -3;
	}

	
	//每5分钟写一个文件
// 	if(currenttm>m_Responseline.m_Stm+v_mediainterval*60 && m_Responseline.m_filename!="")
// 	{
// 		l=0;
// 		//写对照文件
// 		if(GetFileLength(Responseline.filename)>100000)
// 		{
// 			WriteFlashIndexFIle(this, currenttm);
// 			//copy 对照文件和视频文件
// 			if (!CaseID.IsEmpty() || (!rubbishflag && v_openflagFlashMedia))
// 			{
// 				CStringArray attfiles;
// 				CopyFileToTmpdir(CaseID, Responseline.filename, Responseline.indexfilename, attfiles,
// 					pthisMissObj->m_nowstm, pthisMissObj->m_ddefiletype, pthisMissObj->m_ServerPower,
// 					(v_openflagFlashMedia)? pthisMissObj->m_ServerPower+1 : NULL,
// 					ruleidarr,fenleiarr,levelarr,cardarr,namearr,peopletypearr,
// 					peoplearr,keytypearr,IsOnlyAlarmArr);
// 				
// 			}
// 		}
// 		//建立新的pResponseline		//变量初始化
// 		Responseline.Clear();
// 		Responseline.Stm=currenttm;
// 		Responseline.Stm1=currenttm1;
// 	}

	return re;	
}

BOOL WriteFlashIndexFIle(FlashSession* s, time_t fendtm)
{
	// 写对照文件			
/*	FILE *fp=0;
	s->Responseline.indexfilename=s->Responseline.filename+".indexfile.txt";
	fp=fopen(s->Responseline.indexfilename,"wb");
	if(!fp)
		return FALSE;
	try
	{
		AAAInfo pAaaInfo;
		CString tmppos;
		if(GetProbeAAAInfo(s->sip,s->dip,s->smac,s->dmac,&pAaaInfo,s->ipidarr,
			tmppos))
		{
			;
		}
		else
			tmppos.Format("%s",s->spyaddr);

		char starttm[40], endtm[40];
		GetTimeString(s->Responseline.Stm, "%Y-%m-%d %H:%M:%S", starttm, 40);
		GetTimeString(fendtm, "%Y-%m-%d %H:%M:%S", endtm,  40);
		CString sipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)s->csip)));
		CString dipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)s->cdip)));
		
		if (s->CaseID.IsEmpty())
		{
			WarnInfo warninfo;
			SearchIsWarnData(s->UserAccount1, s->sip.Left(15),s->smac,NULL, 0, warninfo,s->sip.Left(15), "media_FLASH");
			if (warninfo.fenleiarr.GetSize()>0)
			{
				s->fenleiarr.Append(warninfo.fenleiarr);
				s->cardarr.Append(warninfo.cardarr);
				s->keytypearr.Append(warninfo.keytypearr);
				s->levelarr.Append(warninfo.levelarr);
				s->namearr.Append(warninfo.namearr);
				s->peoplearr.Append(warninfo.peoplearr);
				s->peopletypearr.Append(warninfo.peopletypearr);
				s->ruleidarr.Append(warninfo.ruleidarr);
				s->IsOnlyAlarmArr.Append(warninfo.IsOnlyAlarmArr);
				s->dummytypeArr.Append(warninfo.dummytypeArr);
			}
		}
		
		// 写Idc统计信息
		WriteIdcStat(s, "flash", "FLV音视频");
		// 写活动历史
		WriteHisData(s, starttm, sipaddr, dipaddr, "media_FLASH", "视频聊天");
		
		WriteWarnAlarmArrToData(MEDIA_PROTOCOL,
			s->fenleiarr,s->ruleidarr,s->peopletypearr,
			s->peoplearr,s->namearr,s->cardarr,
			s->keytypearr,starttm,"","media_FLASH",
			s->levelarr,starttm,starttm,s->spyaddr,s->dummytypeArr);

		CString tmpsip, tmpdip;
		CString tmpsport, tmpdport;
		LPCTSTR tmpsmac, tmpdmac;
//		if (s->bfromserver==1)
//		{
//			tmpsip = s->dip.Left(15);
//			tmpdip = s->sip.Left(15);
//			tmpsport = s->dip.Right(5);
//			tmpdport = s->sip.Right(5);
//			tmpsmac = s->dmac;
//			tmpdmac = s->smac;
//		}
//		else
//		{
			tmpsip = s->sip.Left(15);
			tmpdip = s->dip.Left(15);
			tmpsport = s->sip.Right(5);
			tmpdport = s->dip.Right(5);
			tmpsmac = s->smac;
			tmpdmac = s->dmac;
//		}
		BOOL isReal;
		int ltype;
		if (s->LimitType > 200)
		{
			AddRealIP(tmpsip, s->CaseID, s->LimitAddr, s->LimitType);
			ltype = s->LimitType - 200;
			isReal = FALSE;
		}
		else if (s->LimitType > 100)
		{
			ltype = s->LimitType - 100;
			isReal = TRUE;
		}
		else
		{
			ltype = s->LimitType;
			isReal = FALSE;
		}
		tmpsip.Replace(".","_");
		tmpdip.Replace(".","_");
		WriteIndexfileField(fp, "[1]\r\npro= media_FLASH\r\nspyaddr", tmppos);
		WriteIndexfileField(fp, "sip",		tmpsip);
		WriteIndexfileField(fp, "sport",	tmpsport);
		WriteIndexfileField(fp, "dip",		tmpdip);
		WriteIndexfileField(fp, "dport",	tmpdport);
		WriteIndexfileField(fp, "sipaddr",	sipaddr);
		WriteIndexfileField(fp, "dipaddr",	dipaddr);
		WriteIndexfileField(fp, "datasrc",	s->m_dataSrc);
		WriteIndexfileField(fp, "ISP",		s->m_strISP);
		WriteIndexfileField(fp, "smac",		tmpsmac);
		WriteIndexfileField(fp, "dmac",		tmpdmac);
		WriteIndexfileField(fp,"begtm",		starttm);
		WriteIndexfileField(fp,"endtm",		endtm);
		WriteIndexfileField(fp,"filetype",	"音视频");
		if (!s->CaseID.IsEmpty())
		{
			WriteIndexfileField(fp, "caseid",	s->CaseID);
			WriteIndexfileField(fp, "objid",	s->ObjectID);
			WriteIndexfileField(fp, "limitaddr",s->LimitAddr);
			WriteIndexfileField(fp, "limittype",ltype);
		}
		else if (!pAaaInfo.CaseID.IsEmpty())
		{
			WriteIndexfileField(fp, "caseid",		pAaaInfo.CaseID);
			WriteIndexfileField(fp, "objid",		pAaaInfo.ObjectID);
			WriteIndexfileField(fp, "limitaddr",	pAaaInfo.LimitAddr);
			WriteIndexfileField(fp, "limittype",	pAaaInfo.LimitType);
		}
		if (!pAaaInfo.UserAccount1.IsEmpty())
		{
			WriteIndexfileField(fp, "account", pAaaInfo.UserAccount1);
			WriteIndexfileField(fp, "phone", pAaaInfo.Phone1);
			WriteIndexfileField(fp, "2account", pAaaInfo.UserAccount2);
			WriteIndexfileField(fp, "2phone", pAaaInfo.Phone2);
		}
		else
		{
			WriteIndexfileField(fp, "account", s->UserAccount1);
			WriteIndexfileField(fp, "phone", s->Phone1);
			WriteIndexfileField(fp, "2account", s->UserAccount2);
			WriteIndexfileField(fp, "2phone", s->Phone2);

		}

		WriteIndexfileField(fp, "filename",		GetShortName(s->Responseline.filename));
//		if(s->bfromserver==3)
//		{
			WriteIndexfileField(fp,"方向","=>>");
//		}
//		else
//		{
//			WriteIndexfileField(fp,"方向",(s->bfromserver)?"<<=":"=>>");
//		}
			
		if (isReal)
			WriteIndexfileField(fp,"扩展", "TRUE");
		fclose(fp);
	}
	catch (...)
	{
		WriteLog(strMedia, "WriteFlashIndexFIle() Error!!");
		fclose(fp);
		return FALSE;
	}*/
	return TRUE;
}
int CopyData(UCHAR * buf,int len,  UCHAR* videobuf,int & videolen,int timestramp,MediaCommInfo * pheader)
{
	if(len+videolen<0x10000)
	{
		if(timestramp>0x400)
			timestramp=0x100;
		pheader->m_timestramp+=timestramp;
		timestramp=pheader->m_timestramp;
		memcpy(videobuf+videolen,&len,4);
		videolen+=4;
		memcpy(videobuf+videolen,&timestramp,4);
		videolen+=4;
		memcpy(videobuf+videolen,buf,len);
		videolen+=len;
	}
	else{
		//printf("error\n");
	}
	return 1;

}
int CopyVideoAudioData(UCHAR * buf,int len,
					   UCHAR* videobuf,int & videolen,
					   UCHAR* audiobuf,int & audiolen,
					   int cmdtype)
{
	if(cmdtype==0x8)
	{
		if(len+audiolen<0x10000)
		{
			memcpy(audiobuf+audiolen,buf,len);
			audiolen+=len;
		}
// 		else
// 			printf("error\n");
		return 8;
	}
	else if(cmdtype==0x9)
	{
		if(len+videolen<0x10000)
		{

			memcpy(videobuf+videolen,buf,len);
			videolen+=len;
		}
// 		else
// 			printf("error\n");
		return 9;
	}
	else
		return 0;

}

int FlashSession::ParserFlashFormat(UCHAR * buf,int buflen )
{

	int re=0;
	int i=0;
//	filename.Format("%s%02d.flv",fname,i++);
	FILE * fs=0;

	MediaCommInfo *pheader=&m_Responseline;
	int bufpos=0;

	int videonum=-1;

//	amfobj pamfobg[0x40];

	INT amfheader;
	INT amfnumber;
	INT amftype;
	int headerlen=0;

	INT lastamfnumber=0;
	
//	memset (pamfobg,0,sizeof(amfobj)*0x40);
//	memset( &pheader,0,sizeof(MediaCommInfo));


	


	try
	{
		while (bufpos<buflen-0x100)
		{
			amfheader=(buf[bufpos] & 0xC0);
			amfheader>>=6;
			amfnumber=(buf[bufpos] & 0x3F);

			//
re=1;
			if( amfheader==0 || amfheader==1 )
			{
				//是否写文件
				pheader->m_pamfobg[amfnumber].bstart=FALSE;
				//
				amftype=buf[bufpos+7];
				if(amftype>0x14)
				{
//					printf("error");
					re=-3;
					break;
				}
				pheader->m_pamfobg[amfnumber].amfcmd=amftype;

				USHORT ilong=0;
				//ilong=*(USHORT*)(buf+bufpos+5);
				net2bytetoshort(buf+bufpos+5,pheader->m_pamfobg[amfnumber].bodysize);
				pheader->m_pamfobg[amfnumber].timestramp=0;
				net3bytetolong(buf+bufpos+1,pheader->m_pamfobg[amfnumber].timestramp);
				pheader->m_pamfobg[amfnumber].chunksize=pheader->m_pamfobg[amfnumber].bodysize;
				if(pheader->m_pamfobg[amfnumber].amfcmd==1)
				{
					if(amfheader==0)
					{
						net2bytetoshort(buf+bufpos+14,pheader->m_currentchunksize);
					}
					else
					{
						net2bytetoshort(buf+bufpos+10,pheader->m_currentchunksize);
					}

					//printf("error");
				}
				lastamfnumber=amfnumber;
			}
			else if(amfheader==2)
			{
				net3bytetolong(buf+bufpos+1,pheader->m_pamfobg[amfnumber].timestramp);
	// 			if( pheader->pamfobg[amfnumber].amfcmd==0x08)
	// 			{
	// 				if(pheader->pamfobg[amfnumber].chunksize==0)
	// 					pheader->pamfobg[amfnumber].chunksize=0x41;
	// 			}
				pheader->m_pamfobg[amfnumber].bstart=TRUE;
				if(pheader->m_pamfobg[amfnumber].bodysize==0 && pheader->m_pamfobg[amfnumber].chunksize>0)
				{
					pheader->m_pamfobg[amfnumber].bodysize=pheader->m_pamfobg[amfnumber].chunksize;
				}
				else if(pheader->m_pamfobg[amfnumber].bodysize==0 && pheader->m_pamfobg[amfnumber].chunksize==0)
				{
//					printf("error");
					re=-3;
					break;
				}

			}
			else 
			{
				if(pheader->m_pamfobg[amfnumber].bodysize==0 && pheader->m_pamfobg[amfnumber].chunksize>0)
				{
					pheader->m_pamfobg[amfnumber].bodysize=pheader->m_pamfobg[amfnumber].chunksize;
				}
				else if(pheader->m_pamfobg[amfnumber].bodysize==0 && pheader->m_pamfobg[amfnumber].chunksize==0)
				{
//					printf("error");
					re=-3;
					break;
				}
				//net3bytetolong(buf+bufpos+1,pheader->pamfobg[amfnumber].timestramp);
	// 			if( pheader->pamfobg[amfnumber].amfcmd==0x08)
	// 			{
	// 				if(pheader->pamfobg[amfnumber].chunksize==0)
	// 				pheader->pamfobg[amfnumber].chunksize=0x41;
	// 			}
			}
			
			switch(amfheader)
			{
			case 0:
				headerlen=12;
				break;
			case 1:
				headerlen=8;
				break;
			case 2:
				headerlen=4;
				break;
			case 3:
				headerlen=1;
				break;
			}
			bufpos+=headerlen;
re=2;
			if(amfheader==0 ||amfheader ==1   )
			{
				if(pheader->m_pamfobg[amfnumber].bodysize>pheader->m_currentchunksize)
				{
re=3;
					CopyVideoAudioData(buf+bufpos,pheader->m_currentchunksize,
							m_videobuf,m_videolen,
							m_audiobuf,m_audiolen,pheader->m_pamfobg[amfnumber].amfcmd	);
					pheader->m_pamfobg[amfnumber].bodysize-=pheader->m_currentchunksize;
					bufpos+=pheader->m_currentchunksize;
				}
				else
				{
re=4;
					CopyVideoAudioData(buf+bufpos,pheader->m_pamfobg[amfnumber].bodysize,
							m_videobuf,m_videolen,
							m_audiobuf,m_audiolen,pheader->m_pamfobg[amfnumber].amfcmd);
					bufpos+=pheader->m_pamfobg[amfnumber].bodysize;				
					pheader->m_pamfobg[amfnumber].bodysize=0;

					if(pheader->m_pamfobg[amfnumber].amfcmd==9){
re=5;
						CopyData(m_videobuf,m_videolen,m_tvideobuf,m_tvideolen,pheader->m_pamfobg[amfnumber].timestramp,pheader);
re=6;
						WriteFlashStream(&fs,m_tvideobuf,m_tvideolen,9,pheader);

						m_videolen=0;
						m_tvideolen=0;
					}
					else if(pheader->m_pamfobg[amfnumber].amfcmd==8 )
					{
re=7;
						CopyData(m_audiobuf,m_audiolen,m_taudiobuf,m_taudiolen,pheader->m_pamfobg[amfnumber].timestramp,pheader);
re=8;
						if(m_videolen==0 || m_taudiolen>0x8000){
re=9;
							WriteFlashStream(&fs,m_taudiobuf,m_taudiolen,8,pheader);
							m_taudiolen=0;
						}
						m_audiolen=0;
					}
				}
			}
			else
			{
					if(pheader->m_pamfobg[amfnumber].chunksize>0)
					{
						if(pheader->m_pamfobg[amfnumber].bodysize>pheader->m_currentchunksize)
						{
re=10;
							CopyVideoAudioData(buf+bufpos,pheader->m_currentchunksize,
									m_videobuf,m_videolen,
									m_audiobuf,m_audiolen,pheader->m_pamfobg[amfnumber].amfcmd);
							pheader->m_pamfobg[amfnumber].bodysize-=pheader->m_currentchunksize;
							bufpos+=pheader->m_currentchunksize;
						}
						else if(pheader->m_pamfobg[amfnumber].bodysize>0)
						{
re=11;
							CopyVideoAudioData(buf+bufpos,pheader->m_pamfobg[amfnumber].bodysize,
									m_videobuf,m_videolen,
									m_audiobuf,m_audiolen,pheader->m_pamfobg[amfnumber].amfcmd);
							bufpos+=pheader->m_pamfobg[amfnumber].bodysize;					
							pheader->m_pamfobg[amfnumber].bodysize=0;
							if(pheader->m_pamfobg[amfnumber].amfcmd==9){
re=12;
								CopyData(m_videobuf,m_videolen,m_tvideobuf,m_tvideolen,pheader->m_pamfobg[amfnumber].timestramp,pheader);
re=13;
								WriteFlashStream(&fs,m_tvideobuf,m_tvideolen,9,pheader);
								m_videolen=0;
								m_tvideolen=0;
							}	
							else if(pheader->m_pamfobg[amfnumber].amfcmd==8 )
							{
re=14;
								CopyData(m_audiobuf,m_audiolen,m_taudiobuf,m_taudiolen,pheader->m_pamfobg[amfnumber].timestramp,pheader);
re=15;
								if(m_videolen==0 || m_taudiolen>0x8000){
									//ASSERT(m_taudiolen<=0x8000);
re=16;
									WriteFlashStream(&fs,m_taudiobuf,m_taudiolen,8,pheader);
									m_taudiolen=0;
								}
								m_audiolen=0;
							}
						}
						else
						{
							re=-3;
							break;
							bufpos+=pheader->m_pamfobg[amfnumber].chunksize;
						}
					}
					else
					{
//						printf("error");
						re=-3;
						break;
					}
			
			}

			//amfheader
			
		}
	//保存Flash 对象的数据缓存
		if(m_Responseline.m_audiolen>0){
			delete[]m_Responseline.m_audiobuf;
			m_Responseline.m_audiobuf=0;
		}
		if(m_audiolen>0)
		{
			m_Responseline.m_audiobuf=new UCHAR[m_audiolen];
			memcpy(m_Responseline.m_audiobuf,m_audiobuf,m_audiolen);
		}
		m_Responseline.m_audiolen=m_audiolen;
		///////////////////
		if(m_Responseline.m_videolen>0){
			delete[]m_Responseline.m_videobuf;
			m_Responseline.m_videobuf=0;
		}
		if(m_videolen>0)
		{
			m_Responseline.m_videobuf=new UCHAR[m_videolen];
			memcpy(m_Responseline.m_videobuf,m_videobuf,m_videolen);
		}
		m_Responseline.m_videolen=m_videolen;
		/////////////////////////////////////
		if(m_Responseline.m_taudiolen>0){
			delete[]m_Responseline.m_taudiobuf;
			m_Responseline.m_taudiobuf=0;
		}
		if(m_taudiolen>0)
		{
			m_Responseline.m_taudiobuf=new UCHAR[m_taudiolen];
			memcpy(m_Responseline.m_taudiobuf,m_taudiobuf,m_taudiolen);
		}
		m_Responseline.m_taudiolen=m_taudiolen;
		/////////////////////////////////////
		if(m_Responseline.m_tvideolen>0){
			delete[]m_Responseline.m_tvideobuf;
			m_Responseline.m_tvideobuf=0;
		}
		if(m_tvideolen>0)
		{
			m_Responseline.m_tvideobuf=new UCHAR[m_tvideolen];
			memcpy(m_Responseline.m_tvideobuf,m_tvideobuf,m_tvideolen);
		}
		m_Responseline.m_tvideolen=m_tvideolen;					

		//////////////////////////////////////////
		if(m_Responseline.m_Reservedlen>0){
			delete[]m_Responseline.m_reserverbuf;
			m_Responseline.m_reserverbuf=0;
		}

		if(buflen-bufpos>0)
		{
			m_Responseline.m_reserverbuf=new UCHAR[buflen-bufpos];
			memcpy(m_Responseline.m_reserverbuf,
				buf+bufpos,buflen-bufpos);
		}
		m_Responseline.m_Reservedlen=buflen-bufpos;
		//ok


	}
	catch (...)
	{
		WriteLog(MEDIAPRO, "ParserFlashFormat Error %d  %08x", re, bufpos);
	}
	try{
		if(fs>0 && ftell(fs)>0)
		{
	re=8;
			fseek(fs,0x35,SEEK_SET);
			CHAR durtime[8];
//			printf("filedurtime %d\n",pheader->timestramp/1000);
			doubletonet8byte((UCHAR*)durtime,pheader->m_timestramp/1000);
	re=9;
			fwrite(durtime,1,8,fs);
	re=10;
			fclose(fs);
		}
		else if(fs>0){
			fclose(fs);
//			DeleteFile(fname);
		}

	}
	catch(...)
	{
		WriteLog(MEDIAPRO, "ParserFlashFormat Error = %d", re);	
	}
	return re;
}


int FlashSession::lstToFlvFileOld(UCHAR * buf,UCHAR * savefilebuf, UINT Seq)
{
	int re=0;
	int i=0;
	//////////////////////////////////////////////////////////////////////////

	UINT filebufferlen=0;
	FILE* pubfs=0;
	UINT bittySeq=0		;//
	UCHAR mediabuf[2][MEDIABUFFERSIZE];
	memset(mediabuf[0],0,MEDIABUFFERSIZE*2);
	flashstatus flashdatastatus;
	memset(&flashdatastatus,0,sizeof(flashdatastatus));

	//////////////////////////////////////////////////////////////////////////
	UINT packetendSeq=0;//video
	UINT vediotimestramp=0;
	//flashdata *tmpflahdatahead1;
	//////////////////////////////////////////////////////////////////////////
	UINT mediabuflength[2];
	mediabuflength[0]=0;
	mediabuflength[1]=0;
int error=0;
	//////////////////////////////////////////////////////////////////////////
	//音频 0x54 数据开始 0x3f  0xc4是片断符号 断长0x41


	TcpPacketListIt start,last,itl,tmpitl;
	start=m_Packetlst.begin();
	last=m_Packetlst.end();
	static int l = 0;
	l++;
	if (l == 21)
		l = 21;
	try
	{

	
		while(m_Packetlst.size()>1)
		{
error=0;
			itl=m_Packetlst.begin();
			tmpitl=itl;
			tmpitl++;
			last=m_Packetlst.end();
			m_endfiletm=itl->tm;
error=1;
			if(itl->Nextsequence < m_Responseline.m_fileseq)
			{
				m_Packetlst.pop_front();
				continue;
			}
			if(itl->Ssequence > m_Responseline.m_nseq)
			{
				bittySeq=0;
				memset(buf,0,MEDIABUFFERSIZE);
				memset(mediabuf[0],0,MEDIABUFFERSIZE*2);
				memset(&flashdatastatus,0,sizeof(flashstatus));
				m_Responseline.m_fileseq=itl->Ssequence;
				m_Responseline.m_nseq=itl->Nextsequence;
				mediabuflength[0]=0;
				mediabuflength[1]=0;
#ifdef		_DEBUG
//				printf("Loast Packet %u\n",Responseline.nseq);
#endif
				m_Packetlst.pop_front();
				continue;
			}//endif
			if(bittySeq==0 && m_Packetlst.size()<10)
				break;

error=2;
			if(itl->Ssequence < m_Responseline.m_fileseq)
			{
				memcpy(buf,	itl->buf+m_Responseline.m_fileseq-itl->Ssequence,
					itl->Slen-m_Responseline.m_fileseq+itl->Ssequence);
			}
			else
			{
				memcpy(buf-m_Responseline.m_fileseq+itl->Ssequence, itl->buf, itl->Slen);				
			}

			if(m_Responseline.m_nseq< itl->Nextsequence)
				m_Responseline.m_nseq=itl->Nextsequence;

//		//判断buf的前面是否有 flash头
			if(bittySeq==0)
			{
				if(ParserFlashCommand(buf,flashdatastatus,0))
				{
					//写缓冲区
error=3;
					if(m_Responseline.m_fileseq +12-flashdatastatus.commandtype[0] +flashdatastatus.residual[0]==flashdatastatus.medialength[0])
					{
						memcpy(mediabuf[0],buf+12-flashdatastatus.commandtype[0],flashdatastatus.residual[0]);
						mediabuflength[0]=flashdatastatus.residual[0];
						bittySeq+=12-flashdatastatus.commandtype[0]+flashdatastatus.residual[0];
						if(flashdatastatus.mediatype[0]==8)
						{
							bittySeq+=12-flashdatastatus.commandtype[0]+flashdatastatus.residual[0]	;						
						}
						else
						{
							bittySeq+=12-flashdatastatus.commandtype[0]+flashdatastatus.residual[0];
							packetendSeq=bittySeq;
							flashdatastatus.saveok[0]=TRUE;
							flashdatastatus.saveok[1]=TRUE;
							//ok
						}	
					}
					else
					{
						if(flashdatastatus.mediatype[0]==8)
						{
							bittySeq= m_Responseline.m_fileseq+12-flashdatastatus.commandtype[0]+0x41;
							memcpy(mediabuf[0],buf+12-flashdatastatus.commandtype[0],0x41);
							mediabuflength[0]=0x41;
						}
						else
						{
							bittySeq= m_Responseline.m_fileseq+12-flashdatastatus.commandtype[0]+0x80;
							memcpy(mediabuf[0],buf+12-flashdatastatus.commandtype[0],0x80);
							mediabuflength[0]=0x80;
						}

					}	
error=4;
				}
				else
				{
					int mmm=SearchFlashHeader(buf,itl->Slen-m_Responseline.m_fileseq+itl->Ssequence);
					if(mmm>=0)
					{
						m_Responseline.m_fileseq+=mmm;
						if(itl->Ssequence < m_Responseline.m_fileseq)
						{
							memcpy(buf,	itl->buf+m_Responseline.m_fileseq-itl->Ssequence,
								itl->Slen-m_Responseline.m_fileseq+itl->Ssequence);
						}
						else
						{
							memcpy(buf-m_Responseline.m_fileseq+itl->Ssequence,itl->buf,itl->Slen);				
						}
						ParserFlashCommand(buf,flashdatastatus,0);
						if(m_Responseline.m_fileseq +12-flashdatastatus.commandtype[0] +flashdatastatus.residual[0]==flashdatastatus.medialength[0])
						{
							memcpy(mediabuf[0],buf+12-flashdatastatus.commandtype[0],flashdatastatus.residual[0]);
							mediabuflength[0]=flashdatastatus.residual[0];
							bittySeq+=12-flashdatastatus.commandtype[0]+flashdatastatus.residual[0];
							if(flashdatastatus.mediatype[0]==8)
							{
								bittySeq+=12-flashdatastatus.commandtype[0]+flashdatastatus.residual[0]	;						
							}
							else
							{
								bittySeq+=12-flashdatastatus.commandtype[0]+flashdatastatus.residual[0];
								packetendSeq=bittySeq;
								flashdatastatus.saveok[0]=TRUE;
								flashdatastatus.saveok[1]=TRUE;
								//ok
							}	
						}
						else
						{
							if(flashdatastatus.mediatype[0]==8)
							{
								bittySeq= m_Responseline.m_fileseq+12-flashdatastatus.commandtype[0]+0x41;
								memcpy(mediabuf[0],buf+12-flashdatastatus.commandtype[0],0x41);
								mediabuflength[0]=0x41;
							}
							else
							{
								bittySeq= m_Responseline.m_fileseq+12-flashdatastatus.commandtype[0]+0x80;
								memcpy(mediabuf[0],buf+12-flashdatastatus.commandtype[0],0x80);
								mediabuflength[0]=0x80;
							}
							
						}	

					}
					else
					{
						bittySeq=0;
						memset(buf,0,MEDIABUFFERSIZE);
						memset(mediabuf[0],0,MEDIABUFFERSIZE*2);
						memset(&flashdatastatus,0,sizeof(flashstatus));
						m_Responseline.m_fileseq=itl->Nextsequence;
						m_Responseline.m_nseq=itl->Nextsequence;
						mediabuflength[0]=0;
						mediabuflength[1]=0;
						//下一个包
						m_Packetlst.pop_front();
						continue;
					}

				}

			}
error=5;		
			if(!(flashdatastatus.saveok[0]==1 && flashdatastatus.saveok[0]==1) &&bittySeq>0 )
			{
error=51;
				for(;bittySeq<m_Responseline.m_nseq;)
				{
error=52;
					//下一个包是新的头// (TcpPacket*)(*(UINT *)(&tmpitl)+8)
					if(tmpitl == last)
						break;
					if(::ParserFlashCommand((UCHAR *)tmpitl->buf,TRUE) && m_Responseline.m_nseq==tmpitl->Ssequence)
					{
						//命令0 4 的情况
						//判是否和主 数据段一样
error=53;
						if((UCHAR )tmpitl->buf[7]==flashdatastatus.mediatype[0] 
							|| ((UCHAR )tmpitl->buf[0] & 0xf) ==(flashdatastatus.separator[0] & 0xf) ){
							packetendSeq=m_Responseline.m_nseq;
						}
						else
						{
							//ParserFlashCommand(buf,flashdatastatus,1);
						}
						//packetendSeq=nseq;
					}//endif
error=6;
//////////////////////////////////////////////////////////////////////////
					//是否是主数据段
					if(buf[bittySeq-m_Responseline.m_fileseq]==flashdatastatus.separator[0])
					{
error=7;
						//数据是否结束 
						if(flashdatastatus.medialength[0]==mediabuflength[0]+flashdatastatus.residual[0] &&
							m_Responseline.m_nseq>=bittySeq+((flashdatastatus.residual[0]==0) ? 0 : (flashdatastatus.residual[0]+1)))
						{
							//memcpy
							memcpy(mediabuf[0]+mediabuflength[0],buf+bittySeq-m_Responseline.m_fileseq+ ((flashdatastatus.residual[0]==0) ? 0 : 1),flashdatastatus.residual[0]);
							mediabuflength[0]=mediabuflength[0]+flashdatastatus.residual[0];
							bittySeq+=(flashdatastatus.residual[0]==0) ? 0 : (flashdatastatus.residual[0]+1) ;
							//
							if(bittySeq==packetendSeq)
							{
								//置为成功标志
								flashdatastatus.saveok[0]=TRUE;
								flashdatastatus.saveok[1]=TRUE;
								break;
							}
error=8;
							if(bittySeq==m_Responseline.m_nseq)
								break;
							
							if(::ParserFlashCommand(buf+bittySeq-m_Responseline.m_fileseq,TRUE))
							{
								if((UCHAR )buf[bittySeq-m_Responseline.m_fileseq+7]==flashdatastatus.mediatype[0]					
									|| ((UCHAR )buf[bittySeq-m_Responseline.m_fileseq] & 0xf) ==(flashdatastatus.separator[0] & 0xf) )
								{
									packetendSeq=bittySeq;
									//置为成功标志
									flashdatastatus.saveok[0]=TRUE;
									flashdatastatus.saveok[1]=TRUE;
									break;
								}
								else
								{
									ParserFlashCommand(buf+bittySeq-m_Responseline.m_fileseq,flashdatastatus,1);
									if(flashdatastatus.mediatype[1]==8)
									{
										bittySeq=bittySeq+12-flashdatastatus.commandtype[1]+0x41;
										memcpy(mediabuf[1],buf+bittySeq-m_Responseline.m_fileseq,0x41);
										mediabuflength[1]=0x41;
									}
									else
									{
										bittySeq=bittySeq+12-flashdatastatus.commandtype[1]+0x80;
										memcpy(mediabuf[1],buf+bittySeq-m_Responseline.m_fileseq,0x80);
										mediabuflength[1]=0x80;
									}
									continue;
								}
error=9;
							}
							else if( m_bfromserver!=3 && buf[bittySeq-m_Responseline.m_fileseq]==flashdatastatus.separator[1] )
							{
								continue;
							}
							else if(m_bfromserver==3 && (buf[bittySeq-m_Responseline.m_fileseq]==m_AVseparator[0] || buf[bittySeq-m_Responseline.m_fileseq]==m_AVseparator[1]) ){
								if(buf[bittySeq-m_Responseline.m_fileseq]==m_AVseparator[0]){
									//下行
									m_bfromserver=1;
									SWAPinfo();
									flashdatastatus.separator[1]=m_AVseparator[0];
								}
								else if (buf[bittySeq-m_Responseline.m_fileseq]==m_AVseparator[1]){
									//上行
									m_bfromserver=0;
									flashdatastatus.separator[1]=m_AVseparator[1];
								}
							}
							else
							{
								//退出循环代码
								bittySeq=0;
								m_Responseline.m_fileseq=m_Responseline.m_nseq;
								mediabuflength[0]=0;
								mediabuflength[1]=0;
								memset(buf,0,MEDIABUFFERSIZE);
								memset(mediabuf[0],0,MEDIABUFFERSIZE*2);
								memset(&flashdatastatus,0,sizeof(flashstatus));
								//初始化							
								break;		
							}
							//
							
						}//数据是否结束 
error=10;
						//根据余数
						if(m_Responseline.m_nseq-((flashdatastatus.mediatype[0]!=8)?0x81:0x42) < bittySeq 
							&& flashdatastatus.medialength[0]>0)
							break;
						if(bittySeq==m_Responseline.m_nseq)
							continue;
						//memcpy
						memcpy(mediabuf[0]+mediabuflength[0],buf+bittySeq-m_Responseline.m_fileseq+1,(flashdatastatus.mediatype[0]!=8)?0x80:0x41);
						bittySeq+=(flashdatastatus.mediatype[0]!=8)?0x81:0x42;
						mediabuflength[0]+=(flashdatastatus.mediatype[0]!=8)?0x80:0x41;	
						if(packetendSeq==bittySeq)
						{
							flashdatastatus.saveok[0]=TRUE;
							flashdatastatus.saveok[1]=TRUE;
							break;
						}
							//savepacket=TRUE;
						continue;
					}//end 主数据段
//////////////////////////////////////////////////////////////////////////
					//次数据段
					else if
					((m_bfromserver!=3 && 	buf[bittySeq-m_Responseline.m_fileseq]==flashdatastatus.separator[1]) ||
						(m_bfromserver==3 && (buf[bittySeq-m_Responseline.m_fileseq]==m_AVseparator[0] ||
											buf[bittySeq-m_Responseline.m_fileseq]==m_AVseparator[1]) )
					)
					{
						if(m_bfromserver==3 && buf[bittySeq-m_Responseline.m_fileseq]==m_AVseparator[0]){
							//下行
							m_bfromserver=1;
							SWAPinfo();
							flashdatastatus.separator[1]=m_AVseparator[0];
						}
						else if (m_bfromserver==3 && buf[bittySeq-m_Responseline.m_fileseq]==m_AVseparator[1]){
							//上行
							m_bfromserver=0;
							flashdatastatus.separator[1]=m_AVseparator[1];
						}
						//次数据无长度
						if(m_Responseline.m_nseq-((flashdatastatus.mediatype[1]!=8)?0x81:0x42) < bittySeq 
							&& flashdatastatus.medialength[1]==0)
							break;	
error=11;
						//次数据有长度
						if(flashdatastatus.medialength[1]==mediabuflength[1]+flashdatastatus.residual[1] &&
							m_Responseline.m_nseq>=bittySeq+((flashdatastatus.residual[1]==0) ? 0 : (flashdatastatus.residual[1]+1))
							&& flashdatastatus.medialength[1]>0)
						{
							
							//memcpy
							memcpy(mediabuf[1]+mediabuflength[1],buf+bittySeq-m_Responseline.m_fileseq+ ((flashdatastatus.residual[1]==0) ? 0 : 1),flashdatastatus.residual[1]);
							mediabuflength[1]=mediabuflength[1]+flashdatastatus.residual[1];
							bittySeq+=(flashdatastatus.residual[1]==0) ? 0 : (flashdatastatus.residual[1]+1) ;
							if(bittySeq==packetendSeq)
							{
								//置为成功标志
								flashdatastatus.saveok[0]=TRUE;
								flashdatastatus.saveok[1]=TRUE;
								break;
							}
error=12;
							if(bittySeq==m_Responseline.m_nseq)
								break;
							//
							if(::ParserFlashCommand(buf+bittySeq-m_Responseline.m_fileseq,TRUE))
							{
error=13;
								if((UCHAR )buf[bittySeq-m_Responseline.m_fileseq+7]==flashdatastatus.mediatype[0]					
									|| ((UCHAR )buf[bittySeq-m_Responseline.m_fileseq] & 0xf) ==(flashdatastatus.separator[0] & 0xf) ){
									packetendSeq=bittySeq;
									//置为成功标志
									flashdatastatus.saveok[0]=TRUE;
									flashdatastatus.saveok[1]=TRUE;
									break;
								}
								else
								{
									ParserFlashCommand(buf+bittySeq-m_Responseline.m_fileseq,flashdatastatus,1);
									if(flashdatastatus.mediatype[1]==8){
										bittySeq=bittySeq+12-flashdatastatus.commandtype[1]+0x41;
										memcpy(mediabuf[1],buf+bittySeq-m_Responseline.m_fileseq,0x41);
										mediabuflength[1]=0x41;
									}
									else{
										bittySeq=bittySeq+12-flashdatastatus.commandtype[1]+0x80;
										memcpy(mediabuf[1],buf+bittySeq-m_Responseline.m_fileseq,0x80);
										mediabuflength[1]=0x80;
									}
									continue;
								}
							}
							else if( m_bfromserver!=3 && buf[bittySeq-m_Responseline.m_fileseq]==flashdatastatus.separator[1] )
							{
								continue;
							}
							else if(m_bfromserver==3 && (buf[bittySeq-m_Responseline.m_fileseq]==m_AVseparator[0] || buf[bittySeq-m_Responseline.m_fileseq]==m_AVseparator[1]) ){
								if(buf[bittySeq-m_Responseline.m_fileseq]==m_AVseparator[0]){
									//下行
									m_bfromserver=1;
									SWAPinfo();
									flashdatastatus.separator[1]=m_AVseparator[0];
								}
								else if (buf[bittySeq-m_Responseline.m_fileseq]==m_AVseparator[1]){
									//上行
									m_bfromserver=0;
									flashdatastatus.separator[1]=m_AVseparator[1];
								}
								continue;
							}
//							else if(buf[bittySeq-fileseq]==flashdatastatus.separator[1] ){
//								continue;
//							}
							else
							{
								//退出循环代码
								bittySeq=0;
								memset(buf,0,MEDIABUFFERSIZE);
								m_Responseline.m_fileseq=m_Responseline.m_nseq;
								mediabuflength[0]=0;
								mediabuflength[1]=0;
								memset(mediabuf[0],0,MEDIABUFFERSIZE*2);
								memset(&flashdatastatus,0,sizeof(flashstatus));
								//初始化							
								break;		
							}
							//

							
						}
error=14;
						if(bittySeq==m_Responseline.m_nseq)
							continue;
						//次数据有长度
						if(m_Responseline.m_nseq-((flashdatastatus.mediatype[1]!=8)?0x81:0x42) < bittySeq 
							&& flashdatastatus.medialength[1]>0)
							break;	
						//memcpy
error=141;
						memcpy(mediabuf[1]+mediabuflength[1],buf+bittySeq-m_Responseline.m_fileseq+1,(flashdatastatus.mediatype[1]!=8)?0x80:0x41);
						bittySeq+=(flashdatastatus.mediatype[1]!=8)?0x81:0x42;
						mediabuflength[1]+=(flashdatastatus.mediatype[1]!=8)?0x80:0x41;	
						if(packetendSeq==bittySeq){
							flashdatastatus.saveok[0]=TRUE;
							flashdatastatus.saveok[1]=TRUE;
							break;

						}
						continue;

					}//次数据段
//////////////////////////////////////////////////////////////////////////
error=15;
					ASSERT(bittySeq-m_Responseline.m_fileseq< 1000000);
					//判是否结束 //主数据是否length ok ...
					if(::ParserFlashCommand(buf+bittySeq-m_Responseline.m_fileseq,TRUE))
					{
error=16;
						if((UCHAR )buf[bittySeq-m_Responseline.m_fileseq+7]==flashdatastatus.mediatype[0]					
							|| ((UCHAR )buf[bittySeq-m_Responseline.m_fileseq] & 0xf) ==(flashdatastatus.separator[0] & 0xf) )
						{
							packetendSeq=bittySeq;
							//置为成功标志
							flashdatastatus.saveok[0]=TRUE;
							flashdatastatus.saveok[1]=TRUE;
							break;
						}
						else
						{
							ParserFlashCommand(buf+bittySeq-m_Responseline.m_fileseq,flashdatastatus,1);
							if(flashdatastatus.mediatype[1]==8)
							{
								bittySeq=bittySeq+12-flashdatastatus.commandtype[1]+0x41;
								memcpy(mediabuf[1],buf+bittySeq-m_Responseline.m_fileseq,0x41);
								mediabuflength[1]=0x41;
							}
							else
							{
								bittySeq=bittySeq+12-flashdatastatus.commandtype[1]+0x80;
								memcpy(mediabuf[1],buf+bittySeq-m_Responseline.m_fileseq,0x80);
								mediabuflength[1]=0x80;
							}
							continue;
						}
					}


					flashdatastatus.saveok[0]=FALSE;
					flashdatastatus.saveok[1]=FALSE;
					//退出循环代码
					bittySeq=0;
					memset(buf,0,MEDIABUFFERSIZE);
					m_Responseline.m_fileseq=m_Responseline.m_nseq;
					mediabuflength[0]=0;
					mediabuflength[1]=0;
					memset(&flashdatastatus,0,sizeof(flashstatus));
					memset(mediabuf[0],0,MEDIABUFFERSIZE*2);
					//PacketCount--;
					break;
				}// end for
			}//end if
error=17;
			//可以写文件
			if(flashdatastatus.saveok[0] && flashdatastatus.saveok[1] )
			{
				//写文件...
				if(m_realfname.IsEmpty())
				{
					m_realfname = CreateMediaFilename("flv", m_sip, m_dip);
					if (m_realfname.IsEmpty())
					{
						return -2;
					}
				}
error=18;
				if(pubfs==0)
				{	
					pubfs=fopen(m_realfname,"a+b");
					if(pubfs==0)
					{
						WriteLog(MEDIAPRO, "Open File %s Error!!", m_realfname);
						break;
					}
//					printf("open file:%08X\t", (DWORD)pubfs);
				}
				//初始化变量...
				if(flashdatastatus.mediatype[0]==9  )
				{
					::WriteFlashStream(pubfs,
						mediabuf[0],
						mediabuflength[0],
						flashdatastatus.medialength[0],
						flashdatastatus.mediatype[0],
						((flashdatastatus.commandtype[0]!=0)?flashdatastatus.timestram[0]:0x20),
						&m_Responseline);

				}
error=19;
				if( flashdatastatus.mediatype[1]==9 )
				{
					::WriteFlashStream(pubfs,mediabuf[1],mediabuflength[1],flashdatastatus.medialength[1],
						flashdatastatus.mediatype[1],((flashdatastatus.commandtype[1]!=0)?flashdatastatus.timestram[1]:0x20),&m_Responseline);
				}
error=191;
				if( flashdatastatus.mediatype[0]==8 )
				{
					::WriteFlashStream(pubfs,mediabuf[0],mediabuflength[0],flashdatastatus.medialength[0],
						flashdatastatus.mediatype[0],((flashdatastatus.commandtype[0]!=0)?flashdatastatus.timestram[0]:0x20),&m_Responseline);
					
				}
error=192;
				if(flashdatastatus.mediatype[1]==8 )
				{
					::WriteFlashStream(pubfs,mediabuf[1],mediabuflength[1],flashdatastatus.medialength[1],
						flashdatastatus.mediatype[1],((flashdatastatus.commandtype[1]!=0)?flashdatastatus.timestram[1]:0x20),&m_Responseline);
				}
error=20;			
				//初始化变量
				//判断是否为 8x
				if(buf[bittySeq-m_Responseline.m_fileseq]>>4!=8){
					memset(&flashdatastatus,0,sizeof(flashstatus));
				}
				flashdatastatus.saveok[0]=FALSE;
				flashdatastatus.saveok[1]=FALSE;
				m_Responseline.m_fileseq=packetendSeq;
error=200;
				bittySeq=0;
				memset(buf,0,MEDIABUFFERSIZE);
				mediabuflength[0]=0;
				mediabuflength[1]=0;
				memset(mediabuf[0],0,MEDIABUFFERSIZE*2);
error=21;
				if(itl->Nextsequence <= m_Responseline.m_fileseq)
				{
					m_Packetlst.pop_front();
					//printf("\n");
				}
				else if(itl->Ssequence <=  m_Responseline.m_fileseq)
				{
					memcpy(buf ,
						itl->buf+  m_Responseline.m_fileseq -itl->Ssequence,
						itl->Slen- m_Responseline.m_fileseq+itl->Ssequence);
					m_Packetlst.pop_front();
				}
				else
				{
					m_Packetlst.pop_front();
					//printf("\n");
					//memcpy(buf -  fileseq+itl->Ssequence,itl->buf,itl->Slen);				
				}

				if(m_Packetlst.size()<=50)
					break;
			}
			else
			{
error=22;
				m_Packetlst.pop_front();
			}
		}
	}
	catch (...)
	{
		WriteLog(MEDIAPRO, "LstToFlvFile() Error=%d(file=%08X)!!", error, (DWORD)pubfs);
		if(error==22)
			return 1;
		return -3;
	}
	if(pubfs>0)
	{
//		printf("Close file:%08X\t", (DWORD)pubfs);
		fclose(pubfs);
		pubfs = 0;
	}
	
	//每5分钟写一个文件
// 	if(currenttm>m_Responseline.m_Stm+v_mediainterval*60 && m_Responseline.m_filename!="")
// 	{
// 		l=0;
// 		//写对照文件
// 		if(GetFileLength(Responseline.filename)>100000)
// 		{
// 			WriteFlashIndexFIle(this, currenttm);
// 			//copy 对照文件和视频文件
// 			if (!CaseID.IsEmpty() || (!rubbishflag && v_openflagFlashMedia))
// 			{
// 				CStringArray attfiles;
// 				CopyFileToTmpdir(CaseID, Responseline.filename, Responseline.indexfilename, attfiles,
// 					pthisMissObj->m_nowstm, pthisMissObj->m_ddefiletype, pthisMissObj->m_ServerPower,
// 					(v_openflagFlashMedia)? pthisMissObj->m_ServerPower+1 : NULL,
// 					ruleidarr,fenleiarr,levelarr,cardarr,namearr,peopletypearr,
// 					peoplearr,keytypearr,IsOnlyAlarmArr);
// 				
// 			}
// 		}
// 		//建立新的pResponseline		//变量初始化
// 		Responseline.Clear();
// 		Responseline.Stm=currenttm;
// 		Responseline.Stm1=currenttm1;
// 	}

	return re;	
}

void FlashSession::SWAPinfo()
{
	// 交换IP方向
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

	m_MediaDirection = _T("<<==");
}

