#pragma once
#include "StdAfx.h"
#include "..\src\http\http.h"
#include "..\src\http\HttpSession.h"		
#include "QQhttpSession.h"
#include "VoipUrl.h"


int IsQQhttpSession(HttpSession * pcalssHttpSession)
{
	/*
	if(  (pcalssHttpSession->m_Requestline.m_Host.Find("hm.l.qq.com")  != -1 ) && 
		 (pcalssHttpSession->m_Requestline.m_URI.Find("/adshm_index?") != -1 )  )
	{
		return QQ_HTTP_LOGIN;
	}
	else if(  (pcalssHttpSession->m_Requestline.m_Host.Find("cgi.qqweb.qq.com")  != -1 ) && 
			  (pcalssHttpSession->m_Requestline.m_URI.Find("/pc/getzip?") != -1 )  )
	{
		return QQ_HTTP_LOGOUT;
	}
	else if(  (pcalssHttpSession->m_Requestline.m_useragent.Compare("QQClient") == 0 ) &&		
	(pcalssHttpSession->m_Requestline.m_URI.Find("/?ver=") != -1 )  )
	{
		return QQ_HTTP_BUDDY_TRANSFILE;
	}
	else if(  (pcalssHttpSession->m_Requestline.m_Host.Find("client.show.qq.com")  != -1 ) && 
		(pcalssHttpSession->m_Requestline.m_URI.Find("/cgi-bin/qqshow_user_props_info") != -1 )  )
	{
		return QQ_HTTP_BUDDY_MSG;
	}
	else if(  (pcalssHttpSession->m_Requestline.m_Host.Find("web.qun.qq.com")  != -1 ) && 
		(pcalssHttpSession->m_Requestline.m_URI.Find("/cgi-bin/notice/get_data?") != -1 )  )
	{
		return QQ_HTTP_QUN_MSG;
	}
	else if(  (pcalssHttpSession->m_Requestline.m_Host.Find("cgi.find.qq.com")  != -1 ) && 
		(pcalssHttpSession->m_Requestline.m_URI.Find("/qqfind/qun/") != -1 )  )
	{
		return QQ_HTTP_QUN_SRARCHBUDDY;
	}
	else if(  (pcalssHttpSession->m_Requestline.m_Host.Find("cgi.find.qq.com")  != -1 ) && 
		(pcalssHttpSession->m_Requestline.m_URI.Find("/qqfind/buddy/") != -1 )  )
	{
		return QQ_HTTP_SEARCHBUDDY;
	}

	else if(  (pcalssHttpSession->m_Requestline.m_Host.Find("id.qq.com")  != -1 ) &&		//POST
		(pcalssHttpSession->m_Requestline.m_URI.Find("/cgi-bin/recommend_friend") != -1 )  )
	{
		return QQ_HTTP_ADDBUDDY;
	}

	else if(  (pcalssHttpSession->m_Requestline.m_Host.Find("cgi.find.qq.com")  != -1 ) &&	//群
		(pcalssHttpSession->m_Requestline.m_URI.Find("/qqfind_attach/") != -1 )  )
	{
		return QQ_HTTP_QUN_ATTACHBUDDY;
	}
	else if(  (pcalssHttpSession->m_Requestline.m_Host.Find("ctfs.ftn.qq.com")  != -1 ) && 
		(pcalssHttpSession->m_Requestline.m_URI.Find("/ftn_handler/") != -1 )  )
	{
		return QQ_HTTP_QUN_DOWNLOADFILE;
	}
	else if(  (pcalssHttpSession->m_Requestline.m_useragent.Compare("TXFTNActiveX_RawSocket")  == 0 ) &&	//POST
			  (pcalssHttpSession->m_Requestline.m_URI.Find("/ftn_handler/") != -1 )  )
	{
		return QQ_HTTP_QUN_UPLOADFILE;
	}
// 	else if(  (pcalssHttpSession->m_Requestline.m_Host.Find("qinfo.clt.qq.com")  != -1 ) && 
// 			  (pcalssHttpSession->m_Requestline.m_URI.Find("/cgi-bin/qun_info/get_group_share_download?") != -1 )  )
// 	{
// 		return QQ_HTTP_BUDDY_TRANSFILE;
// 	}
	else if(  (pcalssHttpSession->m_Requestline.m_Host.Find("pan.qun.qq.com")  != -1 ) && 
		(pcalssHttpSession->m_Requestline.m_URI.Find("/cgi-bin/thumbnail") != -1 )  )
	{
		return QQ_HTTP_QUN_FILE_LIST;
	}
	else if(  (pcalssHttpSession->m_Requestline.m_Host.Find("admin.qun.qq.com")  != -1 ) && 
		(pcalssHttpSession->m_Requestline.m_URI.Find("/cgi-bin/qun_admin/store_grp_host_info") != -1 )  )
	{
		return QQ_HTTP_QUN_OWNERINFO;
	}
	else if(  (pcalssHttpSession->m_Requestline.m_Host.Find("admin.qun.qq.com")  != -1 ) && 
		(pcalssHttpSession->m_Requestline.m_URI.Find("/cgi-bin/qun_admin/create_group") != -1 )  )
	{
		return QQ_HTTP_QUN_CREATE;
	}*/
	return FALSE;
	
}

