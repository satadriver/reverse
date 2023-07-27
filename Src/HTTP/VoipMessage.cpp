#pragma once
#include "StdAfx.h"
#include "..\src\http\http.h"
#include "..\src\http\HttpSession.h"		//..\是上一层目录   .\是当前目录,两个都对
#include "..\src\http\VoipUrl.h"
#include "VoipMessage.h"
#define ITEMLENTH 64


BOOL IsVoipALITONGMessage(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("sms.joycent.com")  != -1 ) && 
		 ( pHttpSession->m_Requestline.m_URI.Find("/smgsendn") != -1 )   )	//不完全的uri,应该是版本名称，忽略
	{
		return TRUE;
	}
	if(  ( pHttpSession->m_Requestline.m_Host.Find("a.atonghua.net")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/newinface/sendmsg.asp?") != -1 )   )	//不完全的uri,应该是版本名称，忽略
	{
		return VOIP_PHONE_MSG;
	}
	return FALSE;
}

BOOL VoipALITONGMessage( HttpSession *pHttpSession,const char *pPacket,int Value)
{
	char  CallerPhone[32];
	char  CalleePhone[32];
	char  CallerID[32];
	char  CallerPSW[32];
	char  MessageContent[512];	//单条短信最长70个汉字，最多140（GB2312) - 280(UTF-8)个字节
	
	char * HeadPtr;
	char * EndPtr;
	
	memset(CallerPhone,0,32);
	memset(CalleePhone,0,32);
	memset(CallerID,0,32);
	memset(CallerPSW,0,32);
	memset(MessageContent,0,512);
	
	HeadPtr = strstr(pPacket, "username=");
	if (HeadPtr)
	{
		HeadPtr += strlen( "username=");
		if(EndPtr = strstr(HeadPtr, "pwd=") )
		{
			EndPtr -- ;
			memmove(CallerID, HeadPtr, EndPtr - HeadPtr);
		}
	}
	
	if (Value == VOIP_PHONE_MSG)
	{
		HeadPtr = strstr(HeadPtr,"receive=");
		if (HeadPtr)
		{
			HeadPtr += strlen("receive=");
			EndPtr = strstr(HeadPtr,"msg=");
			if (EndPtr)
			{
				EndPtr -- ;
				memmove(CalleePhone, HeadPtr, EndPtr - HeadPtr);			
			}
		}
	}
	else
	{	
		HeadPtr = strstr(HeadPtr,"number=");
		if ( HeadPtr )
		{
			HeadPtr += strlen( "number=");
			if(EndPtr = strstr(HeadPtr, "msg=") )
			{
				EndPtr -- ;
				memmove(CalleePhone, HeadPtr, EndPtr - HeadPtr);
			}
		}
	}
	
	HeadPtr = strstr(HeadPtr,"msg=");
	if ( HeadPtr )
	{
		HeadPtr += strlen( "msg=");
		if( EndPtr = strstr(HeadPtr, "Submit=ok") )
		{
			EndPtr -- ;
			memmove(MessageContent, HeadPtr, EndPtr - HeadPtr);
			MessageContent[EndPtr - HeadPtr] = 0;
			Utf8ToGB2312( DecodeUrl(MessageContent) );
		}
	}
	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=短信\r\ntype=voip_msg\r\n",
			CallerID,CallerPSW,CallerPhone,CalleePhone,MessageContent);
				
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_alcall.indexfile.txt", 
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("media_voip_alcall","media_voip_alcall",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
	
	return true;
}

//注意URL,URI等包含的字符串内容，URL被主程序组装成了诸如 "http://sms.joycent.com/smgsendn2010.asp"的完整域名结构
//URI则只是POST /smgsendn2010.asp HTTP/1.1中
//从POST到HTTP/1.1的一段从空格后面包括反斜杠在内开始的一段字符串，例如/smgsendn2010.asp
BOOL IsVoipZHCALLMessage(HttpSession * pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("sms.tisoon.com")  != -1 ) && 
		 ( pHttpSession->m_Requestline.m_URI.Find("/smgsendn.asp") != -1 )   )
	{
		return TRUE;
	}
	
	return FALSE;
	
}

