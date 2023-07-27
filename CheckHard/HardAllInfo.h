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

#define CHECK_DEFAULT		0			// δ��״̬, �������в���д��־ʱ���ʱ��
#define CHECK_OK			1			// ����״̬, �������в��� �ػ��з���200OK
#define CHECK_ERR_OPENREG		2		// �쳣, ������ļ�ʧ��
#define CHECK_ERR_HDRHASH		3		// �쳣, ����ļ�ͷHashʧ��
#define CHECK_ERR_VER			4		// �쳣, ����ļ��汾�����汾����
#define CHECK_ERR_REGTYPE		5		// �쳣, ע����ʧ��(��ʽ�浫��ʼʱ�䲻Ϊ0�����ð���ʼʱ��Ϊ0)
#define CHECK_ERR_FILESIZE		6		// �쳣, �ڴ���С��ʵ�ʲ���
#define CHECK_ERR_BLOCK			7		// �쳣, ����������(�����)
#define CHECK_ERR_GETREGCARD	8		// �쳣, �޷�����κ�Mac
#define CHECK_ERR_GETREGDISK	9		// �쳣, �޷�����κ�Ӳ��
#define CHECK_ERR_GETSN			10		// �쳣, �������Ϣ��������ʧ��
#define CHECK_ERR_HARDID		11		// �쳣, ����ļ�δ�ҵ����������
#define CHECK_ERR_FTRYNOW		12		// �쳣, ���ð汾��ʱ�䲻����ɷ�Χ��
#define CHECK_ERR_FLOCAL		13		// �쳣, �ɱ�����־�ļ�������ʱ��ʧ��
#define CHECK_ERR_FNTP			14		// �쳣, ��NTPʱ�������������ʱ��ʧ��
#define CHECK_ERR_F200OK		15		// �쳣, ��200OK������ʱ���쳣(��10��������)
#define CHECK_ERR_FREGNOW		16		// �쳣, ��ʽ��������ʱ��������������ֹʱ��
#define CHECK_ERR_NOVER		17		// �쳣, �޸��Ӱ汾��Ϣ
#define CHECK_ERR_NOCUS		18		// �쳣, �޸��ӿͻ���Ϣ

//////////////////////////////////////////////////////////////////////////
extern CString	g_strRegDat;		// �����Ϣ�ļ�
extern DWORD	g_checkTmTID;		// ���ʱ���߳�ID
extern HANDLE	g_checkHandle;		// ���ʱ���߳̾��
extern time_t	*g_pcheckRegBegin;	// ��ʼ(==0��ʽ��)
extern time_t	*g_pcheckRegEnd;	// ����
extern DWORD	*g_pCheckState;		// ״̬, ֵΪCHECK_ϵ�к�

//////////////////////////////////////////////////////////////////////////
DWORD WINAPI CheckTmThread(LPVOID p);	// �̺߳���
BOOL WriteCheckError(DWORD checkErr);	// д����״̬

//////////////////////////////////////////////////////////////////////////
// ����û��б�
BOOL	GetUserInfo(CString &Info);
// �����ʵMac�б�
int		GetRegeditKey(CString &arrInfo);
// ������,������
bool	InstallAntiAttach();
// ��������Ϣbase64��
CString Res_QueryHardInfo(LPCTSTR *pFields, LPCTSTR *pValues, int count);
// �������Ϣ��������
bool GetExeAttInfo(CStringArray &arrCus, CString& strVer);
BOOL	GetSN(CStringArray &arrAtt, const char *systemstring, const char *baseString, 
		   unsigned char snReg[16], unsigned char snTry[16]);

#endif // !defined(AFX_HARDALLINFO_H__0566EEE0_0213_4C7B_B33E_2C985980CF20__INCLUDED_)
