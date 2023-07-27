// thread.cpp: implementation of the thread class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "http.h"
#include "httpsession.h"
#include "publicfun.h"
#include "thread.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL thunder_DealThunderSend(HttpSession& s)
{
	//ͨ��Post URL���ݶ�THREAD��λ
	CString posturl;
	if(s.m_Requestline.m_URL.GetLength()>0)
	{
		string tmpurlstring;
		CString tmpurl;
		tmpurl = s.m_Requestline.m_URL;
		tmpurlstring = thunder_UrlDecode((char*)(LPCTSTR)tmpurl);
		posturl = tmpurlstring.c_str();
	}
	else
	{
		return FALSE;
	}
	//�Խ��ܺ�����ݽ��з���
	CString fileurl,pageurl,zclient;
	THREADSTRUCT threads;
	int pos;
	pos = posturl.Find("fileurl=");
	if(pos!=-1)
	{
		int pos2,pos3;
		pos2 = posturl.Find("&pageurl=");
		if(pos!=-1)
		{
			fileurl = posturl.Mid(pos+8,pos2-pos-8);
		}
		pos3 = posturl.Find("&z={");
		if(pos3!=-1)
		{
			pageurl = posturl.Mid(pos2+9,pos3-pos2-9);
			zclient = thunder_ClientSplit(posturl.Right(posturl.GetLength()-pos3-3),threads);
		}
		//////////////////////////////////////////////////////////////////////////
		//д�����ļ�
		CString txt;
		txt.Format("fileurl=%s\r\npageurl=%s\r\nclientmac=%s\r\nclientip=%s\r\nwifissid=%s\r\nclientpcname=%s\r\n",
			fileurl,pageurl,threads.mac,threads.ip,threads.wifissid,threads.pcname);
		SYSTEMTIME now;
		GetLocalTime(&now);
		CString tmppath = g_httpMissObj->GetTmpPath();
		ASSERT(!tmppath.IsEmpty());
		s.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_p2p_thunder.indexfile.txt",
			tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
			now.wMinute, now.wSecond, s.m_sip, s.m_dip, rand());
		s.WriteIndexFile("p2p_thunder","p2p_thunder",txt,g_httpMissObj);
		//////////////////////////////////////////////////////////////////////////
		//����ϢMAC��ַ�ύ��MAC��ַ��
		WriteThreadMacIndexFile(threads.pcname,threads.mac,s.m_sip,"",s.m_AuthInfo.m_UserAccount1);	
	}
	return TRUE;
}

BOOL thunder_ClientSplit(CString posturl,THREADSTRUCT& threads)
{
	if(posturl.GetLength()>0)
	{
		posturl.Replace("{","");
		posturl.Replace("}","");
		posturl.Replace("\"","");

		CStringArray posturlArray1;
		Splitstr(posturl,posturlArray1,',');
		for (int i=0; i<posturlArray1.GetSize(); i++)
		{
			CStringArray posturlArray2;
			Splitstr(posturlArray1[i],posturlArray2,':');

			if(posturlArray2.GetSize()==2)
			{
				if(posturlArray2[0]=="p")
				{
					threads.mac = posturlArray2[1].Left(posturlArray2[1].GetLength()-4);
				}
				else if(posturlArray2[0]=="ip")
				{
					threads.ip = posturlArray2[1];
				}
				else if(posturlArray2[0]=="wifissid")
				{
					threads.wifissid = posturlArray2[1];
				}
				else if(posturlArray2[0]=="pn")
				{
					threads.pcname = posturlArray2[1];
				}	
			}
		}
	}
	return TRUE;
}

std::string thunder_UrlDecode(const std::string& src)
{
    std::string dst, dsturl;
    int srclen = src.size();
    for (size_t i = 0; i < srclen; i++)
    {
        if (src[i] == '%')
        {
            if(isxdigit(src[i + 1]) && isxdigit(src[i + 2]))
            {
                char c1 = src[++i];
                char c2 = src[++i];
                c1 = c1 - 48 - ((c1 >= 'A') ? 7 : 0) - ((c1 >= 'a') ? 32 : 0);
                c2 = c2 - 48 - ((c2 >= 'A') ? 7 : 0) - ((c2 >= 'a') ? 32 : 0);
                dst += (unsigned char)(c1 * 16 + c2);
            }
        }
        else
            if (src[i] == '+')
            {
                dst += ' ';
            }
            else
            {
                dst += src[i];
            }
    }
    return dst;
}

BOOL thunder_DealThunderLogin(HttpSession& s,LPCTSTR pPacket)
{
	int pos;
	pos = 24;	
	char username[100]={0};	
	memcpy(username,pPacket+pos,100);
	CString txt;
	txt.Format("user=%s\r\nstate=��½\r\n",username);
	//д�����ļ�
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = g_httpMissObj->GetTmpPath();
	ASSERT(!tmppath.IsEmpty());
	s.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_p2p_thunder.indexfile.txt",
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond, s.m_sip, s.m_dip, rand());
	s.WriteIndexFile("p2p_thunder","p2p_thunder",txt,g_httpMissObj);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// Ѹ�׿ͻ�����Ϣ��ȡģ��

