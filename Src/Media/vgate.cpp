
//*****************************************************************************
// 语音网关分析
//*****************************************************************************

#include "stdafx.h"
#include "MediaSession.h"
#include "media.h"
#include "vgate.h"
#include "VoipMedia.h"

void WriteVGateIndexFile(DATEHEADER& dateheader, LPCTSTR spyaddr, int PacketFrom,
		LPCTSTR formtelno, LPCTSTR totelno, LPCTSTR useragent, 
		VOIPStruct* pvoipinfo=NULL)
{
/*	WarnInfo warninfo;
	
	// sip 是语音网关
	char indexfile[200];
	char nowtm[40];
	SocketSession tmpsession(dateheader, PacketFrom);
	SearchCaseForSession(&tmpsession, v_opencasevgate, dateheader.tm);
	if (v_opencasevgate && tmpsession.CaseID.IsEmpty())
	{
		CString str;
		if (pvoipinfo == NULL)
			str.Format("%s\1%s\1%s", formtelno, totelno, useragent);
		else
			str.Format("%s\1%s\1%s\1%s\1%s", formtelno, totelno, useragent,
				pvoipinfo->calledtype, pvoipinfo->callingtype);
		CaseInfo csinfo;
		if (SearchCaseByKey(str, str.GetLength(), csinfo))
		{
			tmpsession.CaseID = csinfo.CaseID;
			tmpsession.LimitAddr = csinfo.LimitAddr;
			tmpsession.LimitType = csinfo.LimitType;
			tmpsession.ObjectID = csinfo.ObjectID;
		}
		else
		{
			if (tmpsession.rubbishflag || !v_openflagvgate)
				return;
		}
	}
	
	CString tmppos;
	AAAInfo pAaaInfo;
	if(GetProbeAAAInfo(tmpsession.sip,tmpsession.dip,tmpsession.smac,tmpsession.dmac,&pAaaInfo,tmpsession.ipidarr,
		tmppos))
	{

	}
	else
		tmppos.Format("%s",spyaddr);

	// 开始写对照文件
	GetTimeString(dateheader.tm, "%Y%m%d%H%M%S", nowtm, 40);
	sprintf(indexfile, "%s%s_%s_%s_%u.vgate.indexfile.txt", MediaTmpPath,
		nowtm, tmpsession.sip, tmpsession.dip, dateheader.tm1+rand());
	FILE *fp = fopen(indexfile, "wb");
	if (fp != NULL)
	{
		GetTimeString(dateheader.tm, "%Y-%m-%d %H:%M:%S", nowtm, 40);
		CString tmpsip;
		CString tmpdip;
		tmpsip = tmpsession.sip.Left(15);
		tmpdip = tmpsession.dip.Left(15);
		BOOL isReal;
		int ltype;
		if (tmpsession.LimitType > 200)
		{
			AddRealIP(tmpsip, tmpsession.CaseID, tmpsession.LimitAddr, tmpsession.LimitType);
			ltype = tmpsession.LimitType - 200;
			isReal = FALSE;
		}
		else if (tmpsession.LimitType > 100)
		{
			ltype = tmpsession.LimitType - 100;
			isReal = TRUE;
		}
		else
		{
			ltype = tmpsession.LimitType;
			isReal = FALSE;
		}
		if (tmpsession.CaseID.IsEmpty())
		{
			SearchIsWarnData(tmpsession.UserAccount1, tmpsession.sip.Left(15),tmpsession.smac,NULL, 0, warninfo,tmpsession.sip.Left(15), "vgate");
		}
		
		WriteWarnAlarmArrToData(MEDIA_PROTOCOL,
			warninfo.fenleiarr,warninfo.ruleidarr,warninfo.peopletypearr,
			warninfo.peoplearr,warninfo.namearr,warninfo.cardarr,
			warninfo.keytypearr,nowtm,"","vgate",
			warninfo.levelarr,nowtm,nowtm,spyaddr,warninfo.dummytypeArr);
			
		tmpsip.Replace(".","_");
		tmpdip.Replace(".","_");
		CString sipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)tmpsession.csip)));
		CString dipaddr = pthisMissObj->QueryIpAddr(Swap4Byte(*((DWORD*)tmpsession.cdip)));
		WriteIndexfileField(fp, "[1]\r\npro", "vgate");
		WriteIndexfileField(fp, "spyaddr",	tmppos);
		WriteIndexfileField(fp, "tm",		nowtm);
		if (pvoipinfo == NULL)
		{
			WriteIndexfileField(fp, "sip",		tmpdip);
			WriteIndexfileField(fp, "sport",	tmpsession.dip.Right(5));
			WriteIndexfileField(fp, "dip",		tmpsip);
			WriteIndexfileField(fp, "dport",	tmpsession.sip.Right(5));
			WriteIndexfileField(fp, "sipaddr",	dipaddr);
			WriteIndexfileField(fp, "dipaddr",	sipaddr);
			WriteIndexfileField(fp, "datasrc",	tmpsession.m_dataSrc);
			WriteIndexfileField(fp, "ISP",		tmpsession.m_strISP);
			WriteIndexfileField(fp, "smac",		tmpsession.dmac);
			WriteIndexfileField(fp, "dmac",		tmpsession.smac);
		}
		else
		{
			WriteIndexfileField(fp, "sip",		tmpsip);
			WriteIndexfileField(fp, "sport",	tmpsession.sip.Right(5));
			WriteIndexfileField(fp, "dip",		tmpdip);
			WriteIndexfileField(fp, "dport",	tmpsession.dip.Right(5));
			WriteIndexfileField(fp, "sipaddr",	sipaddr);
			WriteIndexfileField(fp, "dipaddr",	dipaddr);
			WriteIndexfileField(fp, "datasrc",	tmpsession.m_dataSrc);
			WriteIndexfileField(fp, "ISP",		tmpsession.m_strISP);
			WriteIndexfileField(fp, "smac",		tmpsession.smac);
			WriteIndexfileField(fp, "dmac",		tmpsession.dmac);
		}
		if (!tmpsession.CaseID.IsEmpty())
		{
			WriteIndexfileField(fp, "caseid",	tmpsession.CaseID);
			WriteIndexfileField(fp, "objid",	tmpsession.ObjectID);
			WriteIndexfileField(fp, "limitaddr",tmpsession.LimitAddr);
			WriteIndexfileField(fp, "limittype",ltype);
		}
		WriteIndexfileField(fp, "account", tmpsession.UserAccount1);
		WriteIndexfileField(fp, "phone", tmpsession.Phone1);
		WriteIndexfileField(fp, "2account", tmpsession.UserAccount2);
		WriteIndexfileField(fp, "2phone", tmpsession.Phone2);
		WriteIndexfileField(fp, "主叫", formtelno);
		WriteIndexfileField(fp, "被叫", totelno);
		if (pvoipinfo != NULL)
		{
			WriteIndexfileField(fp, "vgateip1", tmpdip);
			WriteIndexfileField(fp, "主叫号码类型", pvoipinfo->callingtype);
			WriteIndexfileField(fp, "被叫号码类型", pvoipinfo->calledtype);
			WriteIndexfileField(fp, "显示号码", pvoipinfo->displaynumber);
		}
		else
		{
			WriteIndexfileField(fp, "vgateip1", tmpsip);
		}
		if (isReal)
			WriteIndexfileField(fp,"扩展", "TRUE");
		fclose(fp);
		
		if (!tmpsession.CaseID.IsEmpty() || (!tmpsession.rubbishflag && v_openflagvgate))
		{
			CStringArray attfile;
			CopyFileToTmpdir(tmpsession.CaseID, "", indexfile, attfile,
				pthisMissObj->m_nowstm, "vgate", pthisMissObj->m_ServerPower,
				(v_openflagvgate)? pthisMissObj->m_ServerPower+1 : NULL,
				warninfo.ruleidarr,warninfo.fenleiarr,warninfo.levelarr,warninfo.cardarr,warninfo.namearr,warninfo.peopletypearr,
				warninfo.peoplearr,warninfo.keytypearr,warninfo.IsOnlyAlarmArr,warninfo.dummytypeArr);
			
		}
		DeleteFile(indexfile);
	}*/
}

