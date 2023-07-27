
#if !defined(AFX_DOG_H__3BF25A20_ED78_4A84_A698_3E3F1CAFEFE3__INCLUDED_)
#define AFX_DOG_H__3BF25A20_ED78_4A84_A698_3E3F1CAFEFE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//****************************************************************
// ϵͳ�ƻ�����
//****************************************************************
// ����ʹ���������Ĵ���(Win2K)
void Goto_Ring0_1();
void Goto_Ring0_2();

//****************************************************************
// ����ʹ���������Ĵ���(Win2K)
void ResetComputer();

//****************************************************************
// �����Ժ���
//****************************************************************
// ����Ƿ��е�����
BOOL CheckDebug();

//****************************************************************
// 
//****************************************************************
extern DWORD	tea_key[4];	// TEA�����㷨��KEY 16Byte

//������:	MD5Tran
//��  ��:	��һ������MD5�任
//��  ��:	pmd5	����MD5ֵ�Ļ�����ָ�뻺�������Ⱥ㶨Ϊ16���ֽ�
//			pinstr	��Ҫ�任���ַ�����ָ��
//			len		��Ҫ�任���ַ����ĳ���(��λ���ֽ�)
//����ֵ:	
void MD5Tran(void* pmd5, void* pinstr, int len);

//������:	MD5Tran
//��  ��:	��һ����ֵ��MD5�任
//��  ��:	pmd5	����MD5ֵ�Ļ�����ָ�뻺�������Ⱥ㶨Ϊ16���ֽ�
//			inNum	��Ҫ�任����ֵ
//����ֵ:	
void MD5Tran(void* pmd5, DWORD inNum);


//************************������־�ຯ������***************************
//������:	WriteAppLogEx
//��  ��:	����д������־
//��  ��:	logfile	��־�ļ����ļ���(����ȫ·��)
//			logstr	Ҫд�����־�Ĵ�(����)
//			strlen	Ҫд��Ĵ��ĳ���(Byte)
//			maxsize ��־�ļ����������(Byte)ȱʡ��5M
//����ֵ:	>0		д����ֽ���(��־�ļ�д�ɹ�)
//			==0		�ļ������������
//			<0		�����(��־�ļ�дʧ��)

int WriteAppLogEx(LPCTSTR logfilename, LPCTSTR logstr, int strlen, DWORD maxsize=5*1024*1024);


//������:	DecryptLogFile
//��  ��:	���ܳ������־�ļ�
//��  ��:	scrlogfile	�����ܵ���־�ļ����ļ���(����ȫ·��)
//			dstlogfile	���ܺ����־�ļ����ļ���(����ȫ·��)
//			
//����ֵ:	���ܺ����־�ļ����ļ���С
int DecryptLogFile(LPCTSTR scrlogfile, LPCTSTR dstlogfile);

//************************��Ʒע���ຯ������***************************
BOOL CheckSingleDog(LPCTSTR szBuildDate, LPCTSTR szStructDat = NULL);		// ������Ψһ��
CString GetSingleID();		// ��û���Ψһֵ
BOOL CheckRegState(DWORD *pCurState = NULL);

//////////////////////////////////////////////////////////////////////////
DWORD		IMWGetPrivateProfileString( 
				LPCTSTR lpAppName, 
				LPCTSTR lpKeyName, 
				LPCTSTR lpDefault, 
				LPTSTR lpReturnedString,
				DWORD nSize, 
				LPCTSTR lpFileName);		//  bCrypt ��ʾҪ��ȡ���ı��Ƿ���Ҫ���ܴ���

UINT		IMWGetPrivateProfileInt(
			  LPCTSTR lpAppName,	// section name
			  LPCTSTR lpKeyName,	// key name
			  INT nDefault,			// return value if key name not found
			  LPCTSTR lpFileName); 

void		IMWFreeBaseINIMap();	// �ͷ������ļ�ռ�õ��ڴ�

#endif // !defined(AFX_DOG_H__3BF25A20_ED78_4A84_A698_3E3F1CAFEFE3__INCLUDED_)
