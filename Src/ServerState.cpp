
#include "stdafx.h"
#include "dcePub.h"

#define		MAXJIYITIME		3

// static LPCTSTR PROLIST[] =				// ����Э��
// {
// 	_T("aaa"),					// 1		0x0000 00000001
// 	_T("allnet"),				// 2		0x0000 00000002
// 	_T("ftp"),					// 3		0x0000 00000004
// 	_T("httpdown"),				// 4		0x0000 00000008
// 	_T("httpmailrec"),			// 5		0x0000 00000010
// 	_T("httpmailsend"),			// 6		0x0000 00000020
// 	_T("httpmsn"),				// 7		0x0000 00000040
// 	_T("httpup"),				// 8		0x0000 00000080
// 	_T("httpurl"),				// 9		0x0000 00000100
// 	_T("httpymsg"),				// 10		0x0000 00000200
// 	_T("imsg_oicq"),			// 11		0x0000 00000400
// 	_T("imsg_msn"),				// 12		0x0000 00000800
// 	_T("imsg_ymsg"),			// 13		0x0000 00001000
// 	_T("imsg_icqaol"),			// 14		0x0000 00002000
// 	_T("imsg_lianzhong"),		// 15		0x0000 00004000
// 	_T("imsg_mir"),				// 16		0x0000 00008000
// 	_T("imsg_hf"),				// 17		0x0000 00010000
// 	_T("imsg_bf"),				// 18		0x0000 00020000
// 	_T("imsg_soqq"),			// 19		0x0000 00040000
// 	_T("imsg_eht"),				// 20		0x0000 00080000
// 	_T("imsg_vp"),				// 21		0x0000 00100000
// 	_T("media"),				// 22		0x0000 00200000
// 	_T("invalid"),				// 23		0x0000 00400000
// 	_T("pop3"),					// 24		0x0000 00800000
// 	_T("smtp"),					// 25		0x0000 01000000
// 	_T("telnet"),				// 26		0x0000 02000000
// 	_T("unip"),					// 27		0x0000 04000000
// 	_T("vgate"),				// 28		0x0000 08000000
// 	_T("webmail"),				// 29		0x0000 10000000
// 	_T("oyescam"),				// 30		0x0000 20000000
// 	_T("p2p_emule"),			// 31		0x0000 40000000
// 	_T("rc_vnc"),				// 32		0x0000 80000000
// 	_T("rtsp"),					// 33		0x0001 00000000
// 	_T("ultrabrowse"),			// 34		0x0002 00000000
// 	_T("freegateup"),			// 35		0x0004 00000000
// 	_T("freegatedown"),			// 36		0x0008 00000000
// 	_T("freegateurl"),			// 37		0x0010 00000000
// 	_T("dynapassup"),			// 38		0x0020 00000000
// 	_T("dynapassdown"),			// 39		0x0040 00000000
// 	_T("dynapassurl"),			// 40		0x0080 00000000
// 	_T("firephoenix"),			// 41		0x0100 00000000
// 	_T("")
// };

ServerArray g_DseServer;		// dse����������


int CheckOneDir(LPCTSTR dirpath1, LPCTSTR dirpath2);
ULONGLONG CheckOneServer(LPCTSTR serverip, LPCTSTR servertype, BYTE* state);

ServerArray::ServerArray()
{
	m_pServer = NULL;
	m_count = 0;
	m_totalpower = 0;
	m_AllDsePower = 0;
	memset(m_CurDsePower, 0, sizeof(m_CurDsePower));
	m_pCurDse = m_CurDsePower;
}

ServerArray::~ServerArray()
{
	if (m_pServer)
	{
		delete[] m_pServer;
		m_pServer = NULL;
	}
	if (m_AllDsePower != NULL)
	{
		delete[] m_AllDsePower;
	}
}