// 处理sip INVITE包   fixpro=1为wcall
BOOL ProcessSipPacket(DATEHEADER& dateheader, char* tmpbuf, LPCTSTR spyaddr, int PacketFrom,int fixpro=0)
{
 	CString formtelno;
	CString totelno;
	CString useragent;
	CString domain;
	CString domain1;
	if (strstr(tmpbuf, "SIP/2.0")>0 )
	{
		// 获取源电话号码
		char *tp1 = strstr(tmpbuf, "From: <sip:");
		if (tp1 == NULL)
			return FALSE;
		tp1 += strlen("From: <sip:");
		
		char *tp2 = strchr(tp1, '@');
		if (tp2 == NULL)
		{
			return FALSE;
		}

		char * tp3 = strchr(tp2,'>');
		if (tp3 == NULL)
		{
			return FALSE;
		}

		if((tp2-tp1>0) && (tp3 - tp2 -1 > 0))
		{
			formtelno  = tp1; //打电话的号码
			formtelno = formtelno.Left(tp2 - tp1);
			domain =tp2 + 1; //域
			domain = domain.Left(tp3 - tp2 -1);
		}
		else
		{
			return FALSE;
		}

		// 获取目的电话号码
		tp1 = strstr(tp3, "To: <sip:");
		if (tp1 == NULL)
			return FALSE;
		tp1 += strlen("To: <sip:");
		
		tp2 = strchr(tp1, '@');
		if (tp2 == NULL)
		{
			return FALSE;
		}
		
		tp3 = strchr(tp2,'>');
		if (tp3 == NULL)
		{
			return FALSE;
		}
		
		if((tp2-tp1>0) && (tp3 - tp2 -1 > 0))
		{
			totelno = tp1; //打电话的号码
			totelno = totelno.Left(tp2 - tp1);
			domain1 =tp2 + 1; //域
			domain1 = domain1.Left(tp3 - tp2 -1);
		}
		else
		{
			return FALSE;
		}		

		// 获取useragent
		tp1 = strstr(tmpbuf, "User-Agent:");
		if (tp1!=NULL)
		{
			tp1 += strlen("User-Agent:");
			tp2 = strstr(tp1, "\r\n");
			if (tp2!=NULL)
			{
				useragent = tp1;
				useragent = useragent.Left(tp2 - tp1);
			}	
			else
				useragent = tp1+11;
		}

		// 写对照文件
		// WriteVGateIndexFile(DataHeader, spyaddr, PacketFrom, formtelno, totelno, useragent);
		SocketSession vgatesession(dateheader, PacketFrom);
		vgatesession.m_OpenIndxFlg = v_openflagrtsp;
		vgatesession.m_OpenCaseFlg = v_opencasertsp;
		vgatesession.m_OpenWarnFlg = v_openwarnrtsp;

		CString text;	// 需要匹配案件和预警的串
		text.Format("%s\r\n\r\n%s\r\n\r\n%s", formtelno, totelno, useragent);
		// 匹配案件和预警
		DWORD dwState = 0;
		SearchCaseOrWarn(&vgatesession, text, text.GetLength(), dwState);

		// 处理IDC数据
		// 写活动历史
		// 写对照文件并输出
// 		WriteIndexfileField(fp, "主叫", formtelno);
// 		WriteIndexfileField(fp, "被叫", totelno);
// 		if (pvoipinfo != NULL)
// 		{
// 			WriteIndexfileField(fp, "vgateip1", tmpdip);
// 			WriteIndexfileField(fp, "主叫号码类型", pvoipinfo->callingtype);
// 			WriteIndexfileField(fp, "被叫号码类型", pvoipinfo->calledtype);
// 			WriteIndexfileField(fp, "显示号码", pvoipinfo->displaynumber);
// 		}
// 		else
// 		{
// 			WriteIndexfileField(fp, "vgateip1", tmpsip);
// 		}

		//目的ip为关守
		text.Format("主叫= %s\r\n被叫= %s\r\nvgateip1= %s\r\nvoippro=sip\r\nuseragent=%s\r\n",
			formtelno,totelno,vgatesession.m_dip.Left(15),useragent);
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmpdir = g_MediaMissObj->GetTmpPath();		
		vgatesession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_vgate.indexfile.txt",
			tmpdir, now.wYear, now.wMonth, now.wDay,
			now.wHour, now.wMinute, now.wSecond, vgatesession.m_sip, vgatesession.m_dip, ++g_MediaFileNum);
		vgatesession.m_spyaddr = spyaddr;

		/*
		  user-agent: Star-Net 为福州星网锐捷
		  user-agent: eyeBeam release 1011d 为中港任意显	
		  fixpro=1 为wcall 
		  domain  为www.diicall.com 的为diicall
		 */

		CString pro;

		if(domain.Find("diicall.com")>0 || domain1.Find("diicall.com")>0 )
		{
			pro="voip_diicall";
		}
		else if(fixpro==1)
		{
			pro="voip_wcall";
		}

		else if(useragent.Find("aaa")>=0)
		{
			pro="media_voip_kc";
		}
		else if(useragent.Find("IDingLing")>=0)
		{
			pro="media_voip_kc";
		}
		else if (useragent.Find("QQVoice") >= 0)
		{
			pro = "media_voip_yt";
		}
		else if (useragent.Find("QfishPhone") >= 0)
		{
			pro = "media_voip_alcall";
		}
		else if (useragent.Find("hhcall.com") >= 0)
		{
			pro = "media_voip_hhcall";
		}
		else if (useragent.Find("hhcall.sip") >= 0)
		{
			pro = "media_voip_hhcall";
		}
		else if(useragent.Find("star-net")>=0)
		{
			pro="voip_starnet";
		}
		else if(useragent.Find("eyebeam")>=0)
		{
			pro="voip_eyebeam";
			 
		}
		else if(useragent.Find("wingizmo")>=0)
		{
			pro="voip_gizmo";
		 
		}
		else if(useragent.Find("kc phone")>=0)
		{
			pro="voip_mtalk";
		 
		}
		else if(useragent.Find("mrtalk")>=0)
		{
			pro="voip_mrtalk";
		}
		else //其它
		{
			pro="vgate";
			
		}

		vgatesession.WriteIndexFile(pro, pro, text, g_MediaMissObj);
		// 输出活动历史
		if (v_statImsgOther)
		{
			HistoryRecord hisRec;
 			hisRec.SetRecord(IDX_FORWTYPE,	"1");
 			hisRec.SetRecord(IDX_ID,		formtelno);
			hisRec.SetRecord(IDX_CONTACT,		totelno);
			hisRec.SetRecord(IDX_STATE, "通迅");
			vgatesession.WriteHisData(pro, pro, &hisRec, g_MediaMissObj,MEDIA_PROTOCOL);//, HISTORY_DATA);
		}	 
		return TRUE;
	}
	return FALSE;
}





