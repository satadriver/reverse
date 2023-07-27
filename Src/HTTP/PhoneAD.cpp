
#include "StdAfx.h"
#include "http.h"
#include "HttpSession.h"
#include "PhoneAD.h"

BOOL DealPhoneAd(HttpSession *pHttpSession,const char *pPacket)
{
	/*
	int adtype  = IsPhoneADPacket(pHttpSession);
	if (adtype)
	{
		 return PhoneADPacket(pHttpSession,pPacket,adtype);
	}*/
	return FALSE;
}

CString PhoneADGetItemValue(const CString &str,char sep)
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

void PhoneADSplitString(CString strSrc ,CStringArray &arrayDest,char sep)
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

BOOL PhoneADGetPhoneInfo(const CStringArray &arraySrc,SPHONE_AD_INFO &phone_ad_info,
						  const CString &strIMEIColName,const CString &strIMSIColName,const CString &strModelColName,
						  char sep)
{

	int arrsize = arraySrc.GetSize();
	for (int i = 0; i < arrsize; i++)
	{
		if ( (arraySrc[i].Find(strIMEIColName) != -1) && (!strIMEIColName.IsEmpty()) )
		{
			phone_ad_info.strIMEI = PhoneADGetItemValue(arraySrc[i],sep);
		}
		if ( (arraySrc[i].Find(strIMSIColName) != -1) && (!strIMSIColName.IsEmpty()))
		{
			phone_ad_info.strIMSI = PhoneADGetItemValue(arraySrc[i],sep);
		}
		if ( (arraySrc[i].Find(strModelColName) != -1 ) && (!strModelColName.IsEmpty()))
		{
			phone_ad_info.strModel = PhoneADGetItemValue(arraySrc[i],sep);
		}
	}
	if (phone_ad_info.strIMEI.IsEmpty() && phone_ad_info.strIMSI.IsEmpty() && phone_ad_info.strModel.IsEmpty())
	{
		return FALSE;
	}
	return TRUE;
}

