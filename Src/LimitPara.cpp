// LimitPara.cpp: implementation of the LimitPara class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "DcePub.h"
#include "LimitPara.h"
#include "SocketSession.h"
#include "SyncTable.h"
#include "zwsqldb.h"

 CaseLimitPara		v_CaseLimitPara;
 ccicpara			v_ccicpara;

 IndexLimitPara		v_IndexLimitPara;
 RubbishPara		v_RubbishPara;
 CustomUrlFieldList	v_CustUrlList[2];
 long				v_CustUrlAccID = 0;
 CustomUrlFieldList	v_HttpregistList[2];
 long				v_HttpregistAccID;
 IndexMap		  v_Indexlimtmap[2];
 long				v_IdexlimitID;

 MobileqqMap	g_mobileqqMap;
 MobileqqKeyMap g_mobileqqkeymap;
 MobileqqpwdkeyMap	g_mobileqqpwdkeymap;
 MobileqqStateMap	g_mobileqqstateMap;
 mobileqqWeiboMap	v_mobileqqWeiboMap;
 MomoMap	v_MomoMap;
 KuaidiMap	v_kuaidi;
 DiDiMap v_DiDiMap;
 sshMap	v_sshMap;
 
 long				v_IDCHttpKeyflag = 0;
 IDCHttpKeyWordList v_IDCHttpKeyLilt[2];
 IDCSvrPort			v_IDCSvrPort;
 CMapStringToString v_localmac;
 
 imsiphone	v_imsiphone;
CaseInfo::~CaseInfo()
{
}

CaseInfo::CaseInfo()
{
	LimitType=0;
}

CaseInfo::CaseInfo(CString& filter)
{
// 	LimitType = 50;
// 	Keynum = 0;
// 	SearchKey[0]=0;
// 	SearchKey[1]=0;
// 	LimitAddr = filter;
// 	InitSearch(LimitAddr);
//	if (printpara)
//		WriteLog("dce", "����HTTP���˲���: %s", LimitAddr);	
}
CaseInfo::CaseInfo(LPLIMITDATA plimit)
{
	CaseID = plimit->caseid;
	CaseID.TrimRight();
	CString sobjtype, sobjname;
	sobjtype = plimit->objtype;
	sobjtype.TrimRight();
	sobjname = plimit->objname;
	sobjname.TrimRight();
	ObjectID.Format("%s_%s", sobjtype, sobjname);
	AlarmState = plimit->AlarmState;
	LimitId = plimit->limitid;
	nocappro = plimit->nocappro;		//���ػ�Э��
	webalarmpro = plimit->webalarmpro;	//ҳ��澯Э��
	handalarmpro = plimit->handaarmpro;

// 	Keynum = 0;
// 	SearchKey[0]=0;
// 	SearchKey[1]=0;
	LimitAddr = plimit->limit;
	LimitAddr.TrimRight();
	switch (LimitType = plimit->ltype)
	{
	case 18:		// KeyWord
		if (plimit->ifexpand==1)
			LimitType += 200;
	case 19:		// ����KeyWord
// 		InitSearch(LimitAddr);
		break;
	case 14:		// URL
		if (plimit->ifexpand==1)
			LimitType += 200;
// 	case 17:		// phone
// 		LimitAddr.MakeLower();
// 		SearchKey[0] = new Search(LimitAddr);
// 		Keynum = 1;
// 		break;
// 	case 16:		// �˺�
// 		if (strnicmp(LimitAddr, "vlanid_", 7)!=0)
// 		{
// 			CString straccount = LimitAddr;
// 			straccount.MakeLower();
// 			SearchKey[0] = new Search(straccount);
// 			Keynum = 1;
// 		}
// 		break;
// 	case 34:
// 		LimitAddr.MakeLower();
// 		SearchKey[0] = new Search(LimitAddr.Mid(16));
// 		Keynum = 1;
// 		break;
	default:
		break;
	}
// 	
// //	if (printpara)
// //		WriteLog("dce", "����Limit: %s\t%s\t%d\t%s\t%d", CaseID, ObjectID, LimitType, LimitAddr, AllWacthFlg);
}

CaseInfo::InitSearch(LPCTSTR strKey)
{
// 	char *p = strstr(strKey, "\x80\x80\x80\x80\x80\x80");
// 	CString Key1, Key2;
// 	Keynum = 1;
// 	if (p != NULL)
// 	{
// 		*p = 0;
// 		SearchKey[0] = new Search((LPCTSTR)strKey);
// 		Keynum = 2;
// 		SearchKey[1] = new Search((LPCTSTR)p+6);
// 		memcpy(p, " ���� ", 6);
// 	}
// 	else
// 	{
// 		SearchKey[0] = new Search((LPCTSTR)strKey);
// 	}
// 	LimitAddr = strKey;
}

 BOOL SearchCaseByURL(const char* ptext, int tlen, CaseInfo& caseinfo)
{
	try
	{
// 		CaseKeyWordList& urllist = v_CaseLimitPara.m_urllist[v_CaseLimitPara.m_readflg];
// 		typedef CaseKeyWordList::iterator CaseInfoItem;
// 		CaseInfoItem item;
// 		for (item = urllist.begin(); item != urllist.end(); item++)
// 		{
// 			if (-1 != item->SearchKey[0]->find((char*)ptext, tlen))
// 			{
// 				caseinfo = *item;
// 				caseinfo.SearchKey[0] = caseinfo.SearchKey[1] = 0;
// 				return TRUE;
// 			}
// 		}
	}
	catch (...)
	{
		WriteLog("dce", "SearchCaseByURL() Error ptext=%s", ptext);
	}
	return FALSE;
}
 
 //�ȶ�ip+port����
 int SearchCaseByIPPORT(CString sipport, CString dipport, CaseInfoArray& caseinfoarr)
{
	CaseIpMacMap &ipportmap = v_CaseLimitPara.m_ipportmap[v_CaseLimitPara.m_readflg];
	CaseInfoArray* pcs = NULL;
	if (ipportmap.Lookup(sipport, pcs))
	{
		for (int i = 0;i < pcs->GetSize(); i++)
		{
			caseinfoarr.Add(pcs->GetAt(i));
		}
		return 1;
	}
	else if (ipportmap.Lookup(dipport, pcs))
	{
		for (int i = 0;i < pcs->GetSize(); i++)
		{
			caseinfoarr.Add(pcs->GetAt(i));
		}
		return 2;
	}
	return 0;
}
int  SearchCaseByMail(const char* userid,
					CaseInfoArray& caseinfoarr)
{
	CaseIpMacMap &emalmap = v_CaseLimitPara.m_emailmap[v_CaseLimitPara.m_readflg];
	CaseInfoArray* pcs = NULL;
	if (emalmap.Lookup(userid, pcs))
	{
		caseinfoarr.Append(*pcs);
		return 1;
	}
	return 0;
}

int  SearchCaseByIm(const char* userid,
					CaseInfoArray& caseinfoarr)
{
	CaseIpMacMap &Immap = v_CaseLimitPara.m_immap[v_CaseLimitPara.m_readflg];
	CaseInfoArray* pcs = NULL;
	if (Immap.Lookup(userid, pcs))
	{
		caseinfoarr.Append(*pcs);
		return 1;
	}
	return 0;

}
int  SearchCaseByIP(const char* strsip, const char* strdip,
					CaseInfoArray& caseinfoarr, CMapStringToString *m_ridcasemap)
{
	CaseIpMacMap &ipmap = v_CaseLimitPara.m_ipmap[v_CaseLimitPara.m_readflg];
	CaseInfoArray* pcs = NULL;
	if (ipmap.Lookup(strsip, pcs))
	{
// 		caseinfo = *pcs;
// 		caseinfo.SearchKey[0] = caseinfo.SearchKey[1] = 0;
		if(m_ridcasemap != NULL)
		{
			for (int i = 0; i < pcs->GetSize(); i++)
			{
				CString tmpstr;
				if (!m_ridcasemap->Lookup(pcs->GetAt(i).LimitAddr, tmpstr))
				{
					caseinfoarr.Add(pcs->GetAt(i));
				}

			}
		}
		else
		{
			caseinfoarr.Append(*pcs);
		}
		return 1;
	}
	else if (ipmap.Lookup(strdip, pcs))
	{
// 		caseinfo = *pcs;
// 		caseinfo.SearchKey[0] = caseinfo.SearchKey[1] = 0;

		if(m_ridcasemap != NULL)
		{
			for (int i = 0; i < pcs->GetSize(); i++)
			{
				CString tmpstr;
				if (!m_ridcasemap->Lookup(pcs->GetAt(i).LimitAddr, tmpstr))
				{
					caseinfoarr.Add(pcs->GetAt(i));
				}

			}
		}
		else
		{
			caseinfoarr.Append(*pcs);
		}

		return 2;
	}
// 	else if (v_programtype == SYSTEM_SLEUTH)
// 		return SearchRealIP(strsip, strdip, caseinfo);
	else		
		return 0;
}