inline CString GetPhoneNoType(BYTE type)
{
	if(type==0x81)
	{
		return "E.164 ISDN/telephony numbering";
	}
	else if(type==0xa1)
	{
		return "E.164 ISDN/telephony numbering National Number";
//		CString tempstr;
//		tempstr=" E.164 ISDN/telephony numbering "+'\1' ;
//		tempstr+="National Number";
//		return tempstr;
	}
	else 
		return "";
}





BOOL SplitH225_IE(UCHAR *buf, int len, VOIPStruct& voipinfo)
{
	if(len<3)
		return FALSE;
	int paketlen=0;
	int tmpoffset=0; //偏移
	if(buf[0]==UserUserIE)
	{
		paketlen=*(WORD*)(buf+1);
		tmpoffset=3;
	}
	else
	{
		paketlen=(int)buf[1];
		tmpoffset=2;
	}
	if(paketlen<len)
	{
		// 获取电话信息
		if (CallingPartyNumberIE == (InformationElementCodes)buf[0])
		{
			voipinfo.callingtype = GetPhoneNoType(buf[tmpoffset]);
//			char *tp = voipinfo.callingnumber.GetBufferSetLength(paketlen-1);
//			memcpy(tp, buf+tmpoffset+1, paketlen-1);
			voipinfo.callingnumber= CString((LPCTSTR)(buf+tmpoffset+1), paketlen-1);
		}
		else if(CalledPartyNumberIE == (InformationElementCodes)buf[0])
		{
			voipinfo.calledtype=GetPhoneNoType(buf[tmpoffset]);
//			char *tp = voipinfo.callednumber.GetBufferSetLength(paketlen-1);
//			memcpy(tp, buf+tmpoffset+1, paketlen-1);
			voipinfo.callednumber= CString((LPCTSTR)(buf+tmpoffset+1), paketlen-1);
		}
		else if(DisplayIE == (InformationElementCodes)buf[0])
		{
//			char *tp = voipinfo.displaynumber.GetBufferSetLength(paketlen);
//			memcpy(tp, buf+tmpoffset, paketlen);
			voipinfo.displaynumber= CString((LPCTSTR)(buf+tmpoffset), paketlen);
		}
		SplitH225_IE(buf+tmpoffset+paketlen, len-tmpoffset-paketlen, voipinfo);
	}
	else
		return FALSE;
	return TRUE;
}




