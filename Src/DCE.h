
#if !defined(AFX_DCE_H__6D1B1909_E86F_4308_BDB8_726BEE03DD5A__INCLUDED_)
#define AFX_DCE_H__6D1B1909_E86F_4308_BDB8_726BEE03DD5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 函数定义
void ProcessCmdLine(int argc, char **argv);		// 处理命令行参数
int GetLocalHostIP(int& group, int& machine);	// 获得本机IP
int	ReadDdeHost(int ddegroup, int dcemachine, DDELIST& ddelist);
//int	ReadDceHost(DCELIST& dcelist);
int	ReadDseServerHost();
int ReadParaServerHost();
BOOL UpdatePara();								// 更新参数
BOOL Inipubvar();
void ReadHttpurl();

#ifdef		ADD_RUNSTATE_INFO
void DumpRunState(DWORD pro);
#endif

#define GPRSBASICINFO "e:\\netspy\\para\\wcdma.txt"
// 处理基站号对照信息
typedef struct  BasicInfoStruct		// 整合时每个ip对应的文件句柄和文件个数
{
	CString     name;				// 基站名
	CString		address;			// 基站位置
	CString     longitude;			// 经度
	CString     latitude;			// 纬度
}BasicInfoStruct,*LPBasicInfoStruct;

typedef CMap<CString, LPCTSTR, BasicInfoStruct, BasicInfoStruct&> Basicinfomap;

extern Basicinfomap m_basicinfomap;

#endif // !defined(AFX_DCE_H__6D1B1909_E86F_4308_BDB8_726BEE03DD5A__INCLUDED_)