//  BOOL SearchCaseByVlan(WORD vlanid, CaseInfo& caseinfo)
// {
// 	ASSERT(vlanid>0 && vlanid<0x1000);
// 	if (vlanid>0 && vlanid<0x1000)
// 	{
// 		CaseInfo* pcs = v_CaseLimitPara.m_vlanid[v_CaseLimitPara.m_readflg][vlanid];
// 		if (pcs!=NULL)
// 		{
// // 			caseinfo = *pcs;
// // 			caseinfo.SearchKey[0] = caseinfo.SearchKey[1] = 0;
// 			return TRUE;
// 		}
// 	}
// 	return FALSE;
// }


 int  SearchCaseByMAC(const char* strsmac, const char* strdmac,
								  CaseInfoArray& caseinfoarr,  CMapStringToString *m_ridcasemap)
{
	CaseIpMacMap &macmap = v_CaseLimitPara.m_macmap[v_CaseLimitPara.m_readflg];
	CaseInfoArray* pcs = NULL;
	if (macmap.Lookup(strsmac,pcs))
	{
// 		caseinfo = *pcs;
// 		caseinfo.SearchKey[0] = 0;
// 		caseinfo.SearchKey[1] = 0;
		if(m_ridcasemap != NULL)
		{
			for (int i = 0; i < pcs->GetSize(); i++)
			{
				CString tmpstr;
				if (!m_ridcasemap->Lookup(pcs->GetAt(i).LimitAddr, tmpstr))
				{
					caseinfoarr.Add(pcs->GetAt(i));
				}

			}
		}
		else
		{
			caseinfoarr.Append(*pcs);
		}

		return 1;
	}
	else if (macmap.Lookup(strdmac, pcs))
	{
// 		caseinfo = *pcs;
// 		caseinfo.SearchKey[0] = 0;
// 		caseinfo.SearchKey[1] = 0;

		if(m_ridcasemap != NULL)
		{
			for (int i = 0; i < pcs->GetSize(); i++)
			{
				CString tmpstr;
				if (!m_ridcasemap->Lookup(pcs->GetAt(i).LimitAddr, tmpstr))
				{
					caseinfoarr.Add(pcs->GetAt(i));
				}

			}
		}
		else
		{
			caseinfoarr.Append(*pcs);
		}

		return 2;
	}
	else
		return 0;
}

 BOOL SearchCaseByKey(const char* ptext, int tlen, CaseInfo& caseinfo, BOOL isjz)
{
	// ��BTreeƥ�䣬URL(14), �ؼ���(18), ����ؼ���(19)ͬʱƥ��
	try
	{
//		CaseKeyWordList& keylist = v_CaseLimitPara.m_keywordlist[v_CaseLimitPara.m_readflg];
//		typedef CaseKeyWordList::iterator CaseInfoItem;
//		CaseInfoItem item;
//		for (item = keylist.begin(); item != keylist.end(); item++)
//		{
//			if (-1 != item->SearchKey[0]->find((char*)ptext, tlen))
//			{
//				if ((item->SearchKey[1]==NULL) ||
//					(-1 != item->SearchKey[1]->find((char*)ptext, tlen)))
//				{
//					caseinfo = *item;
//					caseinfo.SearchKey[0] = 0;
//					caseinfo.SearchKey[1] = 0;
//					return TRUE;
//				}
//			}
//		}
// 		if (MatchingDicAndHaveReturn(&v_CaseLimitPara.m_keyworddic[v_CaseLimitPara.m_readflg],(const unsigned char *)ptext, tlen, caseinfo,TRUE)) 
// 		{
// 			return TRUE;
// 		}
		
//		if (isjz)
		//20070305 lp jz�ؼ��ֶ�����Э�鶼������
		{
			// ��Ҫƥ�似��Ĺؼ���
//			CaseKeyWordList& jzkeylist = v_CaseLimitPara.m_jzkeywordlist[v_CaseLimitPara.m_readflg];
//			CaseInfoItem jzitem;
//			for (jzitem = jzkeylist.begin(); jzitem != jzkeylist.end(); jzitem++)
//			{
//				if (-1 != jzitem->SearchKey[0]->find((char*)ptext, tlen))
//				{
//					if ((jzitem->SearchKey[1]==NULL) ||
//						(-1 != jzitem->SearchKey[1]->find((char*)ptext, tlen)))
//					{
//						caseinfo = *jzitem;
//						caseinfo.SearchKey[0] = 0;
//						caseinfo.SearchKey[1] = 0;
//						return TRUE;
//					}
//				}
//			}
// 			if (MatchingDicAndHaveReturn(&v_CaseLimitPara.m_jzkeyworddic[v_CaseLimitPara.m_readflg],(const unsigned char *)ptext, tlen,caseinfo,TRUE)) 
// 			{
// 				return TRUE;
// 			}
		}
		return FALSE;
	}
	catch (...)
	{
		WriteLog("dce", "SearchCaseByKey error ptext=%s", ptext);
		return FALSE;
	}

}

 CString SearchIDCHttpKey(const char* filename)
{
	CString str;
	char* ptxt;
	char buf[10*1024];
	int tlen=GetFileLength(filename);
	if(tlen<=0)
		return "";
	FILE * fs=0;
	fs=fopen(filename,"rb");
	if(fs>0	)
	{
		if (tlen>=sizeof(buf)-10)
			ptxt = str.GetBufferSetLength(tlen+10);
		else
			ptxt = buf;
		memset(ptxt,0,tlen+10);
		fread(ptxt,1,tlen,fs);
		fclose(fs);
	}
	else
		return "";
	BOOL isfindkey=FALSE;
	typedef IDCHttpKeyWordList::iterator IDCKeyWordItem;
	IDCKeyWordItem item;
	IDCHttpKeyWordList& httpfilterlist = v_IDCHttpKeyLilt[v_IDCHttpKeyflag];
	for (item = httpfilterlist.begin(); item != httpfilterlist.end(); item++)
	{
		if (item->keyword1.find(ptxt, tlen) > -1)
		{
			if (item->num == 2)
			{
				if (item->keyword2.find(ptxt, tlen) > -1)
					return item->servertype;
			}
			else
			{
				return item->servertype;
			}
		}
	}
	return "";
}

 BOOL SearchHttpFilter(const char* filename,const char* sip,const char* dip,const char* url,const char* host,CString pro,CStringArray* arrfile/*, CaseInfo& caseinfo*/)
{
	if((v_programtype == SYSTEM_SLEUTH) ||
		(v_programtype == SYSTEM_SZFJ))
		return TRUE;
	if (pro.CompareNoCase("httpdown")==0||
		pro.CompareNoCase("httpup")==0)
	{
		CString str;
		char* ptxt;
		char buf[10*1024];
		int tlen=GetFileLength(filename);
		if(tlen<=0)
			return FALSE;
		FILE * fs=0;
		fs=fopen(filename,"rb");
		if(fs>0	)
		{
			if (tlen>=sizeof(buf)-10)
				ptxt = str.GetBufferSetLength(tlen+10);
			else
				ptxt = buf;
			memset(ptxt,0,tlen+10);
			fread(ptxt,1,tlen,fs);
			fclose(fs);
		}
		else
			return FALSE;
		BOOL isfindkey=FALSE;
		CStringArray toarr;
		CString timestr;
		SYSTEMTIME nowtm;
		GetLocalTime(&nowtm);
		timestr.Format("%04d-%02d-%02d %02d:%02d:%02d",
			nowtm.wYear, nowtm.wMonth, nowtm.wDay,
			nowtm.wHour, nowtm.wMinute, nowtm.wSecond);
		CString dipport,sipport;
		dipport.Format("%s",dip);
		dipport=dipport.Right(5);
		sipport.Format("%s",sip);
		sipport=sipport.Right(5);

		matchcallpara m_matchcallpara;
		m_matchcallpara.state = 0;
		v_IndexLimitPara.m_keyworddic[v_IndexLimitPara.m_keywordreadflg].MatchingDic((const unsigned char*)ptxt, tlen, &HttpfiltMatchingKey, &m_matchcallpara);
		if (m_matchcallpara.state)
		{
			return TRUE;
		}
/*		typedef IndexKeyWordList::iterator KeyWordItem;
		KeyWordItem item;
		IndexKeyWordList& httpfilterlist = v_IndexLimitPara.m_keywordlist[v_IndexLimitPara.m_keywordreadflg];
		for (item = httpfilterlist.begin(); item != httpfilterlist.end(); item++)
		{
			if (item->find(ptxt, tlen) > -1)
			{
//				if (v_stat)
//				{
//					CString keystr;
//					keystr.Format("%s\1",timestr);
//					
//					keystr=keystr+sip+'\1'+dip+'\1'+sipport+'\1'+dipport+'\1'+sip+'\1'+dip;
//					keystr.Format("%s\1%s\1%s\1%s\1%s",(CString)keystr,item->patt,pro,url,host);
//					toarr.Add(keystr);
//				}
//				else
					return TRUE;
			}
		}*/
		CaseInfo caseinfo;
// 		if (MatchingDicAndHaveReturn(&v_IndexLimitPara.m_keyworddic[v_IndexLimitPara.m_keywordreadflg],(const unsigned char *)ptxt, tlen, caseinfo)) 
// 		{
// 			return TRUE;
// 		}
		
//		if (toarr.GetSize()>0)
//		{
//			WriteArrToData(HTTP_PROTOCOL, INDEXKEY_DATA, "", &toarr);
//			toarr.RemoveAll();
//			return TRUE;
//		}
//		else
// 			return FALSE;
	}
	else if (arrfile!=NULL)
	{
		BOOL isfindkey=FALSE;
		for(int hh=0;hh<arrfile->GetSize();hh++)
		{
			CString tmpfile = arrfile->GetAt(hh);
			if (tmpfile.GetLength()>2)
			{
				if (tmpfile.GetAt(tmpfile.GetLength()-2)=='*')
				{
					if (tmpfile.GetAt(tmpfile.GetLength()-1)!='1')
						continue;	// ��ʵ��
					tmpfile.ReleaseBuffer(tmpfile.GetLength()-2);
				}
			}
			else
				continue;
			CString str;
			char* ptxt;
			char buf[10*1024];
			int tlen=GetFileLength(tmpfile);
			if(tlen<=0)
				continue;
			FILE * fs=0;
			fs=fopen(tmpfile,"rb");
			if(fs>0	)
			{
				if (tlen>=sizeof(buf)-10)
					ptxt = str.GetBufferSetLength(tlen+10);
				else
					ptxt = buf;
				memset(ptxt,0,tlen+10);
				fread(ptxt,1,tlen,fs);
				fclose(fs);
			}
			else
				continue;
			CStringArray toarr;
			CString timestr;
			SYSTEMTIME nowtm;
			GetLocalTime(&nowtm);
			timestr.Format("%04d-%02d-%02d %02d:%02d:%02d",
				nowtm.wYear, nowtm.wMonth, nowtm.wDay,
				nowtm.wHour, nowtm.wMinute, nowtm.wSecond);
			CString dipport,sipport;
			dipport.Format("%s",dip);
			dipport=dipport.Right(5);
			sipport.Format("%s",sip);
			sipport=sipport.Right(5);

/*			typedef IndexKeyWordList::iterator KeyWordItem;
			KeyWordItem item;
			IndexKeyWordList& httpfilterlist = v_IndexLimitPara.m_keywordlist[v_IndexLimitPara.m_keywordreadflg];
			for (item = httpfilterlist.begin(); item != httpfilterlist.end(); item++)
			{
				if (item->find(ptxt, tlen) > -1)
				{
//					if (v_stat)
//					{
//						CString keystr;
//						keystr.Format("%s\1",timestr);
//						
//						keystr=keystr+sip+'\1'+dip+'\1'+sipport+'\1'+dipport+'\1'+sip+'\1'+dip;
//						keystr.Format("%s\1%s\1%s\1%s\1%s",(CString)keystr,item->patt,pro,url,host);
//						toarr.Add(keystr);
//					}
//					else
						return TRUE;
				}
			}
			*/
// 			CaseInfo caseinfo;
// 			if (MatchingDicAndHaveReturn(&v_IndexLimitPara.m_keyworddic[v_IndexLimitPara.m_keywordreadflg],(const unsigned char *)ptxt, tlen, caseinfo)) 
// 			{
// 				return TRUE;
// 			}
			
//			if (toarr.GetSize()>0)
//			{
//				if (pro.CompareNoCase("pop3")==0)
//					WriteArrToData(POP3_PROTOCOL, INDEXKEY_DATA, "", &toarr);
//				else if (pro.CompareNoCase("smtp")==0)
//					WriteArrToData(SMTP_PROTOCOL, INDEXKEY_DATA, "", &toarr);
//				toarr.RemoveAll();
//				return TRUE;
//			}
//			else
// 				return FALSE;
		}
	}
	else
		return FALSE;
	return FALSE;
}

 BOOL SearchCaseFileByKey(const char* filename,  CaseInfo& caseinfo, BOOL isjz)
{
	try
	{
		int bufid = 0;
		UINT flen;
		char *ptext=0;
		if(caseinfo.CaseID!="")
			return TRUE;
		
		char buf[10*1024];
		CString s;
		flen=GetFileLength(filename);
		if (flen>sizeof(buf)-10)
		{
			ptext = s.GetBufferSetLength(flen+10);
			memset(ptext+flen,0,9);
		}
		else if (flen > 0)
		{
			ptext = buf;
			memset(ptext+flen,0,9);
		}
		else
			return FALSE;
		
		FILE * fs=0;
		fs=fopen(filename,"rb");
		if(fs==0)
			return FALSE;
		
		UINT readsize=0;
		readsize=fread(ptext,1,flen,fs);
		fclose(fs);
		return SearchCaseByKey(ptext, readsize, caseinfo, isjz);
	}
	catch(...)
	{
		WriteLog("dce", "SearchCaseFileByKey() Error");
	}
	return FALSE;
}

BOOL SearchIndexByIPPORT(CString sipport, CString dipport)
{
	CString tmpstr;
	if (v_IndexLimitPara.m_ipportmap[v_IndexLimitPara.m_limitreadflg].Lookup(sipport,tmpstr))
	{
		return TRUE;
	}
	if (v_IndexLimitPara.m_ipportmap[v_IndexLimitPara.m_limitreadflg].Lookup(dipport,tmpstr))
	{
		return TRUE;
	}
	return	FALSE;
}

 int SearchIndexByIPURL(const char* ip1, const char* ip2, const char* strurl, int urllen)
{
	try
	{
		IndexIpUrlMap& indexipurlmap = v_IndexLimitPara.m_ipurlmap[v_IndexLimitPara.m_limitreadflg];
		IndexKeyWordList* purllist = NULL;
		if (indexipurlmap.Lookup(ip1, purllist) || indexipurlmap.Lookup(ip2, purllist))
		{
			ASSERT(purllist != NULL);
			typedef IndexKeyWordList::iterator IndexUrlItem;
			IndexUrlItem item;
			for (item = purllist->begin(); item != purllist->end(); item++)
			{
				if (-1 != item->find((char*)strurl, urllen))
				{
					return 3;
				}
			}
//			return 1;
		}
	}
	catch(...)
	{
		WriteLog("dce", "SearchIndexByIPURL() Error");
	}
	return 0;
}

 int SearchCaseByIPURL(const char* ip1, const char* ip2, const char* strurl, int urllen, CaseInfo& caseinfo)
{
	try
	{
		if(caseinfo.CaseID!="")
			return 3;
// 		CaseIpUrlMap& caseipurlmap = v_CaseLimitPara.m_ipurlmap[v_CaseLimitPara.m_readflg];
// 		CaseKeyWordList* purllist = NULL;
// 		if (caseipurlmap.Lookup(ip1, purllist) || caseipurlmap.Lookup(ip2, purllist))
// 		{
// 			ASSERT(purllist != NULL);
// 			typedef CaseKeyWordList::iterator CaseInfoItem;
// 			CaseInfoItem item;
// 			for (item = purllist->begin(); item != purllist->end(); item++)
// 			{
// // 				if (-1 != item->SearchKey[0]->find((char*)strurl, urllen))
// // 				{
// // 					caseinfo = *item;
// // 					caseinfo.SearchKey[0] = 0;
// // 					caseinfo.SearchKey[1] = 0;
// // 					return 3;
// // 				}
// 			}
// 			return 1;
// 		}
	}
	catch(...)
	{
		WriteLog("dce", "SearchCaseByIPURL() Error");
	}
	return 0;
}

 BOOL SearchCaseByAAA(LPCTSTR phone, LPCTSTR account, CaseInfoArray& caseinfoarr, CMapStringToString *m_ridcasemap)
{
	try
	{
		CString lowerphone,loweraccount;
		loweraccount.Format("%s",account);
		loweraccount.MakeLower();
		lowerphone.Format("%s",phone);
		lowerphone.MakeLower();
		CaseIpMacMap &phoneaccountmap = v_CaseLimitPara.m_phoneaccountmap[v_CaseLimitPara.m_readflg];
		CaseInfoArray* pcs = NULL;
		if (phoneaccountmap.Lookup(lowerphone, pcs))
		{
			if(m_ridcasemap != NULL)
			{
				for (int i = 0; i < pcs->GetSize(); i++)
				{
					CString tmpstr;
					if (!m_ridcasemap->Lookup(pcs->GetAt(i).LimitAddr, tmpstr))
					{
						caseinfoarr.Add(pcs->GetAt(i));
					}

				}
			}
			else
			{
				caseinfoarr.Append(*pcs);
			}
			return 1;
		}
		else if (phoneaccountmap.Lookup(loweraccount, pcs))
		{
	// 		caseinfo = *pcs;
	// 		caseinfo.SearchKey[0] = caseinfo.SearchKey[1] = 0;

			if(m_ridcasemap != NULL)
			{
				for (int i = 0; i < pcs->GetSize(); i++)
				{
					CString tmpstr;
					if (!m_ridcasemap->Lookup(pcs->GetAt(i).LimitAddr, tmpstr))
					{
						caseinfoarr.Add(pcs->GetAt(i));
					}

				}
			}
			else
			{
				caseinfoarr.Append(*pcs);
			}

			return 2;
		}
		else		
			return 0;
	}
	catch(...)
	{
		WriteLog("dce", "SearchCaseByAAA() Error");
	}
	return 0;
}
//
//// int  SearchCaseByIPAndAAA(const char* strsip, const char* strdip, CaseInfo& caseinfo);
////
 BOOL SearchBruuishByAAA(const char* strphone, const char* struser)
{
	if (v_programtype == SYSTEM_SLEUTH)
		return FALSE;
	CMapStringToPtr& phonemap = v_RubbishPara.m_rubbaaamap[v_RubbishPara.m_readflg];
	void *val = NULL;
	char buf[100];
	memset(buf, 0, sizeof(buf));
	int len = strlen(strphone);
	if (len>0)
	{
		memcpy(buf, strphone, min(96, len));
		strlwr(buf);
		if (phonemap.Lookup(strphone, val))
			return TRUE;
	}
	len = strlen(struser);
	if (len > 0)
	{
		memset(buf, 0, sizeof(buf));
		memcpy(buf, struser, min(96, len));
		strlwr(buf);
		if (phonemap.Lookup(buf, val))
			return TRUE;
	}
	return FALSE;
}

 BOOL SearchBurrishByURL(CString strurl)
{
	if (v_programtype == SYSTEM_SLEUTH)
		return FALSE;
	
	IndexKeyWordList& rubburllist = v_RubbishPara.m_rubburllist[v_RubbishPara.m_readflg];
	typedef IndexKeyWordList::iterator KeyWordItem;
	KeyWordItem item;
	for (item = rubburllist.begin(); item != rubburllist.end(); item++)
	{
		if (item->find(strurl, strurl.GetLength()) > -1)
			return TRUE;
	}
	return FALSE;
}

 BOOL SearchBurrishByMail(CString strmail)
{
	if (v_programtype == SYSTEM_SLEUTH)
		return FALSE;

 	IndexKeyWordList& rubbmaillist = v_RubbishPara.m_rubbmaillist[v_RubbishPara.m_readflg];
	typedef IndexKeyWordList::iterator KeyWordItem;
	KeyWordItem item;
	for (item = rubbmaillist.begin(); item != rubbmaillist.end(); item++)
	{
		if (item->find(strmail, strmail.GetLength()) > -1)
			return TRUE;
	}
	return FALSE;
}

 BOOL SearchBurrishByIP(const char* ip1, const char* ip2)
{
	if (v_programtype == SYSTEM_SLEUTH)
		return FALSE;
	CMapStringToPtr& rubbipmap = v_RubbishPara.m_rubbipmap[v_RubbishPara.m_readflg];
	void* v = 0;
	if (rubbipmap.Lookup(ip1, v))
	{
		ASSERT((int)v == 8);
	}
	else if (rubbipmap.Lookup(ip2, v))
	{
		ASSERT((int)v == 8);
	}
	return ((int)v == 8);
}