// 处理h.225 setup包
BOOL ProcessH225Packet(DATEHEADER& dateheader, char* pPacket, LPCTSTR spyaddr, int PacketFrom)
{
	VOIPStruct voipinfo;
	if (pPacket[9] == 4)
	{
		if(!SplitH225_IE((BYTE*)(pPacket+9),dateheader.m_datalen, voipinfo))
			return FALSE;
	}
	else
	{
		if(!SplitH225_IE((BYTE*)(pPacket+10),dateheader.m_datalen, voipinfo))
			return FALSE;
	}
	if (voipinfo.callednumber.IsEmpty() && voipinfo.callingnumber.IsEmpty())
		return FALSE;

// 	WriteVGateIndexFile(dateheader, spyaddr, PacketFrom, voipinfo.callingnumber,voipinfo.callednumber,
// 		"",  &voipinfo);

	SocketSession vgatesession(dateheader, PacketFrom);

	vgatesession.m_OpenIndxFlg = v_openflagrtsp;
	vgatesession.m_OpenCaseFlg = v_opencasertsp;
	vgatesession.m_OpenWarnFlg = v_openwarnrtsp;

	char text[2000];
	int len = 0;
	// 生成需要匹配案件和预警的串
	len = sprintf(text, "%s\r\n\r\n%s\r\n\r\n%s\r\n\r\n%s", voipinfo.callingnumber,
		voipinfo.callednumber, voipinfo.calledtype, voipinfo.callingtype);

	// 匹配案件和预警
	DWORD dwState = 0;
	SearchCaseOrWarn(&vgatesession, text, len, dwState);

	// 处理IDC数据
	if (vgatesession.m_dataSrc == DATASRC_OTHER)
	{
		if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)vgatesession.m_cdip),
				vgatesession.m_strISP)
			|| (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)vgatesession.m_csip),
					vgatesession.m_strISP)))
		{
			vgatesession.m_dataSrc = DATASRC_IDC;
		}
	}

	// 写活动历史

	// 写对照文件并输出
