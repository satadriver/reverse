#pragma once
#include "StdAfx.h"
#include "http.h"
#include "HttpSession.h"	
#include "PhoneMediaApp.h"



CString PhoneMediaGetItemValue(const CString &str,char sep)
{
	if(str.Find(sep) != -1)
	{
		return str.Mid(str.Find(sep) + 1);
	}
	else
	{
		return "";
	}
}

void PhoneMediaSplitString(CString strSrc ,CStringArray &arrayDest,char sep)//ÓÃÓÚ·Ö¸îµ¥¸ö×Ö·û
{
	CString tmpstr;
	
	int pos = strSrc.Find(sep);	
	while (pos >= 0)
	{
		tmpstr=strSrc.Left(pos);
		tmpstr.TrimRight();
		tmpstr.TrimLeft();
		arrayDest.Add(tmpstr);
		strSrc = strSrc.Mid(pos+1);
		pos = strSrc.Find(sep);
	}
	if((pos == -1) && (!strSrc.IsEmpty()))
	{
		arrayDest.Add(strSrc);	
	}
}
/*
void PhoneMediaSplitString(CString &strSrc ,CStringArray &arrayDest,char *pSep)
{
	
	while( (*pSep != '\0') || (pSep == NULL))
	{
		PhoneMediaSplitString(strSrc,arrayDest,*pSep);
		if (arrayDest.GetSize() > 1)
		{
			break;
		}
		pSep++;
	}
}*/

BOOL DealPhoneMedia( HttpSession *pHttpSession,const char *pPacket)
{
	int nRes = 0;	
	if (IsPhoneMediaNBALive(pHttpSession))
	{
		return PhoneMediaNBALive(pHttpSession,pPacket);
	}
	if (IsPhoneMediaAiQiYi(pHttpSession))
	{
		return PhoneMediaAiQiYi(pHttpSession,pPacket);
	}
	if (nRes = IsPhoneMediaBaiDuMusic(pHttpSession))
	{
		return PhoneMediaBaiDuMusic(pHttpSession,pPacket,nRes);
	}
	if (nRes = IsPhoneMediaPPTV(pHttpSession))
	{
		return PhoneMediaPPTV(pHttpSession,pPacket,nRes);
	}
	if (nRes = IsPhoneMediaTianTianPlay(pHttpSession))
	{
		return PhoneMediaTianTianPlay(pHttpSession,pPacket,nRes);
	}
	if (nRes = IsPhoneMediaHePaiSchool(pHttpSession))
	{
		return PhoneMediaHePaiSchool(pHttpSession,pPacket,nRes);
	}
	if (IsPhoneMediaYouKu(pHttpSession))
	{
		return PhoneMediaYouKu(pHttpSession,pPacket);
	}
	if (nRes = IsPhoneMediaKaiXunVideo(pHttpSession))
	{
		return PhoneMediaKaiXunVideo(pHttpSession,pPacket,nRes);
	}
	if (nRes = IsPhoneMediaKWMusic(pHttpSession))
	{
		return PhoneMediaKWMusic(pHttpSession,pPacket,nRes);
	}
	if (nRes = IsPhoneMediaQQMusic(pHttpSession))
	{
		return PhoneMediaQQMusic(pHttpSession,pPacket,nRes);
	}
	if (nRes = IsPhoneMediaKaoLaFM(pHttpSession))
	{
		return PhoneMediaKaoLaFM(pHttpSession,pPacket,nRes);
	}
	return FALSE;
}

BOOL IsPhoneMediaNBALive(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("api.estream.cn")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/nba/client_statistic") != -1 )   )	
	{
		return TRUE;
	}
	return FALSE;
}

BOOL PhoneMediaNBALive( HttpSession *pHttpSession, const char *pPacket )
{
	int nposbeg = -1;
	int nposend = -1;
	int noffset = 0;
	CString tem(pPacket);
	nposbeg = tem.Find("model");
	nposend = tem.Find("&imei");
	noffset = 6;
	CString model;
	if ( (-1 != nposbeg ) && (-1 != nposend))
	{
		model = tem.Mid(nposbeg + noffset,nposend - nposbeg - noffset);
	}
	else
	{
		return FALSE;
	}
	nposbeg = tem.Find("&imei");
	nposend = tem.Find("&signal");
	noffset = 6;
	CString imei;
	if ( (-1 != nposbeg ) && (-1 != nposend))
	{
		imei = tem.Mid(nposbeg + noffset,nposend - nposbeg - noffset);
	}
	else
	{
		return FALSE;
	}
	CString txt;
	txt.Format("imei=%s\r\nwebname=%s\r\nstate=%s\r\n",imei,model,"´ò¿ªÊ×Ò³");
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemedia_nba.indexfile.txt", 
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("phonemedia_nba","phonemedia_nba",txt,pHttpSession->m_dceprocotol->m_httpMissObj);	
	return TRUE;
}

