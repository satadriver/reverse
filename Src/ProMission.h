// ProMission.h: interface for the ProMission class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROMISSION_H__C7EC12BC_F0B7_45DB_BDCF_2DE5239604C6__INCLUDED_)
#define AFX_PROMISSION_H__C7EC12BC_F0B7_45DB_BDCF_2DE5239604C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>


// �����ļ��õĹ�ϣ��Ԫ����
typedef struct  _IpDataStruct_		// ����ʱÿ��ip��Ӧ���ļ�������ļ�����
{
	int			m_filenum;			// �ļ���
	DWORD		m_realflen;			// ʵ���ļ�����
// 	UINT		m_type;				// ����(0: ���ݣ�1: ������2:Ԥ��������ֵ����)
// 	CString		m_CidWType;			// ����ID(type==1)��Ԥ������(type==2)��մ�(type==0)
// 	CString		m_pro;				// Э��
	CString		m_indexfname;		// �����ļ���
	CString		m_realfname;		// ʵ���ļ���
	FILE*		m_prealfile;		// �����ļ�ָ��
	FILE*		m_pindexfile;		// ʵ���ļ�ָ��
}IpDataStruct,*LPIpDataStruct;

inline void AFXAPI DestructElements(IpDataStruct* ppElements, int nCount)
{
	if (ppElements->m_pindexfile != NULL)
	{
		fclose(ppElements->m_pindexfile);	// �ر����ϵĶ����ļ�
	}
	if (ppElements->m_prealfile != NULL)
	{
		fclose(ppElements->m_prealfile);	// �ر����ϵ�ʵ���ļ�
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



// Э������ඨ��
class DceProcotol
{
public:
	DceProcotol();
	virtual	~DceProcotol();
public:
// 	int				m_nowstm;		// �ļ�����ĵ�ǰʱ��Ŀ¼(00 - 59)
 	LPCTSTR			m_ddefiletype;	// ����dde�ļ�����������
//	LPCTSTR*		m_outdirlist;	// ���������·���б�Э���б�
	UINT			m_HashSize;		// Socket��ϣ��Ŀ��
	DWORD			m_PacketFrom;	// ��ǰ�����������
	LPIpDataStruct	m_pDseFileInfo;	// DSE����ļ���Ϣ�ṹ����ָ��
	int				m_DseInfoCount;	// DSE����ļ���Ϣ�ṹ��(3�ı���)
	CString			m_IndexFileData;// �����ļ�������(���������)
	CStringArray	m_TmpFileArr;	// ��ʱû���ύ�ɹ����ļ���
public:
	virtual BOOL InitObject(DceMission* pMiss);
	virtual BOOL ProcessPacket(DATEHEADER& dataheader,const char *pPacket, LPCTSTR spyaddr)
	{
		return TRUE;
	}
	virtual void ProcessBuffer(DATABUFFER& packetbuf, time_t& packtm);
	virtual void RenameTmpDir();
	virtual BOOL WriteObjectState(time_t packtm, CString& strtxt){return FALSE;}

	// ����ļ�(���ϡ���������)
	void OutPutFile(LPCTSTR pro,			// Э��
					UINT type,				// ����(0: ���ݣ�1: ������2:Ԥ��������ֵ����)
					LPCTSTR CIDWType,		// ����ID(type==1)��Ԥ������(type==2)��մ�(type==0)
					CString indexfname,
					CString realfname,
					CStringArray& attfiles,
					LPCTSTR filekey,		// ���ݷ���ʱ��KEY(��ʱ��KEY������pro��CIDWType)
					CString AlarmText= ""		// �����ݵĸ澯�ı�
					);

	// ����ļ�����()
	void OutPutFileToStream(LPCTSTR pro,	// Э��
					UINT type,				// ����(0: ���ݣ�1: ������2:Ԥ��������ֵ����)
					LPCTSTR CIDWType,		// ����ID(type==1)��Ԥ������(type==2)��մ�(type==0)
					CString indexfname,
					CString realfname,
					CStringArray& attfiles,
					LPCTSTR filekey,		// ���ݷ���ʱ��KEY(��ʱ��KEY������pro��CIDWType)
//					CString AlarmText= "",	// �����ݵĸ澯�ı�
					UINT	id = 1			// ����������Ĵ���(һ������ƥ���˶������)
					);

	// ����ļ���������Ŀ¼
	void CopyFileToServer(	LPCTSTR indexfname,			// �����ļ�
							LPCTSTR realfname,			// ʵ���ļ�
							CStringArray& attfile,		// �����ļ�
							CString DstDir,				// Ŀ��·��
							BOOL isDelSrcFile			// �Ƿ�ɾ��Դ�ļ�
							);

	// ��ȡ���ݵ����·��
	CString GetOutDir(LPCTSTR pro,			// Э��
					UINT type,				// ����(0: ���ݣ�1: ������2:Ԥ��������ֵ����)
					LPCTSTR CIDWType,		// ����ID(type==1)��Ԥ������(type==2)��մ�(type==0)
					UINT hashval,			// ���ݷ��������
					int* pSvrIdx = NULL		// ������ķ�����ID(����֤Э������)
						);

	void OutPutAlarm(CString DstDir,		// Ŀ��·��
					CString AlarmText		// �澯�ı�
					);

	LPIpDataStruct AllocFileToDse(UINT DataType,	// ��������(0����, 1���� 2Ԥ��)
					UINT KeyHash					// ���ݹ�ϣ
					);

// 	void RenameTmpSubDir(LPCTSTR Ip,		// Dse������IP
// 					LPCTSTR strpro,			// Э����
// 					LPCTSTR SvrType			// ����������
// 					);

	// �ļ�����ʧ�ܾͷ���FALSE
	BOOL MergeFile(LPIpDataStruct pDstFile,	// Ŀ���ļ���Ϣ�ṹ
					LPCTSTR indexfname,		// �����ļ�
					LPCTSTR realfname		// ʵ���ļ�
					);

	// �����ļ�����
	BOOL MergeFileToStream(LPIpDataStruct pDstFile,	// Ŀ���ļ���Ϣ�ṹ
					DceFileStream* pDceStream,
					LPCTSTR srcfname,		// ԭ�ļ�
					LPCTSTR	strnum				// �ļ������滻��
					);

	// �����ڴ����ݵ���
	BOOL MergeMemDataToStream(LPIpDataStruct pDstFile,	// Ŀ���ļ���Ϣ�ṹ
					DceFileStream* pDceStream,
					LPCTSTR	pfdata,						// �ڴ�����(srcfname=NULLʱ��Ч)
					int		fdatalen,					// �ڴ����ݳ���
					LPCTSTR dstfname					// д��������ļ���
					);

// 	void OutPutFileToStream(LPCTSTR pro,	// Э��
// 					UINT type,				// ����(0: ���ݣ�1: ������2:Ԥ��������ֵ����)
// 					LPCTSTR CIDWType,		// ����ID(type==1)��Ԥ������(type==2)��մ�(type==0)
// 					CString indexfname,
// 					CString realfname,
// 					CStringArray& attfiles,
// 					LPCTSTR filekey,		// ���ݷ���ʱ��KEY(��ʱ��KEY������pro��CIDWType)
// 					CString AlarmText= "",	// �����ݵĸ澯�ı�
// 					UINT	id = 1			// ����������Ĵ���(һ������ƥ���˶������)
// 					);
};
#endif // !defined(AFX_PROMISSION_H__C7EC12BC_F0B7_45DB_BDCF_2DE5239604C6__INCLUDED_)