// ����һ��DSE
void ServerArray::AddDse(LPCTSTR ip, int power)
{
	if ((ip[0] != 0) && (power>=0))		// ip != ""
	{
		if (0 == (m_count & 31))
		{
			SERVERHOST* tp = new SERVERHOST[m_count+32];
			memset(tp, 0, sizeof(SERVERHOST)*(m_count+32));
			memcpy(tp, m_pServer, sizeof(SERVERHOST)*m_count);
			delete[] m_pServer;
			m_pServer = tp;
		}
		SERVERHOST* pSvr = m_pServer+m_count;
		memcpy(pSvr->m_ip, ip, min(strlen(ip), 16));
		pSvr->m_power = power;
		m_count++;
		m_totalpower += power;
		CString dir;
		dir.Format("%s\\IndexServer\\%s", v_datapath, ip);
		CreateDir(dir);
		dir.Format("%s\\CaseServer\\%s", v_datapath, ip);
		CreateDir(dir);
		dir.Format("%s\\WarnServer\\%s", v_datapath, ip);
		CreateDir(dir);

		// �ύ��ǰ�����������ļ�
		RenameTmpFile(ip);
	}
}

//*************************************************
//	��������:		MoveJiYaFile
//	����:			�ƶ���ѹ�����ݵ�������DSE��
//	�������:		DataType	��������(Case / Index / Warn)
//					nSrcSvr		��ѹ��DSE��
//					nDstSvr		�������ݵ�DSE��
//	�������:		
//	����ֵ:			
//	����:			��dce�����˻�ѹ��־ʱ�ƶ��ļ���������DSE
//*************************************************
void ServerArray::MoveJiYaFile(LPCTSTR DataType, int nSrcSvr, int nDstSvr)
{
	ASSERT(m_pCurDse != NULL);
	if ((m_pCurDse->m_dsepower == m_totalpower)		// ����DSE�����
		|| (m_pCurDse->m_dsepower <= 0))			// ����DSE�������
	{
		// ֻ�����д���DSE������²��ƶ���ѹ����
		return;
	}

	ASSERT(nSrcSvr>=0);
	ASSERT(nSrcSvr<m_count);
	ASSERT(nDstSvr>=0);
	ASSERT(nSrcSvr<m_count);
	HANDLE hfindfile;
	WIN32_FIND_DATA finddata;
	char path[512];
	char srcralfile[512];
	char dstralfile[512];
	char srcidxfile[512];
	char dstidxfile[512];
	char fname[512];
	// CaseServer
	int len = sprintf(path, "%s\\%s\\%s\\20*.idx", v_datapath, DataType, m_pServer[nSrcSvr].m_ip);
	hfindfile = FindFirstFile(path, &finddata);
	if (hfindfile != INVALID_HANDLE_VALUE)
	{
		len -= 7;	// 20*.idx
		path[len] = 0;
		do
		{
			// �ƶ�*.file�ļ�
			int len1 = strlen(finddata.cFileName);
			strcpy(fname, finddata.cFileName);
			strcpy(fname+len1-4, ".file");

			sprintf(srcralfile, "%s%s", path, fname);
			sprintf(dstralfile, "%s\\%s\\%s\\%s",
				v_datapath, DataType, m_pServer[nDstSvr].m_ip, fname);

			if (_access(srcralfile, 0) == 0)
			{
				int re = rename(srcralfile, dstralfile);
				if (re == 0)
				{
					// �ƶ�*.idx�ļ�
					sprintf(srcidxfile, "%s%s", path, finddata.cFileName);
					sprintf(dstidxfile, "%s\\%s\\%s\\%s",
						v_datapath, DataType, m_pServer[nDstSvr].m_ip, finddata.cFileName);
					re = rename(srcidxfile, dstidxfile);
					if (re == 0)
					{
						// �ύ�ɹ�
					}
					else
					{
						WriteLog("dce", "�ƶ��ļ�<%s>ʧ��!", srcralfile);
					}
				}
				else
				{
					// �ȵ���һ�����ƶ�
				}
			}
		}while(FindNextFile(hfindfile, &finddata));
		FindClose(hfindfile);
	}

}