//Ѹ��MAC��ַ��ȡ
BOOL thunder_GetDevInfo(HttpSession& s,LPCTSTR pPacket)
{
	//URL����
	CString AnylazeUrl;
	AnylazeUrl = s.m_url;
	CStringArray urlArray;
	thunder_SplitUrl(AnylazeUrl,urlArray);
	//�����Ҫ��������Ϣ
	CString MacStr,SystemStr,SystemVerStr;
	for(int i=0; i<urlArray.GetSize(); i++)
	{
		if(urlArray[i].Find("u4="))
		{
			MacStr = thunder_GetItemValue(urlArray[i]);
		}
		else if(urlArray[i].Find("u5="))
		{
			SystemStr = thunder_GetSystemName(thunder_GetItemValue(urlArray[i]));
		}
		else if(urlArray[i].Find("u6="))
		{
			SystemVerStr = thunder_GetItemValue(urlArray[i]);
		}
	}
	//Ѹ��ÿ��������3�����Ƶ���������˴����ڹ��˵�����3��
	if(SystemVerStr != "32" || SystemVerStr != "64")
	{
		return FALSE;
	}
	CString ostype;
	ostype.Format("%s(%s)",SystemStr,SystemVerStr);
	//д�����ļ�
// 	CString txt;
// 	txt.Format("devicemac=%s\r\nplatform=%s(%s)\r\n",MacStr,SystemStr,SystemVerStr);
// 
// 	SYSTEMTIME now;
// 	GetLocalTime(&now);
// 	CString tmppath = g_httpMissObj->GetTmpPath();
// 	ASSERT(!tmppath.IsEmpty());
// 
// 	s.m_indexfname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u_thunder_mac.indexfile.txt",
// 		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour,
// 		now.wMinute, now.wSecond, s.m_sip, s.m_dip, rand());
// 
// 	s.WriteIndexFile("thunder_mac","thunder_mac",txt,g_httpMissObj);
	MacStr = MacStr.Replace(":","");
	WriteThreadMacIndexFile("",MacStr,s.m_sip,ostype,s.m_AuthInfo.m_UserAccount1);
	return TRUE;
}

BOOL thunder_SplitUrl(CString urlstr,CStringArray &urlArray)
{
	CString tmpurl;
	if(urlstr.GetLength()>0)
	{
		tmpurl = urlstr.Mid(urlstr.Find("?")+1);
		Splitstr(tmpurl,urlArray,'&');
	}
	return TRUE;
}

CString thunder_GetItemValue(CString str)
{
	if(str.Find("=")!=-1)
	{
		return str.Mid(str.Find("=")+1);
	}
	else
		return "";
}

//����ϵͳ�汾�ŷ���ϵͳ����
CString thunder_GetSystemName(CString str)
{

	if(str == "4.0")
	{
		return "win95";
	}
	else if(str == "4.1")
	{
		return "win98";
	}
	else if(str == "5.0")
	{
		return "win2000";
	}
	else if(str == "5.1")
	{
		return "winxp";
	}
	else if(str == "5.2")
	{
		return "win2003";
	}
	else if(str == "6.0")
	{
		return "winVista";
	}
	else if(str == "6.1")
	{
		return "win7";
	}

	return "undefined";
}

//дMAC������Ϣ
BOOL WriteThreadMacIndexFile(CString pcname,CString pcmac,CString pcip,CString pcostype,CString pcaccount)
{

	//����IP
	int pos = pcip.Find("_");
	if(pos!=-1)
	{
		CString tmppcip;
		tmppcip = pcip.Left(pos);
		pcip = tmppcip;
	}

	CString currenttm;
	SYSTEMTIME now;
	GetLocalTime(&now);

	currenttm.Format("%04d%02d%02d%02d%02d%02d",
		now.wYear, now.wMonth, now.wDay, now.wHour,
		now.wMinute, now.wSecond);

	CString writecontent;
	//��ʽ��mac\t���������\tʱ��\t���������\t�����mac\t�����ip\t���������ϵͳ\t�����ʺ�\n
	//       0------1---------2--------3---------4-----------5----------6----------
	writecontent.Format("Ѹ��\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
						v_ddelist.spyaddr,currenttm,pcname,pcmac,pcip,pcostype,pcaccount);


	CString threadmacpath;
	threadmacpath.Format("e:\\netspy\\basedata\\tmpmac");
	if(access(threadmacpath,0)!=0)
	{
		mkdir(threadmacpath);
	}

	//�����ļ���
	CString savefilepath;
	savefilepath.Format("%s\\%s_%s_mac.tmp",threadmacpath,currenttm,v_ddelist.spyaddr);

	CFile cf;
	cf.Open(savefilepath,CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate);
	cf.Write(writecontent,writecontent.GetLength());
	cf.Close();

	return TRUE;
}
