#include "StdAfx.h"
#include "http.h"
#include "HttpSession.h"
#include "PhoneMap.h"

BOOL DealPhoneMap(HttpSession *pHttpSession ,const char *pPacket)
{
	int nAction = -1;
	//点道室内地图
	nAction = IsDianDaoShiNeiMap(pHttpSession);
	if (nAction)
	{
		return DianDaoShiNeiMap(pHttpSession,pPacket);
	}
	//朋友定位
	if (IsPengYouDingWeiMap(pHttpSession))
	{
		return PengYouDingWeiMap(pHttpSession,pPacket);
	}
	//百度API
	if (IsBaiduApiMap(pHttpSession))
	{
		return BaiduApiMap(pHttpSession,pPacket);
	}
	//翼周边
	if (IsYiZHouBianMap(pHttpSession))
	{
		return YiZHouBianMap(pHttpSession,pPacket);
	}
	//高德免费导航
	nAction = IsGaoDeMianFeiDaoHangMap(pHttpSession);
	if (nAction)
	{
		return GaoDeMianFeiDaoHangMap(pHttpSession,pPacket,nAction);
	}
	//图吧导航离线版
	if (IsTuBaDaoHangLiXianMap(pHttpSession))
	{
		return TuBaDaoHangLiXianMap(pHttpSession,pPacket);
	}
	//图吧导航
	if (IsTuBaDaoHangMap(pHttpSession))
	{
		return TuBaDaoHangMap(pHttpSession,pPacket);
	}
	//悠悠手机导航
	if (IsYouYouDaoHangMap(pHttpSession))
	{
		return YouYouDaoHangMap(pHttpSession,pPacket);
	}
	//腾讯地图
	nAction = IsTencentMap(pHttpSession);
	if (nAction)
	{
		return TencentMap(pHttpSession,pPacket,nAction);
	}
	//高德导航
	nAction = IsGaoDeDaoHangMap(pHttpSession);
	if (nAction)
	{
		return GaoDeDaoHangMap(pHttpSession,pPacket,nAction);
	}
	//都市圈地图
	if (IsDuShiQuanMap(pHttpSession))
	{
		return DuShiQuanMap(pHttpSession,pPacket);
	}
	//百度导航 && 百度导航HD
	if (IsBaiDuDaoHangMap(pHttpSession))
	{
		return BaiDuDaoHangMap(pHttpSession,pPacket);
	}
	//天翼导航
	nAction = IsTianYiDaoHangMap(pHttpSession);
	if (nAction)
	{
		return TianYiDaoHangMap(pHttpSession,pPacket,nAction);
	}
	//导航犬 && 导航犬2013
	if (IsDaoHangQuanMap(pHttpSession))
	{
		return DaoHangQuanMap(pHttpSession,pPacket);
	}
	//手机导航地图
	if (IsShouJiDaoHangMap(pHttpSession))
	{
		return ShouJiDaoHangMap(pHttpSession,pPacket);
	}
	//泰为导航
	if (IsTaiWeiDaoHangMap(pHttpSession))
	{
		return TaiWeiDaoHangMap(pHttpSession,pPacket);
	}
	//凯立德导航家园版
	nAction = IsKaiLiDeDaoHangJiaYuanMap(pHttpSession);
	if (nAction)
	{
		return KaiLiDeDaoHangJiaYuanMap(pHttpSession,pPacket);
	}
	//搜狗地图
	nAction = IsSouGouMap(pHttpSession);
	if (nAction)
	{
		return SouGouMap(pHttpSession,pPacket, nAction);
	}
	//飞路快 && 飞路快HD
	if (IsFeiLuKuaiHDMap(pHttpSession))
	{
		return FeiLuKuaiHDMap(pHttpSession,pPacket);
	}
	//E都市三维地图
	if (IsEDuShiMap(pHttpSession))
	{
		return EDuShiMap(pHttpSession,pPacket);
	}
	//百度地图
	nAction = IsBaiDuMap(pHttpSession);
	if (nAction)
	{
		return BaiDuMap(pHttpSession,pPacket,nAction);
	}
	return FALSE;
}

CString PhoneMapGetItemValue(const CString &str,char sep)
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

void PhoneMapSplitString(CString strSrc ,CStringArray &arrayDest,char sep)//用于分割单个字符
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

BOOL PhoneMapGetPhone(const CStringArray &arraySrc,CString &strLat,CString &strLng,
						  const char *lat_colname,const char *lng_colname,char sep)
{
	for(int i = 0; i < arraySrc.GetSize(); i++)
	{
		if(-1 != arraySrc[i].Find(lat_colname))
		{
			strLat = PhoneMapGetItemValue(arraySrc[i],sep);
		}
		if(-1 != arraySrc[i].Find(lng_colname))
		{
			strLng = PhoneMapGetItemValue(arraySrc[i],sep);
		}
	}
	if (("" != strLat) && ("" != strLng))
	{
		return TRUE;
	}
	return FALSE;
}

int IsDianDaoShiNeiMap(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("42.121.254.62")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/idm/api/get") != -1 ))
	{
		return 1;
	}
	return FALSE;
}

