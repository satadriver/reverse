
#include "stdafx.h"
#include "DcePub.h"
#include "ProMission.h"

#define		MAX_MARGE_NUM		100		// �ļ���������϶�
#define		SPACEMARK			"\r\n\r\nguid=AB7AA4CB_451A_4dc8_A78A_F1D2C788C02D_8CCE92F2_5E9A_439c_A29B_51A9210B1315_194B7FB8_5B07_4bd8_A139_527C062C5AA4\r\n\r\n"



DceProcotol::DceProcotol()
{
	m_ddefiletype	= NULL;				// ����dde�ļ�����������
//	m_outdirlist	= NULL;				// ���������·���б�
	m_HashSize		= 0;				// Socket��ϣ��Ŀ��
	m_PacketFrom	= 0;				// ��ǰ�����������
	m_pDseFileInfo	= NULL;
	m_DseInfoCount	 = 0;
//	m_DseFileMap.InitHashTable(5009);	// �����õĹ�ϣ��
}

DceProcotol::~DceProcotol()
{
//	m_outdirlist = NULL;
	if (m_pDseFileInfo != NULL)
	{
		delete[] m_pDseFileInfo;
	}
}

BOOL DceProcotol::InitObject(DceMission* pMiss) {return TRUE;}

void DceProcotol::ProcessBuffer(DATABUFFER& packetbuf, time_t& packtm) {}

// �����ļ�����
BOOL DceProcotol::MergeFileToStream(LPIpDataStruct pDstFile,	// Ŀ���ļ���Ϣ�ṹ
				DceFileStream* pDceStream,
				LPCTSTR srcfname,		// ԭ�ļ�
				LPCTSTR	strnum			// �ļ������滻��
				)
{
	ASSERT(!pDstFile->m_indexfname.IsEmpty());
	ASSERT(!pDstFile->m_realfname.IsEmpty());
	ASSERT(pDstFile->m_pindexfile != NULL);
	ASSERT(pDstFile->m_prealfile != NULL);
	ASSERT(pDceStream != NULL);
	ASSERT(srcfname != NULL);
	ASSERT(srcfname[0] != 0);

	pDceStream->BeginPOS = ftell(pDstFile->m_prealfile);
	if (pDceStream->BeginPOS != pDstFile->m_realflen)
	{
		WriteLog("dce", "����ļ�<%s>ʱ ��ʵ���ļ����ȴ���%d_%d",
			srcfname, pDceStream->BeginPOS, pDstFile->m_realflen);
		return FALSE;
	}
	CFile file;
	if (file.Open(srcfname, CFile::modeRead))
	{
		// �����ļ�
		pDceStream->FileLen = 0;
		DWORD flen = file.GetLength();
		char buffer[8192];
		while(flen > 0)
		{
			int rlen = min(flen, sizeof(buffer));
			if (rlen != file.Read(buffer, rlen))
			{
				break;
			}
			DWORD wlen = fwrite(buffer, 1, rlen, pDstFile->m_prealfile);
			flen -= rlen;
			pDceStream->FileLen += wlen;
		}
		file.Close();
		if (flen > 0)
		{
			// �ļ���ȡʧ��
			return FALSE;
		}
		pDstFile->m_realflen += pDceStream->FileLen;

		// д�ļ���
		CString tmpname = GetShortName(srcfname);
		if (strnum != NULL)
		{
			tmpname.Replace("_1_", strnum);
		}
		ASSERT(tmpname.GetLength() <= sizeof(pDceStream->Filename));
		memset(pDceStream->Filename, 0, sizeof(pDceStream->Filename));
		memcpy(pDceStream->Filename, tmpname, tmpname.GetLength());
		DWORD idxflen = ftell(pDstFile->m_pindexfile);
		if (idxflen != pDstFile->m_filenum*sizeof(DceFileStream))
		{
			WriteLog("dce", "����ļ�<%s>ʱ �������ļ����ȴ���%d_%d",
				srcfname, idxflen, pDstFile->m_filenum*sizeof(DceFileStream));
			return FALSE;
		}
		pDstFile->m_filenum++;
		fwrite(pDceStream, sizeof(DceFileStream), 1, pDstFile->m_pindexfile);
		return TRUE;
	}
	else
	{
		// д��־
	}
	return FALSE;
}

// �����ڴ����ݵ���
BOOL DceProcotol::MergeMemDataToStream(LPIpDataStruct pDstFile,	// Ŀ���ļ���Ϣ�ṹ
				DceFileStream* pDceStream,
				LPCTSTR	pfdata,						// �ڴ�����(srcfname=NULLʱ��Ч)
				int		fdatalen,					// �ڴ����ݳ���
				LPCTSTR dstfname					// д��������ļ���
				)
{
	ASSERT(!pDstFile->m_indexfname.IsEmpty());
	ASSERT(pDstFile->m_pindexfile != NULL);
	ASSERT(!pDstFile->m_realfname.IsEmpty());
	ASSERT(pDstFile->m_prealfile != NULL);
	ASSERT(pDceStream != NULL);
	pDceStream->BeginPOS = ftell(pDstFile->m_prealfile);
	if (pDceStream->BeginPOS != pDstFile->m_realflen)
	{
		WriteLog("dce", "����ļ�<%s>ʱ ��ʵ���ļ����ȴ���%d_%d",
			pDstFile->m_realfname, pDceStream->BeginPOS, pDstFile->m_realflen);
		return FALSE;
	}
	if (fdatalen > 0)
	{
		ASSERT(pfdata != NULL);
		DWORD wlen = fwrite(pfdata, 1, fdatalen, pDstFile->m_prealfile);
		pDstFile->m_realflen += wlen;
		pDceStream->FileLen = wlen;
		memset(pDceStream->Filename, 0, sizeof(pDceStream->Filename));
		if (dstfname != NULL)
		{
			// д�ļ���
			strcpy(pDceStream->Filename, dstfname);
		}
		DWORD idxflen = ftell(pDstFile->m_pindexfile);
		if (idxflen != pDstFile->m_filenum*sizeof(DceFileStream))
		{
			WriteLog("dce", "����ļ�<%s>ʱ �������ļ����ȴ���%d_%d",
				pDstFile->m_indexfname, idxflen, pDstFile->m_filenum*sizeof(DceFileStream));
			return FALSE;
		}
		pDstFile->m_filenum++;
		fwrite(pDceStream, sizeof(DceFileStream), 1, pDstFile->m_pindexfile);
	}
	return TRUE;
}

