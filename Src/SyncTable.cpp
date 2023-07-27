// SyncPara.cpp: implementation of the CSyncPara class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SyncTable.h"
#include "ZwString.h"

#include "HttpLib.h"
#ifdef _DEBUG
#pragma message("����HttpLib���԰�")
#pragma comment(lib, "httplibd.lib")
#else
#pragma message("����HttpLib���а�")
#pragma comment(lib, "httplib.lib")
#endif

#define		ASMANAGE_PORT		9000
#define		DEFAULT_TABLE_VER	"19000101010100"
#define		XOR_STRING			"820f6a300a8e8627fd78cfdecde5cbb4"	// MD5("ɽ����������");
#define		XOR_LEN				32									// sizeof(XOR_STRING);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
CSyncPara::CSyncPara()
{
	m_pBuf = NULL;
	m_lenBuf = 0;
}

CSyncPara::~CSyncPara()
{
	if(m_pBuf)
	{
		delete []m_pBuf;
	}
}


BOOL CSyncPara::SavePara(LPCTSTR szBcpPath, CTime tmModify, BOOL bCrypt)
{
	BOOL bSave = FALSE;
	if(m_pBuf)
	{
		CString strTbl = szBcpPath;
		FILE *fpTbl = fopen(strTbl, "wb");
		if(fpTbl)
		{
			if(bCrypt)
			{
				for (int ixor = 0; ixor < m_lenBuf ; ixor++)
				{
					m_pBuf[ixor] = m_pBuf[ixor]^(XOR_STRING[ixor%XOR_LEN]);
				}
			}
			fwrite(m_pBuf, 1, m_lenBuf, fpTbl);
			fclose(fpTbl);

			// ��¼�汾, ���޸�ʱ��
			if(tmModify != 0)
			{
				CFileStatus fstat;
				if(CFile::GetStatus(strTbl, fstat))
				{
					fstat.m_mtime = tmModify;
					CFile::SetStatus(strTbl, fstat);
				}
			}
			bSave = TRUE;
			delete []m_pBuf;
			m_lenBuf = 0;
			m_pBuf = NULL;
		}
	}
	return bSave;
}