//中华通经过UTF-8解码后是GB2312编码，而其他几个程序经UTF-8解码后是UNICODE编码
//总是单包处理？？？？？
BOOL VoipZHCALLMessage( HttpSession * pHttpSession,const char *pPacket  )
{
	char  CallerPhone[32];
	char  CalleePhone[32];
	char  CallerID[32];
	char  CallerPSW[32];
	char  MessageContent[512];	//单条短信最长70个汉字，最多140（GB2312) - 280(UTF-8)个字节
	
	char * HeadPtr;
	char * EndPtr;
	
	memset(CallerPhone,0,32);
	memset(CalleePhone,0,32);
	memset(CallerID,0,32);
	memset(CallerPSW,0,32);
	memset(MessageContent,0,512);
	
	//单包中HTTPSESSION中的字段无意义,HttpCommInfo中的字段总有意义
	//单包中的'&",回车,换行等字符没有没替换
	
	HeadPtr = strstr(pPacket, "username=");
	if (HeadPtr)
	{
		HeadPtr += strlen( "username=");
		if(EndPtr = strstr(HeadPtr, "pwd=") )
		{
			EndPtr -- ;
			memmove(CallerID, HeadPtr, EndPtr - HeadPtr);
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	
	
	//中华通可能群发，为了便面内存越界的风险这里只取一个电话号码
	HeadPtr = strstr(HeadPtr,"number=");
	if ( HeadPtr )
	{
		HeadPtr += strlen( "number=");
		memmove(CalleePhone, HeadPtr, 11);
	}
	else
	{
		return FALSE;
	}
	
	HeadPtr = strstr(HeadPtr,"msg=");
	if ( HeadPtr )
	{
		HeadPtr += strlen( "msg=");
		if(EndPtr = strstr(HeadPtr, "Submit=ok") )
		{
			EndPtr --;
			memmove(MessageContent, HeadPtr, EndPtr - HeadPtr);
			MessageContent[EndPtr - HeadPtr] = 0;
			DecodeUrl(MessageContent) ;	
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=短信\r\ntype=voip_msg\r\n",
			CallerID,CallerPSW,CallerPhone,CalleePhone,MessageContent);
				
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_zhcall.indexfile.txt",
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("media_voip_zhcall","media_voip_zhcall",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
	
//	printf("发送者电话号码:%s\r\n发送者账号:%s\r\n内容:%s\r\n",CallerID,CalleePhone,MessageContent);
	return TRUE;
}


BOOL IsVoip97CALLMessage(HttpSession * pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("www.97call.com")  != -1 ) && 
		 ( pHttpSession->m_Requestline.m_URI.Find("/usercenter/UCAjaxProcessor.jsp") != -1 )   )
	{
		return TRUE;
	}
	return FALSE;

}


//下面两个模块在多包中处理？？？？？
//Packet 是数据开始地址，不是包头地址
BOOL Voip97CALLMessage(  HttpSession * pHttpSession,const char *pPacket )
{
	char  CallerPhone[32];
	char  CalleePhone[32];
	char  CallerID[32];
	char  CallerPSW[32];
	char  MessageContent[512];	//单条短信最长70个汉字，最多140（GB2312) - 280(UTF-8)个字节
	
	char * HeadPtr;
	char * EndPtr;
	
	memset(CallerPhone,0,32);
	memset(CalleePhone,0,32);
	memset(CallerID,0,32);
	memset(CallerPSW,0,32);
	memset(MessageContent,0,512);
	
	HeadPtr = strstr(pPacket, "action=SMSsend");	//不要试图查找&，已经被改为了换行符
	if ( HeadPtr )
	{
		HeadPtr += strlen("action=SMSsend");
		HeadPtr = strstr(HeadPtr,"args=");
		if (HeadPtr)
		{
			HeadPtr += strlen("args=");
			if(EndPtr = strchr(HeadPtr, ',') )
			{
				memmove(CalleePhone, HeadPtr, EndPtr - HeadPtr);
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	
	HeadPtr = EndPtr + strlen(",");
	EndPtr = (char *)pPacket + pHttpSession->m_datalen;
	memmove(MessageContent, HeadPtr, EndPtr - HeadPtr);
	MessageContent[EndPtr - HeadPtr] = 0;
	Utf8ToGB2312( DecodeUrl(MessageContent) );
	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=短信\r\ntype=voip_msg\r\n",
			CallerID,CallerPSW,CallerPhone,CalleePhone,MessageContent);
				
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_97call.indexfile.txt",
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("media_voip_97call","media_voip_97call",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
	
//	printf("发送者电话号码:%s\r\n发送者账号:%s\r\n内容:%s\r\n",CallerID,CalleePhone,MessageContent);
	return TRUE;
}


BOOL IsVoipDYTMessage(HttpSession * pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("www.51dyt.com")  != -1 ) && 
		 ( pHttpSession->m_Requestline.m_URI.Find("/php/main/sms.php") != -1 )   )
	{
		return TRUE;
	}
	return FALSE;

}


//弄清楚如何处理HTTP包，PACKET中放着数据，HTTPSESSION或者其他结构中存放该包的其他控制信息
BOOL VoipDYTMessage( HttpSession * pHttpSession,const char *pPacket )
{
	char  CallerPhone[32];
	char  CalleePhone[32];
	char  CallerID[32];
	char  CallerPSW[32];
	char  MessageContent[512];	//单条短信最长70个汉字，最多140（GB2312) - 280(UTF-8)个字节

	char * HeadPtr;
	char * EndPtr;

	memset(CallerPhone,0,32);
	memset(CalleePhone,0,32);
	memset(CallerID,0,32);
	memset(CallerPSW,0,32);
	memset(MessageContent,0,512);
	
	HeadPtr = strstr(pPacket, "destMobile=");
	if ( HeadPtr )
	{
		HeadPtr += strlen( "destMobile=");
		if(EndPtr = strstr(HeadPtr,"content=") )
		{
			EndPtr -- ;
			memmove(CallerID, HeadPtr, 11);
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	HeadPtr = strstr(HeadPtr,"content=");
	if(HeadPtr)
	{
		HeadPtr += strlen( "content=");
		EndPtr = (char *)pPacket + pHttpSession->m_datalen;
		memmove(MessageContent, HeadPtr, EndPtr - HeadPtr);
		MessageContent[EndPtr - HeadPtr] = 0;
		Utf8ToGB2312( DecodeUrl(MessageContent) );
	}
	else
	{
		return FALSE;
	}

	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=短信\r\ntype=voip_msg\r\n",
			CallerID,CallerPSW,CallerPhone,CalleePhone,MessageContent);
				
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_dyt.indexfile.txt",
					tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
					now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("media_voip_dyt","media_voip_dyt",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
	return TRUE;
}












BOOL DealVoipCall(HttpSession *pHttpSession ,const char *pPacket)
{
	int Result = 0;

	Result = IsVoipYMCALLCall(pHttpSession);
	if (Result)
	{
		return VoipYMCALLCall(pHttpSession,pPacket,Result);
	}

	Result = IsVoip97CALLCall(pHttpSession);
	if (Result)
	{
		return Voip97CALLCall(pHttpSession,pPacket,Result);
	}
	
	Result = IsVoipDACALLCall(pHttpSession);
	if (Result)
	{
		return VoipDACALLCall(pHttpSession,pPacket,Result);
	}
	
	Result = IsVoipKUAIBOCall(pHttpSession);
	if (Result)
	{
		return VoipKUAIBOCall(pHttpSession,pPacket,Result);
	}

	Result = IsVoipKUBAOCall(pHttpSession);
	if (Result)
	{
		return VoipKUBAOCall(pHttpSession,pPacket,Result);
	}
	
	Result = IsVoipALICALLCall(pHttpSession);
	if (Result)
	{
		return VoipALICALLCall(pHttpSession,pPacket,Result);
	}
	
	Result = IsZHCALLCall(pHttpSession);
	if (Result)
	{
		return ZHCALLCall(pHttpSession,pPacket,Result);
	}

	Result = IsVoipDYTCall(pHttpSession);
	if (Result)
	{
		return VoipDYTCall(pHttpSession,pPacket,Result);
	}

	Result = IsVoipYOUXINCall(pHttpSession);
	if (Result)
	{
		return VoipYOUXINCall(pHttpSession,pPacket,Result);
	}	
	
	Result = IsVoip66CALLCall(pHttpSession);
	if (Result)
	{
		return Voip66CALLCall(pHttpSession,pPacket,Result);
	}
	Result = IsVoipKcCall(pHttpSession);
	if (Result)
	{
		return VoipKcCall(pHttpSession,pPacket,Result);
	}	
	Result = IsVoipFEIYINCall(pHttpSession);
	if (Result)
	{
		return VoipFEIYUCall(pHttpSession,pPacket,Result);
	}

	Result = IsVoipFEIYUCall(pHttpSession);
	if (Result)
	{
		return VoipFEIYINCall(pHttpSession,pPacket,Result);
	}

	Result = IsVoipHHCallCall(pHttpSession);
	if (Result)
	{
		return VoipHHCallCall(pHttpSession,pPacket,Result);
	}

	Result = IsVoipAiLiaoCall(pHttpSession);
	if (Result)
	{
		return VoipAiLiaoCall(pHttpSession,pPacket,Result);
	}

	Result = IsVoipAiLiaoCall(pHttpSession);
	if (Result)
	{
		return VoipAiLiaoCall(pHttpSession,pPacket,Result);
	}
	Result = IsVoipTTCall(pHttpSession);
	if (Result)
	{
		return VoipTTCall(pHttpSession,pPacket,Result);
	}

	/*
	Result = IsVoipTalkBoxCall(pHttpSession);
	if (Result)
	{
		return VoipTalkBoxCall(pHttpSession,pPacket,Result);
	}
*/
	return FALSE;
}



int IsVoipTTCall(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("ttcfg.app.tt.gameabc.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/ttvoicecfg/update/") != -1 )  )
	{
		return VOIP_CALL;
	}
	return 0;
	
}

int VoipTTCall( HttpSession *pHttpSession,const char* pPacket,int Value)
{
	if (Value == VOIP_CALL)
	{
		CString Uid = pPacket;
		int Offset = Uid.Find("\"uid\":\"");
		if (Offset == -1)
		{
			return FALSE;
		}
		Uid = Uid.Mid(Offset + strlen("\"uid\":\""));

		Offset = Uid.Find("\",");
		if (Offset == -1)
		{
			return FALSE;
		}
		Uid = Uid.Left(Offset);
		
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",Uid,"",Uid,"","","通话");
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_tt.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_tt","media_voip_tt",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;	
	}
	return FALSE;
}









//YMCALL 0X01
int IsVoipYMCALLCall(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("www.ymcall.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/get_srv_list.php?username=") != -1 )  )
	{
		return VOIP_LOGIN;
	}
	if(  ( pHttpSession->m_Requestline.m_Host.Find("www.ymcall.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/get_balance.php?") != -1 )  )
	{
		return VOIP_PHONE_CALL;
	}
	return 0;
	
}

int VoipYMCALLCall( HttpSession *pHttpSession,const char* pPacket,int Value)
{
	char * ItemHead;
	char * ItemEnd;
	unsigned char	CallerPhone[ITEMLENTH]	=	{0};
	unsigned char	CalleePhone[ITEMLENTH]	=	{0};
	unsigned char	CallerID[ITEMLENTH]		=	{0};
	unsigned char	CallerPSW[ITEMLENTH]	=	{0};
	char *			LogIn			= "登陆";
	char *			Dial			= "通话";

	if (Value == VOIP_LOGIN)
	{
		ItemHead = strstr(pPacket,"username=");
		if (ItemHead == 0)
		{
			return FALSE;
		}

		ItemHead += strlen("username=");
		ItemEnd = strstr(ItemHead,"HTTP");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		ItemEnd -- ;

		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);
		memmove(CallerPhone, ItemHead , ItemEnd - ItemHead);
		memmove(CallerID , ItemHead, ItemEnd - ItemHead);
		
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
			CallerID,CallerPSW,CallerPhone,CalleePhone,"",LogIn);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_ymcall.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_ymcall","media_voip_ymcall",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;	
	}

	if (Value == VOIP_PHONE_CALL)
	{
		ItemHead = strstr(pPacket,"username=");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		
		ItemHead += strlen("username=");
		ItemEnd = strstr(ItemHead,"password=");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		ItemEnd -- ;

		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);
		memmove(CallerID , ItemHead, ItemEnd - ItemHead);
		memmove(CallerPhone, ItemHead , ItemEnd - ItemHead);
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
			CallerID,CallerPSW,CallerPhone,CalleePhone,"",Dial);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_ymcall.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_ymcall","media_voip_ymcall",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
	
		return TRUE;
	}
	return FALSE;
}


