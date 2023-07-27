#include "stdafx.h"
#include "imsg.h"
#include "Msn.h"


Search serch_msncmd[8] = \
{
	Search("MSG "),
	Search("USR "),
	Search("JOI "),
	Search("CAL "),
	Search("PGD "),
	Search("REM "),
	Search("ADD "),
	Search("INVITE MSNMSGR:")
};

Search srh_filetag[7] = \
{
	Search("To: "),
	Search("From: "),
	Search("EUF-GUID: "),
	Search("Context: ")
};

#define			CODEPAGE_COUNT		1

typedef struct CODEPAGE
{
	Search		codestr;
	int			code;
	LPCTSTR		language;
public:
	CODEPAGE(LPCTSTR str, int c, LPCTSTR lang)
	:codestr(str)
	{
		code = c;
		language = lang;
	}

}CODEPAGE;

CODEPAGE MSNCODEPAGE[CODEPAGE_COUNT] = \
{
	{CODEPAGE("cs=81;", 949, "(韩文)")}		// 韩语
};

Search serch_crlf("\r\n");

int FormarMsnMSGSection(MsnSession *s, char *pbuf, LPMSNMSG msg, int len, int direction);//, CStringArray* temstr);
int FormarMsnUUMMSGSection(MsnSession *s, char *pbuf, LPMSNMSG msg, int len, int direction);//, CStringArray* temstr);
int FormartWin7MsnMSGSection(MsnSession *s, char *pbuf, LPMSNMSG msg, int len, int direction);//, CStringArray* temstr);
int FormarMsnUSRSection(MsnSession *s, char *pbuf, LPMSNMSG msg, int len, int direction);//, CStringArray* temstr);
int FormarMsnADDSection(MsnSession *s, char *pbuf, LPMSNMSG msg, int len, int direction);
int FormarMsnREMSection(MsnSession *s, char *pbuf, LPMSNMSG msg, int len, int direction);
int FormarMsnJOISection(MsnSession *s, char *pbuf, LPMSNMSG msg, int len, int direction);
int FormarMsnCALSection(MsnSession *s, char *pbuf, LPMSNMSG msg, int len, int direction);
int FormarMsnANSSection(MsnSession *s, char *pbuf, LPMSNMSG msg, int len, int direction);
int FormarMsnPGDSection(MsnSession *s, char *pbuf, LPMSNMSG msg, int len, int direction);

CString SplitMsnName(CString msnname);
extern LPVOID g_pDecodeMsn;
CString DecodeMsnMessage(char *str, CString key)
{
	if(g_pDecodeMsn == NULL)
		return str;

	char * szBufferEn = str;
	const char * szKey = key;
	char szOut[0x800] = {0};
	
	__asm
	{
		push 0x800
		lea eax,szOut;
		push eax
		push szKey
		push szBufferEn
		call g_pDecodeMsn
			
	}
	return CString(szOut);
}
inline void SetDecodeKey(CString& DeKey, MsnSession *s, int direction)
{
	if(direction==1)
	{
		s->m_ownDeKey = DeKey;
	}
	else if(direction==2)
	{
		s->m_othDeKey = DeKey;
	}
}

inline void SetSessionUserInfo(CString& User, MsnSession *s, int direction)
{
	if((direction==1) && (s->m_userid.GetLength() < User.GetLength()))
	{
		s->m_userid = User;
	}
	else if((direction==2) && (s->m_Colloquist.GetLength() < User.GetLength()))
	{
		s->m_Colloquist = User;
	}
}

int GetNextPacket(const char *p, int len)
{
	char *tmp = (char*)p;
	int minpos;
	for (int i = 0; i < 8; i++)
	{
		minpos = serch_msncmd[i].find(tmp, len);
		if(minpos == -1)
		{
			minpos = len;
		}
		else
		{
			len = minpos;
		}
	}
	return minpos;
}

