/*************  全局函数定义  ************/
#include "iphlpapi.h"
#include "winsock2.h"


#ifndef DCE_PUBLIC_FUNCTION_H__20050506__INCLUDED
#define DCE_PUBLIC_FUNCTION_H__20050506__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif

 void WriteLog(LPCTSTR module, LPCTSTR FmtTxt, ...);
 void ExitDce(int exitcode);
 void CheckAllServerState();
// void RefurServerPower(SERVERPOWER* pSvrPower, int svrtype);
 void CreateDir(CString dir);
//  void RenameTmpSubDir(LPCTSTR Ip,LPCTSTR subdir,LPCTSTR strServertype, LPCTSTR workpath);
 int  GetFileLength(const char * filename);
//  void InsertAuthToHashTable(NEWAAAPACKET* pAAAData);
//  void InsertProbeAuthToHashTable(NEWAAAPACKET* pAAAData);
 CString GetMacAddress(IPAddr ipDst,CString ip);
 void FormatMac(BYTE* pData, CString& smac, CString& dmac, DATEHEADER& packh);
 CString GetTimeString(time_t ttm,const char *format);
 int  GetTimeString(time_t ttm, LPCTSTR fmt, char* tmbuf, int buflen);
// void CopyFileToTmpdir(LPCTSTR CaseId, LPCTSTR filename, LPCTSTR indexfilename,
//					  CStringArray& attfiles, LPCTSTR Tm12, LPCTSTR subdir, SERVERPOWER* pCsPwr,
//					  SERVERPOWER* pIsPwr);
//  void CopyFileToTmpdir(LPCTSTR CaseId, LPCTSTR filename, LPCTSTR indexfilename,
// 								   CStringArray& attfiles, int Tm12, LPCTSTR subdir, SERVERPOWER* pCsPwr,
// 								   SERVERPOWER* pIsPwr,
// 								   CStringArray	&ruleidarr,CStringArray	&fenleiarr,CStringArray	&levelarr,
// 								   CStringArray	&cardarr,CStringArray	&namearr,CStringArray	&peopletypearr,
// 								   CStringArray	&peoplearr,CStringArray	&keytypearr,CStringArray &IsOnlyAlarmArr,
// 								   CStringArray &dummytypearr);


 void SendSpreadInfo(const char *InfoBuf, int bufLen);
 BOOL CheckIfTrueMail(const char * buff, int len);	//检查一封邮件是否是真正的邮件

 int DealRarFile(CString rarfilename, CString realfname, CStringArray& attarray, LPCTSTR ProType, CString& attflst);
int DealEmlFile(CString rarfilename, CString realfname, CStringArray& attarray, LPCTSTR ProType, CString& attflst,char* buf = NULL);

 CString GetExtName(const char* chFileName);
 CString NetBytestoAnsi(LPCTSTR strMessage, int CodePage = CP_ACP);
 CString ConverHttp2Html(CString strcontent);
// CString Getemlfield(IMessagePtr& iMsg);
// CString GetemlText(IMessagePtr& iMsg);
 CString Getemlheader(LPCTSTR emlfilename, char* buf, int bufsize);
BOOL WINAPI CheckImsgProxy(IMSGPROXY* imsgproxy);
// void SetImsgProxyFunction(PROCESSIMSGPROXYDATA pfunction);
 void GetAAAMapSize(int& c1, int& c2);
 BOOL GetHeapInfo(HANDLE heapH, DWORD& CommittedSize, DWORD& UnCommitedSize, int& BlockCount);
 BOOL WriteArrToData(int num, int type, CString str, CStringArray* Arrstr=NULL, LPCTSTR pqq=NULL);
 BOOL WriteQQGroup(CStringArray& dataarr);
 BOOL InitLanguageDice(const char* wordchar=NULL, int eWordRatio=5, int sWordRatio=3);
 LPCTSTR CheckSPL(CString strArticle);
 LPCTSTR CheckSPL(LPCTSTR strArticle, int len);
 void Splitstr(CString str ,CStringArray &array,char e);