//97CALL
int IsVoip97CALLCall(HttpSession *pHttpSession)
{
	if(  (pHttpSession->m_Requestline.m_Host.Find("www.97call.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/UserloginFromSoftClient.jsp?username=") != -1 )  )
	{
		return VOIP_LOGIN;
	}
	return 0;
	
}

int Voip97CALLCall( HttpSession *pHttpSession,const char* pPacket,int Value)
{
	char * ItemHead;
	char * ItemEnd;
	unsigned char	CallerPhone[ITEMLENTH]	=	{0};
	unsigned char	CalleePhone[ITEMLENTH]	=	{0};
	unsigned char	CallerID[ITEMLENTH]		=	{0};
	unsigned char	CallerPSW[ITEMLENTH]	=	{0};

	ItemHead = strstr (pPacket,"username=");
	if(ItemHead == 0)
	{
		return FALSE;
	}
	ItemHead += strlen("username=");

	ItemEnd = strstr(ItemHead, "password=");
	if (!ItemEnd)
	{
		return FALSE;
	}
	ItemEnd --;

	if (ItemEnd - ItemHead >= ITEMLENTH)
	{
		return FALSE;
	}

	memset(CallerID,0,ITEMLENTH);
	memset(CallerPhone,0,ITEMLENTH);
	memset(CallerPSW,0,ITEMLENTH);
	memset(CalleePhone,0,ITEMLENTH);
	memmove(CallerID,ItemHead ,ItemEnd - ItemHead);
	memmove(CallerPhone,ItemHead ,ItemEnd - ItemHead);
	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
			CallerID,CallerPSW,CallerPhone,CalleePhone,"","登陆");
				
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_97call.indexfile.txt", 
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("media_voip_97call","media_voip_97call",txt,
		pHttpSession->m_dceprocotol->m_httpMissObj);
	return TRUE;
}