//  BOOL SearchCaseForSession(SocketSession* s, BOOL opencaseflag, time_t nowtm)
// {
// 	AAAInfo *pAAA;
// 	AAAInfo aaa;
// 	CaseInfo caseinfo;
// 	if (s->m_dataSrc != DATASRC_CDMA)
// 	{
// 		if (NULL != (pAAA = GetAAAInfo(*((DWORD*)s->csip), *((DWORD*)s->cdip), aaa, nowtm)))
// 		{
// 			s->UserAccount1 = pAAA->UserAccount1;
// 			s->Phone1 = pAAA->Phone1;
// 			s->UserAccount2 = pAAA->UserAccount2;
// 			s->Phone2 = pAAA->Phone2;
// 			if (!pAAA->CaseID.IsEmpty() && opencaseflag)
// 			{
// 				s->CaseID = pAAA->CaseID;
// 				s->LimitAddr = pAAA->LimitAddr;
// 				s->LimitType = pAAA->LimitType;
// 				s->ObjectID = pAAA->ObjectID;
// 			}
// 			else
// 			{
// 				s->rubbishflag = SearchBruuishByAAA(pAAA->Phone1, pAAA->UserAccount1);
// 				if (!s->rubbishflag)
// 					s->rubbishflag = SearchBruuishByAAA(pAAA->Phone2, pAAA->UserAccount2);
// 			}
// 		}
// 		else if (s->m_vLanID!=0)
// 		{
// 			// û��ƥ�����ʺžͰ�vlan���뵹�ʺ��ֶ���
// 			s->UserAccount1.Format("VLanID_%d", s->m_vLanID);
// 			// ƥ��Vlan������
// 			if (opencaseflag && SearchCaseByVlan(s->m_vLanID, caseinfo))
// 			{
// 				s->CaseID = caseinfo.CaseID;
// 				s->LimitAddr = caseinfo.LimitAddr;
// 				s->LimitType = caseinfo.LimitType;
// 				s->ObjectID = caseinfo.ObjectID;
// 			}
// 			else
// 			{
// 				s->rubbishflag = SearchBruuishByAAA("", s->UserAccount1);
// 			}
// 		}
// 	}
// 	else
// 	{
// 		if (opencaseflag && (NULL!=SearchCaseByAAA(s->Phone1, s->UserAccount1, &aaa)))
// 		{
// 			s->CaseID = aaa.CaseID;
// 			s->LimitAddr = aaa.LimitAddr;
// 			s->LimitType = aaa.LimitType;
// 			s->ObjectID = aaa.ObjectID;
// 		}
// 		else
// 		{
// 			s->rubbishflag = SearchBruuishByAAA(s->Phone1, s->UserAccount1);
// 		}
// 	
// 	}
// 	if (opencaseflag && s->CaseID.IsEmpty())
// 	{
// 		char sip[32];
// 		char dip[32];
// 		memset(sip+12, 0, 4);
// 		memcpy(sip, (LPCTSTR)s->sip, 15);
// 		memset(dip+12, 0, 4);
// 		memcpy(dip, (LPCTSTR)s->dip, 15);
// 		if (SearchCaseByIP(sip, dip, caseinfo))
// 		{
// 			s->CaseID = caseinfo.CaseID;
// 			s->LimitAddr = caseinfo.LimitAddr;
// 			s->LimitType = caseinfo.LimitType;
// 			s->ObjectID = caseinfo.ObjectID;
// 		}
// 		else if(!s->rubbishflag)
// 		{
// 			s->rubbishflag = SearchBurrishByIP(sip, dip);
// 		}
// 	}
// 	if ((s->m_dataSrc != DATASRC_CDMA) && opencaseflag && s->CaseID.IsEmpty())
// 	{
// 		ASSERT(!s->smac.IsEmpty());
// 		ASSERT(!s->dmac.IsEmpty());
// 		if (SearchCaseByMAC(s->smac, s->dmac, caseinfo))
// 		{
// 			s->CaseID = caseinfo.CaseID;
// 			s->LimitAddr = caseinfo.LimitAddr;
// 			s->LimitType = caseinfo.LimitType;
// 			s->ObjectID = caseinfo.ObjectID;
// 		}
// 	}
// 	return 0;
// }

// BOOL SearchUnipPort(WORD port1, WORD port2, int packettype)
// {
// 	BYTE* tp = v_IndexLimitPara.m_port[v_IndexLimitPara.m_limitreadflg];
// 	if (tp[port1]==0x55 || tp[port2]==0x55)
// 		return TRUE;
// 	else
// 		return FALSE;
// }

//typedef struct QQPWD
//{
//	QQPWD*	pNext;
//	char	password[20];
//	char	pwd2md5[16];
//};
//
//typedef	CMap<DWORD, DWORD, QQPWD*, QQPWD*&> OicqPasswrdMap;
//
//class  
//{
//public:
//	RubbishPara()
//	{
//		m_pFreeList = NULL;
//		m_readflg = 0;
//	}
//	
//	long	m_readflg;
//	OicqPasswrdMap m_qqpwdmap[2];
//	
//	QQPWD*	m_pFreeList;
//	
//};
QQPWD* OicqPwdPara::GetNewUnit()
{
	QQPWD* tp;
	if (m_pFreeList==NULL)
	{
		tp = new QQPWD[1024];
		for (int i=0; i<1024; i++)
		{
			tp->pNext = m_pFreeList;
			m_pFreeList = tp;
			tp++;
		}
	}
	tp = m_pFreeList;
	m_pFreeList = tp->pNext;
	return tp;
}

void OicqPwdPara::ReleaseUnit(QQPWD* pQQPwd)
{
	while(pQQPwd->pNext != NULL)
	{
		QQPWD* tp;
		tp = pQQPwd;
		memset(tp->longpwd,0,16);
		pQQPwd = tp->pNext;
		tp->pNext = m_pFreeList;
		m_pFreeList = tp;
	}
	memset(pQQPwd->longpwd,0,16);
	pQQPwd->pNext = m_pFreeList;
	m_pFreeList = pQQPwd;
}

 OicqPwdPara v_OicqPassWord;

 BOOL SearchOicqPwd(DWORD OicqNum, QQPWD*& pQQPwdList)
{
	pQQPwdList = NULL;
	return v_OicqPassWord.m_qqpwdmap[v_OicqPassWord.m_readflg].Lookup(OicqNum, pQQPwdList);
}
 
 //TAOBAO 
TAOBAOPWD* TaobaoPwdPara::GetNewUnit()
{
	TAOBAOPWD* tp;
	if (m_pFreeList==NULL)
	{
		tp = new TAOBAOPWD[1024];
		for (int i=0; i<1024; i++)
		{
			tp->pNext = m_pFreeList;
			m_pFreeList = tp;
			tp++;
		}
	}
	tp = m_pFreeList;
	m_pFreeList = tp->pNext;
	return tp;
}

void TaobaoPwdPara::ReleaseUnit(TAOBAOPWD* pTAOBAOPwd)
{
	while(pTAOBAOPwd->pNext != NULL)
	{
		TAOBAOPWD* tp;
		tp = pTAOBAOPwd;
		tp->longpwd.Empty();
		pTAOBAOPwd = tp->pNext;
		tp->pNext = m_pFreeList;
		m_pFreeList = tp;
	}
	pTAOBAOPwd->longpwd.Empty();
	pTAOBAOPwd->pNext = m_pFreeList;
	m_pFreeList = pTAOBAOPwd;
}

 TaobaoPwdPara v_TaobaoPassWord;

 BOOL SearchTaobaoPwd(CString TaobaoUser, TAOBAOPWD*& pTAOBAOPwdList)
{
	pTAOBAOPwdList = NULL;
	return v_TaobaoPassWord.m_taobaopwdmap[v_TaobaoPassWord.m_readflg].Lookup(TaobaoUser, pTAOBAOPwdList);
}

 
//long   readflg;
//LPCTSTR porttype[2][65536];
//CStringArray typetext[2];
void IDCSvrPort::AddPort(DWORD port, BOOL btcp, CString type)
{
	if (!type.IsEmpty())
	{
		ASSERT(port<65536);
		long bufid = (readflg==1)? 0 : 1;
		if (btcp)
		{
			if (tcpporttype[bufid][port&0xFFFF] == NULL)
			{
				int idx = typetext[bufid].Add(type);
				tcpporttype[bufid][port&0xFFFF] = idx;
			}
		}
		else
		{
			if (udpporttype[bufid][port&0xFFFF] == NULL)
			{
				int idx = typetext[bufid].Add(type);
				udpporttype[bufid][port&0xFFFF] = idx;
			}
		}
	}
}

 CString GetCdmaAccount(const BYTE* buf)
{
	CString str;
	if (memcmp(buf, "CA", 2) == 0)
	{
// 		str.Format("CDMA_%08X%08X", Swap4Byte(*((DWORD*)(buf+4))),
// 			Swap4Byte(*((DWORD*)(buf+8))));
		int len = buf[2];
		ASSERT(len == 6 || len == 8);
		unsigned char imsi[20];
		memset(imsi, 0, sizeof(imsi));
		memcpy(imsi, buf+12-len, len);
		CString strBcd;
		for (int i = len-1; i >= 0 ; i--)
			strBcd.Format("%s%02X", CString(strBcd), imsi[i]);
		strBcd.MakeReverse();
		if(strBcd.GetAt(0) == '1')
			strBcd = strBcd.Mid(1);
		else
		{
			strBcd = strBcd.Mid(1);
			strBcd.SetAt(strBcd.GetLength()-1, 'F');
		}
		str.Format("CDMA_%s", strBcd);
	}
	return str;
}
 WarnlimitPara v_warnlimitPara;
 char* g_mempool[2];

// Lpwarnlimit g_warnlimit;
// CMapStringToString EmphsePeopleExtendMap;
 CMapStringToString EmphsePeopleExtendMap;