//  BOOL WriteWarnAlarmArrToData(int num,CStringArray& fenleiarr,CStringArray& ruleidarr,CStringArray& peopletypearr,
// 				CStringArray& peoplearr,CStringArray& namearr,CStringArray& cardarr,CStringArray& keytypearr,
// 				CString spytm,CString state,CString pro,CStringArray& levelarr,CString begtm,CString endtm,
// 				CString spyaddr,CStringArray& dummytypearr);
 long GetZFreeDisk(int &zFreeSpace);

// 检查Z盘是否可用
 BOOL CheckZDrive();
	
 BOOL CreateFlgFile(LPCTSTR filename = NULL);

 BOOL ConvertUtf8toGbk(LPCTSTR cchString, int lenString, CString &strGbk);

BOOL UnRarDbf(LPCTSTR rarfile, LPCTSTR undir, int wait);

BOOL BakProData(int proidx, DATEHEADER& packh,const char *pPacket);

std::string GetStringFromHexString(const unsigned char * str, int length);

std::string formatstr(const char* fm, ...);

std::string formatstr_lit(const char* fm, ...);

 /*! \fn BOOL  CheckIfExcludeChinese(char *str,int len)   
	\brief	判断字符串是否不包含汉字
	\param	[IN]*str 要判断的字符串
	\param	[IN]len  字符串长度
	\return	如果不包含,函数返回TRUE,否则函数返回FALSE

*/
BOOL  CheckIfExcludeChinese(char *str,int len);
class CChineseCodeLib
{
public:
	//UTF-8 转为 GB2312
	void CChineseCodeLib::UTF_8ToGB2312(CString &pOut, char *pText, int pLen);
	//GB2312 转为 UTF-8
	void CChineseCodeLib::GB2312ToUTF_8(CString& pOut,char *pText, int pLen);
	// GB2312 转换成　Unicode
	void CChineseCodeLib::Gb2312ToUnicode(WCHAR* pOut,char *gbBuffer);
	// 把Unicode 转换成 GB2312 
	void CChineseCodeLib::UnicodeToGB2312(char* pOut,unsigned short uData);
	// Unicode 转换成UTF-8 
	void CChineseCodeLib::UnicodeToUTF_8(char* pOut,WCHAR* pText);
	  // 把UTF-8转换成Unicode
	void CChineseCodeLib::UTF_8ToUnicode(WCHAR* pOut,char *pText);

	// GBK =〉GB2312
// 中華人民共和國 --> 中华人民共和国
	void GBK2GB(char *szBuf);

	// GB2312 => GBK
	// 中华人民共和国 --> 中華人民共和國
	void GB2GBK(char *szBuf);

	// GBK => Big5
	// 中華人民共和國 --> い地チ㎝瓣
	void GBK2BIG5(char *szBuf);
	
	// Big5 => GBK：
	// い地チ㎝瓣 --> 中華人民共和國
	void BIG52GBK(char *szBuf);

	//GB2312=>big5
	void GBBIG5(char *szBuf);
	
	void BIG5GB(char *szBuf);

protected:
private:
};
// typedef int ( *LPWRITEAPPLOGEX) (LPCTSTR logfilename, LPCTSTR logstr, int strlen, DWORD maxsize);
// 
// extern  LPWRITEAPPLOGEX v_pWriteLogFun;

// inline Function
inline void WriteIndexfileField(FILE*fp, LPCTSTR field, int iValue)
{
	fprintf(fp, "%s= %d\r\n", field, iValue);
}

inline void WriteIndexfileField(FILE*fp, LPCTSTR field, LPCTSTR sValue)
{
	if (sValue[0]!=0)	// 等效于(sValue != "")
	{
		fprintf(fp, "%s= %s\r\n", field, sValue);
	}
}

inline int WriteField(char* pbuf, LPCTSTR field, int iValue)
{
	return sprintf(pbuf, "%s= %d\r\n", field, iValue);
}

