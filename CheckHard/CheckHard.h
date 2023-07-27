// CheckHard.h: interface for the CCheckHard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKHARD_H__676D4E2C_A71F_41B1_B575_9BBAEBD99EFB__INCLUDED_)
#define AFX_CHECKHARD_H__676D4E2C_A71F_41B1_B575_9BBAEBD99EFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//                  �����벻��ȥ, �޸�StdAfx.h, ���ļ�ͷ���Ӻ�.
// #define   _WIN32_WINNT   0x0500

// ʹ�÷���A
// �ؾ�֮·����, �Ϸ�����������, �������������,������
// #ifndef _DEBUG
// 	CheckHard(NULL, 0);
// #endif

// ʹ�÷���B, ������
// ĳ����ʹ��֮��, ����ĳ������
// Ҳ���Խ�����֮���ȱ����, �����ڳ�����.
// 	char runFlg[MAX_PATH] = FILE_FLG_RUN;
 //	DWORD lenFlg = strlen(runFlg);
// #ifndef _DEBUG
 //	GetRunFlag(runFlg, lenFlg);
// #endif

// �ڱؾ�֮·����, ����ɹ���XOR��
// #ifndef _DEBUG
// 	CheckHard( runFlg, lenFlg);
// #endif

// ʹ��ȫ��֮��, ���δ��ȷ�����������, ������
// 	if(_access(runFlg, 0) != 0))
// 	{
// 		exit(-1);
// 	}

#define DOG_BASEINI	"d:\\netspy\\ini\\local.ini"
#define FLG_BLOCKVM	"e:\\netspy\\flg\\skipvm.flg"

#include "DiskInfo.h"
#include "DogBM.h"
#include "WmiInfo.h"
#include "HardAllInfo.h"
#include "CheckRegTime.h"

// #define		TIMEOUT -1
// #define		_DEBUG_HARDID_		// �����Ҫ����, �򿪴���
// CString GetAllHardInfo();
// #include "Wbemidl.h"
// #pragma comment(lib, "wbemuuid.lib") 

#ifndef _DEBUG
	#pragma message("����imwdoglib.lib")
	#pragma comment(lib, "imwdoglib.lib")
#endif

inline void GetRunFlag(char *buf, int len)
{
	unsigned char pHardHash[] = 
		{"\x49\xd3\x11\x30\xa6\xcd\xe3\x85\x57\x8a\x3e\xec\xc7\x1d\xe7\xf8"};

	int iXorOff = 1;
	for (int i = 0; i< len; i++)
	{
		buf[i] ^= pHardHash[iXorOff-1];
		if(0 == (iXorOff%16))
			iXorOff = 1;
	}
}

#ifdef _DEBUG_HARDID_
CString toMd5String(const unsigned char *Md5Sum)
{
	CString Rst;
	char *pRst = Rst.GetBufferSetLength(40);
	int off = 0;
	for (int i1 = 0; i1 < 16 ; i1++)
	{
		off += sprintf(pRst+off, "%02x", Md5Sum[i1]);
	}
	Rst.ReleaseBuffer(32);
	
	return Rst;
}
#endif