LPIpDataStruct DceProcotol::AllocFileToDse(UINT DataType,	// ��������(0����, 1���� 2Ԥ��)
				UINT KeyHash					// ���ݹ�ϣ
				)
{
	static long fnum = 1;
// 	if (DataType > 2)
// 	{
// 		DataType = 1;
// 	}
	int svrid = 0;
	LPCTSTR DseIp = "";
	LPIpDataStruct pVal = NULL;
	if(DataType == 3 || DataType == 4)
	{
		pVal = m_pDseFileInfo;
		if (pVal->m_indexfname.IsEmpty())
		{
			if (g_DseServer.m_count > 1)
			{
				UINT hash = GetTickCount(); //(rand()<<13) + rand();
				DseIp = g_DseServer.AllocateData(hash, &svrid);
			}
			else
			{
				DseIp = g_DseServer.AllocateData(0, &svrid);
			}
		}
	}
	else
	{
		ASSERT(0 == (m_DseInfoCount % 3));
	 	ASSERT(m_pDseFileInfo != NULL);
		ASSERT(DataType<3);		// 0/1/2
		if (m_DseInfoCount != 3)
		{
			ASSERT(m_DseInfoCount == g_DseServer.m_count * 3);
			// ��������
			CString str;
			DseIp = g_DseServer.AllocateData(KeyHash, &svrid);
			pVal = m_pDseFileInfo + (svrid*3) + DataType;
		}
		else
		{
			// �������
			pVal = m_pDseFileInfo + DataType;
			if (pVal->m_indexfname.IsEmpty())
			{
				if (g_DseServer.m_count > 1)
				{
					UINT hash = GetTickCount(); //(rand()<<13) + rand();
					DseIp = g_DseServer.AllocateData(hash, &svrid);
	// 				if ((DataType == 0) && (strcmp(m_ddefiletype, "http") == 0))
	// 				{
	// 					WriteLog("dce", "File Hash = %d, DSeIP = %s", hash, DseIp);
	// 				}
				}
				else
				{
					DseIp = g_DseServer.AllocateData(0, &svrid);
				}
			}
		}
	}
	ASSERT(pVal != NULL);
	if (pVal->m_indexfname.IsEmpty())
	{
		char curtm[40];
		GetTimeString(time(0), "%Y%m%d%H%M%S", curtm, 36);
		long fid = InterlockedIncrement(&fnum);
		CString dir;
		switch(DataType)
		{
		case 0:		// ��������
			dir.Format("%s\\IndexServer\\%s\\", v_datapath, DseIp);
			break;
		case 1:		// ��������
			dir.Format("%s\\CaseServer\\%s\\", v_datapath, DseIp);
			break;
		case 2:		// Ԥ������
			dir.Format("%s\\keyobjServer\\%s\\", v_datapath, DseIp);
			break;
		case 3:		// Ԥ������
			dir.Format("%s\\taobao\\", v_StatPath);
			break;
		case 4:		// Ԥ������
			dir.Format("%s\\qqpass\\", v_StatPath);
			break;
		}
		CreateDir(dir);
		pVal->m_indexfname.Format("%stmp%s_%d_%s_Combo", dir,
			curtm, fid%10000, m_ddefiletype);
		pVal->m_realfname = pVal->m_indexfname + ".file";
		pVal->m_indexfname += ".idx";
		DeleteFile(pVal->m_indexfname);
		DeleteFile(pVal->m_realfname);
		pVal->m_filenum=0;
		pVal->m_realflen=0;
	}
	ASSERT(!pVal->m_realfname.IsEmpty());
	if (pVal->m_prealfile == NULL)
	{
		pVal->m_prealfile = fopen(pVal->m_realfname, "a+b");
	}
	if (pVal->m_pindexfile == NULL)
	{
		pVal->m_pindexfile = fopen(pVal->m_indexfname, "a+b");
	}
	fseek(pVal->m_pindexfile, 0, SEEK_END);
	fseek(pVal->m_prealfile, 0, SEEK_END);
	ASSERT(pVal->m_prealfile != NULL);
	ASSERT(pVal->m_pindexfile != NULL);
	return pVal;
}