int ProcessQQhttpSession( HttpSession * pclassHttpSession,const char * lpcstrPacket,int iValue)
{
	CString cstrTmp;
	CString cstrText;
	int		iHead		= 0;
	int		iEnd		= 0;

	if (iValue == QQ_HTTP_LOGIN)			//GET
	{
		cstrTmp		= lpcstrPacket;
		iHead = cstrTmp.Find("uin=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("uid=") );
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			return FALSE;
		}
		CString cstrLoginID = cstrTmp.Left(iEnd);	
		
		cstrTmp = lpcstrPacket;
		iHead = cstrTmp.Find("gender=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("gender=") );
		char iGender = cstrTmp.GetAt(0);
		CString cstrGender;
		if (iGender == 'm')
		{
			cstrGender = "男";
		}
		else if (iGender == 'f')
		{
			cstrGender = "女";
		}
		else
		{
			cstrGender = "未知";
		}

		cstrTmp = lpcstrPacket;
		iHead = cstrTmp.Find("age=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("age=") );
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			return FALSE;
		}
		CString cstrAge = cstrTmp.Left(iEnd);	

		cstrTmp = lpcstrPacket;
		iHead = cstrTmp.Find("ver=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("ver=") );
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			return FALSE;
		}
		CString cstrVersion = cstrTmp.Left(iEnd);	

		cstrTmp = lpcstrPacket;
		iHead = cstrTmp.Find("vocation=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("vocation=") );
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			return FALSE;
		}
		CString cstrVocation = cstrTmp.Left(iEnd);	

// 		cstrTmp = lpcstrPacket;
// 		iHead = cstrTmp.Find("group_type=");
// 		if (iHead < 0)
// 		{
// 			return FALSE;
// 		}
// 		cstrTmp = cstrTmp.Mid(iHead + strlen("group_type=") );
// 		iEnd = cstrTmp.Find(";");
// 		if (iEnd < 0)
// 		{
// 			return FALSE;
// 		}
// 		CString cstrGroup = cstrTmp.Left(iEnd);	
		cstrText.Format("state=%s\r\nuser=%s\r\n","登陆",cstrLoginID);
	}
	else if (iValue == QQ_HTTP_LOGOUT)		//GET
	{
		cstrTmp	= lpcstrPacket;
		iHead = cstrTmp.Find("clientuin=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("clientuin=") );
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			return FALSE;
		}
		CString cstrLogoutID = cstrTmp.Left(iEnd);	
		cstrText.Format("state=%s\r\nuser=%s\r\n","退出",cstrLogoutID);
	}
	else if (iValue == QQ_HTTP_BUDDY_TRANSFILE)				//POST packet
	{
		//HTTP协议中的长度字段表示的长度从正文开始,而不是从包头开始
		//POST包m_databuffer与lpcstrPacket相同，GET包m_databuffer与lpcstrPacket不相同
		//所有被分片后的HTTP包其包头各个字段都相同
		cstrTmp = pclassHttpSession->m_Requestline.m_URI;	//m_URI比m_URL少m_Host字段
		iHead = cstrTmp.Find("filekey=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("filekey="));
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			return FALSE;
		}
		CString cstrFileName = cstrTmp.Left(iEnd);
	
		CString cstrFileContent = pclassHttpSession->m_databuffer;		// or lpcstrPacket
		cstrText.Format("state=%s\r\nmsgtype=%s\r\nmsg=%s\r\n","传文件",
			cstrFileName,cstrFileContent);		
	}
	else if (iValue == QQ_HTTP_BUDDY_MSG)		//POST packet
	{
		cstrTmp = lpcstrPacket;
		iHead = cstrTmp.Find("\r\n\r\n");
		iEnd = iHead;
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("\r\n\r\n"));
		CString cstrBackTmp = cstrTmp;

		iHead = cstrTmp.Find("senduin=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("senduin=") );
		CString cstrRecvID;
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			if (cstrTmp.GetLength() > 12 )		//QQ id lenth <= 12
			{
				return FALSE;
			}
			cstrRecvID = cstrTmp;
		}
		else
		{
			cstrRecvID = cstrTmp.Left(iEnd);	
		}

		cstrTmp = cstrBackTmp;
		iHead = cstrTmp.Find("uin=");
		if (iHead < 0)
		{
			return FALSE;
		}
		CString cstrSendID;
		cstrTmp = cstrTmp.Mid(iHead + strlen("uin=") );
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			if (cstrTmp.GetLength() > 12 )
			{
				return FALSE;
			}
			cstrSendID = cstrTmp;
		}
		else
		{
			cstrSendID = cstrTmp.Left(iEnd);
		}
		cstrText.Format("state=%s\r\nfrom=%s\r\nto=%s\r\n","发送消息",cstrSendID,cstrRecvID);		
	}
	else if (iValue == QQ_HTTP_QUN_MSG)		//GET packet
	{
		cstrTmp = lpcstrPacket;
		iHead = cstrTmp.Find("qid=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("qid=") );
		CString cstrGroupId;
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			if (cstrTmp.GetLength() > 12 )
			{
				return FALSE;
			}
			cstrGroupId = cstrTmp;
		}
		else
		{
			cstrGroupId = cstrTmp.Left(iEnd);
		}

		cstrTmp = pclassHttpSession->m_Requestline.m_Cookie;
		iHead = cstrTmp.Find("uin=o");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("uin=o"));
		CString cstrMemberId;
		iEnd = cstrTmp.Find(";");
		if (iEnd < 0)
		{
			if (cstrTmp.GetLength() > 12)
			{
				return FALSE;
			}
			cstrMemberId = cstrTmp;
		}
		else
		{
			cstrMemberId = cstrTmp.Left(iEnd);
		}
		cstrText.Format("state=%s\r\noid=%s\r\nmid=%s\r\n","群消息",cstrGroupId,cstrMemberId);		
	}
	else if (iValue == QQ_HTTP_ADDBUDDY)		//GET
	{
		cstrTmp = pclassHttpSession->m_Requestline.m_Cookie;
		iHead = cstrTmp.Find("uin=o");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("uin=o"));
		CString cstrUserQQId;
		iEnd = cstrTmp.Find(";");
		if (iEnd < 0)
		{
			if (cstrTmp.GetLength() > 12)
			{
				return FALSE;
			}
			cstrUserQQId = cstrTmp;
		}
		else
		{
			cstrUserQQId = cstrTmp.Left(iEnd);
		}
		
		cstrTmp = lpcstrPacket;