BOOL CSyncPara::UpdateFromAS(CStringArray &arrCServer, LPCSTR szTable, CTime &tmVer, CString strSQL, BOOL bForce)
{
// funName=GetSyncTableInfo
// PWD=TABLEQUERY      	//���ƣ�һ��Լ���õ��ַ���
// TABLE=    	//����
// VERSION=	//�汾�ţ���ʽ��20081117112300��
// OPTYPE=query	  //��������
// STRSQL=   	//SQL���
// REMARK=   	//�����ֶ�

// 	��ǰ�汾�� \3 Record1 \1 Record2.....
// 	Record=�ֶ�1 \2 �ֶ�2 ...
	int error=0;
	try
	{
		CString strRst, strFunc;
		CString strTblVer;
		if(bForce)
			strTblVer = DEFAULT_TABLE_VER;
		else
			strTblVer = tmVer.Format("%Y%m%d%H%M%S");
		strFunc.Format(
			"funName=GetSyncTableInfo\r\n"
			"PWD=TABLEQUERY\r\n"
			"TABLE=%s\r\nVERSTION=%s\r\n"
			"OPTYPE=query\r\nSTRSQL=%s\r\nREMARK=\r\n",
			szTable, strTblVer, strSQL);
		BOOL bHaveNew = FALSE;
		CStringArray arrVer;
		CStringArray arrRec;
		CStringArray arrField;
error=1;
		DWORD lenHave = 0;		// ��ʹ��
		DWORD lenBuf = 0;		// �ѷ���
		char  *pBuf = NULL;		// �µò�������
		for (int ics = 0; ics < arrCServer.GetSize() ; ics++)
		{
			strRst = CommGetData(
				arrCServer[ics], 
				ASMANAGE_PORT, 
				"/ZW_TABLEQUERY/", 
				strFunc, 
				strFunc.GetLength());

			arrVer.RemoveAll();
			StrSplit(arrVer, strRst, '\3');
			if(arrVer.GetSize() == 2 && arrVer[0].GetLength() == 14)	// Ver=YYYYmmddHHMMSS
			{
				if(strTblVer.Compare(arrVer[0]) < 0 || bForce)
				{
					// �汾�»�ǿ�Ƹ���
					bForce = FALSE;		// �õ�һ��, �������Ҫ�ȴ˱��²Ÿ���
					if(pBuf)
						delete []pBuf;

					lenHave = 0;
					lenBuf = arrVer[1].GetLength()*2;
					pBuf = new char[lenBuf];
					memset(pBuf, 0, lenBuf);		// ��û�����

					arrRec.RemoveAll();
					StrSplit(arrRec, arrVer[1], '\1');
					for (int irec = 0; irec < arrRec.GetSize() ; irec++)
					{
						arrField.RemoveAll();
						StrSplit(arrField, arrRec[irec], '\2');

						lenHave += sprintf(pBuf+lenHave, "%s", arrField[0]);
						for (int ifield = 1; ifield < arrField.GetSize() ; ifield++)
						{
							lenHave += sprintf(pBuf+lenHave, "\t%s", arrField[ifield]);
						}
						lenHave += sprintf(pBuf+lenHave, "\r\n");
					}
					lenBuf = lenHave;
					strTblVer = arrVer[0];
					tmVer = CTime(
						atoi(strTblVer.Left(4)), 
						atoi(strTblVer.Mid(4, 2)), 
						atoi(strTblVer.Mid(6, 2)),
						atoi(strTblVer.Mid(8, 2)), 
						atoi(strTblVer.Mid(10, 2)), 
						atoi(strTblVer.Mid(12, 2)));

					bHaveNew = TRUE;
					printf("%s �����²���%s, �汾%s\n", arrCServer[ics], szTable, strTblVer);
				}
			}
		}
error=3;
		if(bHaveNew)
		{
			if(m_pBuf)
				delete []m_pBuf;
			m_lenBuf = lenBuf;
			m_pBuf = pBuf;
		}
		
		return bHaveNew;
	}
	catch (...)
	{
			WriteLog("dce", "UpdateFromAS error =%d!",error);	
	}
	return true;
}