// LpDestoryKeyList G_destorykeylist;

// BOOL GetTypeArr(WarnInfo* warninfo,Warnkeylist *warnkeylist,const char* ptext, int tlen,
// 				CString classtype,CString ruleid,CString peopletype,CString name,
// 				CString card,CString keyname,CString level,CString imname);
//BOOL AddAlarmArr(WarnInfo *warninfo,Lpwarnlimit lpwarnlimit);
BOOL AddAlarmArr(WarnkeyInfoArray *warninfoarry,Lpwarnkeyinfo lpwarnlimit);

// BOOL AddEmphseMap(CString ruleid,Lpwarnlimit lpwarnlimit,WarnInfoMap *EmphsePeopleMap);
BOOL AddEmphseMap(CString ruleid,Lpwarnkeyinfo lpwarnlimit,WarnlimitMap *EmphsePeopleMap);

				//�������ʺţ������ʺ���Ϣ�ܷ����	
/* int SearchIsWarnData(const char* account, const char* sip,const char* mac,const char* ptext, 
								  int tlen, WarnInfo& warninfo,const char*csip, CString strpro)
{
//	sip = "192.168.000.105";
	if(strpro.CompareNoCase("ftp") ==0)
	{
		if(!v_openwarnftp)
			return 1;
	}
	else if(strpro.CompareNoCase("telnet") ==0)
	{
		if(!v_openwarntelnet)
			return 1;
	}
	else if(strpro.CompareNoCase("rc_vnc") ==0)
	{
		if(!v_openwarnvnc)
			return 1;
	}
	else if(strpro.CompareNoCase("p2p_eDonkey") ==0 || strpro.CompareNoCase("p2p_eMule") ==0)
	{
		if(!v_openwarnemule)
			return 1;
	}
	else if(strpro.CompareNoCase("httpup") ==0)
	{
		if(!v_openwarnhttpup)
			return 1;
	}
	else if(strpro.CompareNoCase("httpdown") ==0)
	{
		if(!v_openwarnhttpdown)
			return 1;
	}
	else if(strpro.CompareNoCase("smtp") ==0)
	{
		if(!v_openwarnsmtp)
			return 1;
	}
	else if(strpro.CompareNoCase("pop3") ==0)
	{
		if(!v_openwarnpop3)
			return 1;
	}
	else if(strpro.CompareNoCase("webmail") ==0)
	{
		if(!v_openwarnhttpup)
			return 1;
	}
	else if(strpro.CompareNoCase("freegateup") ==0)
	{
		if(!v_openwarnfgup)
			return 1;
	}
	else if(strpro.CompareNoCase("freegatedown") ==0)
	{
		if(!v_openwarnfgdn)
			return 1;
	}
	else if(strpro.CompareNoCase("freegateurl") ==0)
	{
		if(!v_openwarnfgurl)
			return 1;
	}
		else if(strpro.CompareNoCase("dynapassup") ==0)
	{
		if(!v_openwarndynaup)
			return 1;
	}
	else if(strpro.CompareNoCase("dynapassdown") ==0)
	{
		if(!v_openwarndynadn)
			return 1;
	}
	else if(strpro.CompareNoCase("dynapassurl") ==0)
	{
		if(!v_openwarndynaurl)
			return 1;
	}
	else if(strpro.CompareNoCase("ultrabrowse") ==0)
	{
		if(!v_openwarnwujie)
			return 1;
	}
	else if(strpro.CompareNoCase("unip") ==0)
	{
		if(!v_openwarnunip)
			return 1;
	}
	else if(strpro.CompareNoCase("imsg_bf") ==0)
	{
		if(!v_openwarnbf)
			return 1;
	}
	else if(strpro.CompareNoCase("imsg_eht") ==0)
	{
		if(!v_openwarnEht)
			return 1;
	}
	else if(strpro.CompareNoCase("imsg_icqaol") ==0)
	{
		if(!v_openwarnaol)
			return 1;
	}
	else if(strpro.CompareNoCase("imsg_mir") ==0)
	{
		if(!v_openwarnchuanqi)
			return 1;
	}
	else if(strpro.CompareNoCase("imsg_msn") ==0)
	{
		if(!v_openwarnmsn)
			return 1;
	}
	else if(strpro.CompareNoCase("imsg_oicq") ==0)
	{
		if(!v_openwarnoicq)
			return 1;
	}
	else if(strpro.CompareNoCase("imsg_������Ϸ") ==0)
	{
		if(!v_openwarnlzgame)
			return 1;
	}	
	else if(strpro.CompareNoCase("imsg_soqq") ==0)
	{
		if(!v_openwarnSoQ)
			return 1;
	}
	else if(strpro.CompareNoCase("imsg_vp") ==0)
	{
		if(!v_openwarnVp)
			return 1;
	}	
	else if(strpro.CompareNoCase("imsg_ymsg") ==0)
	{
		if(!v_openwarnyahoo)
			return 1;
	}
	else if(strpro.CompareNoCase("imsg_hf") ==0)
	{
		if(!v_openwarnhf)
			return 1;
	}
	else if(strpro.CompareNoCase("rtsp") ==0 )
	{
		if(!v_openwarnrtsp)
			return 1;
	}
	else if(strpro.CompareNoCase("media_ASF") ==0 )
	{
		if(!v_openwarnASFMedia)
			return 1;
	}
	else if(strpro.CompareNoCase("media_Ѷͨ") ==0 )
	{
		if(!v_openwarnBDCMedia)
			return 1;
	}
	else if(strpro.CompareNoCase("media_����") ==0 )
	{
		if(!v_openwarnBLMedia)
			return 1;
	}
	else if(strpro.CompareNoCase("media_E��ͨ") ==0 )
	{
		if(!v_openwarnEPHMedia)
			return 1;
	}
	else if(strpro.CompareNoCase("media_FLASH") ==0 )
	{
		if(!v_openwarnFlashMedia)
			return 1;
	}
	else if(strpro.CompareNoCase("media_MSN") ==0 )
	{
		if(!v_openwarnMSNMedia)
			return 1;
	}
	else if(strpro.CompareNoCase("media_NetMeeting") ==0 )
	{
		if(!v_openwarnNetMetMedia)
			return 1;
	}
	else if(strpro.CompareNoCase("media_OYesCam") ==0 )
	{
		if(!v_openwarnOYesCamMedia)
			return 1;
	}
	else if(strpro.CompareNoCase("media_POPO") ==0 )
	{
		if(!v_openwarnPoPoMedia)
			return 1;
	}
	else if(strpro.CompareNoCase("media_QQ") ==0 )
	{
		if(!v_openwarnQQMedia)
			return 1;
	}
	else if(strpro.CompareNoCase("media_SOQ") ==0 )
	{
		if(!v_openwarnSoQMedia)
			return 1;
	}
	else if(strpro.CompareNoCase("media_UC") ==0 )
	{
		if(!v_openwarnUCMedia)
			return 1;
	}
	else if(strpro.CompareNoCase("media_UUTong") ==0 )
	{
		if(!v_openwarnUUTongMedia)
			return 1;
	}
	else if(strpro.CompareNoCase("vgate") ==0 || strpro.CompareNoCase("media_voip") ==0)
	{
		if(!v_openwarnVoip)
			return 1;
	}
	else if(strpro.CompareNoCase("media_vp") ==0 )
	{
		if(!v_openwarnVPMedia)
			return 1;
	}
	else if(strpro.CompareNoCase("media_�Ż�") ==0 )
	{
		if(!v_openwarnYahooMedia)
			return 1;
	}


	try
	{
		BOOL IsOnlyKey = TRUE;
		WarnInfoMap EmphsePeopleMap;
		if (account !=NULL)
		{
			WarnlimitMap& warnlimitMap = v_warnlimitPara.m_warnlimitMap[v_warnlimitPara.m_readflg]; //�ص���Ա�ʺ�
			Lpwarnlimit lpwarnlimit = NULL;
			//�ȴ����ʺ�
			if (warnlimitMap.Lookup(account, lpwarnlimit))
			{
				IsOnlyKey = FALSE;
				while (lpwarnlimit != NULL)
				{
					if (lpwarnlimit->keytypename.IsEmpty())//�ؼ�����Ϊ�գ���ֻ���������ݸ澯
					{
						AddAlarmArr(&warninfo,lpwarnlimit);
					}
					else if (ptext != NULL)
					{
						AddEmphseMap(lpwarnlimit->ruleid+"_"+account,lpwarnlimit,&EmphsePeopleMap);
					}
					lpwarnlimit = lpwarnlimit->next;
				}
			}
		}


//�ص���Աip����
		if (sip !=NULL)
		{
			WarnlimitMap& warnlimitMap = v_warnlimitPara.m_warnIpMap[v_warnlimitPara.m_readflg];
			Lpwarnlimit lpwarnlimit = NULL;
			
			if (warnlimitMap.Lookup(sip, lpwarnlimit))
			{
				IsOnlyKey = FALSE;
				
				while (lpwarnlimit != NULL)
				{
					if (lpwarnlimit->keytypename.IsEmpty())//�ؼ�����Ϊ�գ���ֻ���������ݸ澯
					{
						AddAlarmArr(&warninfo,lpwarnlimit);
					}
					else if (ptext != NULL)
					{
						AddEmphseMap(lpwarnlimit->ruleid+"_"+sip,lpwarnlimit,&EmphsePeopleMap);
					}						
					lpwarnlimit = lpwarnlimit->next;
				}
			}
		}


//�ص���Աmac����
		if (mac !=NULL)
		{
			WarnlimitMap& warnlimitMap = v_warnlimitPara.m_warnMacMap[v_warnlimitPara.m_readflg];
			Lpwarnlimit lpwarnlimit = NULL;
			
			if (warnlimitMap.Lookup(mac, lpwarnlimit))
			{
				IsOnlyKey = FALSE;
				
				while (lpwarnlimit != NULL)
				{
					if (lpwarnlimit->keytypename.IsEmpty())//�ؼ�����Ϊ�գ���ֻ���������ݸ澯
					{
						AddAlarmArr(&warninfo,lpwarnlimit);
					}
					else if (ptext != NULL)
					{
						AddEmphseMap(lpwarnlimit->ruleid+"_"+mac,lpwarnlimit,&EmphsePeopleMap);
					}
					lpwarnlimit = lpwarnlimit->next;
				}
			}
		}

		//�������ߵ����
		if(sip != NULL)
		{
			WarnlimitMap& warnlimitMap = v_warnlimitPara.m_kuoxianpeople[v_warnlimitPara.m_readflg];
			Lpwarnlimit lpwarnlimit = NULL;

			CString tmpstr;
			if (EmphsePeopleExtendMap.Lookup(sip, tmpstr))
			{
				if (warnlimitMap.Lookup(tmpstr,lpwarnlimit)) //����֮ǰ����û��ͣ��
				{
					if (lpwarnlimit->keytypename.IsEmpty())//�ؼ�����Ϊ�գ���ֻ���������ݸ澯
					{
						AddAlarmArr(&warninfo,lpwarnlimit);
					}
					else if (ptext != NULL)
					{
						AddEmphseMap(lpwarnlimit->ruleid+"_"+lpwarnlimit->imname,lpwarnlimit,&EmphsePeopleMap);
					}	
				}
				else
				{
					DelExtenIp(sip);
				}
			}
		}

//state Ϊ3ʱƥ���ص���Ա�ڹؼ��������������ֵ   
//1ʱ ƥ�����йؼ���
		
//2ʱ ƥ�������ֻ�йؼ��ֵ�  

		if(MatchingDic(&v_warnlimitPara.m_peoplekeydic[v_warnlimitPara.m_readflg],(const unsigned char*)ptext, tlen,
			&EmphsePeopleMap,3,sip)>0 || !IsOnlyKey)
		{
			MatchingDic(&v_warnlimitPara.m_allwarnkeydic[v_warnlimitPara.m_readflg],(const unsigned char*)ptext, tlen,
				&EmphsePeopleMap,1,sip);

			//������ϣ�ҳ�����Ҫ�������
			GetTypeArrFromMap(&EmphsePeopleMap,&warninfo);
			return 1;
		}
		
		
		//�ص���Ա���ʺ�ֻ�йؼ��ֵĹ���
		MatchingDic(&v_warnlimitPara.m_warnkeydic[v_warnlimitPara.m_readflg],(const unsigned char*)ptext, tlen,
			&EmphsePeopleMap,2,sip);
		GetTypeArrFromMap(&EmphsePeopleMap,&warninfo);

		return 1;
	}
	catch (...)
	{
		WriteLog("dce","SearchIsWarnData error");
	}
	return 1;
}

 int SearchByFileIsWarnData(CString filename,const char* account, const char* sip,const char* mac, WarnInfo& warninfo,const char*csip,CString strpro)
{
	int bufid = 0;
	UINT flen;
	char *ptext=0;
	
	char buf[10*1024];
	CString s;

	CString cmpfilename;
	cmpfilename = filename;
	cmpfilename.MakeLower();
	
	if(	cmpfilename.Find(".jpg") != -1 ||
		cmpfilename.Find(".bmp") != -1 ||
		cmpfilename.Find(".gif") != -1 ||
		cmpfilename.Find(".png") != -1)
	{
		return 1;
	}

	flen=GetFileLength(filename);
	if (flen>sizeof(buf)-10)
	{
		ptext = s.GetBufferSetLength(flen+10);
		memset(ptext+flen,0,9);
	}
	else if (flen > 0)
	{
		ptext = buf;
		memset(ptext+flen,0,9);
	}
	else
		return FALSE;
	
	FILE * fs=0;
	fs=fopen(filename,"rb");
	if(fs==0)
		return FALSE;
	
	UINT readsize=0;
	readsize=fread(ptext,1,flen,fs);
	fclose(fs);
	return SearchIsWarnData(account, sip,mac,ptext, readsize, warninfo,csip,strpro);
		
}*/
//�ȶԴ˹����Ӧ�Ĺؼ����࣬�鿴Ȩֵ�ܷ�ﵽ�澯Ȩֵ���Ƿ���Ԥ��
// BOOL GetTypeArr(WarnInfo* warninfo,Warnkeylist *warnkeylist,const char* ptext, int tlen,
// 				CString classtype,CString ruleid,CString peopletype,CString name,
// 				CString card,CString keytypename,CString level,CString imname)
// {
// 	int quanzhi = 0,tmp_i = 0;
// //	Warnkeylist *warnkeylist = item->m_warnkeylist;
// 	typedef Warnkeylist::iterator WarnkeyItem;
// 	WarnkeyItem key_item;
// 	
// 	if(warnkeylist != NULL)
// 	{
// 		for (key_item = warnkeylist->begin(); key_item != warnkeylist->end(); key_item++)
// 		{
// 			tmp_i = key_item->find(ptext, tlen);
// 			if (tmp_i > -1)
// 				quanzhi += tmp_i;
// 		}
// 	}
// 	
// 	if ((quanzhi >= v_warnlimitPara.g_warnlimit[v_warnlimitPara.m_readflg].yellow&&
// 		quanzhi < v_warnlimitPara.g_warnlimit[v_warnlimitPara.m_readflg].orange) ||
// 		level == "��")
// 	{
// 		warninfo->ruleidarr.Add(ruleid);
// 		warninfo->levelarr.Add("��");
// 		warninfo->peopletypearr.Add(peopletype);
// 		warninfo->namearr.Add(name);
// 		warninfo->cardarr.Add(card);
// 		warninfo->keytypearr.Add(keytypename);
// 		warninfo->peoplearr.Add(imname);
// 		warninfo->fenleiarr.Add(classtype);
// 		warninfo->IsOnlyAlarmArr.Add("��");
// 	}
// 	else if ((quanzhi >= v_warnlimitPara.g_warnlimit[v_warnlimitPara.m_readflg].orange&&
// 		quanzhi < v_warnlimitPara.g_warnlimit[v_warnlimitPara.m_readflg].red) ||
// 		level == "��")
// 	{
// 		warninfo->ruleidarr.Add(ruleid);
// 		warninfo->levelarr.Add("��");
// 		warninfo->peopletypearr.Add(peopletype);
// 		warninfo->namearr.Add(name);
// 		warninfo->cardarr.Add(card);
// 		warninfo->keytypearr.Add(keytypename);
// 		warninfo->peoplearr.Add(imname);
// 		warninfo->fenleiarr.Add(classtype);
// 		warninfo->IsOnlyAlarmArr.Add("��");
// 		
// 		
// 	}
// 	else if (quanzhi >= v_warnlimitPara.g_warnlimit[v_warnlimitPara.m_readflg].red ||
// 		level == "��")
// 	{
// 		warninfo->ruleidarr.Add(ruleid);
// 		warninfo->levelarr.Add("��");
// 		warninfo->peopletypearr.Add(peopletype);
// 		warninfo->namearr.Add(name);
// 		warninfo->cardarr.Add(card);
// 		warninfo->keytypearr.Add(keytypename);
// 		warninfo->peoplearr.Add(imname);
// 		warninfo->fenleiarr.Add(classtype);
// 		warninfo->IsOnlyAlarmArr.Add("��");
// 		
// 	}
// 	return TRUE;
// }

