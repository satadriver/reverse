//
// C++ Implementation: Getmoniterweb
//
// Description: 
//
//
// Author: lp&tcf <netspy@netspy>, (C) 2010
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "stdafx.h"
//#include <dirent.h> //search dir
#include <map>
#include "Getmoniterweb.h"

#include "synctable.h"
extern CString GetDomain(CString url);

moniterweb g_moniterweb;
moniterweb::moniterweb()
{
	m_build=0;
}


moniterweb::~moniterweb()
{
}
bool moniterweb::UpdateMap()
{
	CStringArray urllist,infotypelist,splitlist, memolist;
	CString httpupfile;

	static int failtimes = 0;
	httpupfile.Format( "e:\\netspy\\para\\dce\\ekeyword.bcp" );
	int build=(m_build==0)?1:0;
	try
	{


		failtimes = 0;

		CSyncPara synPara;
		DWORD dealPos = 0;
		
		if(FALSE == synPara.LoadPara(httpupfile, dealPos,FALSE))
		{
			return FALSE;
		}
		CStringArray arrField;
		BOOL bGetOne;
		CString strV, strSplit, strDesc, strQqinfo;
		CString classid,webname,strUrl,matchtype;
		int InfoType, FieldType;
		InfoType = FieldType = 0;
		BOOL bInvalid;
		do{
			arrField.RemoveAll();
			bGetOne = synPara.GetOneRecord(dealPos, arrField);
			if(FALSE == bGetOne)
				break;

			//ASSERT(arrField.size() == bcp_allipaddr_count);
			if(arrField.GetSize() < 4)
			{
				WriteLog("dce","upsplit para is illegal, Item %d low 5", arrField.GetSize());
				continue;
			}

			sMoniterUnit s_MoniterUnit;
			bInvalid = TRUE;
			
			s_MoniterUnit.classid = arrField[0];
			s_MoniterUnit.classid.TrimRight();
			s_MoniterUnit.classid.TrimLeft();
			
			s_MoniterUnit.webname = arrField[0];
			s_MoniterUnit.webname.TrimRight();
			s_MoniterUnit.webname.TrimLeft();
					
			s_MoniterUnit.strUrl = arrField[3];
			s_MoniterUnit.strUrl.TrimRight();
			s_MoniterUnit.strUrl.TrimLeft();

			s_MoniterUnit.matchtype = arrField[4];
			s_MoniterUnit.matchtype.TrimRight();
			s_MoniterUnit.matchtype.TrimLeft();

			CString spliturl=GetUrl(s_MoniterUnit.strUrl);
			CString domain=GetDomain(s_MoniterUnit.strUrl);
			CString topdomain=SplitTopDomain(domain);
			
			if(s_MoniterUnit.matchtype.CompareNoCase("模糊")==0)
			{
				map<CString,sMoniterUnit>::iterator iter;
				iter = m_moniterdomainmap[build].find(topdomain);
				if(iter == m_moniterdomainmap[build].end())
				{
					m_moniterdomainmap[build][topdomain]=s_MoniterUnit;
				}
			}
			else
			{
				map<CString,sMoniterUnit>::iterator iter;
				iter = m_moniterurlnmap[build].find(spliturl);
				if(iter == m_moniterurlnmap[build].end())
				{
					m_moniterurlnmap[build][spliturl]=s_MoniterUnit;
				}
			}


		}while(bGetOne);

		m_build=build;
		m_moniterdomainmap[(m_build==0)?1:0].clear();
		m_moniterurlnmap[(m_build==0)?1:0].clear();		
		return TRUE;
	}
	catch(...)
	{
		WriteLog("dce","moniterweb Catch Error!");
		return FALSE;
	}

	return true;
}
int moniterweb::GetMoniterweb(char* buf,CString url)
{
int error=0;
	try
	{
		int pos=0;
		map<CString,sMoniterUnit>::iterator iter;

		int numpos=0;
		CString nowtime;
error=1;
				
		CString spliturl=GetUrl(url);
		CString domain=GetDomain(url);
		CString topdomain=SplitTopDomain(domain);
error=2;
		iter = m_moniterurlnmap[m_build].find(spliturl);
		if(iter != m_moniterurlnmap[m_build].end())
		{
			pos += sprintf( buf, "moniterclassid=%s\r\n",  iter->second.classid);
			pos += sprintf( buf+pos, "webname=%s\r\n",  iter->second.webname);
		}
error=3;
		iter = m_moniterdomainmap[m_build].find(topdomain);
		if(iter != m_moniterdomainmap[m_build].end())
		{
			pos += sprintf( buf+pos, "moniterclassid=%s\r\n",  iter->second.classid);
			pos += sprintf( buf+pos, "webname=%s\r\n",  iter->second.webname);
		}
error=4;
		return pos;
	}
	catch (...)
	{
		WriteLog("dce","GetMoniterweb Catch Error!=%d",error);
		return FALSE;

	}
	

}