BOOL IsPhoneMediaAiQiYi(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("iface.iqiyi.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("api/registe") != -1 )   )	
	{
		return TRUE;
	}
	return FALSE;
}

BOOL PhoneMediaAiQiYi( HttpSession *pHttpSession, const char *pPacket )
{
	
	CString strTempData(pPacket);
	strTempData = ConverHttp2Html(strTempData);
	ConvertUtf8toGbk(strTempData,strTempData.GetLength(),strTempData);
	CStringArray arrTemp;
	PhoneMediaSplitString(strTempData,arrTemp,'&');
	int arrsize = arrTemp.GetSize();
	CString strUser,strPass,strIMEI,strModel;
	for (int i = 0; i < arrsize; i++)
	{
		if ( arrTemp[i].Find("email=") != -1)
		{
			strUser = PhoneMediaGetItemValue(arrTemp[i],'=');
		}
		if  (arrTemp[i].Find("passwd=") != -1)
		{
			strPass = PhoneMediaGetItemValue(arrTemp[i],'=');
		}
		if ( arrTemp[i].Find("id=") != -1)
		{
			strIMEI = PhoneMediaGetItemValue(arrTemp[i],'=');
		}
		if  (arrTemp[i].Find("ua=") != -1)
		{
			strModel = PhoneMediaGetItemValue(arrTemp[i],'=');
		}
		
	}	
	if (strUser.IsEmpty() || strPass.IsEmpty())
	{
		return FALSE;
	}
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nimei=%s\r\nwebname=%s\r\nstate=×¢²á\r\n",strUser,strPass,strIMEI,strModel);
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemedia_aqy.indexfile.txt", 
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());	
	pHttpSession->WriteIndexFile("phonemedia_aqy","phonemedia_aqy",txt,pHttpSession->m_dceprocotol->m_httpMissObj);	
	return TRUE;
}

BOOL IsPhoneMediaBaiDuMusic(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("wappass.baidu.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/wp/api/login?") != -1 )  )
	{
		return 1;
	}
	return FALSE;
}

BOOL PhoneMediaBaiDuMusic( HttpSession *pHttpSession,const char *pPacket ,int Value)
{	
	char *pHead = strstr(pPacket,"username");
	if (!pHead)
	{
		return FALSE;
	}
	CString strTempData(pHead);
	int posend = strTempData.Find("&");
	if (posend == -1)
	{
		posend = strTempData.Find("\n");
	}	
	CString strUser;
	if (-1 != posend)
	{
		int posbeg = strTempData.Find("=");
		if (-1 != posbeg)
		{
			strUser = strTempData.Mid(posbeg + 1,posend - posbeg - 1);
			strUser = ConverHttp2Html(strUser);
			ConvertUtf8toGbk(strUser,strUser.GetLength(),strUser);
		}	
	}
	if (strUser.IsEmpty())
	{
		return FALSE;
	}
	CString txt;
	txt.Format("user=%s\r\nstate=µÇÂ½\r\n",strUser);
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemedia_bdyy.indexfile.txt", 
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("phonemedia_bdyy","phonemedia_bdyy",txt,pHttpSession->m_dceprocotol->m_httpMissObj);	
	return TRUE;
}

