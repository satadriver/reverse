// NetHotTable.cpp: implementation of the NetHotTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "http.h"
#include "httpSession.h"
#include "publicfun.h"
#include "NetHotTable.h"
#include <direct.h>

#define KindCount 9

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern void ConverHttp2Html(char *tmp_content, int tmp_content_len);

//***********�����ȵ�ؼ��ֵ�������Ϣ******************
#define			DOMAINNUM		19

typedef BOOL (NetHotTable::*LPANALYFUN)(const char* url, DATEHEADER* packh, HttpCommInfo& header);	// ����ؼ��ֲ�ֺ���ָ��

typedef struct SearchDomainInfo
{
	LPCTSTR			pdomain;
	int				domainlen;
	const Search*	pSearchKey;
	LPANALYFUN		panalyfun;
}SearchDomainInfo;

const Search sKey_Baidu[] = 
{
	Search("wd="),
	Search("word="),
	Search("kd="),
};

const Search sKey_Google[] =
{
	Search("q="),
};

const Search sKey_China[] = 
{
	Search("word="),
	Search("query="),
};

static SearchDomainInfo pSearchDomanArr[DOMAINNUM] = 
{
	{_T("baidu.com"),			9,		sKey_Baidu,		NetHotTable::Analysbaidu},
	{_T("google.com"),			10,		sKey_Google,	NetHotTable::AnalysGoogle},
	{_T("so.china.com"),		12,		NULL,			NetHotTable::Analychina},		//�л������� so.china.com
	{_T("iask.sina.com.cn"),	16,		NULL,			NetHotTable::AnalyiAsk},
	{_T("v.yisou.com"),			11,		NULL,			NetHotTable::AnalysYisou},
	{_T("so.163.com"),			10,		NULL,			NetHotTable::Analys163},
	{_T("search.msn.com"),		14,		NULL,			NetHotTable::AnalysMsn},
	{_T("sogou.com"),			9,		NULL,			NetHotTable::AnalySobao},      //SOGOU���� 
	{_T("mytianwang.cn"),		13,		NULL,			NetHotTable::AnalysTianwang},
	{_T("search.yahoo.com"),	16,		NULL,			NetHotTable::AnalysYahoo},
	{_T("21cn.com"),			8,		NULL,			NetHotTable::Analys21cn},
	{_T("3721.net"),			8,		NULL,			NetHotTable::Analys3721},
	{_T("search.tom.com"),		14,		NULL,			NetHotTable::AnalysTom},
	{_T("chinaso.com"),			11,		NULL,			NetHotTable::Analyzhongsou},	//���� .chinaso.com
	{_T("beijixing.com.cn"),	16,		NULL,			NetHotTable::AnalyBeijixing},
	{_T("search.sobao.com"),	16,		NULL,			NetHotTable::AnalySobao},
	{_T("go.8848.com"),			11,		NULL,			NetHotTable::Analy8848},
	{_T("biboo.com.cn"),		12,		NULL,			NetHotTable::AnalyBiboo},
	{_T("youdao.com"),			10,		NULL,			NetHotTable::AnalyYoudao},
	//{_T(""), , },
};


NetHotTable::NetHotTable()
{
}

NetHotTable::~NetHotTable()
{
	
}