//获得顶级域名
CString SplitTopDomain(const char *domain)
{
	CString tmpTop;
	CString strDomain = domain;
	CString Part1, Part2, Part3;
	int offDot;
	int havePart = 0;	// 0x4 info, 0x2 com, 0x1 cn

	offDot = strDomain.ReverseFind('.');
	if(offDot == -1)
	{
		return "";
	}
	
		int num;
	int ipc1, ipc2, ipc3, ipc4; 
	
	// 构造IP单元对象
	num = sscanf(domain,"%d.%d.%d.%d",&ipc1,&ipc2,&ipc3,&ipc4);
	if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
		(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
		(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
	{
		;
	}
	else
	{
		return domain;
	}
	Part3 = strDomain.Mid(offDot);
	if(strlen(Part3) == 3)		// 国家代码, 2位+'.'共3位
		havePart |= 0x1;
	else
		havePart |= 0x2;

	strDomain = strDomain.Left(offDot);
	offDot = strDomain.ReverseFind('.');
	if(offDot == -1)
	{
		tmpTop.Format("%s%s", strDomain, Part3);
		return tmpTop;
	}
	Part2 = strDomain.Mid(offDot);
	if(havePart&0x2)	// have .com
	{
		tmpTop.Format("%s%s", Part2.Mid(1), Part3);
		return tmpTop;
	}
	else
	{
		Search sch(Part2);
//		sch.InitSearch;
		if(sch.find(".com.net.edu.gov.org.aero.biz.cat.coop.info.jobs.mobi.museum.name.pro.travel.mil.int") != -1)
			havePart |= 0x2;
		else
		{
			tmpTop.Format("%s%s", Part2.Mid(1), Part3);
			return tmpTop;
		}
	}

	strDomain = strDomain.Left(offDot);
	offDot = strDomain.ReverseFind('.');
	if(offDot == -1)
		tmpTop.Format("%s%s%s", strDomain, Part2, Part3);
	else
	{
		Part1 = strDomain.Mid(offDot);
		tmpTop.Format("%s%s%s", Part1.Mid(1), Part2, Part3);
	}
	return tmpTop;
}


//获取url，去掉？后边的，把http www等去掉
CString GetUrl(CString url)
{
	CString domain;
	
	if(url.GetLength()<=7)
		domain = url;
	else
	{
		if(memcmp(url,"http://",7)==0)
		{
			int pos=0;
			CString tmp;
			tmp=url.Mid(7);
			pos=tmp.Find('?');
			if(pos>0)
			{
				CString tmpstr;
				tmpstr=tmp.Left(pos);
				domain=tmpstr;
			}
			else
			{
				domain=tmp;
			}
		}
		else
		{
			domain=url;
		}
	}
	if (domain.GetLength() > 4)
	{
		if (memcmp(domain,"www.",4)==0)
		{
			domain.Format("%s",domain.Mid(4));
		}
	}

	return domain;
}