BOOL IsPhoneMediaPPTV(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("api.passport.pptv.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("login/ex_login.") != -1 )  )
	{
		return 1;
	}	
	return FALSE;
}

BOOL PhoneMediaPPTV( HttpSession *pHttpSession,const char *pPacket ,int Value)
{
	CString tem(pHttpSession->m_Requestline.m_URI);
	CStringArray arrtemp;
	PhoneMediaSplitString(tem,arrtemp,'&');
	CString username,password;
	for (int i = 0; i < arrtemp.GetSize();i++)
	{
		if (arrtemp[i].Find("username") != -1)
		{
			username = PhoneMediaGetItemValue(arrtemp[i],'=');
			username = ConverHttp2Html(username);
			ConvertUtf8toGbk(username,username.GetLength(),username);
		}
		if (arrtemp[i].Find("password") != -1)
		{
			password = PhoneMediaGetItemValue(arrtemp[i],'=');
		}
	}
	if ( username.IsEmpty() || password.IsEmpty())
	{
		return FALSE;
	}
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nstate=%s\r\n",username,password,"µÇÂ½");
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemedia_pptv.indexfile.txt", 
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("phonemedia_pptv","phonemedia_pptv",txt,pHttpSession->m_dceprocotol->m_httpMissObj);	
	return TRUE;
}

BOOL IsPhoneMediaTianTianPlay(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("v2.ttus.ttpod.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/ttus/user") != -1 )  )
	{
		return 1;
	}	
	return FALSE;
}


BOOL PhoneMediaTianTianPlay( HttpSession *pHttpSession,const char *pPacket ,int Value)
{

	char *pHead = strstr(pPacket,"args\":{");
	if (!pHead)
	{
		return FALSE;
	}
	CString strTempData(pHead + strlen("args\":{") + 1);
	CStringArray arrTemp;
	PhoneMediaSplitString(strTempData,arrTemp,',');	
	int arrsize = arrTemp.GetSize();
	CString strUser,strPass;
	for (int i = 0; i < arrsize; i++)
	{
		
		if ( arrTemp[i].Find("user_name") != -1)
		{
			CString strtemp = arrTemp[i];
			strUser = PhoneMediaGetItemValue(arrTemp[i],':');
			strUser.Replace("\"","");
			strUser.Replace("}","");
			strUser.Replace("{","");
		}
		if  (arrTemp[i].Find("password") != -1)
		{
			strPass = PhoneMediaGetItemValue(arrTemp[i],':');
			strPass.Replace("\"","");
			strPass.Replace("}","");
			strPass.Replace("{","");
		}	
	}
	if (strUser.IsEmpty() )
	{
		return FALSE;
	}
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nstate=µÇÂ½\r\n",strUser,strPass);
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemedia_ttdt.indexfile.txt", 
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("phonemedia_ttdt","phonemedia_ttdt",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
	return TRUE;
}


BOOL IsPhoneMediaHePaiSchool(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("campusvideo.herpar.cn")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("userloginMobile") != -1 )  )
	{
		return 1;
	}
	return FALSE;
}

BOOL PhoneMediaHePaiSchool( HttpSession *pHttpSession,const char *pPacket ,int Value)
{
	/*
	if (1 == Value)
	{
		CString txt;
		CString tem(pPacket);
		CString username = tem.Mid(tem.Find("%E7%94%A8%E6%88%B7") + 18,tem.Find("&age") - tem.Find("%E7%94%A8%E6%88%B7") - 18);
		txt.Format("user=%s\r\n");
		CString smac = pHttpSession->m_smac;
		hepaischoolmap.SetAt(smac,txt);
	}
	if(2 == Value)
	{
		CString txt;
		int temmmp = strlen(pPacket);
		CString tem(pPacket+temmmp+1);
		CString imei = tem.Mid(tem.Find("&imei") + 6,tem.Find("&imsi") - tem.Find("&imei") - 6);
		CString imsi = tem.Mid(tem.Find("&imsi") + 6,tem.Find("&nt") - tem.Find("&imsi") - 6);
		txt.Format("imei=%s\r\nnick=%s\r\n",imei,imsi);
		CString tmp;
		hepaischoolmap.Lookup(pHttpSession->m_smac,tmp);
		txt = tmp + txt;
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemedia_hpxy.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("phonemedia_hpxy","phonemedia_hpxy",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	
		*/	
	return FALSE;
	
}