void DceProcotol::OutPutFileToStream(LPCTSTR pro,
					UINT type,				// ����(0: ���ݣ�1: ������2:Ԥ��������ֵ����)
					LPCTSTR CIDWType,		// ����ID(type==1)��Ԥ������(type==2)��մ�(type==0)
					CString indexfname,
					CString realfname,
					CStringArray& attfiles,
					LPCTSTR filekey,		// ���ݷ���ʱ��KEY(��ʱ��KEY������pro��CIDWType)
//					CString AlarmText,		// �����ݵĸ澯�ı�
					UINT	id				// ����������Ĵ���(һ������ƥ���˶������)
					)
{
//******************************************************
// �ļ������Ϻͷ������
// 	Smtp��Pop3
// 		���ʼ�ʵ���CRC32��ֵ����
// 	��ʱfilekeyӦ�ô��ʼ�ʵ���CRC32��ֵ
// 
// 	����Э��
// 		�������
//******************************************************
	// ���ȸ���Э��ѡ��ͬ�ķ��䷽ʽ ����ȷ��pval��ֵ��
	UINT hashval = 0;
	BOOL bOnLine;
	LPIpDataStruct pval = NULL;
	if (stricmp(pro, "smtp") == 0)
	{
		bOnLine = ((g_DseServer.m_count <= 30)		// DSE������<=30
			|| ((g_DseServer.m_count <= 100) && (type = 0)));	// ֻ��SMTP���ݵ����ļ����ִ�״̬
		pval = AllocFileToDse(type & 0xF, HashKey(filekey));
	}
	else if (stricmp(pro, "pop3") == 0)
	{
		// ��CRC����
		bOnLine = (g_DseServer.m_count <= 20);			
		if (filekey[0] != 0)
		{
			pval = AllocFileToDse(type & 0xF, HashKey(filekey));
		}
		else
		{
			pval = AllocFileToDse(type & 0xF, rand());	// ��ʵ����������
		}
	}
	else
	{
#ifdef _DEBUG
		if(strstr(pro, "media") != NULL)
		{
			// ������, ��������ǰ��media, ����ֱ�Ӳ�������
			if(_access("e:\\netspy\\flg\\testmedia.flg", 0) == 0 )
			{
				CString oname;
				if(_access(realfname, 0) == 0)
				{
					oname = realfname.Mid(realfname.ReverseFind('\\')+1);
					CopyFile(realfname, CString("d:\\")+oname, FALSE);
				}
			}
		}
#endif

		// �������
		pval = AllocFileToDse(type & 0xF, 0);
		bOnLine = TRUE;
	}

	// �γ����ļ�
	DceFileStream	IdxFileStream;
	memset(&IdxFileStream, 0, sizeof(IdxFileStream));
	IdxFileStream.Type = type;
	strcpy(IdxFileStream.Pro, pro);
	strcpy(IdxFileStream.CaseIDorWarnID, CIDWType);

	CString strnum;
	LPCTSTR pstrnum = NULL;
	if (id > 1)
	{
		strnum.Format("_%d_", id);
		pstrnum = strnum;
		indexfname.Replace("_1_", strnum);
	}

	BOOL MergeOK = TRUE;
	// ����
	MergeOK = MergeMemDataToStream(pval, &IdxFileStream,
		m_IndexFileData, m_IndexFileData.GetLength(), indexfname);
	
	// ʵ��
	if ((MergeOK) && (!realfname.IsEmpty()))
	{
		MergeOK = MergeFileToStream(pval, &IdxFileStream, realfname, pstrnum);
	}

	if (MergeOK)
	{
		// ����
		for (int i=0; i<attfiles.GetSize(); i++)
		{
			CString attfname = attfiles[i];
			if (_access(attfname, 0) == 0)
			{
				if (!MergeFileToStream(pval, &IdxFileStream, attfname, /*pstrnum�����еĸ�����Ҫ�����һֱ�����Բ��ٸ���*/NULL))
				{
					MergeOK = FALSE;
					break;
				}
			}
		}
	}

	// �澯
// 	if ((MergeOK) && (!AlarmText.IsEmpty()))
// 	{
// 		IdxFileStream.Type = 4;		// �澯����
// 		MergeOK = MergeMemDataToStream(pval, &IdxFileStream, AlarmText, AlarmText.GetLength(), NULL);
// 	}

	// �ж����ļ���С, >2MByte ���ύ
	DWORD reallen = ftell(pval->m_prealfile);
	if ((reallen >= 2*1024*1024) || (!bOnLine) || (!MergeOK))
	{
		// �ر����ļ�
		int re = fclose(pval->m_pindexfile);
		if (re != 0)
		{
			WriteLog("dce", "�ر��ļ�<%s>ʧ��! ErrCode = %d", pval->m_indexfname, re);
		}
		pval->m_pindexfile = NULL;
		re = fclose(pval->m_prealfile);
		if (re != 0)
		{
			WriteLog("dce", "�ر��ļ�<%s>ʧ��! ErrCode = %d", pval->m_realfname, re);
		}
		pval->m_prealfile = NULL;
		if ((reallen >= 2*1024*1024) || (!MergeOK))
		{
			// �ύ���ļ�
			CString idxfname = pval->m_indexfname;
			CString ralfname = pval->m_realfname;
			idxfname.Replace("tmp20", "20");
			ralfname.Replace("tmp20", "20");
			if (_access(v_WorkPath+"\\flg\\bakdcedata.flg",0) == 0)
			{
				if (_access("e:\\netspy\\bakdcedata",0)!=0)
				{
					_mkdir("e:\\netspy\\bakdcedata");
				}
				CString tmpfile;
				tmpfile.Format("e:\\netspy\\bakdcedata\\%s", pval->m_realfname.Mid(pval->m_realfname.ReverseFind('\\')));
				CopyFile(pval->m_realfname,tmpfile,TRUE);
				tmpfile.Format("e:\\netspy\\bakdcedata\\%s", pval->m_indexfname.Mid(pval->m_indexfname.ReverseFind('\\')));
				CopyFile(pval->m_indexfname,tmpfile,TRUE);

			}
//			rename(pval->m_realfname, ralfname);
			int re = rename(pval->m_realfname, ralfname);
			if (re != 0)
			{
				::Sleep(200);
				re = rename(pval->m_realfname, ralfname);
				if (re != 0)
				{
					m_TmpFileArr.Add(pval->m_realfname);
				}
			}
			
//			rename(pval->m_indexfname, idxfname);
			re = rename(pval->m_indexfname, idxfname);
			if (re != 0)
			{
				::Sleep(200);
				re = rename(pval->m_indexfname, idxfname);
				if (re != 0)
				{
					m_TmpFileArr.Add(pval->m_indexfname);
				}
			}
// 			DeleteFile(pval->m_realfname);
// 			DeleteFile(pval->m_indexfname);
			pval->m_realfname.Empty();
			pval->m_indexfname.Empty();
			pval->m_filenum=0;
			pval->m_realflen=0;
		}
	}
	else
	{
		if (pval->m_prealfile != NULL)
		{
			fflush(pval->m_prealfile);
		}
		if (pval->m_pindexfile != NULL)
		{
			fflush(pval->m_pindexfile);
		}
	}
}

