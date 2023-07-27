// HardAllInfo.h: interface for the HardAllInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HARDALLINFO_H__0566EEE0_0213_4C7B_B33E_2C985980CF20__INCLUDED_)
#define AFX_HARDALLINFO_H__0566EEE0_0213_4C7B_B33E_2C985980CF20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
#define SUPPORT_MAC			"d:\\netspy\\bin\\ZwCardId.dll"
#define SUPPORT_HARDID		"d:\\netspy\\ini\\Struct.dat"
#define DOG_BASEINI			"d:\\netspy\\ini\\local.ini"
#define ZW_TIMESTAMP		"d:\\netspy\\bin\\ZwCode.dll"
#define ZW_TIMESTAMP_SRC	"d:\\netspy\\bin\\zwCardid.dll"
#define THM_CHECKLOCAL	WM_USER+0x1101
#define THM_CHECKNTP	WM_USER+0x1102
#define THM_CHECK200OK	WM_USER+0x1103
#define THM_EXIT		WM_USER+0x1104

#define CHECK_DEFAULT		0			// 未决状态, 程序运行并于写日志时检查时间
#define CHECK_OK			1			// 正常状态, 程序运行并于 截获中分析200OK
#define CHECK_ERR_OPENREG		2		// 异常, 打开许可文件失败
#define CHECK_ERR_HDRHASH		3		// 异常, 许可文件头Hash失败
#define CHECK_ERR_VER			4		// 异常, 许可文件版本与程序版本不符
#define CHECK_ERR_REGTYPE		5		// 异常, 注册码失败(正式版但起始时间不为0或试用版起始时间为0)
#define CHECK_ERR_FILESIZE		6		// 异常, 期待大小与实际不符
#define CHECK_ERR_BLOCK			7		// 异常, 遇到屏蔽项(虚拟机)
#define CHECK_ERR_GETREGCARD	8		// 异常, 无法获得任何Mac
#define CHECK_ERR_GETREGDISK	9		// 异常, 无法获得任何硬盘
#define CHECK_ERR_GETSN			10		// 异常, 由许可信息获得许可码失败
#define CHECK_ERR_HARDID		11		// 异常, 许可文件未找到本机许可码
#define CHECK_ERR_FTRYNOW		12		// 异常, 试用版本机时间不在许可范围内
#define CHECK_ERR_FLOCAL		13		// 异常, 由本机标志文件检查许可时间失败
#define CHECK_ERR_FNTP			14		// 异常, 由NTP时间服务器检查许可时间失败
#define CHECK_ERR_F200OK		15		// 异常, 由200OK检查许可时间异常(超10个服务器)
#define CHECK_ERR_FREGNOW		16		// 异常, 正式版程序编译时间大于免费升级截止时间
#define CHECK_ERR_NOVER		17		// 异常, 无附加版本信息
#define CHECK_ERR_NOCUS		18		// 异常, 无附加客户信息

//////////////////////////////////////////////////////////////////////////
extern CString	g_strRegDat;		// 许可信息文件
extern DWORD	g_checkTmTID;		// 检查时间线程ID
extern HANDLE	g_checkHandle;		// 检查时间线程句柄
extern time_t	*g_pcheckRegBegin;	// 起始(==0正式版)
extern time_t	*g_pcheckRegEnd;	// 结束
extern DWORD	*g_pCheckState;		// 状态, 值为CHECK_系列宏

//////////////////////////////////////////////////////////////////////////
DWORD WINAPI CheckTmThread(LPVOID p);	// 线程函数
BOOL WriteCheckError(DWORD checkErr);	// 写错误状态

//////////////////////////////////////////////////////////////////////////
// 获得用户列表
BOOL	GetUserInfo(CString &Info);
// 获得真实Mac列表
int		GetRegeditKey(CString &arrInfo);
// 反调试,禁附加
bool	InstallAntiAttach();
// 获得许可信息base64串
CString Res_QueryHardInfo(LPCTSTR *pFields, LPCTSTR *pValues, int count);
// 据许可信息获得许可码
bool GetExeAttInfo(CStringArray &arrCus, CString& strVer);
BOOL	GetSN(CStringArray &arrAtt, const char *systemstring, const char *baseString, 
		   unsigned char snReg[16], unsigned char snTry[16]);

#endif // !defined(AFX_HARDALLINFO_H__0566EEE0_0213_4C7B_B33E_2C985980CF20__INCLUDED_)