BOOL IsPhoneMediaXinHuaTv(HttpSession *pHttpSession)
{
	/*
	if(  ( pHttpSession->m_Requestline.m_Host.Find("push2.sj.91.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("api.ashx?act") != -1 ))
	{
		return 1;
	}
	*/
	return FALSE;
}
/*
BOOL PhoneMediaXinHuaTv( HttpSession *pHttpSession,const char *pPacket ,int Value)
{
	
	int temmmp = strlen(pPacket);
	CString tem(pPacket+temmmp+1);

	int nposbeg = -1;
	int nposend = -1;
	int noffset = 0;
	
	nposbeg = tem.Find("&imei");
	nposend = tem.Find("&imsi");
	noffset = 6;
	CString imei;
	if ( (-1 != nposbeg ) && (-1 != nposend))
	{
		imei = tem.Mid(nposbeg + noffset,nposend - nposbeg - noffset);
	}
	else
	{
		return FALSE;
	}
	
	nposbeg = tem.Find("&imsi");
	nposend = tem.Find("&nt");
	noffset = 6;
	CString imsi;
	if ( (-1 != nposbeg ) && (-1 != nposend))
	{
		imsi = tem.Mid(nposbeg + noffset,nposend - nposbeg - noffset);
	}
	else
	{
		return FALSE;
	}

	nposbeg = tem.Find("&dm");
	nposend = tem.Find("&chl");
	noffset = 4;
	CString model;
	if ( (-1 != nposbeg ) && (-1 != nposend))
	{
		model = tem.Mid(nposbeg + noffset,nposend - nposbeg - noffset);
	}
	else
	{
		return FALSE;
	}


	CString txt;
	txt.Format("imei=%s\r\nnick=%s\r\nwebname=%s\r\nstate=%s\r\n",imei,imsi,model,"´ò¿ªÊ×Ò³");
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemeida_xhds.indexfile.txt", 
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("phonemedia_xhds","phonemeida_xhds",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
	return TRUE;
}
*/

BOOL IsPhoneMediaYouKu(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("user.api.3g.youku.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/user/passport/login") != -1 )   )	
	{
		return TRUE;
	}
	return FALSE;
}


BOOL PhoneMediaYouKu( HttpSession *pHttpSession, const char *pPacket )
{
	CString strTempData(pHttpSession->m_Requestline.m_URI);

	CStringArray arrTemp;
	PhoneMediaSplitString(strTempData,arrTemp,'&');	
	int arrsize = arrTemp.GetSize();
	CString strUser,strIMEI;
	for (int i = 0; i < arrsize; i++)
	{
		if ( arrTemp[i].Find("uname=") != -1)
		{
			strUser = PhoneMediaGetItemValue(arrTemp[i],'=');
			strUser = ConverHttp2Html(strUser);
			ConvertUtf8toGbk(strUser,strUser.GetLength(),strUser);
		}
		if ( arrTemp[i].Find("imei=") != -1)
		{
			strIMEI = PhoneMediaGetItemValue(arrTemp[i],'=');
		}
	}	
	if (strUser.IsEmpty() || strIMEI.IsEmpty() )
	{
		return FALSE;
	}	
	CString txt;
	txt.Format("user=%s\r\nimei=%s\r\nstate=µÇÂ½\r\n",strUser,strIMEI);
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemeida_yk.indexfile.txt", 
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("phonemedia_yk","phonemedia_yk",txt,pHttpSession->m_dceprocotol->m_httpMissObj);	
	return TRUE;
}


BOOL IsPhoneMediaKaiXunVideo(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("auth.kascend.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("method=auth.login") != -1 ))
	{
		return 1;
	}	
	return FALSE;
}

BOOL PhoneMediaKaiXunVideo(HttpSession *pHttpSession,const char *pPacket ,int Value)
{
	
	CString tem(pHttpSession->m_Requestline.m_URI);
	
	int nposbeg = -1;
	int nposend = -1;
	int noffset = 0;
	
	nposbeg = tem.Find("&imei");
	nposend = tem.Find("&mac");
	noffset = 6;
	CString imei;
	if ( (-1 != nposbeg ) && (-1 != nposend))
	{
		imei = tem.Mid(nposbeg + noffset,nposend - nposbeg - noffset);
	}
	else
	{
		return FALSE;
	}
	
	CString txt;
	txt.Format("imei=%s\r\nstate=%s\r\n",imei,"µÇÂ¼");
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemedia_kxsp.indexfile.txt", 
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("phonemedia_kxsp","phonemedia_kxsp",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
	return TRUE;
}

BOOL IsPhoneMediaKWMusic(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("i.kuwo.cn")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("login_kw_") != -1 ))
	{
		return 1;
	}
	
	return FALSE;
}