void DceProcotol::OutPutFile(LPCTSTR pro, UINT type, LPCTSTR CIDWType,
				CString indexfname, CString realfname, CStringArray& attfiles,
				LPCTSTR filekey, CString AlarmText)
{
//******************************************************
// �ļ������Ϻͷ������
// 	Smtp��Pop3(��غ���ȡһ��)
// 		����: �����ϣ��԰���ID+�ʼ�ʵ���CRC32��ֵ����
// 		Ԥ��: �����ϣ���Ԥ������+�ʼ�ʵ���CRC32��ֵ����
// 		����: �����ϣ����ʼ�ʵ���CRC32��ֵ����
// 	��ʱfilekeyӦ�ô��ʼ�ʵ���CRC32��ֵ
// 
// 	WebMail��Media��Unip(��غ���ȡһ��)
// 		����: �����ϣ��������
// 		Ԥ��: �����ϣ��������
// 		����: �����ϣ��������
// 	��ʱfilekey������
// 
// 	��ص�����Э��(ֻ����txt�ļ�)
// 		����: ��filekey+Э��+����ID���ϣ��������
// 		Ԥ��: ��filekey+Э��+Ԥ���������ϣ��������
// 		����: ��filekey+Э�����ϣ��������
// 	��ʱfilekey��ȡֵ��Χ:
// 		filekey == "" ��ʾ������ֻ�ж����ļ�
// 		filekey == "txt" ��ʾ��������Ҫ����
// 		�����ʾ�����ݲ���Ҫ����
// 		
// 	��ȡ������Э��
// 		����: ��filekey+�ͻ���IP+Э��+����ID���ϣ��������
// 		����: ��filekey+�ͻ���IP+Э�����ϣ��������
// 	��ʱfilekey��ȡֵ��Χ:
// 		filekey == "" ��ʾ������ֻ�ж����ļ�
// 		filekey == "txt" ��ʾ��������Ҫ����
// 		�����ʾ�����ݲ���Ҫ����
//******************************************************
		
//******************************************************
//	�澯���ݵ����
//  1. �澯���ݴ����Alarm.txt�ļ���
//	2. һ���澯�ļ�(Alarm.txt)���԰��������澯����
//	3. ÿ���澯�������ڵ��ļ�(Alarm.txt)Ҫ��������ļ���ͬһ��Ŀ¼��
//	3. �����澯�ı�֮�����GUID���ָ�(��SPACEMARK��)
//******************************************************
	ASSERT(FALSE);
	// �����ж�Э��
// 	CString hashkey;
// 	UINT hashval = 0;
// 	BOOL Output = FALSE;
// 	if (  (stricmp(pro, "smtp") == 0)
// 		||(stricmp(pro, "pop3") == 0)
// 		||(strnicmp(pro, "httpmail", 8) == 0))
// 	{
// 		hashkey.Format("%s_%s", CIDWType, filekey);
// 		hashval = HashKey(hashkey);	// ��CRC����
// 		Output = TRUE;
// 	}
// 	else if ( (stricmp(pro, "webmail") == 0)
// 			||(stricmp(pro, "media") == 0)
// 			||(stricmp(pro, "unip") == 0))
// 	{
// 		hashval = rand();	// �������
// 		Output = TRUE;
// 	}
// 	else
// 	{
// 		hashval = rand();	// �������
// 		if ((strcmp(filekey, "") == 0) || (stricmp(filekey, "txt") == 0))
// 		{
// 			hashkey.Format("%s_%s_%s", pro, CIDWType, filekey);
// 		}
// 		else
// 		{
// 			Output = TRUE;
// 		}
// 	}
// 
// 	if (Output)
// 	{
// 		// ����ļ�
// 		CString dstdir = GetOutDir(pro, type, CIDWType, hashval);
// 		if (!dstdir.IsEmpty())
// 		{
// 			CopyFileToServer(indexfname, realfname, attfiles,
// 				dstdir, (type == 0)? TRUE : FALSE);		// �ǻ��ݾ�ɾ��ԭ�ļ�
// 			if (!AlarmText.IsEmpty())
// 			{
// 				OutPutAlarm(dstdir, AlarmText);
// 			}
// 		}		
// 		else
// 		{
// 			// ���������� д��־
// 		}
// 	}
// 	else
// 	{
// 		// �����ļ������ϵ������в����и�����
// 		LPIpDataStruct pval = NULL;
// 		if (!m_DseFileMap.LookupEx(hashkey, pval))
// 		{
// 			CString dstdir = GetOutDir(pro, type, CIDWType, hashval);
// 			if (!dstdir.IsEmpty())
// 			{
// 				IpDataStruct dsefinfo;
// 				dsefinfo.m_num = 1;
// 				dsefinfo.m_pindexfile = NULL;
// 				dsefinfo.m_prealfile = NULL;
// 				dsefinfo.m_indexfname.Format("%s%s", dstdir,
// 					GetShortName(indexfname));
// 				if (strcmp(realfname, "") != 0)
// 				{
// 					dsefinfo.m_realfname.Format("%s%s", dstdir,
// 						GetShortName(realfname));
// 				}
// 				if (!MergeFile(&dsefinfo, indexfname, realfname))
// 				{
// 					CopyFileToServer(indexfname, realfname, attfiles,
// 						dstdir, (type == 0)? TRUE : FALSE);		// �ǻ��ݾ�ɾ��ԭ�ļ�
// 					if (!AlarmText.IsEmpty())
// 					{
// 						OutPutAlarm(dstdir, AlarmText);
// 					}
// 				}
// 				else
// 				{
// 					if (!AlarmText.IsEmpty())
// 					{
// 						OutPutAlarm(dstdir, AlarmText);
// 					}
//  					m_DseFileMap.SetAt(hashkey, dsefinfo);
// 				}
// 			}
// 			else
// 			{
// 				// ���������� д��־
// 			} // end if (!dstdir.IsEmpty())
// 		}
// 		else
// 		{
// 			ASSERT(pval != NULL);
// 			if (!MergeFile(pval, indexfname, realfname))
// 			{
// 				CString dstdir = GetOutDir(pro, type, CIDWType, hashval);
// 				if (!dstdir.IsEmpty())
// 				{
// 					CopyFileToServer(indexfname, realfname, attfiles,
// 						dstdir, (type == 0)? TRUE : FALSE);		// �ǻ��ݾ�ɾ��ԭ�ļ�
// 					if (!AlarmText.IsEmpty())
// 					{
// 						OutPutAlarm(dstdir, AlarmText);
// 					}
// 				}
// 				else
// 				{
// 					// ���������� д��־
// 				} // end if (!dstdir.IsEmpty())
// 			}
// 			else
// 			{
// 				if (!AlarmText.IsEmpty())
// 				{
// 					OutPutAlarm(GetPath(pval->m_indexfname), AlarmText);
// 				}
// 
// 				if (pval->m_num > MAX_MARGE_NUM)
// 				{
// 					// ���ϴﵽ�����
// 					m_DseFileMap.RemoveKey(hashkey);
// 				}
// 			}
// 		}
// 	}// end if (Output)
}


