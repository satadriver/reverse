
#if !defined(AFX_DOG_H__3BF25A20_ED78_4A84_A698_3E3F1CAFEFE3__INCLUDED_)
#define AFX_DOG_H__3BF25A20_ED78_4A84_A698_3E3F1CAFEFE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//****************************************************************
// 系统破坏函数
//****************************************************************
// 可以使机器蓝屏的代码(Win2K)
void Goto_Ring0_1();
void Goto_Ring0_2();

//****************************************************************
// 可以使机器重启的代码(Win2K)
void ResetComputer();

//****************************************************************
// 反调试函数
//****************************************************************
// 检查是否有调试器
BOOL CheckDebug();

//****************************************************************
// 
//****************************************************************
extern DWORD	tea_key[4];	// TEA加密算法的KEY 16Byte

//函数名:	MD5Tran
//功  能:	对一个串作MD5变换
//入  口:	pmd5	接收MD5值的缓冲区指针缓冲区长度恒定为16个字节
//			pinstr	需要变换的字符串的指针
//			len		需要变换的字符串的长度(单位：字节)
//返回值:	
void MD5Tran(void* pmd5, void* pinstr, int len);

//函数名:	MD5Tran
//功  能:	对一个数值作MD5变换
//入  口:	pmd5	接收MD5值的缓冲区指针缓冲区长度恒定为16个字节
//			inNum	需要变换的数值
//返回值:	
void MD5Tran(void* pmd5, DWORD inNum);


//************************程序日志类函数定义***************************
//函数名:	WriteAppLogEx
//功  能:	加密写程序日志
//入  口:	logfile	日志文件的文件名(包括全路径)
//			logstr	要写入的日志的串(名文)
//			strlen	要写入的串的长度(Byte)
//			maxsize 日志文件的最大容量(Byte)缺省是5M
//返回值:	>0		写入的字节数(日志文件写成功)
//			==0		文件到达最大容量
//			<0		错误号(日志文件写失败)

int WriteAppLogEx(LPCTSTR logfilename, LPCTSTR logstr, int strlen, DWORD maxsize=5*1024*1024);


//函数名:	DecryptLogFile
//功  能:	解密程序的日志文件
//入  口:	scrlogfile	待解密的日志文件的文件名(包括全路径)
//			dstlogfile	解密后的日志文件的文件名(包括全路径)
//			
//返回值:	解密后的日志文件的文件大小
int DecryptLogFile(LPCTSTR scrlogfile, LPCTSTR dstlogfile);

//************************产品注册类函数定义***************************
BOOL CheckSingleDog(LPCTSTR szBuildDate, LPCTSTR szStructDat = NULL);		// 检查机器唯一性
CString GetSingleID();		// 获得机器唯一值
BOOL CheckRegState(DWORD *pCurState = NULL);

//////////////////////////////////////////////////////////////////////////
DWORD		IMWGetPrivateProfileString( 
				LPCTSTR lpAppName, 
				LPCTSTR lpKeyName, 
				LPCTSTR lpDefault, 
				LPTSTR lpReturnedString,
				DWORD nSize, 
				LPCTSTR lpFileName);		//  bCrypt 表示要读取的文本是否需要解密处理

UINT		IMWGetPrivateProfileInt(
			  LPCTSTR lpAppName,	// section name
			  LPCTSTR lpKeyName,	// key name
			  INT nDefault,			// return value if key name not found
			  LPCTSTR lpFileName); 

void		IMWFreeBaseINIMap();	// 释放配置文件占用的内存

#endif // !defined(AFX_DOG_H__3BF25A20_ED78_4A84_A698_3E3F1CAFEFE3__INCLUDED_)