// ���DSE�Ļ�ѹ״̬
void ServerArray::CheckServerJiyaState()
{
	if (m_count <= 1)
	{
		return;		// 1��DSE������ѹ״̬
	}
	static int printstate = 0;
	BOOL bAllDseJiya = TRUE;
	BOOL bRefurDse = (m_pCurDse->m_pdsepower == NULL);
	CString WorkStateText;
	SERVERHOST* pTmpServer = m_pServer;
	char dir[MAX_PATH];
	int len;
	for (int i=0; i<m_count; i++, pTmpServer++)
	{
		BYTE st[4];
		memset(st, 0, sizeof(4));
		st[3] = pTmpServer->m_workstate[3];
		// ���ȼ��x:\\netspy\\IndexServerĿ¼
		len = sprintf(dir, "%s\\IndexServer\\%s\\", v_datapath, pTmpServer->m_ip);
		CreateDir(dir);
		strcpy(dir+len, "jiya.flg");
		if (_access(dir, 0) == 0)
		{
			// ��Ŀ¼��ѹ
			st[0] = 100;
			st[3] |= 1;
			MoveJiYaFile("IndexServer", i, (i+m_count-1) % m_count);
		}
		else
		{
			strcpy(dir+len, "ok.flg");
			if (_access(dir, 0) == 0)
			{
				// ������ok.flg�ļ�
				if (++pTmpServer->m_workstate[0] >= MAXJIYITIME)
				{
					strcpy(dir+len, "jiya.flg");
					CreateFlgFile(dir);		// ������ѹ��־�ļ�
					pTmpServer->m_workstate[0] = 0;
					st[3] |= 1;
				}
			}
			else
			{
				st[0] = 0;
				st[3] &= ~1;
				CreateFlgFile(dir);		// ����Ok.flg��־�ļ�
			}
		}

		// ��μ��x:\\netspy\\CaseServerĿ¼
		len = sprintf(dir, "%s\\CaseServer\\%s\\", v_datapath, pTmpServer->m_ip);
		CreateDir(dir);
		strcpy(dir+len, "jiya.flg");
		if (_access(dir, 0) == 0)
		{
			// ��Ŀ¼��ѹ
			st[1] = 100;
			st[3] |= 2;
			MoveJiYaFile("CaseServer", i, (i+m_count-1) % m_count);
		}
		else
		{
			strcpy(dir+len, "ok.flg");
			if (_access(dir, 0) == 0)
			{
				// ������ok.flg�ļ�
				if (++pTmpServer->m_workstate[1] >= MAXJIYITIME)
				{
					strcpy(dir+len, "jiya.flg");
					CreateFlgFile(dir);		// ������ѹ��־�ļ�
					pTmpServer->m_workstate[1] = 0;
					st[3] |= 2;
				}
			}
			else
			{
				st[1] = 0;
				st[3] &= ~2;
				CreateFlgFile(dir);		// ����Ok.flg��־�ļ�
			}
		}

		// �����x:\\netspy\\WarnServerĿ¼
		len = sprintf(dir, "%s\\WarnServer\\%s\\", v_datapath, pTmpServer->m_ip);
		CreateDir(dir);
		strcpy(dir+len, "jiya.flg");
		if (_access(dir, 0) == 0)
		{
			// ��Ŀ¼��ѹ
			st[2] = 100;
			st[3] |= 4;
			MoveJiYaFile("WarnServer", i, (i+m_count-1) % m_count);
		}
		else
		{
			strcpy(dir+len, "ok.flg");
			if (_access(dir, 0) == 0)
			{
				// ������ok.flg�ļ�
				if (++pTmpServer->m_workstate[2] >= MAXJIYITIME)
				{
					strcpy(dir+len, "jiya.flg");
					CreateFlgFile(dir);		// ������ѹ��־�ļ�
					pTmpServer->m_workstate[2] = 0;
					st[3] |= 4;
				}
			}
			else
			{
				st[2] = 0;
				st[3] &= ~4;
				CreateFlgFile(dir);		// ����Ok.flg��־�ļ�
			}
		}
		if (st[3] == 0)
		{
			bAllDseJiya = FALSE;
		}
		if (st[3] != pTmpServer->m_workstate[3])
		{
			if (st[3] == 0)
			{
				// �����ѹ
				WorkStateText.Format("%s\r\nDSE<%s>�����ѹ!",
					(CString)WorkStateText, pTmpServer->m_ip);
			}
			else
			{
				// ��û�л�ѹ����ѹ
				WorkStateText.Format("%s\r\nDSE<%s>��ѹ! ��ѹ״̬=%08X",
					(CString)WorkStateText, pTmpServer->m_ip, *((DWORD*)st));
			}
			bRefurDse = TRUE;
			pTmpServer->m_workstate[3] = st[3];
		}
		else if ((pTmpServer->m_workstate[3] != 0) && (0 == (printstate % 60)))
		{
			WorkStateText.Format("%s\r\nDSE<%s>��ѹ! ��ѹ״̬=%08X",
				(CString)WorkStateText, pTmpServer->m_ip, *((DWORD*)st));
		}
	}
	printstate++;

	if (bRefurDse)
	{
		// ������֯DSE��������
		RefurDsePower(bAllDseJiya);
	}

	if (!WorkStateText.IsEmpty())
	{
		// д��ѹ״̬��־
		WriteLog("dce", WorkStateText);
	}
	return;
}