inline int WriteField(char* pbuf, LPCTSTR field, LPCTSTR sValue)
{
	if (sValue[0]!=0)	// 等效于(sValue != "")
	{
		return sprintf(pbuf, "%s= %s\r\n", field, sValue);
	}
	else
	{
		return 0;
	}
}

inline LPCTSTR GetShortName(LPCTSTR filename)
{
	char *p = strrchr(filename, '\\');
	if (p!=NULL)
		return p+1;
	else
		return filename;
}

inline CString GetPath(LPCTSTR filename)
{
	CString path;
	char *p = strrchr(filename, '\\');
	if (p != NULL)
	{
		path = CString(filename, p - filename + 1);
	}
	return path;
}

//**********************************************************
//	交换2个字节的顺序
//**********************************************************
inline WORD Swap2Byte(WORD dat)
{
//#ifdef		_DEBUG
	WORD rdat;
//#endif
//	__asm mov	edx, pdat
//	__asm mov	ax, word ptr [edx]
	__asm mov	ax, dat
	__asm bswap	eax
	__asm shr	eax, 16
//	__asm ret
//	__asm mov	word ptr [edx], ax
//#ifdef		_DEBUG
	__asm mov	rdat, ax
	return rdat;
//#endif

}

//**********************************************************
//	交换4个字节的顺序
//**********************************************************
inline DWORD Swap4Byte(DWORD dat)
{
//#ifdef		_DEBUG
	DWORD rdat;
//#endif
	//	__asm mov	edx, pdat
//	__asm mov	eax, dword ptr [edx]
	__asm mov	eax, dat
	__asm bswap	eax
//	__asm ret
//	__asm mov	dword ptr [edx], eax
//#ifdef		_DEBUG
	__asm mov	rdat, eax
	return rdat;
//#endif
}

static LPCTSTR DataSrcArr[DATASRC_OTHER] = 
{
	_T("IDC"),
	_T("CDMA"),
	_T("GPRS")
};

//*************************************************
//	函数名称:			GetDataSrc
//	描述:				获取数据源的文本
//	输入参数:			数据源的值
//	输出参数:			
//	返回值:				数据源的文本
//	其它:				
//*************************************************
inline LPCTSTR GetDataSrc(DWORD dwDatasrc)
{
	if (v_programtype == SYSTEM_FYY)
	{
		return _T("nobusiness");
	}
	else if (v_programtype == SYSTEM_ZONE)
	{
		return _T("zone");
	}
	else if (v_programtype == SYSTEM_HOTEL)
	{
		return _T("hotel");
	}
	else if (v_programtype == SYSTEM_DEPARTMENT)
	{
		return _T("department");
	}	
	else if (v_programtype == SYSTEM_ZK)
	{
		return _T("zk");
	}
	else if (v_programtype == SYSTEM_TELEWIRELESS)
	{
		return _T("telewireless");
	}
	else if (v_programtype == SYSTEM_UNICOMWIRELESS)
	{
		return _T("unicomwireless");
	}
	else if(dwDatasrc >= DATASRC_OTHER)
	{
		return _T("其他");
	}
	else
	{
		return DataSrcArr[dwDatasrc];
	}
}
inline LPCTSTR GetDataSrcDword(DWORD dwDatasrc)
{
	if (v_programtype == SYSTEM_FYY)
	{
		return "4";
	}
	else if (v_programtype == SYSTEM_ZONE)
	{
		return "4";
	}
	else if (v_programtype == SYSTEM_HOTEL)
	{
		return "2";
	}
	else if (v_programtype == SYSTEM_DEPARTMENT)
	{
		return "3";
	}	
	else if (v_programtype == SYSTEM_ZK)
	{
		return "3";
	}
	else if (v_programtype == SYSTEM_TELEWIRELESS)
	{
		return "5";
	}
	else if (v_programtype == SYSTEM_UNICOMWIRELESS)
	{
		return "6";
	}
	return "7";
}
std::string UrlDecode(const std::string& src);
char checkfiletype(CString filename,CString houzhui);

#endif