BOOL CSyncPara::UpdatePara( LPCTSTR szPath, LPCTSTR szName, CStringArray &arrCS, DWORD synMethod, BOOL bForce, LPCTSTR szSQL)
{
int error=0;
	try
	{
		CString strRst, strFunc;
		CString strTbl;
		BOOL bUseRar = FALSE;
		//strstr �����ַ���str�е�һ�γ����Ӵ�substr�ĵ�ַ�����û�м������Ӵ�������null 
		if(strstr(szName, ".rar") != NULL)
		{
			bUseRar = TRUE;
			strTbl.Format("%s\\%s", szPath, szName);
		}
		else
		{
			bUseRar = FALSE;
			strTbl.Format("%s\\%s.bcp", szPath, szName);
		}
error=1;
		CTime tmLocalBcp = 0;
		CFileStatus fstat;
		if(FALSE == bForce)
		{
			if(CFile::GetStatus(strTbl, fstat))
				tmLocalBcp = fstat.m_mtime;
		}

		BOOL bHaveNew = FALSE;
		if(synMethod == syn_sql)
		{
			// ���󱾵�״̬��, ����Ϊbcp 
			// ֻ֧�ֱ���
			ASSERT(arrCS[0] == "127.0.0.1");

		}
		else if(synMethod == syn_bcp)
		{
			// �����Ƿ����µ�
			CFile fpBcp;
			CTime tmModify;
			CString strFindMax;
			CString strRemote;
			CFileStatus fstat;
			for (int ics = 0; ics < arrCS.GetSize() ; ics++)
			{
				// ����������Ŀ¼, �õ����²������ڷ�����
				if(bUseRar)
				{
					if((arrCS[ics] == "127.0.0.1")
						|| (strcmp(arrCS[ics], v_localip) == 0))
						strRemote.Format("e:\\netspy\\para\\%s", szName);
					else
						strRemote.Format("\\\\%s\\e$\\netspy\\para\\%s", arrCS[ics], szName);
				}
				else
				{
					if((arrCS[ics] == "127.0.0.1")
						|| (strcmp(arrCS[ics], v_localip) == 0))
						strRemote.Format("e:\\netspy\\para\\%s*.bcp", szName);
					else
						strRemote.Format("\\\\%s\\e$\\netspy\\para\\%s*.bcp", arrCS[ics], szName);
				}
error=2;
				CFileFind finder;
				BOOL bWork = finder.FindFile(strRemote);
				while(bWork)
				{
					bWork = finder.FindNextFile();	
					tmModify = 0;
					if(finder.GetLastWriteTime(tmModify))
					{
						if(tmModify > tmLocalBcp)
						{
							tmLocalBcp = tmModify;
							strFindMax = finder.GetFilePath();
							if(fpBcp.m_hFile != CFile::hFileNull)
								fpBcp.Close();
							fpBcp.Open(strFindMax, CFile::modeRead|CFile::shareDenyWrite);	// ռ��
						}
					}
				}
				finder.Close();
			}
error=3;
			// ��ѡ������������Bcp������
			int iTry = 0;
			while(iTry < 5 && fpBcp.m_hFile != CFile::hFileNull)
			{
				if(CopyFile(strFindMax, strTbl, FALSE))
				{
					WriteLog("dce", "Find New Para(%s) %s", (bForce)?"Force":"no Force", strFindMax);
					bHaveNew = TRUE;
					fpBcp.Close();		// �ͷ�
					CFileStatus fstat;
					if(CFile::GetStatus(strFindMax, fstat))
					{
						//fstat.m_mtime = tmLocalBcp;
						CFile::SetStatus(strTbl, fstat);
					}
					break;
				}
				iTry ++;
			}
		}
		else
		{
			ASSERT(synMethod == syn_as);
			// ���bcp��������չ��
/*			if(UpdateFromAS(arrCS, szName, tmLocalBcp, szSQL, bForce))
			{
				bHaveNew = TRUE;
				SavePara(strTbl, tmLocalBcp, TRUE);
			}*/
		}	
		return bHaveNew;
	}
	catch (...)
	{
			WriteLog("dce", "UpdatePara error =%d!",error);	
	}
	return true;
}

BOOL CSyncPara::LoadPara( LPCTSTR szBcpPath, DWORD &seekPos, BOOL bCrypt )
{
	BOOL bRead = FALSE;
	CString strTbl = szBcpPath;
	FILE *fpTbl = fopen(strTbl, "rb");
	DWORD lenFile = 0;
	if(fpTbl)
	{
		lenFile = _filelength(fpTbl->_file);
		if(lenFile > 16)
		{
//			ASSERT(NULL == m_pBuf);
			if(m_pBuf)
			{
				delete []m_pBuf;
				m_pBuf = NULL;
			}
			m_lenBuf = lenFile;
			m_pBuf = new char[m_lenBuf];

			fread(m_pBuf, 1, m_lenBuf, fpTbl);
			if(bCrypt)
			{
				for (int ixor = 0; ixor < m_lenBuf ; ixor++)
				{
					m_pBuf[ixor] = m_pBuf[ixor]^(XOR_STRING[ixor%XOR_LEN]);
				}
			}
			seekPos = 0;	// λ��16�׼�¼
			bRead = TRUE;
		}
		fclose(fpTbl);
	}
	return bRead;
}

BOOL CSyncPara::GetOneRecord( DWORD &seekPos, CStringArray &arrField )
{
	BOOL bGetOK = FALSE;
	if(m_pBuf && seekPos < m_lenBuf)
	{
		CString oneRecLine;
		DWORD skip = 0;
		char *pfind = strstr(m_pBuf+seekPos, "\n");
		if(pfind)
		{
			skip = pfind-m_pBuf-seekPos;
			oneRecLine = CString(m_pBuf+seekPos, skip);
			StrSplit(arrField, oneRecLine, '\t');
			seekPos += skip+1;
		}
		else
		{
			// ASSERT(FALSE);	// ÿ��¼�����\r\n
			skip = m_lenBuf-seekPos;
			oneRecLine = CString(m_pBuf+seekPos, skip);
			StrSplit(arrField, oneRecLine, '\t');
			seekPos = m_lenBuf;
		}
		bGetOK = TRUE;
	}
	return bGetOK;
}