//DACALL
int IsVoipDACALLCall(HttpSession *pHttpSessiono)
{
	if(  ( pHttpSessiono->m_Requestline.m_Host.Find("www.callda.com")  != -1 ) && 
		(  pHttpSessiono->m_Requestline.m_URI.Find("/interface/dynamicSip.jsp?phoneNumber=") != -1 )  )
	{
		return VOIP_LOGIN;
	}
	if(  (  pHttpSessiono->m_Requestline.m_Host.Find("www.callda.com")  != -1 ) && 
		(  pHttpSessiono->m_Requestline.m_URI.Find("/interface/balance.jsp?name=") != -1 )  )
	{
		return VOIP_CALL;
	}
	return FALSE;
}

int VoipDACALLCall(HttpSession *pHttpSession,const char* pPacket,int Value)
{
	char * ItemHead;
	char * ItemEnd;
	unsigned char	CallerPhone[ITEMLENTH] =	{0};
	unsigned char	CalleePhone[ITEMLENTH]	=	{0};
	unsigned char	CallerID[ITEMLENTH]	=		{0};
	unsigned char	CallerPSW[ITEMLENTH]	=	{0};
	char *			LogIn			= "登陆";
	char *			Dial			= "通话";

	if(Value == VOIP_LOGIN) 
	{
		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);

		ItemHead = strstr(pPacket,"phoneNumber=");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		ItemHead += strlen("phoneNumber=");

		ItemEnd = strstr(ItemHead,"pwd=");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		ItemEnd --;
		memmove(CallerPhone, ItemHead, ItemEnd - ItemHead);
		
		ItemHead = ItemEnd + 1;
		ItemHead += strlen("pwd=");
		ItemEnd = strstr(ItemHead,"HTTP/1.1");
		if(ItemEnd == 0)
		{
			strcpy((char*)CallerPSW, ItemHead);
		}
		else
		{
			ItemEnd --;
			memmove(CallerPSW, ItemHead, ItemEnd - ItemHead);
		}
		
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
		CallerID,CallerPSW,CallerPhone,CalleePhone,"",LogIn);		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_97call.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_97call","media_voip_97call",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	if(Value == VOIP_CALL)
	{
		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);

		ItemHead = strstr(pPacket,"name=");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		ItemHead += strlen("name=");

		ItemEnd = strstr(ItemHead,"pwd=");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		ItemEnd --;
		memmove(CallerPhone, ItemHead, ItemEnd - ItemHead);

		ItemHead = ItemEnd + 1;
		ItemHead += strlen("pwd=");
		ItemEnd = strstr(ItemHead,"HTTP/1.1");
		if(ItemEnd == 0)
		{
			strcpy((char*)CallerPSW, ItemHead);
		}
		else
		{
			ItemEnd --;
			memmove(CallerPSW, ItemHead, ItemEnd - ItemHead);
		}
				
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
			CallerID,CallerPSW,CallerPhone,CalleePhone,"",Dial);				
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_97call.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_97call","media_voip_97call",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}


int IsVoipKUAIBOCall(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("api.kbcall.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/web/api/login.php?") != -1 )   )
	{
		return VOIP_LOGIN;
	}
	if(  ( pHttpSession->m_Requestline.m_Host.Find("api.kbvoice.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/web/cs/notice.php?") != -1 )   )
	{
		return VOIP_CALL;
	}
	return 0;
}

BOOL VoipKUAIBOCall( HttpSession *pHttpSession,const char* pPacket,int Value)
{
	char * ItemHead;
	char * ItemEnd;
	unsigned char	CallerPhone[ITEMLENTH] =	{0};
	unsigned char	CalleePhone[ITEMLENTH]	=	{0};
	unsigned char	CallerID[ITEMLENTH]	=		{0};
	unsigned char	CallerPSW[ITEMLENTH]	=	{0};
	char *			LogIn			= "登陆";
	char *			Dial			= "通话";

	memset(CallerID,0,ITEMLENTH);
	memset(CallerPhone,0,ITEMLENTH);
	memset(CallerPSW,0,ITEMLENTH);
	memset(CalleePhone,0,ITEMLENTH);

	char * State = 0;
	if (Value == VOIP_LOGIN)
	{
		if( (ItemHead = strstr(pPacket,"username=") )  &&  (ItemEnd = strstr(ItemHead,"pwd=") ) )
		{
			ItemHead += strlen("username=");
			ItemEnd -- ;
			memmove(CallerID, ItemHead , ItemEnd - ItemHead);

			State = "login";
		}
	}
	else if (Value == VOIP_PHONE_CALL)
	{
		if( (ItemHead = strstr(pPacket,"username=") )  &&  (ItemEnd = strstr(ItemHead,"key=") ) )
		{
			ItemHead += strlen("username=");
			ItemEnd -- ;
			memmove(CallerID, ItemHead , ItemEnd - ItemHead);

			State = "call";
		}
	}
	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
		CallerID,CallerPSW,CallerPhone,CalleePhone,"",State);		
				
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_kuaibo.indexfile.txt", 
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("media_voip_kuaibo","media_voip_kuaibo",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
	return TRUE;	
}


int IsVoipKUBAOCall(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("imservice.kubao.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/addressboxservice.asmx/AddUserCallNumber") != -1 )  )
	{
		return VOIP_CALL;
	}
	return FALSE;
	
}

BOOL VoipKUBAOCall( HttpSession *pHttpSession,const char* pPacket,int Value)
{

	char * ItemHead;
	char * ItemEnd;
	unsigned char	CallerPhone[ITEMLENTH] =	{0};
	unsigned char	CalleePhone[ITEMLENTH]	=	{0};
	unsigned char	CallerID[ITEMLENTH]	=		{0};
	unsigned char	CallerPSW[ITEMLENTH]	=	{0};
	char *			LogIn			= "登陆";
	char *			Dial			= "通话";

	if( (ItemHead = strstr(pPacket,"AssID=") )&&(ItemEnd = strstr(ItemHead,"TargetPhone=") )&&(Value == VOIP_CALL ))
	{
		ItemHead += strlen("AssID=");
		ItemEnd --;
		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);
		memmove(CallerID, ItemHead , ItemEnd - ItemHead);
	}
	else
	{
		return FALSE;
	}
	
	if( (ItemHead = strstr(ItemEnd,"TargetPhone=") )  &&  (ItemEnd = strstr(ItemHead,"VKey=") ) )
	{
		ItemHead += strlen("TargetPhone=");
		ItemEnd --;
		memmove(CallerPhone, ItemHead , ItemEnd - ItemHead);
	}
	else
	{
		return FALSE;
	}
	
	CString txt;
	txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
		CallerID,CallerPSW,CallerPhone,CalleePhone,"",Dial);		
				
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_kubao.indexfile.txt", 
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
	pHttpSession->WriteIndexFile("media_voip_kubao","media_voip_kubao",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
	return TRUE;	
}