int	ProcessMsnData(MsnSession* pMsnSion, IMSGPARA* pPara)
{
	CString strText;
	int pos = 0;
	char* buf = pPara->m_packetbuf;
	while(pos < pPara->m_packetlen)
	{
#ifdef		_DEBUG
		char tmpcmd[8];
		memcpy(tmpcmd, buf + pos, 4);
		memset(tmpcmd+4, 0, 4);
#endif
		char *tmpbuf = buf+pos;
		int tmppos = -1;
		MSNMSG msnmsg;
		msnmsg.Direction = 3;
		msnmsg.language = _T("");
		msnmsg.Msg.ReleaseBuffer(0);
		msnmsg.Acceptant.ReleaseBuffer(0);
		msnmsg.Sender.ReleaseBuffer(0);
		msnmsg.operation = 3;
		
		// 片断解析失败就返回-1
		if (memcmp(tmpbuf, "MSG ", 4) == 0)
		{
			tmppos = FormarMsnMSGSection(pMsnSion, tmpbuf + 4, &msnmsg,
				pPara->m_packetlen - pos - 4, pPara->m_direction);//, &pPara->m_hisstrarr);
		}
		else if (memcmp(tmpbuf, "USR ", 4) == 0)
		{
			tmppos = FormarMsnUSRSection(pMsnSion, tmpbuf + 4, &msnmsg,
				pPara->m_packetlen - pos - 4, pPara->m_direction);//, &pPara->m_hisstrarr);
		}
		else if (memcmp(tmpbuf, "CAL ", 4) == 0)
		{
			tmppos = FormarMsnCALSection(pMsnSion, tmpbuf + 4, &msnmsg,
				pPara->m_packetlen - pos - 4, pPara->m_direction);
		}
		else if (memcmp(tmpbuf, "ANS ", 4) == 0)
		{
			tmppos = FormarMsnANSSection(pMsnSion, tmpbuf + 4, &msnmsg,
				pPara->m_packetlen - pos - 4, pPara->m_direction);
		}
		else if (memcmp(tmpbuf, "JOI ", 4) == 0)
		{
			tmppos = FormarMsnJOISection(pMsnSion, tmpbuf + 4, &msnmsg,
				pPara->m_packetlen - pos - 4, pPara->m_direction);
		}
		else if (memcmp(tmpbuf, "PGD ", 4) == 0)
		{
			tmppos = FormarMsnPGDSection(pMsnSion, tmpbuf + 4, &msnmsg,
				pPara->m_packetlen - pos - 4, pPara->m_direction);
		}
		else if (memcmp(tmpbuf, "ADD ", 4) == 0)
		{
			tmppos = FormarMsnADDSection(pMsnSion, tmpbuf + 4, &msnmsg,
				pPara->m_packetlen - pos - 4, pPara->m_direction);
		}
		else if (memcmp(tmpbuf, "REM ", 4) == 0)
		{
			tmppos = FormarMsnREMSection(pMsnSion, tmpbuf + 4, &msnmsg,
				pPara->m_packetlen - pos - 4, pPara->m_direction);
		}
		else if (memcmp(tmpbuf, "UUM ", 4) == 0)
		{
			//	UUM 15 suyang1865311@msn.cn 1 1 168
			//	MIME-Version: 1.0
			//	Content-Type: text/plain; charset=UTF-8
			//	X-MMS-IM-Format: FN=Microsoft%20YaHei%20; EF=; CO=0; CS=1; PF=0
			//	Dest-Agent: client
			//
			//	ddddddddddddddddddddd"
			tmppos = FormarMsnUUMMSGSection(pMsnSion, tmpbuf + 4, &msnmsg,
				pPara->m_packetlen - pos - 4, pPara->m_direction);//, &pPara->m_hisstrarr);
		}
		else if (memcmp(tmpbuf, "SDG ", 4) == 0)
		{
			//	SDG 35 381
			//
			//	Routing: 1.0
			//
			//	To: 1:suyang5311186@hotmail.com
			//
			//	From: 1:jncf20@hotmail.com;epid={422c7e98-9de4-45e6-b708-3aec46af8554}
			//
			//
			//
			//	Reliability: 1.0
			//
			//
			//
			//	Messaging: 2.0
			//
			//	Message-Type: Text
			//
			//	Content-Length: 79
			//
			//	Content-Type: text/plain; charset=UTF-8
			//
			//	X-MMS-IM-Format: FN=Microsoft%20YaHei; EF=; CO=0; CS=1; PF=0
			//	
			//
			//	1111111111111111111111111111111111111111111111111111111111111111111111111111111

			tmppos = FormartWin7MsnMSGSection(pMsnSion, tmpbuf + 4, &msnmsg,
				pPara->m_packetlen - pos - 4, pPara->m_direction);
		}
		else if (memcmp(tmpbuf, "INVITE MSNMSGR", 14) == 0)
		{
			if(ProcessMsnTransfile(tmpbuf, pPara->m_packetlen-pos, &msnmsg))
			{
				tmppos = pPara->m_packetlen-pos-4;
				msnmsg.operation = 5;	// 无法确定主动方
				msnmsg.Direction = pPara->m_direction;
#ifdef _DEBUG
				printf("2. %s --> %s, file %s\r\n", msnmsg.Sender, msnmsg.Acceptant, msnmsg.fname);
#endif
			}
		}
		else
		{
			pos += GetNextPacket(tmpbuf + 4, pPara->m_packetlen - pos - 4) + 4;
			continue;
		}

		if (tmppos > 0)
		{
			pos += tmppos + 4;
		}
		else if (tmppos == 0)
		{
			break;
		}
		else
		{
			pos += GetNextPacket(tmpbuf + 4, pPara->m_packetlen - pos - 4) + 4;
			continue;
		}

		if ((pMsnSion->m_direction == 0) && (msnmsg.Direction != 3))
		{
			if (msnmsg.Direction == pPara->m_direction)
			{
				pMsnSion->m_direction = 1;
			}
			else
			{
				// 交换IP方向
				CString tstr = pMsnSion->m_sip;
				pMsnSion->m_sip = pMsnSion->m_dip;
				pMsnSion->m_dip = tstr;
				tstr = pMsnSion->m_dmac;
				pMsnSion->m_dmac = pMsnSion->m_smac;
				pMsnSion->m_smac = tstr;
				DWORD ip;
				memcpy(&ip, pMsnSion->m_csip, 4);
				memcpy(pMsnSion->m_csip, pMsnSion->m_cdip, 4);
				memcpy(pMsnSion->m_cdip, &ip, 4);
				pMsnSion->m_AuthInfo.SwapAuthInfo();
				pMsnSion->m_direction = 2;
			}

			// 匹配IDC数据源
			if (pMsnSion->m_dataSrc == DATASRC_OTHER)
			{
				if (v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)pMsnSion->m_cdip), pMsnSion->m_strISP)
					|| v_IdcIpLib.QueryIdcIP(Swap4Byte(*(DWORD*)pMsnSion->m_csip), pMsnSion->m_strISP))
				{
					pMsnSion->m_dataSrc = DATASRC_IDC;
				}
			}
		}

		if (pMsnSion->m_direction == 0)
		{
			// 不明确方向时就不输出信息
			continue;
		}
		if(msnmsg.operation == 4 || msnmsg.operation == 5)
		{
			pMsnSion->WriteTransFileIndexFile(&msnmsg);
			continue;		
		}
		
		if (!msnmsg.Msg.IsEmpty())
		{
			int msglen = pPara->m_msglen;
			char* buf = pPara->m_msgbuf;
			msglen += sprintf(buf + msglen, "MSN%s: ", msnmsg.language);
			msglen += GetTimeString(pPara->m_packtm, "%Y-%m-%d %H:%M:%S",
				buf + msglen, 40);
			if (msnmsg.Direction == 1)
			{
				// Client --> Server
				msglen += sprintf(buf + msglen, "    %s → %s    %s → %s:\r\n%s\r\n\r\n",
					pMsnSion->m_sip, pMsnSion->m_dip, pMsnSion->m_userid,
					msnmsg.Acceptant, msnmsg.Msg);
			}
			else
			{
				// Server --> Client
				msglen += sprintf(buf + msglen, "    %s ← %s    %s ← %s:\r\n%s\r\n\r\n",
					pMsnSion->m_sip, pMsnSion->m_dip, pMsnSion->m_userid,
					msnmsg.Sender, msnmsg.Msg);
			}
			pPara->m_msglen = msglen;
		}

		if (v_stat && v_statImsgOther && !pMsnSion->m_userid.IsEmpty()
			&& ((msnmsg.operation == 1) || ((msnmsg.operation == 2) && !pMsnSion->m_bRecord)))
		{
			if (msnmsg.operation == 1)
			{
				pPara->m_bIdcSvr = TRUE;
			}
			CString userid = SplitMsnName(pMsnSion->m_userid);
			// 写活动历史
 			CString hisstr,webfigstr;
			HistoryRecord hisRec;
			hisRec.SetRecord(IDX_FORWTYPE,	"1");
			hisRec.SetRecord(IDX_ID,		userid);
			hisRec.SetRecord(IDX_USER,		userid);

			if (msnmsg.operation == 1)
			{
				hisRec.SetRecord(IDX_STATE,	"上线");
				hisstr = pMsnSion->GetWebidStr("imsg", "imsg_msn", &hisRec,
					g_pImsgMissObj, pPara->m_packtm);
// 				hisstr = pMsnSion->GetHisStr("imsg", "imsg_msn", &hisRec,
// 					g_pImsgMissObj, pPara->m_packtm);
// 				webfigstr = pMsnSion->GetWebfigStr("imsg", "imsg_msn", &hisRec,
// 					g_pImsgMissObj, pPara->m_packtm);

			}
 			else if (!pMsnSion->m_Colloquist.IsEmpty())
			{
				CString colloquist = SplitMsnName(pMsnSion->m_Colloquist);
				pMsnSion->m_bRecord = TRUE;
				hisRec.SetRecord(IDX_CONTACT, colloquist);
				hisRec.SetRecord(IDX_STATE,	"通讯");
 				hisstr = pMsnSion->GetWebidStr("imsg", "imsg_msn", &hisRec,
						g_pImsgMissObj, pPara->m_packtm);
//  			hisstr = pMsnSion->GetHisStr("imsg", "imsg_msn", &hisRec,
// 					g_pImsgMissObj, pPara->m_packtm);
// 		    	webfigstr = pMsnSion->GetWebfigStr("imsg", "imsg_msn", &hisRec,
// 					g_pImsgMissObj, pPara->m_packtm);

			}
			if (!hisstr.IsEmpty())
			{
				pPara->m_webidarr.Add(hisstr);
			}
// 			if (!hisstr.IsEmpty())
// 			{
// 				pPara->m_hisstrarr.Add(hisstr);
// 			}
// 			if (!webfigstr.IsEmpty())
// 			{
// 				pPara->m_webfigrarr.Add(webfigstr);
// 			}
		}
	}
	return pos;
}
void MsnSession::WriteTransFileIndexFile(MSNMSG *pImsg)
{
	CString str;
	// 匹配案件、预警和屏蔽信息
	DWORD state = 0;
	
	CString text;
	text.Format("%s\r\n%s", pImsg->Acceptant, pImsg->Sender);
	
	SearchCaseOrWarn(this, text, text.GetLength(), state,"imsg_msn_tranfile");
	
	// 匹配案件和预警
	// 	m_OpenCaseFlg = v_opencaseVp;
	// 	m_OpenWarnFlg = v_openwarnVp;
	m_OpenIndxFlg = v_openflagqqtranfile;
	
	if(pImsg->operation == 4)
	{
		m_indexfname = CreateFilename(m_sip, m_dip, "imsg_msn_tranfile.indexfile.txt");
		if(pImsg->Direction == 2)
		{
			str.Format("user=%s\r\nfrom=%s\r\ncontact=%s\r\nto=%s\r\ntranfilename=%s\r\n",
				pImsg->Acceptant, pImsg->Sender, pImsg->Acceptant,pImsg->Acceptant, pImsg->fname);
		}
		else
		{
			str.Format("user=%s\r\nfrom=%s\r\ncontact=%s\r\nto=%s\r\ntranfilename=%s\r\n",
				pImsg->Sender, pImsg->Sender, pImsg->Acceptant, pImsg->Acceptant, pImsg->fname);
		}
	}
	else
	{
		if(pImsg->Direction == 1)
			m_indexfname = CreateFilename(m_sip, m_dip, "imsg_msn_tranfile.indexfile.txt");
		else
			m_indexfname = CreateFilename(m_dip, m_sip, "imsg_msn_tranfile.indexfile.txt");
		str.Format("from=%s\r\ncontact=%s\r\nto=%s\r\ntranfilename=%s\r\n",
			pImsg->Sender, pImsg->Acceptant, pImsg->fname);

	}
	WriteIndexFile("imsg_msn_tranfile", "imsg_msn_tranfile", str, g_pImsgMissObj);
	m_CaseArr.RemoveAll();
	m_WarnArr.RemoveAll();
	m_begfiletm = pImsg->sendtm;
	m_closetm += 10;
}

CString GetLineFromBuffer(char *buffer, int len)
{
	int pos = serch_crlf.find(buffer, len);
	if (pos > 0)
	{
		CString strRet = CString((LPCSTR)buffer, pos);
		return strRet;
	}
	else
		return "";
}