BOOL DceProcotol::MergeFile(LPIpDataStruct pDstFile,	// Ŀ���ļ���Ϣ�ṹ
				LPCTSTR indexfname,		// �����ļ�
				LPCTSTR realfname		// ʵ���ļ�
				)
{
// 	ASSERT(pDstFile != NULL);
// 	BOOL bMergeReal;
// 	if (strcmp(pDstFile->m_realfname, "") == 0)
// 	{
// 		bMergeReal = FALSE;
// 	}
// 	else
// 	{
// 		bMergeReal = TRUE;
// 	}
// 
// 	if (((strcmp(realfname, "") == 0) && bMergeReal)
// 		|| ((strcmp(realfname, "") != 0) && !bMergeReal))
// 	{
// 		// ��ʵ������ݺͲ���ʵ������ݲ������ϵ�һ��
// 		return FALSE;
// 	}
// 
// 	// ��Ŀ�Ķ����ļ�
// 	if (pDstFile->m_pindexfile == NULL)
// 	{
// 		ASSERT(!pDstFile->m_indexfname.IsEmpty());
// 		pDstFile->m_pindexfile = fopen(pDstFile->m_indexfname, "ab");
// 		if (pDstFile->m_pindexfile == NULL)
// 		{
// 			return FALSE;			//  ��Ŀ�Ķ����ļ�ʧ��
// 		}
// 	}
// 
// 	// ��Ŀ��ʵ���ļ�
// 	if (bMergeReal)
// 	{
// 		if (pDstFile->m_prealfile == NULL)
// 		{
// 			pDstFile->m_prealfile = fopen(pDstFile->m_realfname, "ab");
// 			if (pDstFile->m_prealfile == NULL)
// 			{
// 				return FALSE;			//  ��Ŀ��ʵ���ļ�ʧ��
// 			}
// 		}
// 	}
// 
// 	// ��ȡԴ�����ļ�
// 	CFile file;
// 	CString txt1, txt2;
// 	char buf1[4096], buf2[4096];
// 	char* p1 = buf1;
// 	char* p2 = buf2;
// 	int len1 = 0;
// 	int len2 = 0;
// 	if (file.Open(indexfname, CFile::modeRead))
// 	{
// 		len1 = file.GetLength();
// 		if (len1 > (sizeof(buf1) - 32))
// 		{
// 			p1 = txt1.GetBufferSetLength(len1+32);
// 		}
// 		memset(p1, 0, len1+32);
// 		file.Read(p1+24, len1);		// p1+24��Ҫ�ó���Ƭ�κŵ�λ��
// 		file.Close();
// 		// дƬ�κ�
// 		char buf[12];
// 		int tlen = sprintf(buf, "[%d]", pDstFile->m_num);
// 		p1 = p1 + 24 + 3 - tlen;
// 		memcpy(p1, buf, tlen);
// 		len1 = len1 - 3 + tlen;
// 	}
// 	else
// 	{
// 		return FALSE;		// ��Դ�����ļ�ʧ��
// 	}
// 
// 	// ��ȡԴʵ���ļ�
// 	if (bMergeReal)
// 	{
// 		if (file.Open(realfname, CFile::modeRead))
// 		{
// 			len2 = file.GetLength();
// 			if (len2 > (sizeof(buf2) - sizeof(SPACEMARK) - 8))
// 			{
// 				p2 = txt1.GetBufferSetLength(len2 + sizeof(SPACEMARK) + 8);
// 			}
// 			memset(p2, 0, len2 + sizeof(SPACEMARK) + 8);
// 			file.Read(p2 + sizeof(SPACEMARK) - 1, len2);	// �ó�дGUID��λ��
// 			file.Close();
// 		}
// 		else
// 		{
// 			return FALSE;		// ��Դʵ���ļ�ʧ��
// 		}
// 		// дGUID
// 		if (pDstFile->m_num > 1)
// 		{
// 			memcpy(p2, SPACEMARK, sizeof(SPACEMARK)-1);
// 			len2 += sizeof(SPACEMARK)-1;
// 		}
// 		else
// 		{
// 			p2 += sizeof(SPACEMARK)-1;
// 		}
// 	}
// 
// 	// дĿ���ļ�
// 	if (bMergeReal)
// 	{
// 		fwrite(p2, 1, len2, pDstFile->m_prealfile);
// 		fflush(pDstFile->m_prealfile);
// 	}
// 	fwrite(p1, 1, len1, pDstFile->m_pindexfile);
// 	fflush(pDstFile->m_pindexfile);
// 	pDstFile->m_num++;
	return TRUE;
}