int IsVoipALICALLCall(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("sms.joycent.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/cs/getvalue.asp?") != -1 )   )
	{
		return VOIP_LOGIN;
	}
	 if(  ( pHttpSession->m_Requestline.m_Host.Find("www.tisoon.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/cs/ad_new.asp?") != -1 )   )
	{
		return VOIP_LOGIN;
	}
	 if(  ( pHttpSession->m_Requestline.m_Host.Find("www.tisoon.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/NewInface/Zb/pc_logincheckm.asp?") != -1 )   )
	{
		return VOIP_LOGIN;
	}
	if(  ( pHttpSession->m_Requestline.m_Host.Find("sms.joycent.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/smgqueryb.asp?") != -1 )   )
	{
		return VOIP_CALL;
	}	
	if(  ( pHttpSession->m_Requestline.m_Host.Find("a.atonghua.net")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/newinface/Zb/getvaluem.asp?") != -1 )   )
	{
		return VOIP_PHONE_CALL;
	}

	return FALSE;
	
}

BOOL VoipALICALLCall(HttpSession *pHttpSession,const char* pPacket,int Value)
{

	char * ItemHead;
	char * ItemEnd;
	unsigned char	CallerPhone[ITEMLENTH] =	{0};
	unsigned char	CalleePhone[ITEMLENTH]	=	{0};
	unsigned char	CallerID[ITEMLENTH]	=		{0};
	unsigned char	CallerPSW[ITEMLENTH]	=	{0};
	char *			LogIn			= "登陆";
	char *			Dial			= "通话";
	if(Value == VOIP_LOGIN)
	{
		ItemHead = strstr(pPacket ,"username=");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		ItemHead += strlen("username=");

		ItemEnd = strstr(ItemHead , "pwd=");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		ItemEnd --;

		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);
		memmove(CallerID, ItemHead,ItemEnd - ItemHead);
		memmove(CalleePhone, ItemHead,ItemEnd - ItemHead);
		
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
		CallerID,CallerPSW,CallerPhone,CalleePhone,"",LogIn);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_alcall.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_alcall","media_voip_alcall",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	
	if( Value == VOIP_CALL) 
	{
		ItemHead = strstr(pPacket ,"username=");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		ItemHead += strlen("username=");

		ItemEnd = strstr(ItemHead , "time=");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		ItemEnd --;

		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);
		memmove(CallerID, ItemHead,ItemEnd - ItemHead);
		memmove(CallerPhone, ItemHead,ItemEnd - ItemHead);
	
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
		CallerID,CallerPSW,CallerPhone,CalleePhone,"",Dial);
					
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_alcall.indexfile.txt", 
						tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
						now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_alcall","media_voip_alcall",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	if( Value == VOIP_PHONE_CALL) 
	{
		ItemHead = strstr(pPacket ,"username=");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		ItemHead += strlen("username=");
		
		ItemEnd = strstr(ItemHead , "pwd=");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		ItemEnd --;
		
		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);
		memmove(CallerID, ItemHead,ItemEnd - ItemHead);
		memmove(CallerPhone, ItemHead,ItemEnd - ItemHead);
		
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
			CallerID,CallerPSW,CallerPhone,CalleePhone,"",Dial);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_alcall.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_alcall","media_voip_alcall",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}

	return FALSE;
}

int IsZHCALLCall(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("www.tisoon.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/cs/getvalue.asp?username=") != -1 ) )
	{
		return VOIP_LOGIN;
	}
	 if(  ( pHttpSession->m_Requestline.m_Host.Find("www.tisoon.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/cs/notification.asp?username=") != -1 )  )
	{
		return VOIP_LOGIN;
	}
	 if(  ( pHttpSession->m_Requestline.m_Host.Find("sms.tisoon.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/smgqueryb.asp?username=") != -1 ) )
	{
		return VOIP_CALL;
	}
	 if(  ( pHttpSession->m_Requestline.m_Host.Find("mbx.tisoon.com")  != -1 ) && 
		 ( pHttpSession->m_Requestline.m_URI.Find("/newinface/") != -1 ) )
	 {
		 return VOIP_PHONE_CALL;
	}	

	return FALSE;

}
BOOL ZHCALLCall( HttpSession *pHttpSession,const char* pPacket,int Value)
{
	char * ItemHead;
	char * ItemEnd;
	unsigned char	CallerPhone[ITEMLENTH] =	{0};
	unsigned char	CalleePhone[ITEMLENTH]	=	{0};
	unsigned char	CallerID[ITEMLENTH]	=		{0};
	unsigned char	CallerPSW[ITEMLENTH]	=	{0};
	char *			LogIn			= "登陆";
	char *			Dial			= "通话";

	if(Value == VOIP_CALL)
	{
		ItemHead = strstr(pPacket ,"username=");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		ItemHead += strlen("username=");
		
		ItemEnd = strstr(ItemHead , "time=");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		ItemEnd --;
		
		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);
		memmove(CallerID, ItemHead,ItemEnd - ItemHead);
		memmove(CallerPhone, ItemHead,ItemEnd - ItemHead);
		
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=\r\nstate=%s\r\ntype=voip_call\r\n",
			CallerID,CallerPSW,CallerPhone,CalleePhone,Dial);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_alcall.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_alcall","media_voip_alcall",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}

	if(Value == VOIP_PHONE_CALL)
	{
		ItemHead = strstr(pPacket ,"username=");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		ItemHead += strlen("username=");
		
		ItemEnd = strstr(ItemHead , "pwd=");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		ItemEnd --;
		
		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);
		memmove(CallerID, ItemHead,ItemEnd - ItemHead);
		memmove(CallerPhone, ItemHead,ItemEnd - ItemHead);
		
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=\r\nstate=%s\r\ntype=voip_call\r\n",
			CallerID,CallerPSW,CallerPhone,CalleePhone,Dial);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_alcall.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_alcall","media_voip_alcall",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}