#define		HARDIDVER		6
// md5(md5(ɽ���������缼�����޹�˾)) = 49d31130a6cde385578a3eecc71de7f8 
#define		HEADMD5_HARDID	"\x49\xd3\x11\x30\xa6\xcd\xe3\x85\x57\x8a\x3e\xec\xc7\x1d\xe7\xf8"
//#define		HEADMD5_HARDID	"\x56\xf2\x22\x4a\xb7\xe6\x7a\x8b\x23\x84\x3a\xab\x5e\x1f\xa6\x9f"
//////////////////////////////////////////////////////////////////////////
// ��ȡ�����Ӳ����Ϣ, ��鶪ʧ��
// �����쳣���, �崫�����strInfo, ��ѭ��, ������
// ����, runFlag ��λ��� ����
inline BOOL CheckHard( LPCTSTR szBuildDate, char *runFlag, int len, LPCTSTR szStructDat )
{
	WriteLog("dce","CHECK-HARD -1 \n");
	unsigned int lenHard;
	char *pHardHash = NULL;
	unsigned int offHard = 0;		// δ����Ψһֵ
	int iCheck = CHECK_DEFAULT;
	DWORD cntHard = 0;

	R_RSA_PUBLIC_KEY	RbPubKey2;
	LoadPublicKey(2, &RbPubKey2);

	CString strReg;
	if(szStructDat)
		strReg = szStructDat;
	else
		strReg = SUPPORT_HARDID;
	CString strLocalHard;
	char *pHard = NULL;
	CFile fpIn;
	fpIn.Open(strReg, CFile::modeRead | CFile::modeReadWrite);
	if(fpIn.m_hFile == CFile::hFileNull)
	{
		WriteLog("dce","CHECK-HARD ��Ψһֵ�� ʧ��\n");
#ifdef _DEBUG_HARDID_
		printf("��Ψһֵ�� ʧ��\n");
#endif
		iCheck = CHECK_ERR_OPENREG;
		lenHard = 0;
	}
	else 
	{
		WriteLog("dce","CHECK-HARD -2 \n");
		lenHard = fpIn.GetLength();
		pHard = strLocalHard.GetBufferSetLength(lenHard+2);
		fpIn.ReadHuge(pHard, lenHard);
		fpIn.Close();

		// ����
		PublicDecrypt(NULL, &lenHard, pHard, lenHard, &RbPubKey2);
		pHardHash = new char[lenHard+4];
		PublicDecrypt(pHardHash, &lenHard, pHard, lenHard, &RbPubKey2);

		// 16md5 4ver 4rev 4rev 4cnthard cnthard*64
		// ����ļ�ͷ
		if(memcmp(pHardHash, HEADMD5_HARDID, 16) != 0)
		{
			WriteLog("dce","CHECK-HARD Ψһֵ���ʽ�Ƿ� ʧ��\n");
#ifdef _DEBUG_HARDID_
			printf("Ψһֵ���ʽ�Ƿ� ʧ��\n");
#endif
			iCheck = CHECK_ERR_HDRHASH;
			lenHard = 0;
		}

		WriteLog("dce","CHECK-HARD -3 \n");
		if(lenHard > 0x18)
		{
			// ���汾
			if (HARDIDVER != *((DWORD*)(pHardHash+16)))
			{
				WriteLog("dce","CHECK-HARD �汾��ƥ�� ʧ��\n");
#ifdef _DEBUG_HARDID_
				printf("�汾��ƥ�� ʧ��\n");
#endif
				iCheck = CHECK_ERR_VER;
				lenHard = 0x18;
			}
		}


		if(lenHard >= 0x20)
		{
			// ��������С
			WriteLog("dce","CHECK-HARD -4 \n");
			cntHard = *((DWORD*)(pHardHash+0x1c));
			if(cntHard*64+32 != lenHard)
			{
				WriteLog("dce","CHECK-HARD �����С��ƥ�� ʧ��\n");
#ifdef _DEBUG_HARDID_
				printf("�����С��ƥ�� ʧ��\n");
#endif
				iCheck = CHECK_ERR_FILESIZE;
				lenHard = 0x18;
			}
		}
	}
	
	BOOL bTry = TRUE;
	DWORD offRead = 0x20;
	CString strHardId;
	if(pHardHash && lenHard > 0x18)
	{
		WriteLog("dce","CHECK-HARD -5 \n");
		TCHAR  infoBuf[100];
		DWORD  bufCharCount = 100;
		
		// ������
		strHardId.Format("RegVer:4\r\n");
		if( GetComputerName( infoBuf, &bufCharCount ) )
		{
			strHardId.Format("%sComputer:%s\r\n", CString(strHardId), infoBuf); 
		}
		// �û��б�
		GetUserInfo(strHardId);

		// ���������Ϣ
		GetRegeditKey(strHardId);

		// ȡӲ�����к�
// 		getHardDriveComputerID(strHardId);
		DiskInfo dkInfo = DiskInfo::GetDiskInfo();
		if(dkInfo.LoadDiskInfo())
		{
			CString stronedisk;
			for (int iidisk = 0; iidisk < dkInfo.GetCount() ; iidisk++)
			{
				stronedisk.Format("Disk:%s\r\n", dkInfo.SerialNumber(iidisk));
				strHardId += stronedisk;
			}
		}

		if(_access(FLG_BLOCKVM, 0) != 0)
		{
			if(strHardId.Find("Card:") == -1)
			{
				iCheck = CHECK_ERR_GETREGCARD;
				WriteLog("dce","CHECK-HARD icheck=%d\n",iCheck);
			}
// 			if(strHardId.Find("Disk:") == -1)
// 			{
// 				iCheck = CHECK_ERR_GETREGDISK;
// 			}
		}

		
		WriteLog("dce","CHECK-HARD -6 \n");
		// WMIȡһЩ����
		GetWMIInfo(strHardId);
//#ifdef _DEBUG
//		printf("ԭʼ:\n%s\n\n", strHardId);
		{
			CFile fp;
			fp.Open("v:\\ori.dat", CFile::modeCreate|CFile::modeWrite);
			if(fp.m_hFile != CFile::hFileNull)
			{
				fp.WriteHuge(strHardId, strHardId.GetLength());
				fp.Close();
			}
		}
//#endif
// 		//////////////////////////////////////////////////////////////////////////
		DWORD offRead = 0x20;
		// �����
		if(iCheck == 0)
		{
			if(_access(FLG_BLOCKVM, 0) != 0)
			{
				if(strHardId.Find("VMware") != -1)
					iCheck = CHECK_ERR_BLOCK;
				if(strHardId.Find("VirtualBox") != -1)
					iCheck = CHECK_ERR_BLOCK;
				if(strHardId.Find("Virtual Machine") != -1)
					iCheck = CHECK_ERR_BLOCK;

				WriteLog("dce","CHECK-HARD icheck=%d\n",iCheck);
			}
		}

		if(iCheck == 0)
		{
			// ȡSYSTEM, �����������Ϣ��
			char rcstr[256];
			memset(rcstr, 0, 256);
			IMWGetPrivateProfileString("SYSTEM", "SYSTEM", "**", rcstr, 250, DOG_BASEINI);
			strupr(rcstr);

			WriteLog("dce","CHECK-HARD �õ�ϵͳ���� %s\n",rcstr);
#ifdef _DEBUG_HARDID_
			printf("�õ�ϵͳ���� %s\n", rcstr);
#endif

			CString strVer;
			CStringArray arrCus;
			GetExeAttInfo(arrCus, strVer);
			if(arrCus.GetSize() == 0)
				iCheck = CHECK_ERR_NOCUS;
			if(iCheck == 0 && strVer.IsEmpty())
				iCheck = CHECK_ERR_NOVER;

			WriteLog("dce","CHECK-HARD icheck=%d\n",iCheck);

#ifdef _DEBUG_HARDID_
			int ipstruct = 1;
#endif
			unsigned char RegHash[16];
			unsigned char TryHash[16];
			int icus = 0;
			int ihavereg = cntHard;
			int boffread = offRead;
			CStringArray arrAtt;
			while (icus < arrCus.GetSize() && 0==iCheck && 0==offHard)
			{
				arrAtt.RemoveAll();
				arrAtt.Add(strVer);
				arrAtt.Add(arrCus[icus]);
				
				if(FALSE == GetSN(arrAtt, rcstr, strHardId, RegHash, TryHash))
				{
					iCheck = CHECK_ERR_GETSN;

					WriteLog("dce","CHECK-HARD icheck=%d\n",iCheck);
				}
#ifdef _DEBUG_HARDID_
				printf("Ver %s\nCus %s\nTry %s\nReg %s\n", 
					strVer, arrCus[icus], toMd5String(TryHash), toMd5String(RegHash));
#endif

				cntHard = ihavereg;
				offRead = boffread;
				while(iCheck == 0 && cntHard > 0)
				{
#ifdef _DEBUG_HARDID_
					if(ipstruct)
						printf("No%d %s\n", cntHard, toMd5String((unsigned char*)pHardHash+offRead));
#endif
					if(memcmp(pHardHash+offRead, RegHash, 16) == 0)
					{
						offHard = offRead;
						bTry = FALSE;

						WriteLog("dce","CHECK-HARD ��ʽ��\n");
						// ��ʽ��
						break;
					}

					if(memcmp(pHardHash+offRead, TryHash, 16) == 0)
					{
						offHard = offRead;
						bTry = TRUE;

						WriteLog("dce","CHECK-HARD ���ð�\n");
						// ���ð�
						break;
					}
					offRead += 64;
					cntHard --;
				}
#ifdef _DEBUG_HARDID_
				ipstruct = 0;
#endif
				icus ++;
			}
		}
	}

	if (iCheck != 0)
	{

	}
	else if(offHard == 0)
	{
		WriteLog("dce","CHECK-HARD FAILS δ�ڱ��в鵽�������\n");
#ifdef _DEBUG_HARDID_
		printf("FAILS δ�ڱ��в鵽�������\n");
#endif
		iCheck = CHECK_ERR_HARDID;
	}

	if(iCheck == 0)
	{
		// ��鱾��ʱ��
		time_t tmBegin = *(DWORD*)(pHardHash+offHard+16);
		if( (!bTry && tmBegin != 0)		// ��ʽ��, ��ʼʱ���Ϊ0
		 || (bTry && tmBegin == 0) )	// ���ð�, ��ʼʱ��ش���0
		{
			// �汾���ͱ�ʶ����
			iCheck = CHECK_ERR_REGTYPE;
		}
		if(tmBegin > 0)
		{
			time_t tmEnd = *(DWORD*)(pHardHash+offHard+20);
			// ���ð�, ʱ����Ч
			time_t tmNow = time(0);
			if(tmNow < tmBegin-2*60*60 || tmNow > tmEnd+2*60*60)
			{
				// ���ð�, ���Ѳ�����ɷ�Χ��
				iCheck = CHECK_ERR_FTRYNOW;
			}
			if(iCheck == CHECK_OK)
			{
				// ���ð�, ʱ����Ч
				if(FALSE == CheckLocalAllow(tmBegin, tmEnd))// < tmNow)
				{
					// ���ð�, ���Ѳ�����ɷ�Χ��
					iCheck = CHECK_ERR_FLOCAL;
				}
			}
			WriteLog("dce","CHECK-HARD icheck=%d\n",iCheck);

		}
		else
		{
			time_t tmEnd = *(DWORD*)(pHardHash+offHard+20);
			if(szBuildDate != NULL)
			{
				COleDateTime oleDate;
				if(oleDate.ParseDateTime(szBuildDate))
				{
					SYSTEMTIME sysTime;
					oleDate.GetAsSystemTime(sysTime);
					CTime tmBuild = CTime(sysTime);
					if(tmBuild.GetTime() > tmEnd)
					{
						// ��ʽ��, ������ʱ������������
						iCheck = CHECK_ERR_FREGNOW;
					}
				}
			}
			WriteLog("dce","CHECK-HARD icheck=%d\n",iCheck);
		}
	}

	g_pCheckState = new DWORD;
	*g_pCheckState = CHECK_DEFAULT;			// δ��

	CString str;
	while(iCheck)
	{
		if(*g_pCheckState == CHECK_DEFAULT)
		{
			if(*g_pCheckState >= CHECK_ERR_FTRYNOW)
				WriteCheckError(iCheck);
		}
		*g_pCheckState = iCheck;	// �Ƿ�

		WriteLog("dce","CHECK-HARD icheck=%d\n",iCheck);
		WriteLog("dce","CHECK-HARD // �Ƿ�\n",iCheck);
		str.Format("Sleep(%d000)", iCheck);
		OutputDebugString(str);
		Sleep(5000);
		SwitchToThread();
	}

	if(FALSE == bTry)
	{
		*g_pCheckState = CHECK_OK;		// �Ϸ�(��ʽ��)
	}
	else
	{
		g_checkHandle = CreateThread(
			NULL, 
			0, 
			CheckTmThread,
			NULL, 
			NULL, 
			&g_checkTmTID);
		if(g_checkHandle)
		{
			g_pcheckRegBegin = new time_t;
			*g_pcheckRegBegin = *(time_t*)(pHardHash+offHard+16);
			g_pcheckRegEnd = new time_t;
			*g_pcheckRegEnd = *(time_t*)(pHardHash+offHard+20);
// 			DWORD tmBegin = *(DWORD*)(pHardHash+offHard+16);
// 			DWORD tmEnd = *(DWORD*)(pHardHash+offHard+20);
// 			while(FALSE == PostThreadMessage(g_checkTmTID, THM_SETTIME, (WPARAM)tmBegin, (LPARAM)tmEnd))
// 			{
// 				Sleep(1000);
// 			}
		}
	}
	
	if(runFlag != NULL)
	{
		int iXorOff = 1;		// �̶����ԭ
		for (int iaa = 0; iaa< len; iaa++)
		{
			runFlag[iaa] ^= pHardHash[iXorOff-1];
			if(0 == (iXorOff%16))
				iXorOff = 1;
		}
	}
	
	if(pHardHash)
	{
		delete []pHardHash;
		pHardHash = NULL;
	}

	return lenHard;
}

