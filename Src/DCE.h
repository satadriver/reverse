
#if !defined(AFX_DCE_H__6D1B1909_E86F_4308_BDB8_726BEE03DD5A__INCLUDED_)
#define AFX_DCE_H__6D1B1909_E86F_4308_BDB8_726BEE03DD5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ��������
void ProcessCmdLine(int argc, char **argv);		// ���������в���
int GetLocalHostIP(int& group, int& machine);	// ��ñ���IP
int	ReadDdeHost(int ddegroup, int dcemachine, DDELIST& ddelist);
//int	ReadDceHost(DCELIST& dcelist);
int	ReadDseServerHost();
int ReadParaServerHost();
BOOL UpdatePara();								// ���²���
BOOL Inipubvar();
void ReadHttpurl();

#ifdef		ADD_RUNSTATE_INFO
void DumpRunState(DWORD pro);
#endif

#define GPRSBASICINFO "e:\\netspy\\para\\wcdma.txt"
// �����վ�Ŷ�����Ϣ
typedef struct  BasicInfoStruct		// ����ʱÿ��ip��Ӧ���ļ�������ļ�����
{
	CString     name;				// ��վ��
	CString		address;			// ��վλ��
	CString     longitude;			// ����
	CString     latitude;			// γ��
}BasicInfoStruct,*LPBasicInfoStruct;

typedef CMap<CString, LPCTSTR, BasicInfoStruct, BasicInfoStruct&> Basicinfomap;

extern Basicinfomap m_basicinfomap;

#endif // !defined(AFX_DCE_H__6D1B1909_E86F_4308_BDB8_726BEE03DD5A__INCLUDED_)