int IsVoipDYTCall(HttpSession * pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("www.51dyt.com")  != -1 ) && 
		(  pHttpSession->m_Requestline.m_URI.Find("/getserver.php?") != -1 )  )
	{
		return VOIP_LOGIN;
	}
	if(  ( pHttpSession->m_Requestline.m_Host.Find("www.51dyt.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/getmoney.php?") != -1 )  )
	{
		return VOIP_CALL;
	}

	return FALSE;
	
}

BOOL VoipDYTCall( HttpSession * pHttpSession,const char *pPacket,int Value)
{
	char * ItemHead;
	char * ItemEnd;
	unsigned char	CallerPhone[ITEMLENTH] =	{0};
	unsigned char	CalleePhone[ITEMLENTH]	=	{0};
	unsigned char	CallerID[ITEMLENTH]	=		{0};
	unsigned char	CallerPSW[ITEMLENTH]	=	{0};
	char *			LogIn			= "登陆";
	char *			Dial			= "通话";

	if(Value == VOIP_LOGIN) 
	{
		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);
		ItemHead = strstr(pPacket,"username=");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		ItemHead += strlen("username=");

		ItemEnd = strstr(ItemHead,"HTTP/1.1");
		if (ItemEnd == 0)
		{
			strcpy((char*)CallerID,ItemHead);
		}
		else
		{
			ItemEnd --;
			memmove(CallerID,ItemHead, ItemEnd - ItemHead);
		}

		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
		CallerID,CallerPSW,CallerPhone,CalleePhone,"",LogIn);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_dyt.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_dyt","media_voip_dyt",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	
	if(Value == VOIP_CALL) 
	{
		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);
		ItemHead = strstr(pPacket,"username=");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		ItemHead += strlen("username=");
		ItemEnd = strstr(ItemHead,"md5password=");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		ItemEnd --;

		memmove(CallerID,ItemHead, ItemEnd - ItemHead);
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
		CallerID,CallerPSW,CallerPhone,CalleePhone,"",Dial);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_dyt.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_dyt","media_voip_dyt",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}

//KC
int IsVoipKcCall(HttpSession * pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("www.keepc.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/resource/") != -1 )  )
	{
		return VOIP_LOGIN;
	}
	
	return FALSE;	
}


int VoipKcCall(HttpSession * pHttpSession,const char * pPacket,int Value)
{
	char * ItemHead;
	char * ItemEnd;
	unsigned char	CallerPhone[ITEMLENTH] =	{0};
	unsigned char	CalleePhone[ITEMLENTH]	=	{0};
	unsigned char	CallerID[ITEMLENTH]	=		{0};
	unsigned char	CallerPSW[ITEMLENTH]	=	{0};
	char *			LogIn			= "登陆";
	char *			Dial			= "通话";

	if(Value == VOIP_LOGIN) 
	{

		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);
		ItemHead = strstr(pPacket,"uid=");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		ItemHead += strlen("uid=");
		ItemEnd = strstr(ItemHead,"brandid=");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		ItemEnd --;
		memmove(CallerID,ItemHead, ItemEnd - ItemHead);

		ItemHead = strstr(ItemEnd,"account=");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		ItemHead += strlen("account=");
		ItemEnd = strstr(ItemHead,"accountType=");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		ItemEnd --;
		memmove(CallerPhone,ItemHead, ItemEnd - ItemHead);
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
			CallerID,CallerPSW,CallerPhone,CalleePhone,"",LogIn);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_kc.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_kc","media_voip_kc",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}



//手机飞音，电脑版的在IMSG中处理
int IsVoipFEIYINCall(HttpSession * pHttpSession)
{
	if( (pHttpSession->m_Requestline.m_Host.Find("omp.guoling.com") != -1) &&
		(pHttpSession->m_Requestline.m_URI.Find("/report.action") != -1) )
	{
		return VOIP_CALL;
	}
	return FALSE;
}


int VoipFEIYINCall(HttpSession * pHttpSession,const char * pPacket,int Value)
{
	char * ItemHead;
	char * ItemEnd;
	if (ItemHead = strstr(pPacket,"\"uid\":\""))
	{
		ItemHead += strlen("\"uid\":\"");
		ItemEnd = strchr(ItemHead,'"');
		if (ItemEnd == 0)
		{
			return FALSE;
		}

		CString CallerID = ItemHead;
		CallerID = CallerID.Left(ItemEnd - ItemHead);

		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
			CallerID,"","","","","呼叫");
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_kc.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_kc","media_voip_kc",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}




int IsVoipFEIYUCall(HttpSession * pHttpSession)
{
	if( (pHttpSession->m_Requestline.m_Host.Find("www.italk24.com") != -1) &&
		(pHttpSession->m_Requestline.m_URI.Find("/k!lo.action?") != -1) )
	{
		return VOIP_CALL;
	}
	return FALSE;
}


int VoipFEIYUCall(HttpSession * pHttpSession,const char * pPacket,int Value)
{
	char * ItemHead;
	char * ItemEnd;
	if (ItemHead = strstr(pPacket,"mobilPhone="))
	{
		ItemHead += strlen("mobilPhone=");
		ItemEnd = strstr(ItemHead,"oi=");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		
		ItemEnd --;
		CString CallerID = ItemHead;
		CallerID = CallerID.Left(ItemEnd - ItemHead);
		
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
			CallerID,"","","","","呼叫");
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_kc.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_kc","media_voip_kc",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}



int IsVoipTUXINCall(HttpSession * pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("www.tuxins.com")  != -1 ) && 
	   (  pHttpSession->m_Requestline.m_URI.Find("/client/pcfavour.htm?") != -1 )  )
	{
		return VOIP_LOGIN;
	}
	return FALSE;
}


int VoipTUXINCall(HttpSession *pHttpSession,const char *pPacket,int Value)
{
	char * ItemHead;
	char * ItemEnd;
	unsigned char	CallerPhone[ITEMLENTH] =	{0};
	unsigned char	CalleePhone[ITEMLENTH]	=	{0};
	unsigned char	CallerID[ITEMLENTH]	=		{0};
	unsigned char	CallerPSW[ITEMLENTH]	=	{0};
	char *			LogIn			= "登陆";
	char *			Dial			= "通话";
	if (Value == VOIP_LOGIN)
	{
		ItemHead = strstr(pPacket,"uid=");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		ItemHead += strlen("uid=");
		
		ItemEnd = strstr(ItemHead,"brandid=");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		ItemEnd --;
		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);
		memmove(CallerID, ItemHead , ItemEnd - ItemHead);


		ItemHead = strstr(ItemEnd,"account=");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		ItemHead += strlen("account=");
		
		ItemEnd = strstr(ItemHead,"accountType=");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		ItemEnd --;
		memmove(CallerPhone, ItemHead , ItemEnd - ItemHead);
		
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
			CallerID,CallerPSW,CallerPhone,CalleePhone,"",LogIn);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_yx.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_yx","media_voip_yx",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}



