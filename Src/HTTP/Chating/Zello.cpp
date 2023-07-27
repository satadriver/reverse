
#include "StdAfx.h"
#include "Zello.h"

int Zello::IsZelloPacket(DATEHEADER & packh, const char * pPacket)
{
	if( (packh.m_type == TCPPacket) && (packh.m_dport == 443) )	
	{
		if(memcmp(pPacket, "LT\n:", strlen("LT\n")) ==0) 
		{ 
			if (strstr(pPacket,"logon_init"))
			{
				return 1;
			}
			
		}	
	}
	return 0;
}

int Zello::ProcessZelloPacket(DATEHEADER & packh, const char * pPacket,HttpProtocol * pHttpProtocol)
{
	StringManipulation ms;
	string data(pPacket);
	ms.Replace(data,"\n",",");
	ms.Replace(data,"\r",",");	
	ms.Replace(data,"\"","");
	ms.Replace(data,"{","");
	ms.Replace(data,"}","");
	ms.SplitStringToMap(data,',',':');
	string user = ms.GetValueFromMapByKey("un");
	string ver = ms.GetValueFromMapByKey("version");
	string os = ms.GetValueFromMapByKey("platform");
	string lang = ms.GetValueFromMapByKey("language");
	
	if (user.empty())
	{
		return 0;
	}
	HttpSession pHttpSession(packh, g_httpFrom);		
	pHttpSession.m_dceprocotol=pHttpProtocol;
	

	CString cstrText;
	cstrText.Format("user=%s\r\ntype=%s\r\nmsgtype=%s\r\nlanguage=%s\r\nstate=ÉÏÏß\r\n",
		user.c_str(),os.c_str(),ver.c_str(),lang.c_str());
	SYSTEMTIME stNow;
	GetLocalTime(&stNow);
	CString tmppath = pHttpSession.m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_software_running.indexfile.txt", 
		tmppath, stNow.wYear, stNow.wMonth, stNow.wDay, stNow.wHour,stNow.wMinute, stNow.wSecond, 
		pHttpSession.m_sip, pHttpSession.m_dip, rand());
	pHttpSession.WriteIndexFile("im_zello","im_zello",cstrText,pHttpSession.m_dceprocotol->m_httpMissObj);
	return TRUE;
	
}