CString GetLineFromSearch(const char *buffer, int len, Search srh, BOOL bLast = FALSE)
{
	CString strRet;
	int pos = srh.find(buffer, len);
	if (pos > 0 && pos < len+srh.m)
	{
		int endpos = serch_crlf.find(buffer+pos+srh.m, len-srh.m-pos);
		if(endpos > 0)
		{
			strRet = CString((LPCSTR)buffer+pos+srh.m, endpos);
		}
		else if(bLast)
		{
			strRet = CString((LPCSTR)buffer+pos+srh.m, len-srh.m-pos);
		}
	}
	return strRet;
}

//**********************************************************
//	格式化PGD命令的内容，提取其聊天内容(手机短信)
//**********************************************************
int FormarMsnPGDSection(MsnSession *s, char *pbuf, LPMSNMSG pmsg, int len, int direction)
{
//	0030  ff e7 8b 4f 00 00 50 47  44 20 34 37 20 74 65 6c         PG D 47 tel
//	0040  3a 2b 38 36 31 33 38 36  34 33 30 31 35 35 33 20   :+861386 4301553 
//	0050  31 20 39 38 0d 0a 3c 54  45 58 54 20 78 6d 6c 3a   1 98..<T EXT xml:
//	0060  73 70 61 63 65 3d 22 70  72 65 73 65 72 76 65 22   space="p reserve"
//	0070  20 65 6e 63 3d 22 75 74  66 2d 38 22 3e e6 b5 8b    enc="ut f-8">...
//	0080  e8 af 95 e6 b5 8b e8 af  95 e6 b5 8b e8 af 95 3c   ........ .......<
//	0090  2f 54 45 58 54 3e 20 3c  4c 43 49 44 3e 32 30 35   /TEXT> < LCID>205
//	00a0  32 3c 2f 4c 43 49 44 3e  20 3c 43 53 3e 67 62 32   2</LCID>  <CS>gb2
//	00b0  33 31 32 3c 2f 43 53 3e                            312</CS>         

	ASSERT(pbuf!=NULL);
	try
	{
		//==============================================================================================
		//	从缓冲区取一行数据，如果取不到则该包的数据不全，应该于下一个包
		//	一起处理
		//==============================================================================================
		CString Line = GetLineFromBuffer(pbuf, len);
		if (Line.IsEmpty())		// 47 tel:+8613854601553 1 98
		{
			return (len > 200)? -1 : 0;
		}
		
		LPCTSTR tp = (LPCTSTR)Line;
		int msglen = 0;
		char *p = strchr(tp, ' ');
		if (p!=NULL)
		{
			p++;
			if (memcmp(p, "tel:", 4) == 0)
			{
				char* p1 = strchr(p, ' ');
				if (p1 != NULL)
				{
					*p1++ = 0;
					char *p2 = strrchr(p1, ' ');
					if (p2!=NULL)
					{
						p2++;
						msglen = atoi(p2);
						if ((msglen>0) && msglen<=len-Line.GetLength()-2)
						{
							pmsg->Acceptant = p;
							pmsg->Direction = 1;
							pmsg->operation = 2;		// 对象聊天
							pmsg->Msg = CString((LPCTSTR)(pbuf + Line.GetLength() + 2),
								msglen);
							pmsg->Msg = NetBytestoAnsi(pmsg->Msg, s->m_language);
							pmsg->Msg = ConverHttp2Html(pmsg->Msg);
							return 	msglen + Line.GetLength() + 2;
						}
					}
				}
			}
		}
	}
	catch(...)
	{
		WriteLog(IMSGPRO, "FormarMsnPGDSection() Thread=%d", GetCurrentThreadId());
	}
	return -1;
}
/*****************************************************************************
* Class : Global Function
* Function : to_MultiByte
* Description: 把Unicode转换成多字节
* Parameters : char* strSM -- 要转换的Unicode缓冲
* int nLength -- Unicode缓冲的长度
* Return : 转换后的多字节字串
* Input :
* Output :
* History : Created by Rongdian Monday, April 29, 2002 11:03:15
* Process :
******************************************************************************/
CString to_MultiByte(char* strSM, int nLength)
{
	UINT nLen = 0;
	PBYTE lpszM;
	PBYTE lpszW = new BYTE[nLength];
	memcpy(lpszW, strSM, nLength);
// 	for(int i = 0; i < nLength/2; i++)
// 		*((unsigned short*)lpszW + i) = ntohs(*((unsigned short*)lpszW + i));
	nLen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK,
		(const unsigned short*)lpszW, nLength/2, NULL, 0, NULL, NULL);
	lpszM = new BYTE[nLen+1];
	nLen = WideCharToMultiByte(CP_ACP, 0,
		(const unsigned short*)lpszW, nLength/2, (char*)lpszM, nLen, NULL, NULL);
	lpszM[nLen] = 0;
	CString csSM((LPCTSTR)lpszM, nLen);
	delete lpszM;
	delete lpszW;
	return csSM;
} 
CString GetMsnId(CString msnlongid)
{
	//<msnmsgr:coolstar1980@msn.com;{8b4abd1b-c9c2-407b-9191-f9bb8e58491a}>
	 CString msnid = msnlongid;
	 msnid.TrimLeft("<msnmsgr");
	 msnid.TrimLeft(":");
	 int ipos = msnid.Find(";{");
	 if(ipos > 0)
		 msnid = msnid.Left(ipos);
	return msnid;
}

int ProcessMsnTransfile(const char *pbuf, int datalen, MSNMSG *msg)
{
	//Content-Type: application/x-msnmsgrp2p\r\n
	//P2P-Dest: ...\r\n
	//P2P-Src: ...\r\n
	//\r\n
	//48-4字节头INVITE MSNMSG R:{id} MSNSLP/1.0\r\m
	//To: ...\r\n
	//From: ...\r\n
	//EUF-GUID: {5D3E02AB-6190-11D3-BBBB-00C04F795683}\r\n
	//...
	//Context: 512字节(base64编码)
	CString strfrom, strto, strfname, strrid;
	int ii = serch_msncmd[7].find(pbuf, datalen);
	if(ii < 0)
		return 0;

	strrid = GetMsnId(pbuf+ii+serch_msncmd[7].m);
	CString tmpchk = GetLineFromSearch(pbuf, datalen, srh_filetag[2]);
	if(tmpchk.Compare("{5D3E02AB-6190-11D3-BBBB-00C04F795683}") == 0)
	{
		strto = GetLineFromSearch(pbuf, datalen, srh_filetag[0]);
		strfrom = GetLineFromSearch(pbuf, datalen, srh_filetag[1]);
		CString tmpfilename = GetLineFromSearch(pbuf, datalen, srh_filetag[3], TRUE);
		if(FALSE == tmpfilename.IsEmpty())
		{
			char ttresult[1024];
			char *pbasebuf = tmpfilename.GetBuffer(tmpfilename.GetLength());
			CMailCoder::base64_decode(pbasebuf, tmpfilename.GetLength(), ttresult);
			for (int ll = 0;  ll < tmpfilename.GetLength() ; ll+=2)
			{
				if(ttresult[ll+0x14]== 0 && ttresult[ll+0x15] == 0)
					break;
			}
			strfname =to_MultiByte(ttresult+0x14, ll);
			if(FALSE == strfname.IsEmpty())
			{
				// 提取strfrom, strto中的id, coolstar1980@msn.com
				// To: <msnmsgr:coolstar1980@msn.com;{8b4abd1b-c9c2-407b-9191-f9bb8e58491a}>\r\n
				strto = GetMsnId(strto);
				strfrom = GetMsnId(strfrom);
				if(FALSE == strto.IsEmpty() && FALSE == strfrom.IsEmpty())
				{
					if(strrid.CompareNoCase(strto) == 0)
					{
						msg->Sender = strfrom;
						msg->Acceptant = strto;
					}
					else
					{
						msg->Sender = strfrom;
						msg->Acceptant = strto;
					}
					
					msg->fname = strfname;
				}
				return 1;
			}
		}
	}
	return 0;
}

void BkErrMsgPacket(const char *pPacket, int len)
{
	static unsigned long staticseqcatch = 1;
	if(staticseqcatch > 10)	// 备份10个就可以了, 不需要很多
		return ;
	
	CreateDir("e:\\ddebak");
	FILE* fp = NULL;
	try
	{
		CString ocpfile;
		ocpfile.Format("e:\\ddebak\\msnmsg_%04d.dat", staticseqcatch++);
		fp = fopen(ocpfile, "w+b");
		if (fp == NULL)
		{
			ocpfile.Empty();
			return ;
		}
		fwrite(pPacket, 1, len, fp);
		fclose(fp);
		fp = NULL;
	}
	catch(...)
	{
		WriteLog(IMSGPRO, "BkErrMsgPacket() Error!!");
		if (fp!=NULL)
			fclose(fp);
	}
	return ;
}