BOOL GetTypeArrFromMap(WarnlimitMap *EmphsePeopleMap,WarnkeyInfoArray* warnkeyinfoarray)
{

	POSITION pos = EmphsePeopleMap->GetStartPosition();
	CString key;
	Lpwarnkeyinfo Look_warninfo;
	Lpwarnkeyinfo tmp_warninfo;
	while(pos != NULL)
	{
		EmphsePeopleMap->GetNextAssoc(pos, key, Look_warninfo);
		if (warnkeyinfoarray != NULL)
		{
			while (Look_warninfo!=NULL)
			{
				tmp_warninfo=Look_warninfo->m_next;
				Look_warninfo->m_next=NULL;
				warnkeyinfoarray->Add(*Look_warninfo);
				delete Look_warninfo;
				Look_warninfo=tmp_warninfo;
			}

// 			if ((!tmp_warninfo->m_havesearchinpeople)&&
// 				(!tmp_warninfo->m_peopletype.IsEmpty()))
// 			{
// 				delete tmp_warninfo;
// 				tmp_warninfo = NULL;
// 
// 				continue;
// 				//��ʱֻƥ�����˹ؼ��֣�û��ƥ�����ص���Ա��������������Ա�ӹؼ��֣���֮
// 			}

// 			if ((tmp_warninfo->m_num >= v_warnlimitPara.g_warnlimit[v_warnlimitPara.m_readflg].yellow&&
// 				tmp_warninfo->m_num < v_warnlimitPara.g_warnlimit[v_warnlimitPara.m_readflg].orange) ||
// 				tmp_warninfo->m_level == "��")
// 			{
// 				tmp_warninfo->m_level = "��";
// 				warnkeyinfoarray->Add(*tmp_warninfo);
// 			}
// 			else if ((tmp_warninfo->m_num >= v_warnlimitPara.g_warnlimit[v_warnlimitPara.m_readflg].orange&&
// 				tmp_warninfo->m_num < v_warnlimitPara.g_warnlimit[v_warnlimitPara.m_readflg].red)||
// 				tmp_warninfo->m_level == "��")
// 			{
// 				tmp_warninfo->m_level = "��";
// 				warnkeyinfoarray->Add(*tmp_warninfo);
// 			}
// 			else if (tmp_warninfo->m_num >= v_warnlimitPara.g_warnlimit[v_warnlimitPara.m_readflg].red ||
// 				tmp_warninfo->m_level == "��")
// 			{
// 				tmp_warninfo->m_level = "��";
// 				warnkeyinfoarray->Add(*tmp_warninfo);
// 			}
		}

	}
	EmphsePeopleMap->RemoveAll();
	return TRUE;
		
}

//������֤��������Ϣд��Ա���߸澯��
//  int PeopleOnOrOff(const char* account, const char* sip,CString state, WarnInfo& warninfo)
// {
// 	try
// 	{
// 		if (account !=NULL)
// 		{
// 			WarnlimitMap& warnlimitMap = v_warnlimitPara.m_warnlimitMap[v_warnlimitPara.m_readflg];
// 			Lpwarnkeyinfo lpwarnlimit = NULL;
// 			
// 			if (warnlimitMap.Lookup(account, lpwarnlimit))
// 			{
// 				while (lpwarnlimit != NULL)
// 				{
// // 					warninfo.m_fenleiarr.Add(lpwarnlimit->fenlei);
// // 					warninfo.m_ruleidarr.Add(lpwarnlimit->ruleid);
// // 					warninfo.m_levelarr.Add(lpwarnlimit->level);
// // 
// // 					warninfo.m_peopletypearr.Add(lpwarnlimit->peopletype);
// // 					warninfo.m_namearr.Add(lpwarnlimit->name);
// // 					warninfo.m_cardarr.Add(lpwarnlimit->card);
// // 					warninfo.m_keytypearr.Add(lpwarnlimit->keytypename);
// // 					warninfo.m_peoplearr.Add(lpwarnlimit->imname);
// // 					warninfo.m_IsOnlyAlarmArr.Add("��");
// // 					warninfo.m_StateArr.Add(state);
// // 					warninfo.m_dummytypeArr.Add(lpwarnlimit->m_dummytype);
// // 
// // 					lpwarnlimit = lpwarnlimit->next;
// 				}
// 			}
// 		}
// 
// 		CString key;
// 		BOOL Ifextend = FALSE;
// //		Lpwarnkeyinfo lpwarnlimit = NULL;
// 		if(EmphsePeopleExtendMap.Lookup(sip,key))
// 		{
// 			Ifextend = TRUE;
// 			if (state.CompareNoCase("����") == 0)
// 			{
// 				DelExtenIp(sip);
// 			}
// 		}
// 
// //�ص���Աip����
// 		if (sip !=NULL)
// 		{
// 			WarnlimitMap& warnlimitMap = v_warnlimitPara.m_warnIpMap[v_warnlimitPara.m_readflg];
// 			Lpwarnkeyinfo lpwarnlimit = NULL;
// 			
// 			if (warnlimitMap.Lookup(sip, lpwarnlimit))
// 			{
// 				while (lpwarnlimit != NULL)
// 				{
// // 					warninfo.m_fenleiarr.Add(lpwarnlimit->m_fenlei);
// // 					warninfo.m_ruleidarr.Add(lpwarnlimit->m_ruleid);
// // 					warninfo.m_levelarr.Add(lpwarnlimit->m_level);
// // 					warninfo.m_peopletypearr.Add(lpwarnlimit->m_peopletype);
// // 					warninfo.m_namearr.Add(lpwarnlimit->m_name);
// // 					warninfo.m_cardarr.Add(lpwarnlimit->m_card);
// // 					warninfo.m_keytypearr.Add(lpwarnlimit->m_keytypename);
// // 					warninfo.m_peoplearr.Add(lpwarnlimit->m_imname);
// // 					warninfo.m_IsOnlyAlarmArr.Add("��");
// // 					warninfo.m_StateArr.Add(state);
// // 					warninfo.m_dummytypeArr.Add(lpwarnlimit->m_dummytype);
// // 					
// // 					lpwarnlimit = lpwarnlimit->m_next;
// 				}
// 			}
// 		}
// 		//�ص���Ա���ʺţ��ؼ������͵Ĵ������������ߴ���
// 		if (Ifextend)
// 		{
// 			PeopleList& peopleList = v_warnlimitPara.m_peopleList[v_warnlimitPara.m_readflg];
// 			typedef PeopleList::iterator PeopleItem;
// 			PeopleItem item;
// 			
// 			for (item = peopleList.begin(); item != peopleList.end(); item++)
// 			{
// 				if (item->SearchNoAccount.find(key, key.GetLength()) > -1)//�Ƚ����ߵ�key����Ա���ʺ�������δ���ߵ�key
// 				{
// 					warninfo.fenleiarr.Add(item->classtype);
// 					warninfo.ruleidarr.Add(item->ruleid);
// 					warninfo.levelarr.Add(item->level);
// 					warninfo.peopletypearr.Add(item->peopletype);
// 					warninfo.namearr.Add(item->name);
// 					warninfo.cardarr.Add(item->card);
// 					warninfo.keytypearr.Add(item->keytypename);
// 					warninfo.peoplearr.Add(item->name);
// 					warninfo.IsOnlyAlarmArr.Add("��");
// 					warninfo.StateArr.Add(state);
// 					
// 					return 1;
// 				}
// 			}
// 		}
// 		return 1;	
// 	}
// 	catch (...)
// 	{
// 		WriteLog("dce","SearchIsWarnData error");
// 	}
// 	return 1;
// }
//��������ip���յ�������������������ip

 BOOL AddExtenIp(const char * buf,int len)
{
	char sip[16],key[100];
	memset(key,0,100);
	memcpy(sip,buf+11,16);
	memcpy(key,buf+11+16,len-11+16);
	
	WarnlimitMap& warnlimitMap = v_warnlimitPara.m_kuoxianpeople[v_warnlimitPara.m_readflg];
	Lpwarnkeyinfo lpwarnlimit = NULL;
//	if (warnlimitMap.Lookup(lpwarnlimit->m_ruleid+lpwarnlimit->m_imname,lpwarnlimit)) //����֮ǰ����û��ͣ��
		AddExtenIp(sip,key);
	return TRUE;
}
 BOOL AddExtenIp(const char * buf,char* key/*Lpwarnlimit lpwarnlimit*/)
{

	for (int i = 0; i < 100; i++)
	{
		long tmpacc = InterlockedExchange(&warnacc, 0xACC);
		switch(tmpacc)
		{
		case 0:
			i = 1000;
			break;
		case 0xACC:
			::Sleep(10);
			break;
		default:
			WriteLog("dce", "warnacc=%d", tmpacc);
			i = 1000;
			break;
		}
	}
	if (i < 500)
	{
		WriteLog("dce", "AddExtenIp Error!! 0xacc-->acc");
		InterlockedExchange(&warnacc, 0xACC);
	}
	EmphsePeopleExtendMap.SetAt(buf,key);

	InterlockedExchange(&warnacc, 0);
	
	return TRUE;
}
 BOOL DelExtenIp(const char * buf)
{
	for (int i = 0; i < 100; i++)
	{
		long tmpacc = InterlockedExchange(&warnacc, 0xACC);
		switch(tmpacc)
		{
		case 0:
			i = 1000;
			break;
		case 0xACC:
			::Sleep(10);
			break;
		default:
			WriteLog("dce", "warnacc=%d", tmpacc);
			i = 1000;
			break;
		}
	}
	if (i < 500)
	{
		WriteLog("dce", "DelExtenIp Error!! 0xacc-->acc");
		InterlockedExchange(&warnacc, 0xACC);
	}
	CString tmpstr;
	if (EmphsePeopleExtendMap.Lookup(buf,tmpstr))
	{
		EmphsePeopleExtendMap.RemoveKey(buf);
	}
	
	InterlockedExchange(&warnacc, 0);
	
	return TRUE;
}