BOOL CSyncPara::GetAllIpOneRecord( DWORD &seekPos, CStringArray &arrField )
{
	BOOL bGetOK = FALSE;

	if(m_pBuf && seekPos < m_lenBuf)
	{
		CString oneRecLine;
		DWORD skip = 0;
		char *pfind = strstr(m_pBuf+seekPos, "\n");
		if(pfind)
		{
			skip = pfind-m_pBuf-seekPos;
			oneRecLine = CString(m_pBuf+seekPos, skip);
			CString begip,endip,iparea;
			CStringArray oneRecArr;
			Splitstr(oneRecLine,oneRecArr,'\t');
			begip = oneRecArr[0];
			endip = oneRecArr[2];
			iparea = oneRecArr[4];
//			begip = oneRecLine.Left(16);
//			endip = oneRecLine.Mid(16,16);
//			iparea = oneRecLine.Mid(32,oneRecLine.GetLength()-32);
			begip.TrimRight();
			endip.TrimRight();
			iparea.TrimRight();
			int num;
			int ipc1, ipc2, ipc3, ipc4; 
			// ����IP��Ԫ����
			num = sscanf(begip,"%d.%d.%d.%d",&ipc1,&ipc2,&ipc3,&ipc4);
			if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
				(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
				(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
			{
				return FALSE;
			}

			begip.Format("%03d.%03d.%03d.%03d",ipc1,ipc2,ipc3,ipc4);

			// ����IP��Ԫ����
			num = sscanf(endip,"%d.%d.%d.%d",&ipc1,&ipc2,&ipc3,&ipc4);
			if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
				(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
				(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
			{
				return FALSE;
			}
			endip.Format("%03d.%03d.%03d.%03d",ipc1,ipc2,ipc3,ipc4);
			arrField.Add(begip);
			arrField.Add(endip);
			arrField.Add(iparea);
			seekPos += skip+1;
		}
		else
		{
			// ASSERT(FALSE);	// ÿ��¼�����\r\n
			skip = m_lenBuf-seekPos;
			oneRecLine = CString(m_pBuf+seekPos, skip);
			CString begip,endip,iparea;
			begip = oneRecLine.Left(16);
			endip = oneRecLine.Mid(16,16);
			iparea = oneRecLine.Mid(32,oneRecLine.GetLength()-32);
			begip.TrimRight();
			endip.TrimRight();
			iparea.TrimRight();
			int num;
			int ipc1, ipc2, ipc3, ipc4; 
			// ����IP��Ԫ����
			num = sscanf(begip,"%d.%d.%d.%d",&ipc1,&ipc2,&ipc3,&ipc4);
			if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
				(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
				(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
			{
				return FALSE;
			}
			begip.Format("%03d.%03d.%03d.%03d",ipc1,ipc2,ipc3,ipc4);
			// ����IP��Ԫ����
			num = sscanf(endip,"%d.%d.%d.%d",&ipc1,&ipc2,&ipc3,&ipc4);
			if ((num != 4) || (ipc1 < 0) || (ipc1 > 255) ||
				(ipc2 < 0) || (ipc2 > 255) || (ipc3 < 0) ||
				(ipc3 > 255) || (ipc4 < 0) || (ipc4 > 255))
			{
				return FALSE;
			}
			endip.Format("%03d.%03d.%03d.%03d",ipc1,ipc2,ipc3,ipc4);
			arrField.Add(begip);
			arrField.Add(endip);
			arrField.Add(iparea);
			seekPos = m_lenBuf;
		}
		bGetOK = TRUE;
	}
	return bGetOK;
}