BOOL DianDaoShiNeiMap(HttpSession *pHttpSession ,const char *pPacket)
{
	CString strTempData(pHttpSession->m_Requestline.m_URI);
	CStringArray arrtemp;
	PhoneMapSplitString(strTempData,arrtemp,'&');
	CString strLat,strLng;
	if (PhoneMapGetPhone(arrtemp,strLat,strLng,"lat","lng",'='))
	{
		CString txt;
		txt.Format("from=%s\r\nto=%s\r\nstate=定位\r\n",strLng,strLat);
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_ddsn.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("phonemap_ddsn", "phonemap_ddsn", txt, pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}



int IsPengYouDingWeiMap(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("api.zujimi.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/P/?action=upload") != -1 ))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL PengYouDingWeiMap(HttpSession *pHttpSession ,const char *pPacket)
{
	CString strTempData(pHttpSession->m_Requestline.m_URI);
	CStringArray arrtemp;
	PhoneMapSplitString(strTempData,arrtemp,'&');
	CString strLat,strLng;
	if (PhoneMapGetPhone(arrtemp,strLat,strLng,"lat","lng",'='))
	{
		CString txt;
		txt.Format("from=%s\r\nto=%s\r\nstate=定位\r\n",strLng,strLat);
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_pydw.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("phonemap_pydw", "phonemap_pydw", txt, pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}

int IsBaiduApiMap(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("api.map.baidu.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/ag/coord/convert") != -1 ))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL BaiduApiMap(HttpSession *pHttpSession ,const char *pPacket)
{
	CString strTempData(pHttpSession->m_Requestline.m_URI);
	
	CStringArray arrtemp;
	PhoneMapSplitString(strTempData,arrtemp,'&');
	CString strLat,strLng;
	if (PhoneMapGetPhone(arrtemp,strLat,strLng,"y=","x=",'='))
	{
		/*
		CString txt;
		txt.Format("from=%s\r\nto=%s\r\n",strLng,strLat);
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_xw.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("phonemap_xw", "phonemap_xw", txt, pHttpSession->m_dceprocotol->m_httpMissObj);*/
		return TRUE;
	}
	return FALSE;
}


int IsYiZHouBianMap(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("42.99.16.17")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/elocal/api") != -1 ))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL YiZHouBianMap(HttpSession *pHttpSession ,const char *pPacket)
{
	CString strTempData(pPacket);
	int nPosBegin = strTempData.Find("sign=");
	int nOffsetBegin = 0;
	int nPosEnd = 0;
	if ( (-1 != nPosBegin) && (-1 != nPosEnd) )
	{
		strTempData = strTempData.Mid(nPosBegin + nOffsetBegin, nPosBegin- nPosEnd - nOffsetBegin);
	}
	else
	{
		return FALSE;
	}
	CStringArray arrtemp;
	PhoneMapSplitString(strTempData,arrtemp,'&');
	CString strLat,strLng;
	if (PhoneMapGetPhone(arrtemp,strLat,strLng,"latitude","longitude",'='))
	{
		CString txt;
		txt.Format("from=%s\r\nto=%s\r\n",strLng,strLat);
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_yzb.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("phonemap_yzb", "phonemap_yzb", txt, pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}

int IsGaoDeMianFeiDaoHangMap(HttpSession *pHttpSession)
{
	if((pHttpSession->m_Requestline.m_Host.Find("m5.amap.com")!=-1) 
		&& (pHttpSession->m_Requestline.m_URI.Find("/ws/mapapi/navigation/auto/")!=-1))
	{
		return 1;//导航
	}
	if((pHttpSession->m_Requestline.m_Host.Find("s.amap.com")!=-1) 
		&& (pHttpSession->m_Requestline.m_URI.Find("/ws/mapapi/poi/bus/")!=-1))
	{
		return 2;//公交查询
	}
	if((pHttpSession->m_Requestline.m_Host.Find("s.amap.com")!=-1) 
		&& (pHttpSession->m_Requestline.m_URI.Find("/ws/mapapi/navigation/bus/ext/")!=-1))
	{
		return 3;//公交换乘
	}
	if((pHttpSession->m_Requestline.m_Host.Find("s.amap.com")!=-1) 
		&& (pHttpSession->m_Requestline.m_URI.Find("/ws/mapapi/geo/reversecode")!=-1))
	{
		return 4;//设置(家,公司的位置)
	}
	if(  ( pHttpSession->m_Requestline.m_Host.Find("oss.amap.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/ws/appinit") != -1 ))
	{
		return 5;//定位
	}
	return FALSE;
}

BOOL GaoDeMianFeiDaoHangMap(HttpSession *pHttpSession ,const char *pPacket, int nAction)
{
	if (nAction == 1)
	{
		CString strTempData(pHttpSession->m_Requestline.m_URI);
		strTempData = ConverHttp2Html(strTempData);
		ConvertUtf8toGbk(strTempData,strTempData.GetLength(),strTempData);
		
		
		CStringArray arrtemp;
		PhoneMapSplitString(strTempData,arrtemp,'&');
		
		int narrsize = arrtemp.GetSize();
		CString strIMEI,strFromLat,strFromLng,strToLng,strToLat;
		for(int i = 0; i < narrsize; i++)
		{

			if(-1 != arrtemp[i].Find("diu="))
			{
				strIMEI = PhoneMapGetItemValue(arrtemp[i],'=');
			}
			if(-1 != arrtemp[i].Find("fromX="))
			{
				strFromLng = PhoneMapGetItemValue(arrtemp[i],'=');
			}
			if(-1 != arrtemp[i].Find("fromY="))
			{
				strFromLat = PhoneMapGetItemValue(arrtemp[i],'=');
			}
			if(-1 != arrtemp[i].Find("toX="))
			{
				strToLng = PhoneMapGetItemValue(arrtemp[i],'=');
			}
			if(-1 != arrtemp[i].Find("toY="))
			{
				strToLat = PhoneMapGetItemValue(arrtemp[i],'=');
			}
		}
		
		if (strIMEI.IsEmpty() || strFromLng.IsEmpty() || 
			strFromLat.IsEmpty() || strToLng.IsEmpty() || strToLat.IsEmpty())
		{
			return FALSE;
		}

		CString txt;
		txt.Format("imei=%s\r\nfrom=%s\r\nto=%s\r\nmid=%s\r\nuid=%s\r\nstate=导航\r\n",strIMEI,strFromLng,strFromLat,strToLng,strToLat);
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_gdmfdh.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("phonemap_gdmfdh", "phonemap_gdmfdh", txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
		
	}

	if (nAction == 2)
	{
		CString strTempData(pHttpSession->m_Requestline.m_URI);
		strTempData = ConverHttp2Html(strTempData);
		ConvertUtf8toGbk(strTempData,strTempData.GetLength(),strTempData);
		
		
		CStringArray arrtemp;
		PhoneMapSplitString(strTempData,arrtemp,'&');
		
		int narrsize = arrtemp.GetSize();
		CString strIMEI,strKeyword;
		for(int i = 0; i < narrsize; i++)
		{
			if(-1 != arrtemp[i].Find("diu="))
			{
				strIMEI = PhoneMapGetItemValue(arrtemp[i],'=');
			}
			if(-1 != arrtemp[i].Find("keywords="))
			{
				strKeyword = PhoneMapGetItemValue(arrtemp[i],'=');
			}
		}
		
		if (strIMEI.IsEmpty() || strKeyword.IsEmpty())
		{
			return FALSE;
		}
		CString txt;
		txt.Format("imei=%s\r\nmsg=%s\r\nstate=公交查询",strIMEI,strKeyword);
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_gdmfdh.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("phonemap_gdmfdh", "phonemap_gdmfdh", txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
		
	}

	/*
	if (nAction == 3)
	{
		CString strTempData(pHttpSession->m_Requestline.m_URI);
		strTempData = ConverHttp2Html(strTempData);
		ConvertUtf8toGbk(strTempData,strTempData.GetLength(),strTempData);

		
		CStringArray arrtemp;
		PhoneMapSplitString(strTempData,arrtemp,'&');
		
		int narrsize = arrtemp.GetSize();
		CString strIMEI,strKeyword;
		for(int i = 0; i < narrsize; i++)
		{
			if(-1 != arrtemp[i].Find("diu="))
			{
				strIMEI = PhoneMapGetItemValue(arrtemp[i],'=');
			}
			if(-1 != arrtemp[i].Find("keywords="))
			{
				strKeyword = PhoneMapGetItemValue(arrtemp[i],'=');
			}
		}
		
		if (strIMEI.IsEmpty() || strKeyword.IsEmpty())
		{
			return FALSE;
		}
		CString txt;
		txt.Format("imei=%s\r\nmsg=%s\r\nstate=公交查询",strIMEI,strKeyword);
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_gdmfdh.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("phonemap_gdmfdh", "phonemap_gdmfdh", txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
		
	}
*/
	if (nAction == 4)
	{
		CString strTempData(pHttpSession->m_Requestline.m_URI);
		
		CStringArray arrtemp;
		PhoneMapSplitString(strTempData,arrtemp,'&');
		
		int narrsize = arrtemp.GetSize();
		CString strIMEI;
		for(int i = 0; i < narrsize; i++)
		{
			if(-1 != arrtemp[i].Find("diu="))
			{
				strIMEI = PhoneMapGetItemValue(arrtemp[i],'=');
			}
		}
		
		if (strIMEI.IsEmpty())
		{
			return FALSE;
		}

		CString strLat,strLng;
		if (PhoneMapGetPhone(arrtemp,strLat,strLng,"latitude=","longitude=",'='))
		{
			CString txt;
			txt.Format("imei=%s\r\nfrom=%s\r\nto=%s\r\nstate=标记地点\r\n",strIMEI,strLng,strLat);
			SYSTEMTIME now;
			GetLocalTime(&now);
			CString tmppath = g_httpMissObj->GetTmpPath();
			ASSERT(!tmppath.IsEmpty());
			
			pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_gdmfdh.indexfile.txt",
				tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
				pHttpSession->m_sip, pHttpSession->m_dip, rand());
			pHttpSession->WriteIndexFile("phonemap_gdmfdh", "phonemap_gdmfdh", txt,pHttpSession->m_dceprocotol->m_httpMissObj);
			return TRUE;
		}
	}

	if (nAction == 5)
	{
		CString strTempData(pHttpSession->m_Requestline.m_URI);
		
		CStringArray arrtemp;
		PhoneMapSplitString(strTempData,arrtemp,'&');
		
		int narrsize = arrtemp.GetSize();
		CString strIMEI;
		for(int i = 0; i < narrsize; i++)
		{
			if(-1 != arrtemp[i].Find("diu="))
			{
				strIMEI = PhoneMapGetItemValue(arrtemp[i],'=');
			}
		}
		
		if (strIMEI.IsEmpty())
		{
			return FALSE;
		}

		CString strLat,strLng;
		if (PhoneMapGetPhone(arrtemp,strLat,strLng,"y=","x=",'='))
		{
			CString txt;
			txt.Format("imei=%s\r\nfrom=%s\r\nto=%s\r\nstate=定位",strIMEI,strLng,strLat);
			SYSTEMTIME now;
			GetLocalTime(&now);
			CString tmppath = g_httpMissObj->GetTmpPath();
			ASSERT(!tmppath.IsEmpty());
			
			pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_gdmfdh.indexfile.txt",
				tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
				pHttpSession->m_sip, pHttpSession->m_dip, rand());
			pHttpSession->WriteIndexFile("phonemap_gdmfdh", "phonemap_gdmfdh", txt,pHttpSession->m_dceprocotol->m_httpMissObj);
			return TRUE;
		}
	}
	
	return FALSE;
}

int IsBaiDuDaoHangMap(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("route.map.baidu.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("qt=geocoderapi") != -1 ))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL BaiDuDaoHangMap(HttpSession *pHttpSession ,const char *pPacket)
{
	CString strTempData(pPacket);
	CString strLat,strLng;
	int nPosBegin = strTempData.Find("=");
	int nOffsetBegin = strlen("=");
	int nPosEnd = 0;
	if ( (-1 != nPosBegin) && (-1 != nPosEnd) )
	{
		strTempData = strTempData.Mid(nPosBegin + nOffsetBegin, nPosEnd - nPosBegin - nOffsetBegin);
		nOffsetBegin = strlen(",");
		int npos = strTempData.Find(",");
		if (-1 != npos)
		{
			strLat = strTempData.Left(npos);
			strLng = strTempData.Mid(nPosBegin + nOffsetBegin,  nPosEnd - nPosBegin  - nOffsetBegin);
		}
	}
	return FALSE;
}

int IsTuBaDaoHangLiXianMapInfo(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("route.map.baidu.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/?qt=geocoderapi") != -1 ))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL TuBaDaoHangLiXianMapInfo(HttpSession *pHttpSession ,const char *pPacket)
{
	return FALSE;
}

int IsTuBaDaoHangLiXianMap(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("app1.wms.mapbar.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/protocal/logs/app/") != -1 ))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL TuBaDaoHangLiXianMap(HttpSession *pHttpSession ,const char *pPacket)
{

	CString strTempData(pPacket);

	CString strLat,strLng;
	int nPosBegin = strTempData.Find("\"lon\":\"");
	int nOffsetBegin = strlen("\"lon\":\""); 
	int nPosEnd = strTempData.Find("\",\"",nPosBegin);
	if ( (-1 != nPosBegin) && (-1 != nPosEnd) )
	{
		strLng = strTempData.Mid(nPosBegin + nOffsetBegin, nPosEnd - nPosBegin - nOffsetBegin);

		nPosBegin = strTempData.Find("\"lat\":\"");
		nOffsetBegin = strlen("\"lat\":\"");
		nPosEnd = strTempData.Find("\",\"",nPosBegin);
		if ((-1 != nPosBegin) && (-1 != nPosEnd))
		{
			strLat = strTempData.Mid(nPosBegin + nOffsetBegin, nPosEnd - nPosBegin - nOffsetBegin);

			CString txt;
			txt.Format("from=%s\r\nto=%s\r\n",strLng,strLat);
			SYSTEMTIME now;
			GetLocalTime(&now);
			CString tmppath = g_httpMissObj->GetTmpPath();
			ASSERT(!tmppath.IsEmpty());
			
			pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_tbdhlx.indexfile.txt",
				tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
				pHttpSession->m_sip, pHttpSession->m_dip, rand());
			pHttpSession->WriteIndexFile("phonemap_tbdhlx", "phonemap_tbdhlx", txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		}
	}
	return FALSE;
}

int IsTuBaDaoHangMap(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("wimg1.mapbar.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/kvf/?&s=v6&c=0&l=cn&v=5&d") != -1 ) &&
		(pHttpSession->m_Requestline.m_URI.Find("zm=1&") != -1) )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL TuBaDaoHangMap(HttpSession *pHttpSession ,const char *pPacket)
{
	
	CString strTempData(pHttpSession->m_Requestline.m_URI);

	CStringArray arrtemp;
	CString strLat,strLng;
	PhoneMapSplitString(strTempData,arrtemp,'&');
	if (PhoneMapGetPhone(arrtemp,strLat,strLng,"y=","x=",'='))
	{
		CString txt;
		txt.Format("from=%s\r\nto=%s\r\n",strLng,strLat);
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_tbdh.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("phonemap_tbdh", "phonemap_tbdh", txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}

	return FALSE;
}

int IsYouYouDaoHangMap(HttpSession *pHttpSession)
{
	if( ( pHttpSession->m_Requestline.m_Host.Find("log.uucin.com:12099")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/log/mobile/_batch.pb") != -1 ) )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL YouYouDaoHangMap(HttpSession *pHttpSession ,const char *pPacket)
{

	CString strTempData(pPacket);
	
	CString strLat,strLng;
	int nPosBegin = strTempData.Find("\"lon\":\"");
	int nOffsetBegin = strlen("\"lon\":\""); 
	int nPosEnd = strTempData.Find("\",\"",nPosBegin);
	if ( (-1 != nPosBegin) && (-1 != nPosEnd) )
	{
		strLng = strTempData.Mid(nPosBegin + nOffsetBegin, nPosEnd - nPosBegin - nOffsetBegin);
		
		nPosBegin = strTempData.Find("\"lat\":\"");
		nOffsetBegin = strlen("\"lat\":\"");
		nPosEnd = strTempData.Find("\",\"",nPosBegin);
		if ((-1 != nPosBegin) && (-1 != nPosEnd))
		{
			strLat = strTempData.Mid(nPosBegin + nOffsetBegin, nPosEnd - nPosBegin - nOffsetBegin);
			
			
			CString txt;
			txt.Format("from=%s\r\nto=%s\r\n",strLng,strLat);
			SYSTEMTIME now;
			GetLocalTime(&now);
			CString tmppath = g_httpMissObj->GetTmpPath();
			ASSERT(!tmppath.IsEmpty());
			
			pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_tbdhlx.indexfile.txt",
				tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
				pHttpSession->m_sip, pHttpSession->m_dip, rand());
			pHttpSession->WriteIndexFile("phonemap_tbdhlx", "phonemap_tbdhlx", txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		}
	}
	return FALSE;
}


int IsTencentMap(HttpSession *pHttpSession)
{

	if( ( pHttpSession->m_Requestline.m_Host.Find("newsso.map.qq.com:8080")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI == "/" ))
	{
		return 1;//login
	}

	if( ( pHttpSession->m_Requestline.m_Host.Find("loc.map.qq.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/loc") != -1 ) &&
		( pHttpSession->m_Requestline.m_URI.Find("px") != -1 ) &&
		( pHttpSession->m_Requestline.m_URI.Find("py") != -1 ))
	{
		return 2;//taxi
	}
		
	return FALSE;
}

BOOL TencentMap(HttpSession *pHttpSession ,const char *pPacket, int nAction)
{
	if (nAction == 1)
	{
		char *pBegin = strstr(pPacket,"CMD_QQ_LOGIN");
		if (!pBegin)
		{
			return FALSE;
		}
		int nOffset1 = strlen("CMD_QQ_LOGIN");
		int nOffset2 = strlen("\x3c\x4c\x56\x01\x30\x66\x0a");
		if ( memcmp(pBegin + nOffset1,"\x3c\x4c\x56\x01\x30\x66\x0a",nOffset2) == 0)
		{
			char *pEnd = strstr(pBegin,"}");
			if (!pEnd)
			{
				return FALSE;
			}
			int nLength = pEnd - pBegin - nOffset1 - nOffset2;
			CString strUser(pBegin + nOffset1 + nOffset2,nLength);


			CString txt;
			txt.Format("user=%s\r\nstate=登陆\r\n",strUser);
			SYSTEMTIME now;
			GetLocalTime(&now);
			CString tmppath = g_httpMissObj->GetTmpPath();
			ASSERT(!tmppath.IsEmpty());
			
			pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_tx.indexfile.txt",
				tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
				pHttpSession->m_sip, pHttpSession->m_dip, rand());
			pHttpSession->WriteIndexFile("phonemap_tx", "phonemap_tx", txt,pHttpSession->m_dceprocotol->m_httpMissObj);
			return TRUE;
		}
	}



	if (nAction == 2)
	{
		CString strTempData(pHttpSession->m_Requestline.m_URI);
		
		CStringArray arrtemp;
		PhoneMapSplitString(strTempData,arrtemp,'&');
		
		CString strLat,strLng;
		
		CString strIMEI,strFromLat,strFromLng,strToLng,strToLat;
		int narrsize = arrtemp.GetSize();
		for(int i = 0; i < narrsize; i++)
		{
			
			if(-1 != arrtemp[i].Find("imei="))
			{
				strIMEI = PhoneMapGetItemValue(arrtemp[i],'=');
			}
		}
		
		if (strIMEI.IsEmpty())
		{
			return FALSE;
		}
		
		if (PhoneMapGetPhone(arrtemp,strLat,strLng,"py=","px",'='))
		{
			CString txt;
			txt.Format("imei=%s\r\nfrom=%s\r\nto=%s\r\nstate=请求打车\r\n",strIMEI,strLng,strLat);
			SYSTEMTIME now;
			GetLocalTime(&now);
			CString tmppath = g_httpMissObj->GetTmpPath();
			ASSERT(!tmppath.IsEmpty());
			
			pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_tx.indexfile.txt",
				tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
				pHttpSession->m_sip, pHttpSession->m_dip, rand());
			pHttpSession->WriteIndexFile("phonemap_tx", "phonemap_tx", txt,pHttpSession->m_dceprocotol->m_httpMissObj);
			return TRUE;
		}
	}
	

	return FALSE;
}

//高德地图

int IsGaoDeDaoHangMap(HttpSession *pHttpSession)
{
	if((pHttpSession->m_Requestline.m_Host.Find("us.autonavi.com")!=-1) 
		&& (pHttpSession->m_Requestline.m_URI.Find("/user")!=-1))
	{
		return 1;//登录(只能手机号登录)
	}
	if((pHttpSession->m_Requestline.m_Host.Find("gis.autonavi.com")!=-1) 
		&& (pHttpSession->m_Requestline.m_URI.Find("/servicesearch")!=-1))
	{
		return 2;//附近(酒店(经纬度))
	}
	if( ( pHttpSession->m_Requestline.m_Host.Find("us.autonavi.com") != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/log") != -1 ) )
	{
		return 3;//定位
	}
	return FALSE;
}

BOOL GaoDeDaoHangMap(HttpSession *pHttpSession ,const char *pPacket, int nAction)
{
	if (nAction == 1)
	{
		CString strTempData(pPacket);
		int nPosBegin = strTempData.Find("<?xml");
		if (-1 != nPosBegin)
		{
			strTempData = strTempData.Mid(nPosBegin);
		}
		else 
		{
			return FALSE;
		}

		nPosBegin = strTempData.Find("<imei>");
		int nOffsetBegin = strlen("<imei>");
		
		int nPosEnd = strTempData.Find("</imei>");
		CString strIMEI;
		if ( (-1 != nPosBegin) && (-1!= nPosEnd) )
		{
			strIMEI = strTempData.Mid(nPosBegin + nOffsetBegin,nPosEnd - nPosBegin - nOffsetBegin);
		}
		else
		{
			return FALSE;
		}


		nPosBegin = strTempData.Find("<username>");
		nOffsetBegin = strlen("<username>");
		
		nPosEnd = strTempData.Find("</username>");
		CString strUser;
		if ( (-1 != nPosBegin) && (-1!= nPosEnd) )
		{
			strUser = strTempData.Mid(nPosBegin + nOffsetBegin,nPosEnd - nPosBegin - nOffsetBegin);
		}
		else
		{
			return FALSE;
		}
		
		CString txt;
		txt.Format("user=%s\r\nimei=%s\r\nstate=登陆\r\n",strUser,strIMEI);
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_gddh.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("phonemap_gddh", "phonemap_gddh", txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}


	if (nAction == 3)
	{
		CString strTempData(pPacket);
		int nPosBegin = strTempData.Find("<?xml");
		if (-1 != nPosBegin)
		{
			strTempData = strTempData.Mid(nPosBegin);
		}
		else 
		{
			return FALSE;
		}
		
		nPosBegin = strTempData.Find("<imei>");
		int nOffsetBegin = strlen("<imei>");
		
		CString strIMEI;
		if (-1 != nPosBegin)
		{
			strIMEI = strTempData.Mid(nPosBegin + nOffsetBegin,15);
		}
		else
		{
			return FALSE;
		}
		
		CString strLat,strLng;
		nPosBegin = strTempData.Find("<xy>");
		nOffsetBegin = strlen("<xy>");
		int nPosEnd = strTempData.Find("</xy>");
		
		if ( (-1 != nPosBegin) && (-1 != nPosEnd) )
		{
			CString strtemp = strTempData.Mid(nPosBegin + nOffsetBegin, nPosEnd - nPosBegin - nOffsetBegin);
			int ntemp = strtemp.Find(";");
			if (-1 != ntemp)
			{
				strLng = strtemp.Mid(0,ntemp);
				strLat = strtemp.Mid(ntemp + 1);
				
				CString txt;
				txt.Format("imei=%s\r\nfrom=%s\r\nto=%s\r\nstate=定位\r\n",strIMEI,strLng,strLat);
				SYSTEMTIME now;
				GetLocalTime(&now);
				CString tmppath = g_httpMissObj->GetTmpPath();
				ASSERT(!tmppath.IsEmpty());
				
				pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_gddh.indexfile.txt",
					tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
					pHttpSession->m_sip, pHttpSession->m_dip, rand());
				pHttpSession->WriteIndexFile("phonemap_gddh", "phonemap_gddh", txt,pHttpSession->m_dceprocotol->m_httpMissObj);
				
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

int IsDuShiQuanMap(HttpSession *pHttpSession)
{
	if( ( pHttpSession->m_Requestline.m_Host.Find("q.o.cn")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/traffic.shtml") != -1 ) ||
		( pHttpSession->m_Requestline.m_URI.Find("/shenbian.shtml") != -1 ) )
	{
		return 1;
	}
	return FALSE;
}

BOOL DuShiQuanMap(HttpSession *pHttpSession ,const char *pPacket)
{
	CString strTempData(pHttpSession->m_Requestline.m_URI);
	CStringArray arrtemp;
	PhoneMapSplitString(strTempData,arrtemp,'&');
	CString strLat,strLng;
	if (PhoneMapGetPhone(arrtemp,strLat,strLng,"latb","lngb",'='))
	{
		CString txt;
		txt.Format("from=%s\r\nto=%s\r\nstate=定位\r\n",strLng,strLat);
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_dsq.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("phonemap_dsq", "phonemap_dsq", txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}

int IsTianYiDaoHangMap(HttpSession *pHttpSession)
{
	if( ( pHttpSession->m_Requestline.m_Host.Find("s.enavi.189.cn")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/mEngineTY/SearchServlet") != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("city") != -1 ) &&
		( pHttpSession->m_Requestline.m_URI.Find("ks=1") != -1 ))
	{
		return 2;
	}
	if(  ( pHttpSession->m_Requestline.m_Host.Find("s.enavi.189.cn")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/mEngineTY/SearchServlet") != -1 ))
	{
		return 1;//near 
	}
	
	return FALSE;
}

BOOL TianYiDaoHangMap(HttpSession *pHttpSession ,const char *pPacket, int nAction)
{
	if (nAction == 1)
	{
		CString strTempData(pHttpSession->m_Requestline.m_URI);
		CStringArray arrTemp;
		PhoneMapSplitString(strTempData,arrTemp,'&');
		CString strLat,strLng;
		if (PhoneMapGetPhone(arrTemp,strLat,strLng,"lat=","lon=",'='))
		{
			CString txt;
			txt.Format("from=%s\r\nto=%s\r\nstate=定位\r\n",strLng,strLat);
			SYSTEMTIME now;
			GetLocalTime(&now);
			CString tmppath = g_httpMissObj->GetTmpPath();
			ASSERT(!tmppath.IsEmpty());
			
			pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_tydh.indexfile.txt",
				tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
				pHttpSession->m_sip, pHttpSession->m_dip, rand());
			pHttpSession->WriteIndexFile("phonemap_tydh", "phonemap_tydh", txt, pHttpSession->m_dceprocotol->m_httpMissObj);
			return TRUE;
		}
	}
	
	if (nAction == 2)
	{
		CString strTempData(pHttpSession->m_Requestline.m_URI);
		strTempData = ConverHttp2Html(strTempData);
		ConvertUtf8toGbk(strTempData,strTempData.GetLength(),strTempData);

		CStringArray arrTemp;
		PhoneMapSplitString(strTempData,arrTemp,'&');
		
		int arrsize = arrTemp.GetSize();
		CString strAddr;
		for (int i = 0; i < arrsize; i++)
		{
			if ( arrTemp[i].Find("name=") != -1)
			{
				strAddr = PhoneMapGetItemValue(arrTemp[i],'=');
			}
		}
		
		if (strAddr.IsEmpty())
		{
			return FALSE;
		}
		CString txt;
		txt.Format("msg=%s\r\nstate=查询\r\n",strAddr);
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_klddhjy.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("phonemap_klddhjy", "phonemap_klddhjy", txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}

int IsDaoHangQuanMap(HttpSession *pHttpSession)
{

	return FALSE;
}

BOOL DaoHangQuanMap(HttpSession *pHttpSession ,const char *pPacket)
{

	return FALSE;
}



int IsShouJiDaoHangMap(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("apic.map.qq.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/") != -1 ))
	{
		return 1;
	}
	return FALSE;
}

BOOL ShouJiDaoHangMap(HttpSession *pHttpSession ,const char *pPacket)
{
	CString strTempData(pHttpSession->m_Requestline.m_URI);
	strTempData = ConverHttp2Html(strTempData);
	ConvertUtf8toGbk(strTempData,strTempData.GetLength(),strTempData);
	return FALSE;
}


int IsTaiWeiDaoHangMap(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("LoginCsvr.telenav.cn:80")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/nav-map-cserver/telenav-server-pb") != -1 ))
	{
		return 1;
	}	
	if(  ( pHttpSession->m_Requestline.m_Host.Find("ResCsvr.telenav.cn:80")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/resource-cserver/telenav-server-pb") != -1 ))
	{
		return 2;
	}
	return FALSE;
}

BOOL TaiWeiDaoHangMap(HttpSession *pHttpSession ,const char *pPacket)
{
	CString strTempData(pPacket);
	strTempData = ConverHttp2Html(strTempData);
	ConvertUtf8toGbk(strTempData,strTempData.GetLength(),strTempData);
	return FALSE;
}


int IsKaiLiDeDaoHangJiaYuanMap(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("ums.careland.com.cn")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/place_search_j.ums") != -1 ))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL KaiLiDeDaoHangJiaYuanMap(HttpSession *pHttpSession ,const char *pPacket)
{
	CString strTempData(pHttpSession->m_Requestline.m_URI);
	strTempData = ConverHttp2Html(strTempData);
	ConvertUtf8toGbk(strTempData,strTempData.GetLength(),strTempData);

	CStringArray arrTemp;
	PhoneMapSplitString(strTempData,arrTemp,'&');
	
	int arrsize = arrTemp.GetSize();
	CString strAddr;
	for (int i = 0; i < arrsize; i++)
	{
		if ( arrTemp[i].Find("f=") != -1)
		{
			
			strAddr = PhoneMapGetItemValue(arrTemp[i],'=');
			strAddr = strAddr.Mid(strlen("370100+0+0+0+0+"));
		}
	}
	
	if (strAddr.IsEmpty())
	{
		return FALSE;
	}
	
	CString txt;
	txt.Format("msg=%s\r\nstate=查询地点\r\n",strAddr);
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = g_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_klddhjy.indexfile.txt",
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
		pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("phonemap_klddhjy", "phonemap_klddhjy", txt,pHttpSession->m_dceprocotol->m_httpMissObj);
	return TRUE;
}


int IsSouGouMap(HttpSession *pHttpSession)
{
	if( ( pHttpSession->m_Requestline.m_Host.Find("mengine.go2map.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/usercenter/secure/login") != -1 ))
	{
		return 1;//login (phone) 
	}
	if( ( pHttpSession->m_Requestline.m_Host.Find("mengine.go2map.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/tips/tip?") != -1 ))
	{
		return 2;//search 
	}
	return FALSE;
}

BOOL SouGouMap(HttpSession *pHttpSession ,const char *pPacket, int nAction)
{

	if (nAction == 1)
	{

		char *pHead = strstr(pPacket,"uid=");
		if (!pHead)
		{
			return FALSE;
		}
		CString strTempData(pHead);
		strTempData = ConverHttp2Html(strTempData);
		ConvertUtf8toGbk(strTempData,strTempData.GetLength(),strTempData);
		CStringArray arrTemp;
		PhoneMapSplitString(strTempData,arrTemp,'&');

		CString strUser,strPass,strIMEI;
		int arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			if ( arrTemp[i].Find("uid=") != -1)
			{
				strUser = PhoneMapGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("passwd=") != -1)
			{
				strPass = PhoneMapGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("deviceid=") != -1)
			{
				strIMEI = PhoneMapGetItemValue(arrTemp[i],'=');
			}
		}
		
		if (strUser.IsEmpty() || strPass.IsEmpty() || strIMEI.IsEmpty())
		{
			return FALSE;
		}

		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nimei=%s\r\nstate=登陆\r\n",strUser,strPass,strIMEI);
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_sg.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("phonemap_sg", "phonemap_sg", txt,pHttpSession->m_dceprocotol->m_httpMissObj);

	}

	if (nAction == 2)
	{
		CString strTempData(pHttpSession->m_Requestline.m_URI);
		strTempData = ConverHttp2Html(strTempData);
		ConvertUtf8toGbk(strTempData,strTempData.GetLength(),strTempData);
		CStringArray arrTemp;
		PhoneMapSplitString(strTempData,arrTemp,'&');
		
		int arrsize = arrTemp.GetSize();
		CString strAddr;
		for (int i = 0; i < arrsize; i++)
		{
			if ( arrTemp[i].Find("key=") != -1)
			{
				strAddr = PhoneMapGetItemValue(arrTemp[i],'=');
			}
		}
		
		if (strAddr.IsEmpty())
		{
			return FALSE;
		}
		
		CString txt;
		txt.Format("msg=%s\r\nstate=查询\r\n",strAddr);
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_sg.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("phonemap_sg", "phonemap_sg", txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}

	return FALSE;
}


int IsFeiLuKuaiHDMap(HttpSession *pHttpSession)
{
	return FALSE;
}

BOOL FeiLuKuaiHDMap(HttpSession *pHttpSession ,const char *pPacket)
{
	return FALSE;
}

int IsEDuShiMap(HttpSession *pHttpSession)
{
	if( ( pHttpSession->m_Requestline.m_Host.Find("mapi.edushi.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("REQ=L_S_FUZZYSEARCH") != -1) &&
		( pHttpSession->m_Requestline.m_URI.Find("sCityCode") != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("KeyWords") != -1 )
		)
	{
		return 1;
	}

/*
	if( ( pHttpSession->m_Requestline.m_Host.Find("mapi.edushi.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("REQ=L_S_FUZZYSEARCH") != -1) &&
		( pHttpSession->m_Requestline.m_URI.Find("sCityCode") != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("KeyWords") != -1 )
		)
	{
		return 1;
	}*/
	return FALSE;
}

BOOL EDuShiMap(HttpSession *pHttpSession ,const char *pPacket)
{
	CString strTempData(pHttpSession->m_Requestline.m_URI);
	strTempData = ConverHttp2Html(strTempData);
	ConvertUtf8toGbk(strTempData,strTempData.GetLength(),strTempData);
	
	CStringArray arrTemp;
	PhoneMapSplitString(strTempData,arrTemp,'&');
	
	int arrsize = arrTemp.GetSize();
	CString strIMEI,strTemp;
	for (int i = 0; i < arrsize; i++)
	{
		if ( arrTemp[i].Find("imei=") != -1)
		{
			strIMEI = PhoneMapGetItemValue(arrTemp[i],'=');
		}
		if ( arrTemp[i].Find("p=") != -1)
		{
			strTemp = PhoneMapGetItemValue(arrTemp[i],'=');
		}
	}
	
	if (strIMEI.IsEmpty() || strTemp.IsEmpty())
	{
		return FALSE;
	}

	strTemp.Replace("\"","");
	int posbegin = strTemp.Find("sKeyWords:");
	if (posbegin == -1)
	{
		return FALSE;
	}

	int posend = strTemp.Find(",",posbegin);
	if (posend == -1)
	{
		posend = strTemp.Find("}",posbegin);
		if (posend == -1)
		{
			return FALSE;
		}
		
	}
	
	int tmpoffset = strlen("sKeyWords:");

	CString strAddr = strTemp.Mid(posbegin + tmpoffset, posend - posbegin - tmpoffset);
	CString txt;
	txt.Format("imei=%s\r\nmsg=%s\r\nstate=查询\r\n",strIMEI,strAddr);
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = g_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_phonemap_eds.indexfile.txt",
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
		pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("phonemap_eds", "phonemap_eds", txt,pHttpSession->m_dceprocotol->m_httpMissObj);
	return TRUE;
}


int IsBaiDuMap(HttpSession *pHttpSession)
{
	if( ( pHttpSession->m_Requestline.m_Host.Find("client.map.baidu.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/?qt") != -1))
	{
		return 1;
	}
	//百度地图 第三方接口
// 	if (pHttpSession->m_Requestline.m_Host.Find("loc.map.baidu.com") != -1 &&
// 		pHttpSession->m_Requestline.m_URI.Find("/sdk.php") != -1)
// 	{
// 		return 1;
// 	}
	return 0;
}

BOOL BaiDuMap(HttpSession *pHttpSession ,const char *pPacket, int nAction)
{
    //string hehe(pPacket);
	//AfxMessageBox(hehe.c_str());

	CString strTempData(pHttpSession->m_Requestline.m_URI);
	strTempData = ConverHttp2Html(strTempData);
	ConvertUtf8toGbk(strTempData,strTempData.GetLength(),strTempData);

	CStringArray arrTemp;
	PhoneMapSplitString(strTempData,arrTemp,'&');
	
	int arrsize = arrTemp.GetSize();
	CString strIMEI,strTemp;
	for (int i = 0; i < arrsize; i++)
	{
		
		if ( arrTemp[i].Find("wd=") != -1)
		{
			strTemp = PhoneMapGetItemValue(arrTemp[i],'=');
			int pos = strTemp.Find("wd=");
			if (pos != -1)
			{
				strTemp = strTemp.Mid(pos + 3);
				strTemp = ConverHttp2Html(strTemp);
				ConvertUtf8toGbk(strTemp,strTemp.GetLength(),strTemp);
			}
		}
		
/*
		strTemp = PhoneMapGetItemValue(arrTemp[i],'=');
		strTemp = ConverHttp2Html(strTemp);
		ConvertUtf8toGbk(strTemp,strTemp.GetLength(),strTemp);
		printf("left : %s -- right : %s \n",arrTemp[i],strTemp);*/
	}
	
	if (strIMEI.IsEmpty() || strTemp.IsEmpty())
	{
		return FALSE;
	}
	return FALSE;
}