//可能别的程序已经提走，所以接收不到
int IsVoipYOUXINCall(HttpSession *pHttpSession)
{
	if(  ( pHttpSession->m_Requestline.m_Host.Find("uxin.keicei.com")  != -1 ) && 
		(  pHttpSession->m_Requestline.m_URI.Find("/login?account=") != -1 )  )
	{
		return VOIP_LOGIN;
	}

	if(  ( pHttpSession->m_Requestline.m_Host.Find("uxin.keicei.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/emodel_report?") != -1 )  )
	{
		return VOIP_CALL;
	}

	if(  ( pHttpSession->m_Requestline.m_Host.Find("conn.uxin.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/v2/emodel_report?") != -1 )  )
	{
		return VOIP_PHONE_CALL;
	}
	return FALSE;
}

BOOL VoipYOUXINCall(HttpSession *pHttpSession,const char* pPacket,int Value )
{
	char * ItemHead;
	char * ItemEnd;
	unsigned char	CallerPhone[ITEMLENTH] =	{0};
	unsigned char	CalleePhone[ITEMLENTH]	=	{0};
	unsigned char	CallerID[ITEMLENTH]	=		{0};
	unsigned char	CallerPSW[ITEMLENTH]	=	{0};
	char *			LogIn			= "登陆";
	char *			Dial			= "通话";
	if (Value == VOIP_LOGIN)
	{
		ItemHead = strstr(pPacket,"account=");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		ItemHead += strlen("account=");

		ItemEnd = strstr(ItemHead,"imei=");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		ItemEnd --;

		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);
		memmove(CallerID, ItemHead , ItemEnd - ItemHead);
		memmove(CallerPhone, ItemHead , ItemEnd - ItemHead);
			
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
		CallerID,CallerPSW,CallerPhone,CalleePhone,"",LogIn);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_yx.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_yx","media_voip_yx",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	if (Value == VOIP_CALL)
	{
		ItemHead = strstr(pPacket,"caller_no%22%3A%22");
		if (ItemHead == 0)		//注意4个字段的顺序不能乱，否则出错
		{
			return FALSE;
		}
		ItemHead += strlen("caller_no%22%3A%22");

		ItemEnd = strstr(ItemHead,"%22%2C%22caller_uid%22%3A%22");
		if(ItemEnd == 0)
		{
			return FALSE;
		}
		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);
		memmove(CallerPhone, ItemHead , ItemEnd - ItemHead);
			
		ItemHead = ItemEnd + strlen("%22%2C%22caller_uid%22%3A%22");
		ItemEnd = strstr(ItemHead,"%22%2C%22callee%22%3A%22");
		if(ItemEnd == 0)
		{
			return FALSE;
		}
		memset(CallerID,0,ITEMLENTH);
		memmove(CallerID, ItemHead , ItemEnd - ItemHead);

		ItemHead = ItemEnd + strlen("%22%2C%22callee%22%3A%22");
		ItemEnd = strstr(ItemHead,"%22%2C%22");
		if(ItemEnd == 0)
		{
			return FALSE;
		}
		memset(CalleePhone,0,ITEMLENTH);
		memmove(CalleePhone, ItemHead , ItemEnd - ItemHead);	
		
		CString txt;
				txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
		CallerID,CallerPSW,CallerPhone,CalleePhone,"",Dial);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_yx.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_yx","media_voip_yx",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}

	if (Value == VOIP_PHONE_CALL)
	{
		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);
		
		ItemHead = strstr(pPacket,"\"caller_uid\":\"");
		if (ItemHead == 0)	
		{
			return FALSE;
		}
		ItemHead += strlen("\"caller_uid\":\"");
		
		ItemEnd = strchr(ItemHead,'"');
		if(ItemEnd == 0)
		{
			return FALSE;
		}
		memmove(CallerID, ItemHead , ItemEnd - ItemHead);
		
		
		ItemHead = strstr(pPacket,"\"caller_no\":\"");
		if (ItemHead == 0)	
		{
			return FALSE;
		}
		ItemHead += strlen("\"caller_no\":\"");
		
		ItemEnd = strchr(ItemHead,'"');
		if(ItemEnd == 0)
		{
			return FALSE;
		}
		memmove(CallerPhone, ItemHead , ItemEnd - ItemHead);
		
		
		ItemHead = strstr(pPacket,"\"callee\":\"");
		if (ItemHead == 0)	
		{
			return FALSE;
		}
		ItemHead += strlen("\"callee\":\"");
		
		ItemEnd = strchr(ItemHead,'"');
		if(ItemEnd == 0)
		{
			return FALSE;
		}
		memmove(CalleePhone, ItemHead , ItemEnd - ItemHead);
		
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
			CallerID,CallerPSW,CallerPhone,CalleePhone,"",Dial);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_yx.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_yx","media_voip_yx",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	
	return FALSE;
}
int IsVoipRYBCall(HttpSession * pHttpSession)
{
	return 0;
}