inline BOOL CheckRunState( DWORD *pCurState = NULL )
{
	try
	{
		BOOL bOk = TRUE;
		if(bOk)
		{
			if(*g_pCheckState > CHECK_OK)
				bOk = FALSE;
		}
		if(pCurState != NULL)
		{
			*pCurState = *g_pCheckState;
		}

		return bOk;
	}
	catch (...)
	{
		WriteLog("dce","CheckRunState error.");
		return FALSE;
	}
}

// �߲�ʱ��
inline BOOL DealCheck200OKDate(DWORD svrIP, const char *pData, int datalen)
{
	static time_t g_tmPrevCheck200OK = 0;
	BOOL bNeedCheck = TRUE;
	if(abs(time(0)-g_tmPrevCheck200OK) < 2*60)
	{
		// 2���Ӽ�һ��
		bNeedCheck = FALSE;
	}
	if(g_pCheckState == NULL || g_checkTmTID == 0)
	{
		// δ��ʼ�����
		bNeedCheck = FALSE;
	}
	else
	{
		if(*g_pCheckState > CHECK_OK)
		{
			// �Ѳ�״̬�쳣
			bNeedCheck = FALSE;
		}
	}

	BOOL bCheckOK = FALSE;
	if(bNeedCheck)
	{
		int offFind = 0;
		int freeLen = datalen;
		int offHdr = g_chk200Search200OK.find(pData, freeLen);
		if(offHdr != -1)
		{
			freeLen -= (offHdr+g_chk200Search200OK.m);
			offFind = offHdr+g_chk200Search200OK.m;
			offHdr = g_chk200SearchDate.find(pData+offFind, freeLen);
			if(offHdr != -1)
			{
				offFind += offHdr;	// ָ��Date: λ��
				freeLen -= offHdr;
				offHdr = g_chk200SearchCRLF.find(pData+offFind, freeLen);
				if(offHdr != -1 && offHdr < 1000)
				{
					// �õ�Date:��
					char *p200OK = new char[offHdr+4];	// �߳���ɾ
					memset(p200OK, 0, offHdr+4);
					memcpy(p200OK, pData+offFind, offHdr+2);

					if(g_checkTmTID)
					{
						bCheckOK = PostThreadMessage(g_checkTmTID, THM_CHECK200OK, (WPARAM)svrIP, (LPARAM)p200OK);
					}
					if(FALSE == bCheckOK)
					{
						delete []p200OK;
					}
				}
			}
		}
	}

	if(FALSE == bCheckOK || bCheckOK)
	{
		// �Ѳ��������һ��200OK
		time(&g_tmPrevCheck200OK);
	}

	return TRUE;
}

#endif // !defined(AFX_CHECKHARD_H__676D4E2C_A71F_41B1_B575_9BBAEBD99EFB__INCLUDED_)