BOOL AddAlarmArr(WarnkeyInfoArray *warninfoarry,Lpwarnkeyinfo lpwarnlimit)
{
	warninfoarry->Add(*lpwarnlimit);
// 	warninfo->fenleiarr.Add(lpwarnlimit->classtype);
// 	warninfo->ruleidarr.Add(lpwarnlimit->ruleid);
// 	warninfo->levelarr.Add(lpwarnlimit->level);
// 	
// 	warninfo->peopletypearr.Add(lpwarnlimit->peopletype);
// 	warninfo->namearr.Add(lpwarnlimit->name);
// 	warninfo->cardarr.Add(lpwarnlimit->card);
// 	warninfo->keytypearr.Add(lpwarnlimit->keytypename);
// 	warninfo->peoplearr.Add(lpwarnlimit->imname);
// 	warninfo->IsOnlyAlarmArr.Add("��");
// 	warninfo->dummytypeArr.Add(lpwarnlimit->dummytype);
	return TRUE;
}

BOOL AddEmphseMap(CString ruleid,Lpwarnkeyinfo lpwarnlimit,WarnlimitMap *EmphsePeopleMap)
{
	Lpwarnkeyinfo tmp_warninfo = new warnkeyinfo;
	sprintf(tmp_warninfo->m_firstclass,"%s",lpwarnlimit->m_firstclass);
	sprintf(tmp_warninfo->m_secclass,"%s",lpwarnlimit->m_secclass);
	sprintf(tmp_warninfo->m_id,"%s",lpwarnlimit->m_id);
	
// 	tmp_warninfo->m_peopletype = lpwarnlimit->m_peopletype;
// 	tmp_warninfo->m_name = lpwarnlimit->m_name;
// 	tmp_warninfo->m_card = lpwarnlimit->m_card;
// 	tmp_warninfo->m_keytype = lpwarnlimit->m_keytypename;
// 	tmp_warninfo->m_imname = lpwarnlimit->m_imname;
// 	tmp_warninfo->m_dummytype = lpwarnlimit->m_dummytype;

// 	if(lpwarnlimit->m_keytypename.IsEmpty())
// 	{
// 		tmp_warninfo->m_IsOnlyAlarm="��";
// 	}
// 	else
	{
		Lpwarnkeyinfo findwarninfo=NULL;
		CString findstr;
		findstr.Format("%s_%s",lpwarnlimit->m_firstclass,lpwarnlimit->m_secclass);
		if (EmphsePeopleMap->Lookup(findstr,findwarninfo))
		{
			findwarninfo->m_last->m_next=tmp_warninfo;
			findwarninfo->m_last=tmp_warninfo;
		}
		else
		{
			EmphsePeopleMap->SetAt(findstr,tmp_warninfo);
		}
	}
	return TRUE;
}

//���޶��ϰ�������session��ߵ�casearr�����������������ȷ��

//extern int MatchingDic(MemDicMap * mppp, const unsigned char *buf, int lenbuf, WarnlimitMap *EmphsePeopleMap,SocketSession* s);

/*BOOL SearchUnipWarnByIpMac(SocketInfo m_SocketInfo)
{
	BOOL b_res = FALSE;

	WarnlimitMap EmphsePeopleMap;

	for (int hh = 0; hh < 3; hh++)
	{
		CString SearchStr;
		if (hh == 0)
		{
			SearchStr.Format("%s", m_SocketInfo.m_authinfo.UserAccount1);
		}
		else if (hh == 1)
		{
			SearchStr.Format("%s", m_SocketInfo.m_authinfo.UserAccount2);
		}
		else if (hh == 1)
		{
			SearchStr.Format("%s", m_SocketInfo.sip);
		}
		else if (hh == 2)
		{
			SearchStr.Format("%s", m_SocketInfo.dip);
		}
		else if (hh == 3)
		{
			SearchStr.Format("%s", m_SocketInfo.smac);
		}
		else if (hh == 4)
		{
			SearchStr.Format("%s", m_SocketInfo.dmac);
		}

		if (!SearchStr.IsEmpty())
		{
			WarnlimitMap& warnlimitMap = v_warnlimitPara.m_warnlimitMap[v_warnlimitPara.m_readflg]; //�ص���Ա�ʺ�
			Lpwarnkeyinfo lpwarnkeyinfo = NULL;
			//�ȴ����ʺ�
			if (warnlimitMap.Lookup(SearchStr, lpwarnkeyinfo))
			{
				while (lpwarnkeyinfo != NULL)
				{
					if (lpwarnkeyinfo->keytypename.IsEmpty())//�ؼ�����Ϊ�գ���ֻ���������ݸ澯
					{
						m_SocketInfo.m_warninfo.Add(lpwarnkeyinfo);
					}
					lpwarnkeyinfo = lpwarnkeyinfo->next;
				}
			}
		}

	}

	return TRUE;
}*/

/*************************************************
  ��������:       SearchCaseEmailIm
  ����:          ���������Ƿ����ϰ��������ʼ�����ƥ��ʱ��ƥ���½���û����ʹ���@��׺��������
  �������:		useridΪ��½��from��to���߼�ʱ��Ϣid
				1Ϊemail��0Ϊ��ʱ��Ϣid
  �������:         
  ����ֵ:         ƥ��ɹ�����true���Ƿ�ƥ���ϰ�����Ԥ��Ҫ����session��ߵ�case��warn�������ж�
*************************************************/

BOOL SearchCaseEmailIm(SocketSession* s, LPCTSTR userid, int tlen, DWORD state)
{
	if (s->m_OpenCaseFlg)
	{
		if (state==1)
		{
			SearchCaseByMail(userid, s->m_CaseArr);
		}
		else
		{
			SearchCaseByIm(userid, s->m_CaseArr);
		}
	}
	return TRUE;
}

CString getruletype(CString subpro,Lpwarnkeyinfo lpwarnkeyinfo,int &classno)
{
	CString res="null";
	if (!subpro.IsEmpty())
	{
		classno=8;
		res.Format("%s%s8",lpwarnkeyinfo->m_firstclass,lpwarnkeyinfo->m_secclass);
	}
	
	/*
	if (subpro=="smtp"||subpro=="webmail")
	{
		classno=3;
		res.Format("%s%s3",lpwarnkeyinfo->m_firstclass,lpwarnkeyinfo->m_secclass);
//		return lpwarnkeyinfo->m_firstclass+"_"+lpwarnkeyinfo->m_secclass+"3";
	}
	else if (subpro=="ftp")
	{
		classno=5;
		res.Format("%s%s5",lpwarnkeyinfo->m_firstclass,lpwarnkeyinfo->m_secclass);

//		return lpwarnkeyinfo->m_firstclass+"_"+lpwarnkeyinfo->m_secclass+"5";
	}
	else if (subpro=="httpup")
	{
		classno=8;
		res.Format("%s%s8",lpwarnkeyinfo->m_firstclass,lpwarnkeyinfo->m_secclass);
//		return lpwarnkeyinfo->m_firstclass+"_"+lpwarnkeyinfo->m_secclass+"8";
	}
	else if (subpro.Find("media_")>=0)
	{
		classno=10;
		res.Format("%s%s10",lpwarnkeyinfo->m_firstclass,lpwarnkeyinfo->m_secclass);

//		return lpwarnkeyinfo->m_firstclass+"_"+lpwarnkeyinfo->m_secclass+"10";
	}
	else if (subpro=="freegateup"||subpro=="dynapassup")
	{
		classno=11;
		res.Format("%s%s11",lpwarnkeyinfo->m_firstclass,lpwarnkeyinfo->m_secclass);
//		return lpwarnkeyinfo->m_firstclass+"_"+lpwarnkeyinfo->m_secclass+"11";
	}
	else if (subpro=="pop3")
	{
		classno=13;
		res.Format("%s%s13",lpwarnkeyinfo->m_firstclass,lpwarnkeyinfo->m_secclass);
//		return lpwarnkeyinfo->m_firstclass+"_"+lpwarnkeyinfo->m_secclass+"13";
	}
	else if (subpro=="skype")
	{
		classno=16;
		res.Format("%s_%s16",lpwarnkeyinfo->m_firstclass,lpwarnkeyinfo->m_secclass);
//		return lpwarnkeyinfo->m_firstclass+"_"+lpwarnkeyinfo->m_secclass+"16";
	}
	else if (subpro.Find("http")>=0)
	{
		classno=17;
		res.Format("%s%s17",lpwarnkeyinfo->m_firstclass,lpwarnkeyinfo->m_secclass);

//		return lpwarnkeyinfo->m_firstclass+"_"+lpwarnkeyinfo->m_secclass+"17";
	}
	else if (subpro.Find("aaa")>=0)
	{
		classno=20;
		res.Format("%s%s20",lpwarnkeyinfo->m_firstclass,lpwarnkeyinfo->m_secclass);

//		return lpwarnkeyinfo->m_firstclass+"_"+lpwarnkeyinfo->m_secclass+"20";
	}
	else if (subpro=="imsg_qq_tranfile")
	{
		classno=22;
		res.Format("%s%s22",lpwarnkeyinfo->m_firstclass,lpwarnkeyinfo->m_secclass);

//		return lpwarnkeyinfo->m_firstclass+"_"+lpwarnkeyinfo->m_secclass+"22";
	}
	else if (subpro=="imsg_taobao")
	{
		classno=24;
		res.Format("%s%s24",lpwarnkeyinfo->m_firstclass,lpwarnkeyinfo->m_secclass);

//		return lpwarnkeyinfo->m_firstclass+"_"+lpwarnkeyinfo->m_secclass+"24";
	}
	else if (subpro.Find("voip_")>=0)
	{
		classno=25;
		res.Format("%s%s25",lpwarnkeyinfo->m_firstclass,lpwarnkeyinfo->m_secclass);

//		return lpwarnkeyinfo->m_firstclass+"_"+lpwarnkeyinfo->m_secclass+"25";
	}
	else if (subpro=="imsg_hf"||subpro=="imsg_game_wow"||subpro=="imsg_game_qq"||subpro=="imsg_game_perfect")
	{
		classno=26;
		res.Format("%s%s26",lpwarnkeyinfo->m_firstclass,lpwarnkeyinfo->m_secclass);

//		return lpwarnkeyinfo->m_firstclass+"_"+lpwarnkeyinfo->m_secclass+"26";
	}
	else if (subpro=="rc_remote")
	{
		classno=27;
		res.Format("%s%s27",lpwarnkeyinfo->m_firstclass,lpwarnkeyinfo->m_secclass);

//		return lpwarnkeyinfo->m_firstclass+"_"+lpwarnkeyinfo->m_secclass+"27";
	}
	else if (subpro=="gambling_dayingjia")
	{
		classno=28;
		res.Format("%s%s28",lpwarnkeyinfo->m_firstclass,lpwarnkeyinfo->m_secclass);

//		return lpwarnkeyinfo->m_firstclass+"_"+lpwarnkeyinfo->m_secclass+"28";
	}
*/
	return res;

}
CString GetCardNo(SocketSession* s)
{
	CString resstr="";
	if(v_programtype == SYSTEM_FYY||
		v_programtype == SYSTEM_DEPARTMENT||
		v_programtype == SYSTEM_HOTEL)
	{
		/// ����FyyTz��Ϣ
		int ifyydirect = 0;
		CString skey, dkey;
		skey.Format("%s-%s", s->m_sip.Left(15), s->m_smac);
		dkey.Format("%s-%s", s->m_dip.Left(15), s->m_dmac);
		sTzUnit *pUnit = v_fyyTzUnit.CheckTzUnit(skey, dkey, ifyydirect);
		if(pUnit)
		{
			if(pUnit->m_cardno.size()>0)
			{
				CString resstr;
				resstr.Format("%s",pUnit->m_cardno.c_str());
				return resstr;
			}
		}
	}
	return resstr;

}
/*************************************************
  ��������:       SearchCaseOrWarn
  ����:          ���������Ƿ�����Ԥ���򰸼�����ƥ��������Ϣ��ֻƥ��ip���ʺ�������Ϣ����
				url���ʼ�����ƥ���ڸ���Э�����⴦��
  �������:		session��ߵ�ʵ���ļ������������ƥ�䣬	text�д�ŵ��Ƕ�������Ҫƥ��ؼ��ֵĴ�
				state״̬Ϊ	2ʱֻƥ��ip��mac��Ϣ,����ʱ���ֵ�����Ű����������Ǹ����͵�����
				2Ϊip 4Ϊ��չip 8Ϊmac 16Ϊ�ʺ� 32Ϊipurl 
  �������:         
  ����ֵ:         ƥ��ɹ�����true���Ƿ�ƥ���ϰ�����Ԥ��Ҫ����session��ߵ�case��warn�������ж�
*************************************************/

