// NetHotTable.h: interface for the NetHotTable class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_NETHOTTABLE_H__8740255A_AC11_461A_8FBC_A64624F9B6D9__INCLUDED_)
#define AFX_NETHOTTABLE_H__8740255A_AC11_461A_8FBC_A64624F9B6D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//定义热点关键字结构类型
//struct S_NetHotKey 
//{
//	CStringArray KeyName;
//	CString SearchName;
//	CStringArray SearchClass;
//};

class NetHotTable  
{
public:
//	NetHotTable(/*CString workpath,CString dbfname*/);
	NetHotTable();
	~NetHotTable();

	//取得搜索引擎基本域名
//	BOOL GetSearchUrl(CStringArray &SearchUrl);
	//处理网络热点关键字
	BOOL AnalysNeyHot(const char *UrlName,DATEHEADER* packh,CString Domain,HttpCommInfo& header);
//*****************每种搜索对应一个处理函数***************************
	//百度
	BOOL Analysbaidu(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//google
	BOOL AnalysGoogle(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//bing
	BOOL AnalysBing(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//一搜
	BOOL AnalysYisou(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//网易搜索
	BOOL Analys163(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//搜狐
	BOOL AnalysSogou(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//MSN搜索
	BOOL AnalysMsn(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//天网千帆文件搜索
	BOOL AnalysTianwang(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//雅虎
	BOOL AnalysYahoo(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//21cn搜索
	BOOL Analys21cn(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//3721搜索
	BOOL Analys3721(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//TOM搜索 11
	BOOL AnalysTom(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//中华网搜索
	BOOL Analychina(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//新浪搜索
	BOOL AnalySina(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//爱问
	BOOL AnalyiAsk(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//中国搜索
	BOOL Analyzhongsou(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//北极星网站搜索
	BOOL AnalyBeijixing(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//搜豹 18  
	BOOL AnalySobao(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//8848
	BOOL Analy8848(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//biboo
	BOOL AnalyBiboo(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//youdao
	BOOL AnalyYoudao(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	
//*******************数据库操作*************************
//	BOOL CreateTable();
//	BOOL OpenNetHotKeyDB();
//	BOOL SaveToDB(S_NetHotKey &arrSaveKey);
// 	BOOL CloseNetHotKeyDB();

//拆分出关键字
	BOOL SplitString(const char *AnalyString,CString &resultstring,CStringArray &SearchKey,BOOL IfswitchCode,CString &FrontStr);
//在关键字串基础上,根据搜索语法细化关键字拆分
	BOOL SplitStrFormat(CString &SplitStr,DATEHEADER* packh,const char *AnalyString);
//取类别
	BOOL MacthClass(CString &ClassStr,CStringArray &ClassArr,CStringArray &KeyClass);
	BOOL GetWriteString(CString ResultString,CString pro,HttpCommInfo& header);
private:	
//	CString m_workpath;	
//	CString m_NetHotdbfname;
//	CString m_NetHotcdxname;
//	CString m_url;
};

#endif // !defined(AFX_NETHOTTABLE_H__8740255A_AC11_461A_8FBC_A64624F9B6D9__INCLUDED_)