// CString DceProcotol::GetOutDir(LPCTSTR pro, UINT type, LPCTSTR CIDWType, UINT hashval, 
// 							   int* pSvrIdx)
// {
// 	CString outdir;
// 	CString str;
// 	// ��������
// 	LPCTSTR DseIp = g_DseServer.AllocateData(hashval, pSvrIdx);
// 	switch(type)
// 	{
// 	case 0:		// ��������
// 		str.Format("IndexServer\\%s\\%s", DseIp, pro);
// 		break;
// 	case 1:		// ��������
// 		str.Format("CaseServer\\%s\\%s", DseIp, pro);
// 		break;
// 	case 2:		// Ԥ������
// 		str.Format("WarnServer\\%s\\%s", DseIp, pro);
// 		break;
// 	default:	// �Ƿ�
// 		return outdir;		// return "";
// 		break;
// 	}
// 
// 	char dir[512];
// 	if (strcmp(pro, "media") != 0)
// 	{
// 		// ���Z��
// 		int zFreeSpace = 0;
// 		if (CheckZDrive())
// 		{
// 			// Z�̿���
// 			int curdirtm = m_nowstm;
// 			for (int i=0; i<60; i++)
// 			{
// 				int len = sprintf(dir, "v:\\netspy\\%s\\%02dwt.flg", str, curdirtm);
// 				len -= 6;
// 				if (_access(dir, 0) == 0)			// ���wt.flg
// 				{
// 					if (type != 0)
// 					{
// 						sprintf(dir + len, "\\%s\\", CIDWType);
// 					}
// 					else
// 					{
// 						strcpy(dir + len, "\\");
// 					}
// 					CreateDir(dir);
// 					return dir;
// 				}
// 				else
// 				{
// 					dir[len] = 0;
// 					CreateDir(dir);
// 					strcpy(dir + len, "ok.flg");
// 					if (_access(dir, 0) != 0)		// ���ok.flg
// 					{
// 						strcpy(dir + len, "wt.flg");
// 						if (CreateFlgFile(dir))			// ����wt.flg
// 						{
// 							// ����wt.flg�ɹ�
// 							if (type != 0)
// 							{
// 								sprintf(dir + len, "\\%s\\", CIDWType);
// 							}
// 							else
// 							{
// 								strcpy(dir + len, "\\");
// 							}
// 							CreateDir(dir);
// 							return dir;
// 						}
// 					}
// 				} // end if (_access(dir, 0) == 0)
// 				curdirtm = (++curdirtm) % 60;
// 			} // end for
// 		} // end if (GetZFreeDisk(zFreeSpace) > 20)
// 	}
// 
// 	// ���D��
// 	int curdirtm = m_nowstm;
// 	for (int j=0; j<60; j++)
// 	{
// 		int len = sprintf(dir, "x:\\xxx\\%s\\%02dwt.flg", str, curdirtm);
// 		len -= 6;
// 		if (_access(dir, 0) == 0)			// ���wt.flg
// 		{
// 			if (type != 0)
// 			{
// 				sprintf(dir + len, "\\%s\\");
// 			}
// 			else
// 			{
// 				strcpy(dir + len, "\\");
// 			}
// 			CreateDir(dir);
// 			return dir;
// 		}
// 		else
// 		{
// 			dir[len] = 0;
// 			CreateDir(dir);
// 			strcpy(dir + len, "ok.flg");
// 			if (_access(dir, 0) != 0)		// ���ok.flg
// 			{
// 				strcpy(dir + len, "wt.flg");
// 				if (CreateFlgFile(dir))			// ����wt.flg
// 				{
// 					// ����wt.flg�ɹ�
// 					if (type != 0)
// 					{
// 						sprintf(dir + len, "\\%s\\", CIDWType);
// 					}
// 					else
// 					{
// 						strcpy(dir + len, "\\");
// 					}
// 					CreateDir(dir);
// 					return dir;
// 				}
// 			}
// 		} // end if (_access(dir, 0) == 0)
// 		curdirtm = (++curdirtm) % 60;
// 	} // end for
// 	return outdir;	// return "";
// }

