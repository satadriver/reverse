// NetHotTable.h: interface for the NetHotTable class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_NETHOTTABLE_H__8740255A_AC11_461A_8FBC_A64624F9B6D9__INCLUDED_)
#define AFX_NETHOTTABLE_H__8740255A_AC11_461A_8FBC_A64624F9B6D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//�����ȵ�ؼ��ֽṹ����
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

	//ȡ�����������������
//	BOOL GetSearchUrl(CStringArray &SearchUrl);
	//���������ȵ�ؼ���
	BOOL AnalysNeyHot(const char *UrlName,DATEHEADER* packh,CString Domain,HttpCommInfo& header);
//*****************ÿ��������Ӧһ��������***************************
	//�ٶ�
	BOOL Analysbaidu(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//google
	BOOL AnalysGoogle(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//bing
	BOOL AnalysBing(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//һ��
	BOOL AnalysYisou(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//��������
	BOOL Analys163(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//�Ѻ�
	BOOL AnalysSogou(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//MSN����
	BOOL AnalysMsn(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//����ǧ���ļ�����
	BOOL AnalysTianwang(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//�Ż�
	BOOL AnalysYahoo(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//21cn����
	BOOL Analys21cn(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//3721����
	BOOL Analys3721(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//TOM���� 11
	BOOL AnalysTom(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header); 
	//�л�������
	BOOL Analychina(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//��������
	BOOL AnalySina(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//����
	BOOL AnalyiAsk(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//�й�����
	BOOL Analyzhongsou(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//��������վ����
	BOOL AnalyBeijixing(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//�ѱ� 18  
	BOOL AnalySobao(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//8848
	BOOL Analy8848(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//biboo
	BOOL AnalyBiboo(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	//youdao
	BOOL AnalyYoudao(const char *AnalyString,DATEHEADER* packh,HttpCommInfo& header);
	
//*******************���ݿ����*************************
//	BOOL CreateTable();
//	BOOL OpenNetHotKeyDB();
//	BOOL SaveToDB(S_NetHotKey &arrSaveKey);
// 	BOOL CloseNetHotKeyDB();

//��ֳ��ؼ���
	BOOL SplitString(const char *AnalyString,CString &resultstring,CStringArray &SearchKey,BOOL IfswitchCode,CString &FrontStr);
//�ڹؼ��ִ�������,���������﷨ϸ���ؼ��ֲ��
	BOOL SplitStrFormat(CString &SplitStr,DATEHEADER* packh,const char *AnalyString);
//ȡ���
	BOOL MacthClass(CString &ClassStr,CStringArray &ClassArr,CStringArray &KeyClass);
	BOOL GetWriteString(CString ResultString,CString pro,HttpCommInfo& header);
private:	
//	CString m_workpath;	
//	CString m_NetHotdbfname;
//	CString m_NetHotcdxname;
//	CString m_url;
};

#endif // !defined(AFX_NETHOTTABLE_H__8740255A_AC11_461A_8FBC_A64624F9B6D9__INCLUDED_)
