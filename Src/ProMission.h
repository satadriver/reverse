// ProMission.h: interface for the ProMission class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROMISSION_H__C7EC12BC_F0B7_45DB_BDCF_2DE5239604C6__INCLUDED_)
#define AFX_PROMISSION_H__C7EC12BC_F0B7_45DB_BDCF_2DE5239604C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>


// 整合文件用的哈希单元定义
typedef struct  _IpDataStruct_		// 整合时每个ip对应的文件句柄和文件个数
{
	int			m_filenum;			// 文件数
	DWORD		m_realflen;			// 实体文件长度
// 	UINT		m_type;				// 类型(0: 回溯，1: 案件，2:预警，其他值错误)
// 	CString		m_CidWType;			// 案件ID(type==1)或预警分类(type==2)或空串(type==0)
// 	CString		m_pro;				// 协议
	CString		m_indexfname;		// 对照文件名
	CString		m_realfname;		// 实体文件名
	FILE*		m_prealfile;		// 对照文件指针
	FILE*		m_pindexfile;		// 实体文件指针
}IpDataStruct,*LPIpDataStruct;

inline void AFXAPI DestructElements(IpDataStruct* ppElements, int nCount)
{
	if (ppElements->m_pindexfile != NULL)
	{
		fclose(ppElements->m_pindexfile);	// 关闭整合的对照文件
	}
	if (ppElements->m_prealfile != NULL)
	{
		fclose(ppElements->m_prealfile);	// 关闭整合的实体文件
	}
}

class CDseFileMap : public CMap<CString, LPCTSTR, IpDataStruct, IpDataStruct&>
{
public:
	BOOL LookupEx(CString key, LPIpDataStruct& pValue)
	{
		ASSERT_VALID(this);

		UINT nHash;
		CAssoc* pAssoc = GetAssocAt(key, nHash);
		if (pAssoc == NULL)
			return FALSE;  // not in map

		pValue = &pAssoc->value;
		return TRUE;
	}
};



// 协议对象类定义
class DceProcotol
{
public:
	DceProcotol();
	virtual	~DceProcotol();
public:
// 	int				m_nowstm;		// 文件输出的当前时间目录(00 - 59)
 	LPCTSTR			m_ddefiletype;	// 处理dde文件的类型名称
//	LPCTSTR*		m_outdirlist;	// 数据输出的路径列表（协议列表）
	UINT			m_HashSize;		// Socket哈希表的宽度
	DWORD			m_PacketFrom;	// 当前处理包的类型
	LPIpDataStruct	m_pDseFileInfo;	// DSE输出文件信息结构数组指针
	int				m_DseInfoCount;	// DSE输出文件信息结构大(3的倍数)
	CString			m_IndexFileData;// 对照文件的数据(用于流输出)
	CStringArray	m_TmpFileArr;	// 当时没有提交成功的文件名
public:
	virtual BOOL InitObject(DceMission* pMiss);
	virtual BOOL ProcessPacket(DATEHEADER& dataheader,const char *pPacket, LPCTSTR spyaddr)
	{
		return TRUE;
	}
	virtual void ProcessBuffer(DATABUFFER& packetbuf, time_t& packtm);
	virtual void RenameTmpDir();
	virtual BOOL WriteObjectState(time_t packtm, CString& strtxt){return FALSE;}

	// 输出文件(整合、分配和输出)
	void OutPutFile(LPCTSTR pro,			// 协议
					UINT type,				// 类型(0: 回溯，1: 案件，2:预警，其他值错误)
					LPCTSTR CIDWType,		// 案件ID(type==1)或预警分类(type==2)或空串(type==0)
					CString indexfname,
					CString realfname,
					CStringArray& attfiles,
					LPCTSTR filekey,		// 数据分配时的KEY(此时的KEY不包含pro和CIDWType)
					CString AlarmText= ""		// 该数据的告警文本
					);

	// 输出文件到流()
	void OutPutFileToStream(LPCTSTR pro,	// 协议
					UINT type,				// 类型(0: 回溯，1: 案件，2:预警，其他值错误)
					LPCTSTR CIDWType,		// 案件ID(type==1)或预警分类(type==2)或空串(type==0)
					CString indexfname,
					CString realfname,
					CStringArray& attfiles,
					LPCTSTR filekey,		// 数据分配时的KEY(此时的KEY不包含pro和CIDWType)
//					CString AlarmText= "",	// 该数据的告警文本
					UINT	id = 1			// 该数据输出的次数(一条数据匹配了多个线索)
					);

	// 输出文件到服务器目录
	void CopyFileToServer(	LPCTSTR indexfname,			// 对照文件
							LPCTSTR realfname,			// 实体文件
							CStringArray& attfile,		// 附件文件
							CString DstDir,				// 目的路径
							BOOL isDelSrcFile			// 是否删除源文件
							);

	// 获取数据的输出路径
	CString GetOutDir(LPCTSTR pro,			// 协议
					UINT type,				// 类型(0: 回溯，1: 案件，2:预警，其他值错误)
					LPCTSTR CIDWType,		// 案件ID(type==1)或预警分类(type==2)或空串(type==0)
					UINT hashval,			// 数据分配的因子
					int* pSvrIdx = NULL		// 分配给的服务器ID(仅认证协议中用)
						);

	void OutPutAlarm(CString DstDir,		// 目的路径
					CString AlarmText		// 告警文本
					);

	LPIpDataStruct AllocFileToDse(UINT DataType,	// 数据类型(0回溯, 1案件 2预警)
					UINT KeyHash					// 数据哈希
					);

// 	void RenameTmpSubDir(LPCTSTR Ip,		// Dse服务器IP
// 					LPCTSTR strpro,			// 协议名
// 					LPCTSTR SvrType			// 服务器类型
// 					);

	// 文件整合失败就返回FALSE
	BOOL MergeFile(LPIpDataStruct pDstFile,	// 目的文件信息结构
					LPCTSTR indexfname,		// 对照文件
					LPCTSTR realfname		// 实体文件
					);

	// 整合文件到流
	BOOL MergeFileToStream(LPIpDataStruct pDstFile,	// 目的文件信息结构
					DceFileStream* pDceStream,
					LPCTSTR srcfname,		// 原文件
					LPCTSTR	strnum				// 文件名的替换串
					);

	// 整合内存数据到流
	BOOL MergeMemDataToStream(LPIpDataStruct pDstFile,	// 目的文件信息结构
					DceFileStream* pDceStream,
					LPCTSTR	pfdata,						// 内存数据(srcfname=NULL时有效)
					int		fdatalen,					// 内存数据长度
					LPCTSTR dstfname					// 写到流里的文件名
					);

// 	void OutPutFileToStream(LPCTSTR pro,	// 协议
// 					UINT type,				// 类型(0: 回溯，1: 案件，2:预警，其他值错误)
// 					LPCTSTR CIDWType,		// 案件ID(type==1)或预警分类(type==2)或空串(type==0)
// 					CString indexfname,
// 					CString realfname,
// 					CStringArray& attfiles,
// 					LPCTSTR filekey,		// 数据分配时的KEY(此时的KEY不包含pro和CIDWType)
// 					CString AlarmText= "",	// 该数据的告警文本
// 					UINT	id = 1			// 该数据输出的次数(一条数据匹配了多个线索)
// 					);
};
#endif // !defined(AFX_PROMISSION_H__C7EC12BC_F0B7_45DB_BDCF_2DE5239604C6__INCLUDED_)