//**********************************************************
//	格式化MSG命令的内容，提取其聊天内容
//**********************************************************
int FormarMsnMSGSection(MsnSession *s, char *pbuf, LPMSNMSG msg,int len, int direction)//, CStringArray* temstr)
{
	ASSERT(pbuf!=NULL);

	CString Line, tmpLine;
	CString MsgType, MsgLen, MsgSeq;
	int		pos1 = 0, pos2 = 0;
	int		HeadLen;
	int		datalen = 0;
	char*	pRawPacket = pbuf ;
	int orilen = len;
int ierror = 0;
	try
	{
		//==============================================================================================
		//	从缓冲区取一行数据，如果取不到则该包的数据不全，应该于下一个包
		//	一起处理
		//==============================================================================================
ierror = 1;
		Line = GetLineFromBuffer(pbuf, len);
		// 89 N 859, 前4字符在进此函数前已经去掉, 实际此行为MSG 89 N 859
		if(Line.IsEmpty())
		{
			return (len > 100)? -1 : 0;
		}
		
ierror = 2;
		HeadLen = Line.GetLength() + 2;
		pbuf += HeadLen;
		len -= HeadLen;
		
		pos2 = Line.Find(' ', pos1);
		if(pos2 == -1)
		{
			return (len > 100)? -1 : 0;
		}

ierror = 3;	
		//msgseq 为账号名	
		MsgSeq = Line.Mid(pos1, pos2 - pos1);	// 89
		pos1 = pos2 + 1;
		
		pos2 = Line.Find(' ', pos1);
		if(pos2 == -1)
		{
			return (len>100)? -1 : 0;
		}

ierror = 4;
		//msgtype 现在是用户名
		MsgType = Line.Mid(pos1, pos2 - pos1);	// N
		
		pos2++;
		MsgLen = Line.Mid(pos2, Line.GetLength() - pos2);
		datalen = atoi(MsgLen);					// 859
		if(datalen <= 0)
		{
			return -1;
		}
		if (datalen > len)
		{
			//==============================================================================================
			//	如果数据长度大于缓冲区剩余数据的长度，说明该包只包含了消息的一部分，
			//	应该存到socket的缓冲区里，和下一个包一起处理
			//==============================================================================================
			return (datalen > 50*1024)? -1 : 0;
		}
ierror = 5;
		
		CString ContentType, TypeUser, tmpKey, CharSet;
		BOOL bisHandShake = FALSE;
		BOOL bisNeedDecode = FALSE;

		int ttlen, offlen;
		offlen = 0;
		while(len > 2)
		{
			tmpLine = GetLineFromBuffer(pbuf, len);
			if(tmpLine.IsEmpty())
			{
				if(memcmp(pbuf, "\r\n", 2) == 0)
				{
					pbuf += 2;
					len -= 2;
					offlen += 2;
				}
				break;
			}

			ttlen = tmpLine.GetLength()+2;
			offlen += ttlen;
			if(memicmp(tmpLine, "MIME-Version", 12) == 0)
			{	//MIME-Version: 1.0\r\n
			}
			else if(memicmp(tmpLine, "Content-Type", 12) == 0)
			{	// Content-Type: text/plain; charset=UTF-8
				ContentType = tmpLine;
				int icharset = ContentType.Find("charset=");
				if(icharset > 0)
				{
					CharSet = ContentType.Mid(icharset);
					CharSet.MakeLower();
				}
			}
			else if(memicmp(tmpLine, "TypingUser: ", 12) == 0)
			{	// TypingUser: floatingsword@hotmail.com
				TypeUser = tmpLine.Mid(12);
			}
			else if(memicmp(tmpLine, "state: ", 7) == 0)
			{	// state: ssShellHandShake1
				bisHandShake = (tmpLine.Find("ssShellHandShake1")!= -1);
			}
			else if(memicmp(tmpLine, "bobo1: ", 7) == 0)
			{	// bobo1: 564154674617247110960772158627.420931678076717332190106194633
				tmpKey = tmpLine.Mid(7);
				int idot = tmpKey.Find('.');
				if(idot > 0)
				{
					tmpKey = tmpKey.Left(idot);
					tmpKey += ".65537";
				}
			}
			else if(memicmp(tmpLine, "encrypted: ", 11) == 0)
			{	// encrypted: 1
				bisNeedDecode = (tmpLine.CompareNoCase("encrypted: 1") == 0);
			}
			pbuf += ttlen;
			len -= ttlen;
		}
ierror = 6;

		//u:user,D:p2p,N:msg
		BOOL bDeal = FALSE;
		if(MsgType.GetLength() > 1)
		{
			// S-->C
ierror = 7;
			if(FALSE == TypeUser.IsEmpty())//是控制信息
			{
				bDeal = TRUE;
ierror = 8;
				SetSessionUserInfo(TypeUser, s, 2);
			}
			else if(ContentType.Find("text/x-bobo",0) > -1 && bisHandShake)
			{
ierror = 9;
				SetDecodeKey(tmpKey, s, 2);
			}
			else if(ContentType.Find("text/plain",0) > -1)
			{
ierror = 10;
				bDeal = TRUE;
				//==============================================================================================
				//	获得真正的可读的消息字符串
				//==============================================================================================
				s->m_language = CP_ACP;
				for (int a = 0; a < CODEPAGE_COUNT; a++)
				{
					if (MSNCODEPAGE[a].codestr.find(tmpLine, tmpLine.GetLength()) > 0)
					{
						s->m_language = MSNCODEPAGE[a].code;
						msg->language = MSNCODEPAGE[a].language;
						break;
					}
				}
				if(offlen > datalen)
				{
					BkErrMsgPacket(pRawPacket, orilen);
					WriteLog("error", "recv msn message offlen %d, datalen %d", offlen, datalen);
				}
				else
				{
ierror = 11;
					CString str;
					char *pData= str.GetBufferSetLength(datalen+1024);
					memset(pData, 0, datalen);
					memcpy(pData, pbuf, datalen-offlen);
ierror = 12;
					if(bisNeedDecode)
					{
						Line = DecodeMsnMessage(pData, s->m_ownDeKey);
						Line = NetBytestoAnsi(Line, s->m_language);
					}
					else
					{
						Line = NetBytestoAnsi(pData, s->m_language);
					}
					if(!Line.IsEmpty())
					{
						msg->Direction = 2;
						msg->Sender.Format("%s", MsgType);
						msg->Acceptant = s->m_userid;
						msg->Msg = Line;

						SetSessionUserInfo(msg->Sender, s, 2);
						msg->operation = 2;		// 对象聊天
					}
				}
			}
		}
		if(FALSE == bDeal && MsgType == "U")
		{
ierror = 21;
			if(FALSE == TypeUser.IsEmpty())
			{
				msg->Sender = TypeUser;
				msg->Direction = 1;
				SetSessionUserInfo(msg->Sender, s, 1);
			}
		}
		else if(FALSE == bDeal &&  (MsgType == "A" || MsgType == "N"))
		{
ierror = 31;
			if(ContentType.Find("text/x-bobo",0) > -1 && bisHandShake)
			{
ierror = 32;
				SetDecodeKey(tmpKey, s, 1);
			}
			else
			{
ierror = 33;
				s->m_language = CP_ACP;
				for (int a = 0; a < CODEPAGE_COUNT; a++)
				{
					if (MSNCODEPAGE[a].codestr.find(CharSet, CharSet.GetLength())>0)
					{
						s->m_language = MSNCODEPAGE[a].code;
						msg->language = MSNCODEPAGE[a].language;
						break;
					}
				}
				//==============================================================================================
				//	获得真正的可读的消息字符串
				//==============================================================================================
				if(offlen > datalen)
				{
					BkErrMsgPacket(pRawPacket, orilen);
					WriteLog("error", "send msn message offlen %d, datalen %d", offlen, datalen);
				}
				else
				{
ierror = 34;
					CString str;
					char *pData = str.GetBufferSetLength(datalen+1024);
					memset(pData, 0, datalen);
					memcpy(pData, pbuf , datalen - offlen);
ierror = 35;
					if(bisNeedDecode)
					{
						Line = DecodeMsnMessage(pData, s->m_othDeKey);
						Line = NetBytestoAnsi(Line, s->m_language);
					}
					else
					{
						Line = NetBytestoAnsi(pData, s->m_language);
					}
					if(!Line.IsEmpty())
					{
						msg->Msg = Line;
						msg->Direction = 1;
						msg->Sender = s->m_userid;
						msg->Acceptant = s->m_Colloquist;
						msg->operation = 2;		// 对象聊天
					}
				}
			}
		}
		else if( FALSE == bDeal && ContentType.Find("application/x-msnmsgrp2p") > 0)// && MsgType == "D")
		{
ierror = 41;
			if(ProcessMsnTransfile(pbuf, datalen-offlen, msg))
			{
ierror = 42;
				msg->operation = 4;		// 对象聊天
				if(MsgType == "D")
				{
					msg->Direction = 1;
#ifdef _DEBUG
					printf("1d. %s --> %s, file %s\r\n", msg->Sender, msg->Acceptant, msg->fname);
#endif
				}
				else
				{
					msg->Direction = 2;
#ifdef _DEBUG
					printf("1u. %s <-- %s, file %s\r\n", msg->Acceptant, msg->Sender, msg->fname);
#endif
				}
			}
		}
	}
	catch(...)
	{
		WriteLog(IMSGPRO, "FormarMsnMSGSection() Error=%d Thread=%d", ierror, GetCurrentThreadId());
	}

	return datalen + HeadLen;
}


