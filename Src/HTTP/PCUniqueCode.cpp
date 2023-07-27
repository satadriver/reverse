

#include "StdAfx.h"
#include "http.h"
#include "HttpSession.h"
#include "PCUniqueCode.h"


BOOL DealDevUC(HttpSession * pHttpSession ,const char *pPacket)
{
	/*
	if ( IsDevUC_YYYuYin(pHttpSession) )
	{
		return DevUC_YYYuyin(pHttpSession ,pPacket);
	}
	if ( IsDevUC_FengXing(pHttpSession))
	{
		return DevUC_FengXing(pHttpSession ,pPacket);
	}
	if ( IsDevUC_XunLei(pHttpSession))
	{
		return DevUC_XunLei(pHttpSession,pPacket);
	}
	if ( IsDevUC_BaoFeng(pHttpSession))
	{
		return DevUC_BaoFeng(pHttpSession,pPacket);
	}
	if ( IsDevUC_360AQWS(pHttpSession))
	{
		return DevUC_360AQWS(pHttpSession,pPacket);
	}
	*/
	return FALSE;
}

BOOL IsDevUC_YYYuYin(HttpSession * pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("ylog.hiido.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("yylogin") != -1 ) &&
		( pHttpSession->m_Requestline.m_URI.Find("mac=") != -1 ) )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL DevUC_YYYuyin(HttpSession * pHttpSession ,const char *pPacket)
{
	int npos = pHttpSession->m_Requestline.m_URI.Find("mac=");
	if (npos != -1)
	{
		//4 = strlen("mac=")
		//12 = strlen("AABBCCDDEEFF")
		CString strMac = pHttpSession->m_Requestline.m_URI.Mid(npos + 4,12);

		CString txt;
		txt.Format("webname=%s\r\n",strMac);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_Devuc_yy.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("devuc_yy", "devuc_yy", txt, pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}

BOOL IsDevUC_FengXing(HttpSession * pHttpSession)
{
	if( ( pHttpSession->m_Requestline.m_Host.Find("api2.funshion.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/api/popwind") != -1 ) &&
		( pHttpSession->m_Requestline.m_URI.Find("mac=") != -1 ) )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL DevUC_FengXing(HttpSession * pHttpSession ,const char *pPacket)
{
	int npos = pHttpSession->m_Requestline.m_URI.Find("mac=");
	if (npos != -1)
	{
		//4 = strlen("mac=")
		//12 = strlen("AABBCCDDEEFF")
		CString strMac = pHttpSession->m_Requestline.m_URI.Mid(npos + 4,12);
		
		CString txt;
		txt.Format("webname=%s\r\n",strMac);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_Devuc_fengxing.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("devuc_fengxing", "devuc_fengxing", txt, pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}

BOOL IsDevUC_XunLei(HttpSession * pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("kkpgv2.stat.kankan.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/?u=kk_download_reffer") != -1 ) &&
		( pHttpSession->m_Requestline.m_URI.Find("u4=") != -1 ) )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL DevUC_XunLei(HttpSession * pHttpSession ,const char *pPacket)
{
	int npos = pHttpSession->m_Requestline.m_URI.Find("u4=");
	if (npos != -1)
	{
		//3 = strlen("u4=")
		//16 = strlen("1122334455667788")
		CString strMac = pHttpSession->m_Requestline.m_URI.Mid(npos + 3,16);
		
		CString txt;
		txt.Format("webname=%s\r\n",strMac);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_Devuc_fengxing.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("devuc_xunlei", "devuc_xunlei", txt, pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}


BOOL IsDevUC_BaoFeng(HttpSession * pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("config.plugin.baofeng.net")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/plugin.xml") != -1 ) &&
		( pHttpSession->m_Requestline.m_URI.Find("uid=") != -1 ) )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL DevUC_BaoFeng(HttpSession * pHttpSession ,const char *pPacket)
{
	int npos = pHttpSession->m_Requestline.m_URI.Find("uid={");
	if (npos != -1)
	{
		//5 = strlen("u4={")
		//16 = strlen("9282DF5F-4EE3-DF01-FBEB-CB45FC5E2E76")
		CString strMac = pHttpSession->m_Requestline.m_URI.Mid(npos + 5,strlen("9282DF5F-4EE3-DF01-FBEB-CB45FC5E2E76"));
		
		CString txt;
		txt.Format("webname=%s\r\n",strMac);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_Devuc_fengxing.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("devuc_baofeng", "devuc_baofeng", txt, pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}

BOOL IsDevUC_360AQWS(HttpSession * pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("safe.matrix.uc.360.cn")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/index.php?method") != -1 ) &&
		( pHttpSession->m_Requestline.m_URI.Find("mid=") != -1 ) )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL DevUC_360AQWS(HttpSession * pHttpSession ,const char *pPacket)
{
	int npos = pHttpSession->m_Requestline.m_URI.Find("mid=");
	if (npos != -1)
	{
		//4 = strlen("u4={")
		//16 = strlen("9282DF5F-4EE3-DF01-FBEB-CB45FC5E2E76")
		CString strMac = pHttpSession->m_Requestline.m_URI.Mid(npos + 4,strlen("3220733d7d07fe726db2d901196293f9"));
		
		CString txt;
		txt.Format("webname=%s\r\n",strMac);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_devuc_aqws360.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("devuc_aqws360", "devuc_aqws360", txt, pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}

BOOL IsDevUC_QQ(HttpSession * pHttpSession)
{
	if( 
		( pHttpSession->m_Requestline.m_URI.Find("/assistant/sdklib/update.ini") != -1 ) &&
		( pHttpSession->m_Requestline.m_URI.Find("mkey=") != -1 ) )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL DevUC_QQ(HttpSession * pHttpSession ,const char *pPacket)
{
	int npos = pHttpSession->m_Requestline.m_URI.Find("mkey=");
	if (npos != -1)
	{
		//5 = strlen("mkey=")
		CString strMac = pHttpSession->m_Requestline.m_URI.Mid(npos + 5,strlen("532916eb8fafce8e"));
		
		CString txt;
		txt.Format("webname=%s\r\n",strMac);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_devuc_qq.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, 
			pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("devuc_qq", "devuc_qq", txt, pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}