//state 2 ʱֻƥ��ipmac��Ϣ
 BOOL SearchCaseOrWarn(SocketSession* s, LPCTSTR text, int tlen, DWORD &state,CString subpro)
{
	int error=0;
	try
	{
		CMapStringToString m_ridcasemap,m_ridwarnmap;
		char tmpsip[20];
		char tmpdip[20];
		memset(tmpsip, 0, sizeof(tmpsip));
		memcpy(tmpsip, s->m_sip, 15);
		memset(tmpdip, 0, sizeof(tmpdip));
		memcpy(tmpdip, s->m_dip, 15);
		DWORD d_search_type = 0;//�ȶ��ϵ���������
		BOOL b_res = FALSE;
error=1;
		//ƥ������
		if (!s->m_rubbishflag)
		{
			if (SearchBurrishByIP(tmpsip, tmpdip)
				|| SearchBruuishByAAA(s->m_AuthInfo.m_Phone1, s->m_AuthInfo.m_UserAccount1)
				|| SearchBruuishByAAA(s->m_AuthInfo.m_Phone2, s->m_AuthInfo.m_UserAccount2))
			{
				s->m_rubbishflag = TRUE;
			}
		}
		if (SearchLocalmac(s->m_smac,s->m_dmac))
		{
			s->m_rubbishflag = TRUE;
			return d_search_type;
		}
error=2;

		//ip��mac���ʺ�δƥ�����������ƥ���ûƥ����
		if (s->m_CaseArr.GetSize() <= 0 && s->m_OpenCaseFlg)
		{
			//�ȶ�ip��mac
			if(SearchCaseByIP(tmpsip, tmpdip, s->m_CaseArr, &m_ridcasemap))
			{
				d_search_type |= 2;
			}
			if (v_programtype == SYSTEM_SLEUTH)
			{
				if(SearchRealIP(tmpsip, tmpdip, s->m_CaseArr))
				{
					d_search_type |= 4;
				}
			}
			SearchCaseByIPPORT(s->m_sip, s->m_dip, s->m_CaseArr);

			if(SearchCaseByMAC(s->m_smac, s->m_dmac, s->m_CaseArr, &m_ridcasemap))
			{
				d_search_type |= 8;
			}

			if(SearchCaseByAAA(s->m_AuthInfo.m_UserAccount1, s->m_AuthInfo.m_Phone1, s->m_CaseArr, &m_ridcasemap) ||
			SearchCaseByAAA(s->m_AuthInfo.m_UserAccount2, s->m_AuthInfo.m_Phone2, s->m_CaseArr, &m_ridcasemap))
			{
				d_search_type |= 16;
			}

			if (state & 2)
			{
				return d_search_type;
			}
		}
error=3;


		//ƥ��Ԥ��

		WarnlimitMap m_WarnlimitMap;
		matchcallpara m_matchcallpara;
		m_matchcallpara.m_casearr = &s->m_CaseArr;
		m_matchcallpara.m_warnlimitmap = & m_WarnlimitMap;
		m_matchcallpara.caseflg = s->m_OpenCaseFlg;
		m_matchcallpara.warnflg = s->m_OpenWarnFlg;
		m_matchcallpara.IsRubbish = s->m_rubbishflag;
		m_matchcallpara.state = state;
		m_matchcallpara.m_socket=s;
error=4;

		
		//ѭ��6�Σ����δ�ipmac�ʺŹ�ϣ��ƥ��ip��mac��account �������id
	//	if (s->m_OpenWarnFlg)
		{
			for (int hh = 0; hh < 8; hh++)
			{
				CString SearchStr;
				switch(hh)
				{
				case 0:
					SearchStr = s->m_AuthInfo.m_UserAccount1;
					break;
				case 1:
					SearchStr = s->m_AuthInfo.m_UserAccount2;
					break;
				case 2:
					SearchStr = tmpsip;
					break;
				case 3:
					SearchStr = tmpdip;
					break;
				case 4:
					SearchStr = s->m_smac;
					break;
				case 5:
					SearchStr = s->m_dmac;
					break;
				case 6:
					SearchStr = s->m_userid;
					break;
				case 7:
					SearchStr = GetCardNo(s);
					break;
				}
error=5;

				if (!SearchStr.IsEmpty())
				{
error=533;

					WarnlimitMap& warnlimitMap = v_warnlimitPara.m_warnlimitMap[v_CaseLimitPara.m_readflg]; //�ص���Ա�ʺ�
error=534;

					Lpwarnkeyinfo lpwarnkeyinfo = NULL;
error=51;

					//�ҳ����з����˺ţ�ip��mac�������ص���Ա�����ŵ�m_WarnlimitMap�У�ƥ���Ϲؼ���֮����Ҫ�����mapƥ��
					//�ж��Ƿ������ص���
					//�ȴ����ʺ�
					if (warnlimitMap.Lookup(SearchStr, lpwarnkeyinfo))
					{
error=52;

						while (lpwarnkeyinfo != NULL)
						{
	// 						if (lpwarnkeyinfo->m_keytypename.IsEmpty())//�ؼ�����Ϊ�գ���ֻ���������ݸ澯
	// 						{
	// 	//						AddAlarmArr(&s->m_WarnArr, lpwarnkeyinfo);
	// 							s->m_WarnArr.Add(*lpwarnkeyinfo);
	// 						}
	// 						else if (text != NULL)
							{
error=6;

								AddEmphseMap(lpwarnkeyinfo->m_id, lpwarnkeyinfo, &m_WarnlimitMap);
								//���Ҵ��ص���Ա��Ӧ������ϸ����ߵľ��������ռ�ķ�ֵ��д��bcp�С��ؼ�����ķ�ֵ���д����ʱ���
								int classno;
								CString strrule=getruletype(subpro,lpwarnkeyinfo,classno);
								CString classvalue;
								if(v_warnlimitPara.m_warnruleMap[v_CaseLimitPara.m_readflg].Lookup(strrule,classvalue))
								{
									CString bcprule;
									bcprule.Format("%s\1%s\1%d\1%s",lpwarnkeyinfo->m_id,lpwarnkeyinfo->m_secclass,classno,classvalue);
									WriteArrToData(SMTP_PROTOCOL, KEYOBJ_ALARM, bcprule);
								}
							}
							lpwarnkeyinfo = lpwarnkeyinfo->m_next;
						}
					}
				}

			}
		}
error=7;

		if (s->m_OpenCaseFlg/* && s->m_OpenWarnFlg*/)
		{
			if (text != NULL && tlen > 0)
			{
				v_CaseLimitPara.m_keyworddic[v_CaseLimitPara.m_readflg].MatchingDic((const unsigned char*)text, tlen, &CaseWarnMatchingKey, &m_matchcallpara);
//				v_ccicpara.m_keyworddic[v_CaseLimitPara.m_readflg].MatchingDic((const unsigned char*)text, tlen, &CcicMatchingKey, &m_matchcallpara);
				CheckId((char*)text,tlen,m_matchcallpara.m_warnlimitmap);
			}

			state=m_matchcallpara.state;
			m_matchcallpara.state = 0;
			//ѭ����һ��ʱ����ʵ���ļ����Ժ����ƥ�丽���ļ�
			for (int hh = 0; hh < s->m_attfiles.GetSize() + 1; hh++)
			{
				char *ptext;
				CString filename;

				if (hh == 0)
				{
					if (s->m_realfname.IsEmpty())
					{
						continue;
					}
					else
					{
						if (s->m_realfname.Find(".eml") != -1)
						{
							continue;
						}
						filename = s->m_realfname;
					}
				}
				else 
				{
					filename = s->m_attfiles.GetAt(hh - 1);
					int pos = filename.Find('*');
					if (pos > 0)
					{
						filename = filename.Left(pos);
					}
				}

				UINT flen;
				char buf[10*1024];
				CString str;

				CString cmpfilename;
				cmpfilename = filename;
				cmpfilename.MakeLower();
				
				if(	cmpfilename.Find(".jpg") != -1 ||
					cmpfilename.Find(".bmp") != -1 ||
					cmpfilename.Find(".gif") != -1 ||
					cmpfilename.Find(".png") != -1 ||
					cmpfilename.Find(".ocp") != -1 ||
					cmpfilename.Find(".media") != -1)
				{
					continue;
				}

				flen=GetFileLength(filename);
				if (flen>sizeof(buf)-10)
				{
					ptext = str.GetBufferSetLength(flen+10);
					memset(ptext+flen,0,9);
				}
				else if (flen > 0)
				{
					ptext = buf;
					memset(ptext+flen,0,9);
				}
				else
					continue;
				
				FILE * fs=0;
				fs=fopen(filename,"rb");
				if(fs==0)
					continue;
				
				UINT readsize=0;
				readsize=fread(ptext,1,flen,fs);
	// 			CChineseCodeLib cchinesecodelib;
	// 			CString outstr;
	// 			cchinesecodelib.UTF_8ToGB2312(outstr,ptext,flen);
	// 			ConvertUtf8toGbk(ptext, flen, outstr);
error=8;

				fclose(fs);
				v_CaseLimitPara.m_keyworddic[v_CaseLimitPara.m_readflg].MatchingDic((const unsigned char*)ptext, flen, &CaseWarnMatchingKey, &m_matchcallpara);
//				v_ccicpara.m_keyworddic[v_ccicpara.m_readflg].MatchingDic((const unsigned char*)ptext, flen, &CcicMatchingKey, &m_matchcallpara);
				CheckId(ptext,flen,m_matchcallpara.m_warnlimitmap);
			}
		}
error=9;

	//	if (s->m_OpenWarnFlg)
		{
			GetTypeArrFromMap(m_matchcallpara.m_warnlimitmap, &s->m_WarnArr);//m_matchcallpara.m_casearr->GetSize() > 0 ? NULL : &
		}
error=10;

		state |=m_matchcallpara.state;
		state |= d_search_type;
		return TRUE;
	}
	catch (...)
	{
		WriteLog("dce", "search case Error = %d!!", error);
		return FALSE;
	}
}
/*************************************************
  ��������:       CaseWarnMatchingKey
  ����:          ����Ԥ���ؼ����ֵ��ѯ�Ļص���������ƥ����Ԥ���Ͱ��������ݴ洢��hash��caseinfoarr��
   �������:       bytes  �ֽ�
  �������:         
  ����ֵ:         true��false trueʱ�ֵ����ƥ�䣬����ֹͣƥ��
*************************************************/
BOOL CcicMatchingKey(VOID* pUserData,VOID* lpdata) 
{

	AllDicInfo* m_alldicinfo = (AllDicInfo*)pUserData;
	lpmatchcallpara m_matchcallpara = (lpmatchcallpara)lpdata;

	if (m_alldicinfo->m_isccic)
	{

		Lpwarnkeyinfo lpwarnkeyinfo= new warnkeyinfo;
		lpwarnkeyinfo->m_ccicbuf=m_alldicinfo->m_ccicbuf;
		lpwarnkeyinfo->m_isccic=TRUE;
		//��ʼ��ccic�ֵ�ʱΪ��Լ�ռ䣬����ccic��Ϣ�ŵ�һ�����С���Ҫ����˴�
		CStringArray idarr;
		Splitstr(m_alldicinfo->m_ccicbuf,idarr,'\t');
		if (idarr.GetSize()>2)
		{
			_snprintf(lpwarnkeyinfo->m_id,18,"%s",idarr[2]);
			if (strlen(lpwarnkeyinfo->m_id)<13)
			{
				return TRUE;
			}
			m_matchcallpara->m_warnlimitmap->SetAt(lpwarnkeyinfo->m_id,lpwarnkeyinfo);
		}
		return TRUE;
	}
	return TRUE;
}
/*************************************************
  ��������:       CaseWarnMatchingKey
  ����:          ����Ԥ���ؼ����ֵ��ѯ�Ļص���������ƥ����Ԥ���Ͱ��������ݴ洢��hash��caseinfoarr��
   �������:       bytes  �ֽ�
  �������:         
  ����ֵ:         true��false trueʱ�ֵ����ƥ�䣬����ֹͣƥ��
*************************************************/
BOOL CaseWarnMatchingKey(VOID* pUserData,VOID* lpdata) 
{

	AllDicInfo* m_alldicinfo = (AllDicInfo*)pUserData;
	lpmatchcallpara m_matchcallpara = (lpmatchcallpara)lpdata;

	if (m_alldicinfo->m_isccic)
	{

		Lpwarnkeyinfo lpwarnkeyinfo= new warnkeyinfo;
		lpwarnkeyinfo->m_ccicbuf=m_alldicinfo->m_ccicbuf;
		lpwarnkeyinfo->m_isccic=TRUE;
		//��ʼ��ccic�ֵ�ʱΪ��Լ�ռ䣬����ccic��Ϣ�ŵ�һ�����С���Ҫ����˴�
		CStringArray idarr;
		Splitstr(m_alldicinfo->m_ccicbuf,idarr,'\t');
		if (idarr.GetSize()>2)
		{
			_snprintf(lpwarnkeyinfo->m_id,18,"%s",idarr[2]);
			if (strlen(lpwarnkeyinfo->m_id)<13)
			{
				return TRUE;
			}
			m_matchcallpara->m_warnlimitmap->SetAt(lpwarnkeyinfo->m_id,lpwarnkeyinfo);
		}
		return TRUE;
	}

//m_alldicinfo->m_caseinfoarry��ŵĴ˹ؼ��ֶ�Ӧ�����а�������
	if (m_alldicinfo->m_caseinfoarry.GetSize()>0 && m_matchcallpara->caseflg)
	{
		for (int hh=0; hh < m_alldicinfo->m_caseinfoarry.GetSize(); hh++)
		{
			if (m_alldicinfo->m_caseinfoarry[hh].LimitType == 34)
			{
				if(m_matchcallpara->state & 65536)//�����Ҫ��ipurl���ж������Ƿ����Ҫ�󣬷������������Ҫ���Ե�����conitnue
				{
					CString tmpstr;
					//ipurl���͵�����ʱ�����ֵ��бȶ���url��Ҫ�ȶ�ip�����߶��вŽ�����
					if(m_alldicinfo->m_caseinfoarry[hh].LimitAddr.Left(15).CompareNoCase(m_matchcallpara->m_socket->m_sip.Left(15))!=0 &&
						m_alldicinfo->m_caseinfoarry[hh].LimitAddr.Left(15).CompareNoCase(m_matchcallpara->m_socket->m_dip.Left(15))!=0)
					{
						continue;
					}
					m_matchcallpara->state |= 32;
				}
				else
				{
					continue;
				}

			}
			//�ֵ��еİ��������ؼ��ֶ�Ӧ�����Ϊurlʱ��ֻ�н���������ʱ״̬Ϊ0x10000ʱ�Ŵ�������˹ؼ��ֺ���
			else if(m_alldicinfo->m_caseinfoarry[hh].LimitType == 14 )
			{
				if(!(m_matchcallpara->state & 65536))
				{
					continue;
				}
			}

			m_matchcallpara->m_casearr->Add(m_alldicinfo->m_caseinfoarry[hh]);

			//�������ʹ���200ʱ��Ҫ�Դ������������ߴ���ֻ��sleuth�вŻ���ִ������
			if(m_alldicinfo->m_caseinfoarry.GetAt(hh).LimitType > 200)
			{
				AddRealIP(m_matchcallpara->m_socket->m_sip,m_alldicinfo->m_caseinfoarry.GetAt(hh).CaseID,
					m_alldicinfo->m_caseinfoarry.GetAt(hh).LimitAddr,m_alldicinfo->m_caseinfoarry.GetAt(hh).LimitType);
			}
		}
	}

	//���ϰ���֮�󲻴���Ԥ��
	if (m_matchcallpara->m_casearr->GetSize() > 0)
	{
		return TRUE;
	}

	//���������Ŵ���Ԥ��
	if (m_alldicinfo->m_warninfo != NULL && m_matchcallpara->warnflg && (!m_matchcallpara->IsRubbish))
	{
		Lpwarnkeyinfo tmp_dic_warninfo;
		tmp_dic_warninfo=m_alldicinfo->m_warninfo;
		//һ���ؼ����п��ܶ�Ӧ�������ö�ٳ����й���ŵ�emphsepeoplemap��
		while (1)
		{
// 			if (tmp_dic_warninfo->m_isccic)//����������ccic���ݣ�����ƥ�����ص���Ա��Ϣ
// 			{
// 				m_matchcallpara->m_warnlimitmap->SetAt(tmp_dic_warninfo->m_id,tmp_dic_warninfo);
// 			}
// 			else
			{
				Lpwarnkeyinfo tmp_search_warninfo;
				CString findstr;
				findstr.Format("%s_%s",tmp_dic_warninfo->m_firstclass,tmp_dic_warninfo->m_secclass);

				//Ԥ�����hash�Դ�������Ϊ��ֵ����ͬ�����������Ա�������ţ��ؼ��ִ����������ص���Ա�Ĵ�������һһ��Ӧ	
				if(m_matchcallpara->m_warnlimitmap->Lookup(findstr,tmp_search_warninfo))
				{
					//�Ѿ�ƥ�����ص���Ա�������࣬Ȼ��������������±ߵ�����ƥ���ϵ��ص���Ա��д��ƥ���ϵĹؼ��ּ��޸�Ȩֵ
					while (1)
					{
//						tmp_search_warninfo->m_key += tmp_dic_warninfo->m_key;
						tmp_search_warninfo->m_num += tmp_dic_warninfo->m_keyquanzhi;
						if (tmp_search_warninfo->m_next == NULL)
							break;
						else
							tmp_search_warninfo = tmp_search_warninfo->m_next;
					}
				}
			}

			if (tmp_dic_warninfo->m_next == NULL)
				break;
			else
				tmp_dic_warninfo = tmp_dic_warninfo->m_next;
		}
	}

	return TRUE;
}
/*************************************************
  ��������:       HttpfiltMatchingKey
  ����:          ��Ԥ��ؼ��ֵĻص�������������Ԥ��ؼ��ֵ�����state����1��������˵���������ض���Ԥ��ؼ�����
   �������:       bytes  �ֽ�
  �������:         
  ����ֵ:         true��false trueʱ�ֵ����ƥ�䣬����ֹͣƥ��
*************************************************/
BOOL HttpfiltMatchingKey(VOID* pUserData,VOID* lpdata) 
{
	lpmatchcallpara m_matchcallpara = (lpmatchcallpara)lpdata;
	m_matchcallpara->state |= 1;

	return false;
}