//**********************************************************
//	格式化UUM 方式 MSG命令的内容，提取其聊天内容
//**********************************************************
int FormarMsnUUMMSGSection(MsnSession *s, char *pbuf, LPMSNMSG msg,int len, int direction)//, CStringArray* temstr)
{
	//	UUM 15 suyang1865311@msn.cn 1 1 168
	//	MIME-Version: 1.0
	//	Content-Type: text/plain; charset=UTF-8
	//	X-MMS-IM-Format: FN=Microsoft%20YaHei%20; EF=; CO=0; CS=1; PF=0
	//	Dest-Agent: client
	//
	//	ddddddddddddddddddddd"
	ASSERT(pbuf!=NULL);

	CString Line, tmpLine;
	CString MsgType, MsgLen, MsgSeq, tmp1, tmp2 ;
	int		pos1 = 0, pos2 = 0;
	int		HeadLen;
	int		datalen = 0;
	char*	pRawPacket = pbuf ;
	int orilen = len;
int ierror = 0;
	try
	{
		//==============================================================================================
		//	从缓冲区取一行数据，如果取不到则该包的数据不全，应该于下一个包
		//	一起处理
		//==============================================================================================
ierror = 1;
		Line = GetLineFromBuffer(pbuf, len);
		// 15 suyang1865311@msn.cn 1 1 168, 前4字符在进此函数前已经去掉, 实际此行为15 suyang1865311@msn.cn 1 1 168
		if(Line.IsEmpty())
		{
			return (len > 100)? -1 : 0;
		}
		
ierror = 2;
		HeadLen = Line.GetLength() + 2;
		pbuf += HeadLen;
		len -= HeadLen;
		
		pos2 = Line.Find(' ', pos1);
		if(pos2 == -1)
		{
			return (len > 100)? -1 : 0;
		}

ierror = 3;		
		MsgSeq = Line.Mid(pos1, pos2 - pos1);	// 15
		pos1 = pos2 + 1;
		
		pos2 = Line.Find(' ', pos1);
		if(pos2 == -1)
		{
			return (len>100)? -1 : 0;
		}

ierror = 4;
		MsgType = Line.Mid(pos1, pos2 - pos1);	// suyang1865311@msn.cn
		pos1 = pos2 + 1;


		pos2 = Line.Find(' ', pos1);
		if(pos2 == -1)
		{
			return (len>100)? -1 : 0;
		}
		tmp1 = Line.Mid(pos1, pos2 - pos1);	// 1
		pos1 = pos2 + 1;


		pos2 = Line.Find(' ', pos1);
		if(pos2 == -1)
		{
			return (len>100)? -1 : 0;
		}
		tmp2 = Line.Mid(pos1, pos2 - pos1);	// 1

		pos2++;
		MsgLen = Line.Mid(pos2, Line.GetLength() - pos2);
		datalen = atoi(MsgLen);				// 859
		if(datalen <= 0)
		{
			return -1;
		}
		if (datalen > len)
		{
			//==============================================================================================
			//	如果数据长度大于缓冲区剩余数据的长度，说明该包只包含了消息的一部分，
			//	应该存到socket的缓冲区里，和下一个包一起处理
			//==============================================================================================
			return (datalen > 50*1024)? -1 : 0;
		}
ierror = 5;
		
		CString ContentType, TypeUser, tmpKey, CharSet;
		BOOL bisHandShake = FALSE;
		BOOL bisNeedDecode = FALSE;

		int ttlen, offlen;
		offlen = 0;
		while(len > 2)
		{
			tmpLine = GetLineFromBuffer(pbuf, len);
			if(tmpLine.IsEmpty())
			{
				if(memcmp(pbuf, "\r\n", 2) == 0)
				{
					pbuf += 2;
					len -= 2;
					offlen += 2;
				}
				break;
			}

			ttlen = tmpLine.GetLength()+2;
			offlen += ttlen;
			if(memicmp(tmpLine, "MIME-Version", 12) == 0)
			{	//MIME-Version: 1.0\r\n
			}
			else if(memicmp(tmpLine, "Content-Type", 12) == 0)
			{	// Content-Type: text/plain; charset=UTF-8
				ContentType = tmpLine;
				int icharset = ContentType.Find("charset=");
				if(icharset > 0)
				{
					CharSet = ContentType.Mid(icharset);
					CharSet.MakeLower();
				}
			}
			else if(memicmp(tmpLine, "TypingUser: ", 12) == 0)
			{	// TypingUser: floatingsword@hotmail.com
				TypeUser = tmpLine.Mid(12);
			}
			else if(memicmp(tmpLine, "state: ", 7) == 0)
			{	// state: ssShellHandShake1
				bisHandShake = (tmpLine.Find("ssShellHandShake1")!= -1);
			}
			else if(memicmp(tmpLine, "bobo1: ", 7) == 0)
			{	// bobo1: 564154674617247110960772158627.420931678076717332190106194633
				tmpKey = tmpLine.Mid(7);
				int idot = tmpKey.Find('.');
				if(idot > 0)
				{
					tmpKey = tmpKey.Left(idot);
					tmpKey += ".65537";
				}
			}
			else if(memicmp(tmpLine, "encrypted: ", 11) == 0)
			{	// encrypted: 1
				bisNeedDecode = (tmpLine.CompareNoCase("encrypted: 1") == 0);
			}
			pbuf += ttlen;
			len -= ttlen;
		}
ierror = 6;

		//u:user,D:p2p,N:msg
		BOOL bDeal = FALSE;
		if(MsgType.GetLength() > 1)
		{
			// S-->C
ierror = 7;
			if(FALSE == TypeUser.IsEmpty())//是控制信息
			{
				bDeal = TRUE;
ierror = 8;
				SetSessionUserInfo(TypeUser, s, 2);
			}
			else if(ContentType.Find("text/x-bobo",0) > -1 && bisHandShake)
			{
ierror = 9;
				SetDecodeKey(tmpKey, s, 2);
			}
			else if(ContentType.Find("text/plain",0) > -1)
			{
ierror = 10;
				bDeal = TRUE;
				//==============================================================================================
				//	获得真正的可读的消息字符串
				//==============================================================================================
				s->m_language = CP_ACP;
				for (int a = 0; a < CODEPAGE_COUNT; a++)
				{
					if (MSNCODEPAGE[a].codestr.find(tmpLine, tmpLine.GetLength()) > 0)
					{
						s->m_language = MSNCODEPAGE[a].code;
						msg->language = MSNCODEPAGE[a].language;
						break;
					}
				}
				if(offlen > datalen)
				{
					BkErrMsgPacket(pRawPacket, orilen);
					WriteLog("error", "recv msn message offlen %d, datalen %d", offlen, datalen);
				}
				else
				{
ierror = 11;
					CString str;
					char *pData= str.GetBufferSetLength(datalen+1024);
					memset(pData, 0, datalen);
					memcpy(pData, pbuf, datalen-offlen);
ierror = 12;
					if(bisNeedDecode)
					{
						Line = DecodeMsnMessage(pData, s->m_ownDeKey);
						Line = NetBytestoAnsi(Line, s->m_language);
					}
					else
					{
						Line = NetBytestoAnsi(pData, s->m_language);
					}
					if(!Line.IsEmpty())
					{
						msg->Direction = 2;
						msg->Sender.Format("%s", MsgType);
						msg->Acceptant = s->m_userid;
						msg->Msg = Line;

						SetSessionUserInfo(msg->Sender, s, 2);
						msg->operation = 2;		// 对象聊天
					}
				}
			}
		}
		if(FALSE == bDeal && MsgType == "U")
		{
ierror = 21;
			if(FALSE == TypeUser.IsEmpty())
			{
				msg->Sender = TypeUser;
				msg->Direction = 1;
				SetSessionUserInfo(msg->Sender, s, 1);
			}
		}
		else if(FALSE == bDeal &&  (MsgType == "A" || MsgType == "N"))
		{
ierror = 31;
			if(ContentType.Find("text/x-bobo",0) > -1 && bisHandShake)
			{
ierror = 32;
				SetDecodeKey(tmpKey, s, 1);
			}
			else
			{
ierror = 33;
				s->m_language = CP_ACP;
				for (int a = 0; a < CODEPAGE_COUNT; a++)
				{
					if (MSNCODEPAGE[a].codestr.find(CharSet, CharSet.GetLength())>0)
					{
						s->m_language = MSNCODEPAGE[a].code;
						msg->language = MSNCODEPAGE[a].language;
						break;
					}
				}
				//==============================================================================================
				//	获得真正的可读的消息字符串
				//==============================================================================================
				if(offlen > datalen)
				{
					BkErrMsgPacket(pRawPacket, orilen);
					WriteLog("error", "send msn message offlen %d, datalen %d", offlen, datalen);
				}
				else
				{
ierror = 34;
					CString str;
					char *pData = str.GetBufferSetLength(datalen+1024);
					memset(pData, 0, datalen);
					memcpy(pData, pbuf , datalen - offlen);
ierror = 35;
					if(bisNeedDecode)
					{
						Line = DecodeMsnMessage(pData, s->m_othDeKey);
						Line = NetBytestoAnsi(Line, s->m_language);
					}
					else
					{
						Line = NetBytestoAnsi(pData, s->m_language);
					}
					if(!Line.IsEmpty())
					{
						msg->Msg = Line;
						msg->Direction = 1;
						msg->Sender = s->m_userid;
						msg->Acceptant = s->m_Colloquist;
						msg->operation = 2;		// 对象聊天
					}
				}
			}
		}
		else if( FALSE == bDeal && ContentType.Find("application/x-msnmsgrp2p") > 0)// && MsgType == "D")
		{
ierror = 41;
			if(ProcessMsnTransfile(pbuf, datalen-offlen, msg))
			{
ierror = 42;
				msg->operation = 4;		// 对象聊天
				if(MsgType == "D")
				{
					msg->Direction = 1;
#ifdef _DEBUG
					printf("1d. %s --> %s, file %s\r\n", msg->Sender, msg->Acceptant, msg->fname);
#endif
				}
				else
				{
					msg->Direction = 2;
#ifdef _DEBUG
					printf("1u. %s <-- %s, file %s\r\n", msg->Acceptant, msg->Sender, msg->fname);
#endif
				}
			}
		}
	}
	catch(...)
	{
		WriteLog(IMSGPRO, "FormarMsnMSGSection() Error=%d Thread=%d", ierror, GetCurrentThreadId());
	}

	return datalen + HeadLen;
}