// 		iHead = cstrTmp.Find("\r\n\r\n");
// 		if (iHead < 0)
// 		{
// 			return FALSE;
// 		}
// 		cstrTmp = cstrTmp.Mid(iHead + strlen("\r\n\r\n"));

		iHead = cstrTmp.Find("u=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("u="));
		CString cstrDstQQid;
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			if (cstrTmp.GetLength() > 12)
			{
				return FALSE;
			}
			cstrDstQQid = cstrTmp;
		}
		else
		{
			cstrDstQQid = cstrTmp.Left(iEnd);
		}
		cstrText.Format("state=%s\r\nuser=%s\r\nuid=%s\r\n","添加好友",cstrUserQQId,cstrDstQQid);
	}
	/*
	else if (iValue == QQ_HTTP_SEARCHBUDDY)		//POST
	{
		cstrTmp = pclassHttpSession->m_Requestline.m_Cookie;
		iHead = cstrTmp.Find("uin=o");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("uin=o"));
		CString cstrQQid;
		iEnd = cstrTmp.Find(";");
		if (iEnd < 0)
		{
			if (cstrTmp.GetLength() > 12)
			{
				return FALSE;
			}
			cstrQQid = cstrTmp;
		}
		else
		{
			cstrQQid = cstrTmp.Left(iEnd);
		}
		
		cstrTmp = lpcstrPacket;
		iHead = cstrTmp.Find("\r\n\r\n");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("\r\n\r\n"));
		CString cstrBackTmp = cstrTmp;
		
		iHead = cstrTmp.Find("keyword=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("keyword="));
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			return FALSE;
		}
		cstrTmp= cstrTmp.Left(iEnd);
		char * strTmp = cstrTmp.GetBuffer(cstrTmp.GetLength());
		Utf8ToGB2312(DecodeUrl(strTmp));
		CString cstrDstQQnick  = strTmp;
		
		
		cstrTmp = cstrBackTmp;
		iHead = cstrTmp.Find("gid=");
		if (iHead < 0)
		{
			cstrText.Format("state=%s\r\nuserid=%s\r\nsearchnick=%s\r\n","qq_findbuddy",cstrQQid,cstrDstQQnick);
		}
		else
		{
			cstrTmp = cstrTmp.Mid(iHead + strlen("gid="));
			CString cstrDstQQid;
			iEnd = cstrTmp.Find("&");
			if (iEnd < 0)
			{
				if (cstrTmp.GetLength() > 12)
				{
					return FALSE;
				}
				cstrDstQQid = cstrTmp;
			}
			else
			{
				cstrDstQQid = cstrTmp.Left(iEnd);
			}
			cstrText.Format("state=%s\r\nuserid=%s\r\nsearchnick=%s\r\nsearchid=%s\r\n","qq_findbuddy",cstrQQid,cstrDstQQnick,cstrDstQQid);
		}
	}*/
	/*
	else if (iValue == QQ_HTTP_QUN_ATTACHBUDDY)		//POST
	{
		cstrTmp = pclassHttpSession->m_Requestline.m_Cookie;
		iHead = cstrTmp.Find("uin=o");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("uin=o"));
		CString cstrQQid;
		iEnd = cstrTmp.Find(";");
		if (iEnd < 0)
		{
			if (cstrTmp.GetLength() > 12)
			{
				return FALSE;
			}
			cstrQQid = cstrTmp;
		}
		else
		{
			cstrQQid = cstrTmp.Left(iEnd);
		}
		
		cstrTmp = lpcstrPacket;
		iHead = cstrTmp.Find("\r\n\r\n");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("\r\n\r\n"));
		CString cstrBackTmp = cstrTmp;
		
		iHead = cstrTmp.Find("keyword=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("keyword="));
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			return FALSE;
		}
		cstrTmp= cstrTmp.Left(iEnd);
		char * strTmp = cstrTmp.GetBuffer(cstrTmp.GetLength());
		Utf8ToGB2312(DecodeUrl(strTmp));
		CString cstrDstQQnick  = strTmp;		

		cstrTmp = cstrBackTmp;
		iHead = cstrTmp.Find("gid=");
		if (iHead < 0)
		{
			cstrText.Format("state=%s\r\nuserid=%s\r\nsearchnick=%s\r\n","qq_qun_addbuddy",cstrQQid,cstrDstQQnick);
		}
		else
		{
			cstrTmp = cstrTmp.Mid(iHead + strlen("gid="));
			CString cstrDstQQid;
			iEnd = cstrTmp.Find("&");
			if (iEnd < 0)
			{
				if (cstrTmp.GetLength() > 12)
				{
					return FALSE;
				}
				cstrDstQQid = cstrTmp;
			}
			else
			{
				cstrDstQQid = cstrTmp.Left(iEnd);
			}
			cstrText.Format("state=%s\r\nuserid=%s\r\nsearchnick=%s\r\ngroupid=%s\r\n","qq_addbuddy",cstrQQid,cstrDstQQnick,cstrDstQQid);
		}
	}*//*
	else if (iValue == QQ_HTTP_QUN_SRARCHBUDDY)		//GET
	{
		//Cookie中用分号分割每一项
		cstrTmp = pclassHttpSession->m_Requestline.m_Cookie;
		iHead = cstrTmp.Find("uin=o");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("uin=o"));
		CString cstrQQid;
		iEnd = cstrTmp.Find(";");
		if (iEnd < 0)
		{
			if (cstrTmp.GetLength() > 12)
			{
				return FALSE;
			}
			cstrQQid = cstrTmp;
		}
		else
		{
			cstrQQid = cstrTmp.Left(iEnd);
		}
		
		cstrTmp = lpcstrPacket;
		iHead = cstrTmp.Find("\r\n\r\n");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("\r\n\r\n"));
		CString cstrBackTmp = cstrTmp;
		
		
		iHead = cstrTmp.Find("k=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("k="));
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			return FALSE;
		}
		cstrTmp= cstrTmp.Left(iEnd);
		char * strTmp = cstrTmp.GetBuffer(cstrTmp.GetLength());
		Utf8ToGB2312(DecodeUrl(strTmp));
		CString cstrDstQQnick  = strTmp;		
		
		cstrTmp = cstrBackTmp;
		iHead = cstrTmp.Find("gid=");
		if (iHead < 0)
		{
			cstrText.Format("state=%s\r\nuserid=%s\r\nsearchnick=%s\r\n","findqun",cstrQQid,cstrDstQQnick);
		}
		else
		{
			cstrTmp = cstrTmp.Mid(iHead + strlen("gid="));
			CString cstrDstQQid;
			iEnd = cstrTmp.Find("&");
			if (iEnd < 0)
			{
				if (cstrTmp.GetLength() > 12)
				{
					return FALSE;
				}
				cstrDstQQid = cstrTmp;
			}
			else
			{
				cstrDstQQid = cstrTmp.Left(iEnd);
			}
			cstrText.Format("state=%s\r\nuserid=%s\r\nsearchnick=%s\r\nsearchid=%s\r\n","qq_findqun",cstrQQid,cstrDstQQnick,cstrDstQQid);
		}
	}*/
	else if (iValue == QQ_HTTP_QUN_DOWNLOADFILE)		
	{
		cstrTmp = lpcstrPacket;
		iHead = cstrTmp.Find("fname=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("fname="));
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			return FALSE;
		}
		CString cstrFileName = cstrTmp.Left(iEnd);

		char * strTmp = cstrFileName.GetBuffer(cstrFileName.GetLength());
		DecodeUrl(strTmp);
		Decode_UrlUnicode(strTmp);
		cstrFileName= strTmp;

		cstrTmp = pclassHttpSession->m_Requestline.m_Cookie;
		iHead = cstrTmp.Find("uin=o");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("uin=o"));
		iEnd = cstrTmp.Find(";");
		if (iEnd < 0)
		{
			return FALSE;
		}
		CString cstrQQid = cstrTmp.Left(iEnd);
		cstrText.Format("state=%s\r\nuser=%s\r\nmsgtype=%s\r\n","下载群文件",cstrQQid,cstrFileName);
	}
	else if (iValue == QQ_HTTP_QUN_UPLOADFILE)
	{
		cstrTmp = pclassHttpSession->m_Requestline.m_URI;
		iHead = cstrTmp.Find("bmd5=");
		if (iHead < 0)
		{
			return FALSE;
		}
		CString cstrFileName;
		cstrTmp = cstrTmp.Mid(iHead + strlen("bmd5="));
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			if (cstrTmp.GetLength() != 32)
			{
				return FALSE;
			}
			cstrFileName = cstrTmp;
		}
		cstrFileName = cstrTmp.Left(iEnd);
		CString cstrFileContent = lpcstrPacket;
		cstrText.Format("state=%s\r\nmsgtype=%s\r\nmsg=%s\r\n","上传群文件",cstrFileName,cstrFileContent);
	}
	else if (iValue == QQ_HTTP_BUDDY_TRANSFILE)
	{
		cstrTmp	= lpcstrPacket;
		iHead = cstrTmp.Find("gc=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("gc=") );
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			return FALSE;
		}
		CString cstrUploadID = cstrTmp.Left(iEnd);	

		cstrTmp	= lpcstrPacket;
		iHead = cstrTmp.Find("file_name=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("file_name=") );
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			return FALSE;
		}
		CString cstrFileName = cstrTmp.Left(iEnd);	

		char * strTmp = cstrFileName.GetBuffer(cstrFileName.GetLength());
		DecodeUrl(strTmp);
		Decode_UrlUnicode(strTmp);
		cstrFileName= strTmp;

		cstrTmp	= pclassHttpSession->m_Requestline.m_Cookie;
		iHead = cstrTmp.Find("uin=o");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("uin=o") );
		iEnd = cstrTmp.Find(";");
		if (iEnd < 0)
		{
			return FALSE;
		}
		CString cstrDownloadID = cstrTmp.Left(iEnd);	

		cstrText.Format("state=%s\r\nuploaderid=%s\r\ndownloaderid=%s\r\nfilename=%s\r\n",
		"qq_transfile",cstrUploadID,cstrDownloadID,cstrFileName);
	}	
	/*
	else if (iValue == QQ_HTTP_QUN_FILE_LIST)			//POST
	{
		cstrTmp = pclassHttpSession->m_Requestline.m_Cookie;
		iHead = cstrTmp.Find("uin=o");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("uin=o"));
		CString cstrDownloadID;
		iEnd = cstrTmp.Find(";");
		if (iEnd < 0)
		{
			if (cstrTmp.GetLength() > 12)
			{
				return FALSE;
			}
			cstrDownloadID = cstrTmp;
		}
		else
		{
			cstrDownloadID = cstrTmp.Left(iEnd);
		}

		cstrTmp = lpcstrPacket;
// 		iHead = cstrTmp.Find("\r\n\r\n");
// 		if (iHead < 0)
// 		{
// 			return FALSE;
// 		}
// 		cstrTmp = cstrTmp.Mid(iHead + strlen("\r\n\r\n"));
		CString cstrBackTmp = cstrTmp;

		iHead = cstrTmp.Find("gc=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("gc=") );
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			return FALSE;
		}
		CString cstrUploadID = cstrTmp.Left(iEnd);	
		
		cstrTmp	= cstrBackTmp;
		iHead = cstrTmp.Find("fp_list=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("fp_list=") );
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			return FALSE;
		}
		CString cstrFileName = cstrTmp.Left(iEnd);	
		
		char * strTmp = cstrFileName.GetBuffer(cstrFileName.GetLength());
		DecodeUrl(strTmp);
		Decode_UrlUnicode(strTmp);
		cstrFileName = strTmp;
		cstrText.Format("qunownerid=%s\r\ndownloaderid=%s\r\nfilelist=%s\r\nstate=%s\r\n",
			"qq_qun_filelist",cstrUploadID,cstrDownloadID,cstrFileName);
	}
	*//*
	else if (iValue == QQ_HTTP_QUN_OWNERINFO)		//GET
	{
		cstrTmp = pclassHttpSession->m_Requestline.m_Cookie;
		iHead = cstrTmp.Find("uin=o");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("uin=o"));
		CString cstrUserID;
		iEnd = cstrTmp.Find(";");
		if (iEnd < 0)
		{
			if (cstrTmp.GetLength() > 12)
			{
				return FALSE;
			}
			cstrUserID = cstrTmp;
		}
		else
		{
			cstrUserID = cstrTmp.Left(iEnd);
		}
		
		cstrTmp = lpcstrPacket;
		iHead = cstrTmp.Find("\r\n\r\n");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("\r\n\r\n"));
		CString cstrBackTmp = cstrTmp;
		
		iHead = cstrTmp.Find("n=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("n=") );
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			return FALSE;
		}
		CString cstrOwnerNick = cstrTmp.Left(iEnd);	
		
		cstrTmp	= cstrBackTmp;
		iHead = cstrTmp.Find("v=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("v=") );
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			return FALSE;
		}
		CString cstrOwnerPhone = cstrTmp.Left(iEnd);	
		cstrText.Format("state=%s\r\nuserqqid=%s\r\nqq_qun_ownerid=%s\r\nqun_owner_phone=%s\r\n",
			"qq_qun_ownerinfo",cstrUserID,cstrOwnerNick,cstrOwnerPhone);
	}
	else if (iValue == QQ_HTTP_QUN_CREATE)
	{
		cstrTmp = pclassHttpSession->m_Requestline.m_Cookie;
		iHead = cstrTmp.Find("uin=o");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("uin=o"));
		CString cstrCreateId;
		iEnd = cstrTmp.Find(";");
		if (iEnd < 0)
		{
			if (cstrTmp.GetLength() > 12)
			{
				return FALSE;
			}
			cstrCreateId = cstrTmp;
		}
		else
		{
			cstrCreateId = cstrTmp.Left(iEnd);
		}
		
		cstrTmp = lpcstrPacket;
		iHead = cstrTmp.Find("\r\n\r\n");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("\r\n\r\n"));
		CString cstrBackTmp = cstrTmp;
		
		iHead = cstrTmp.Find("gn=");
		if (iHead < 0)
		{
			return FALSE;
		}
		cstrTmp = cstrTmp.Mid(iHead + strlen("gn=") );
		iEnd = cstrTmp.Find("&");
		if (iEnd < 0)
		{
			return FALSE;
		}
		CString cstrGroupName = cstrTmp.Left(iEnd);	
		char * strGroupName = cstrGroupName.GetBuffer(cstrGroupName.GetLength());
		Utf8ToGB2312(DecodeUrl(strGroupName));
		cstrGroupName = strGroupName;
		
		cstrTmp	= cstrBackTmp;
		iHead = cstrTmp.Find("m=");
		CString cstrGroupMember;
		if (iHead )
		{
			cstrTmp = cstrTmp.Mid(iHead + strlen("m=") );
			iEnd = cstrTmp.Find("&");
			if (iEnd)
			{
				cstrGroupMember = cstrTmp.Left(iEnd);	
				char * strGroupMember = cstrGroupMember.GetBuffer(cstrGroupMember.GetLength());
				Utf8ToGB2312(DecodeUrl(strGroupMember));
				cstrGroupMember = strGroupMember;				
			}
		}	

		cstrTmp	= cstrBackTmp;
		iHead = cstrTmp.Find("pos=");
		CString cstrPositon;
		if (iHead )
		{
			cstrTmp = cstrTmp.Mid(iHead + strlen("pos=") );
			iEnd = cstrTmp.Find("&");
			if (iEnd )
			{
				cstrPositon = cstrTmp.Left(iEnd);	
				char * strPositon = cstrPositon.GetBuffer(cstrPositon.GetLength());
				Utf8ToGB2312(DecodeUrl(strPositon));
				cstrPositon = strPositon;	
			}
		}
		cstrText.Format("state=%s\r\ncreatid=%s\r\ngroupname=%s\r\ngroupmember=%s\r\ngroupposition=%s\r\n",
			"qq_creategroup",cstrCreateId,cstrGroupName,cstrGroupMember,cstrPositon);
	}
		*/		
	SYSTEMTIME stNow;
	GetLocalTime(&stNow);
	CString tmppath = pclassHttpSession->m_dceprocotol->m_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	pclassHttpSession->m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_imsg_oicq.indexfile.txt", 
	tmppath, stNow.wYear, stNow.wMonth, stNow.wDay, stNow.wHour,stNow.wMinute, stNow.wSecond, 
	pclassHttpSession->m_sip, pclassHttpSession->m_dip, rand());
	pclassHttpSession->WriteIndexFile("imsg_qq","imsg_qq",cstrText,pclassHttpSession->m_dceprocotol->m_httpMissObj);
	return TRUE;
}