BOOL PhoneMediaKWMusic( HttpSession *pHttpSession,const char *pPacket ,int Value)
{
	
	CString tem(pHttpSession->m_Requestline.m_URI);

	int nposbeg = -1;
	int nposend = -1;
	int noffset = 0;
	
	nposbeg = tem.Find("&dev_id");
	nposend = tem.Find("&dev_name");
	noffset = 8;
	CString imei;
	if ( (-1 != nposbeg ) && (-1 != nposend))
	{
		imei = tem.Mid(nposbeg + noffset,nposend - nposbeg - noffset);
	}
	else
	{
		return FALSE;
	}
	
	nposbeg = tem.Find("&username");
	nposend = tem.Find("&password");
	noffset = 10;
	CString phonenum;
	if ( (-1 != nposbeg ) && (-1 != nposend))
	{
		phonenum = tem.Mid(nposbeg + noffset,nposend - nposbeg - noffset);
		phonenum.Replace("%25","%");
		phonenum = ConverHttp2Html(phonenum);
		ConvertUtf8toGbk(phonenum,phonenum.GetLength(),phonenum);
	}
	else
	{
		return FALSE;
	}

	
	nposbeg = tem.Find("&password");
	nposend = tem.Find("&dev_id");
	noffset = 10;
	CString password;
	if ( (-1 != nposbeg ) && (-1 != nposend))
	{
		password = tem.Mid(nposbeg + noffset,nposend - nposbeg - noffset);
	}
	else
	{
		return FALSE;
	}

	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nimei=%s\r\nstate=%s\r\n",phonenum,password,imei,"µÇÂ¼");

	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemedia_kwyy.indexfile.txt", 
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("phonemedia_kwyy","phonemedia_kwyy",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
	return TRUE;
}


BOOL IsPhoneMediaQQMusic(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("comet2.music.qq.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/listen") != -1 )  )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL PhoneMediaQQMusic(HttpSession *pHttpSession,const char *pPacket,int Action)
{
	CString tem(pHttpSession->m_Requestline.m_URI);
	int nposbeg = -1;
	int nposend = -1;
	int noffset = 0;
	CStringArray str;
	PhoneMediaSplitString(tem,str,'&');
	CString qqnum;
	for (int i = 0 ; i < str.GetSize() ; i++)
	{
		CString temp = str.GetAt(i);
		if (((nposbeg = temp.Find("uin=")) != -1))
		{
			nposend = temp.Find('=',nposbeg);
			if (nposbeg != -1)
			{
				qqnum = temp.Mid(nposend  + 1,strlen(temp) - nposend - 1);
			}	
		}
	}
	if (qqnum.IsEmpty())
	{
		return FALSE;
	}
	else
	{
		/*
		CString txt;
		txt.Format("user=%s\r\nstate=µÇÂ½\r\n",qqnum);
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemedia_qqyy.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
//		pHttpSession->WriteIndexFile("phonemedia_qqyy","phonemedia_qqyy",txt,pHttpSession->m_dceprocotol->m_httpMissObj);*/
		return TRUE;
	}

	return FALSE;
}


BOOL IsPhoneMediaKaoLaFM(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("user.kaolafm.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/usercenter/login.json") != -1 ))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL PhoneMediaKaoLaFM(HttpSession *pHttpSession,const char *pPacket,int Value)
{
	CString strTempData(pPacket);
	CStringArray arrTemp;
	PhoneMediaSplitString(strTempData,arrTemp,'\n');

	CString strIMEI,strIMSI,strModel;

	int arrsize = arrTemp.GetSize();
	for (int i = 0; i < arrsize; i++)
	{
		if ( arrTemp[i].Find("imei") != -1)
		{
			strIMEI = PhoneMediaGetItemValue(arrTemp[i],':');
			strIMEI.TrimLeft();
		}
		if ( arrTemp[i].Find("imsi") != -1)
		{
			strIMSI = PhoneMediaGetItemValue(arrTemp[i],':');
			strIMSI.TrimLeft();
		}
		if ( arrTemp[i].Find("agent") != -1)
		{
			strModel = PhoneMediaGetItemValue(arrTemp[i],':');
			strModel.TrimLeft();
		}
	}

	if (strIMEI.IsEmpty() || strIMSI.IsEmpty() || strModel.IsEmpty())
	{
		return FALSE;
	}
	
	CString txt;
	txt.Format("imei=%s\r\nnick=%s\r\nwebname=%s\r\n",strIMEI,strIMSI,strModel);
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemedia_klfm.indexfile.txt", 
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("phonemedia_klfm","phonemedia_klfm",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
	return TRUE;
	
}