void ServerArray::RefurDsePower(BOOL bAllDseJiya)
{
	// ���(bAllDseJiya==TRUE)
	// ��ô�������е�DSE����ѹ��
	ASSERT(m_count > 0);
	ASSERT(m_totalpower > 0);
	if (m_AllDsePower == NULL)
	{
		m_AllDsePower = new int[m_totalpower];
		int* pPower = m_AllDsePower;
		for (int i=0; i<m_count; i++)
		{
			for (int j=0; j<m_pServer[i].m_power; j++, pPower++)
			{
				*pPower = i;
			}
		}
	}

	DSEPOWER* pTmpCurDse;
	if (m_pCurDse == &m_CurDsePower[0])
		pTmpCurDse = &m_CurDsePower[1];
	else
		pTmpCurDse = &m_CurDsePower[0];

	if (pTmpCurDse->m_pdsepower == NULL)
	{
		pTmpCurDse->m_pdsepower = new int[m_totalpower];
	}

	int* pPower = pTmpCurDse->m_pdsepower;
	memset(pPower, 0, m_totalpower*4);
	pTmpCurDse->m_dsepower = 0;
	if (!bAllDseJiya)
	{
		for (int i=0; i<m_count; i++)
		{
			if (m_pServer[i].m_workstate[3] == 0)
			{
				for (int j=0; j<m_pServer[i].m_power; j++, pPower++)
				{
					*pPower = i;
					pTmpCurDse->m_dsepower++;
				}
			}
		}
	}

	InterlockedExchange((long*)&m_pCurDse, (long)pTmpCurDse);
}

LPCTSTR ServerArray::AllocateData(UINT hash, int* psvridx/* = NULL*/)
{
	ASSERT(m_pServer != NULL);
	ASSERT(m_count>0);
	if (m_count == 1)
	{
		if (psvridx != NULL)
		{
			*psvridx = 0;
		}
		return m_pServer->m_ip;
	}

	ASSERT(m_AllDsePower != NULL);
	int idx = m_AllDsePower[hash % m_totalpower];
	ASSERT(idx <= m_count);
	BOOL bFindDse = FALSE;
	if ((0 != m_pServer[idx].m_workstate[3])
		&& (m_pCurDse->m_dsepower != 0))
	{
		DSEPOWER* pTmpCurDse = m_pCurDse;
		idx = pTmpCurDse->m_pdsepower[hash % pTmpCurDse->m_dsepower];
		ASSERT(idx <= m_count);
	}

	if (psvridx != NULL)
		*psvridx = idx;
	return m_pServer[idx].m_ip;
}