// 		WriteIndexfileField(fp, "主叫", formtelno);
// 		WriteIndexfileField(fp, "被叫", totelno);
// 		if (pvoipinfo != NULL)
// 		{
// 			WriteIndexfileField(fp, "vgateip1", tmpdip);
// 			WriteIndexfileField(fp, "主叫号码类型", pvoipinfo->callingtype);
// 			WriteIndexfileField(fp, "被叫号码类型", pvoipinfo->calledtype);
// 			WriteIndexfileField(fp, "显示号码", pvoipinfo->displaynumber);
// 		}
// 		else
// 		{
// 			WriteIndexfileField(fp, "vgateip1", tmpsip);
// 		}

	len = 0;
	if(voipinfo.callingnumber.IsEmpty())
		len += WriteField(text + len, "主叫",			voipinfo.callingnumber);
	else 
		len += WriteField(text + len, "主叫",			voipinfo.displaynumber);
	len += WriteField(text + len, "被叫",			voipinfo.callednumber);
	len += WriteField(text + len, "vgateip1",		vgatesession.m_dip.Left(15));
	len += WriteField(text + len, "主叫号码类型",	voipinfo.callingtype);
	len += WriteField(text + len, "被叫号码类型",	voipinfo.calledtype);
	len += WriteField(text + len, "显示号码",		voipinfo.displaynumber);
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmpdir = g_MediaMissObj->GetTmpPath();
	
	vgatesession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_vgate.indexfile.txt",
		tmpdir, now.wYear, now.wMonth, now.wDay,
		now.wHour, now.wMinute, now.wSecond, vgatesession.m_sip, vgatesession.m_dip, ++g_MediaFileNum);

	vgatesession.m_spyaddr = spyaddr;

	vgatesession.WriteIndexFile("vgate", "vgate", CString(text, len), g_MediaMissObj);
	return TRUE;
}