//************************************************************************************
//	格式化SDG 方式 MSG命令的内容，提取其聊天内容,Win7 操作系统内的msn2011是这种情况
//************************************************************************************
int FormartWin7MsnMSGSection(MsnSession *s, char *pbuf, LPMSNMSG msg,int len, int direction)//, CStringArray* temstr)
{
	//	SDG 35 381
	//
	//	Routing: 1.0
	//
	//	To: 1:suyang5311186@hotmail.com
	//
	//	From: 1:jncf20@hotmail.com;epid={422c7e98-9de4-45e6-b708-3aec46af8554}
	//
	//
	//
	//	Reliability: 1.0
	//
	//
	//
	//	Messaging: 2.0
	//
	//	Message-Type: Text
	//
	//	Content-Length: 79
	//
	//	Content-Type: text/plain; charset=UTF-8
	//
	//	X-MMS-IM-Format: FN=Microsoft%20YaHei; EF=; CO=0; CS=1; PF=0
	//
	//
	//
	//	1111111111111111111111111111111111111111111111111111111111111111111111111111111
	ASSERT(pbuf!=NULL);

	CString Line, tmpLine;
	CString MsgType, MsgLen, MsgSeq, tmp1, tmp2 ;
	
	CString from, to;

	int		pos1 = 0, pos2 = 0;
	int		HeadLen;
	int		datalen = 0;
	int     tmplen = 0;
	char*	pRawPacket = pbuf ;
	int		orilen = len;
	BOOL    Bexist = FALSE;
int ierror = 0;
	try
	{
		//==============================================================================================
		//	从缓冲区取一行数据，如果取不到则该包的数据不全，应该于下一个包
		//	一起处理
		//==============================================================================================
ierror = 1;
		Line = GetLineFromBuffer(pbuf, len);
		// 35 381, 前4字符在进此函数前已经去掉, 实际此行为35 381
		if(Line.IsEmpty())
		{
			return (len > 100)? -1 : 0;
		}
		
ierror = 2;
		HeadLen = Line.GetLength() + 2;
		pbuf += HeadLen;
		len -= HeadLen;
		
		pos2 = Line.Find(' ', pos1);
		if(pos2 == -1)
		{
			return (len > 100)? -1 : 0;
		}

ierror = 3;		
		MsgSeq = Line.Mid(pos1, pos2 - pos1);	// 35
		pos1 = pos2 + 1;
		
		pos2++;
		MsgLen = Line.Mid(pos2, Line.GetLength() - pos2);
		datalen = atoi(MsgLen);	
ierror = 5;
		
		CString ContentType, TypeUser, tmpKey, CharSet;
		BOOL bisHandShake = FALSE;
		BOOL bisNeedDecode = FALSE;

		int ttlen, offlen;
		offlen = 0;
		while(len > 2)
		{
			tmpLine = GetLineFromBuffer(pbuf, len);
			if(tmpLine.IsEmpty())
			{
				if(memcmp(pbuf, "\r\n", 2) == 0)
				{
					pbuf += 2;
					len -= 2;
					offlen += 2;
				}

				if (Bexist)
				{
					break;
				}
			}

			ttlen = tmpLine.GetLength()+2;
			offlen += ttlen;
			if(memicmp(tmpLine, "Messaging", 9) == 0)
			{	//MIME-Version: 1.0\r\n
			}
			else if(memicmp(tmpLine, "Content-Type", 12) == 0)
			{	// Content-Type: text/plain; charset=UTF-8
				ContentType = tmpLine;
				int icharset = ContentType.Find("charset=");
				if(icharset > 0)
				{
					CharSet = ContentType.Mid(icharset);
					CharSet.MakeLower();
				}
			}
			else if(memicmp(tmpLine, "From: 1:", 8) == 0)
			{	// TypingUser: floatingsword@hotmail.com
				from = tmpLine.Mid(8);
				int iuserpos = 0;
				iuserpos = from.Find(';');
				if(iuserpos != -1)
				{
					from = from.Left(iuserpos);
				}
			}
			else if(memicmp(tmpLine, "to: 1:", 6) == 0)
			{	
				to = tmpLine.Mid(6);
			}
			else if(memicmp(tmpLine, "state: ", 7) == 0)
			{	// state: ssShellHandShake1
				bisHandShake = (tmpLine.Find("ssShellHandShake1")!= -1);
			}
			else if(memicmp(tmpLine, "bobo1: ", 7) == 0)
			{	// bobo1: 564154674617247110960772158627.420931678076717332190106194633
				tmpKey = tmpLine.Mid(7);
				int idot = tmpKey.Find('.');
				if(idot > 0)
				{
					tmpKey = tmpKey.Left(idot);
					tmpKey += ".65537";
				}
			}
			else if(memicmp(tmpLine, "encrypted: ", 11) == 0)
			{	// encrypted: 1
				bisNeedDecode = (tmpLine.CompareNoCase("encrypted: 1") == 0);
			}
			else if (memicmp(tmpLine, "content-length: ",16) == 0)
			{
				CString Msglen = tmpLine.Mid(16);
			}
			else if (memicmp(tmpLine, "message-type: ",14) == 0)
			{
				MsgType = tmpLine.Mid(14);
			}
			else if(memicmp(tmpLine, "x-mms-im-format: ",17) == 0)
			{
				Bexist = TRUE; 
			}

			pbuf += ttlen;
			len -= ttlen;
		}
ierror = 6;

		//u:user,D:p2p,N:msg
		BOOL bDeal = FALSE;
		if(MsgType.GetLength() > 1)
		{
			// S-->C
ierror = 7;
			if(FALSE == TypeUser.IsEmpty())//是控制信息
			{
				bDeal = TRUE;
ierror = 8;
				SetSessionUserInfo(TypeUser, s, 2);
			}
			else if(ContentType.Find("text/x-bobo",0) > -1 && bisHandShake)
			{
ierror = 9;
				SetDecodeKey(tmpKey, s, 2);
			}
			else if(ContentType.Find("text/plain",0) > -1)
			{
ierror = 10;
				bDeal = TRUE;
				//==============================================================================================
				//	获得真正的可读的消息字符串
				//==============================================================================================
				s->m_language = CP_ACP;
				for (int a = 0; a < CODEPAGE_COUNT; a++)
				{
					if (MSNCODEPAGE[a].codestr.find(tmpLine, tmpLine.GetLength()) > 0)
					{
						s->m_language = MSNCODEPAGE[a].code;
						msg->language = MSNCODEPAGE[a].language;
						break;
					}
				}
				if(offlen > datalen)
				{
					BkErrMsgPacket(pRawPacket, orilen);
					WriteLog("error", "recv msn message offlen %d, datalen %d", offlen, datalen);
				}
				else
				{
ierror = 11;
					CString str;
					char *pData= str.GetBufferSetLength(datalen+1024);
					memset(pData, 0, datalen);
					memcpy(pData, pbuf, datalen-offlen);
ierror = 12;
					if(bisNeedDecode)
					{
						Line = DecodeMsnMessage(pData, s->m_ownDeKey);
						Line = NetBytestoAnsi(Line, s->m_language);
					}
					else
					{
						Line = NetBytestoAnsi(pData, s->m_language);
					}
					if(!Line.IsEmpty())
					{
						msg->Direction = 2;
						msg->Sender.Format("%s", from);
						msg->Acceptant = to;
						msg->Msg = Line;
						

						SetSessionUserInfo(msg->Sender, s, 2);
						msg->operation = 2;		// 对象聊天
					}
				}
			}
		}
		if(FALSE == bDeal && MsgType == "U")
		{
ierror = 21;
			if(FALSE == TypeUser.IsEmpty())
			{
				msg->Sender = TypeUser;
				msg->Direction = 1;
				SetSessionUserInfo(msg->Sender, s, 1);
			}
		}
		else if(FALSE == bDeal &&  (MsgType == "A" || MsgType == "N"))
		{
ierror = 31;
			if(ContentType.Find("text/x-bobo",0) > -1 && bisHandShake)
			{
ierror = 32;
				SetDecodeKey(tmpKey, s, 1);
			}
			else
			{
ierror = 33;
				s->m_language = CP_ACP;
				for (int a = 0; a < CODEPAGE_COUNT; a++)
				{
					if (MSNCODEPAGE[a].codestr.find(CharSet, CharSet.GetLength())>0)
					{
						s->m_language = MSNCODEPAGE[a].code;
						msg->language = MSNCODEPAGE[a].language;
						break;
					}
				}
				//==============================================================================================
				//	获得真正的可读的消息字符串
				//==============================================================================================
				if(offlen > datalen)
				{
					BkErrMsgPacket(pRawPacket, orilen);
					WriteLog("error", "send msn message offlen %d, datalen %d", offlen, datalen);
				}
				else
				{
ierror = 34;
					CString str;
					char *pData = str.GetBufferSetLength(datalen+1024);
					memset(pData, 0, datalen);
					memcpy(pData, pbuf , datalen - offlen);
ierror = 35;
					if(bisNeedDecode)
					{
						Line = DecodeMsnMessage(pData, s->m_othDeKey);
						Line = NetBytestoAnsi(Line, s->m_language);
					}
					else
					{
						Line = NetBytestoAnsi(pData, s->m_language);
					}
					if(!Line.IsEmpty())
					{
						msg->Msg = Line;
						msg->Direction = 1;
						msg->Sender = s->m_userid;
						msg->Acceptant = s->m_Colloquist;
						msg->operation = 2;		// 对象聊天
					}
				}
			}
		}
		else if( FALSE == bDeal && ContentType.Find("application/x-msnmsgrp2p") > 0)// && MsgType == "D")
		{
ierror = 41;
			if(ProcessMsnTransfile(pbuf, datalen-offlen, msg))
			{
ierror = 42;
				msg->operation = 4;		// 对象聊天
				if(MsgType == "D")
				{
					msg->Direction = 1;
#ifdef _DEBUG
					printf("1d. %s --> %s, file %s\r\n", msg->Sender, msg->Acceptant, msg->fname);
#endif
				}
				else
				{
					msg->Direction = 2;
#ifdef _DEBUG
					printf("1u. %s <-- %s, file %s\r\n", msg->Acceptant, msg->Sender, msg->fname);
#endif
				}
			}
		}
	}
	catch(...)
	{
		WriteLog(IMSGPRO, "FormarMsnMSGSection() Error=%d Thread=%d", ierror, GetCurrentThreadId());
	}

	return datalen + HeadLen;
}