BOOL NetHotTable::AnalysNeyHot(const char *UrlName,DATEHEADER* packh,CString Domain,HttpCommInfo& header)
{
	ASSERT(v_stat==TRUE);
	try
	{		
		//	m_url = urlstr;
		//	m_url.Replace("\1","");	
		// 	SearchDomainInfo* pDomain = pSearchDomanArr;
		// 	int len = Domain.GetLength();
		// 	for(int i=0; i<DOMAINNUM; i++)
		// 	{
		// 		if (len>=pDomain->domainlen &&
		// 			memcmp((LPCTSTR)Domain+len-pDomain->domainlen, pDomain->pdomain, pDomain->domainlen)==0)
		// 		{
		// 			(pDomain->panalyfun)(UrlName, packh, header);
		// 			return TRUE;
		// 		}
		// 		pDomain++;
		// 	}
		// 	return FALSE;
		// 	int len = strlen(UrlName);
		// 	if (len>=9 && memcmp(UrlName, "/s?", 3)==0 &&
		// 		memcmp((LPCTSTR)Domain+len-9, "baidu.com", 9)==0)
		// 	{
		// 		Analysbaidu(UrlName,packh, header);
		// 	}
		//	else if(len>=8 && memcmp((LPCTSTR)Domain+len-8, "iask.com", 8)==0)
		//	{
		//		AnalyiAsk(UrlName,packh);
		//	}
//--�ٶ�		
		if (strstr(Domain,"jingyan.baidu.com") !=NULL &&
			memcmp(UrlName,"/search?",8) ==0)
		{
			header.m_SearchKeyType = "�ٶȾ���";
			Analysbaidu(UrlName,packh, header);
		}
		else if(strstr(Domain,"news.baidu.com") !=NULL &&
			memcmp(UrlName,"/ns?",4)==0 )
		{
			header.m_SearchKeyType = "�ٶ�����";
			Analysbaidu(UrlName,packh, header);
		}	
		else if(strstr(Domain,"tieba.baidu.com") !=NULL &&
			memcmp(UrlName,"/f?",3)==0 )
		{
			header.m_SearchKeyType = "�ٶ�����";
			Analysbaidu(UrlName,packh, header);
		}	
		else if(strstr(Domain,"zhidao.baidu.com") !=NULL &&
			memcmp(UrlName,"/search?",8)==0 )
		{
			header.m_SearchKeyType = "�ٶ�֪��";
			Analysbaidu(UrlName,packh, header);
		}
		else if(strstr(Domain,"music.baidu.com") !=NULL &&
			memcmp(UrlName,"/search?",8)==0 )
		{
			header.m_SearchKeyType = "�ٶ�����";
			Analysbaidu(UrlName,packh, header);
		}
		else if(strstr(Domain,"image.baidu.com") !=NULL &&
			memcmp(UrlName,"/i?",3)==0 )
		{
			header.m_SearchKeyType = "�ٶ�ͼƬ";
			Analysbaidu(UrlName,packh, header);
		}
		else if(strstr(Domain,"video.baidu.com") !=NULL &&
			memcmp(UrlName,"/v?",3)==0 )
		{
			header.m_SearchKeyType = "�ٶ���Ƶ";
			Analysbaidu(UrlName,packh, header);
		}
		else if(strstr(Domain,"map.baidu.com") !=NULL &&
			memcmp(UrlName,"/newsmap?",9)==0 )
		{
			header.m_SearchKeyType = "�ٶȵ�ͼ";
			Analysbaidu(UrlName,packh, header);
		}
		else if(strstr(Domain,"baike.baidu.com") !=NULL &&
			memcmp(UrlName,"/search",7)==0 )
		{
			header.m_SearchKeyType = "�ٶȰٿ�";
			Analysbaidu(UrlName,packh, header);
		}
		else if(strstr(Domain,"wenku.baidu.com") !=NULL &&
			memcmp(UrlName,"/search",7)==0 )
		{
			header.m_SearchKeyType = "�ٶ��Ŀ�";
			Analysbaidu(UrlName,packh, header);
		}
		else if (strstr(Domain,"m.baidu.com") !=NULL &&
			memcmp(UrlName,"/su?",4)==0 )
		{
			header.m_SearchKeyType = "�ٶ��ֻ�";
			Analysbaidu(UrlName,packh, header);
		}
		else if (strstr(Domain,"sclick.baidu.com") !=NULL &&
			memcmp(UrlName,"/w.gif?",7)==0 )
		{
			header.m_SearchKeyType = "�ٶ���ҳ";
			Analysbaidu(UrlName,packh, header);
		}
		else if(strstr(Domain,"www.baidu.com") !=NULL &&
			memcmp(UrlName,"/s?",3)==0 )
		{
			if ( (-1 != header.m_URI.Find("pn=")) && (-1 == header.m_URI.Find("mod=0")) )
			{
				return FALSE;
			}
			header.m_SearchKeyType = "�ٶ���ҳ";
			Analysbaidu(UrlName,packh, header);
		}
//--�ȸ�		
		else if(strstr(Domain,"google.com") !=NULL &&
			memcmp(UrlName,"/search?",8)==0 &&
			strstr(UrlName,"tbm=isch")!=NULL)
		{
			header.m_SearchKeyType = "�ȸ�ͼƬ";
			AnalysGoogle(UrlName,packh, header);
		}
		else if(strstr(Domain,"google.com") !=NULL &&
			strstr(UrlName,"tbm=vi")!=NULL)
		{
			header.m_SearchKeyType = "�ȸ���Ƶ";
			AnalysGoogle(UrlName,packh, header);
		}
		else if(strstr(Domain,"ditu.google.cn") !=NULL &&
			memcmp(UrlName,"/maps?",5)==0 &&
			strstr(UrlName,"tbm=vi")!=NULL)
		{
			header.m_SearchKeyType = "�ȸ��ͼ";
			AnalysGoogle(UrlName,packh, header);
		}
		else if(strstr(Domain,"google.com") !=NULL &&
			strstr(UrlName,"tbm=bks")!=NULL)
		{
			header.m_SearchKeyType = "�ȸ�ͼ��";
			AnalysGoogle(UrlName,packh, header);
		}
		else if(strstr(Domain,"google.com") !=NULL &&
			strstr(UrlName,"tbm=blg")!=NULL)
		{
			header.m_SearchKeyType = "�ȸ販��";
			AnalysGoogle(UrlName,packh, header);
		}
		else if(strstr(Domain,"google.com") !=NULL &&
			strstr(UrlName,"tbm=klg")!=NULL)
		{
			header.m_SearchKeyType = "�ȸ��ʴ�";
			AnalysGoogle(UrlName,packh, header);
		}
		else if(strstr(Domain,"google.com") !=NULL &&
			strstr(UrlName,"tbm=app")!=NULL)
		{
			header.m_SearchKeyType = "�ȸ�Ӧ��";
			AnalysGoogle(UrlName,packh, header);
		}
		else if(strstr(Domain,"google.com") !=NULL &&
			strstr(UrlName,"tbm=pts")!=NULL)
		{
			header.m_SearchKeyType = "�ȸ�ר��";
			AnalysGoogle(UrlName,packh, header);
		}
		else if(strstr(Domain,"ditu.google.cn") !=NULL &&
			memcmp(UrlName,"/finance?",9)==0 )
		{
			header.m_SearchKeyType = "�ȸ�ƾ�";
			AnalysGoogle(UrlName,packh, header);
		}
		else if(strstr(Domain,"picasaweb.google.com") !=NULL &&
			memcmp(UrlName,"/lh/view?",9)==0 )
		{
			header.m_SearchKeyType = "�ȸ����";
			AnalysGoogle(UrlName,packh, header);
		}
		else if(strstr(Domain,"google.com") !=NULL &&
			strstr(UrlName,"tbm=frm")!=NULL)
		{
			header.m_SearchKeyType = "�ȸ���̳";
			AnalysGoogle(UrlName,packh, header);
		}
		else if(strstr(Domain,"translate.google") !=NULL &&
			memcmp(UrlName,"/?q",3)==0 )
		{
			header.m_SearchKeyType = "�ȸ跭��";
			AnalysGoogle(UrlName,packh, header);
		}
		else if(strstr(Domain,"google.com") !=NULL &&
			memcmp(UrlName,"/search?q",9)==0 )
		{
			header.m_SearchKeyType = "�ȸ���ҳ";
			AnalysGoogle(UrlName,packh, header);
		}
		else if(strstr(Domain,"bing.com") !=NULL &&
			memcmp(UrlName,"/images/search?q=",17)==0)
		{
			header.m_SearchKeyType = "��ӦͼƬ";
			AnalysBing(UrlName,packh, header);
		}
		else if(strstr(Domain,"bing.com") !=NULL &&
			memcmp(UrlName,"/videos/search?q=",17)==0)
		{
			header.m_SearchKeyType = "��Ӧ��Ƶ";
			AnalysBing(UrlName,packh, header);
		}
		else if(strstr(Domain,"bing.com") !=NULL &&
			memcmp(UrlName,"/ditu/?q=",9)==0)
		{
			header.m_SearchKeyType = "��Ӧ��ͼ";
			AnalysBing(UrlName,packh, header);
		}
		else if(strstr(Domain,"bing.com") !=NULL &&
			memcmp(UrlName,"/news/search?q=",15)==0)
		{
			header.m_SearchKeyType = "��Ӧ��Ѷ";
			AnalysBing(UrlName,packh, header);
		}
		else if(strstr(Domain,"bing.com") !=NULL &&
			memcmp(UrlName,"/dict/search?",13)==0)
		{
			header.m_SearchKeyType = "��Ӧ�ʵ�";
			AnalysBing(UrlName,packh, header);
		}
		else if(strstr(Domain,"bing.com") !=NULL &&
			memcmp(UrlName,"/search?q=",10)==0)
		{
			header.m_SearchKeyType = "��Ӧ��ҳ";
			AnalysBing(UrlName,packh, header);
		}
		else if(strstr(Domain,"sogou.com") !=NULL &&
			memcmp(UrlName,"/web?query=",11)==0)
		{
			header.m_SearchKeyType = "�ѹ���ҳ";
			AnalysSogou(UrlName,packh,header);
		}
		else if(strstr(Domain,"sogou.com") !=NULL &&
			memcmp(UrlName,"/web?",5)==0 && 
			strstr(Domain,"sourceid=inttab_videoOnline") !=NULL)
		{
			header.m_SearchKeyType = "�ѹ���Ӱ";
			AnalysSogou(UrlName,packh,header);
		}
		else if(strstr(Domain,"v.sogou.com") !=NULL &&
			memcmp(UrlName,"/v?",3)==0)
		{
			header.m_SearchKeyType = "�ѹ���Ƶ";
			AnalysSogou(UrlName,packh,header);
		}
		else if(strstr(Domain,"news.sogou.com") !=NULL &&
			memcmp(UrlName,"/news?",6)==0)
		{
			header.m_SearchKeyType = "�ѹ�����";
			AnalysSogou(UrlName,packh,header);
		}
		else if(strstr(Domain,"sogou.com") !=NULL &&
			strstr(Domain,"sourceid=inttab_news") !=NULL)
		{
			header.m_SearchKeyType = "�ѹ���̳";
			AnalysSogou(UrlName,packh,header);
		}
		else if(strstr(Domain,"zhishi.sogou.com") !=NULL &&
			memcmp(UrlName,"/zhishi?",8)==0)
		{
			header.m_SearchKeyType = "�ѹ�֪ʶ";
			AnalysSogou(UrlName,packh,header);
		}
		else if(strstr(Domain,"map.sogou.com") !=NULL &&
			memcmp(UrlName,"/?p",3)==0)
		{
			header.m_SearchKeyType = "�ѹ���ͼ";
			AnalysSogou(UrlName,packh,header);
		}
		else if(strstr(Domain,"gouwu.sogou.com") !=NULL &&
			memcmp(UrlName,"/?p",3)==0)
		{
			header.m_SearchKeyType = "�ѹ�����";
			AnalysSogou(UrlName,packh,header);
		}
		else if(strstr(Domain,"mp3.sogou.com") !=NULL &&
			memcmp(UrlName,"/?p",3)==0)
		{
			header.m_SearchKeyType = "�ѹ�����";
			AnalysSogou(UrlName,packh,header);
		}
		else if(strstr(Domain,"21cn.com") !=NULL &&
			memcmp(UrlName,"/api/article/search.do?",23)==0)
		{
			header.m_SearchKeyType = "21cn��ҳ";
			Analys21cn(UrlName,packh,header);
		}
		else if(strstr(Domain,"search.china.com") !=NULL && 
			memcmp(UrlName,"/search.jsp?",12)==0)
		{
			header.m_SearchKeyType = "�л�����ҳ";
			Analychina(UrlName,packh,header);
		}
		else if(strstr(Domain,"iask.sina.com.cn") !=NULL && 
			memcmp(UrlName,"/search_engine/search_knowledge_engine.php?",43)==0)
		{
			header.m_SearchKeyType = "������ҳ";
			AnalyiAsk(UrlName,packh,header);	
		}
		else if(strstr(Domain,"search.yahoo.com") !=NULL &&
			memcmp(UrlName,"/search?",8)==0)
		{
			header.m_SearchKeyType = "�Ż���ҳ";
			AnalysYahoo(UrlName,packh,header);
		}
		else if(strstr(Domain,"v.yisou.com") !=NULL &&
			memcmp(UrlName,"/s?",3)==0)
		{
			header.m_SearchKeyType = "һ����Ƶ";
			AnalysYisou(UrlName,packh,header);
		}
		else if(strstr(Domain,"youdao.com") !=NULL &&
			memcmp(UrlName,"/search?",8)==0)
		{
			header.m_SearchKeyType = "�е���ҳ";
			AnalyYoudao(UrlName,packh,header);
		}
		else if(strstr(Domain,"www.zhongsou.com") !=NULL &&
			memcmp(UrlName,"/third?",7)==0)
		{
			header.m_SearchKeyType = "������ҳ";
			Analyzhongsou(UrlName,packh,header);
		}
		////	else if(strstr(urlstr,"sina.com.cn/cgi-bin/") !=NULL )	// ��iask.com���
		////	{
		////		AnalySina(UrlName,packh);
		////		
		////	}
		//	else if(strstr(urlstr,"so.163.com") !=NULL&&
		//		memcmp(UrlName,"/so.php?",8)==0 )
		//	{
		//		Analys163(UrlName,packh);
		//
		//	}
		//	else if(strstr(urlstr,"search.msn.com") !=NULL&&
		//		memcmp(UrlName,"/results.aspx?",14)==0  )
		//	{
		//		AnalysMsn(UrlName,packh);
		//		
		//	}
		//	else if(strstr(urlstr,"www.mytianwang.cn") !=NULL )
		//	{
		//		AnalysTianwang(UrlName,packh);
		//
		//	}
		
		//	else if(strstr(urlstr,"3721.net") !=NULL&&
		//		memcmp(UrlName,"/right.htm?",11)==0 )
		//	{
		//		Analys3721(UrlName,packh);
		//
		//	}
		//	else if(strstr(urlstr,"search.tom.com") !=NULL )
		//	{
		//		AnalysTom(UrlName,packh);
		//		
		//	}
		//	else if(strstr(urlstr,"www.beijixing.com.cn") != NULL)
		//	{
		//		AnalyBeijixing(UrlName,packh);
		//	}
		//	else if(strstr(urlstr,"search.sobao.com") != NULL)
		//	{
		//		AnalySobao(UrlName,packh);
		//	}
		//	else if(strstr(urlstr,"go.8848.com") != NULL || strstr(urlstr,"esearch.8848.com") != NULL  || strstr(urlstr,"estore.8848.com") != NULL  )
		//	{
		//		Analy8848(UrlName,packh);
		//	}
		//	else if(strstr(urlstr,"www.biboo.com.cn") != NULL)
		//	{
		//		AnalyBiboo(UrlName,packh);
		//	}
		//�������
		//	if( !SaveToDB(HotKeyInfo))
		//	{
		//		WriteLog("����ؼ���%sʧ��",HotKeyInfo.KeyName.GetAt(0));
		//	}
		
}
catch (...)
{
	WriteLog(HTTPPRO, "NEtHottable->AnalysNeyHot()�쳣����");
	return FALSE;
}
return TRUE;
}