// 如果是语音网关的数据就返回TRUE(通知程序处理下一个包)
// 否则返回FALSE
BOOL ProcessVGatePacket(DATEHEADER& dateheader, char* pPacket, LPCTSTR spyaddr, int PacketFrom)
{
	if (dateheader.m_type == TCPPacket)
	{ // TCP
		if ((memcmp(pPacket, "\3\0", 2) == 0) &&
			(memcmp(pPacket+4, "\x08\x02", 2) == 0) &&
			(pPacket[8] == 5) &&
			(Swap2Byte(*((WORD*)(pPacket+2))) == dateheader.m_datalen))
		{
			return ProcessH225Packet(dateheader, pPacket, spyaddr, PacketFrom);
		}
	}
	else
	{ // UDP
		char tmpbuf[2000];
		int fixpro=0;
		if(memcmp(pPacket,"\x01\x00\x00\x00\x00\x00",6)==0
			&& dateheader.m_datalen>0x30 ) //为wcall 
		{
			memcpy(tmpbuf, pPacket+0x24, dateheader.m_datalen+1-0x24);
			fixpro=1;
		}
		else
			memcpy(tmpbuf, pPacket, dateheader.m_datalen+1);
		//strlwr(tmpbuf);

			Voip97CallPhoneNum(dateheader,tmpbuf,PacketFrom);
			
			if(	(memicmp(tmpbuf, "INVITE sip:", strlen("INVITE sip:")) == 0) || 
			(memicmp(tmpbuf, "REGISTER sip:", strlen("REGISTER sip:")) == 0) ||
			(memicmp(tmpbuf, "ACK sip:", strlen("ACK sip:")) == 0) || 
			(memicmp(tmpbuf, "SIP/2.0 200 OK", strlen("SIP/2.0 200 OK")) == 0) ||
			(memicmp(tmpbuf, "INFO sip:", strlen("INFO sip:")) == 0) || 
			(memicmp(tmpbuf, "BYE sip:", strlen("BYE sip:")) == 0) ||
			(memicmp(tmpbuf, "CANCEL sip:", strlen("BYE sip:")) == 0) )
			return ProcessSipPacket(DataHeader, tmpbuf, spyaddr, PacketFrom,fixpro);
	}
	return FALSE;
}