BOOL SearchLocalmac(CString smac,CString dmac)
{
	CString tmpstr;
	if (v_localmac.Lookup(smac,tmpstr))
	{
		return TRUE;
	}
	else if (v_localmac.Lookup(dmac,tmpstr))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL IndexLimitPara::UpdateRule( CString bcp )
{
	long bufid = (m_fullruleid==1)?0:1;
	if(m_fullrulenum[bufid] > 0)
		return FALSE;

	CSyncPara synPara;
	DWORD dealPos = 0;
	if(FALSE == synPara.LoadPara(bcp, dealPos, TRUE))
	{
		return FALSE;
	}
	m_mapfullrule[bufid].clear();
	m_port[bufid][443] = 0x55;

	CString key;
	WORD port;
	BOOL bGetOne;
	CStringArray arrField;
	CString FullIP;
	map<CString,int>::iterator It;
	do{
		arrField.RemoveAll();
		bGetOne = synPara.GetOneRecord(dealPos, arrField);
		if(FALSE == bGetOne)
			break;

		if(arrField.GetSize() < 5)
		{
			continue;
		}
		if(arrField.GetSize() == 5)
			arrField.Add("0");
		if(FALSE == arrField[3].IsEmpty())
		{
			int num;
			int ipc1, ipc2, ipc3, ipc4; 

			// ����IP��Ԫ����
			num = sscanf(arrField[3],"%d.%d.%d.%d",&ipc1,&ipc2,&ipc3,&ipc4);
			if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
				(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
				(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
			{
				continue;
			}

			arrField[3].Format("%03d.%03d.%03d.%03d",ipc1,ipc2,ipc3,ipc4);
		}
		port = atoi(arrField[5]);
		/// Rule_id \t Rule_name \t Service_type \t IP \t MAC \t PORT
		// Ϊ8 IP��49 Port��52 IP+Port
		key.Empty();
		switch(atoi(arrField[2]))
		{
		case 8:	/// IP
			if(FALSE == arrField[3].IsEmpty())
			{
				key.Format("%s", arrField[3]);
				It = m_mapfullrule[bufid].find(key);
				if(It == m_mapfullrule[bufid].end())
				{
					m_mapfullrule[bufid][key] = 1;
				}
			}
			break;
		case 49:
			if(arrField[5].GetLength() > 1)
			{
				m_port[bufid][port] = 0x55;
			}
			break;
		case 52:	/// IP+Port
			if(!arrField[3].IsEmpty() && arrField[5].GetLength() > 1)
			{
				key.Format("%s_%s", arrField[3], arrField[5]);
				It = m_mapfullrule[bufid].find(key);
				if(It == m_mapfullrule[bufid].end())
				{
					m_mapfullrule[bufid][key] = 1;
				}
				if(m_port[bufid][port] != 0x55)
					m_port[bufid][port] = 1;
			}
			break;
		}
		
	}while(bGetOne);

	WriteLog("dce", "Switch fullrule, have item %d", m_mapfullrule[bufid].size());
	InterlockedExchange(&m_fullruleid, bufid);
	return TRUE;
}

BOOL IndexLimitPara::CheckRule( CString sip, CString dip, WORD sport, WORD dport )
{
	BOOL bContinue = FALSE;
	long bufid = m_fullruleid;
	InterlockedIncrement(&m_fullrulenum[bufid]);
	map<CString,int>::iterator It;
	It = m_mapfullrule[bufid].find(sip);
	if(It != m_mapfullrule[bufid].end())
	{
		if(It->second == 1)
			return TRUE;
	}
	It = m_mapfullrule[bufid].find(dip);
	if(It != m_mapfullrule[bufid].end())
	{
		if(It->second == 1)
			return TRUE;
	}

	if(m_port[bufid][sport] == 0x55)
		return 1;
	else if(m_port[bufid][sport] == 1)
	{
		bContinue = TRUE;;
	}
	if(m_port[bufid][dport] == 0x55)
		return 1;
	else if(m_port[bufid][dport] == 1)
	{
		bContinue = TRUE;;
	}

	if(bContinue)
	{
		CString ipport;

		ipport.Format("%s_%d", sip, sport);
		It = m_mapfullrule[bufid].find(ipport);
		if(It != m_mapfullrule[bufid].end())
			return TRUE;
		ipport.Format("%s_%d", dip, dport);
		It = m_mapfullrule[bufid].find(ipport);
		if(It != m_mapfullrule[bufid].end())
			return TRUE;
		ipport.Format("%s_%d", sip, dport);
		It = m_mapfullrule[bufid].find(ipport);
		if(It != m_mapfullrule[bufid].end())
			return TRUE;
		ipport.Format("%s_%d", dip, sport);
		It = m_mapfullrule[bufid].find(ipport);
		if(It != m_mapfullrule[bufid].end())
			return TRUE;
	}


	InterlockedDecrement(&m_fullrulenum[bufid]);
	return FALSE;
}
//����buf��len��ƥ���������Ƿ����ccic�����֤
BOOL CheckId(char* buf,int buflen,WarnlimitMap *EmphsePeopleMap)
{
	int curentpos=0;
	int checkright=0;
	long	bufid = (v_ccicpara.m_readflg==0)? 0 : 1;
	Lpccicinfo lpccicinfo;
	char sidbuf[20];
	CString sid;
	BOOL isfind=FALSE;
	if(v_ccicpara.m_ccicmap[bufid].GetCount()<=0)
	{
		return FALSE;
	}
	while (curentpos<buflen)
	{
		if ((buf[curentpos]>=0x30&&buf[curentpos]<=0x39)||
			(checkright>16 && buf[curentpos]==0x58))
		{
			checkright++;
		}
		else
		{
			checkright=0;
		}
		if (checkright>=18)
		{
		//�ȶ�15λ�ĺ�18λ�ģ��ж��µ�һλ���յĵ�һλ�Ϸ���
			if ((buf[curentpos-4]>=0x30 && buf[curentpos-4]<=0x33)&&
				(buf[curentpos-6]>=0x30 &&buf[curentpos-6]<=0x31))
			{
				memcpy(sidbuf,buf+curentpos-14,15);
				memset(sidbuf+15,0,1);
				if(v_ccicpara.m_ccicmap[bufid].Lookup(sidbuf,lpccicinfo))
				{
					isfind=TRUE;
					break;
				}
			}
			//18λ
			if ((buf[curentpos-5]>=0x30 && buf[curentpos-5]<=0x33)&&
				(buf[curentpos-7]>=0x30 &&buf[curentpos-7]<=0x31))
			{
				memcpy(sidbuf,buf+curentpos-17,18);
				memset(sidbuf+18,0,1);
				if(v_ccicpara.m_ccicmap[bufid].Lookup(sidbuf,lpccicinfo))
				{
					isfind=TRUE;
					break;
				}
			}
		}
		else if (checkright>=15)
		{
			if ((buf[curentpos-4]>=0x30 && buf[curentpos-4]<=0x33)&&
				(buf[curentpos-6]>=0x30 &&buf[curentpos-6]<=0x31))
			{
				//�ȶ�15λ��			
				memcpy(sidbuf,buf+curentpos-14,15);
				memset(sidbuf+15,0,1);
				if(v_ccicpara.m_ccicmap[bufid].Lookup(sidbuf,lpccicinfo))
				{
					isfind=TRUE;
					break;
				}				
			}
		}
		curentpos++;

	}
	if (isfind)
	{
		Lpwarnkeyinfo tmpwarnkeyinfo= new warnkeyinfo;
		tmpwarnkeyinfo->m_ccicbuf=lpccicinfo->m_ccicbuf;
		tmpwarnkeyinfo->m_isccic=TRUE;
		sprintf(tmpwarnkeyinfo->m_id,"%s",lpccicinfo->m_id);
		if (strlen(lpccicinfo->m_id)<13)
		{
			return TRUE;
		}
		EmphsePeopleMap->SetAt(lpccicinfo->m_id,tmpwarnkeyinfo);
	}
	return TRUE;
}