int IsVoip66CALLCall(HttpSession *pHttpSession)
{
	if( ( pHttpSession->m_Requestline.m_Host.Find("new.66call.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("http://new.66call.com/cloud/login.aspx?") != -1 )  )
	{
		return VOIP_LOGIN;
	}
	 if( (pHttpSession->m_Requestline.m_Host.Find("new.66call.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("http://new.66call.com/cloud/GetCalledInfo.aspx?") != -1 )  )
	{
		return VOIP_CALL;
	}
	return FALSE;
}

BOOL Voip66CALLCall( HttpSession *pHttpSession,const char* pPacket,int Value )
{
	char * ItemHead;
	char * ItemEnd;
	unsigned char	CallerPhone[ITEMLENTH] =	{0};
	unsigned char	CalleePhone[ITEMLENTH]	=	{0};
	unsigned char	CallerID[ITEMLENTH]	=		{0};
	unsigned char	CallerPSW[ITEMLENTH]	=	{0};
	char *			LogIn			= "登陆";
	char *			Dial			= "通话";
	if(Value == VOIP_LOGIN) 
	{
		ItemHead = strstr(pPacket ,"acct=");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		ItemHead += strlen("acct=");

		ItemEnd = strstr(ItemHead,"pwd=");	//不要试图查找例如'&','\n','\r'等这样的字符，因为在多包中已经被改变
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		ItemEnd --;

		if (ItemEnd - ItemHead >= ITEMLENTH)
		{
			return FALSE;
		}

		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);
		memmove(CallerID,ItemHead , ItemEnd - ItemHead);
		memmove(CallerPhone,ItemHead , ItemEnd - ItemHead);
		
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
			CallerID,CallerPSW,CallerPhone,CalleePhone,"",LogIn);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_kc.indexfile.txt", 
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_kc","media_voip_kc",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	if(Value == VOIP_CALL) 
	{
		ItemHead = strstr(pPacket,"info=");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		ItemHead += strlen("info=");

		ItemEnd = strstr(ItemHead,"validate=");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		ItemEnd --;

		if (ItemEnd - ItemHead >= ITEMLENTH)
		{
			return FALSE;
		}

		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);
		memmove(CallerID,ItemHead , ItemEnd - ItemHead);
		memmove(CallerPhone,ItemHead , ItemEnd - ItemHead);
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
		CallerID,CallerPSW,CallerPhone,CalleePhone,"",Dial);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_kc.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		
		pHttpSession->WriteIndexFile("media_voip_kc","media_voip_kc",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}


int IsVoipYUNTONGCall(HttpSession *pHttpSession)
{
	return 0;
}

int IsVoipYUNHUCall(HttpSession *pHttpSession)
{
	return 0;
}

int IsVoipYUNHUACall(HttpSession *pHttpSession)
{
	return 0;
}

int IsVoipHHCallCall(HttpSession *pHttpSession)
{
	if( ( pHttpSession->m_Requestline.m_Host.Find("www.hhcall.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/index.php/Tp/Index/softlogin") != -1 )  )
	{
		return VOIP_LOGIN;
	}	
	if( (pHttpSession->m_Requestline.m_Host.Find("www.hhcall.com")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/index.php/Tp/Index/softgetmoney") != -1 )  )
	{
		return VOIP_CALL;
	}
	return FALSE;
}

BOOL VoipHHCallCall(HttpSession *pHttpSession,const char* pPacket,int Value)
{
	char * ItemHead;
	char * ItemEnd;
	unsigned char	CallerPhone[ITEMLENTH] =	{0};
	unsigned char	CalleePhone[ITEMLENTH]	=	{0};
	unsigned char	CallerID[ITEMLENTH]	=		{0};
	unsigned char	CallerPSW[ITEMLENTH]	=	{0};
	char *			LogIn			= "登陆";
	char *			Dial			= "通话";

	if(Value == VOIP_LOGIN) 
	{
		ItemHead = strstr(pHttpSession->m_Requestline.m_URI,"/username/");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		ItemHead += strlen("/username/");

		ItemEnd = strstr(ItemHead,"/pws/");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memset(CalleePhone,0,ITEMLENTH);
		memmove(CallerID, ItemHead ,ItemEnd - ItemHead);
		memmove(CallerPhone,ItemHead,ItemEnd - ItemHead);
		
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
		CallerID,CallerPSW,CallerPhone,CalleePhone,"",LogIn);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_hhcall.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_hhcall","media_voip_hhcall",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	
	if(Value == VOIP_CALL)
	{
		ItemHead = strstr(pHttpSession->m_Requestline.m_URI,"/username/");
		if (ItemHead == 0)
		{
			return FALSE;
		}
		ItemHead += strlen("/username/");
		
		ItemEnd = strstr(ItemHead,"/pws/");
		if (ItemEnd == 0)
		{
			return FALSE;
		}
		memset(CallerID,0,ITEMLENTH);
		memset(CallerPhone,0,ITEMLENTH);
		memset(CallerPSW,0,ITEMLENTH);
		memmove(CallerID, ItemHead ,ItemEnd - ItemHead);
		memmove(CallerPhone,ItemHead,ItemEnd - ItemHead);
		
		CString txt;
		txt.Format("user=%s\r\npass=%s\r\nfrom=%s\r\nto=%s\r\nmsg=%s\r\nstate=%s\r\ntype=voip_call\r\n",
		CallerID,CallerPSW,CallerPhone,CalleePhone,"",Dial);
		
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_hhcall.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_hhcall","media_voip_hhcall",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	return FALSE;
}

int IsVoipAiLiaoCall(HttpSession *pHttpSession)
{/*
	if( ( pHttpSession->m_Requestline.m_Host.Find("n.a.aliaotian.net")  != -1 ) && 
		( pHttpSession->m_Requestline.m_URI.Find("/sms/share_wxurl.php?username=") != -1 )  )
	{
		return VOIP_LOGIN;
	}*/
	return 0;
}

BOOL VoipAiLiaoCall(HttpSession *pHttpSession,const char *pPacket,int Value )
{
/*
	int nPosBegin = pHttpSession->m_Requestline.m_URI.Find("username=");

	int nLenBegin = strlen("username=");
	int nLenEnd = 11;
	if (-1 != nPosBegin)
	{
		CString strUser = pHttpSession->m_Requestline.m_URI.Mid(nPosBegin + nLenBegin, nLenEnd);

		CString txt;
		txt.Format("user=%s\r\npass=\r\nfrom=%s\r\nto=\r\nmsg=%s\r\nstate=登陆\r\ntype=voip_call\r\n",
		strUser,"","","","");
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = pHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		pHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_media_voip_AiLiao.indexfile.txt", 
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, pHttpSession->m_sip, pHttpSession->m_dip, rand());
		pHttpSession->WriteIndexFile("media_voip_m_ailiao","media_voip_m_ailiao",txt,pHttpSession->m_dceprocotol->m_httpMissObj);
		return TRUE;
	}
	*/
	return FALSE;
}


CString VOIPGetItemValue(const CString &str,char sep)
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

void VOIPSplitString(CString strSrc ,CStringArray &arrayDest,char sep)
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