//**********************************************************
//	格式化USR命令。此命令用于身份验证
//**********************************************************
int FormarMsnUSRSection(MsnSession *s, char *pbuf, LPMSNMSG msg,int len, int direction)///*,LPCTSTR tmpsip, LPCTSTR tmpdip*/,CStringArray* temstr)
{
	ASSERT(pbuf!=NULL);
	
	CString Line;
	CString TmpLine;
	int		pos1=0,pos2=0;
	
	try
	{
		Line = GetLineFromBuffer(pbuf, len);
		pos2 = Line.Find(' ', pos1);
		if(pos2 == -1)
		{
			return (len>100)? -1 : 0;
		}
		pos1 = pos2 + 1;
		
		pos2 = Line.Find(' ', pos1);
		if(pos2 == -1)
		{
			return (len > 100)? -1 : 0;
		}
		TmpLine = Line.Mid(pos1, pos2 - pos1);
		pos1 = pos2 + 1;

		if(TmpLine == "OK")
		{
			// Server --> Client
			msg->Direction = 2;

			CString TmpLine1;
			CString TmpLine2;
			pos2 = Line.Find(' ', pos1);
			if(pos2 == -1)
			{
				pos2=Line.GetLength();
				TmpLine1 = Line.Mid(pos1,pos2-pos1);
				TmpLine1 = NetBytestoAnsi(TmpLine1,s->m_language);
				TmpLine1 = ConverHttp2Html(TmpLine1);

				int itry = TmpLine1.Find(';');
				if(itry > 0)
				{
					TmpLine1 = TmpLine1.Left(itry);
				}
			}
			else
			{
				TmpLine1 = Line.Mid(pos1, pos2 - pos1);
				pos1 = pos2 + 1;
				pos2 = Line.Find(' ', pos1);
				if(pos2 == -1)
				{
					pos2 = Line.GetLength();
					TmpLine2 = Line.Mid(pos1, pos2 - pos1);
					TmpLine2 = NetBytestoAnsi(TmpLine2, s->m_language);
					TmpLine2 = ConverHttp2Html(TmpLine2);

					int itry = TmpLine1.Find(';');
					if(itry > 0)
					{
						TmpLine1 = TmpLine1.Left(itry);
					}
				}
			}

			if(TmpLine2 == "")
			{
				msg->Acceptant = TmpLine1;
			}
			else
			{
				msg->Acceptant.Format("%s[%s]", TmpLine2, TmpLine1);
			}

			SetSessionUserInfo(msg->Acceptant, s, 1);

			//==========================================================================================
			//	判断并标明session的客户端
			//==========================================================================================
			msg->Sender = "服务器" ;
			msg->Msg.Format("系统消息:    %s 上线。", msg->Acceptant);
			msg->operation = 1;		// 对象上线
		}
		else
		{
			pos2 = Line.Find(' ', pos1);
			if(pos2 != -1)
			{
				TmpLine = Line.Mid(pos1,pos2-pos1);
				pos1=pos2+1;

				if(TmpLine=="I")
				{
					// Client --> Server
					msg->Direction=1;
					pos2 = Line.Find(' ', pos1);
					if(pos2 == -1)
					{
						pos2 = Line.GetLength();
					}
					TmpLine = Line.Mid(pos1, pos2 - pos1);

					SetSessionUserInfo(TmpLine, s, 1);

					msg->Sender.Format("%s",TmpLine);
					msg->Acceptant="服务器";
					msg->Msg.Format("系统消息:    %s 正在认证身份！",TmpLine);
				}
			}
		}
	}
	catch(...)
	{
		WriteLog(IMSGPRO, "FormarMsnUSRSection() Error!! Thread=%d", GetCurrentThreadId());
	}

	return Line.GetLength() + 2;
}