int ServerArray::RenameTmpFile(LPCTSTR DseIP)
{
	HANDLE hfindfile;
	WIN32_FIND_DATA finddata;
	char path[512];
	char tmpfile[512];
	char outfile[512];
	// CaseServer
	int len = sprintf(path, "%s\\CaseServer\\%s\\tmp20*.*", v_datapath, DseIP);
	hfindfile = FindFirstFile(path, &finddata);
	if (hfindfile != INVALID_HANDLE_VALUE)
	{
		len -= 8;	// tmp20*.*
		path[len] = 0;
		do
		{
			sprintf(tmpfile, "%s%s", path, finddata.cFileName);
			sprintf(outfile, "%s%s", path, finddata.cFileName+3);
			int re = rename(tmpfile, outfile);
			if (re != 0)
			{
				WriteLog("dce", "�����ļ�<%s>ʧ��!", tmpfile);
			}
		}while(FindNextFile(hfindfile, &finddata));
		FindClose(hfindfile);
	}
	
	// IndexServer
	len = sprintf(path, "%s\\IndexServer\\%s\\tmp20*.*", v_datapath, DseIP);
	hfindfile = FindFirstFile(path, &finddata);
	if (hfindfile != INVALID_HANDLE_VALUE)
	{
		len -= 8;	// tmp20*.*
		path[len] = 0;
		do
		{
			sprintf(tmpfile, "%s%s", path, finddata.cFileName);
			sprintf(outfile, "%s%s", path, finddata.cFileName+3);
			int re = rename(tmpfile, outfile);
			if (re != 0)
			{
				WriteLog("dce", "�����ļ�<%s>ʧ��!", tmpfile);
			}
		}while(FindNextFile(hfindfile, &finddata));
		FindClose(hfindfile);
	}

	// WarnServer
	len = sprintf(path, "%s\\WarnServer\\%s\\tmp20*.*", v_datapath, DseIP);
	hfindfile = FindFirstFile(path, &finddata);
	if (hfindfile != INVALID_HANDLE_VALUE)
	{
		len -= 8;	// tmp20*.*
		path[len] = 0;
		do
		{
			sprintf(tmpfile, "%s%s", path, finddata.cFileName);
			sprintf(outfile, "%s%s", path, finddata.cFileName+3);
			int re = rename(tmpfile, outfile);
			if (re != 0)
			{
				WriteLog("dce", "�����ļ�<%s>ʧ��!", tmpfile);
			}
		}while(FindNextFile(hfindfile, &finddata));
		FindClose(hfindfile);
	}

	return 0;
}

// 
// ULONGLONG CheckOneServer(LPCTSTR serverip, LPCTSTR servertype, BYTE* state)
// {
// 	ASSERT(serverip[0] != 0);
// 	ASSERT(state != NULL);
// 	ULONGLONG jiyastate = 0;
// // 	int i = 0;
// // 	char path1[200];
// // 	char path2[200];
// // 	while(PROLIST[i][0] != 0)
// // 	{
// // 		ASSERT(i < PRONUM);
// // 		state[i]++;
// // 		if ((state[i] & 0x7F) >= MAXJIYIDIR)
// // 		{
// // 			sprintf(path1, "v:\\netspy\\%s\\%s\\%s\\*ok.flg", servertype, serverip, PROLIST[i]);
// // 			sprintf(path2, "%s\\%s\\%s\\%s\\*ok.flg", v_RootPath, servertype, serverip, PROLIST[i]);
// // 			int datnum = CheckOneDir(path1, path2);
// // 			if (datnum >= MAXJIYIDIR)
// // 			{
// // 				// ��ѹ
// // 				datnum = MAXJIYIDIR - 3;	// 3���Ӻ��ټ��һ��
// // 				jiyastate |= (1 << i);
// // 				state[i] = 0x80 | datnum;
// // 			}
// // 		}
// // 		else if (state[i] & 0x80)
// // 		{
// // 			// ��ѹ״̬
// // 			jiyastate |= (1 << i);
// // 		}
// // 		i++;
// // 	}
//  	return jiyastate;
// }