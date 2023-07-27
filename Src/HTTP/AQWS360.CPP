
#include "StdAfx.h"
#include "http.h"
#include "HttpSession.h"
#include "PublicFun.h"
#include "..\\DceMission.h"
#include "AQWS360.H"


CString AQWS360SplitProcess(CString &strData,CString &strProcessName,CString &strProcessDisp)
{
	int nPosBegin = 0;
	int nPosEnd = 0;
	int nLenBegin = 0;
	int nLenEnd = 0;\

	nPosBegin = strData.Find("<file_name><![CDATA[");
	nLenBegin = strlen("<file_name><![CDATA[");
	nPosEnd = strData.Find("]]></file_name>");
	nLenEnd = strlen("]]></file_name>");
	if ( (-1 != nPosBegin) && (-1 != nPosEnd))
	{
		strProcessName = strData.Mid(nPosBegin + nLenBegin, nPosEnd - nPosBegin - nLenBegin);
		strData = strData.Mid(nPosEnd + nLenEnd);
		
		nPosBegin = strData.Find("<disp_name><![CDATA[");
		nLenBegin = strlen("<disp_name><![CDATA[");
		nPosEnd = strData.Find("]]></disp_name>");
		nLenEnd = strlen("]]></disp_name>");
		if ((-1 != nPosBegin) && (-1 != nPosEnd))
		{
			strProcessDisp = strData.Mid(nPosBegin + nLenBegin, nPosEnd - nPosBegin - nLenBegin);
			strData = strData.Mid(nPosEnd + nLenEnd);
			return strData;
		}
		return "";
	}
	return "";
}


BOOL IsAQWS360DevProcess(const char *pPacket)
{
	/*
	CString strData(pPacket);
	if ( (-1 != strData.Find("<file_name><![CDATA[") ) &&
		(-1 != strData.Find("]]></file_name>")) &&
		(-1 != strData.Find("<disp_name><![CDATA[")) &&
		(-1 != strData.Find("]]></disp_name>")))
	{
		return TRUE;
	}
	*/
	return FALSE;
}

BOOL AQWS360DevProcess(const char *pPacket,DATEHEADER& packh ,HttpProtocol* httpprotocl)
{
	CString strData(pPacket);

	CString strText("webname=");
	CString strProcessName;
	CString strProcessDisp;
	while(1)
	{
		if ("" != strData)
		{
			strProcessName = "";
			strProcessDisp = "";
			strData = AQWS360SplitProcess(strData,strProcessName,strProcessDisp);
			strText = strText + "进程名:" + strProcessName + "  进程描述:" + strProcessDisp + ";";
		}
		else
		{
			break;
		}
	}

	if ("" == strText)
	{
		return FALSE;
	}
	strText += "\r\n";
	HttpSession  httpsession(packh,g_httpFrom);
	httpsession.m_spyaddr = g_DDEPOS;
	httpsession.m_dceprocotol=httpprotocl;
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = g_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	
	httpsession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_aqws360.indexfile.txt",
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, httpsession.m_sip, httpsession.m_dip, rand());
		httpsession.WriteIndexFile("aqws360_proc", "aqws360_proc", strText, httpsession.m_dceprocotol->m_httpMissObj);


	return TRUE;
}