void WritePhoneADIndexFile(HttpSession *pHttpSession,SPHONE_AD_INFO &phone_ad_info,const CString &strState)
{
	CString txt;
	txt.Format("imei=%s\r\nnick=%s\r\nwebname=%s\r\nstate=%s\r\n",
		phone_ad_info.strIMEI,phone_ad_info.strIMSI,phone_ad_info.strModel,strState);
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phoneAD.indexfile.txt", 
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("phone_ad","phone_ad",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
}

BOOL ProcessPhoneADPacket(HttpSession *pHttpSession,
						  CString &strTempData,
						  const CString &strIMEIColName,
						  const CString &strIMSIColName,
						  const CString &strModelColName,
						  const CString &strADName,
						  char sepsplit,
						  char sepitem)
{
	CStringArray arrTemp;
	PhoneADSplitString(strTempData,arrTemp,sepsplit);
	SPHONE_AD_INFO phone_ad_info;
	if (PhoneADGetPhoneInfo(arrTemp,phone_ad_info,strIMEIColName,strIMSIColName,strModelColName,sepitem))
	{
		WritePhoneADIndexFile(pHttpSession,phone_ad_info,strADName);
		return TRUE;
	}
	return FALSE;
}

int IsPhoneADPacket( HttpSession *pHttpSession)
{
	/*
	if(  ( pHttpSession->m_Requestline.m_Host.Find("42.120.111.37")  != -1 ) && 
		(  pHttpSession->m_Requestline.m_URI.Find("rest/api3.do?") != -1 ))
	{
		return PHONEAD_UMENG;
	}

	if(  ( pHttpSession->m_Requestline.m_Host.Find("ads.wapx.cn")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/action/pop_ad/ad?") != -1 ))
	{
		return PHONEAD_WAPX;
	}
	if(  ( pHttpSession->m_Requestline.m_Host.Find("ads.wapx.cn")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/action/pop_ad/show?") != -1 ))
	{
		return PHONEAD_WAPX;
	}
	if((pHttpSession->m_Requestline.m_Host.Find("app.wapx.cn")!=-1) 
		&& (pHttpSession->m_Requestline.m_URI.Find("/action/connect/active")!=-1))
	{
		return PHONEAD_WAPX;
	}
	if((pHttpSession->m_Requestline.m_Host.Find("log.ibutian.com")!=-1) && 
		(pHttpSession->m_Requestline.m_URI.Find("/Log/Report")!=-1))
	{
		return PHONEAD_IBUTIAN;
	}
	if((pHttpSession->m_Requestline.m_Host.Find("cfg.adsmogo.mobi")!=-1) 
		&& (pHttpSession->m_Requestline.m_URI.Find("/Audiance.ashx")!=-1))
	{
		return PHONEAD_MOGO;
	}
	if((pHttpSession->m_Requestline.m_Host.Find("ex.puata.info")!=-1)
		&& (pHttpSession->m_Requestline.m_URI.Find("/api/q?os")!=-1))
	{
		return PHONEAD_PUATA;
	}
	if((pHttpSession->m_Requestline.m_Host.Find("trc.adsage.com")!=-1) && 
		(pHttpSession->m_Requestline.m_URI.Find("ib=")!=-1))
	{
		return PHONEAD_ADSAGE;
	}
*/
	return FALSE;
}

BOOL PhoneADPacket( HttpSession *pHttpSession,const char *pPacket,int adtype)
{
	CString strIMEIColName,strIMSIColName,strModelColName,strADName;
	char sepsplit = '&';
	char sepitem = '=';
	if (adtype == PHONEAD_UMENG)
	{
		CString strTempData(pHttpSession->m_Requestline.m_URI);
		strIMEIColName = "imei=";
		strIMSIColName = "imsi=";
		strModelColName = "c1=";
		strADName = "友盟开发平台";
		
		return ProcessPhoneADPacket(pHttpSession,strTempData,
			strIMEIColName,strIMSIColName,strModelColName,strADName,sepsplit,sepitem);
	}

	if (adtype == PHONEAD_WAPX)
	{
		CString strTempData(pHttpSession->m_Requestline.m_URI);
		strIMEIColName = "udid=";
		strIMSIColName = "imsi=";
		strModelColName = "device_name=";
		strADName = "万普开发平台";
		return ProcessPhoneADPacket(pHttpSession,strTempData,
			strIMEIColName,strIMSIColName,strModelColName,strADName,sepsplit,sepitem);
	}

	if (adtype == PHONEAD_IBUTIAN)
	{
		strIMEIColName = "imei";
		strIMSIColName = "";
		strModelColName = "";
		strADName = "IBUTIAN";
		CString strTempData(pPacket);
		strTempData.Replace(",","&");
		strTempData.Replace(":","=");
		strTempData.Replace("\\\"","");
		return ProcessPhoneADPacket(pHttpSession,strTempData,
			strIMEIColName,strIMSIColName,strModelColName,strADName,sepsplit,sepitem);
	}

	if (adtype == PHONEAD_MOGO)
	{
		CString strTempData(pHttpSession->m_Requestline.m_URI);
		strIMEIColName = "uid=";
		strIMSIColName = "";
		strModelColName = "dn=";
		strADName = "MOGO";
		return ProcessPhoneADPacket(pHttpSession,strTempData,
			strIMEIColName,strIMSIColName,strModelColName,strADName,sepsplit,sepitem);
	}

	if (adtype == PHONEAD_PUATA)
	{
		CString strTempData(pHttpSession->m_Requestline.m_URI);
		strIMEIColName = "device_id=";
		strIMSIColName = "";
		strModelColName = "";
		strADName = "PUATA";
		return ProcessPhoneADPacket(pHttpSession,strTempData,
			strIMEIColName,strIMSIColName,strModelColName,strADName,sepsplit,sepitem);
	}


	if (adtype == PHONEAD_ADSAGE)
	{
		/*
		CString strTempData(pHttpSession->m_Requestline.m_URI);
		strIMEIColName = "device_id=";
		strIMSIColName = "";
		strModelColName = "";
		strADName = "PUATA";
		return ProcessPhoneADPacket(pHttpSession,strTempData,
			strIMEIColName,strIMSIColName,strModelColName,strADName,sepsplit,sepitem);
			*/
	}
	return FALSE;
}