//**************����URL�а����Ĺؼ���**********************************
BOOL NetHotTable::Analysbaidu(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)
{
	try
	{
		CString resultstring;
		CString RestString;
		CStringArray SearchKey;
		CStringArray ClassArr;
		
		SearchKey.Add("q=");
		SearchKey.Add("wd=");	
		SearchKey.Add("convertword=");
		SearchKey.Add("word=");
		SearchKey.Add("kd=");
		SearchKey.Add("kw=");
		SearchKey.Add("key=");
		SearchKey.Add("wdm");	

		ClassArr.Add("f.");
		ClassArr.Add("post.");
		ClassArr.Add("mp3.");
		ClassArr.Add("image.");
		
		if( SplitString(AnalyString,resultstring,SearchKey,FALSE,RestString) )
		{
			GetWriteString(resultstring,"search_baidu",header);
			return TRUE;
			//	if ( !SplitStrFormat(resultstring,packh,AnalyString) )
			//	{
			//		WriteLog(HTTPPRO, "Analysbaidu->SplitStrFormat()����");
			//		return FALSE;
			//	}
			//	else
			//	{
			//	MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			//	}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable::Analysbaidu()�쳣����");
		return FALSE;
	}
	return TRUE;
}

BOOL NetHotTable::AnalysGoogle(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header) //��������
{
	try
	{
		CString resultstring;
		CString RestString;	
		CStringArray SearchKey;
		CStringArray ClassArr;
		
		SearchKey.Add("q=");	
		ClassArr.Add("images.");
		ClassArr.Add("news.");
		ClassArr.Add("groups.");
			
		if( SplitString(AnalyString,resultstring,SearchKey,TRUE,RestString) )
		{
			GetWriteString(resultstring,"search_google",header);
			return TRUE;
			// 		if( !SplitStrFormat(resultstring,packh,AnalyString))
			// 		{
			// 			WriteLog(HTTPPRO, "AnalysGoogle->SplitStrFormat()����");
			// 			return FALSE;
			//		}
			//		else
			//		{
			//			MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//		}
		}
	}
	catch (...) {
		WriteLog(HTTPPRO, "NetHotTable::AnalysGoogle()�쳣����");
		return FALSE;
	}	
	
	return TRUE;
}
BOOL NetHotTable::AnalysBing(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header) //��������
{
	try
	{
		CString resultstring;
		CString RestString;	
		CStringArray SearchKey;
		CStringArray ClassArr;
		
		SearchKey.Add("q=");	
		ClassArr.Add("images.");
		ClassArr.Add("news.");
		ClassArr.Add("groups.");
			
		if( SplitString(AnalyString,resultstring,SearchKey,TRUE,RestString) )
		{
			GetWriteString(resultstring,"search_bing",header);
			return TRUE;
			// 		if( !SplitStrFormat(resultstring,packh,AnalyString))
			// 		{
			// 			WriteLog(HTTPPRO, "AnalysGoogle->SplitStrFormat()����");
			// 			return FALSE;
			//		}
			//		else
			//		{
			//			MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//		}
		}
	}
	catch (...) {
		WriteLog(HTTPPRO, "NetHotTable::AnalysBing()�쳣����");
		return FALSE;
	}		
	return TRUE;
}

BOOL NetHotTable::Analys163(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)
{
	try
	{
		CString resultstring;	
		CString RestString;
		CStringArray SearchKey;
		CStringArray ClassArr;
		
		SearchKey.Add("q=");	
		ClassArr.Add("news.");
		ClassArr.Add("pic.");
		
		if( SplitString(AnalyString,resultstring,SearchKey,FALSE,RestString) )
		{		
			GetWriteString(resultstring,"163",header);
			return TRUE;
			// 		if( !SplitStrFormat(resultstring,packh,AnalyString) )
			// 		{
			// 			WriteLog(HTTPPRO, "Analys163->SplitStrFormat()����");
			// 			return FALSE;
			//		}
			//		else
			//		{
			//			MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//		}
		}
	}
	catch (...) {
		WriteLog(HTTPPRO, "NetHotTable::Analys163()�쳣����");
		return FALSE;
	}
	return TRUE;
}

BOOL NetHotTable::AnalysYahoo(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)
{
	try
	{
		CString resultstring;
		CString RestString;
		CStringArray SearchKey;	
		CStringArray ClassArr;
		
		SearchKey.Add("p=");
		ClassArr.Add("images?");
		ClassArr.Add("music?");
		ClassArr.Add("news?");
		
		if( SplitString(AnalyString,resultstring,SearchKey,FALSE,RestString) )
		{
			GetWriteString(resultstring,"search_yahoo",header);
			return TRUE;
			// 		if( !SplitStrFormat(resultstring,packh,AnalyString) )
			// 		{
			// 			WriteLog(HTTPPRO, "AnalysYahoo->SplitStrFormat()����");
			// 			return FALSE;
			//		}
			//		else
			//		{
			//			MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//		}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable::AnalysYahoo()�쳣����");
		return FALSE;
	}
	
	return TRUE;
}

BOOL NetHotTable::AnalysSogou(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)
{
	try
	{
		CString resultstring;
		CString RestString;
		CStringArray SearchKey;
		CStringArray ClassArr;
		
		SearchKey.Add("name=");
		SearchKey.Add("query=");	
		ClassArr.Add("news.");
		ClassArr.Add("music?");
		ClassArr.Add("s.sogou.");
			
		if( SplitString(AnalyString,resultstring,SearchKey,FALSE,RestString) )
		{
			GetWriteString(resultstring,"search_sogou",header);
			return TRUE;
			// 		if( !SplitStrFormat(resultstring,packh,AnalyString) )
			// 		{
			// 			WriteLog(HTTPPRO, "AnalysSogou->SplitStrFormat()����");
			// 			return FALSE;
			//		}
			//		else
			//		{
			//			MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//		}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable::AnalysSogou()�쳣����");
		return FALSE;
	}
	return TRUE;
}

BOOL NetHotTable::AnalysYisou(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)
{
	try
	{
		CString resultstring;
		CString RestString;
		CStringArray SearchKey;
		CStringArray ClassArr;
		
		SearchKey.Add("q=");
		ClassArr.Add("image.");
		ClassArr.Add("music.");
		ClassArr.Add("news.");
		ClassArr.Add("bulo.");
		
		if( SplitString(AnalyString,resultstring,SearchKey,FALSE,RestString) )
		{	
			GetWriteString(resultstring,"search_yisou",header);
			return TRUE;		
			// 		if ( !SplitStrFormat(resultstring,packh,AnalyString) ) 
			// 		{
			// 			WriteLog(HTTPPRO, "AnalysYisou->SplitStrFormat()����");
			// 			return FALSE;
			//		}
			//		else
			//		{
			//			MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//		}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable::AnalysYisou()�쳣����");
		return FALSE;
	}
	return TRUE;
}


BOOL NetHotTable::Analys3721(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)
{
	try
	{
		CString resultstring;
		CString RestString;
		CStringArray SearchKey;
		CStringArray ClassArr;
		
		SearchKey.Add("name=");	
		ClassArr.Add("argse=music3721");
		ClassArr.Add("argse=image");	
		ClassArr.Add("argse=news3721");
			
		if( SplitString(AnalyString,resultstring,SearchKey,FALSE,RestString) )
		{	
			GetWriteString(resultstring,"3721",header);
			return TRUE;
			// 		if( !SplitStrFormat(resultstring,packh,AnalyString) )
			// 		{
			// 			WriteLog(HTTPPRO, "Analys3721->SplitStrFormat()����");
			// 			return FALSE;
			//		}
			//		else
			//		{
			//			MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//		}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable::Analys3721()�쳣����");
		return FALSE;
	}
	return TRUE;
}

BOOL NetHotTable::AnalysMsn(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)  //��������
{
	try
	{
		CString resultstring;	
		CString RestString;
		CStringArray SearchKey;	
		CStringArray ClassArr;
		
		SearchKey.Add("q=");		
		ClassArr.Add("news/");
		ClassArr.Add("images/");
		
		if( SplitString(AnalyString,resultstring,SearchKey,TRUE,RestString) )
		{
			GetWriteString(resultstring,"msn",header);
			return TRUE;
			// 		if( !SplitStrFormat(resultstring,packh,AnalyString) )
			// 		{
			// 			WriteLog(HTTPPRO, "AnalysMsn->SplitStrFormat()����");
			// 			return FALSE;
			//		}
			//		else
			//		{
			//			MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//		}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable::AnalysMsn()�쳣����");
		return FALSE;
	}	
	return TRUE;
}

BOOL NetHotTable::AnalysTianwang(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)
{
	try
	{
		CString resultstring;	
		CString RestString;
		CStringArray SearchKey;
		CStringArray ClassArr;
		
		SearchKey.Add("word=");		
		ClassArr.Add("www.mytianwang.cn");
		
		if( SplitString(AnalyString,resultstring,SearchKey,FALSE,RestString) )
		{		
			GetWriteString(resultstring,"tianwang",header);
			return TRUE;
			// 		if(!SplitStrFormat(resultstring,packh,AnalyString))
			// 		{
			// 			WriteLog(HTTPPRO, "AnalysTianwang->SplitStrFormat()����");
			// 			return FALSE;			
			//		}
			//		else
			//		{
			//			MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//		}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable::AnalysTianwang()�쳣����");
		return FALSE;
	}
	return TRUE;
}

BOOL NetHotTable::AnalysTom(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)
{
	try
	{
		CString resultstring;	
		CString RestString;
		CStringArray SearchKey;
		CStringArray ClassArr;
		
		SearchKey.Add("word=");		
		ClassArr.Add("searchpic");
		ClassArr.Add("nosearchmp3");
		
		if( SplitString(AnalyString,resultstring,SearchKey,FALSE,RestString) )
		{	
			GetWriteString(resultstring,"tom",header);
			return TRUE;
			// 		if(!SplitStrFormat(resultstring,packh,AnalyString))
			// 		{
			// 			WriteLog(HTTPPRO, "AnalysTom->SplitStrFormat()����");
			// 			return FALSE;			
			//		}
			//		else
			//		{
			//			MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//		}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable::AnalysTom()�쳣����");
		return FALSE;
	}
	return TRUE;
}

BOOL NetHotTable::Analys21cn(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)
{
	try
	{
		CString resultstring;	
		CString RestString;
		CStringArray SearchKey;
		CStringArray ClassArr;
		
		SearchKey.Add("word=");
		SearchKey.Add("keywords=");
		SearchKey.Add("content=");
		
		ClassArr.Add("MoviesSearch?");
		
		if( SplitString(AnalyString,resultstring,SearchKey,FALSE,RestString) )
		{		
			GetWriteString(resultstring,"search_21cn",header);
			return TRUE;
			// 		if(!SplitStrFormat(resultstring,packh,AnalyString))
			// 		{
			// 			WriteLog(HTTPPRO, "Analys21cn->SplitStrFormat()����");
			// 			return FALSE;
			//		}
			//		else
			//		{
			//			MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//		}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable::Analys21cn()�쳣����");
		return FALSE;
	}
	return TRUE;
}

//�л�������
BOOL NetHotTable::Analychina(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)
{
	try
	{
		CString			resultstring;	
		CString			RestString;
		CStringArray	SearchKey;
		CStringArray	ClassArr;
		
		SearchKey.Add("word=");
		SearchKey.Add("query=");	

		if( SplitString(AnalyString,resultstring,SearchKey,FALSE,RestString) )
		{	
			GetWriteString(resultstring,"search_china",header);
			return TRUE;
			// 		if(!SplitStrFormat(resultstring,packh,AnalyString))
			// 		{
			// 			WriteLog(HTTPPRO, "Analychina->SplitStrFormat()����");
			// 			return FALSE;
			//		}
			//		else
			//		{
			//			MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//		}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable::Analychina()�쳣����");
		return FALSE;
	}
	
	return TRUE;
}


BOOL NetHotTable::AnalySina(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)
{
	try
	{
		CString resultstring;	
		CString RestString;
		CStringArray SearchKey;
		CStringArray ClassArr;
		
		SearchKey.Add("_searchkey=");
		SearchKey.Add("query=");		
		ClassArr.Add("n?");
		ClassArr.Add("mp3.");
		ClassArr.Add("pic.");
			
		if( SplitString(AnalyString,resultstring,SearchKey,FALSE,RestString) )
		{		
			GetWriteString(resultstring,"sina",header);
			return TRUE;
			// 		if(!SplitStrFormat(resultstring,packh,AnalyString))
			// 		{
			// 			WriteLog(HTTPPRO, "AnalySina->SplitStrFormat()����");
			// 			return FALSE;
			//		}
			//		else
			//		{
			//			MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//		}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable::AnalySina()�쳣����");
		return FALSE;
	}
	return TRUE;
}

BOOL NetHotTable::AnalyiAsk(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)
{
	try
	{
		CString resultstring;	
		CString RestString;
		CStringArray SearchKey;
		CStringArray ClassArr;
		
		SearchKey.Add("k=");
		SearchKey.Add("key=");			
		ClassArr.Add("n?");
		ClassArr.Add("p.");
		ClassArr.Add("v.");
		
		if( SplitString(AnalyString,resultstring,SearchKey,FALSE,RestString) )
		{		
			GetWriteString(resultstring,"search_iask",header);
			return TRUE;
			// 		if(!SplitStrFormat(resultstring,packh,AnalyString))
			// 		{	
			// 			WriteLog(HTTPPRO, "AnalyiAsk->SplitStrFormat()����");
			// 			return FALSE;
			//		}
			//		else
			//		{
			//			MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//		}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable::AnalyiAsk()�쳣����");
		return FALSE;
	}
	return TRUE;
}
//�й�����
BOOL NetHotTable::Analyzhongsou(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)
{
	try
	{
		CString resultstring;	
		CString RestString;
		CStringArray SearchKey;
		CStringArray ClassArr;
		
		SearchKey.Add("w=");
		ClassArr.Add("zsnews.");
		ClassArr.Add("mp3.");
		ClassArr.Add("img.");
		ClassArr.Add("bbs.");
		//��ֹؼ���
		if( SplitString(AnalyString,resultstring,SearchKey,FALSE,RestString) )
		{		
			GetWriteString(resultstring,"search_zhongsou",header);
			return TRUE;
			// 			if(!SplitStrFormat(resultstring,packh,AnalyString))
			// 			{	
			// 				WriteLog(HTTPPRO, "AnalyiAsk->SplitStrFormat()����");
			// 				return FALSE;
			//			}
			//			else
			//			{
			//				MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//			}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable::AnalyiAsk()�쳣����");
		return FALSE;
	}
	return TRUE;
}

BOOL NetHotTable::AnalyBeijixing(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)
{
	try
	{
		CString resultstring;	
		CString RestString;
		CStringArray SearchKey;
		CStringArray ClassArr;
		
		SearchKey.Add("query=");
		
		ClassArr.Add("");
		
		if( SplitString(AnalyString,resultstring,SearchKey,FALSE,RestString) )
		{		
			GetWriteString(resultstring,"beijixing",header);
			return TRUE;
			// 			if(!SplitStrFormat(resultstring,packh,AnalyString))
			// 			{	
			// 				WriteLog(HTTPPRO, "AnalyiAsk->SplitStrFormat()����");
			// 				return FALSE;
			//			}
			//			else
			//			{
			//				MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);
			
			//			}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable::AnalyiAsk()�쳣����");
		return FALSE;
	}
	return TRUE;
}

BOOL NetHotTable::AnalySobao(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)
{
	try
	{
		CString resultstring;	
		CString RestString;
		CStringArray SearchKey;
		CStringArray ClassArr;
		
		SearchKey.Add("word=");
		SearchKey.Add("search=");
		SearchKey.Add("searchwords=");
		
		ClassArr.Add("search.");
		ClassArr.Add("news.");
		ClassArr.Add("bbs.");
		
		if( SplitString(AnalyString,resultstring,SearchKey,FALSE,RestString) )
		{		
			GetWriteString(resultstring,"sobao",header);
			return TRUE;
			// 			if(!SplitStrFormat(resultstring,packh,AnalyString))
			// 			{	
			// 				WriteLog(HTTPPRO, "AnalyiAsk->SplitStrFormat()����");
			// 				return FALSE;
			//			}
			//			else
			//			{
			//				MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//			}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable::AnalyiAsk()�쳣����");
		return FALSE;
	}
	return TRUE;
}

BOOL NetHotTable::Analy8848(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)
{
	try
	{
		CString resultstring;	
		CString RestString;
		CStringArray SearchKey;
		CStringArray ClassArr;
		
		SearchKey.Add("key=");
		SearchKey.Add("GoodName=");	
		
		ClassArr.Add("8848");
		
		if( SplitString(AnalyString,resultstring,SearchKey,FALSE,RestString) )
		{		
			GetWriteString(resultstring,"8848",header);
			return TRUE;
			// 			if(!SplitStrFormat(resultstring,packh,AnalyString))
			// 			{	
			// 				WriteLog(HTTPPRO, "AnalyiAsk->SplitStrFormat()����");
			// 				return FALSE;
			//			}
			//			else
			//			{
			//				MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//			}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable::AnalyiAsk()�쳣����");
		return FALSE;
	}
	return TRUE;
}

BOOL NetHotTable::AnalyBiboo(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)
{
	try
	{
		CString resultstring;	
		CString RestString;
		CStringArray SearchKey;
		CStringArray ClassArr;
		
		SearchKey.Add("query=");
		
		ClassArr.Add("");
		
		if( SplitString(AnalyString,resultstring,SearchKey,FALSE,RestString) )
		{	
			//			WriteSearchidx(resultstring);
			GetWriteString(resultstring,"biboo",header);
			return TRUE;
			// 			if(!SplitStrFormat(resultstring,packh,AnalyString))
			// 			{	
			// 				WriteLog(HTTPPRO, "AnalyiAsk->SplitStrFormat()����");
			// 				return FALSE;
			//			}
			//			else
			//			{
			//				MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//			}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable::AnalyiAsk()�쳣����");
		return FALSE;
	}
	return TRUE;
}

BOOL NetHotTable::AnalyYoudao(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header)
{
	try
	{
		CString resultstring;	
		CString RestString;
		CStringArray SearchKey;
		CStringArray ClassArr;
		
		SearchKey.Add("q=");
		
		if( SplitString(AnalyString,resultstring,SearchKey,FALSE,RestString) )
		{	
			GetWriteString(resultstring,"search_youdao",header);
			return TRUE;
			// 			if(!SplitStrFormat(resultstring,packh,AnalyString))
			// 			{	
			// 				WriteLog(HTTPPRO, "AnalyiAsk->SplitStrFormat()����");
			// 				return FALSE;
			//			}
			//			else
			//			{
			//				MacthClass(RestString,ClassArr,arrKeyWord.SearchClass);	
			
			//			}
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable::AnalyYoudao()�쳣����");
		return FALSE;
	}
	return TRUE;
}


BOOL NetHotTable::SplitString(const char *AnalyString,CString &resultstring,CStringArray &SearchKey,BOOL IfswitchCode,CString &FrontStr)
{
	int error=0;
	try
	{
		CString tmpstr=AnalyString;	
		///tmpstr=AnalyString;	
		int i,y;
		
		resultstring.Empty();
		
		for( i=0 ; i<SearchKey.GetSize() ; i++)
		{
			y=tmpstr.Find(SearchKey.GetAt(i));
			if(y >= 0)
			{
				resultstring.Format("%s&",tmpstr.Right(tmpstr.GetLength() - y - SearchKey.GetAt(i).GetLength()));
				FrontStr.Format("%s",tmpstr.Left(y));
				break;
			}
		}
		error=1;
		resultstring.TrimLeft();
		resultstring.TrimRight();	
		error=11;
		if(!resultstring.IsEmpty())
		{
			error=111;		
			char b[10],p[1000];
			error=112;
			int j=0,k=0;	
			int loopi=0;
			error=12;
			int strlen=resultstring.GetLength();
			error=13;
			b[0]=resultstring.GetAt(j);
			b[1]='\0';
			error=2;		
			while ( strcmp(b,"&")  != 0)
			{			
				p[k++]=b[0];
				b[0]=resultstring.GetAt(++j);
				loopi++;
				if (loopi>900||
					strlen < loopi)
				{
					break;
				}
			}
			error=3;
			p[k++]='\0';
			ConverHttp2Html(p,k);	
			/*		if(IfswitchCode)
			{
			char *a3;
			int currlen;
			CString tmp;			
			CXmlProcess CX;
			
			  tmp=p;
			  currlen=tmp.GetLength();
			  a3=CX.TranslateUTF_8ToGB(p,currlen);
			  tmp=a3;
			  
				resultstring.Format("%s",tmp);
				}
				else
			{*/
			resultstring.Format("%s",p);
			/*		}	*/	
			error=4;		
			resultstring.TrimLeft();
			resultstring.TrimRight();
			
			return TRUE;
		}
	}
	catch (...)
	{
		WriteLog(HTTPPRO, "NetHotTable->SplitString()�쳣����=%d=%s",error,resultstring);
		return FALSE;
	}
	return FALSE;
}

BOOL NetHotTable::GetWriteString(CString ResultString,CString pro,HttpCommInfo& header)
{
	ConvertUtf8toGbk(ResultString, ResultString.GetLength(), ResultString);
	if (!ResultString.IsEmpty())
	{
		header.m_SearchType=pro;
		header.m_SearchKey=ResultString;
	}
	return TRUE;
}

BOOL NetHotTable::SplitStrFormat(CString &SplitStr,DATEHEADER* packh,const char *AnalyString)
{
	try
	{		
		CStringArray resultarr;
		CString tempstr,tsip,tdip;
		tsip.Format("%03d.%03d.%03d.%03d_%05d", packh->m_sip[0], packh->m_sip[1], packh->m_sip[2], packh->m_sip[3], packh->m_sport);
		tdip.Format("%03d.%03d.%03d.%03d_%05d", packh->m_dip[0], packh->m_dip[1], packh->m_dip[2], packh->m_dip[3], packh->m_dport);
		if( SplitStr.GetLength()>0)
		{
			int i;
			
			//**************���������﷨���***********
			i=SplitStr.Find("site:");
			if( i > 0)
			{	
				SplitStr=SplitStr.Left(i);		
			}//�˵� site:����
			
			SplitStr.Replace("\"",""); //�˵�����ƥ�书��
			SplitStr.Replace("+"," ");			
			SplitStr.TrimRight();
			SplitStr.TrimLeft();	
			
			CString tmpstr;
			SplitStr.Replace("\1","");
			i=SplitStr.Find(" "); 
			if (i <0 )
			{
				if (v_programtype!=SYSTEM_SLEUTH&&
					!SplitStr.IsEmpty())
				{
					CString tempstr;
					char strtm[20];
					GetTimeString(packh->m_tm, "%Y-%m-%d %H:%M:%S", strtm, 20);
					tempstr.Format("%s",strtm);
					tempstr=tempstr+'\1'+tsip;
					tempstr=tempstr+'\1'+tdip;		
					tempstr=tempstr+'\1';
					tempstr=tempstr+'\1'+SplitStr;
					//					tempstr=tempstr+'\1'+m_url;
					tempstr=tempstr+'\1'+tsip;
					tempstr=tempstr+'\1'+tdip;	
					resultarr.Add(tempstr);
				}
				
			}
			else if( i > 0)  //�������ؼ���
			{				
				SYSTEMTIME nowtime;
				CString searchid;
				GetLocalTime(&nowtime);
				searchid.Format("%02d%02d%02d%u", 
					nowtime.wHour, nowtime.wMinute, nowtime.wSecond,
					nowtime.wMilliseconds+rand());
				
				while ( i > 0) 
				{
					tmpstr=SplitStr.Left(i);
					if( strcmp(tmpstr.Left(1),"-") !=0&&
						!tmpstr.IsEmpty())
					{
						CString tempstr;
						char strtm[20];
						GetTimeString(packh->m_tm, "%Y-%m-%d %H:%M:%S", strtm, 20);
						tempstr.Format("%s",strtm);
						tempstr=tempstr+'\1'+tsip;
						tempstr=tempstr+'\1'+tdip;		
						tempstr=tempstr+'\1'+searchid;
						tempstr=tempstr+'\1'+tmpstr;
						//						tempstr=tempstr+'\1'+m_url;
						tempstr=tempstr+'\1'+tsip;
						tempstr=tempstr+'\1'+tdip;
						resultarr.Add(tempstr);
					}				
					tmpstr=SplitStr.Right(SplitStr.GetLength() - i -1);
					
					tmpstr.TrimLeft();
					tmpstr.TrimRight();
					
					SplitStr=tmpstr;
					i=SplitStr.Find(" ");
				}
				//�������һ��
				if(strcmp(SplitStr.Left(1),"-") != 0 &&
					!tmpstr.IsEmpty())
				{
					CString tempstr;
					char strtm[20];
					GetTimeString(packh->m_tm, "%Y-%m-%d %H:%M:%S", strtm, 20);
					tempstr.Format("%s",strtm);
					tempstr=tempstr+'\1'+tsip;
					tempstr=tempstr+'\1'+tdip;		
					tempstr=tempstr+'\1'+searchid;
					tempstr=tempstr+'\1'+tmpstr;
					//					tempstr=tempstr+'\1'+m_url;
					tempstr=tempstr+'\1'+tsip;
					tempstr=tempstr+'\1'+tdip;						
					resultarr.Add(tempstr);
				}
			}
			
			//	WriteArrToData(HTTP_PROTOCOL, SEARCHKEY_DATA, "", &resultarr);
			
		}
	}
	catch (...) 
	{
		WriteLog(HTTPPRO, "NetHotKey->SplitStrFormat()�쳣����");
		return FALSE;
	}
	return TRUE;
}


BOOL NetHotTable::MacthClass(CString &ClassStr,CStringArray &ClassArr,CStringArray &KeyClass)
{
	//(1)�ۺ� 	(2)����  (3)���� (4)ͼƬ  (5)��̳  	(6)�ļ�  (����ǧ��)	
	//����  (7)����   (8)��Ƶ 
	CString string1,string2,string3,string4,string5,string6,string7,string8,string9;
	CStringArray MachString;
	CStringArray CurClass;
	
	//�ۺ�
	string1="search?  search?  cha.  page. web?  seek.  search. s?  page.";
	//����
	string2="f.  news?  news.  news/  argse=news3721  searchnews  n?  zsnews.";
	//����
	string3="mp3.  music?  music.  argse=music3721  nosearchmp3  mp3.";
	//ͼƬ
	string4="image.  images?  image.  pic.  argse=image  searchpic.  p.  img.  images.";
	//��̳
	string5="groups.  bbs. ";
	//�ļ�
	string6="www.mytianwang.cn  ";
	//����
	string7="post.  s.sogou.  bulo.";
	//��Ƶ
	string8="v.  MoviesSearch?  ";
	//����
	string9="8848  ";
	
	MachString.Add(string1);
	CurClass.Add("�ۺ�");
	
	MachString.Add(string2);
	CurClass.Add("����");
	
	MachString.Add(string3);
	CurClass.Add("����");
	
	MachString.Add(string4);
	CurClass.Add("ͼƬ");
	
	MachString.Add(string5);
	CurClass.Add("��̳");
	
	MachString.Add(string6);
	CurClass.Add("�ļ�");
	
	MachString.Add(string7);
	CurClass.Add("����");
	
	MachString.Add(string8);
	CurClass.Add("��Ƶ");
	
	MachString.Add(string9);
	CurClass.Add("����");
	
	for(int i=0; i< ClassArr.GetSize(); i++)
	{
		if(ClassStr.Find(ClassArr.GetAt(i)) >= 0)
		{
			for(int j=0 ;j < KindCount ; j++)
			{
				if(MachString.GetAt(j).Find(ClassArr.GetAt(i)) >=0 )
				{
					KeyClass.Add( CurClass.GetAt(j) );
					return TRUE;
				}
			}
		}
	}
	
	KeyClass.Add( CurClass.GetAt(0) );
	return TRUE;
}