void DceProcotol::OutPutAlarm(CString DstDir,		// Ŀ��·��
					CString AlarmText				// �澯�ı�
					)
{
	ASSERT(!DstDir.IsEmpty());
	ASSERT(!AlarmText.IsEmpty());
	ASSERT(DstDir[DstDir.GetLength()-1] == '\\');
	CString alarmfile;
	alarmfile = DstDir + "Alarm.txt";
	FILE* fp = fopen(alarmfile, "ab");
	if (fp != NULL)
	{
		if (_filelength(fp->_file) > 0)
		{
			// ����ļ���Ϊ�վ���дGUID
			fwrite(SPACEMARK, sizeof(SPACEMARK)-1, 1, fp);
		}
		fwrite((LPCTSTR)AlarmText, AlarmText.GetLength(), 1, fp);
		fclose(fp);
		fp = NULL;
	}
}

void DceProcotol::CopyFileToServer(LPCTSTR indexfname, LPCTSTR realfname,
			CStringArray& attfiles, CString DstDir, BOOL isDelSrcFile)
{
	static long filetotal = 1;

	int arraycount=attfiles.GetUpperBound();
	CString attfile;
	ASSERT(DstDir[DstDir.GetLength()-1] == '\\');
	CreateDir(DstDir);
	BOOL bReplace = FALSE;
	long filenum;
	char buf[20];
	memset(buf, 0, sizeof(buf));
	// �����ж϶����ļ��Ƿ����
	if((indexfname[0] != 0) && (_access(indexfname,0) == 0))
	{
		CString tmpfile = GetShortName(indexfname);
		CString DstIndexFile = DstDir + tmpfile;
		if (_access(DstIndexFile, 0) == 0)
		{
			// �ļ�����ͻ
			bReplace = TRUE;
			filenum = InterlockedIncrement(&filetotal);
			if (filenum > 999900)
			{
				InterlockedExchange(&filetotal, 1);
			}
			sprintf(buf, "_%d_", filenum);
			tmpfile.Replace("_1_", buf);
			DstIndexFile = DstDir + tmpfile;
		}

		// �ύʵ���ļ�
		if((realfname[0] != 0) && (_access(realfname,0) == 0))
		{
			CString DstFile;
			if (bReplace)
			{
				DstFile = GetShortName(realfname);
				DstFile.Replace("_1_", buf);
				::WritePrivateProfileString("1", "filename", DstFile, indexfname);
				DstFile = DstDir + DstFile;
			}
			else
			{
				DstFile = DstDir + GetShortName(realfname);
			}
			if (isDelSrcFile)
			{
				MoveFileEx(realfname, DstFile,
					MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING |MOVEFILE_WRITE_THROUGH);
			}
			else
			{
				CopyFile(realfname, DstFile, FALSE);
			}
		}

		// �ύ����
		for(int i=0; i<=arraycount; i++)
		{
			attfile=attfiles.GetAt(i);
			if((!attfile.IsEmpty()) && (_access(attfile, 0)==0))
			{
				CString DstFile;
				if (bReplace)
				{
					char attfid[20];
					sprintf(attfid, "attfile%d", i+1);
					DstFile = GetShortName(attfile);
					DstFile.Replace("_1_", buf);
					::WritePrivateProfileString("1", attfid, DstFile, indexfname);
					DstFile = DstDir + DstFile;
				}
				else
				{
					DstFile = DstDir + GetShortName(attfile);
				}
				if (isDelSrcFile)
				{
					MoveFileEx(attfile, DstFile,
						MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING |MOVEFILE_WRITE_THROUGH);
				}
				else
				{
					CopyFile(attfile, DstFile, FALSE);
				}
			}
		}

		// �ύ�����ļ�
		if (isDelSrcFile)
		{
			MoveFileEx(indexfname, DstIndexFile,
				MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING |MOVEFILE_WRITE_THROUGH);
		}
		else
		{
			CopyFile(indexfname, DstIndexFile, FALSE);
		}
	}
}