//**********************************************************
//	格式化ADD命令。此命令增加一个用户
//**********************************************************
int FormarMsnADDSection(MsnSession *s, char *pbuf, LPMSNMSG msg,int len, int direction/*,LPCTSTR tmpsip, LPCTSTR tmpdip*/)
{
	ASSERT(pbuf!=NULL);
	CString Line;
	CString TmpLine;
	CString TmpNike;
	int		pos1=0,pos2=0;
	try
	{
		Line=GetLineFromBuffer(pbuf, len);
		
		pos2 = Line.Find(' ', pos1);
		pos1 = pos2 + 1;
		pos2 = Line.Find(' ', pos1);
		pos1 = pos2 + 1;
		pos2 = Line.Find(' ', pos1);
		
		TmpLine = Line.Mid(pos1, pos2 - pos1);
		if(TmpLine.Find("@")<0)
		{
			// Server --> Client
			msg->Direction = 2;
			pos1 = pos2 + 1;
			pos2 = Line.Find(' ', pos1);
			TmpLine = Line.Mid(pos1, pos2 - pos1);
			
			pos1 = pos2 + 1;
			pos2 = Line.Find(' ', pos1);
			TmpNike = Line.Mid(pos1, pos2 - pos1);
			TmpNike = NetBytestoAnsi(TmpNike, s->m_language);
			TmpNike = ConverHttp2Html(TmpNike);
			
			msg->Acceptant = s->m_userid;
			msg->Sender = "服务器";
			msg->Msg.Format("系统消息：    %s增加好友%s[%s]到好友列表成功",
				s->m_userid, TmpLine, TmpNike);
		}
		else
		{
			// Client --> Server
			msg->Direction = 1;
			pos1 = pos2 + 1;
			pos2 = Line.Find(' ', pos1);
			TmpNike = Line.Mid(pos1, pos2 - pos1);
			TmpNike = NetBytestoAnsi(TmpNike, s->m_language);
			TmpNike = ConverHttp2Html(TmpNike);
			
			msg->Sender = s->m_userid;
			msg->Acceptant = "服务器";
			msg->Msg.Format("系统消息：    %s增加好友%s[%s]到好友列表",
				s->m_userid, TmpLine, TmpNike);
		}
	}
	catch(...)
	{
		WriteLog(IMSGPRO, "FormarMsnADDSection() Error");
	}
	return Line.GetLength()+2;
}

//**********************************************************
//	格式化REM命令。此命令删除一个用户
//**********************************************************
int FormarMsnREMSection(MsnSession *s, char *pbuf, LPMSNMSG msg,int len, int direction/*,LPCTSTR tmpsip, LPCTSTR tmpdip*/)
{
	ASSERT(pbuf!=NULL);
	CString Line;
	CString TmpLine;
	int		pos1=0,pos2=0;
	try
	{
		Line = GetLineFromBuffer(pbuf, len);
		
		pos2 = Line.Find(' ', pos1);
		pos1 = pos2 + 1;
		pos2 = Line.Find(' ', pos1);
		pos1 = pos2 + 1;
		pos2 = Line.Find(' ', pos1);
		if (pos2 == -1)
		{
			TmpLine = Line.Mid(pos1);
		}
		else
		{
			TmpLine = Line.Mid(pos1, pos2 - pos1);
		}
		if(TmpLine.Find("@") < 0)
		{
			// Server --> Client
			msg->Direction=2;
			pos1 = pos2+1;
			pos2 = Line.Find(' ', pos1);
			if (pos2 == -1)
			{
				TmpLine = Line.Mid(pos1);
			}
			else
			{
				TmpLine = Line.Mid(pos1, pos2 - pos1);
			}
			
			msg->Acceptant = s->m_userid;
			msg->Sender = "服务器";
			msg->Msg.Format("系统消息：    %s从好友列表中删除用户%s成功",
				s->m_userid, TmpLine);
		}
		else
		{
			// Client --> Server
			msg->Direction = 1;
			msg->Sender = s->m_userid;
			msg->Acceptant = "服务器";
			msg->Msg.Format("系统消息：    %s从好友列表中删除用户%s",
				s->m_userid, TmpLine);
		}
	}
	catch(...)
	{
		WriteLog(IMSGPRO, "FormarMsnREMSection() Error!! Thread=%d", GetCurrentThreadId());
	}
	
	return Line.GetLength() + 2;
}

//**********************************************************
//	格式化JOI命令，此命令是响应对方的CAL命令的
//**********************************************************
int FormarMsnJOISection(MsnSession *s, char *pbuf, LPMSNMSG msg,int len, int direction)
{
	// JOI coolstar1980@msn.com z.coolstar 43215142\r\n
	// JOI coolstar1980@msn.com z.coolstar\r\n
	ASSERT(pbuf != NULL);
	ASSERT(s != NULL);
	
	CString Line;
	CString UserName;
	CString NikeName;
	int		pos1 = 0;
	int		pos2 = 0;
	
	try
	{
		Line = GetLineFromBuffer(pbuf, len);
		pos2 = Line.Find(' ', pos1);
		if(pos2 == -1)
		{
			return Line.GetLength() + 2;
		}
		pos1 = pos2 + 1;
		
		UserName = Line.Mid(0, pos1 - 1);
		if (UserName.Find('@') < 0)
		{
			return Line.GetLength() + 2;
		}

		pos2 = Line.Find(' ', pos1);
		if(pos2 > 0)
		{
			NikeName = Line.Mid(pos1, pos2 - pos1);
		}
		
		if (!NikeName.IsEmpty())
		{
			NikeName = ConverHttp2Html(NikeName);
			NikeName = NetBytestoAnsi(NikeName, s->m_language);
			msg->Sender.Format("%s[%s]", NikeName, UserName);
		}
		else
		{
			msg->Sender = UserName;
		}
		

		// Server --> Client
		msg->Direction = 2;
		msg->Acceptant = s->m_userid;
		
		msg->Msg.Format("系统消息 : %s 响应 %s 的呼叫", msg->Sender, msg->Acceptant);
		
		SetSessionUserInfo(msg->Sender, s, 2);
	}
	catch(...)
	{
		WriteLog(IMSGPRO, "FormarMsnJOISection() Error!! Thread=%d", GetCurrentThreadId());
	}
	
	return Line.GetLength() + 2;
}

//**********************************************************
//	格式化CAL命令。此命令呼叫一个人，发起一次会话
//**********************************************************
int FormarMsnCALSection(MsnSession *s, char *pbuf, LPMSNMSG msg,int len, int direction/*,LPCTSTR tmpsip, LPCTSTR tmpdip*/)
{
	ASSERT(pbuf != NULL);
	
	CString Line;
	CString TmpLine;
	int		pos1 = 0;
	int		pos2 = 0;
	
	try
	{
		Line = GetLineFromBuffer(pbuf, len);
		
		pos2 = Line.Find(' ', pos1);
		if(pos2 == -1)
		{
			return (len > 100)? -1 : 0;
		}
		pos1 = pos2 + 1;
		
		pos2 = Line.Find(' ', pos1);
		if(pos2 == -1)
		{
			pos2 = Line.GetLength();
			TmpLine = Line.Mid(pos1, pos2 - pos1);
			
			if ((pos2 - pos1) > 7)
			{
				msg->Acceptant = TmpLine;
				msg->Sender = s->m_userid;
				msg->Msg.Format("系统消息:    %s 呼叫 %s", msg->Sender, TmpLine);
				msg->Direction = 1;
			}
			
			//==========================================================================================
			//	注意：从呼叫上得来的用户名是对方的用户名
			//==========================================================================================
			SetSessionUserInfo(TmpLine, s, 2);
		}
	}
	catch(...)
	{
		WriteLog(IMSGPRO, "FormarMsnCALSection() Error!! Thread=%d", GetCurrentThreadId());
	}
	return Line.GetLength() + 2;
}

//**********************************************************
//	格式化ANS命令
//**********************************************************
int FormarMsnANSSection(MsnSession *s, char *pbuf, LPMSNMSG msg,int len, int direction/*,LPCTSTR tmpsip, LPCTSTR tmpdip*/)
{
	ASSERT(pbuf!=NULL);
	CString Line;
	Line=GetLineFromBuffer(pbuf, len);
	return Line.GetLength()+2;
}

CString SplitMsnName(CString msnname)
{
	int n;
	CString tempmsn;
	tempmsn=msnname;
	n=tempmsn.Find("[");
	if (n>=0 && tempmsn.GetLength()>n+1)
	{
		tempmsn=tempmsn.Mid(n+1);
		n=tempmsn.Find("]");
		if (n>=0)
			tempmsn=tempmsn.Left(n);
	}
	return tempmsn;
}