// void DceProcotol::RenameTmpSubDir(LPCTSTR Ip,		// Dse������IP
// 					LPCTSTR strpro,					// Э����
// 					LPCTSTR SvrType)					// ����������	
// {
// 	char now[20];
// 	sprintf(now, "%02dwt.flg", m_nowstm);	// Ԥ��Ҫ�����Ŀ¼
// 
// 	// ���Z��
// 	if (V_WriteZ)
// 	{
// 		CString tmppath;
// 		tmppath.Format("v:\\netspy\\%s\\%s\\%s\\",SvrType, Ip, strpro);
// 		if(_access(tmppath,0)!=0)
// 		{
// 			CreateDir(tmppath);
// 		}
// 
// 		HANDLE hfindfile;
// 		WIN32_FIND_DATA finddata;
// 		hfindfile = FindFirstFile(tmppath+"*wt.flg", &finddata);
// 		if (hfindfile != INVALID_HANDLE_VALUE)
// 		{
// 			try
// 			{
// 				do
// 				{
// 					if (strcmp(finddata.cFileName, now) != 0)
// 					{
// 						CString wtfname;
// 						wtfname.Format("%s%s", tmppath, finddata.cFileName);
// 						CString okfname = wtfname;
// 						okfname.Replace("wt.flg", "ok.flg");
// 						if (0 != rename(wtfname, okfname))
// 						{
// 							Sleep(200);
// 							rename(wtfname, okfname);
// 						}
// 					}
// 
// 				}while(FindNextFile(hfindfile, &finddata));
// 			}
// 			catch (...)
// 			{
// 				WriteLog("dce", "RenameTmpSubDir(%s) Error!!", strpro);
// 			}
// 			FindClose(hfindfile);
// 		}
// 	}
// 
// 	// ���D��
// 	{
// 		CString tmppath;
// 		tmppath.Format("x:\\xxx\\%s\\%s\\%s\\",SvrType, Ip, strpro);
// 		if(_access(tmppath,0)!=0)
// 		{
// 			CreateDir(tmppath);
// 		}
// 
// 		HANDLE hfindfile;
// 		WIN32_FIND_DATA finddata;
// 		hfindfile = FindFirstFile(tmppath+"*wt.flg", &finddata);
// 		if (hfindfile != INVALID_HANDLE_VALUE)
// 		{
// 			try
// 			{
// 				do
// 				{
// 					if (strcmp(finddata.cFileName, now) != 0)
// 					{
// 						CString wtfname;
// 						wtfname.Format("%s%s", tmppath, finddata.cFileName);
// 						CString okfname = wtfname;
// 						okfname.Replace("wt.flg", "ok.flg");
// 						if (0!=rename(wtfname, okfname))
// 						{
// 							Sleep(200);
// 							rename(wtfname, okfname);
// 						}
// 					}
// 
// 				}while(FindNextFile(hfindfile, &finddata));
// 			}
// 			catch (...)
// 			{
// 				WriteLog("dce", "RenameTmpSubDir(%s) Error!!", strpro);
// 			}
// 			FindClose(hfindfile);
// 		}
// 		return;
// 	}
// }

void DceProcotol::RenameTmpDir()
{
	// �����ύ��ǰ�ύʧ�ܵ��ļ�
	for(int n=0; n<m_TmpFileArr.GetSize(); )
	{
		CString& tmpfile = m_TmpFileArr.ElementAt(n);
		CString outfile = tmpfile;
		outfile.Replace("tmp20", "20");
		int re = rename(tmpfile, outfile);
		if (re != 0)
		{
			if (_access(tmpfile, 0) == 0)
			{
				WriteLog("dce", "�ύ�ļ�<%s>ʧ��! ErrCode = %d", tmpfile, re);
				n++;
			}
			else
			{
				m_TmpFileArr.RemoveAt(n);
			}
		}
		else
		{
			m_TmpFileArr.RemoveAt(n);
		}
	}

	// ����ύ���ļ�
	LPIpDataStruct pval = m_pDseFileInfo;
	for (int i=m_DseInfoCount; i>0; i--, pval++)
	{
		if (!pval->m_indexfname.IsEmpty())
		{
			// �ر����ļ�
			if (pval->m_pindexfile != NULL)
			{
				int re = fclose(pval->m_pindexfile);
				if (re != 0)
				{
					WriteLog("dce", "�ر��ļ�<%s>ʧ��! ErrCode = %d", pval->m_indexfname, re);
				}
				pval->m_pindexfile = NULL;
			}
			if (pval->m_prealfile != NULL)
			{
				int re = fclose(pval->m_prealfile);
				if (re != 0)
				{
					WriteLog("dce", "�ر��ļ�<%s>ʧ��! ErrCode = %d", pval->m_realfname, re);
				}
				pval->m_prealfile = NULL;
			}

			// �ύ���ļ�
			CString idxfname = pval->m_indexfname;
			CString realfname = pval->m_realfname;
			idxfname.Replace("tmp20", "20");
			realfname.Replace("tmp20", "20");
			if (_access(v_WorkPath+"\\flg\\bakdcedata.flg",0) == 0)
			{
				if (_access(v_WorkPath+"\\bakdcedata",0)!=0)
				{
					_mkdir(v_WorkPath+"\\bakdcedata");
				}
				CString tmpfile;
				tmpfile.Format("%s\\bakdcedata\\%s",v_WorkPath, pval->m_realfname.Mid(pval->m_realfname.ReverseFind('\\')));
				CopyFile(pval->m_realfname,tmpfile,TRUE);
				tmpfile.Format("%s\\bakdcedata\\%s",v_WorkPath, pval->m_indexfname.Mid(pval->m_indexfname.ReverseFind('\\')));
				CopyFile(pval->m_indexfname,tmpfile,TRUE);

			}

			int re = rename(pval->m_realfname, realfname);
			if (re != 0)
			{
				::Sleep(200);
				re = rename(pval->m_realfname, realfname);
				if (re != 0)
				{
					m_TmpFileArr.Add(pval->m_realfname);
				}
			}

			re = rename(pval->m_indexfname, idxfname);
			if (re != 0)
			{
				::Sleep(200);
				re = rename(pval->m_indexfname, idxfname);
				if (re != 0)
				{
					m_TmpFileArr.Add(pval->m_indexfname);
				}
			}
		}
		pval->m_realfname.Empty();
		pval->m_indexfname.Empty();
		pval->m_filenum=0;
		pval->m_realflen=0;
	}
}
